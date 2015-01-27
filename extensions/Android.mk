LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := CrossApp_extension_static

LOCAL_MODULE_FILENAME := libextension

LOCAL_SRC_FILES := GUI/CAUIHelper.cpp \
network/HttpClient.cpp \
network/DownloadManager.cpp \
network/WebSocket.cpp \
LocalStorage/LocalStorageAndroid.cpp \
Json/DictionaryHelper.cpp \
Json/CSContentJsonDictionary.cpp \
Json/lib_json/json_value.cpp \
Json/lib_json/json_reader.cpp \
Json/lib_json/json_writer.cpp \
sqlite3/include/sqlite3.c \
device/Device_android/CADevice.cpp \
studio/CAStudioViewController.cpp \
GUI/CASyncImageView.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := CrossApp_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
LOCAL_WHOLE_STATIC_LIBRARIES += libwebsockets_static

LOCAL_CFLAGS += -DCC_ENABLE_CHIPMUNK_INTEGRATION=1
LOCAL_EXPORT_CFLAGS += -DCC_ENABLE_CHIPMUNK_INTEGRATION=1
LOCAL_CPPFLAGS += -DCC_ENABLE_CHIPMUNK_INTEGRATION=1
LOCAL_EXPORT_CPPFLAGS += -DCC_ENABLE_CHIPMUNK_INTEGRATION=1

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/network \
                           $(LOCAL_PATH)/LocalStorage \
						   $(LOCAL_PATH)/Json \
						   $(LOCAL_PATH)/Json/lib_json \
						   $(LOCAL_PATH)/GUI



LOCAL_CFLAGS := -fexceptions
                    
include $(BUILD_STATIC_LIBRARY)

$(call import-module,CrossApp)
$(call import-module,CocosDenshion/android)
$(call import-module,CrossApp/platform/third_party/android/prebuilt/libcurl)
$(call import-module,extensions/libwebsockets/android)
