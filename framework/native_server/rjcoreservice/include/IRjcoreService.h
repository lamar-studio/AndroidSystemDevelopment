
#ifndef ANDROID_IRJCORESERVICE_H
#define ANDROID_IRJCORESERVICE_H

#include <utils/Errors.h>  // for status_t
#include <utils/KeyedVector.h>
#include <utils/RefBase.h>
#include <utils/String16.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <system/audio.h>

namespace android {

class IRjcoreService: public IInterface
{
public:
    DECLARE_META_INTERFACE(RjcoreService);

    // Product Utils
    virtual String16 getCpuInfo() = 0;
    virtual String16 getAndroidVersion() = 0;
    virtual String16 getSystemVersion() = 0;
    virtual String16 getSerialNumber() = 0;
    virtual String16 getProductType() = 0;
    virtual String16 getHardwareVesion() = 0;
    virtual String16 getProductId() = 0;
    virtual String16 getProductName() = 0;
    virtual String16 getBaseboardInfo() = 0;
    virtual String16 getMemoryInfo() = 0;
    virtual String16 getStorageInfo() = 0;
    virtual void setPowerState(int powerState) = 0;
    virtual int getPowerState() = 0;
    virtual bool collectLogFiles_block (String16 zipFilename) = 0;  // time-consuming interface And synchronized
    virtual bool chmodFile(String16 path, String16 mode) = 0;
    virtual bool chmodDir(String16 path, String16 mode) = 0;
    virtual bool rmFile(String16 path) = 0;
    virtual void reboot() = 0;
    virtual void shutdown() = 0;
    virtual int getLedStatus(int color) = 0;
    virtual void setLedStatus(int color, int state) = 0;
    virtual bool setDeviceResolution(int xres, int yres, int refresh, int flag) = 0;
    virtual String16 getCurrentResolution_block() = 0;  // time-consuming interface
    virtual String16 getSupportResolution() = 0;
    virtual String16 getPropertyValue(String16 key) = 0;

    // Network Utils
    virtual bool getNetcardState() = 0;
    virtual String16 getNetcardCurSpeed() = 0;
    virtual String16 getNetcardMaxSpeed() = 0;
    virtual void startAvahi() = 0;
    virtual void stopAvahi() = 0;
    virtual String16 checkIpConflict_block(String16 ip) = 0;    //time-consuming interface
    virtual String16 ping_block(String16 ip, int count, int timeout, int deadline) = 0;  //time-consuming interface
    virtual String16 checkBandwidth_block(String16 serverIp, String16 user, String16 code) = 0;    //time-consuming interface And synchronized

    // Control Utils
    virtual bool getOtaResult() = 0;

    // Application Utils
    virtual bool isProcessRunning(String16 processname) = 0;
    virtual String16 getProcessId(String16 processname) = 0;
    virtual void killByProcessName(String16 processname) = 0;
};

// ----------------------------------------------------------------------------

class BnRjcoreService: public BnInterface<IRjcoreService>
{
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

}; // namespace android

#endif // ANDROID_IRJCORESERVICE_H
