LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := CrossApp_cpp_shared

LOCAL_MODULE_FILENAME := libCrossApp_cpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/MainMenu.cpp \
../../Classes/ViewController/FifthViewController.cpp \
../../Classes/ViewController/FirstViewController.cpp \
../../Classes/ViewController/FourthViewController.cpp \
../../Classes/ViewController/SecondViewController.cpp \
../../Classes/ViewController/ThirdViewController.cpp \
../../Classes/ActivityIndicatorViewTest/ActivityIndicatorViewTest.cpp \
../../Classes/AlertViewTest/AlertViewTest.cpp \
../../Classes/ButtonTest/ButtonTest.cpp \
../../Classes/CollectionViewTest/CollectionViewTest.cpp \
../../Classes/ImageViewTest/ImageViewTest.cpp \
../../Classes/LabelTest/LabelTest.cpp \
../../Classes/ListViewTest/ListViewTest.cpp \
../../Classes/PageViewTest/PageViewTest.cpp \
../../Classes/ProgressTest/ProgressTest.cpp \
../../Classes/ScrollViewTest/ScrollViewTest.cpp \
../../Classes/SegmentedControlTest/SegmentedControlTest.cpp \
../../Classes/SliderTest/SliderTest.cpp \
../../Classes/SwitchTest/SwitchTest.cpp \
../../Classes/TabBarTest/TabBarTest.cpp \
../../Classes/TableViewTest/TableViewTest.cpp \
../../Classes/TableViewTest/MyTableViewCell.cpp \
../../Classes/TextFieldTest/TextFieldTest.cpp \
../../Classes/ExtensionsTest/ExtensionsTest.cpp \
../../Classes/ExtensionsTest/AddressBookTest.cpp \
../../Classes/ExtensionsTest/HttpRequestTest.cpp \	

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../Classes/ActivityIndicatorViewTest \
$(LOCAL_PATH)/../../Classes/AlertViewTest \
$(LOCAL_PATH)/../../Classes/ButtonTest \
$(LOCAL_PATH)/../../Classes/CollectionViewTest \
$(LOCAL_PATH)/../../Classes/ImageViewTest \
$(LOCAL_PATH)/../../Classes/LabelTest \
$(LOCAL_PATH)/../../Classes/ListViewTest \
$(LOCAL_PATH)/../../Classes/PageViewTest \
$(LOCAL_PATH)/../../Classes/ProgressTest \
$(LOCAL_PATH)/../../Classes/ScrollViewTest \
$(LOCAL_PATH)/../../Classes/SegmentedControlTest \
$(LOCAL_PATH)/../../Classes/SliderTest \
$(LOCAL_PATH)/../../Classes/SwitchTest \
$(LOCAL_PATH)/../../Classes/TabBarTest \
$(LOCAL_PATH)/../../Classes/TableViewTest \
$(LOCAL_PATH)/../../Classes/TextFieldTest \
$(LOCAL_PATH)/../../Classes/ExtensionsTest \
$(LOCAL_PATH)/../../Classes/ViewController \

LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,D:/CrossApp)
$(call import-add-path,D:/CrossApp/CrossApp/platform/third_party/android/prebuilt)

$(call import-module,CrossApp)
$(call import-module,CrossApp/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
