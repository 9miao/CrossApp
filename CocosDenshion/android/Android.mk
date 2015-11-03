LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cocosdenshion_static

LOCAL_MODULE_FILENAME := libcocosdenshion

LOCAL_SRC_FILES := SimpleAudioEngine.cpp \
                   jni/SimpleAudioEngineJni.cpp \
                   opensl/OpenSLEngine.cpp \
                   opensl/SimpleAudioEngineOpenSL.cpp

LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_curl_static

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include \
                    $(LOCAL_PATH)/../../CrossApp \
                    $(LOCAL_PATH)/../../CrossApp/kazmath/include \
                    $(LOCAL_PATH)/../../CrossApp/platform/android \
                    $(LOCAL_PATH)/../../CrossApp/the_third_party/freetype/include\

LOCAL_CFLAGS += -Wno-psabi
LOCAL_EXPORT_CFLAGS += -Wno-psabi

include $(BUILD_STATIC_LIBRARY)
