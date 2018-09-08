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
    GET_SERIAL_NUMBER,
    GET_PRODUCT_TYPE,
    GET_HARDWARE_VERSION,
    GET_PRODUCT_ID,
    GET_PRODUCT_NAME,
    GET_BASEBOARD_INFO,
    GET_MEMORY_INFO,
    GET_STORAGE_INFO,
    SET_POWER_STATE,
    GET_POWER_STATE,
    COLLECT_LOG_FILES_BLOCK,
    CHMOD_FILE,
    CHMOD_DIR,
    RM_FILE,
    REBOOT,
    SHUTDOWN,
    GET_LED_STATUS,
    SET_LED_STATUS,
    SET_DEVICE_RESOLUTION,
    GET_CURRENT_RESOLUTION_BLOCK,
    GET_SUPPORT_RESOLUTION,
    GET_PROPERTY_VALUE,

    // Network Utils
    GET_NETCARD_STATE,
    GET_NETCARD_CUR_SPEED,
    GET_NETCARD_MAX_SPEED,
    START_AVAHI,
    STOP_AVAHI,
    CHECK_IP_CONFLICT_BLOCK,
    PING_BLOCK,
    CHECK_BANDWIDTH_BLOCK,

    // Control Utils
    GET_OTA_RESULT,

    // Application Utils
    IS_PROCESS_RUNNING,
    GET_PROCESS_ID,
    KILL_PROCESS_BY_NAME,
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

    virtual String16 getSerialNumber() {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_SERIAL_NUMBER, data, &reply);
        reply.readExceptionCode();
        return reply.readString16();
    }

    virtual String16 getProductType() {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_PRODUCT_TYPE, data, &reply);
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

    virtual String16 getProductId() {
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

    virtual bool collectLogFiles_block(String16 zipFilename) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(zipFilename);
        remote()->transact(COLLECT_LOG_FILES_BLOCK, data, &reply);
        reply.readExceptionCode();
        return reply.readInt32();
    }

    virtual bool chmodFile(String16 path, String16 mode) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(path);
        data.writeString16(mode);
        remote()->transact(CHMOD_FILE, data, &reply);
        reply.readExceptionCode();
        return reply.readInt32();
    }

    virtual bool chmodDir(String16 path, String16 mode) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(path);
        data.writeString16(mode);
        remote()->transact(CHMOD_DIR, data, &reply);
        reply.readExceptionCode();
        return reply.readInt32();
    }

    virtual bool rmFile(String16 path) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(path);
        remote()->transact(RM_FILE, data, &reply);
        reply.readExceptionCode();
        return reply.readInt32();
    }

    virtual void reboot() {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(REBOOT, data, &reply);
        reply.readExceptionCode();
    }

    virtual void shutdown() {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(SHUTDOWN, data, &reply);
        reply.readExceptionCode();
    }

    virtual int getLedStatus(int color) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeInt32(color);
        remote()->transact(GET_LED_STATUS, data, &reply);
        reply.readExceptionCode();
        return reply.readInt32();
    }

    virtual void setLedStatus(int color, int state) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeInt32(color);
        data.writeInt32(state);
        remote()->transact(SET_LED_STATUS, data, &reply);
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

    virtual String16 getCurrentResolution_block() {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_CURRENT_RESOLUTION_BLOCK, data, &reply);
        reply.readExceptionCode();
        return reply.readString16();
    }

    virtual String16 getSupportResolution() {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        remote()->transact(GET_SUPPORT_RESOLUTION, data, &reply);
        reply.readExceptionCode();
        return reply.readString16();
    }

    virtual String16 getPropertyValue(String16 key) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(key);
        remote()->transact(GET_PROPERTY_VALUE, data, &reply);
        reply.readExceptionCode();
        return reply.readString16();
    }

    // Network Utils
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

    virtual String16 checkIpConflict_block(String16 ip) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(ip);
        remote()->transact(CHECK_IP_CONFLICT_BLOCK, data, &reply);
        reply.readExceptionCode();
        return reply.readString16();
    }

    virtual String16 ping_block(String16 ip, int count, int timeout, int deadline) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(ip);
        data.writeInt32(count);
        data.writeInt32(timeout);
        data.writeInt32(deadline);
        remote()->transact(PING_BLOCK, data, &reply);
        reply.readExceptionCode();
        return reply.readString16();
    }

    virtual String16 checkBandwidth_block(String16 serverIp, String16 user, String16 code) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(serverIp);
        data.writeString16(user);
        data.writeString16(code);
        remote()->transact(CHECK_BANDWIDTH_BLOCK, data, &reply);
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
    virtual bool isProcessRunning(String16 processname) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(processname);
        remote()->transact(IS_PROCESS_RUNNING, data, &reply);
        reply.readExceptionCode();
        return reply.readInt32();
    }

    virtual String16 getProcessId(String16 processname) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(processname);
        remote()->transact(GET_PROCESS_ID, data, &reply);
        reply.readExceptionCode();
        return reply.readString16();
    }

    virtual void killByProcessName(String16 processname) {
        Parcel data, reply;
        data.writeInterfaceToken(IRjcoreService::getInterfaceDescriptor());
        data.writeString16(processname);
        remote()->transact(KILL_PROCESS_BY_NAME, data, &reply);
        reply.readExceptionCode();
    }
};

IMPLEMENT_META_INTERFACE(RjcoreService, "com.ruijie.core.library.interfaces.IRjcoreManager");

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
        case GET_SERIAL_NUMBER: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getSerialNumber();
            reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
        case GET_PRODUCT_TYPE: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getProductType();
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
            String16 info = getProductId();
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
        case COLLECT_LOG_FILES_BLOCK: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            int status = collectLogFiles_block(data.readString16());
            reply->writeNoException();
            reply->writeInt32(status);
            return NO_ERROR;
        } break;
        case CHMOD_FILE: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            bool status = chmodFile(data.readString16(),data.readString16());
            reply->writeNoException();
            reply->writeInt32(status);
            return NO_ERROR;
        } break;
        case CHMOD_DIR: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            bool status = chmodDir(data.readString16(),data.readString16());
            reply->writeNoException();
            reply->writeInt32(status);
            return NO_ERROR;
        }break;
        case RM_FILE: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            bool status = rmFile(data.readString16());
            reply->writeNoException();
	    reply->writeInt32(status);
            return NO_ERROR;
        }break;
        case SHUTDOWN: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            shutdown();
            reply->writeNoException();
            return NO_ERROR;
        } break;
        case REBOOT: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            reboot();
            reply->writeNoException();
            return NO_ERROR;
        }break;
        case GET_LED_STATUS: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            int status = getLedStatus(data.readInt32());
            reply->writeNoException();
            reply->writeInt32(status);
            return NO_ERROR;
        }break;
        case SET_LED_STATUS: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            setLedStatus(data.readInt32(), data.readInt32());
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
        case GET_CURRENT_RESOLUTION_BLOCK: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getCurrentResolution_block();
            reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
        case GET_SUPPORT_RESOLUTION: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getSupportResolution();
            reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
        case GET_PROPERTY_VALUE: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getPropertyValue(data.readString16());
            reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;

        // Network Utils
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
        case CHECK_IP_CONFLICT_BLOCK: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = checkIpConflict_block(data.readString16());
            reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
        case PING_BLOCK: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = ping_block(data.readString16(), data.readInt32(), data.readInt32(), data.readInt32());
            reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        } break;
        case CHECK_BANDWIDTH_BLOCK: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = checkBandwidth_block(data.readString16(), data.readString16(), data.readString16());
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
        case IS_PROCESS_RUNNING: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            bool status = isProcessRunning(data.readString16());
            reply->writeNoException();
            reply->writeInt32(status);
            return NO_ERROR;
        } break;
        case GET_PROCESS_ID: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            String16 info = getProcessId(data.readString16());
            reply->writeNoException();
            reply->writeString16(info);
            return NO_ERROR;
        }break;
        case KILL_PROCESS_BY_NAME: {
            CHECK_INTERFACE(IRjcoreService, data, reply);
            killByProcessName(data.readString16());
            reply->writeNoException();
            return NO_ERROR;
        }
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

// ----------------------------------------------------------------------------

}; // namespace android
