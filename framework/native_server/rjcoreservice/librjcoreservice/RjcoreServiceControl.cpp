
#define LOG_TAG "RjcoreServiceControl"
//#define LOG_NDEBUG 0

#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>

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

// Control Utils
bool RjcoreService::getOtaResult() {
	ALOGD("getOtaResult");
    const char *cmd    = "cat /sdcard/com.ruijie.rccstu/config.ini | grep updateFlag";
    char result[1024]  = {0};
    char ota_res[1024] = {0};

    executeCmd(cmd, result);
    if (result[0] == '\0') {
        ALOGD("this machine nonexecution ota update.");
    } else {
        sscanf(result, "updateFlag=%s", ota_res);
        return (strncmp(ota_res, "UPDATE_SUCCESS", sizeof("UPDATE_SUCCESS")) == 0) ? true : false;
    }

	return false;
}


} // namespace android
