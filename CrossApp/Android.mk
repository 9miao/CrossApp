LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := CrossApp_static

LOCAL_MODULE_FILENAME := libCrossApp


LOCAL_SRC_FILES := \
CrossApp.cpp \
video/CAVideoPlayerController.cpp \
video/CAVideoPlayerDecoder.cpp \
video/CAVideoPlayerRender.cpp \
video/CAVideoPlayerView.cpp \
actions/CCAction.cpp \
actions/CCActionCamera.cpp \
actions/CCActionEase.cpp \
actions/CCActionInstant.cpp \
actions/CCActionInterval.cpp \
actions/CCActionManager.cpp \
actions/CCActionTween.cpp \
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
cocoa/CCArray.cpp \
cocoa/CCDictionary.cpp \
cocoa/CCNS.cpp \
cocoa/CCSet.cpp \
cocoa/CCString.cpp \
cocoa/CACalendar.cpp \
control/CABar.cpp \
control/CAButton.cpp \
control/CAControl.cpp \
control/CAProgress.cpp \
control/CASegmentedControl.cpp \
control/CASlider.cpp \
control/CASwitch.cpp \
control/CATextField.cpp \
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
draw_nodes/CCDrawNode.cpp \
draw_nodes/CCDrawingPrimitives.cpp \
images/CAImage.cpp \
images/CAImageCache.cpp \
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
view/CAView.cpp \
view/CAWindow.cpp \
view/CAAlertView.cpp \
view/CAPageView.cpp \
view/CAPickerView.cpp \
view/CADatePickerView.cpp \
view/CAActivityIndicatorView.cpp \
view/CAPullToRefreshView.cpp \
view/CATextView.cpp \
view/CALabelStyle.cpp \
view/CAWebView.cpp \
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
platform/CCImageCommonWebp.cpp \
platform/CCSAXParser.cpp \
platform/CCFileUtils.cpp \
platform/platform.cpp \
platform/CAFreeTypeFont.cpp \
platform/CAFTFontCache.cpp \
platform/CCEGLViewProtocol.cpp \
platform/android/CADensityDpi.cpp \
platform/android/CCDevice.cpp \
platform/android/CCEGLView.cpp \
platform/android/CCAccelerometer.cpp \
platform/android/CCApplication.cpp \
platform/android/CCCommon.cpp \
platform/android/CCFileUtilsAndroid.cpp \
platform/android/CCImage.cpp \
platform/android/CAWebViewImpl.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxBitmap.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxRenderer.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxAccelerometer.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxSDL.cpp \
platform/android/jni/JniHelper.cpp \
platform/android/jni/IMEJni.cpp \
platform/android/jni/TouchesJni.cpp \
platform/android/jni/DPIJni.cpp \
script_support/JSViewController.cpp \
script_support/CCScriptSupport.cpp \


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/kazmath/include \
                    $(LOCAL_PATH)/platform/android \

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/kazmath/include \
                    $(LOCAL_PATH)/platform/android \

LOCAL_LDLIBS := -lGLESv2 \
                -llog \
                -lz

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -llog \
                       -lz

LOCAL_WHOLE_STATIC_LIBRARIES := cocos_libpng_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_jpeg_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libxml2_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libtiff_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libwebp_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_freetype2_static

# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ass_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_avcodec_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_avformat_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_avresample_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_avutil_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_swresample_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_swscale_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_vo-aacenc_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_vo-amrwbenc_static

LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libsdl_static

# define the macro to compile through support/zip_support/ioapi.c
LOCAL_CFLAGS := -Wno-psabi -DUSE_FILE32API -D__STDC_CONSTANT_MACROS
LOCAL_EXPORT_CFLAGS := -Wno-psabi -DUSE_FILE32API -D__STDC_CONSTANT_MACROS

include $(BUILD_STATIC_LIBRARY)

$(call import-module,libjpeg)
$(call import-module,libpng)
$(call import-module,libtiff)
$(call import-module,libwebp)
$(call import-module,libfreetype2)
$(call import-module,libffmpeg)
$(call import-module,libSDL)


