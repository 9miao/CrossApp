

#ifndef __SUPPORT_TRANSFORM_UTILS_H__
#define __SUPPORT_TRANSFORM_UTILS_H__

// TODO: when in MAC or windows, it includes <OpenGL/gl.h>
#include "CCGL.h"
#include "ccMacros.h"

/**
 * @addtogroup base
 * @{
 */

namespace   CrossApp {

struct AffineTransform;
/**@{
 Conversion between mat4*4 and AffineTransform.
 @param m The Mat4*4 pointer.
 @param t Affine transform.
 */
CC_DLL void CGAffineToGL(const AffineTransform &t, GLfloat *m);
CC_DLL void GLToCGAffine(const GLfloat *m, AffineTransform *t);
/**@}*/
}//namespace   CossApp 
/**
 end of base group
 @}
 */
#endif // __SUPPORT_TRANSFORM_UTILS_H__
