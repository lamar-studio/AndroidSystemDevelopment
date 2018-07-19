
#define LOG_TAG "RjcoreServiceApplication"
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
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

namespace android {

// Application Utils
int RjcoreService::isRunningPackage(String16 processname) {
	ALOGD("isRunningPackage");
	char res[1024];
	char cmd[512];
	char pid[10];
	char* ptr;
	char* pid_ptr;
	int count = 0;
	memset(res,0,sizeof(res));
	memset(cmd,0,sizeof(cmd));
	memset(pid,0,sizeof(pid));
	sprintf(cmd,"ps | grep %s",String8(processname).string());
	executeCmd(cmd,res);
	if(res[0] != '\0') {
		ptr = res;
		pid_ptr = pid;
		
		while(1) {
			if(*ptr == ' ')
				break;
			ptr++;
		}
		
		while(1) {
			if(*ptr > 0x30 && *ptr < 0x39)
				break;
			ptr++;
		}
		
		while(*ptr != ' ')
			*pid_ptr++ = *ptr++;
		return atoi(pid);
	}
	
	return -1;
}


} // namespace android
