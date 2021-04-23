
#import "platform/CCPlatformMacros.h"
#import <QuartzCore/QuartzCore.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

@protocol CCESRenderer <NSObject>

- (id) initWithDepthFormat:(unsigned int)depthFormat withPixelFormat:(unsigned int)pixelFormat withSharegroup:(EAGLSharegroup*)sharegroup withMultiSampling:(BOOL) multiSampling withNumberOfSamples:(unsigned int) requestedSamples;

- (BOOL) resizeFromLayer:(CAEAGLLayer *)layer;

- (EAGLContext*) context;
- (CGSize) backingSize;

- (unsigned int) colorRenderBuffer;
- (unsigned int) defaultFrameBuffer;
- (unsigned int) msaaFrameBuffer;
- (unsigned int) msaaColorBuffer;
@end

