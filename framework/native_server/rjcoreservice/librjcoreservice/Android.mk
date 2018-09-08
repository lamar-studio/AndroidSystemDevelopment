LOCAL_PATH:= $(call my-dir)

#
# librjcoreservice
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=  \
    RjcoreService.cpp \
    RjcoreServiceProduct.cpp \
    RjcoreServiceNetwork.cpp \
    RjcoreServiceControl.cpp \
    RjcoreServiceApplication.cpp

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libcrypto \
    libcutils \
    liblog \
    libdl \
    libutils \
    librjcore

LOCAL_C_INCLUDES += \
    $(TOP)/frameworks/native/services/rjcoreservice/include

LOCAL_MODULE:= librjcoreservice

include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))
