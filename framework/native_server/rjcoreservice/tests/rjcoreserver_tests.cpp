#define LOG_NDEBUG 0
#define LOG_TAG "rjcoreserver_tests"
#include "utils/Log.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <utils/threads.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include <utils/Errors.h>  // for status_t
#include <utils/KeyedVector.h>

#include "RjcoreService.h"


using namespace android;


int main() {
	printf("\n=====interface test=====\n");

    String16 result = RjcoreService::getInstance()->getMacAddress();
	printf("main getMacAddress result = %s \n", String8(result).string());

    result = RjcoreService::getInstance()->getGetwayAddress();
	printf("main getGetwayAddress result = %s \n", String8(result).string());

    result = RjcoreService::getInstance()->getNetmask();
	printf("main getNetmask result = %s \n", String8(result).string());

    result = RjcoreService::getInstance()->getDns1Address();
	printf("main getDns1Address result = %s \n", String8(result).string());

    result = RjcoreService::getInstance()->getDns2Address();
	printf("main getDns2Address result = %s \n", String8(result).string());

    result = RjcoreService::getInstance()->getNetcardCurSpeed();
	printf("main getNetcardCurSpeed result = %s \n", String8(result).string());

    result = RjcoreService::getInstance()->getNetcardMaxSpeed();
	printf("main getNetcardMaxSpeed result = %s \n", String8(result).string());

    result = RjcoreService::getInstance()->checkIpConfict();
	printf("main checkIpConfict result = %s \n", String8(result).string());

    String16 ip("www.baidu.com");
    result = RjcoreService::getInstance()->ping(ip, 5);
	printf("main ping result = %s \n", String8(result).string());

    bool state = RjcoreService::getInstance()->getNetcardState();
	printf("main getNetcardState result = %d \n", state);

    state = RjcoreService::getInstance()->getOtaResult();
	printf("main getOtaResult result = %d \n", state);

	printf("main startAvahi\n");
    RjcoreService::getInstance()->startAvahi();
	printf("main stopAvahi\n");
    RjcoreService::getInstance()->stopAvahi();

	printf("\n====test interface end====\n");
	return 0;
}


