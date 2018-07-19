
#define LOG_TAG "RjcoreServiceNetwork"
//#define LOG_NDEBUG 0

#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>

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


namespace android {

// Network Utils
String16 RjcoreService::getMacAddress() {
	ALOGD("getMacAddress");
    const char *cmd = "cat /sys/class/net/eth0/address";
    char result[1024] = {0};

    executeCmd(cmd, result);
	ALOGD("getMacAddress res:%s", result);

	return String16(result);
}

String16 RjcoreService::getIPAddress() {
	ALOGD("getIPAddress");

	return String16("");
}

String16 RjcoreService::getGetwayAddress() {
	ALOGD("getGetwayAddress");
    const char *cmd = "ip route | grep ^d | grep 'eth0 '$ | awk '{print $3}'";
    char result[1024] = {0};

    executeCmd(cmd, result);
	ALOGD("getGetwayAddress res:%s", result);

	return String16(result);
}

String16 RjcoreService::getNetmask() {
	ALOGD("getNetmask");
    const char *cmd = "ifconfig eth0";
    char result[1024] = {0};
    char buf_1[1024]  = {0};
    char buf_2[1024]  = {0};

    executeCmd(cmd, result);
    sscanf (result, "eth0:%[^mask]mask %[^flags] ", buf_1, buf_2);
    ALOGD("getNetmask buf_2:%s", buf_2);

	return String16(buf_2);
}

String16 RjcoreService::getDns1Address() {
	ALOGD("getDns1Address");
    char buf[PROPERTY_VALUE_MAX] = {0};

    if (property_get("net.dns2", buf, NULL) > 0) {
        return String16(buf);
    } else {
        ALOGW("Not found the property [net.dns1]");
    }

	return String16("");
}

String16 RjcoreService::getDns2Address() {
	ALOGD("getDns2Address");
    char buf[PROPERTY_VALUE_MAX] = {0};

    if (property_get("net.dns1", buf, NULL) > 0) {
        return String16(buf);
    } else {
        ALOGW("Not found the property [net.dns2]");
    }

	return String16("");
}

bool RjcoreService::getNetcardState() {
	ALOGD("getNetcardState");
    const char *cmd = "ethtool eth0 | grep \"Link detected\" | awk '{print $3}'";
    char result[1024] = {0};

    executeCmd(cmd, result);
    if (strncmp(result, "yes", sizeof("yes"))) {
        return true;
    } else if (strncmp(result, "no", sizeof("no"))) {
        return false;
    } else {
        ALOGD("Not found the Link detected");
    }

	return false;
}

String16 RjcoreService::getNetcardCurSpeed() {

    const char *cmd   = "ethtool eth0 | grep Speed | awk '{print $2}'";
    char result[1024] = {0};

    executeCmd(cmd, result);
	ALOGD("getNetcardCurSpeed :%s", result);

	return String16(result);
}

String16 RjcoreService::getNetcardMaxSpeed() {
	ALOGD("getNetcardMaxSpeed");
    const char *cmd  = "ethtool eth0 | grep -A2 \"Supported link modes\"";
    char result[1024] = {0};

    executeCmd(cmd, result);
    if (strstr(result, "1000baseT/Full") != NULL) {
        return String16("1000");
    } else if (strstr(result, "100baseT/Full") != NULL) {
        return String16("100");
    } else if (strstr(result, "10baseT/Full") != NULL) {
        return String16("10");
    } else {
        ALOGD("Not found the Speed keyword");
    }

	return String16("");
}

void RjcoreService::startAvahi() {
	ALOGD("startAvahi");
    const char *cmd_1    = "sh /data/data/com.ruijie.rccstu/files/avahi/config_avahi.sh";
    const char *cmd_2    = "/system/bin/avahi-autoipd --kill eth0";
    const char *cmd_3    = "/system/bin/avahi-autoipd --no-drop-root "
                           "--script=/data/data/com.ruijie.rccstu/files/avahi/avahi-autoipd.action eth0 &";
    char result[1024]    = {0};

    if (access("/data/data/org.freedesktop.telepathy/files", 0) != 0) {
        ALOGW("not found the directory, create it.");
        system(cmd_1);
    }

    if (access("/data/data/com.ruijie.rccstu/files/avahi/config_avahi.sh", 0) == 0
        && access("/data/data/com.ruijie.rccstu/files/avahi/avahi-autoipd.action", 0) == 0) {
        system(cmd_2);
        system(cmd_3);
    } else {
        ALOGE("not found the script");
    }

    return ;
}

void RjcoreService::stopAvahi() {
	ALOGD("stopAvahi");
    const char *cmd   = "sh /system/bin/shutdown_avahi.sh";
    char result[1024] = {0};

    if (access("/system/bin/shutdown_avahi.sh", 0) == 0) {
        system(cmd);
    } else {
        ALOGE("not found the script");
    }

    return ;
}


/**
 * 本地ip冲突检测
 * 冲突时 - 返回冲突冲突机器的mac地址
 * 无冲突 - 返回全0的mac地址
 *
 * 注意事项:该接口耗时较长
 */
String16 RjcoreService::checkIpConfict() {
	ALOGD("checkIpConfict");
    const char *cmd_1  = "busybox ifconfig eth0 | grep \"inet addr\" | awk '{print $2}' | cut -c 6-";
    char cmd_2[1024]   = {0};
    char result[1024]  = {0};

    executeCmd(cmd_1, result);        //get the local ip
    result[strlen(result) - 1] = 0;   //remove the special char
    snprintf(cmd_2, sizeof(cmd_2), "checkip %s -s | grep srcMac | awk '{print $2}'", result);

    memset(result, 0, sizeof(result));
    executeCmd(cmd_2, result);        //get the conflict macAddr
	ALOGD("checkIpConfict res:%s", result);

	return String16(result);
}

String16 RjcoreService::ping(String16 ip, int count) {
	ALOGD("ping");
    char cmd[1024]    = {0};
    char result[1024] = {0};

    snprintf(cmd, sizeof(cmd), "ping -c %d %s | grep transmitted", count, String8(ip).string());

    executeCmd(cmd, result);
	ALOGD("ping the result:%s", result);

	return String16(result);
}


} // namespace android
