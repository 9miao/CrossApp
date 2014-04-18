//
//  OpenBrowser.cpp
//  DDHero
//
//  Created by mac on 13-8-17.
//
//

#include "OpenBrowser.h"
#import "EAGLView.h"

void OpenBrowser::openUrl(cocos2d::CCString *strUrl)
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:strUrl->getCString()]]];

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:strUrl->getCString()]]];
    
#endif
    
}