LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_ffmpeg_shared
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libffmpeg.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_SHARED_LIBRARY)
