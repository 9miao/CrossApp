LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_libsdl_static
LOCAL_MODULE_FILENAME := SDL
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libSDL2.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include/android
include $(PREBUILT_STATIC_LIBRARY)
