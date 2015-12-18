LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := CrossApp_freetype_static
LOCAL_MODULE_FILENAME := freetype
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libfreetype.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include/
include $(PREBUILT_STATIC_LIBRARY)
