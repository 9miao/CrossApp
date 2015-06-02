
#ifndef __CCGL_H__
#define __CCGL_H__

#include "GL/glew.h"

#define CC_GL_DEPTH24_STENCIL8		GL_DEPTH24_STENCIL8

// These macros are only for making CCTexturePVR.cpp complied without errors since they are not included in GLEW.
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                      0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                      0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                     0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                     0x8C03

#endif // __CCGL_H__
