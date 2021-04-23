

#ifndef __CCGL_H__
#define __CCGL_H__

#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <OpenGL/glext.h>

#define CC_GL_DEPTH24_STENCIL8      -1


#define glDeleteVertexArrays            glDeleteVertexArraysAPPLE
#define glGenVertexArrays               glGenVertexArraysAPPLE
#define glBindVertexArray               glBindVertexArrayAPPLE
#define glClearDepthf                   glClearDepth
#define glDepthRangef                   glDepthRange
#define glReleaseShaderCompiler(xxx)



#endif // __CCGL_H__
