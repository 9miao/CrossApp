LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dx_static

LOCAL_MODULE_FILENAME := libcocos2d

LOCAL_SRC_FILES := \
CCConfiguration.cpp \
CAScheduler.cpp \
CCCamera.cpp \
ccFPSImages.c \
actions/CCAction.cpp \
actions/CCActionCamera.cpp \
actions/CCActionEase.cpp \
actions/CCActionGrid.cpp \
actions/CCActionGrid3D.cpp \
actions/CCActionInstant.cpp \
actions/CCActionInterval.cpp \
actions/CCActionManager.cpp \
actions/CCActionPageTurn3D.cpp \
actions/CCActionProgressTimer.cpp \
actions/CCActionTiledGrid.cpp \
actions/CCActionTween.cpp \
cocoa/CCAffineTransform.cpp \
cocoa/CCGeometry.cpp \
cocoa/CCAutoreleasePool.cpp \
cocoa/CCDictionary.cpp \
cocoa/CCNS.cpp \
cocoa/CCObject.cpp \
cocoa/CCSet.cpp \
cocoa/CCString.cpp \
cocoa/CCZone.cpp \
cocoa/CCArray.cpp \
cocoa/CCDataVisitor.cpp \
cocoa/CAResponder.cpp \
cocos2d.cpp \
CCDirector.cpp \
draw_nodes/CCDrawingPrimitives.cpp \
draw_nodes/CCDrawNode.cpp \
effects/CCGrabber.cpp \
effects/CCGrid.cpp \
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
keypad_dispatcher/CCKeypadDelegate.cpp \
keypad_dispatcher/CCKeypadDispatcher.cpp \
label_nodes/CCLabelTTF.cpp \
label_nodes/CALabel.cpp \
misc_nodes/CCClippingNode.cpp \
misc_nodes/CCMotionStreak.cpp \
misc_nodes/CCProgressTimer.cpp \
misc_nodes/CCRenderTexture.cpp \
platform/CCImageCommonWebp.cpp \
platform/CCSAXParser.cpp \
platform/CCThread.cpp \
platform/CCFileUtils.cpp \
platform/platform.cpp \
platform/CCEGLViewProtocol.cpp \
platform/android/CCDevice.cpp \
platform/android/CCEGLView.cpp \
platform/android/CCAccelerometer.cpp \
platform/android/CCApplication.cpp \
platform/android/CCCommon.cpp \
platform/android/CCFileUtilsAndroid.cpp \
platform/android/CCImage.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxBitmap.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxRenderer.cpp \
platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxAccelerometer.cpp \
platform/android/jni/JniHelper.cpp \
platform/android/jni/IMEJni.cpp \
platform/android/jni/TouchesJni.cpp \
platform/android/jni/DPIJni.cpp \
shaders/ccShaders.cpp \
shaders/CCGLProgram.cpp \
shaders/ccGLStateCache.cpp \
shaders/CCShaderCache.cpp \
sprite_nodes/CCAnimation.cpp \
sprite_nodes/CCAnimationCache.cpp \
sprite_nodes/CAImageView.cpp \
sprite_nodes/CCSpriteFrame.cpp \
sprite_nodes/CCSpriteFrameCache.cpp \
sprite_nodes/CAScale9ImageView.cpp \
sprite_nodes/CAWindow.cpp \
sprite_nodes/CAView.cpp \
sprite_nodes/CABatchView.cpp \
ui_controller/CAViewController.cpp \
ui_controller/CABarItem.cpp \
ui_controller/CABar.cpp \
ui_controller/CAControl.cpp \
ui_controller/CAButton.cpp \
ui_controller/CASwitch.cpp \
ui_controller/CASlider.cpp \
ui_controller/CASegmentedControl.cpp \
ui_controller/CADateSpinner.cpp \
ui_controller/CAScrollView.cpp \
ui_controller/CATableView.cpp \
ui_controller/CAProgress.cpp \
support/ccUTF8.cpp \
support/CCNotificationCenter.cpp \
support/CCProfiling.cpp \
support/CCPointExtension.cpp \
support/TransformUtils.cpp \
support/user_default/CCUserDefaultAndroid.cpp \
support/base64.cpp \
support/ccUtils.cpp \
support/CCVertex.cpp \
support/data_support/ccCArray.cpp \
support/image_support/TGAlib.cpp \
support/tinyxml2/tinyxml2.cpp \
support/zip_support/ZipUtils.cpp \
support/zip_support/ioapi.cpp \
support/zip_support/unzip.cpp \
support/component/CCComponent.cpp \
support/component/CCComponentContainer.cpp \
text_input_node/CCIMEDispatcher.cpp \
text_input_node/CATextField.cpp \
images/CAImage.cpp \
images/CAImageCache.cpp \
images/CAImageETC.cpp \
images/CAImagePVR.cpp \
images/CAImageAtlas.cpp \
touch_dispatcher/CATouchDispatcher.cpp \
touch_dispatcher/CCTouch.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/include \
                    $(LOCAL_PATH)/kazmath/include \
                    $(LOCAL_PATH)/platform/android

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/include \
                    $(LOCAL_PATH)/kazmath/include \
                    $(LOCAL_PATH)/platform/android

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

# define the macro to compile through support/zip_support/ioapi.c
LOCAL_CFLAGS := -Wno-psabi -DUSE_FILE32API
LOCAL_EXPORT_CFLAGS := -Wno-psabi -DUSE_FILE32API

include $(BUILD_STATIC_LIBRARY)

$(call import-module,libjpeg)
$(call import-module,libpng)
$(call import-module,libtiff)
$(call import-module,libwebp)
