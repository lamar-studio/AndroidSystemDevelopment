
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

namespace android {

class RjcoreService : public BnRjcoreService
{
public:
    static void instantiate();

    //the instance for RjcoreService
    static RjcoreService* getInstance();

    // Common Utils
    void executeCmd(const char *cmd, char *result);

	// Product Utils
	virtual String16 getCpuInfo();
	virtual String16 getAndroidVersion();
	virtual String16 getSystemVersion();
	virtual String16 getClientVersion();
	virtual String16 getSerialNumber();
	virtual String16 getCustProductModel();
	virtual String16 getHardwareVesion();
	virtual String16 getProductID();
	virtual String16 getProductName();
	virtual String16 getBaseboardInfo();
	virtual String16 getMemoryInfo();
	virtual String16 getStorageInfo();
	virtual void setPowerState(int powerState);
	virtual int getPowerState();
	virtual void collectLogFiles (String16 zipFilename);
	virtual void copyData(String16 srcPath, String16 dstPath);
	virtual void chmod(String16 path);
	virtual void shutDown();
	virtual bool setDeviceResolution(int xres, int yres, int refresh, int flag);
	virtual void controlLed(int color, int state);
	virtual void setBrightness(int brightness);
	virtual int getBrightness();
	virtual String16 getCurrentResolution();//add
	
	// Network Utils
	virtual String16 getMacAddress();
	virtual String16 getIPAddress();
	virtual String16 getGetwayAddress();
	virtual String16 getNetmask();
	virtual String16 getDns1Address();
	virtual String16 getDns2Address();
	virtual bool getNetcardState();
	virtual String16 getNetcardCurSpeed();
	virtual String16 getNetcardMaxSpeed();
	virtual void startAvahi();
	virtual void stopAvahi();
	virtual String16 checkIpConfict();
	virtual String16 ping(String16 ip, int count);

	// Control Utils
	virtual bool getOtaResult();

	// Application Utils
	virtual int isRunningPackage(String16 processname);

private:

    RjcoreService();
    virtual ~RjcoreService();

    static RjcoreService *mInstance;

};

}; // namespace android

#endif // ANDROID_RJCORESERVICE_H
