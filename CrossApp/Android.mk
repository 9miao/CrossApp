LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := CrossApp_static

LOCAL_MODULE_FILENAME := libCrossApp

LOCAL_SRC_FILES := \
CrossApp.cpp \
animation/CAViewAnimation.cpp \
animation/CAAnimation.cpp \
basics/CAApplication.cpp \
basics/CAAutoreleasePool.cpp \
basics/CACamera.cpp \
basics/CAGeometry.cpp \
basics/CAObject.cpp \
basics/CAResponder.cpp \
basics/CAScheduler.cpp \
basics/CAFPSImages.c \
basics/CAIndexPath.cpp \
basics/CAThread.cpp \
cocoa/CCNS.cpp \
cocoa/CCSet.cpp \
cocoa/CACalendar.cpp \
control/CABar.cpp \
control/CAButton.cpp \
control/CAControl.cpp \
control/CAProgress.cpp \
control/CASegmentedControl.cpp \
control/CASlider.cpp \
control/CASwitch.cpp \
control/CAPageControl.cpp \
control/CAStepper.cpp \
controller/CABarItem.cpp \
controller/CAViewController.cpp \
controller/CADrawerController.cpp \
dispatcher/CAIMEDispatcher.cpp \
dispatcher/CAKeypadDelegate.cpp \
dispatcher/CAKeypadDispatcher.cpp \
dispatcher/CATouch.cpp \
dispatcher/CATouchDispatcher.cpp \
images/CAImage.cpp \
images/CAImageCache.cpp \
images/CAGif.cpp \
images/gif_lib/dgif_lib.c \
images/gif_lib/gif_err.c \
images/gif_lib/gif_font.c \
images/gif_lib/gif_hash.c \
images/gif_lib/gifalloc.c \
images/gif_lib/quantize.c \
shaders/CATransformation.cpp \
shaders/CAGLProgram.cpp \
shaders/CAShaderCache.cpp \
shaders/ccGLStateCache.cpp \
shaders/ccShaders.cpp \
support/md5.cpp \
support/CANotificationCenter.cpp \
support/CCPointExtension.cpp \
support/CCProfiling.cpp \
support/CCVertex.cpp \
support/TransformUtils.cpp \
support/base64.cpp \
support/ccUTF8.cpp \
support/ConvertUTF.cpp \
support/ConvertUTFWrapper.cpp \
support/ccUtils.cpp \
support/data_support/ccCArray.cpp \
support/image_support/TGAlib.cpp \
support/tinyxml2/tinyxml2.cpp \
support/user_default/CAUserDefault.cpp \
support/user_default/CAUserDefaultAndroid.cpp \
support/zip_support/ZipUtils.cpp \
support/zip_support/ioapi.cpp \
support/zip_support/unzip.cpp \
support/network/HttpClient.cpp \
support/network/DownloadManager.cpp \
support/network/WebSocket.cpp \
support/LocalStorage/LocalStorageAndroid.cpp \
support/Json/CSContentJsonDictionary.cpp \
support/Json/lib_json/json_value.cpp \
support/Json/lib_json/json_reader.cpp \
support/Json/lib_json/json_writer.cpp \
support/sqlite3/include/sqlite3.c \
support/device/Device_android/CADevice.cpp \
view/CABatchView.cpp \
view/CAClippingView.cpp \
view/CAImageView.cpp \
view/CALabel.cpp \
view/CARenderImage.cpp \
view/CAScale9ImageView.cpp \
view/CAScrollView.cpp \
view/CAListView.cpp \
view/CATableView.cpp \
view/CACollectionView.cpp \
view/CAAutoCollectionView.cpp \
view/CAView.cpp \
view/CAWindow.cpp \
view/CAAlertView.cpp \
view/CAPageView.cpp \
view/CAPickerView.cpp \
view/CADatePickerView.cpp \
view/CAActivityIndicatorView.cpp \
view/CAPullToRefreshView.cpp \
view/CATextView.cpp \
view/CATextField.cpp \
view/CATextEditHelper.cpp \
view/CALabelStyle.cpp \
view/CAWebView.cpp \
view/CADrawView.cpp \
view/CADrawingPrimitives.cpp \
view/CAGifView.cpp \
kazmath/src/aabb.c \
kazmath/src/mat3.c \
kazmath/src/mat4.c \
kazmath/src/neon_matrix_impl.c \
kazmath/src/plane.c \
kazmath/src/quaternion.c \
kazmath/src/ray2.c \
kazmath/src/utility.c \
kazmath/src/vec2.c \
kazmath/src/vec3.c \
kazmath/src/vec4.c \
kazmath/src/GL/mat4stack.c \
kazmath/src/GL/matrix.c \
platform/CCSAXParser.cpp \
platform/CCFileUtils.cpp \
platform/platform.cpp \
platform/CAFreeTypeFont.cpp \
platform/CAFTFontCache.cpp \
platform/CCEGLViewProtocol.cpp \
platform/CATempTypeFont.cpp \
platform/android/CADensityDpi.cpp \
platform/android/CCEGLView.cpp \
platform/android/CCAccelerometer.cpp \
platform/android/CCApplication.cpp \
platform/android/CCCommon.cpp \
platform/android/CCFileUtilsAndroid.cpp \
platform/android/CAWebViewImpl.cpp \
platform/android/CAClipboard.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxRenderer.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxAccelerometer.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxSDL.cpp \
platform/android/jni/JniHelper.cpp \
platform/android/jni/IMEJni.cpp \
platform/android/jni/TouchesJni.cpp \
script_support/JSViewController.cpp \
script_support/CCScriptSupport.cpp \



LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/kazmath/include \
                    $(LOCAL_PATH)/platform/android \
                    $(LOCAL_PATH)/support/network \
                    $(LOCAL_PATH)/support/LocalStorage \
                    $(LOCAL_PATH)/support/Json \
                    $(LOCAL_PATH)/support/Json/lib_json \

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/kazmath/include \
                    $(LOCAL_PATH)/platform/android \
                    $(LOCAL_PATH)/support/network \
                    $(LOCAL_PATH)/support/LocalStorage \
                    $(LOCAL_PATH)/support/Json \
                    $(LOCAL_PATH)/support/Json/lib_json \

LOCAL_LDLIBS := -lGLESv2 \
                -llog \
                -lz

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -llog \
                       -lz

LOCAL_WHOLE_STATIC_LIBRARIES += cocos_jpeg_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_png_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_tiff_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_webp_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_freetype_static
LOCAL_WHOLE_STATIC_LIBRARIES += libwebsockets_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static


ifeq ($(VIDEO_ENABLE),1)
LOCAL_SRC_FILES += video/CAVideoPlayerController.cpp \
                   video/CAVideoPlayerDecoder.cpp \
                   video/CAVideoPlayerRender.cpp \
                   video/CAVideoPlayerView.cpp

LOCAL_WHOLE_STATIC_LIBRARIES += cocos_avcodec_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_avformat_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_avresample_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_avutil_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_swresample_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_swscale_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ass_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_vo-aacenc_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_vo-amrwbenc_static
endif

LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libsdl_static

# define the macro to compile through support/zip_support/ioapi.c
LOCAL_CFLAGS := -Wno-psabi -DUSE_FILE32API -D__STDC_CONSTANT_MACROS -fexceptions
LOCAL_EXPORT_CFLAGS := -Wno-psabi -DUSE_FILE32API -D__STDC_CONSTANT_MACROS


include $(BUILD_STATIC_LIBRARY)


$(call import-module,jpeg/prebuilt/android)
$(call import-module,png/prebuilt/android)
$(call import-module,tiff/prebuilt/android)
$(call import-module,webp/prebuilt/android)
$(call import-module,freetype/prebuilt/android)
$(call import-module,SDL/prebuilt/android)
$(call import-module,FFmpeg/prebuilt/android)
$(call import-module,CrossApp/the_third_party/curl/prebuilt/android)
$(call import-module,CrossApp/support/libwebsockets/android)

