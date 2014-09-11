LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cocosdenshion_static

LOCAL_MODULE_FILENAME := libcocosdenshion

LOCAL_SRC_FILES := SimpleAudioEngine.cpp \
                   jni/SimpleAudioEngineJni.cpp \
                   opensl/OpenSLEngine.cpp \
                   opensl/SimpleAudioEngineOpenSL.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include \
                    $(LOCAL_PATH)/../../CrossApp \
                    $(LOCAL_PATH)/../../CrossApp/kazmath/include \
                    $(LOCAL_PATH)/../../CrossApp/platform/android \
                    $(LOCAL_PATH)/../../CrossApp/platform/third_party/android/prebuilt/libfreetype/include \
		    $(LOCAL_PATH)/../../CrossApp/platform/third_party/android/prebuilt/libfreetype/include/freetype2 \

LOCAL_CFLAGS += -Wno-psabi
LOCAL_EXPORT_CFLAGS += -Wno-psabi

include $(BUILD_STATIC_LIBRARY)
