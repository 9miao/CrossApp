LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := scriptingcore-spidermonkey

LOCAL_MODULE_FILENAME := libscriptingcore-spidermonkey

LOCAL_SRC_FILES := ScriptingCore.cpp \
                   CrossApp_specifics.cpp \
                   js_manual_conversions.cpp \
                   js_bindings_system_functions.cpp \
                   js_bindings_system_registration.cpp \
                   js_bindings_core.cpp \
                   js_bindings_opengl.cpp \
                   jsb_opengl_functions.cpp \
                   jsb_opengl_manual.cpp \
                   jsb_opengl_registration.cpp \
                   generated/jsb_CrossApp_auto.cpp \
		   jsb_websocket.cpp \
		   XMLHTTPRequest.cpp \

LOCAL_CFLAGS := -DCOCOS2D_JAVASCRIPT

LOCAL_EXPORT_CFLAGS := -DCOCOS2D_JAVASCRIPT

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/../../../CocosDenshion/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/generated

LOCAL_WHOLE_STATIC_LIBRARIES := spidermonkey_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_extension_static

LOCAL_LDLIBS := -landroid
LOCAL_LDLIBS += -llog

include $(BUILD_STATIC_LIBRARY)

$(call import-module,scripting/javascript/spidermonkey-android)
$(call import-module,CrossApp)
$(call import-module,extensions)

