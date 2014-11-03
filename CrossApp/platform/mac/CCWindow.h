

#ifndef __CC_WINDOW_H__
#define __CC_WINDOW_H__

#import <Cocoa/Cocoa.h>


@interface CCWindow : NSWindow
{
}
- (id) initWithFrame:(NSRect)frame fullscreen:(BOOL)fullscreen;

@end

#endif  // __CC_WINDOW_H__