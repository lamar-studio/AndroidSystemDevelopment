
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
	virtual String16 getClientVersion() = 0;
	virtual String16 getSerialNumber() = 0;
	virtual String16 getCustProductModel() = 0;
	virtual String16 getHardwareVesion() = 0;
	virtual String16 getProductID() = 0;
	virtual String16 getProductName() = 0;
	virtual String16 getBaseboardInfo() = 0;
	virtual String16 getMemoryInfo() = 0;
	virtual String16 getStorageInfo() = 0;
	virtual void setPowerState(int powerState) = 0;
	virtual int getPowerState() = 0;
	virtual void collectLogFiles (String16 zipFilename) = 0;
	virtual void copyData(String16 srcPath, String16 dstPath) = 0;
	virtual void chmod(String16 path) = 0;
	virtual void shutDown() = 0;
	//virtual bool setDeviceResolution(String16 resolution) = 0;
	virtual bool setDeviceResolution(int xres, int yres, int refresh, int flag) = 0;
	virtual void controlLed(int color, int state) = 0;
	virtual void setBrightness(int brightness) = 0;
	virtual int getBrightness() = 0;
	
	// Network Utils
	virtual String16 getMacAddress() = 0;
	virtual String16 getIPAddress() = 0;
	virtual String16 getGetwayAddress() = 0;
	virtual String16 getNetmask() = 0;
	virtual String16 getDns1Address() = 0;
	virtual String16 getDns2Address() = 0;
	virtual bool getNetcardState() = 0;
	virtual String16 getNetcardCurSpeed() = 0;
	virtual String16 getNetcardMaxSpeed() = 0;
	virtual void startAvahi() = 0;
	virtual void stopAvahi() = 0;
	virtual String16 checkIpConfict() = 0;
	virtual String16 ping(String16 ip, int count) = 0;

	// Control Utils
	virtual bool getOtaResult() = 0;

	// Application Utils
	virtual int isRunningPackage(String16 processname) = 0;
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
