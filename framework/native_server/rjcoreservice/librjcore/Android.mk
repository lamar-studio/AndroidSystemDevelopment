LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    IRjcoreService.cpp

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libcrypto \
    libcutils \
    liblog \
    libdl \
    libutils \
    libandroid_runtime \
    libnativehelper


LOCAL_CFLAGS +=

LOCAL_MODULE:= librjcore

LOCAL_C_INCLUDES := \
    $(TOP)/frameworks/base/core/jni \
    $(TOP)/frameworks/native/services/rjcoreservice/include \
    $(JNI_H_INCLUDE)

include $(BUILD_SHARED_LIBRARY)

