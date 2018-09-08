
#define LOG_TAG "RjcoreServiceApplication"
// #define LOG_NDEBUG 0

#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>

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
}

namespace android {

// Application Utils
bool RjcoreService::isProcessRunning(String16 processname) {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K] = { 0 };
    char cmd[SIZE_1K_HALF] = { 0 };

    snprintf(cmd, sizeof(cmd), "pidof %s", String8(processname).string());
    executeCmd(cmd, res, sizeof(res));
    if (res[0] != '\0') {
        return true;
    }
    return false;
}

String16 RjcoreService::getProcessId(String16 processname) {
    CHECK_FUNCTION_IN();
    char res[SIZE_1K_HALF] = { 0 };
    char cmd[SIZE_1K_HALF] = { 0 };

    snprintf(cmd, sizeof(cmd), "pidof %s", String8(processname).string());
    executeCmd(cmd, res, sizeof(res));
    CHECK_STR16_RES(res);
    return String16(res);
}

void RjcoreService::killByProcessName(String16 processname) {
    CHECK_FUNCTION_IN();
    char cmd[SIZE_1K_HALF] = { 0 };

    String16 processPid = getProcessId(processname);
    if (processPid == String16("")) {
        ALOGE("the process not exist please check the processname! ");
        return;
    }

    snprintf(cmd, sizeof(cmd), "kill -9 %s", String8(processPid).string());
    if (mySystem(cmd) < 0) {
        ALOGE("killByProcessName execute failed");
        return;
    }
}

}  // namespace android
