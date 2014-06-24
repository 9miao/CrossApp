

#ifndef __CCGLSTATE_H__
#define __CCGLSTATE_H__

#include "CCGL.h"
#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN

/**
 * @addtogroup shaders
 * @{
 */

class CAGLProgram;

/** vertex attrib flags */
enum {
    kCCVertexAttribFlag_None        = 0,

    kCCVertexAttribFlag_Position    = 1 << 0,
    kCCVertexAttribFlag_Color       = 1 << 1,
    kCCVertexAttribFlag_TexCoords   = 1 << 2,

    kCCVertexAttribFlag_PosColorTex = ( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color | kCCVertexAttribFlag_TexCoords ),
};

/** GL server side states */
typedef enum {
//    CC_GL_SCISSOR_TEST = 1 << 0,
//    CC_GL_STENCIL_TEST = 1 << 1,
//    CC_GL_DEPTH_TEST = 1 << 2,
//    CC_GL_BLEND = 1 << 3,
//    CC_GL_DITHER = 1 << 4,

//    CC_GL_ALL = ( CC_GL_SCISSOR_TEST | CC_GL_STENCIL_TEST | CC_GL_DEPTH_TEST | CC_GL_BLEND | CC_GL_DITHER ),
//    CC_GL_ALL = ( CC_GL_BLEND ),
      CC_GL_ALL = 0,

} ccGLServerState;

/** @file ccGLStateCache.h
*/

/** Invalidates the GL state cache.
 If CC_ENABLE_GL_STATE_CACHE it will reset the GL state cache.
 @since v2.0.0
 */
void CC_DLL ccGLInvalidateStateCache(void);

/** Uses the GL program in case program is different than the current one.
 If CC_ENABLE_GL_STATE_CACHE is disabled, it will the glUseProgram() directly.
 @since v2.0.0
 */
void CC_DLL ccGLUseProgram(GLuint program);

/** Deletes the GL program. If it is the one that is being used, it invalidates it.
 If CC_ENABLE_GL_STATE_CACHE is disabled, it will the glDeleteProgram() directly.
 @since v2.0.0
 */
void CC_DLL ccGLDeleteProgram(GLuint program);

/** Uses a blending function in case it not already used.
 If CC_ENABLE_GL_STATE_CACHE is disabled, it will the glBlendFunc() directly.
 @since v2.0.0
 */
void CC_DLL ccGLBlendFunc(GLenum sfactor, GLenum dfactor);

/** Resets the blending mode back to the cached state in case you used glBlendFuncSeparate() or glBlendEquation().
 If CC_ENABLE_GL_STATE_CACHE is disabled, it will just set the default blending mode using GL_FUNC_ADD.
 @since v2.0.0
 */
void CC_DLL ccGLBlendResetToCache(void);

/** sets the projection matrix as dirty
 @since v2.0.0
 */
void CC_DLL ccSetProjectionMatrixDirty(void);

/** Will enable the vertex attribs that are passed as flags.
 Possible flags:

    * kCCVertexAttribFlag_Position
    * kCCVertexAttribFlag_Color
    * kCCVertexAttribFlag_TexCoords

 These flags can be ORed. The flags that are not present, will be disabled.

 @since v2.0.0
 */
void CC_DLL ccGLEnableVertexAttribs(unsigned int flags);

/** If the Image is not already bound to Image unit 0, it binds it.
 If CC_ENABLE_GL_STATE_CACHE is disabled, it will call glBindTexture() directly.
 @since v2.0.0
 */
void CC_DLL ccGLBindTexture2D(GLuint textureId);


/** If the Image is not already bound to a given unit, it binds it.
 If CC_ENABLE_GL_STATE_CACHE is disabled, it will call glBindTexture() directly.
 @since v2.1.0
 */
void CC_DLL ccGLBindTexture2DN(GLuint textureUnit, GLuint textureId);

/** It will delete a given texture. If the Image was bound, it will invalidate the cached.
 If CC_ENABLE_GL_STATE_CACHE is disabled, it will call glDeleteTextures() directly.
 @since v2.0.0
 */
void CC_DLL ccGLDeleteTexture(GLuint textureId);

/** It will delete a given texture. If the Image was bound, it will invalidate the cached for the given Image unit.
 If CC_ENABLE_GL_STATE_CACHE is disabled, it will call glDeleteTextures() directly.
 @since v2.1.0
 */
void CC_DLL ccGLDeleteTextureN(GLuint textureUnit, GLuint textureId);

/** If the vertex array is not already bound, it binds it.
 If CC_ENABLE_GL_STATE_CACHE is disabled, it will call glBindVertexArray() directly.
 @since v2.0.0
 */
void CC_DLL ccGLBindVAO(GLuint vaoId);

/** It will enable / disable the server side GL states.
 If CC_ENABLE_GL_STATE_CACHE is disabled, it will call glEnable() directly.
 @since v2.0.0
 */
void CC_DLL ccGLEnable( ccGLServerState flags );

// end of shaders group
/// @}

NS_CC_END
    

#endif /* __CCGLSTATE_H__ */
