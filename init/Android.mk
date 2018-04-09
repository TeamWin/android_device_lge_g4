ifeq ($(TARGET_INIT_VENDOR_LIB),libinit_g4)
    LOCAL_PATH := $(call my-dir)
    include $(CLEAR_VARS)
    LOCAL_MODULE_TAGS := optional
    LOCAL_C_INCLUDES := system/core/init \
    			external/selinux/libselinux/include
    LOCAL_CFLAGS := -Wall

    LOCAL_CFLAGS += -DPLATFORM_SDK_VERSION=$(PLATFORM_SDK_VERSION)

ifeq ($(shell test $(PLATFORM_SDK_VERSION) -gt 25; echo $$?),0)
    LOCAL_WHOLE_STATIC_LIBRARIES += libbase
endif

    LOCAL_SRC_FILES := libinit_g4.cpp
    LOCAL_MODULE := libinit_g4
    include $(BUILD_STATIC_LIBRARY)
endif
