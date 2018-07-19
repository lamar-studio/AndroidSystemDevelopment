#include <stdint.h>
#include <sys/types.h>

#include <binder/Parcel.h>
#include <binder/IMemory.h>

#include <utils/Errors.h>  // for status_t
#include <utils/String16.h>

#include "IRjcoreService.h"

namespace android {

enum {
	// Product Utils
    GET_CPU_INFO = IBinder::FIRST_CALL_TRANSACTION,
	GET_ANDROID_VERSION,
	GET_SYSTEM_VERSION,
	GET_CLIENT_VERSION,
	GET_SERIAL_NUMBER,
	GET_CUST_PRODUCT_MODEL,
	GET_HARDWARE_VERSION,
	GET_PRODUCT_ID,
	GET_PRODUCT_NAME,
	GET_BASEBOARD_INFO,
	GET_MEMORY_INFO,
	GET_STORAGE_INFO,
	SET_POWER_STATE,
	GET_POWER_STATE,
	COLLECT_LOG_FILES,
	COPY_DATA,
	CHMOD,
	SHUT_DOWN,
	SET_DEVICE_RESOLUTION,
	CONTROL_LED,
	SET_BRIGHTNESS,
	GET_BRIGHTNESS,

	// Network Utils
	GET_MAC_ADDRESS,
	GET_IP_ADDRESS,
	GET_GETWAY_ADDRESS,
	GET_NETMASK,
	GET_DNS1_ADDRESS,
	GET_DNS2_ADDRESS,
	GET_NETCARD_STATE,
	GET_NETCARD_CUR_SPEED,
	GET_NETCARD_MAX_SPEED,
	START_AVAHI,
	STOP_AVAHI,
	CHECK_IP_CONFICT,
	PING,

	// Control Utils
	GET_OTA_RESULT,

	// Application Utils
	IS_RUNNING_PACKAGE,
};


class BpRjcoreService: public BpInterface<IRjcoreService>
{
public:
    BpRjcoreService(const sp<IBinder>& impl)
        : BpInterface<IRjcoreService>(impl)
    {
    }

	// Product Utils
	virtual String16 getCpuInfo() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_CPU_INFO, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getAndroidVersion() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_ANDROID_VERSION, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getSystemVersion() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_SYSTEM_VERSION, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getClientVersion() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_CLIENT_VERSION, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getSerialNumber() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_SERIAL_NUMBER, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getCustProductModel() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_CUST_PRODUCT_MODEL, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getHardwareVesion() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_HARDWARE_VERSION, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getProductID() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_PRODUCT_ID, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getProductName() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_PRODUCT_NAME, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getBaseboardInfo() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_BASEBOARD_INFO, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getMemoryInfo() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_MEMORY_INFO, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getStorageInfo() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_STORAGE_INFO, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual void setPowerState(int powerState) {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeInt32(powerState);
        remote()->transact(SET_POWER_STATE, data, &reply);
		reply.readExceptionCode();
    }

	virtual int getPowerState() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_POWER_STATE, data, &reply);
		reply.readExceptionCode();
        return reply.readInt32();
    }

	virtual void collectLogFiles(String16 zipFilename) {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(zipFilename);
        remote()->transact(COLLECT_LOG_FILES, data, &reply);
		reply.readExceptionCode();
    }

	virtual void copyData(String16 srcPath, String16 dstPath) {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
		data.writeString16(srcPath);
		data.writeString16(dstPath);
        remote()->transact(COPY_DATA, data, &reply);
		reply.readExceptionCode();
    }

	virtual void chmod(String16 path) {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(path);
        remote()->transact(CHMOD, data, &reply);
		reply.readExceptionCode();
    }

	virtual void shutDown() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(SHUT_DOWN, data, &reply);
		reply.readExceptionCode();
    }

	virtual bool setDeviceResolution(int xres, int yres, int refresh, int flag) {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        //data.writeString16(resolution);
	data.writeInt32(xres);
	data.writeInt32(yres);
	data.writeInt32(refresh);
	data.writeInt32(flag);
        remote()->transact(SET_DEVICE_RESOLUTION, data, &reply);
		reply.readExceptionCode();
        return reply.readInt32();
    }

	virtual void controlLed(int color, int state) {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeInt32(color);
		data.writeInt32(state);
        remote()->transact(CONTROL_LED, data, &reply);
		reply.readExceptionCode();
    }

	virtual void setBrightness(int brightness) {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeInt32(brightness);
        remote()->transact(SET_BRIGHTNESS, data, &reply);
		reply.readExceptionCode();
    }

	virtual int getBrightness() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_BRIGHTNESS, data, &reply);
		reply.readExceptionCode();
        return reply.readInt32();
    }

	// Network Utils
	virtual String16 getMacAddress() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_MAC_ADDRESS, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getIPAddress() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_IP_ADDRESS, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getGetwayAddress() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_GETWAY_ADDRESS, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getNetmask() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_NETMASK, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getDns1Address() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_DNS1_ADDRESS, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getDns2Address() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_DNS2_ADDRESS, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual bool getNetcardState() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_NETCARD_STATE, data, &reply);
		reply.readExceptionCode();
        return reply.readInt32();
    }
	
	virtual String16 getNetcardCurSpeed() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_NETCARD_CUR_SPEED, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 getNetcardMaxSpeed() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_NETCARD_MAX_SPEED, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual void startAvahi() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(START_AVAHI, data, &reply);
		reply.readExceptionCode();
    }

	virtual void stopAvahi() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(STOP_AVAHI, data, &reply);
		reply.readExceptionCode();
    }

	virtual String16 checkIpConfict() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(CHECK_IP_CONFICT, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	virtual String16 ping(String16 ip, int count) {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
		data.writeString16(ip);
		data.writeInt32(count);
        remote()->transact(PING, data, &reply);
		reply.readExceptionCode();
        return reply.readString16();
	}

	// Control Utils
	virtual bool getOtaResult() {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_OTA_RESULT, data, &reply);
		reply.readExceptionCode();
        return reply.readInt32();
    }
	
	// Application Utils
	virtual int isRunningPackage(String16 processname) {
		Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(processname);
        remote()->transact(IS_RUNNING_PACKAGE, data, &reply);
		reply.readExceptionCode();
        return reply.readInt32();
    }
};

IMPLEMENT_META_INTERFACE(RjcoreService, "com.ruijie.rjcore.library.interfaces.IRjcoreManager");

// ----------------------------------------------------------------------

status_t BnRjcoreService::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch (code) {
		// Product Utils
		case GET_CPU_INFO: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getCpuInfo();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_ANDROID_VERSION: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getAndroidVersion();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_SYSTEM_VERSION: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getSystemVersion();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_CLIENT_VERSION: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getClientVersion();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_SERIAL_NUMBER: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getSerialNumber();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_CUST_PRODUCT_MODEL: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getCustProductModel();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_HARDWARE_VERSION: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getHardwareVesion();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_PRODUCT_ID: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getProductID();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_PRODUCT_NAME: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getProductName();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_BASEBOARD_INFO: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getBaseboardInfo();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_MEMORY_INFO: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getMemoryInfo();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_STORAGE_INFO: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getStorageInfo();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case SET_POWER_STATE: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            setPowerState(data.readInt32());
			reply->writeNoException();
            return NO_ERROR;
        } break;
		case GET_POWER_STATE: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            int status = getPowerState();
			reply->writeNoException();
            reply->writeInt32(status);
            return NO_ERROR;
        } break;
		case COLLECT_LOG_FILES: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            collectLogFiles(data.readString16());
			reply->writeNoException();
            return NO_ERROR;
        } break;
		case COPY_DATA: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            copyData(data.readString16(), data.readString16());
			reply->writeNoException();
            return NO_ERROR;
        } break;
		case CHMOD: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            chmod(data.readString16());
			reply->writeNoException();
            return NO_ERROR;
        } break;
		case SHUT_DOWN: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            shutDown();
			reply->writeNoException();
            return NO_ERROR;
        } break;
		case SET_DEVICE_RESOLUTION: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            bool status = setDeviceResolution(data.readInt32(),data.readInt32(),data.readInt32(),data.readInt32());
			reply->writeNoException();
            reply->writeInt32(status);
            return NO_ERROR;
        } break;
		case CONTROL_LED: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            controlLed(data.readInt32(), data.readInt32());
			reply->writeNoException();
            return NO_ERROR;
        } break;
		case SET_BRIGHTNESS: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            setBrightness(data.readInt32());
			reply->writeNoException();
            return NO_ERROR;
        } break;
		case GET_BRIGHTNESS: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            int brightness = getBrightness();
			reply->writeNoException();
            reply->writeInt32(brightness);
            return NO_ERROR;
        } break;

		// Network Utils
		case GET_MAC_ADDRESS: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getMacAddress();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_IP_ADDRESS: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getIPAddress();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_GETWAY_ADDRESS: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getGetwayAddress();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_NETMASK: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getNetmask();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_DNS1_ADDRESS: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getDns1Address();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_DNS2_ADDRESS: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getDns2Address();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_NETCARD_STATE: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            bool state = getNetcardState();
			reply->writeNoException();
            reply->writeInt32(state);
            return NO_ERROR;
        } break;
		case GET_NETCARD_CUR_SPEED: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getNetcardCurSpeed();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case GET_NETCARD_MAX_SPEED: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getNetcardMaxSpeed();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case START_AVAHI: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            startAvahi();
			reply->writeNoException();
            return NO_ERROR;
        } break;
		case STOP_AVAHI: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            stopAvahi();
			reply->writeNoException();
            return NO_ERROR;
        } break;
		case CHECK_IP_CONFICT: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = checkIpConfict();
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
		case PING: {
			CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = ping(data.readString16(), data.readInt32());
			reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;

		// Control Utils
		case GET_OTA_RESULT: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            bool status = getOtaResult();
			reply->writeNoException();
            reply->writeInt32(status);
            return NO_ERROR;
        } break;

		// Application Utils
		case IS_RUNNING_PACKAGE: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            int status = isRunningPackage(data.readString16());
			reply->writeNoException();
            reply->writeInt32(status);
            return NO_ERROR;
        } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

// ----------------------------------------------------------------------------

}; // namespace android
