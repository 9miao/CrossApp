

#include "CAShaderCache.h"
#include "CAGLProgram.h"
#include "ccMacros.h"
#include "ccShaders.h"


NS_CC_BEGIN

enum {
    kCCShaderType_PositionTextureColor,
    kCCShaderType_PositionTextureColorAlphaTest,
    kCCShaderType_PositionColor,
    kCCShaderType_PositionTexture,
    kCCShaderType_PositionTexture_uColor,
    kCCShaderType_PositionTextureA8Color,
    kCCShaderType_Position_uColor,
    kCCShaderType_PositionLengthTexureColor,
    kCCShaderType_ControlSwitch,
    
    kCCShaderType_MAX,
};

static CAShaderCache *_sharedShaderCache = 0;

CAShaderCache* CAShaderCache::sharedShaderCache()
{
    if (!_sharedShaderCache) {
        _sharedShaderCache = new CAShaderCache();
        if (!_sharedShaderCache->init())
        {
            CC_SAFE_DELETE(_sharedShaderCache);
        }
    }
    return _sharedShaderCache;
}

void CAShaderCache::purgeSharedShaderCache()
{
    CC_SAFE_RELEASE_NULL(_sharedShaderCache);
}

CAShaderCache::CAShaderCache()
{

}

CAShaderCache::~CAShaderCache()
{
    CCLOGINFO("CrossApp deallocing 0x%X", this);
}



bool CAShaderCache::init()
{
    loadDefaultShaders();
    return true;
}

void CAShaderCache::loadDefaultShaders()
{
    // Position Image Color shader
    CAGLProgram *p = new CAGLProgram();
    loadDefaultShader(p, kCCShaderType_PositionTextureColor);

    m_mPrograms.insert(kCCShader_PositionTextureColor, p);
    p->release();

    // Position Image Color alpha test
    p = new CAGLProgram();
    loadDefaultShader(p, kCCShaderType_PositionTextureColorAlphaTest);
    m_mPrograms.insert(kCCShader_PositionTextureColorAlphaTest, p);
    p->release();

    //
    // Position, Color shader
    //
    p = new CAGLProgram();
    loadDefaultShader(p, kCCShaderType_PositionColor);
    m_mPrograms.insert(kCCShader_PositionColor, p);
    p->release();

    //
    // Position Image shader
    //
    p = new CAGLProgram();
    loadDefaultShader(p, kCCShaderType_PositionTexture);
    m_mPrograms.insert(kCCShader_PositionTexture, p);
    p->release();

    //
    // Position, Image attribs, 1 Color as uniform shader
    //
    p = new CAGLProgram();
    loadDefaultShader(p, kCCShaderType_PositionTexture_uColor);
    m_mPrograms.insert(kCCShader_PositionTexture_uColor, p);
    p->release();

    //
    // Position Image A8 Color shader
    //
    p = new CAGLProgram();
    loadDefaultShader(p, kCCShaderType_PositionTextureA8Color);
    m_mPrograms.insert(kCCShader_PositionTextureA8Color, p);
    p->release();

    //
    // Position and 1 color passed as a uniform (to simulate glColor4ub )
    //
    p = new CAGLProgram();
    loadDefaultShader(p, kCCShaderType_Position_uColor);
    m_mPrograms.insert(kCCShader_Position_uColor, p);
    p->release();
    
    //
	// Position, Legth(TexCoords, Color (used by Draw Node basically )
	//
    p = new CAGLProgram();
    loadDefaultShader(p, kCCShaderType_PositionLengthTexureColor);
    m_mPrograms.insert(kCCShader_PositionLengthTexureColor, p);
    p->release();

    //
	// ControlSwitch
	//
    p = new CAGLProgram();
    loadDefaultShader(p, kCCShaderType_ControlSwitch);
    m_mPrograms.insert(kCCShader_ControlSwitch, p);
    p->release();
}



void CAShaderCache::reloadDefaultShaders()
{
    // reset all programs and reload them
    
    // Position Image Color shader
    CAGLProgram *p = programForKey(kCCShader_PositionTextureColor);    
    p->reset();
    loadDefaultShader(p, kCCShaderType_PositionTextureColor);

    // Position Image Color alpha test
    p = programForKey(kCCShader_PositionTextureColorAlphaTest);
    p->reset();    
    loadDefaultShader(p, kCCShaderType_PositionTextureColorAlphaTest);
    
    //
    // Position, Color shader
    //
    p = programForKey(kCCShader_PositionColor);
    p->reset();
    loadDefaultShader(p, kCCShaderType_PositionColor);
    
    //
    // Position Image shader
    //
    p = programForKey(kCCShader_PositionTexture);
    p->reset();
    loadDefaultShader(p, kCCShaderType_PositionTexture);
    
    //
    // Position, Image attribs, 1 Color as uniform shader
    //
    p = programForKey(kCCShader_PositionTexture_uColor);
    p->reset();
    loadDefaultShader(p, kCCShaderType_PositionTexture_uColor);
    
    //
    // Position Image A8 Color shader
    //
    p = programForKey(kCCShader_PositionTextureA8Color);
    p->reset();
    loadDefaultShader(p, kCCShaderType_PositionTextureA8Color);
    
    //
    // Position and 1 color passed as a uniform (to simulate glColor4ub )
    //
    p = programForKey(kCCShader_Position_uColor);
    p->reset();
    loadDefaultShader(p, kCCShaderType_Position_uColor);
    
    //
	// Position, Legth(TexCoords, Color (used by Draw Node basically )
	//
    p = programForKey(kCCShader_PositionLengthTexureColor);
    p->reset();
    loadDefaultShader(p, kCCShaderType_PositionLengthTexureColor);
}




void CAShaderCache::loadDefaultShader(CAGLProgram *p, int type)
{
    switch (type) {
        case kCCShaderType_PositionTextureColor:
            p->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccPositionTextureColor_frag);
            
            p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
            p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
            p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
            
            break;
        case kCCShaderType_PositionTextureColorAlphaTest:
            p->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccPositionTextureColorAlphaTest_frag);
            
            p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
            p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
            p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

            break;
        case kCCShaderType_PositionColor:  
            p->initWithVertexShaderByteArray(ccPositionColor_vert ,ccPositionColor_frag);
            
            p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
            p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);

            break;
        case kCCShaderType_PositionTexture:
            p->initWithVertexShaderByteArray(ccPositionTexture_vert ,ccPositionTexture_frag);
            
            p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
            p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

            break;
        case kCCShaderType_PositionTexture_uColor:
            p->initWithVertexShaderByteArray(ccPositionTexture_uColor_vert, ccPositionTexture_uColor_frag);
            
            p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
            p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

            break;
        case kCCShaderType_PositionTextureA8Color:
            p->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, ccPositionTextureA8Color_frag);
            
            p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
            p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
            p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

            break;
        case kCCShaderType_Position_uColor:
            p->initWithVertexShaderByteArray(ccPosition_uColor_vert, ccPosition_uColor_frag);    
            
            p->addAttribute("aVertex", kCCVertexAttrib_Position);    
            
            break;
        case kCCShaderType_PositionLengthTexureColor:
            p->initWithVertexShaderByteArray(ccPositionColorLengthTexture_vert, ccPositionColorLengthTexture_frag);
            
            p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
            p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
            p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
            
            break;

       case kCCShaderType_ControlSwitch:
            p->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccExSwitchMask_frag);

            p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
            p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
            p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

            break;

        default:
            CCLOG("CrossApp: %s:%d, error shader type", __FUNCTION__, __LINE__);
            return;
    }
    
    p->link();
    p->updateUniforms();
    
    CHECK_GL_ERROR_DEBUG();
}

CAGLProgram* CAShaderCache::programForKey(const char* key)
{
    return m_mPrograms.getValue(key);
}

void CAShaderCache::addProgram(CAGLProgram* program, const char* key)
{
    m_mPrograms.insert(key, program);
}

NS_CC_END
