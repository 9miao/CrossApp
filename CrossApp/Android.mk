LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := CrossApp_static

LOCAL_MODULE_FILENAME := libCrossApp

LOCAL_SRC_FILES := \
CrossApp.cpp \
actions/CCAction.cpp \
actions/CCActionCamera.cpp \
actions/CCActionEase.cpp \
actions/CCActionInstant.cpp \
actions/CCActionInterval.cpp \
actions/CCActionManager.cpp \
actions/CCActionTween.cpp \
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
images/CAFlash.cpp \
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
view/CATextField.cpp \
view/CATextEditHelper.cpp \
view/CALabelStyle.cpp \
view/CAWebView.cpp \
view/CADrawView.cpp \
view/CADrawingPrimitives.cpp \
view/CAFlashView.cpp \
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
support/gameswf/base/component_hackery.cpp \
support/gameswf/base/configvars.cpp \
support/gameswf/base/container.cpp \
support/gameswf/base/ear_clip_triangulate_float.cpp \
support/gameswf/base/ear_clip_triangulate_sint16.cpp \
support/gameswf/base/file_util.cpp \
support/gameswf/base/image.cpp \
support/gameswf/base/image_filters.cpp \
support/gameswf/base/jpeg.cpp \
support/gameswf/base/logger.cpp \
support/gameswf/base/membuf.cpp \
support/gameswf/base/triangulate_float.cpp \
support/gameswf/base/triangulate_sint32.cpp \
support/gameswf/base/tu_file.cpp \
support/gameswf/base/tu_file_SDL.cpp \
support/gameswf/base/tu_gc_singlethreaded_marksweep.cpp \
support/gameswf/base/tu_gc_test.cpp \
support/gameswf/base/tu_loadlib.cpp \
support/gameswf/base/tu_random.cpp \
support/gameswf/base/tu_timer.cpp \
support/gameswf/base/tu_types.cpp \
support/gameswf/base/utf8.cpp \
support/gameswf/base/utility.cpp \
support/gameswf/base/zlib_adapter.cpp \
support/gameswf/gameswf/gameswf_abc.cpp \
support/gameswf/gameswf/gameswf_action.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_array.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_boolean.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_broadcaster.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_class.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_color.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_color_transform.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_date.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_event.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_flash.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_geom.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_global.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_key.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_loadvars.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_math.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_matrix.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_mcloader.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_mouse.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_mouse_event.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_netconnection.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_netstream.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_number.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_point.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_selection.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_sharedobject.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_sound.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_string.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_textformat.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_transform.cpp \
support/gameswf/gameswf/gameswf_as_classes/as_xmlsocket.cpp \
support/gameswf/gameswf/gameswf_as_sprite.cpp \
support/gameswf/gameswf/gameswf_avm2.cpp \
support/gameswf/gameswf/gameswf_avm2_jit.cpp \
support/gameswf/gameswf/gameswf_button.cpp \
support/gameswf/gameswf/gameswf_canvas.cpp \
support/gameswf/gameswf/gameswf_character.cpp \
support/gameswf/gameswf/gameswf_disasm.cpp \
support/gameswf/gameswf/gameswf_dlist.cpp \
support/gameswf/gameswf/gameswf_environment.cpp \
support/gameswf/gameswf/gameswf_filters.cpp \
support/gameswf/gameswf/gameswf_font.cpp \
support/gameswf/gameswf/gameswf_fontlib.cpp \
support/gameswf/gameswf/gameswf_freetype.cpp \
support/gameswf/gameswf/gameswf_function.cpp \
support/gameswf/gameswf/gameswf_impl.cpp \
support/gameswf/gameswf/gameswf_jit.cpp \
support/gameswf/gameswf/gameswf_jit_opcode.cpp \
support/gameswf/gameswf/gameswf_listener.cpp \
support/gameswf/gameswf/gameswf_log.cpp \
support/gameswf/gameswf/gameswf_morph2.cpp \
support/gameswf/gameswf/gameswf_movie_def.cpp \
support/gameswf/gameswf/gameswf_mutex.cpp \
support/gameswf/gameswf/gameswf_object.cpp \
support/gameswf/gameswf/gameswf_player.cpp \
support/gameswf/gameswf/gameswf_render.cpp \
support/gameswf/gameswf/gameswf_render_handler_ogles.cpp \
support/gameswf/gameswf/gameswf_root.cpp \
support/gameswf/gameswf/gameswf_shape.cpp \
support/gameswf/gameswf/gameswf_sound.cpp \
support/gameswf/gameswf/gameswf_sprite.cpp \
support/gameswf/gameswf/gameswf_sprite_def.cpp \
support/gameswf/gameswf/gameswf_stream.cpp \
support/gameswf/gameswf/gameswf_styles.cpp \
support/gameswf/gameswf/gameswf_tesselate.cpp \
support/gameswf/gameswf/gameswf_text.cpp \
support/gameswf/gameswf/gameswf_types.cpp \
support/gameswf/gameswf/gameswf_value.cpp \
support/gameswf/gameswf/gameswf_video_impl.cpp \



LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/kazmath/include \
                    $(LOCAL_PATH)/platform/android \
                    $(LOCAL_PATH)/support/gameswf/base \
                    $(LOCAL_PATH)/support/gameswf/gameswf \
                    $(LOCAL_PATH)/support/gameswf/gameswf/gameswf_as_classes\

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/kazmath/include \
                    $(LOCAL_PATH)/platform/android \
                    $(LOCAL_PATH)/support/gameswf/base \
                    $(LOCAL_PATH)/support/gameswf/gameswf \
                    $(LOCAL_PATH)/support/gameswf/gameswf/gameswf_as_classes\

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
LOCAL_CFLAGS := -Wno-psabi -DUSE_FILE32API -D__STDC_CONSTANT_MACROS
LOCAL_EXPORT_CFLAGS := -Wno-psabi -DUSE_FILE32API -D__STDC_CONSTANT_MACROS

include $(BUILD_STATIC_LIBRARY)


$(call import-module,jpeg/prebuilt/android)
$(call import-module,png/prebuilt/android)
$(call import-module,tiff/prebuilt/android)
$(call import-module,webp/prebuilt/android)
$(call import-module,freetype/prebuilt/android)
$(call import-module,SDL/prebuilt/android)
$(call import-module,FFmpeg/prebuilt/android)
$(call import-module,../support/gameswf)
