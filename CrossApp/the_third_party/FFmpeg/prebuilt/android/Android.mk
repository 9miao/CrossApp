LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := CrossApp_avcodec_static
LOCAL_MODULE_FILENAME := avcode
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libavcodec.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := CrossApp_avformat_static
LOCAL_MODULE_FILENAME := avformat
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libavformat.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := CrossApp_avutil_static
LOCAL_MODULE_FILENAME := avutil
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libavutil.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := CrossApp_swresample_static
LOCAL_MODULE_FILENAME := swresample
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libswresample.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := CrossApp_swscale_static
LOCAL_MODULE_FILENAME := swscale
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libswscale.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)
