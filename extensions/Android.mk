LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cocos_extension_static

LOCAL_MODULE_FILENAME := libextension

LOCAL_SRC_FILES := AssetsManager/AssetsManager.cpp \
GUI/CAUIHelper.cpp \
network/CAHttpClient.cpp \
network/HttpClient.cpp \
network/WebSocket.cpp \
LocalStorage/LocalStorageAndroid.cpp \
Json/DictionaryHelper.cpp \
Json/CSContentJsonDictionary.cpp \
Json/lib_json/json_value.cpp \
Json/lib_json/json_reader.cpp \
Json/lib_json/json_writer.cpp \
other/CAAdress/android/CAAddressManager.cpp \
other/CACamera/android/CAMediaManager.cpp \
other/CALocation/android/CALocationManager.cpp \

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
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



LOCAL_CFLAGS := -fexceptions
                    
include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,external/libwebsockets/android)
