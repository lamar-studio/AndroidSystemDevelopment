# Build the unit tests for audio_utils

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SHARED_LIBRARIES := \
	libcrypto \
	libcutils \
	liblog \
	libdl \
	libutils \
	librjcore \
	librjcoreservice\
    	libbinder 


LOCAL_C_INCLUDES := \
	$(TOP)/frameworks/native/services/rjcoreservice/include \
	$(TOP)/frameworks/native/services/rjcoreservice/librjcoreservice

LOCAL_SRC_FILES := \
	rjcoreserver_tests.cpp

LOCAL_CFLAGS := -Wall -Werror

LOCAL_MODULE := rjcoreserver_tests
LOCAL_MODULE_TAGS := tests

include $(BUILD_EXECUTABLE)
