//
//  UIToast.h
//  Smobiler
//
//  Created by 惜风幻 on 15/8/10.
//
//

#import <Foundation/Foundation.h>

@interface UIToast : NSObject
{
}

- (void)showToast:(NSString*)text tiemInterval:(float)time;

- (void)showToast:(NSString*)text tiemInterval:(float)time backGroundRes:(NSString*)bgResPath;

@end
