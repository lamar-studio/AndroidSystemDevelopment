LOCAL_PATH:= $(call my-dir)

# build rjcoreserver's executable
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	main_rjcoreserver.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	liblog \
	libbinder \
	libutils \
	libdl \
	librjcore \
	librjcoreservice

LOCAL_C_INCLUDES := \
    $(TOP)/frameworks/native/services/rjcoreservice/include \
    $(TOP)/frameworks/native/services/rjcoreservice/librjcoreservice

LOCAL_MODULE:= rjcoreserver
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_EXECUTABLES)

include $(BUILD_EXECUTABLE)

