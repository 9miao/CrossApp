LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := uv_static
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libuv_a.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include

include $(PREBUILT_STATIC_LIBRARY)
