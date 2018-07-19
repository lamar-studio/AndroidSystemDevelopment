
#define LOG_TAG "RjcoreService"
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
#include <utils/String8.h>
#include <utils/String16.h>
#include <utils/SystemClock.h>
#include <utils/Timers.h>
#include <utils/Vector.h>

#include "RjcoreService.h"


namespace android {

// static variables
RjcoreService* RjcoreService::mInstance = NULL;


void RjcoreService::instantiate() {
    defaultServiceManager()->addService(
            String16("rjcore.service"), new RjcoreService());
}

RjcoreService* RjcoreService::getInstance()
{
    if (mInstance == NULL) {
            mInstance = new RjcoreService();
    }
    return mInstance;
}

RjcoreService::RjcoreService() {
    ALOGV("RjcoreService created");
}

RjcoreService::~RjcoreService() {
    ALOGV("RjcoreService destroyed");
}

void RjcoreService::executeCmd(const char *cmd, char *result)
{
    char buf_ps[1024];
    char ps[1024]={0};
    FILE *ptr;
    strcpy(ps, cmd);
    if((ptr=popen(ps, "r"))!=NULL) {
        while(fgets(buf_ps, 1024, ptr)!=NULL) {
           strcat(result, buf_ps);

           if(strlen(result)>1024)
               break;
        }
        pclose(ptr);
        ptr = NULL;
    }
    else {
        printf("open %s error\n", ps);
    }
}




}; // namespace android
