
#define LOG_TAG "RjcoreService"
//#define LOG_NDEBUG 0

#include <utils/Log.h>

#include <sys/types.h>
#include <sys/wait.h>
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

/* local function */
static bool readNand(FILE *file, int offset, int length, char *res, bool lengthInNand) {
    // static function not to check the parameters
    int len = 0;
    int val = 0;

    if (fseek(file, offset, SEEK_SET) < 0) {
        ALOGE("readNand fseek error line=%d", __LINE__);
        return false;
    }

    len = fread(res, 1, length, file);
    if (len != length) {
        ALOGE("readNand fread error line=%d", __LINE__);
        return false;
    }

    // check the length of info save in nand whether or not
    if (lengthInNand == false) {
        return true;
    }

    val = res[0];
    len = fread(res, 1, val, file);
    if (val != len) {
        ALOGE("readNand fread error");
        return false;
    }
    return true;

}

void executeCmd(const char *cmd, char *result, size_t size) {
    if (cmd == NULL || *cmd == '\0' || result == NULL) {
        ALOGE("the cmd is invalid");
        return;
    }

    char *pBuf = result;
    int len = 0;
    FILE *ptr = NULL;

    if ((ptr = popen(cmd, "r")) != NULL) {
        while (fgets(pBuf, size, ptr) != NULL) {
            if ((len = strlen(pBuf)) == 0) {
                ALOGW("the result is full");
                break;
            }

            size -= len;
            pBuf += len;
        }

        pclose(ptr);
        ptr = NULL;
    } else {
        ALOGE("open %s err:%s", cmd, strerror(errno));
    }
}

void RjcoreService::instantiate() {
    defaultServiceManager()->addService(
            String16("ruijie.core.service"), new RjcoreService());

}

RjcoreService::RjcoreService() : mInit(false) {
    ALOGV("RjcoreService created");
    if (init() != 0) {
        ALOGE("init NandStoreInfo error");
    }
}

RjcoreService::~RjcoreService() {
    ALOGV("RjcoreService destroyed");
}

int RjcoreService::init() {
    CHECK_FUNCTION_IN();

    int ret = 0;

    //startAvahi();
    // get SN,MAC,ID,hardware version,resolution info from nand
    if (getNandStoreInfo() == false) {
        ALOGE("init NandStoreInfo error");
        return -1;
    }
    mInit = true;

    return 0;
}

int RjcoreService::mySystem(const char *cmd) {
    if (cmd == NULL || *cmd == '\0') {
        ALOGW("the cmd is invalid");
        return -1;
    }
    int ret = 0;
    sighandler_t old_handler;

    old_handler = signal(SIGCHLD, SIG_DFL);
    ret = system(cmd);
    signal(SIGCHLD, old_handler);
    if (ret < 0) {
        ALOGE("cmd:%s err:%s", cmd, strerror(errno));
        return ret;
    }

    if (WIFEXITED(ret)) {
        ALOGV("normal termination, exit status = %d", WEXITSTATUS(ret));
    } else if (WIFSIGNALED(ret)) {
        ALOGW("abnormal termination, signal number = %d", WTERMSIG(ret));
    } else if (WIFSTOPPED(ret)) {
        ALOGW("process stopped, signal number = %d", WSTOPSIG(ret));
    } else {
        ALOGW("exit status = %d", WEXITSTATUS(ret));
    }

    return ret;
}

bool RjcoreService::getNandStoreInfo(void) {

    mSerialNum = String16("");
    mHardwareVersion = String16("");
    mProductType = String16("");
    mProductId = String16("");
    char res[SIZE_1K] = { 0 };
    int i = 0;
    unsigned long product_id = 0;
    FILE *file;

    file = fopen(INFO_PATH, "r+");
    if (file == NULL) {
        ALOGE("getNandStoreInfo the %s open failed!", INFO_PATH);
        return false;
    }

    if (readNand(file, 0, SERIAL_NUMBER_LEN, res, true) == false) {  // read the serialNum
        ALOGE("getNandStoreInfo read the seriaNum failed!");
        fclose(file);
        return false;
    }

    mSerialNum = String16(res);  // save the serialNum
    memset(res, 0, sizeof(res));

    if (readNand(file, PRODUCT_TYPE_OFF, 1, res, true) == false) {  //read the product type
        ALOGE("getNandStoreInfo read the product type failed!");
        fclose(file) ;
        return false;
    }

    mProductType = String16(res);  // save the product type
    memset(res, 0, sizeof(res));

    if (readNand(file, HARDWARE_VERSION_OFF, 1, res, true) == false) {  // read the hardwareVsersion
        ALOGE("getNandStoreInfo read the hardware version failed!");
        fclose(file);
        return false;
    }

    mHardwareVersion = String16(res);  // save the hardware vsersion
    memset(res, 0, sizeof(res));

    if (readNand(file, PRODUCT_ID_OFF, PRODUCT_ID_LEN, res, false) == false) {  // read the product id
        ALOGE("getNandStoreInfo read the product id failed!");
        fclose(file);
        return false;
    }

    for (i = 0; i < PRODUCT_ID_LEN; i++) {  // convert
        product_id |= res[i] << (32 - 8 * (i + 1));
    }

    snprintf(res, sizeof(res), "%08lx", product_id);
    mProductId = String16(res);  // save the product id

    fclose(file);

    return true;
}


};  // namespace android
