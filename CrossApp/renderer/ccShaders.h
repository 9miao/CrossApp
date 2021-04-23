

#ifndef __CCSHADER_H__
#define __CCSHADER_H__
/// @cond DO_NOT_SHOW

#include "CCGL.h"
#include "platform/CCPlatformMacros.h"

/**
 * @addtogroup renderer
 * @{
 */

NS_CC_BEGIN

extern CC_DLL const GLchar * ccPosition_uColor_frag;
extern CC_DLL const GLchar * ccPosition_uColor_vert;

extern CC_DLL const GLchar * ccPositionColor_frag;
extern CC_DLL const GLchar * ccPositionColor_vert;

extern CC_DLL const GLchar * ccPositionColorTextureAsPointsize_vert;

extern CC_DLL const GLchar * ccPositionTexture_frag;
extern CC_DLL const GLchar * ccPositionTexture_vert;

extern CC_DLL const GLchar * ccPositionTextureA8Color_frag;
extern CC_DLL const GLchar * ccPositionTextureA8Color_vert;

extern CC_DLL const GLchar * ccPositionTextureColor_frag;
extern CC_DLL const GLchar * ccPositionTextureColor_vert;

extern CC_DLL const GLchar * ccPositionTextureColor_noMVP_frag;
extern CC_DLL const GLchar * ccPositionTextureColor_noMVP_vert;

extern CC_DLL const GLchar * ccPositionTextureColorAlphaTest_frag;

extern CC_DLL const GLchar * ccPositionTexture_uColor_frag;
extern CC_DLL const GLchar * ccPositionTexture_uColor_vert;

extern CC_DLL const GLchar * ccPositionColorLengthTexture_frag;
extern CC_DLL const GLchar * ccPositionColorLengthTexture_vert;

extern CC_DLL const GLchar * ccPositionTexture_GrayScale_frag;

extern CC_DLL const GLchar * ccLabelDistanceFieldNormal_frag;
extern CC_DLL const GLchar * ccLabelDistanceFieldGlow_frag;
extern CC_DLL const GLchar * ccLabelNormal_frag;
extern CC_DLL const GLchar * ccLabelOutline_frag;

extern CC_DLL const GLchar * ccLabel_vert;

extern CC_DLL const GLchar * cc3D_PositionTex_vert;
extern CC_DLL const GLchar * cc3D_SkinPositionTex_vert;
extern CC_DLL const GLchar * cc3D_ColorTex_frag;
extern CC_DLL const GLchar * cc3D_Color_frag;
extern CC_DLL const GLchar * cc3D_PositionNormalTex_vert;
extern CC_DLL const GLchar * cc3D_SkinPositionNormalTex_vert;
extern CC_DLL const GLchar * cc3D_ColorNormalTex_frag;
extern CC_DLL const GLchar * cc3D_ColorNormal_frag;
extern CC_DLL const GLchar * cc3D_Particle_vert;
extern CC_DLL const GLchar * cc3D_Particle_tex_frag;
extern CC_DLL const GLchar * cc3D_Particle_color_frag;
extern CC_DLL const GLchar * cc3D_Skybox_vert;
extern CC_DLL const GLchar * cc3D_Skybox_frag;
extern CC_DLL const GLchar * cc3D_Terrain_vert;
extern CC_DLL const GLchar * cc3D_Terrain_frag;
extern CC_DLL const GLchar * ccCameraClearVert;
extern CC_DLL const GLchar * ccCameraClearFrag;
// ETC1 ALPHA supports.
extern CC_DLL const GLchar* ccETC1ASPositionTextureColor_frag;
extern CC_DLL const char* ccETC1ASPositionTextureGray_frag;
NS_CC_END
/**
 end of support group
 @}
 */
/// @endcond
#endif /* __CCSHADER_H__ */
