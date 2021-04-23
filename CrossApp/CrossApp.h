//
//  CrossApp.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-6-16.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CROSSAPP_H__
#define __CROSSAPP_H__

// 0x00 HI ME LO
// 00   02 01 00
#define CROSSAPP_VERSION 0x00020100

//
// all CrossApp include files
//

#include "ccConfig.h"
#include "ccMacros.h"
#include "ccTypes.h"

// animation
#include "animation/CACustomAnimation.h"
#include "animation/CAViewAnimation.h"

//basics
#include "basics/CAApplication.h"
#include "basics/CAAutoreleasePool.h"
#include "basics/CACamera.h"
#include "basics/CAFPSImages.h"
#include "basics/CAColor.h"
#include "basics/CAPoint.h"
#include "basics/CASize.h"
#include "basics/CARect.h"
#include "basics/CAPoint3D.h"
#include "basics/CAVec4.h"
#include "basics/CALayout.h"
#include "basics/CAObject.h"
#include "basics/CAResponder.h"
#include "basics/CAScheduler.h"
#include "basics/CASTLContainer.h"
#include "basics/CAIndexPath.h"
#include "basics/CAThread.h"
#include "basics/CASyncQueue.h"
#include "basics/CANotificationCenter.h"
#include "basics/CAPointExtension.h"
#include "basics/CAConfiguration.h"
#include "basics/CAData.h"
//control
#include "control/CABar.h"
#include "control/CAControl.h"
#include "control/CAButton.h"
#include "control/CAProgress.h"
#include "control/CASwitch.h"
#include "control/CASlider.h"
#include "control/CASegmentedControl.h"
#include "control/CAPageControl.h"
#include "control/CAStepper.h"
#include "control/CACheckbox.h"

//controller
#include "controller/CABarItem.h"
#include "controller/CAFont.h"
#include "controller/CAViewController.h"
#include "controller/CADrawerController.h"

//dispatcher
#include "dispatcher/CATouch.h"
#include "dispatcher/CATouchDispatcher.h"
#include "dispatcher/CAKeypadDispatcher.h"
#include "dispatcher/CAProtocols.h"


// images
#include "images/CAImage.h"
#include "images/CAImageCube.h"
#include "images/CAImageCache.h"
#include "images/CAGif.h"
#include "images/CAQrencode.h"


//view
#include "view/CAView.h"
#include "view/CAWindow.h"
#include "view/CAImageView.h"
#include "view/CAScale9ImageView.h"
#include "view/CAGifView.h"
#include "view/CALabel.h"
#include "view/CARichLabel.h"
#include "view/CAPullToRefreshView.h"
#include "view/CACell.h"
#include "view/CAScrollView.h"
#include "view/CAPageView.h"
#include "view/CAListView.h"
#include "view/CATableView.h"
#include "view/CACollectionView.h"
#include "view/CAAutoCollectionView.h"
#include "view/CAWaterfallView.h"
#include "view/CAWebView.h"
#include "view/CARenderImage.h"
#include "view/CAClippingView.h"
#include "view/CAAlertView.h"
#include "view/CAAVPlayerView.h"
#include "view/CAPickerView.h"
#include "view/CADatePickerView.h"
#include "view/CAActivityIndicatorView.h"
#include "view/CADrawingPrimitives.h"
#include "view/CADrawView.h"
#include "view/CACubeView.h"
#include "view/CATextEditHelper.h"

// cocoa
#include "cocoa/CCSet.h"
#include "cocoa/CCNS.h"
#include "cocoa/CACalendar.h"

//math
#include "math/CAAffineTransform.h"
#include "math/CAMath.h"
#include "math/CAMathBase.h"
#include "math/CAVertex.h"
#include "math/CAMat4.h"
#include "math/CAMathUtil.h"
#include "math/CAQuaternion.h"
#include "math/TransformUtils.h"

// renderer
#include "renderer/CCCustomCommand.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCGLProgramStateCache.h"
#include "renderer/CCGroupCommand.h"
#include "renderer/CCMaterial.h"
#include "renderer/CCPass.h"
#include "renderer/CCPrimitive.h"
#include "renderer/CCPrimitiveCommand.h"
#include "renderer/CCQuadCommand.h"
#include "renderer/CCRenderCommand.h"
#include "renderer/CCRenderCommandPool.h"
#include "renderer/CCRenderState.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCTechnique.h"
#include "renderer/CCTrianglesCommand.h"
#include "renderer/CCVertexIndexBuffer.h"
#include "renderer/CCVertexIndexData.h"
#include "renderer/CCFrameBuffer.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/ccShaders.h"


// support
#include "support/ccUtils.h"
#include "support/ccUTF8.h"
#include "support/CAProfiling.h"
#include "support/user_default/CAUserDefault.h"
#include "support/tinyxml2/tinyxml2.h"
#include "support/md5.h"
#include "support/base64.h"
#include "support/Json/CSContentJsonDictionary.h"
#include "support/sqlite3/sqlite3.h"
#include "support/LocalStorage/LocalStorage.h"
#include "support/CAThemeManager.h"

// network
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "network/DownloadManager.h"
#include "network/WebSocket.h"
#include "network/assets-manager/AssetsManager.h"

// platform
#include "platform/CACommon.h"
#include "platform/CAFileUtils.h"
#include "platform/CASAXParser.h"
#include "platform/platform.h"
#include "platform/CCPlatformConfig.h"
#include "platform/CCPlatformMacros.h"
#include "platform/CATextField.h"
#include "platform/CATextView.h"
#include "platform/CADensityDpi.h"
#include "platform/CAFontProcesstor.h"
#include "platform/CAClipboard.h"
#include "platform/CAScanQRcode.h"

// device
#include "device/CADevice.h"
#include "device/CAMotionManager.h"
#include "device/CAImagePickerController.h"
#include "device/CAAddressBook.h"

//script_support
#include "script_support/CAScriptSupport.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "platform/ios/CCApplication.h"
    #include "platform/ios/CCEGLView.h"
    #include "platform/ios/CCGL.h"
    #include "platform/ios/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "platform/android/CCApplication.h"
    #include "platform/android/CCEGLView.h"
    #include "platform/android/CCGL.h"
    #include "platform/android/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#include "platform/win32/CCApplication.h"
	#include "platform/win32/CCEGLView.h"
	#include "platform/win32/CCGL.h"
	#include "platform/win32/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	#include "platform/winrt/CCApplication.h"
	#include "platform/winrt/CCEGLView.h"
	#include "platform/winrt/CCGL.h"
	#include "platform/winrt/CCStdC.h"
	#include "platform/winrt/CCPrecompiledShaders.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WINRT

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	#include "platform/mac/CCApplication.h"
	#include "platform/mac/CCEGLView.h"
	#include "platform/mac/CCGL.h"
	#include "platform/mac/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_MAC


#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	#include "platform/linux/CCApplication.h"
	#include "platform/linux/CCEGLView.h"
	#include "platform/linux/CCGL.h"
	#include "platform/linux/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_LINUX


#include "game/CGSprite.h"
#include "game/CGSpriteFrame.h"
#include "game/CGSpriteFrameCache.h"
#include "game/CGProgressTimer.h"
#include "game/actions/CGAnimation.h"
#include "game/actions/CGAnimationCache.h"
#include "game/actions/CGAction.h"
#include "game/actions/CGActionCamera.h"
#include "game/actions/CGActionCatmullRom.h"
#include "game/actions/CGActionEase.h"
#include "game/actions/CGActionTween.h"
#include "game/actions/CGActionInstant.h"
#include "game/actions/CGActionManager.h"
#include "game/actions/CGActionProgressTimer.h"
#include "game/actions/CGActionInterval.h"
#include "game/actions/CGTweenFunction.h"


NS_CC_BEGIN
CC_DLL const char* CrossAppVersion();
NS_CC_END
#endif // __CrossApp_H__
