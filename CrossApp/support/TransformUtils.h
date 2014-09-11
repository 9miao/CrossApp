

#ifndef __SUPPORT_TRANSFORM_UTILS_H__
#define __SUPPORT_TRANSFORM_UTILS_H__

// todo:
// when in MAC or windows, it includes <OpenGL/gl.h>
#include "CCGL.h"

namespace   CrossApp {

struct CATransformation;

void CGAffineToGL(const CATransformation *t, GLfloat *m);
void GLToCGAffine(const GLfloat *m, CATransformation *t);
}//namespace   CrossApp 

#endif // __SUPPORT_TRANSFORM_UTILS_H__
