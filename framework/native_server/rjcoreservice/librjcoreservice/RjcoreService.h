
#ifndef ANDROID_RJCORESERVICE_H
#define ANDROID_RJCORESERVICE_H

#include <stddef.h>
#include <stdint.h>
#include <utils/threads.h>
#include <utils/Errors.h>
#include <utils/KeyedVector.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include <utils/Vector.h>

#include "IRjcoreService.h"

#define INFO_PATH "/proc/sn.info"

#define SERIAL_NUMBER_LEN       2
#define PRODUCT_TYPE_OFF        32
#define HARDWARE_VERSION_OFF    48
#define PRODUCT_ID_OFF          58
#define PRODUCT_ID_LEN          4
#define SIZE_1K                 1024
#define SIZE_2K                 2048
#define SIZE_1K_HALF            512

#define CHECK_STR16_RES(res)                                          \
    do {                                                              \
        if (res != NULL) {                                            \
            if (*res == '\0') {                                       \
                ALOGE("%s:can not get the result", __FUNCTION__);     \
                return String16("");                                  \
            }                                                         \
        }                                                             \
    } while (0)

#define CHECK_FUNCTION_IN()                                           \
    do {                                                              \
        ALOGV("%s:%d IN", __FUNCTION__, __LINE__);                    \
    } while (0)

#define CHECK_INIT(stat)                                              \
        do {                                                          \
            if (stat == false) {                                      \
                ALOGE("%s:init fail", __FUNCTION__);                  \
                return String16("");                                  \
            }                                                         \
        } while (0)

namespace android {
void executeCmd(const char *cmd, char *result, size_t size);

class RjcoreService : public BnRjcoreService
{
public:
    static void instantiate();

    // Product Utils
    virtual String16 getCpuInfo();
    virtual String16 getAndroidVersion();
    virtual String16 getSystemVersion();
    virtual String16 getSerialNumber();
    virtual String16 getProductType();
    virtual String16 getHardwareVesion();
    virtual String16 getProductId();
    virtual String16 getProductName();
    virtual String16 getBaseboardInfo();
    virtual String16 getMemoryInfo();
    virtual String16 getStorageInfo();
    virtual void setPowerState(int powerState);
    virtual int getPowerState();
    virtual bool collectLogFiles_block(String16 zipFilename);  // time-consuming interface And synchronized
    virtual bool chmodFile(String16 path, String16 mode);
    virtual bool chmodDir(String16 path, String16 mode);
    virtual bool rmFile(String16 path);
    virtual void reboot();
    virtual void shutdown();
    virtual int getLedStatus(int color);
    virtual void setLedStatus(int color, int state);
    virtual bool setDeviceResolution(int xres, int yres, int refresh, int flag);
    virtual String16 getCurrentResolution_block();  // time-consuming interface
    virtual String16 getSupportResolution();
    virtual String16 getPropertyValue(String16 key);

    // Network Utils
    virtual bool getNetcardState();
    virtual String16 getNetcardCurSpeed();
    virtual String16 getNetcardMaxSpeed();
    virtual void startAvahi();
    virtual void stopAvahi();
    virtual String16 checkIpConflict_block(String16 ip);  //time-consuming interface
    virtual String16 ping_block(String16 ip, int count, int timeout, int deadline);  //time-consuming interface
    virtual String16 checkBandwidth_block(String16 serverIp, String16 user, String16 code);  //time-consuming interface And synchronized

    // Control Utils
    virtual bool getOtaResult();

    // Application Utils
    virtual bool isProcessRunning(String16 processname);
    virtual String16 getProcessId(String16 processname);
    virtual void killByProcessName(String16 processname);

private:
    static int mySystem(const char *cmd);

    RjcoreService();
    virtual ~RjcoreService();

    int init();
    bool getNandStoreInfo();

    bool mInit;
    String16 mSerialNum;
    String16 mProductType;
    String16 mHardwareVersion;
    String16 mProductId;
};

}; // namespace android

#endif // ANDROID_RJCORESERVICE_H
