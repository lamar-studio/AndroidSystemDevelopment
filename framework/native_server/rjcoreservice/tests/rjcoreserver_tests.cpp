
#define LOG_TAG "RjcoreServiceTest"

#include <sys/prctl.h>
#include <sys/wait.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <cutils/properties.h>
#include <utils/Log.h>
#include "IRjcoreService.h"

#include "RjcoreService.h"
#include <utils/String16.h>

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/statfs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <fcntl.h>
}

using namespace android;
void doCmd(const char *cmd, char *result);

int main(void)
{
    String16 result;
    char res[1024] = { 0 };
    int ret = 0;
    sp<ProcessState> proc(ProcessState::self());

    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("ruijie.core.service"));

    if (binder == 0) {
        printf("can't get hello service\n");
        return -1;
    }

    sp<IRjcoreService> service = interface_cast<IRjcoreService>(binder);

    result = service->getCpuInfo();
    ALOGW("rxhu getCpuInfo:%s\n",String8(result).string());

    result = service->getAndroidVersion();
    ALOGW("rxhu getAndroidVersion:%s\n",String8(result).string());

    result = service->getSystemVersion();
    ALOGW("rxhu getSystemVersion:%s\n",String8(result).string());

    result = service->getSerialNumber();
    ALOGW("rxhu getSerialNumber:%s\n",String8(result).string());

    result = service->getProductType();
    ALOGW("rxhu getProductType:%s\n",String8(result).string());

    result = service->getHardwareVesion();
    ALOGW("rxhu getHardwareVesion:%s\n",String8(result).string());

    result = service->getProductId();
    ALOGW("rxhu getProductId:%s\n",String8(result).string());

    result = service->getProductName();
    ALOGW("rxhu getProductName:%s\n",String8(result).string());

    result = service->getBaseboardInfo();
    ALOGW("rxhu getBaseboardInfo:%s\n",String8(result).string());

    result = service->getMemoryInfo();
    ALOGW("rxhu getMemoryInfo:%s\n",String8(result).string());

    result = service->getStorageInfo();
    ALOGW("rxhu getStorageInfo:%s\n",String8(result).string());

    ret = service->getNetcardState();
    ALOGW("linzr getNetcardState:%d\n", ret);

    result = service->getNetcardCurSpeed();
    ALOGW("linzr getNetcardCurSpeed:%s\n",String8(result).string());

    result = service->getNetcardMaxSpeed();
    ALOGW("linzr getNetcardMaxSpeed:%s\n",String8(result).string());

    String16 ip_con("172.21.195.18");
    result = service->checkIpConflict_block(ip_con);
    ALOGW("linzr checkIpConflict_block:%s\n",String8(result).string());

    String16 ip("14.215.177.39");
    result = service->ping_block(ip, 10, 0, 10);
    ALOGW("linzr ping_block:%s\n",String8(result).string());

    String16 user("speed");
    String16 code("linzr");
    result = service->checkBandwidth_block(ip_con, user, code);
    ALOGW("linzr checkBandwidth_block:%s\n",String8(result).string());

    ret = service->getOtaResult();
    ALOGW("linzr getOtaResult:%d\n", ret);

    service->startAvahi();
    ALOGW("linzr startAvahi");

    service->stopAvahi();
    ALOGW("linzr stopAvahi");

    service->setPowerState(1);

    ret = service->getPowerState();
    ALOGW("rxhu getPowerState:%d\n",ret);

    result = service->getCurrentResolution_block();
    ALOGW("rxhu getCurrentResolution_block:%s\n",String8(result).string());

    result = service->getSupportResolution();
    ALOGW("rxhu getSupportResolution:%s",String8(result).string());

    result = service->getPropertyValue(String16("wifi.supplicant_scan_interval"));
    ALOGW("rxhu getPropertyValue:%s",String8(result).string());

    if (service->isProcessRunning(String16("com.android.providers.calendar")) == true) {  // modify
        ALOGW("rxhu the process is running");
    } else {
        ALOGW("rxhu the process is not running");
    }

    if ((result = service->getProcessId(String16("daemonsu"))) == String16("")) {  // new add
        ALOGW("rxhu not have the process");
    } else {
        ALOGW("rxhu the process is pid:%s",String8(result).string());
    }

    service->setLedStatus(1, 1);
    ret = service->getLedStatus(1);
    ALOGW("rxhu getLedStatus ret = %d", ret);

    if (service->setDeviceResolution(1920,1080,60,1) == true) {
        ALOGW("rxhu setDeviceResolution set ok\n");
    } else {
        ALOGW("rxhu setDeviceResolution set failed\n");
    }
    service->collectLogFiles_block(String16("localhost_172.21.5.123"));

    doCmd("ls /sdcard/com.ruijie.rccstu/localhost_172.21.5.123.tar.gz",res);
    ALOGW("rxhu check the log file:%s",res);
    if (service->rmFile(String16("/sdcard/com.ruijie.rccstu/localhost_172.21.5.123.tar.gz")) == false) {  // new add
        ALOGW("rxhu rm File failed!");
    } else {
        ALOGW("rxhu rm File Success!");
    }
    system("mkdir /data/theTest");
    system("touch /data/testFile");
    if (service->chmodFile(String16("/data/testFile"), String16("744")) == false) {  // new add
        ALOGW("rxhu chmodFile failed!");
    } else {
        ALOGW("rxhu chmodFile Success");
    }
    if (service->chmodDir(String16("/data/theTest"), String16("755")) == false) {  // new add
        ALOGW("rxhu chmodDir failed!");
    } else {
        ALOGW("rxhu chmodDir Success");
    }
    memset(res, 0 , sizeof(res));
    doCmd("ls -l /data/testFile", res);
    ALOGW("rxhu check the 744 chmodFile:%s", res);
    memset(res, 0, sizeof(res));
    doCmd("ls -l /data | grep theTest",res);
    ALOGW("rxhu check the 755 chmodDir:%s", res);

    service->rmFile(String16("/data/testFile"));
    system("rm -r /data/theTest");
    service->killByProcessName(String16("main_test"));
#if 0
    result = service->getProcessId(String16("dhcpcd"));
    ALOGW("rxhu get the dhcpcd pid = %s", String8(result).string());
    sleep(2);
    result = service->getProcessId(String16("dhcpcd"));
    ALOGW("rxhu killByProcessName check dhcpcd pid = %s", String8(result).string());
    system("/system/bin/dhcpcd&");
#endif
    ALOGW("rxhu test end!");
//    service->reboot();
//    service->shutdown();
    return 0;
}

void doCmd(const char *cmd, char *result)
{
    if ((strlen(cmd) > (1024 - 1)) || (cmd == NULL) || (result == NULL)) {
        ALOGW("the cmd is invalid");
        return;
    }

    char buf_ps[1024] = {0};
    char ps[1024] = {0};
    FILE *ptr;
    strcpy(ps, cmd);
    if ((ptr=popen(ps, "r")) != NULL) {
        while (fgets(buf_ps, 1024, ptr) != NULL) {
            if (strlen(result) > 1024) {
                break;
            }

           strcat(result, buf_ps);
        }
        pclose(ptr);
        ptr = NULL;
    } else {
        ALOGE("open %s error", ps);
    }
}
