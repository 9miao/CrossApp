LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := CrossApp_cpp_shared

LOCAL_MODULE_FILENAME := libCrossApp_cpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/RootWindow.cpp \
../../Classes/FirstViewController.cpp \

#define all-cpp-files
#$(patsubst jni/%,%, $(shell find $(LOCAL_PATH)/../../Classes/ $(LOCAL_PATH) -name #"*.cpp"))  
#endef
#LOCAL_SRC_FILES := $(call all-cpp-files)
			
			
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes


LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += spidermonkey_static
LOCAL_WHOLE_STATIC_LIBRARIES += scriptingcore-spidermonkey
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CrossApp)
$(call import-module,CrossApp/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,scripting/javascript/spidermonkey-android)
$(call import-module,scripting/javascript/bindings)
