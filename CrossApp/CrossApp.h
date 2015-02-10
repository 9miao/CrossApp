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

// actions
#include "actions/CCAction.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionCamera.h"
#include "actions/CCActionManager.h"
#include "actions/CCActionEase.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionTween.h"

//basics
#include "basics/CAApplication.h"
#include "basics/CAAutoreleasePool.h"
#include "basics/CACamera.h"
#include "basics/CAFPSImages.h"
#include "basics/CAGeometry.h"
#include "basics/CAObject.h"
#include "basics/CAResponder.h"
#include "basics/CAScheduler.h"
#include "basics/CASTLContainer.h"
#include "basics/CAIndexPath.h"
#include "basics/CAThread.h"
#include "basics/CASyncQueue.h"

//control
#include "control/CABar.h"
#include "control/CAControl.h"
#include "control/CAButton.h"
#include "control/CAProgress.h"
#include "control/CASwitch.h"
#include "control/CASlider.h"
#include "control/CASegmentedControl.h"
#include "control/CATextField.h"
#include "control/CAPageControl.h"
#include "control/CAStepper.h"

//controller
#include "controller/CABarItem.h"
#include "controller/CAViewController.h"
#include "controller/CADrawerController.h"

//dispatcher
#include "dispatcher/CATouch.h"
#include "dispatcher/CATouchDispatcher.h"
#include "dispatcher/CAKeypadDelegate.h"
#include "dispatcher/CAKeypadDispatcher.h"
#include "dispatcher/CAIMEDelegate.h"
#include "dispatcher/CAIMEDispatcher.h"
#include "dispatcher/CAProtocols.h"


// images
#include "images/CAImage.h"
#include "images/CAImageCache.h"

#include "animation/CAViewAnimation.h"

//view
#include "view/CAView.h"
#include "view/CAWindow.h"
#include "view/CAImageView.h"
#include "view/CAScale9ImageView.h"
#include "view/CALabel.h"
#include "view/CABatchView.h"
#include "view/CARenderImage.h"
#include "view/CAClippingView.h"
#include "view/CAScrollView.h"
#include "view/CAListView.h"
#include "view/CATableView.h"
#include "view/CAPageView.h"
#include "view/CACollectionView.h"
#include "view/CAAlertView.h"
#include "view/CAPickerView.h"
#include "view/CADatePickerView.h"
#include "view/CAActivityIndicatorView.h"
#include "view/CAPullToRefreshView.h"
#include "view/CATextView.h"
#include "view/CAWebView.h"

// cocoa
#include "cocoa/CCDictionary.h"
#include "cocoa/CCArray.h"
#include "cocoa/CCSet.h"
#include "cocoa/CCString.h"
#include "cocoa/CCNS.h"
#include "cocoa/CACalendar.h"

// draw nodes
#include "draw_nodes/CCDrawingPrimitives.h"
#include "draw_nodes/CCDrawNode.h"


// kazmath
#include "kazmath/include/kazmath/kazmath.h"
#include "kazmath/include/kazmath/GL/matrix.h"


// shaders
#include "shaders/CATransformation.h"
#include "shaders/CAGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CAShaderCache.h"
#include "shaders/ccShaders.h"


// support
#include "support/ccUtils.h"
#include "support/ccUTF8.h"
#include "support/CANotificationCenter.h"
#include "support/CCPointExtension.h"
#include "support/CCProfiling.h"
#include "support/user_default/CAUserDefault.h"
#include "support/CCVertex.h"
#include "support/tinyxml2/tinyxml2.h"
#include "support/md5.h"
#include "support/base64.h"

// platform
#include "platform/CCDevice.h"
#include "platform/CCCommon.h"
#include "platform/CCFileUtils.h"
#include "platform/CCImage.h"
#include "platform/CCSAXParser.h"
#include "platform/platform.h"
#include "platform/CCPlatformConfig.h"
#include "platform/CCPlatformMacros.h"
#include "platform/CAFreeTypeFont.h"
#include "platform/CAFTFontCache.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
// video
#include "video/CAVideoPlayerController.h"
#endif //(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

//script_support
#include "script_support/CCScriptSupport.h"
#include "script_support/JSViewController.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "platform/ios/CCAccelerometer.h"
    #include "platform/ios/CCApplication.h"
    #include "platform/ios/CCEGLView.h"
    #include "platform/ios/CCGL.h"
    #include "platform/ios/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "platform/android/CCAccelerometer.h"
    #include "platform/android/CCApplication.h"
    #include "platform/android/CCEGLView.h"
    #include "platform/android/CCGL.h"
    #include "platform/android/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#include "platform/win32/CCAccelerometer.h"
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
	#include "platform/winrt/CCAccelerometer.h"
	#include "platform/winrt/CCPrecompiledShaders.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WINRT

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	#include "platform/winrt/CCApplication.h"
	#include "platform/wp8/CCEGLView.h"
	#include "platform/winrt/CCGL.h"
	#include "platform/winrt/CCStdC.h"
	#include "platform/winrt/CCAccelerometer.h"
	#include "platform/winrt/CCPrecompiledShaders.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WP8

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	#include "platform/mac/CCAccelerometer.h"
	#include "platform/mac/CCApplication.h"
	#include "platform/mac/CCEGLView.h"
	#include "platform/mac/CCGL.h"
	#include "platform/mac/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_MAC


#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	#include "platform/linux/CCAccelerometer.h"
	#include "platform/linux/CCApplication.h"
	#include "platform/linux/CCEGLView.h"
	#include "platform/linux/CCGL.h"
	#include "platform/linux/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_LINUX


NS_CC_BEGIN
CC_DLL const char* CrossAppVersion();
NS_CC_END
#endif // __CrossApp_H__
