
#define LOG_TAG "RjcoreServiceProduct"
//#define LOG_NDEBUG 0

#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>

#include <string.h>

#include <cutils/atomic.h>
#include <cutils/properties.h> // for property_get

#include <utils/misc.h>

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryBase.h>
#include <utils/Errors.h>  // for status_t
#include <utils/String16.h>
#include <utils/SystemClock.h>
#include <utils/Timers.h>
#include <utils/Vector.h>

#include "RjcoreService.h"

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/statfs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <fcntl.h>
}

#define SUFFIX " CPU, @ 1.6GHz"
#define INFO_PATH "/proc/sn.info"


#define SERIAL_NUMBER_OFF		2	
#define CUST_PRODUCT_MODEL_OFF	32
#define HARDWARE_VERSION_OFF 	48
#define PRODUCT_ID_OFF			58
#define PRODUCT_ID_LEN			4

#define ANDROID_VERSION_PROPERTY 	"ro.build.version.release"
#define SYSTEM_VERSION_PROPERTY		"ro.product.version"
#define PRODUCT_NAME_PROPERTY		"ro.product.name"
#define BOARD_INFO_PROPERTY			"ro.product.board"

#define TMP_DIR			"/data/rjtmp"
#define TMP_DIR_DATA	"/data/rjtmp/data"
#define TMP_DIR_PROC	"/data/rjtmp/proc"
#define TMP_DIR_SDCARD	"/data/rjtmp/sdcard"
#define SYSTEM_LOG		"/sdcard/systemLog"
#define	CLIENT_LOG		"/sdcard/com.ruijie.rccstu"
#define	IP_ITF			"eth0"

#define RKNAND_GET_VENDOR_SECTOR0       _IOW('v', 16, unsigned int)
#define RKNAND_STORE_VENDOR_SECTOR0     _IOW('v', 17, unsigned int)

#define VENDOR_SECTOR_OP_TAG        0x444E4556 // "VEND"

#define RKNAND_DEVICE	"/dev/rknand_sys_storage"

#define RKNAND_SYS_STORGAE_DATA_LEN 512

typedef struct tagRKNAND_SYS_STORGAE {
    unsigned int  	tag;
    unsigned int  	len;
    unsigned char   data[RKNAND_SYS_STORGAE_DATA_LEN];
} RKNAND_SYS_STORGAE;


/*******************Function part begin***************************************/
static int createDir(const char* sPathName)
{
	char dirName[256];
    strcpy(dirName,   sPathName);
    int i,len = strlen(dirName);
    if(dirName[len-1]!='/')
		strcat(dirName,   "/");
    len = strlen(dirName);
    for(i=1;   i<len;   i++) {
		if(dirName[i]=='/') {
			dirName[i] = 0;
			if(access(dirName,F_OK) != 0) {
				if(mkdir(dirName,   0755)==-1) {
					ALOGD("grep create the dir:%s failed",dirName);
					return   -1;
				}
			}
			dirName[i]   =   '/';
		}
   }
   return   0;
}

static int get_local_ip(char const* ifname, char* ip)
{
    char *temp = NULL;
    int inet_sock;
    struct ifreq ifr;

    inet_sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    memcpy(ifr.ifr_name, ifname, strlen(ifname));

    if(0 != ioctl(inet_sock, SIOCGIFADDR, &ifr))
    {
        ALOGD("grep get_local_ip ioctl error");
        return -1;
    }

    temp = (char *)inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr);
    memcpy(ip, temp, strlen(temp));

    close(inet_sock);

    return 0;
}

static void litConverBig(char *buf)
{
        if(buf == NULL)
                return ;
        char* tmp;
        tmp = buf;
        while(*tmp != '\0') {
                if(*tmp > 97 && *tmp < 122) {
                        *tmp -= 32;
                }
                tmp++;
        }
}

static void itochar(unsigned long num,char *pstr)
{
        unsigned long val1 = num,val0 = 0;
        int i,j;
        char sastr[1024];
        memset(sastr,0,sizeof(sastr));
        for(i = 0; i < 100; i++) {
                val0 = val1 % 10;
                val1 = val1 / 10;
                sastr[i] = val0 + 48;
                if(val1 < 10) {
                        i++;
                        sastr[i] = val1 + 48;
                        break;
                }
        }
        for(j = 0; j <= i;j++)
                pstr[j] = sastr[i - j];
        pstr[j] = '\0';
}

static void getStorageSize(const char *s, char* result) {
    struct statfs st;
    char unit = 'K';
    long double size;

    if (statfs(s, &st) < 0) {
        
        printf("Can't find %s\n",s);
        return ;
    } else {
        if (st.f_blocks == 0)
            return;
        size = (long double)st.f_blocks * (long double)st.f_bsize;
    }
    size /= 1024;
    if (size > 1024) {
        size /= 1024;
        unit = 'M';
    }
    if (size > 1024) {
        size /= 1024;
        unit = 'G';
    }
    sprintf(result,"%-4.1Lf%c",size,unit);
}

/*******************Function part end***************************************/

namespace android {

// Product Utils
/***********************************************
*	函数名:		getCpuInfo
*	功能描述:	获取CPU信息
*	返回值：	返回获取到的CPU信息
************************************************/
String16 RjcoreService::getCpuInfo() {
	ALOGD("getCpuInfo");
	char res[1024];
	char sub[1024];
	int len = 0;
	
	memset(res,0,sizeof(res));
	memset(sub,0,sizeof(sub));
    executeCmd("cat /proc/cpuinfo | grep Processor",res);
	len = strlen(res);
	res[len - 1] = ' ';
	property_get("ro.product.name",sub,"");
	litConverBig(sub);
    strcat(res,sub);
    strcat(res,SUFFIX);
	return String16(res);
}
/***********************************************
*	函数名:		getAndroidVersion
*	功能描述:	获取安卓版本信息
*	返回值：	返回安卓版本信息
************************************************/
String16 RjcoreService::getAndroidVersion() {
	ALOGD("getAndroidVersion");
	char res[1024];
	memset(res,0,sizeof(res));
	property_get(ANDROID_VERSION_PROPERTY,res,"");
	
	return String16(res);
}

/***********************************************
*	函数名:		getSystemVersion
*	功能描述:	获取系统发布版本信息
*	返回值：	返回系统发布版本信息
************************************************/
String16 RjcoreService::getSystemVersion() {
	ALOGD("getSystemVersion");
	char res[1024];
	memset(res,0,sizeof(res));
	property_get(SYSTEM_VERSION_PROPERTY,res,"");
	
	return String16(res);
}

String16 RjcoreService::getClientVersion() {
	ALOGD("getClientVersion");

	return String16("");
}

/***********************************************
*	函数名:		getSerialNumber
*	功能描述:	获取产品序列号
*	返回值：	返回产品序列号
************************************************/
String16 RjcoreService::getSerialNumber() {
	ALOGD("getSerialNumber");
	char res[1024];
	FILE* file;
    int val = 0;
	int len = 0;
	memset(res,0,sizeof(res));
	
	property_get("ro.product.serialnum",res,"");
	if(res[0] == 0) {
		ALOGD("grep getSerialNumber from /proc/sn.info");
		file = fopen(INFO_PATH,"r+");
		if(file == NULL) {
			ALOGD("the %s open failed",INFO_PATH);
			return String16("");
		}
		len = fread(res, 1, SERIAL_NUMBER_OFF, file);
		if(2 == len) {
			if(res[1] == 0) {
				val = res[0];
			}
		}
		len = fread(res,1,val,file);
		if(val == len) {
			property_set("ro.product.serialnum", res);
			fclose(file);
			return String16(res);
		}
		fclose(file);		
	}
	
	return String16(res);
}
/***********************************************
*	函数名:		getCustProductModel
*	功能描述:	获取产品型号
*	返回值：	返回产品型号
************************************************/
String16 RjcoreService::getCustProductModel() {
	ALOGD("getCustProductModel");
	char res[1024];
	FILE* file;
    int val = 0;
	int len = 0;
	memset(res,0,sizeof(res));
	
	property_get("ro.product.custpromode",res,"");
	if(res[0] == 0) {
		ALOGD("grep getCustProductModel from /proc/sn.info");
		file = fopen(INFO_PATH,"r+");
		if(file == NULL) {
			ALOGD("the %s open failed",INFO_PATH);
			return String16("");
		}
		fseek(file,CUST_PRODUCT_MODEL_OFF,SEEK_SET);
		len =fread(res, 1, 1, file);
		if(len == 1) {
			 val = res[0];
		}
		len = fread(res, 1, val, file);
		if(val == len) {
			property_set("ro.product.custpromode", res);
			fclose(file);
			return String16(res);
		}
		fclose(file);	
	}
		
	return String16(res);
}
/***********************************************
*	函数名:		getHardwareVesion
*	功能描述:	获取硬件版本信息
*	返回值：	返回硬件版本信息
************************************************/
String16 RjcoreService::getHardwareVesion() {
	ALOGD("getHardwareVesion");

	char res[1024];
	FILE* file;
    int val = 0;
	int len = 0;
	memset(res,0,sizeof(res));
	
	property_get("ro.product.hwvesion",res,"");
	if(res[0] == 0) {
		ALOGD("grep getHardwareVesion from /proc/sn.info");
		file = fopen(INFO_PATH,"r+");
		if(file == NULL) {
			ALOGD("the %s open failed",INFO_PATH);
			return String16("");
		}
		fseek(file,HARDWARE_VERSION_OFF,SEEK_SET);
		len =fread(res, 1, 1, file);
		if(len == 1) {
			 val = res[0];
		}
		len = fread(res, 1, val, file);
		if(val == len) {
			property_set("ro.product.hwvesion", res);
			fclose(file);
			return String16(res);
		}
		fclose(file);
	}
	

	return String16(res);
}
/*************************************************************************
*	函数名:		getProductID
*	功能描述:	获取产品ID号
*	返回值：	返回产品ID号
*************************************************************************/
String16 RjcoreService::getProductID() {
	ALOGD("getProductID");

	char res[1024];
	FILE* file;
	int len = 0;
	unsigned long product_id = 0;
	int i;
	memset(res,0,sizeof(res));
	
	property_get("ro.product.productid",res,"");
	if(res[0] == 0) {
		ALOGD("grep getProductID from /proc/sn.info");
		file = fopen(INFO_PATH,"r+");
		if(file == NULL) {
			ALOGD("the %s open failed",INFO_PATH);
			return String16("");
		}
		fseek(file,PRODUCT_ID_OFF,SEEK_SET);
		len = fread(res, 1, PRODUCT_ID_LEN, file);
		if(len == PRODUCT_ID_LEN) { 
			for (i = 0; i < PRODUCT_ID_LEN; i++) {//convert
				product_id |= res[i] << (32 - 8 * (i+1));
			}
			itochar(product_id,res);
			property_set("ro.product.productid",res);
		}
		fclose(file);
	}
		
	return String16(res);
}
/*************************************************************************
*	函数名:		getProductName
*	功能描述:	获取产品名称
*	返回值：	返回产品名称
*************************************************************************/
String16 RjcoreService::getProductName() {
	ALOGD("getProductName");

	char res[1024];
	memset(res,0,sizeof(res));
	property_get(PRODUCT_NAME_PROPERTY,res,"");
	litConverBig(res);
	
	return String16(res);
}
/*************************************************************************
*	函数名:		getBaseboardInfo
*	功能描述:	获取主板信息
*	返回值：	返回主板信息
*************************************************************************/
String16 RjcoreService::getBaseboardInfo() {
	ALOGD("getBaseboardInfo");
	
	char res[1024];
	memset(res,0,sizeof(res));
	property_get(BOARD_INFO_PROPERTY,res,"");

	return String16(res);
}
/*************************************************************************
*	函数名:		getMemoryInfo
*	功能描述:	获取系统内存大小
*	返回值：	返回系统内存大小
*************************************************************************/
String16 RjcoreService::getMemoryInfo() {
	ALOGD("getMemoryInfo");
	
	char res[1024];
	memset(res,0,sizeof(res));
	executeCmd("cat /proc/meminfo | grep MemTotal:",res);

	return String16(res);
}
/*************************************************************************
*	函数名:		getStorageInfo
*	功能描述:	获取系统data分区的大小
*	返回值：	返回系统data分区的大小
*************************************************************************/
String16 RjcoreService::getStorageInfo() {
	ALOGD("getStorageInfo");
	
	char res[1024];
	memset(res,0,sizeof(res));
	getStorageSize("/data",res);

	return String16(res);
}

/*************************************************************************
*	函数名:		setPowerState
*	参数 1：	powerState = 1 开启上电自启
*				powerState = 0 关闭上电自启
*	功能描述:	开关上电自启功能
*	返回值：	无
*************************************************************************/
void RjcoreService::setPowerState(int powerState) {
	ALOGD("setPowerState");
	char res[128];
	memset(res,0,sizeof(res));
	sprintf(res,"beginmode %d",powerState);
	system(res);
}

/*************************************************************************
*	函数名:		getPowerState
*	功能描述:	获取上电自启状态
*	返回值：	返回上电自启状态
*************************************************************************/
int RjcoreService::getPowerState() {
	ALOGD("getPowerState");
	
	char res[1024];
	memset(res,0,sizeof(res));
	executeCmd("beginmode 2",res);

	return res[0];
}

/*************************************************************************
*	函数名:		collectLogFiles
*	参数 1：	zipFilename可以为空，默认使用localhost_ip.tar.gz的格式打包
*				powerState = 0 关闭上电自启
*	功能描述:	打包系统日志，并保存在"/sdcard/com.ruijie.rccstu/"路径下
*	返回值：	无
*************************************************************************/
void RjcoreService::collectLogFiles (String16 zipFilename) {
	ALOGD("collectLogFiles");
	char cmd[1024];
    char ip[64];
	memset(cmd,0,sizeof(cmd));
    memset(ip,0,sizeof(ip));
	get_local_ip(IP_ITF, ip);
	createDir(TMP_DIR);
    createDir(TMP_DIR_DATA);
    createDir(TMP_DIR_PROC);
    createDir(TMP_DIR_SDCARD);
	copyData(String16(SYSTEM_LOG),String16(TMP_DIR_DATA));
    copyData(String16(CLIENT_LOG),String16(TMP_DIR_SDCARD));
	chdir(TMP_DIR);
	sprintf(cmd,"tar zcvf /sdcard/com.ruijie.rccstu/localhost_%s.tar.gz ./ > /dev/null",ip);
	system(cmd);
}

/*************************************************************************
*	函数名:		copyData
*	参数 1：	源文件路径
*	参数 2：	目标路径
*	功能描述:	将目标文件拷贝到对应路径下
*	返回值：	无
*************************************************************************/
void RjcoreService::copyData(String16 srcPath, String16 dstPath) {
	ALOGD("copyData");
	char res[2048];
	memset(res,0,sizeof(res));
	sprintf(res,"cp -r %s %s",String8(srcPath).string(),String8(dstPath).string());
	system(res);
}

/*************************************************************************
*	函数名：	chmod
*	参数 1：	文件路径
*	功能描述:	变更文件权限
*	返回值：	无
*************************************************************************/
void RjcoreService::chmod(String16 path) {
	ALOGD("chmod");
	char res[1024];
	memset(res,0,sizeof(res));
	sprintf(res,"chmod 766 %s",String8(path).string());
	system(res);
}

/*************************************************************************
*	函数名：	shutDown
*	功能描述:	关机
*	返回值：	无
*************************************************************************/
void RjcoreService::shutDown() {
	ALOGD("shutDown");
	system("reboot -p");
}

/*************************************************************************
*	函数名：	setDeviceResolution
*	参数 1：	width reslution(800 ~ 2560)
*	参数 2：	height resolution(600 ~ 1600)
*	参数 3：	refresh resolution，一般为60
*	参数 4：	0 kernel does nothing, 1 kernel set resolution from vendor
*				sector
*	功能描述:	设置分辨率
*	返回值：	成功返回true 失败返回false
*************************************************************************/
bool RjcoreService::setDeviceResolution(int xres, int yres, int refresh, int flag) {
	ALOGD("setDeviceResolution");
	int	ret = 0;
    int	fd = 0;
	RKNAND_SYS_STORGAE sysData;
	unsigned short* p = NULL;
	
	if((xres < 800 && xres > 2560)) {
		ALOGD("grep param error: xres range is (800 ~ 2560)");
		return false;
	}
	
	if(yres < 600 && yres > 1600) {
		ALOGD("grep param error: yres range is (600 ~ 1600)");
		return false;
	}
	
	fd = open(RKNAND_DEVICE, O_RDWR, 0);
	if (fd < 0) {
		ALOGD("rknand_sys_storage open failed");
		return false;
	}
	
	sysData.tag = VENDOR_SECTOR_OP_TAG;
    sysData.len = RKNAND_SYS_STORGAE_DATA_LEN - 8;
    ret = ioctl(fd, RKNAND_GET_VENDOR_SECTOR0, &sysData);
	if (ret < 0) {
		ALOGD("get vendor_sector error");
        close(fd);
        return false;
	}
	
	sysData.tag = VENDOR_SECTOR_OP_TAG;
    sysData.len = RKNAND_SYS_STORGAE_DATA_LEN - 8;
	
	sysData.data[1] = (unsigned char)flag;
    if (flag) {
		sysData.data[1] = 0xAC;         /* resolution flag */
		p = (unsigned short*)&sysData.data[2];
        *p = (unsigned short)xres;
        p = (unsigned short*)&sysData.data[4];
        *p = (unsigned short)yres;
		sysData.data[6] = (unsigned char)refresh;
	}
	ret = ioctl(fd, RKNAND_STORE_VENDOR_SECTOR0, &sysData);
    if (ret < 0) {
		ALOGD("set vendor_sector error");
        close(fd);
        return false;
	}
	sysData.tag = VENDOR_SECTOR_OP_TAG;
    sysData.len = RKNAND_SYS_STORGAE_DATA_LEN - 8;
    ret = ioctl(fd, RKNAND_GET_VENDOR_SECTOR0, &sysData);
    if (ret < 0) {
		ALOGD("get vendor_sector error");
        close(fd);
        return false;
	}

	close(fd);
    return true;
}
/*************************************************************************
*	函数名：	controlLed
*	参数 1：	颜色值
*	参数 2：	状态值（0/1）
*	功能描述:	设置LED小灯状态
*	返回值：	无
*************************************************************************/
void RjcoreService::controlLed(int color, int state) {
	ALOGD("controlLed");
	char res[128];
	memset(res,0,sizeof(res));
	sprintf(res,"set_LED_ctrl %d %d",color,state);
	system(res);
}

void RjcoreService::setBrightness(int brightness) {
	ALOGD("setBrightness");
	
}

int RjcoreService::getBrightness() {
	ALOGD("getBrightness");

	return 0;
}

/*************************************************************************
*	函数名：	getCurrentResolution
*	功能描述:	获取当前的分辨率
*	返回值：	返回当前的分辨率
*************************************************************************/
String16 RjcoreService::getCurrentResolution() {
	ALOGD("getCurrentResolution");
	
	char res[1024];
	memset(res,0,sizeof(res));
	executeCmd("wm size",res);
	
	return String16(res);
}


} // namespace android
