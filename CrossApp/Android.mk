LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := CrossApp_static

LOCAL_MODULE_FILENAME := libCrossApp

ifeq ($(USE_ARM_MODE),1)
LOCAL_ARM_MODE := arm
endif

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
MATHNEONFILE := math/CAMathUtil.cpp.neon
else
MATHNEONFILE := math/CAMathUtil.cpp
endif

LOCAL_SRC_FILES := \
CrossApp.cpp \
animation/CAViewAnimation.cpp \
animation/CACustomAnimation.cpp \
basics/CAApplication.cpp \
basics/CAAutoreleasePool.cpp \
basics/CALayout.cpp \
basics/CAPoint.cpp \
basics/CAColor.cpp \
basics/CASize.cpp \
basics/CARect.cpp \
basics/CAPoint3D.cpp \
basics/CAVec4.cpp \
basics/CAValue.cpp \
basics/CAObject.cpp \
basics/CAResponder.cpp \
basics/CAScheduler.cpp \
basics/CAFPSImages.c \
basics/CAIndexPath.cpp \
basics/CAThread.cpp \
basics/CANotificationCenter.cpp \
basics/CAPointExtension.cpp \
basics/CACamera.cpp \
basics/CACameraBackgroundBrush.cpp \
basics/CAConfiguration.cpp \
basics/CAData.cpp \
cocoa/CCNS.cpp \
cocoa/CCSet.cpp \
cocoa/CACalendar.cpp \
control/CABar.cpp \
control/CAButton.cpp \
control/CAControl.cpp \
control/CACheckbox.cpp \
control/CAProgress.cpp \
control/CASegmentedControl.cpp \
control/CASlider.cpp \
control/CASwitch.cpp \
control/CAPageControl.cpp \
control/CAStepper.cpp \
controller/CABarItem.cpp \
controller/CAViewController.cpp \
controller/CADrawerController.cpp \
controller/CAFont.cpp \
dispatcher/CAKeypadDispatcher.cpp \
dispatcher/CATouchDispatcher.cpp \
dispatcher/CATouch.cpp \
images/CAImage.cpp \
images/CAImageCache.cpp \
images/CAImageCube.cpp \
images/CAQrencode.cpp \
images/CAGif.cpp \
images/qrencode/bitstream.c \
images/qrencode/mask.c \
images/qrencode/mmask.c \
images/qrencode/mqrspec.c \
images/qrencode/qrencode.c \
images/qrencode/qrinput.c \
images/qrencode/qrspec.c \
images/qrencode/rsecc.c \
images/qrencode/split.c \
images/gif_lib/egif_lib.c \
images/gif_lib/openbsd-reallocarray.c \
images/gif_lib/dgif_lib.c \
images/gif_lib/gif_err.c \
images/gif_lib/gif_font.c \
images/gif_lib/gif_hash.c \
images/gif_lib/gifalloc.c \
images/gif_lib/quantize.c \
support/md5.cpp \
support/CAProfiling.cpp \
support/base64.cpp \
support/ccUTF8.cpp \
support/ConvertUTF.cpp \
support/ConvertUTFWrapper.cpp \
support/ccUtils.cpp \
support/CAUIEditorParser.cpp \
support/CAThemeManager.cpp \
support/data_support/ccCArray.cpp \
support/image_support/TGAlib.cpp \
support/tinyxml2/tinyxml2.cpp \
support/user_default/CAUserDefault.cpp \
support/user_default/CAUserDefaultAndroid.cpp \
support/zip_support/ZipUtils.cpp \
support/zip_support/ioapi.cpp \
support/zip_support/unzip.cpp \
support/LocalStorage/LocalStorageAndroid.cpp \
support/Json/CSContentJsonDictionary.cpp \
support/Json/lib_json/json_value.cpp \
support/Json/lib_json/json_reader.cpp \
support/Json/lib_json/json_writer.cpp \
support/sqlite3/sqlite3.c \
support/poly2tri/common/shapes.cc \
support/poly2tri/sweep/advancing_front.cc \
support/poly2tri/sweep/cdt.cc \
support/poly2tri/sweep/sweep.cc \
support/poly2tri/sweep/sweep_context.cc \
support/clipper/clipper.cpp \
support/xxhash/xxhash.c \
network/HttpClient.cpp \
network/DownloadManager.cpp \
network/WebSocket.cpp \
network/assets-manager/AssetsManager.cpp \
network/assets-manager/downloader/Downloader-curl.cpp \
network/assets-manager/downloader/Downloader.cpp \
view/CAAVPlayerView.cpp \
view/CAClippingView.cpp \
view/CAImageView.cpp \
view/CALabel.cpp \
view/CARenderImage.cpp \
view/CAScale9ImageView.cpp \
view/CACell.cpp \
view/CAScrollView.cpp \
view/CAListView.cpp \
view/CATableView.cpp \
view/CACollectionView.cpp \
view/CAAutoCollectionView.cpp \
view/CAWaterfallView.cpp \
view/CAView.cpp \
view/CAWindow.cpp \
view/CAAlertView.cpp \
view/CAPageView.cpp \
view/CAPickerView.cpp \
view/CADatePickerView.cpp \
view/CAActivityIndicatorView.cpp \
view/CAPullToRefreshView.cpp \
view/CATextEditHelper.cpp \
view/CAWebView.cpp \
view/CADrawView.cpp \
view/CADrawingPrimitives.cpp \
view/CAGifView.cpp \
view/CARichLabel.cpp \
math/CAAffineTransform.cpp \
math/CAVertex.cpp \
math/CAMat4.cpp \
math/CAQuaternion.cpp \
math/TransformUtils.cpp \
$(MATHNEONFILE) \
renderer/CCCustomCommand.cpp \
renderer/CCGLProgram.cpp \
renderer/CCGLProgramCache.cpp \
renderer/CCGLProgramState.cpp \
renderer/CCGLProgramStateCache.cpp \
renderer/CCGroupCommand.cpp \
renderer/CCMaterial.cpp \
renderer/CCMeshCommand.cpp \
renderer/CCPass.cpp \
renderer/CCPrimitive.cpp \
renderer/CCPrimitiveCommand.cpp \
renderer/CCProperties.cpp \
renderer/CCBatchCommand.cpp \
renderer/CCQuadCommand.cpp \
renderer/CCRenderCommand.cpp \
renderer/CCRenderState.cpp \
renderer/CCRenderer.cpp \
renderer/CCTechnique.cpp \
renderer/CCTrianglesCommand.cpp \
renderer/CCVertexIndexBuffer.cpp \
renderer/CCVertexIndexData.cpp \
renderer/CCFrameBuffer.cpp \
renderer/ccGLStateCache.cpp \
renderer/ccShaders.cpp \
platform/CASAXParser.cpp \
platform/CAFileUtils.cpp \
platform/platform.cpp \
platform/CCEGLViewProtocol.cpp \
platform/android/CADensityDpi.cpp \
platform/android/CCEGLView.cpp \
platform/android/CCApplication.cpp \
platform/android/CACommon.cpp \
platform/android/CCFileUtilsAndroid.cpp \
platform/android/CAWebViewImpl.cpp \
platform/android/CATextField.cpp \
platform/android/CATextView.cpp \
platform/android/CAClipboard.cpp \
platform/android/CAAlertViewImpl.cpp \
platform/android/CAAVPlayerImpl.cpp \
platform/android/CAFontProcesstor.cpp \
platform/android/CAScanQRcode.cpp \
platform/android/jni/Java_org_CrossApp_lib_CrossAppHelper.cpp \
platform/android/jni/Java_org_CrossApp_lib_CrossAppRenderer.cpp \
platform/android/jni/JniHelper.cpp \
platform/android/jni/TouchesJni.cpp \
device/android/CADevice.cpp \
device/android/CAAddressBook.cpp \
device/android/CAMotionManager.cpp \
device/android/CAImagePickerController.cpp \
game/CGSprite.cpp \
game/CGSpriteFrame.cpp \
game/CGSpriteFrameCache.cpp \
game/CGProgressTimer.cpp \
game/actions/CGAnimation.cpp \
game/actions/CGAnimationCache.cpp \
game/actions/CGAction.cpp \
game/actions/CGActionCamera.cpp \
game/actions/CGActionCatmullRom.cpp \
game/actions/CGActionEase.cpp \
game/actions/CGActionTween.cpp \
game/actions/CGActionInstant.cpp \
game/actions/CGActionManager.cpp \
game/actions/CGActionInterval.cpp \
game/actions/CGTweenFunction.cpp \
script_support/CAScriptSupport.cpp \



LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/platform/android \
                    $(LOCAL_PATH)/support/network \
                    $(LOCAL_PATH)/support/LocalStorage \
                    $(LOCAL_PATH)/support/Json \
                    $(LOCAL_PATH)/support/Json/lib_json \

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/platform/android \
                    $(LOCAL_PATH)/support/network \
                    $(LOCAL_PATH)/support/LocalStorage \
                    $(LOCAL_PATH)/support/Json \
                    $(LOCAL_PATH)/support/Json/lib_json \

LOCAL_LDLIBS := -lGLESv2 \
                -llog \
                -landroid \

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -llog \
                       -landroid \
                       
LOCAL_WHOLE_STATIC_LIBRARIES += zlib_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_jpeg_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_png_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_tiff_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_webp_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_curl_static
LOCAL_WHOLE_STATIC_LIBRARIES += libwebsockets_static
LOCAL_WHOLE_STATIC_LIBRARIES += zbar_static



# define the macro to compile through support/zip_support/ioapi.c
LOCAL_CFLAGS   :=  -DUSE_FILE32API
LOCAL_CFLAGS   +=  -fexceptions
LOCAL_CFLAGS   +=  -D__STDC_CONSTANT_MACROS

#ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -DHAVE_NEON=1
#endif

LOCAL_CPPFLAGS := -Wno-deprecated-declarations

LOCAL_EXPORT_CFLAGS := -DUSE_FILE32API
LOCAL_EXPORT_CPPFLAGS := -Wno-deprecated-declarations


include $(BUILD_STATIC_LIBRARY)

$(call import-module,zlib/prebuilt/android)
$(call import-module,jpeg/prebuilt/android)
$(call import-module,png/prebuilt/android)
$(call import-module,tiff/prebuilt/android)
$(call import-module,webp/prebuilt/android)
$(call import-module,curl/prebuilt/android)
$(call import-module,websockets/prebuilt/android)
$(call import-module,zbar/prebuilt/android)

