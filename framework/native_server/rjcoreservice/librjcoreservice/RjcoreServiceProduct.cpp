
#define LOG_TAG "RjcoreServiceProduct"
// #define LOG_NDEBUG 0

#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>
#include <utils/Mutex.h>

#include <string.h>

#include <cutils/atomic.h>
#include <cutils/properties.h>  // for property_get

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
#include <errno.h>
#include <sys/statfs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <ctype.h>
}

#define SUFFIX                      " CPU, @ 1.6GHz"
#define ANDROID_VERSION_PROPERTY    "ro.build.version.release"
#define SYSTEM_VERSION_PROPERTY     "ro.product.version"
#define PRODUCT_NAME_PROPERTY       "ro.product.name"
#define BOARD_INFO_PROPERTY         "ro.product.board"
#define TOMBSTONES                  "/data/tombstones"
#define CLIENT_LOG                  "/sdcard/com.ruijie.rccstu"
#define PROC_LAST_LOG               "/proc/last_log"
#define SYSTEM_LOG                  "/data/systemLog"
#define IP_ITF                      "eth0"
#define RKNAND_DEVICE               "/dev/rknand_sys_storage"
#define LCDINFO_PATH                "/sdcard/com.ruijie.rccstu/lcdinfo.ini"

#define RKNAND_GET_VENDOR_SECTOR0       _IOW('v', 16, unsigned int)
#define RKNAND_STORE_VENDOR_SECTOR0     _IOW('v', 17, unsigned int)
#define VENDOR_SECTOR_OP_TAG            0x444E4556  // "VEND"
#define RKNAND_SYS_STORGAE_DATA_LEN     512
#define MAX_RESOLUTION_SUPPORT          10  // base on kernel "default_modedb" element nums
#define RESOLUTION_INFO_LEN             4

typedef struct tagRKNAND_SYS_STORGAE {
    unsigned int    tag;
    unsigned int    len;
    unsigned char   data[RKNAND_SYS_STORGAE_DATA_LEN];
} RKNAND_SYS_STORGAE;

static char resolutionIndex[][10] = {  // "resolutionIndex" in accordance with kernel src scaler-edid.c's "default_modedb"
    { "800x600" },
    { "1024x768" },
    { "1280x720" },
    { "1280x1024" },
    { "1360x768" },
    { "1366x768" },
    { "1440x900" },
    { "1600x900" },
    { "1680x1050" },
    { "1920x1080" },
};

namespace android {

/*******************Function part begin***************************************/
static void litConverBig(char *buf) {
    char* tmp;
    tmp = buf;

    if ((buf == NULL) || (strlen(buf) == 0)) {
        return;
    }

    while (*tmp != '\0') {
        *tmp = toupper(*tmp);
        tmp++;
    }
}

static void getStorageSize(const char *s, char *result) {
    struct statfs st;
    char unit = 'K';
    long double size = 0.0;

    if ((s == NULL) || (strlen(s) == 0) || (result == NULL)) {
        sprintf(result, "%-4.1Lf%c", size, unit);  // failed default return 0.0K to result
        return;
    }

    if (statfs(s, &st) < 0) {
        ALOGE("getStorageSize Can't find %s\n", s);
        sprintf(result, "%-4.1Lf%c", size, unit);  // failed default return 0.0K to result
        return;
    } else {
        if (st.f_blocks == 0) {
            sprintf(result, "%-4.1Lf%c", size, unit);
            return;
        }
        size = (long double)st.f_blocks * (long double)st.f_bsize;
    }

    size /= SIZE_1K;

    if (size > SIZE_1K) {
        size /= SIZE_1K;
        unit = 'M';
    }
    if (size > SIZE_1K) {
        size /= SIZE_1K;
        unit = 'G';
    }

    sprintf(result, "%-4.1Lf%c", size, unit);
}

static bool checkFileMode(const char *path, const char *mode, int modeLen) {
    char cmd[SIZE_1K_HALF] = { 0 };
    char checkMode[SIZE_1K_HALF] = { 0 };

    if (modeLen == 3) {  // mode mabye 777 766 xxx ...
        snprintf(cmd, sizeof(cmd), "stat %s | head -n4 |tail -n1 | cut -d \"/\" -f1 | cut -d \"(\" -f2 |  sed 's/.//'", path);
        executeCmd(cmd, checkMode, sizeof(checkMode));
        if (strncmp(checkMode, mode, 3) != 0) {
            return false;
        }
    } else if (modeLen == 4) {  // mode mabye 0777 0766 0xxx ...
        snprintf(cmd, sizeof(cmd), "stat %s | head -n4 |tail -n1 | cut -d \"/\" -f1 | cut -d \"(\" -f2", path);
        executeCmd(cmd, checkMode, sizeof(checkMode));
        if (strncmp(checkMode, mode, 4) != 0) {
            return false;
        }
    } else {
            return false;
    }

    return true;
}

/*******************Function part end***************************************/

static Mutex sLockLog;

// Product Utils
String16 RjcoreService::getCpuInfo() {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };
    char sub[SIZE_1K] = { 0 };
    int len = 0;
    int ret = 0;

    executeCmd("cat /proc/cpuinfo | grep Processor | awk '{print $3,$4,$5,$6,$7}'", res, sizeof(res));
    CHECK_STR16_RES(res);
    len = strlen(res);
    if (res[len - 1] == '\n')
        res[len - 1] = ' ';

    ret = property_get("ro.product.name", sub, NULL);
    if (ret <= 0) {
        ALOGE("getCpuInfo property_get failed!");
        return String16("");
    }

    litConverBig(sub);
    strcat(res, sub);
    strcat(res, SUFFIX);
    return String16(res);
}

String16 RjcoreService::getAndroidVersion() {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };
    int ret = 0;

    ret = property_get(ANDROID_VERSION_PROPERTY, res, NULL);
    if (ret <= 0) {
        ALOGE("getAndroidVersion property_get failed!");
        return String16("");
    }

    return String16(res);
}

String16 RjcoreService::getSystemVersion() {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };
    int ret = 0;

    ret = property_get(SYSTEM_VERSION_PROPERTY, res, NULL);
    if (ret <= 0) {
        ALOGE("getSystemVersion property_get failed!");
        return String16("");
    }

    return String16(res);
}

String16 RjcoreService::getSerialNumber() {
    CHECK_INIT(mInit);
    CHECK_FUNCTION_IN();

    return mSerialNum;
}

String16 RjcoreService::getProductType() {
    CHECK_INIT(mInit);
    CHECK_FUNCTION_IN();

    return mProductType;
}

String16 RjcoreService::getHardwareVesion() {
    CHECK_INIT(mInit);
    CHECK_FUNCTION_IN();

    return mHardwareVersion;
}

String16 RjcoreService::getProductId() {
    CHECK_INIT(mInit);
    CHECK_FUNCTION_IN();

    return mProductId;
}

String16 RjcoreService::getProductName() {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };
    int ret = 0;

    ret = property_get(PRODUCT_NAME_PROPERTY, res, NULL);
    if (ret <= 0) {
        ALOGE("getProductName property_get failed!");
        return String16("");
    }
    litConverBig(res);

    return String16(res);
}

String16 RjcoreService::getBaseboardInfo() {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };
    int ret = 0;

    ret = property_get(BOARD_INFO_PROPERTY, res, NULL);
    if (ret <= 0) {
        ALOGE("getBaseboardInfo property_get failed!");
        return String16("");
    }

    return String16(res);
}

String16 RjcoreService::getMemoryInfo() {
    CHECK_FUNCTION_IN();
    char  res[SIZE_1K] = { 0 };
    int   memval = 0;
    char  unit = 'K';

    executeCmd("cat /proc/meminfo | grep \"MemTotal:\" | awk '{print $2}'", res, sizeof(res));
    CHECK_STR16_RES(res);

    memval = atoi(res);
    if (memval > SIZE_1K) {
        memval /= SIZE_1K;
        unit = 'M';
    }
    if (memval > SIZE_1K) {
        memval /= SIZE_1K;
        unit = 'G';
    }

    memset(res, 0, sizeof(res));
    snprintf(res, sizeof(res), "%d%c", memval, unit);
    return String16(res);
}

String16 RjcoreService::getStorageInfo() {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };

    getStorageSize("/data", res);

    return String16(res);
}

void RjcoreService::setPowerState(int powerState) {
    CHECK_FUNCTION_IN();
    char res[128] = { 0 };

    if (powerState < 0 || powerState > 1) {
        ALOGE("setPowerState param error: 0 <= powerState <= 1");
        return;
    }

    if ((mProductType == String16("Rain100")) || (mProductType == String16("Rain100s")) \
        || (mProductType == String16("Rain100S")) || (mProductType == String16(""))) {
        ALOGE("setPowerState Rain100/Rain100s/Rain100S not support"); 
        return;
    }

    snprintf(res, sizeof(res), "beginmode %d", powerState);
    if (mySystem(res) < 0) {
        ALOGE("setPowerState execute error!");
        return;
    }

    ALOGW("setPowerState set Ok, it will be work after reboot!");
}

int RjcoreService::getPowerState() {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };

    if ((mProductType == String16("Rain100")) || (mProductType == String16("Rain100s")) \
        || (mProductType == String16("Rain100S")) || (mProductType == String16(""))) {
        ALOGE("setPowerState Rain100/Rain100s/Rain100S not support");
        return 0;
    }    

    executeCmd("beginmode 2", res, sizeof(res));
    if (res[0] < '0') {  // '0':enable Wol '1': disable Wol
        ALOGE("getPowerState invalid result");
        return -1;
    }
    return res[0] - '0';
}

/* time-consuming interface */
bool RjcoreService::collectLogFiles_block(String16 zipFilename) {
    CHECK_FUNCTION_IN();
    char cmd[SIZE_1K] = { 0 };
    
    if (zipFilename == String16("")) {
        ALOGE("collectLogFiles_block not set the zipFilename!");
        return false;
    }

    if (strstr(String8(zipFilename).string(), ".zip") == NULL) {  // check zipFilename format
        ALOGE("collectLogFiles_block zipFilename must be *.zip format");
        return false;
    }

    Mutex::Autolock _l(sLockLog);  // add mutex lock

    if (mySystem("save_systeminfo") < 0) {
        ALOGE("collectLogFiles_block execute error!");
        return false;
    }

    if (access(SYSTEM_LOG, F_OK) != 0) {
        ALOGE("collectLogFiles_block the file %s not found!", SYSTEM_LOG);
        return false;
    }

    if (access(TOMBSTONES, F_OK) != 0) {
        if (mySystem("mkdir /data/tombstones") < 0) {
            ALOGE("collectLogFiles_block execute error! line = %d", __LINE__);
            return false;
        }

        if (chmodDir(String16("/data/tombstones"), String16("766")) == false) {
            ALOGE("collectLogFiles_block chmodDir execute error!");
            return false;
        }
    }

    if (access(CLIENT_LOG, F_OK) != 0) {
        ALOGE("collectLogFiles_block the file %s not found!", CLIENT_LOG);
        return false;
    }

    if (access(PROC_LAST_LOG, F_OK) != 0) {
        ALOGE("collectLogFiles_block the file %s not found!", PROC_LAST_LOG);
        return false;
    }

    snprintf(cmd, sizeof(cmd), "tar zcvf /sdcard/localhost.tar.gz %s %s %s %s> /dev/null", \
                String8(CLIENT_LOG).string(), String8(PROC_LAST_LOG).string(),             \
                String8(TOMBSTONES).string(), String8(SYSTEM_LOG).string());

    if (mySystem(cmd) < 0) {
        ALOGE("collectLogFiles_block execute error! line = %d", __LINE__);
        return false;
    }

    snprintf(cmd, sizeof(cmd), "mv /sdcard/localhost.tar.gz /sdcard/%s", String8(zipFilename).string());

    if (mySystem(cmd) < 0) {
        ALOGE("collectLogFiles_block execute error! line = %d", __LINE__);
        return false;
    }
    sync();
    
    return true;
}

bool RjcoreService::chmodFile(String16 path, String16 mode) {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };

    if (mode == String16("")) {
        ALOGE("chmodFile not set the mode!");
        return false;
    }

    if (access(String8(path).string(), F_OK) == 0) {
        if (opendir(String8(path).string()) != NULL) {
            ALOGE("chmodFile not support dir!");
            return false;
        }

        snprintf(res, sizeof(res), "chmod %s %s", String8(mode).string(), String8(path).string());
        if (mySystem(res) < 0) {
            ALOGE("chmodFile execute error!");
            return false;
        }
    } else {
        ALOGE("path not find");
        return false;
    }

    if (checkFileMode(String8(path).string(), String8(mode).string(), strlen(String8(mode).string())) == false) {
        ALOGE("chmodFile file:%s mode change failed", String8(path).string());
        return false;
    }

    return true;
}

bool RjcoreService::chmodDir(String16 path, String16 mode) {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };

    if (mode == String16("") || path == String16("")) {
        ALOGE("chmodDir not set mode or path not set!");
        return false;
    }

    if (opendir(String8(path).string()) != NULL) {
        snprintf(res, sizeof(res), "chmod -R %s %s", String8(mode).string(), String8(path).string());
        if (mySystem(res) < 0) {
            ALOGE("chmodDir execute error!");
            return false;
        }
    } else {
        ALOGE("path not find");
        return false;
    }

    if (checkFileMode(String8(path).string(), String8(mode).string(), strlen(String8(mode).string())) == false) {
        ALOGE("chmodFile file:%s mode change failed", String8(path).string());
        return false;
    }

    return true;
}

bool RjcoreService::rmFile(String16 path) {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };

    if (path == String16("")) {
        ALOGE("Can't supprot remove Dir!");
        return false;
    }

    if (access(String8(path).string(), F_OK) == 0) {
        if (opendir(String8(path).string()) != NULL) {
            ALOGE("rmFile can't support the dir");
            return false;
        }
        snprintf(res, sizeof(res), "rm -f %s", String8(path).string());
        if (mySystem(res) < 0) {
            ALOGE("rmFile execute error!");
            return false;
        }
        ALOGW("remove the file:%s successful!", String8(path).string());
    } else {
        ALOGE("rmFile path not find");
        return false;
    }
    return true;
}

void RjcoreService::reboot() {
    CHECK_FUNCTION_IN();
    if (mySystem("reboot") < 0) {
        ALOGE("reboot execute error!");
        return;
    }
}

void RjcoreService::shutdown() {
    CHECK_FUNCTION_IN();
    if (mySystem("reboot -p") < 0) {
        ALOGE("mySystem execute error!");
        return;
    }
}

int RjcoreService::getLedStatus(int color) {
    CHECK_FUNCTION_IN();
    char res[128] = { 0 };
    char cmd[128] = { 0 };

    if (color < 0 || color > 2) {
        ALOGE("getLedStatus param error: 0 <= color <= 2");
        return -1;
    }

    if (mProductType != String16("Rain200S")) {  // check rain200s
        ALOGE("getLedStatus not support the led control!");
        return -1;
    }
    snprintf(cmd, sizeof(cmd), "get_LED_ctrl %d | awk '{print $4}'", color);
    executeCmd(cmd, res, sizeof(res));
    if (res[0] < '0') {  // '0':GREEN '1':RED '2':BLUE
        ALOGE("getLedStatus invalid result!");
        return -1;
    }
    return res[0] - '0';
}

void RjcoreService::setLedStatus(int color, int state) {
    CHECK_FUNCTION_IN();
    char res[128] = { 0 };

    if (color < 0 || color > 2) {
        ALOGE("setLedStatus param error: 0 <= color <= 2 ");
        return;
    }

    if (state < 0 || state > 1) {
        ALOGE("setLedStatus param error: 0 <= state <= 1 ");
        return;
    }

    if (mProductType != String16("Rain200S")) {  // check rain200s
        ALOGE("setLedStatus not support the led control!");
        return;
    }

    snprintf(res, sizeof(res), "set_LED_ctrl %d %d", color, state);
    if (mySystem(res) < 0) {
        ALOGE("setLedStatus execute error!");
        return;
    }
}

bool RjcoreService::setDeviceResolution(int xres, int yres, int refresh, int flag) {
    CHECK_FUNCTION_IN();
    int ret = 0;
    int fd = 0;
    RKNAND_SYS_STORGAE sysData;
    unsigned short* p = NULL;

    if (refresh < 0 || refresh > 100) {
        ALOGE("setDeviceResolution: param error: 0 <= refresh <= 100");
        return false;
    }

    if (flag < 0 || flag > 1) {
        ALOGE("setDeviceResolution: param error: 0 <= flag <= 1");
        return false;
    }

    if ((xres < 800 || xres > 2560)) {   // xres range is (800 ~ 2560)
        ALOGE("setDeviceResolution: param error: xres range is (800 ~ 2560)");
        return false;
    }

    if (yres < 600 || yres > 1600) {  // yres range is (600 ~ 1600)
        ALOGE("setDeviceResolution: param error: yres range is (600 ~ 1600)");
        return false;
    }

    fd = open(RKNAND_DEVICE, O_RDWR, 0);
    if (fd < 0) {
        ALOGE("setDeviceResolution: knand_sys_storage open failed");
        return false;
    }

    sysData.tag = VENDOR_SECTOR_OP_TAG;
    sysData.len = RKNAND_SYS_STORGAE_DATA_LEN - 8;
    ret = ioctl(fd, RKNAND_GET_VENDOR_SECTOR0, &sysData);
    if (ret < 0) {
        ALOGE("setDeviceResolution: get vendor_sector error");
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
        ALOGE("setDeviceResolution: set vendor_sector error");
        close(fd);
        return false;
    }
    sysData.tag = VENDOR_SECTOR_OP_TAG;
    sysData.len = RKNAND_SYS_STORGAE_DATA_LEN - 8;
    ret = ioctl(fd, RKNAND_GET_VENDOR_SECTOR0, &sysData);
    if (ret < 0) {
        ALOGE("setDeviceResolution: get vendor_sector error");
        close(fd);
        return false;
    }

    close(fd);

    ALOGW("setDeviceResolution: set the resolution ok, it will be work after reboot");

    return true;
}

String16 RjcoreService::getCurrentResolution_block() {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };
    int len = 0;

    executeCmd("wm size | grep \"Physical size:\" | awk '{print $3}'", res, sizeof(res));
    CHECK_STR16_RES(res);
    len = strlen(res);
    if (res[len - 1] == '\n') {
        res[len - 1] = '\0';
    }
    return String16(res);
}

String16 RjcoreService::getSupportResolution() {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };
    char tmp[128] = { 0 };
    FILE *file = NULL;
    int len = 0;
    int i = 0;
    char buf[RESOLUTION_INFO_LEN] = { 0 };
    int supportIndex = 0;

    file = fopen(LCDINFO_PATH, "rb");
    if (file == NULL) {
        ALOGE("getSupportResolution %s fopen failed", LCDINFO_PATH);
        return String16("");
    }

    len = fread(buf, 1, RESOLUTION_INFO_LEN, file);
    if (len != RESOLUTION_INFO_LEN) {
        ALOGE("getSupportResolution fread failed\n");
        fclose(file);
        return String16("");
    }

    supportIndex = buf[1];
    supportIndex = (supportIndex << 8) | buf[0];
    supportIndex &= 0x3FF;  // only check the last 10 bits

    for (i = 1; i < MAX_RESOLUTION_SUPPORT; i++)  {  // skip the 800x600 resolution
        if (supportIndex & (1 << i)) {
            snprintf(tmp, sizeof(tmp), "{%s},", resolutionIndex[i]);
            strcat(res, tmp);
        }
    }
    len = strlen(res);  // grep the last ','
    if (len == 0) {
        ALOGE("getSupportResolution not support the product!");
    } else if (res[len - 1] == ',') {
        res[len - 1] = '\0';
    }

    fclose(file);
    return String16(res);
}

String16 RjcoreService::getPropertyValue(String16 key) {
    CHECK_FUNCTION_IN();
    char res[PROPERTY_VALUE_MAX] = { 0 };

    if (key.size() == 0) {
        ALOGW("getPropertyValue the key is invalid.");
        return String16("");
    }

    property_get(String8(key).string(), res, "");
    if (res[0] == '\0') {
        ALOGW("getPropertyValue return default val");
    }

    return String16(res);
}

}  // namespace android
