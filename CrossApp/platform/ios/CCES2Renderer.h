
#import "CCPlatformMacros.h"
#import "CCESRenderer.h"
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@interface CCES2Renderer : NSObject <CCESRenderer>
{
    // The pixel dimensions of the CAEAGLLayer
    GLint backingWidth_;
    GLint backingHeight_;

    unsigned int    samplesToUse_;
    BOOL            multiSampling_;

    unsigned int    depthFormat_;
    unsigned int    pixelFormat_;

    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view
    GLuint defaultFramebuffer_;
    GLuint colorRenderbuffer_;
    GLuint depthBuffer_;


    //buffers for MSAA
    GLuint msaaFramebuffer_;
    GLuint msaaColorbuffer_;

    EAGLContext *context_;
}

/** Color Renderbuffer */
@property (nonatomic,readonly) GLuint colorRenderbuffer;

/** Default Renderbuffer */
@property (nonatomic,readonly) GLuint defaultFramebuffer;

/** MSAA Framebuffer */
@property (nonatomic,readonly) GLuint msaaFramebuffer;

/** MSAA Color Buffer */
@property (nonatomic,readonly) GLuint msaaColorbuffer;

/** EAGLContext */
@property (nonatomic,readonly) EAGLContext* context;

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;
@end



