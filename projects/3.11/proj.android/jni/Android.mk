LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/RootViewController.cpp \
                   ../../Classes/ViewController.cpp \
                   ../../Classes/UiControl/LMenu.cpp \
                   ../../Classes/UiControl/LyfTableView.cpp \
                   ../../Classes/UiControl/LyfTableViewCell.cpp \
                   ../../Classes/UiControl/LyfIndexPath.cpp \
                   ../../Classes/UiControl/LyfIndicator.cpp \
                   ../../Classes/UiControl/LyfTextFieldTTF.cpp \
                   ../../Classes/network/SocketClient.cpp \
                   ../../Classes/network/Util.cpp \
                   ../../Classes/network/MessageQueue.cpp \
                   ../../Classes/network/ByteBuffer.cpp \
                   ../../Classes/network/message.cpp \
                   ../../Classes/MainInterface.cpp \
                   ../../Classes/MainInterfaceCell.cpp \
                   ../../Classes/LNoClickLayer.cpp \
                   ../../Classes/TextShow.cpp \
                   ../../Classes/LTMPShow.cpp \
                   ../../Classes/LTableBarController.cpp \
                   ../../Classes/LRootLayer.cpp \
                   ../../Classes/LTabBarMenu.cpp \
                   ../../Classes/MySocket.cpp \
                   ../../Classes/CCScrollViewX.cpp \
                   ../../Classes/LUrlHyperLink.cpp \
                   ../../Classes/LUrlSprite.cpp \
                   ../../Classes/http_download/LHttpDownload.cpp \
                   ../../Classes/http_download/android/FSDirectoryManager.cpp \
                   ../../Classes/url/android/OpenBrowser.cpp \
                   
                   
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/UiControl \
					$(LOCAL_PATH)/../../Classes/network \
					$(LOCAL_PATH)/../../Classes/url/ \
					$(LOCAL_PATH)/../../Classes/url/android/ \
					$(LOCAL_PATH)/../../Classes/http_download/ \
					$(LOCAL_PATH)/../../Classes/http_download/android/ \
					
					

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
