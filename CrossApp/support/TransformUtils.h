

#ifndef __SUPPORT_TRANSFORM_UTILS_H__
#define __SUPPORT_TRANSFORM_UTILS_H__

// todo:
// when in MAC or windows, it includes <OpenGL/gl.h>
#include "CCGL.h"

namespace   CrossApp {

struct CCAffineTransform;

void CGAffineToGL(const CCAffineTransform *t, GLfloat *m);
void GLToCGAffine(const GLfloat *m, CCAffineTransform *t);
}//namespace   CrossApp 

#endif // __SUPPORT_TRANSFORM_UTILS_H__
