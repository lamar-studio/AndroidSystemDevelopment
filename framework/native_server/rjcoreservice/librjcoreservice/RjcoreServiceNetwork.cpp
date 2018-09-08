
#define LOG_TAG "RjcoreServiceNetwork"
//#define LOG_NDEBUG 0

#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

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

#define CMD_NETCARDCURSPEED    "ethtool eth0 | grep \"Speed:\" | awk '{print $2}'"
#define CMD_STOPAVAHI          "sh /system/bin/shutdown_avahi.sh"

namespace android {

static Mutex sNetLock;

static int check_ipaddr(const char *str)
{
    struct sockaddr_in6 addr6;
    struct sockaddr_in addr4;

    if (str == NULL || *str == '\0') {
        ALOGW("the str is invalid.");
        return 1;
    }

    if (inet_pton(AF_INET, str, &addr4.sin_addr) == 1) {
        return 0;
    } else if (inet_pton(AF_INET6, str, &addr6.sin6_addr) == 1) {
        return 0;
    }

    return 1;
}

// Network Utils
bool RjcoreService::getNetcardState() {
    CHECK_FUNCTION_IN();
    const char *cmd = "ethtool eth0 | grep \"Link detected:\" | awk '{print $3}'";
    char result[SIZE_1K] = {0};

    executeCmd(cmd, result, sizeof(result));
    if (strncmp(result, "yes", strlen("yes")) == 0) {
        return true;
    } else if (strncmp(result, "no", strlen("no")) == 0) {
        return false;
    } else {
        ALOGW("Not found the Link detected");
    }

    return false;
}

String16 RjcoreService::getNetcardCurSpeed() {
    CHECK_FUNCTION_IN();
    int len = 0;
    char result[SIZE_1K] = {0};

    executeCmd(CMD_NETCARDCURSPEED, result, sizeof(result));
    CHECK_STR16_RES(result);

    len = strlen(result);
    if (result[len - 1] == '\n') {
        result[len - 1] = '\0';
    }

    ALOGV("getNetcardCurSpeed :%s", result);

    return String16(result);
}

String16 RjcoreService::getNetcardMaxSpeed() {
    CHECK_FUNCTION_IN();
    const char *cmd = "ethtool eth0 | grep -A2 \"Supported link modes:\"";
    char result[SIZE_1K] = {0};

    executeCmd(cmd, result, sizeof(result));
    if (strstr(result, "1000baseT/Full") != NULL) {
        return String16("1000");
    } else if (strstr(result, "100baseT/Full") != NULL) {
        return String16("100");
    } else if (strstr(result, "10baseT/Full") != NULL) {
        return String16("10");
    } else {
        ALOGW("Not found the Speed keyword");
    }

    return String16("");
}

void RjcoreService::startAvahi() {
    CHECK_FUNCTION_IN();
    const char *cmd_config = "sh /system/bin/config_avahi.sh";
    const char *cmd_kill = "/system/bin/avahi-autoipd --kill eth0";
    const char *cmd_start = "/system/bin/avahi-autoipd --no-drop-root --script=/system/bin/avahi-autoipd_action.sh eth0 &";

    if (access("/data/data/org.freedesktop.telepathy/files", F_OK) != 0
        && access("/system/bin/config_avahi.sh", F_OK) == 0) {
        ALOGW("not found the directory, create it.");
        if (mySystem(cmd_config) < 0) {
            ALOGE("cmd_config execute error!");
            return;
        }
    }

    if (access("/system/bin/avahi-autoipd_action.sh", F_OK) == 0) {
        if (getNetcardState() == true && getProcessId(String16("avahi-autoipd")) != String16("")) {
            if (mySystem(cmd_kill) < 0) {
                ALOGE("cmd_kill execute error!");
                return;
            }
        }

        if (mySystem(cmd_start) < 0) {
            ALOGE("cmd_start execute error!");
            return;
        }

    } else {
        ALOGE("not found the script");
    }

    return;
}

void RjcoreService::stopAvahi() {
    CHECK_FUNCTION_IN();

    if (access("/system/bin/shutdown_avahi.sh", F_OK) == 0) {
        if (mySystem(CMD_STOPAVAHI) < 0) {
            ALOGE("CMD_STOPAVAHI execute error!");
            return;
        }
    } else {
        ALOGE("not found the script");
    }

    return;
}

//time-consuming interface
String16 RjcoreService::checkIpConflict_block(String16 ip) {
    CHECK_FUNCTION_IN();
    int len = 0;
    char cmd[SIZE_1K] = {0};
    char result[SIZE_1K] = {0};

    if (check_ipaddr(String8(ip).string()) != 0) {
        ALOGW("the ip:%s is invalid.", String8(ip).string());
        return String16("");
    }

    if (getNetcardState() == false) {
        ALOGW("Net disable");
        return String16("");
    }

    snprintf(cmd, sizeof(cmd), "checkip %s -s | grep srcMac | awk '{print $2}'", String8(ip).string());
    executeCmd(cmd, result, sizeof(result));         //get the conflict macAddr
    CHECK_STR16_RES(result);

    len = strlen(result);
    if (result[len - 1] == '\n') {
        result[len - 1] = '\0';
    }

    ALOGV("checkIpConflict_block ip:%s res:%s", String8(ip).string(), result);

    return String16(result);
}

//time-consuming interface
String16 RjcoreService::ping_block(String16 ip, int count, int timeout, int deadline) {
    CHECK_FUNCTION_IN();
    int len = 0;
    char cmd[SIZE_1K] = {0};
    char result[SIZE_1K] = {0};

    if (check_ipaddr(String8(ip).string()) != 0) {
        ALOGW("the ip:%s is invalid.", String8(ip).string());
        return String16("");
    }

    if (count <= 0 || timeout < 0 || deadline < 0) {
        ALOGW("ping_block: the parameter is invalid.");
        return String16("");
    }

    snprintf(cmd, sizeof(cmd), "ping -c %d -W %d -w %d %s | grep transmitted",
             count, timeout, deadline, String8(ip).string());

    executeCmd(cmd, result, sizeof(result));
    CHECK_STR16_RES(result);

    len = strlen(result);
    if (result[len - 1] == '\n') {
        result[len - 1] = '\0';
    }

    ALOGV("ping_block cnt:%d timeout:%d deadline:%d the result:%s", count, timeout, deadline, result);

    return String16(result);
}

//time-consuming interface And synchronized
String16 RjcoreService::checkBandwidth_block(String16 serverIp, String16 user, String16 code) {
    CHECK_FUNCTION_IN();
    const char *bandtmpfile = "/data/data/com.ruijie.rccstu/bandtmpfile";
    char cmd_down[SIZE_1K] = {0};
    char cmd_size[SIZE_1K] = {0};
    char cmd_rm[SIZE_1K] = {0};
    char result[SIZE_1K] = {0};
    char speed_str[SIZE_1K_HALF] = {0};
    char buf_size[SIZE_1K_HALF] = {0};
    time_t t_start, t_end;

    Mutex::Autolock _l(sNetLock);
    if (check_ipaddr(String8(serverIp).string()) != 0) {
        ALOGW("the ip:%s is invalid.", String8(serverIp).string());
        return String16("");
    }

    //download the file and keep time
    if (user.size() == 0 && code.size() == 0) {
        snprintf(cmd_down, sizeof(cmd_down), "wget -O %s ftp://%s/pub/speed_file &>/dev/null -T 15",
                 bandtmpfile, String8(serverIp).string());
    } else {
        snprintf(cmd_down, sizeof(cmd_down), "wget -O %s ftp://%s:%s@%s/pub/speed_file &>/dev/null -T 15",
                 bandtmpfile, String8(user).string(), String8(code).string(), String8(serverIp).string());
    }

    t_start = time(NULL);
    executeCmd(cmd_down, result, sizeof(result));      //block
    if (access(bandtmpfile, F_OK) == 0) {
        t_end = time(NULL);

        memset(result, 0, sizeof(result));
        snprintf(cmd_size, sizeof(cmd_size), "busybox du -h %s | awk '{print $1}'", bandtmpfile);
        executeCmd(cmd_size, result, sizeof(result));
        CHECK_STR16_RES(result);
        sscanf(result, "%[^M]", buf_size);

        //calculate the Bandwidth
        snprintf(speed_str, sizeof(speed_str), "%.2fMb/s", (atof(buf_size) / difftime(t_end, t_start)));
        ALOGV("checkBandwidth_block the speed:%s", speed_str);

        //del the temp file
        snprintf(cmd_rm, sizeof(cmd_rm), "rm %s", bandtmpfile);
        if (mySystem(cmd_rm) < 0) {
            ALOGE("del the bandtmpfile error!");
        }

        return String16(speed_str);
    } else {
        ALOGE("connect %s fail", String8(serverIp).string());
    }

    return String16("");
}

} // namespace android
