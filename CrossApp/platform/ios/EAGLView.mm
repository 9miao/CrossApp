
#import <QuartzCore/QuartzCore.h>
#import "CCEGLView.h"
#import "EAGLView.h"
#import "CCES2Renderer.h"
#import "CAApplication.h"
#import "CCSet.h"
#import "CATouch.h"
#import "OpenGL_Internal.h"
#import "CCEGLView.h"

//CLASS IMPLEMENTATIONS:

#define IOS_MAX_TOUCHES_COUNT     10

static EAGLView *view = 0;

@interface EAGLView (Private)
- (BOOL) setupSurfaceWithSharegroup:(EAGLSharegroup*)sharegroup;
- (unsigned int) convertPixelFormat:(NSString*) pixelFormat;
@end

@implementation EAGLView

@synthesize surfaceSize=size_;
@synthesize pixelFormat=pixelformat_, depthFormat=depthFormat_;
@synthesize context=context_;
@synthesize multiSampling=multiSampling_;

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

+ (id) viewWithFrame:(CGRect)frame
{
    return [[[self alloc] initWithFrame:frame] autorelease];
}

+ (id) viewWithFrame:(CGRect)frame pixelFormat:(NSString*)format
{
    return [[[self alloc]initWithFrame:frame pixelFormat:format] autorelease];
}

+ (id) viewWithFrame:(CGRect)frame pixelFormat:(NSString*)format depthFormat:(GLuint)depth
{
    return [[[self alloc] initWithFrame:frame pixelFormat:format depthFormat:depth preserveBackbuffer:NO sharegroup:nil multiSampling:NO numberOfSamples:0] autorelease];
}

+ (id) viewWithFrame:(CGRect)frame pixelFormat:(NSString*)format depthFormat:(GLuint)depth preserveBackbuffer:(BOOL)retained sharegroup:(EAGLSharegroup*)sharegroup multiSampling:(BOOL)multisampling numberOfSamples:(unsigned int)samples
{
    return [[[self alloc]initWithFrame:frame pixelFormat:format depthFormat:depth preserveBackbuffer:retained sharegroup:sharegroup multiSampling:multisampling numberOfSamples:samples] autorelease];
}

+ (id) sharedEGLView
{
    return view;
}

- (id) initWithFrame:(CGRect)frame
{
    return [self initWithFrame:frame pixelFormat:kEAGLColorFormatRGB565 depthFormat:0 preserveBackbuffer:NO sharegroup:nil multiSampling:NO numberOfSamples:0];
}

- (id) initWithFrame:(CGRect)frame pixelFormat:(NSString*)format 
{
    return [self initWithFrame:frame pixelFormat:format depthFormat:0 preserveBackbuffer:NO sharegroup:nil multiSampling:NO numberOfSamples:0];
}

- (id) initWithFrame:(CGRect)frame pixelFormat:(NSString*)format depthFormat:(GLuint)depth preserveBackbuffer:(BOOL)retained sharegroup:(EAGLSharegroup*)sharegroup multiSampling:(BOOL)sampling numberOfSamples:(unsigned int)nSamples;
{
    if((self = [super initWithFrame:frame]))
    {
        pixelformat_ = format;
        depthFormat_ = depth;
        multiSampling_ = sampling;
        requestedSamples_ = nSamples;
        preserveBackbuffer_ = retained;
        if( ! [self setupSurfaceWithSharegroup:sharegroup] ) {
            [self release];
            return nil;
        }

        view = self;
        
        if ([view respondsToSelector:@selector(setContentScaleFactor:)])
		{
			view.contentScaleFactor = [[UIScreen mainScreen] scale];
		}
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(statusBarOrientationChange:) name:UIApplicationDidChangeStatusBarOrientationNotification object:nil];
    }
        
    return self;
}

-(void) setFrame:(CGRect)frame
{
    [super setFrame:frame];
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGSize size = frame.size;
    size.width *= scale;
    size.height *= scale;
    
    CrossApp::CCEGLView::sharedOpenGLView()->setFrameSize(size.width, size.height);
}

-(int) getWidth
{
    CGSize bound = [self bounds].size;
    return bound.width * self.contentScaleFactor;
}

-(int) getHeight
{
    CGSize bound = [self bounds].size;
    return bound.height * self.contentScaleFactor;
}

- (void)statusBarOrientationChange:(NSNotification *)notification
{
    [self setFrame:[self.superview bounds]];
    UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    
    switch (orientation)
    {
        case UIInterfaceOrientationPortrait:
        {
            CrossApp::CCEGLView::sharedOpenGLView()->setStatusBarOrientation(CrossApp::CAInterfaceOrientation::Portrait);
        }
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
        {
            CrossApp::CCEGLView::sharedOpenGLView()->setStatusBarOrientation(CrossApp::CAInterfaceOrientation::Portrait);
        }
            break;
        case UIInterfaceOrientationLandscapeRight:
        {
            CrossApp::CCEGLView::sharedOpenGLView()->setStatusBarOrientation(CrossApp::CAInterfaceOrientation::Landscape);
        }
            break;
        case UIInterfaceOrientationLandscapeLeft:
        {
            CrossApp::CCEGLView::sharedOpenGLView()->setStatusBarOrientation(CrossApp::CAInterfaceOrientation::Landscape);
        }
            break;
        default:
            break;
    }
}

-(BOOL) setupSurfaceWithSharegroup:(EAGLSharegroup*)sharegroup
{
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:preserveBackbuffer_], kEAGLDrawablePropertyRetainedBacking,
                                    pixelformat_, kEAGLDrawablePropertyColorFormat, nil];
    
    
    renderer_ = [[CCES2Renderer alloc] initWithDepthFormat:depthFormat_
                                         withPixelFormat:[self convertPixelFormat:pixelformat_]
                                          withSharegroup:sharegroup
                                       withMultiSampling:multiSampling_
                                     withNumberOfSamples:requestedSamples_];
    
    NSAssert(renderer_, @"OpenGL ES 2.O is required.");
    if (!renderer_)
        return NO;
    
    context_ = [renderer_ context];
    

    //discardFramebufferSupported_ = [[CCConfiguration sharedConfiguration] supportsDiscardFramebuffer];
    
    CHECK_GL_ERROR();
    
    return YES;
}

- (void) dealloc
{
    [renderer_ release];
    [super dealloc];
}

- (void) layoutSubviews
{
    [renderer_ resizeFromLayer:(CAEAGLLayer*)self.layer];
    size_ = [renderer_ backingSize];

    [EAGLContext setCurrentContext:context_];
    
    if ([NSThread isMainThread])
    {
        CrossApp::CAApplication::getApplication()->drawScene();
    }
}

- (void) swapBuffers
{
    // IMPORTANT:
    // - preconditions
    //    -> context_ MUST be the OpenGL context
    //    -> renderbuffer_ must be the the RENDER BUFFER

#ifdef __IPHONE_4_0
    
    if (multiSampling_)
    {
        /* Resolve from msaaFramebuffer to resolveFramebuffer */
        //glDisable(GL_SCISSOR_TEST);     
        glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, [renderer_ msaaFrameBuffer]);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, [renderer_ defaultFrameBuffer]);
        glResolveMultisampleFramebufferAPPLE();
    }
    
    if( discardFramebufferSupported_)
    {    
        if (multiSampling_)
        {
            if (depthFormat_)
            {
                GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
                glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, attachments);
            }
            else
            {
                GLenum attachments[] = {GL_COLOR_ATTACHMENT0};
                glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, attachments);
            }
            
            glBindRenderbuffer(GL_RENDERBUFFER, [renderer_ colorRenderBuffer]);
    
        }    
        
        // not MSAA
        else if (depthFormat_ ) {
            GLenum attachments[] = { GL_DEPTH_ATTACHMENT};
            glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, attachments);
        }
    }
    
#endif // __IPHONE_4_0
    
     if(![context_ presentRenderbuffer:GL_RENDERBUFFER])
        {
//         //CCLOG(@"CossApp: Failed to swap renderbuffer in %s\n", __FUNCTION__);
        }

#if CROSSAPP_DEBUG
    CHECK_GL_ERROR();
#endif
    
    // We can safely re-bind the framebuffer here, since this will be the
    // 1st instruction of the new main loop
    if( multiSampling_ )
        glBindFramebuffer(GL_FRAMEBUFFER, [renderer_ msaaFrameBuffer]);    
}

- (unsigned int) convertPixelFormat:(NSString*) pixelFormat
{
    // define the pixel format
    GLenum pFormat;
    
    
    if([pixelFormat isEqualToString:@"EAGLColorFormat565"]) 
        pFormat = GL_RGB565;
    else 
        pFormat = GL_RGBA8_OES;
    
    return pFormat;
}

#pragma mark EAGLView - Point conversion

- (CGPoint) convertPointFromViewToSurface:(CGPoint)point
{
    CGRect bounds = [self bounds];
        
        CGPoint ret;
        ret.x = (point.x - bounds.origin.x) / bounds.size.width * size_.width;
        ret.y =  (point.y - bounds.origin.y) / bounds.size.height * size_.height;
    
        return ret;
}

- (CGRect) convertRectFromViewToSurface:(CGRect)rect
{
    CGRect bounds = [self bounds];
    
        CGRect ret;
        ret.origin.x = (rect.origin.x - bounds.origin.x) / bounds.size.width * size_.width;
        ret.origin.y = (rect.origin.y - bounds.origin.y) / bounds.size.height * size_.height;
        ret.size.width = rect.size.width / bounds.size.width * size_.width;
        ret.size.height = rect.size.height / bounds.size.height * size_.height;
    
        return ret;
}


-(void) handleTouchesAfterKeyboardShow
{
    NSArray *subviews = self.subviews;
    
    for(UIView* view in subviews)
    {
        if([view isKindOfClass:NSClassFromString(@"CustomUITextField")])
        {
            if ([view isFirstResponder])
            {
                [view resignFirstResponder];
                return;
            }
        }
    }
}

// Pass the touches to the superview
#pragma mark EAGLView - Touch Delegate
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* ids[IOS_MAX_TOUCHES_COUNT] = {0};
    float xs[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    float ys[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = touch;
        xs[i] = [touch locationInView: [touch view]].x * view.contentScaleFactor;;
        ys[i] = [touch locationInView: [touch view]].y * view.contentScaleFactor;;
        ++i;
    }
    
    CrossApp::CAEvent* theEvent = new CrossApp::CAEvent();
    theEvent->setEventType(CrossApp::EventType::iosEvent);
    CrossApp::CCEGLView::sharedOpenGLView()->handleTouchesBegin(i, (intptr_t*)ids, xs, ys, theEvent);
    theEvent->release();
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* ids[IOS_MAX_TOUCHES_COUNT] = {0};
    float xs[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    float ys[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = touch;
        xs[i] = [touch locationInView: [touch view]].x * view.contentScaleFactor;;
        ys[i] = [touch locationInView: [touch view]].y * view.contentScaleFactor;;
        ++i;
    }
    CrossApp::CAEvent* theEvent = new CrossApp::CAEvent();
    theEvent->setEventType(CrossApp::EventType::iosEvent);
    CrossApp::CCEGLView::sharedOpenGLView()->handleTouchesMove(i, (intptr_t*)ids, xs, ys, theEvent);
    theEvent->release();
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* ids[IOS_MAX_TOUCHES_COUNT] = {0};
    float xs[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    float ys[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = touch;
        xs[i] = [touch locationInView: [touch view]].x * view.contentScaleFactor;;
        ys[i] = [touch locationInView: [touch view]].y * view.contentScaleFactor;;
        ++i;
    }
    CrossApp::CAEvent* theEvent = new CrossApp::CAEvent();
    theEvent->setEventType(CrossApp::EventType::iosEvent);
    CrossApp::CCEGLView::sharedOpenGLView()->handleTouchesEnd(i, (intptr_t*)ids, xs, ys, theEvent);
    theEvent->release();
}
    
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* ids[IOS_MAX_TOUCHES_COUNT] = {0};
    float xs[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    float ys[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = touch;
        xs[i] = [touch locationInView: [touch view]].x * view.contentScaleFactor;;
        ys[i] = [touch locationInView: [touch view]].y * view.contentScaleFactor;;
        ++i;
    }
    CrossApp::CAEvent* theEvent = new CrossApp::CAEvent();
    theEvent->setEventType(CrossApp::EventType::iosEvent);
    CrossApp::CCEGLView::sharedOpenGLView()->handleTouchesCancel(i, (intptr_t*)ids, xs, ys, theEvent);
    theEvent->release();
}

@end
