LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := scriptingcore-spidermonkey

LOCAL_MODULE_FILENAME := libscriptingcore-spidermonkey



LOCAL_CFLAGS := -DCOCOS2D_JAVASCRIPT

LOCAL_EXPORT_CFLAGS := -DCOCOS2D_JAVASCRIPT

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
										$(LOCAL_PATH)/auto \
										$(LOCAL_PATH)/manual \
										$(LOCAL_PATH)/../../CrossApp/platform \
										$(LOCAL_PATH)/../../CrossApp/support \
										$(LOCAL_PATH)/../../CrossApp/basics \
										$(LOCAL_PATH)/../../CrossApp/cocoa \
										$(LOCAL_PATH)/../../CrossApp/control \
										$(LOCAL_PATH)/../../CrossApp/images \
										$(LOCAL_PATH)/../../CrossApp/view

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \

LOCAL_WHOLE_STATIC_LIBRARIES := spidermonkey_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_static

LOCAL_LDLIBS := -landroid
LOCAL_LDLIBS += -llog

include $(BUILD_STATIC_LIBRARY)

$(call import-module,scripting/js-bindings/spidermonkey/prebuilt/android)
