
#include "renderer/ccShaders.h"

#define STRINGIFY(A)  #A

NS_CC_BEGIN
//

#include "renderer/ccShader_Position_uColor.frag"

#include "renderer/ccShader_Position_uColor.vert"

//
#include "renderer/ccShader_PositionColor.frag"
#include "renderer/ccShader_PositionColor.vert"

//
#include "renderer/ccShader_PositionColorTextureAsPointsize.vert"

//
#include "renderer/ccShader_PositionTexture.frag"
#include "renderer/ccShader_PositionTexture.vert"

//
#include "renderer/ccShader_PositionTextureA8Color.frag"
#include "renderer/ccShader_PositionTextureA8Color.vert"

//
#include "renderer/ccShader_PositionTextureColor.frag"
#include "renderer/ccShader_PositionTextureColor.vert"

//
#include "renderer/ccShader_PositionTextureColor_noMVP.frag"
#include "renderer/ccShader_PositionTextureColor_noMVP.vert"

//
#include "renderer/ccShader_PositionTextureColorAlphaTest.frag"

//
#include "renderer/ccShader_PositionTexture_uColor.frag"
#include "renderer/ccShader_PositionTexture_uColor.vert"

#include "renderer/ccShader_PositionColorLengthTexture.frag"
#include "renderer/ccShader_PositionColorLengthTexture.vert"

#include "renderer/ccShader_UI_Gray.frag"
//
#include "renderer/ccShader_Label.vert"
#include "renderer/ccShader_Label_df.frag"
#include "renderer/ccShader_Label_df_glow.frag"
#include "renderer/ccShader_Label_normal.frag"
#include "renderer/ccShader_Label_outline.frag"

//
#include "renderer/ccShader_3D_PositionTex.vert"
#include "renderer/ccShader_3D_Color.frag"
#include "renderer/ccShader_3D_ColorTex.frag"
#include "renderer/ccShader_3D_PositionNormalTex.vert"
#include "renderer/ccShader_3D_ColorNormal.frag"
#include "renderer/ccShader_3D_ColorNormalTex.frag"
#include "renderer/ccShader_3D_Particle.vert"
#include "renderer/ccShader_3D_Particle.frag"
#include "renderer/ccShader_3D_Skybox.vert"
#include "renderer/ccShader_3D_Skybox.frag"
#include "renderer/ccShader_3D_Terrain.vert"
#include "renderer/ccShader_3D_Terrain.frag"
#include "renderer/ccShader_CameraClear.vert"
#include "renderer/ccShader_CameraClear.frag"

// ETC1 ALPHA support
#include "renderer/ccShader_ETC1AS_PositionTextureColor.frag"
#include "renderer/ccShader_ETC1AS_PositionTextureGray.frag"
NS_CC_END
