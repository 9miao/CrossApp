

#ifndef __CCTYPES_H__
#define __CCTYPES_H__

#include <string>
#include "basics/CASize.h"
#include "basics/CAPoint.h"
#include "basics/CAPoint3D.h"
#include "basics/CARect.h"
#include "basics/CAColor.h"
#include "CCGL.h"


NS_CC_BEGIN

/** A texcoord composed of 2 floats: u, y
 @since v0.8
 */
typedef struct _ccTex2F {
     GLfloat u;
     GLfloat v;
} ccTex2F;

static inline ccTex2F tex2(const float u, const float v)
{
    ccTex2F t = {u , v};
    return t;
}

 
//! Point Sprite component
typedef struct _DPointSprite
{
    DPoint    pos;        // 8 bytes
    CAColor4B    color;        // 4 bytes
    GLfloat        size;        // 4 bytes
} DPointSprite;

//!    A 2D Quad. 4 * 2 floats
typedef struct _ccQuad2 {
    DPoint        tl;
    DPoint        tr;
    DPoint        bl;
    DPoint        br;
} ccQuad2;


//!    A 3D Quad. 4 * 3 floats
typedef struct _ccQuad3 {
    DPoint3D        bl;
    DPoint3D        br;
    DPoint3D        tl;
    DPoint3D        tr;
} ccQuad3;

//! a Point with a vertex point, a tex coord point and a color 4B
typedef struct _ccV2F_C4B_T2F
{
    //! vertices (2F)
    DPoint        vertices;
    //! colors (4B)
    CAColor4B        colors;
    //! tex coords (2F)
    ccTex2F            texCoords;
} ccV2F_C4B_T2F;

//! a Point with a vertex point, a tex coord point and a color 4F
typedef struct _ccV2F_C4F_T2F
{
    //! vertices (2F)
    DPoint        vertices;
    //! colors (4F)
    CAColor4F        colors;
    //! tex coords (2F)
    ccTex2F            texCoords;
} ccV2F_C4F_T2F;

//! a Point with a vertex point, a tex coord point and a color 4B
typedef struct _ccV3F_C4B_T2F
{
    //! vertices (3F)
    DPoint3D        vertices;            // 12 bytes
//    char __padding__[4];

    //! colors (4B)
    CAColor4B        colors;                // 4 bytes
//    char __padding2__[4];

    // tex coords (2F)
    ccTex2F            texCoords;            // 8 bytes
} ccV3F_C4B_T2F;

//! A Triangle of ccV2F_C4B_T2F
typedef struct _ccV2F_C4B_T2F_Triangle
{
	//! Point A
	ccV2F_C4B_T2F a;
	//! Point B
	ccV2F_C4B_T2F b;
	//! Point B
	ccV2F_C4B_T2F c;
} ccV2F_C4B_T2F_Triangle;

//! A Quad of ccV2F_C4B_T2F
typedef struct _ccV2F_C4B_T2F_Quad
{
    //! bottom left
    ccV2F_C4B_T2F    bl;
    //! bottom right
    ccV2F_C4B_T2F    br;
    //! top left
    ccV2F_C4B_T2F    tl;
    //! top right
    ccV2F_C4B_T2F    tr;
} ccV2F_C4B_T2F_Quad;

//! 4 DPoint3DTex2FColor4B
typedef struct _ccV3F_C4B_T2F_Quad
{
    //! top left
    ccV3F_C4B_T2F    tl;
    //! bottom left
    ccV3F_C4B_T2F    bl;
    //! top right
    ccV3F_C4B_T2F    tr;
    //! bottom right
    ccV3F_C4B_T2F    br;
} ccV3F_C4B_T2F_Quad;

//! 4 DPointTex2FColor4F Quad
typedef struct _ccV2F_C4F_T2F_Quad
{
    //! bottom left
    ccV2F_C4F_T2F    bl;
    //! bottom right
    ccV2F_C4F_T2F    br;
    //! top left
    ccV2F_C4F_T2F    tl;
    //! top right
    ccV2F_C4F_T2F    tr;
} ccV2F_C4F_T2F_Quad;


struct CC_DLL BlendFunc
{
    /** source blend function */
    GLenum src;
    /** destination blend function */
    GLenum dst;

    bool operator==(const BlendFunc &a) const
    {
        return src == a.src && dst == a.dst;
    }
    
    bool operator!=(const BlendFunc &a) const
    {
        return src != a.src || dst != a.dst;
    }
    
    bool operator<(const BlendFunc &a) const
    {
        return src < a.src || (src == a.src && dst < a.dst);
    }
};

const BlendFunc BlendFunc_disable = {GL_ONE, GL_ZERO};
const BlendFunc BlendFunc_alpha_premultiplied = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
const BlendFunc BlendFunc_alpha_non_premultiplied = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
const BlendFunc BlendFunc_additive = {GL_SRC_ALPHA, GL_ONE};

static const BlendFunc kCCBlendFuncDisable = {GL_ONE, GL_ZERO};

// types for animation in particle systems

// texture coordinates for a quad
typedef struct _ccT2F_Quad
{
    //! bottom left
    ccTex2F    bl;
    //! bottom right
    ccTex2F    br;
    //! top left
    ccTex2F    tl;
    //! top right
    ccTex2F    tr;
} ccT2F_Quad;

// struct that holds the size in pixels, texture coordinates and delays for animated CCParticleSystemQuad
typedef struct
{
    ccT2F_Quad texCoords;
    float delay;
    DSize size; 
} CACustomAnimationFrameData;

enum class CAStatusBarStyle
{
    Default          = 0, // Dark content, for use on light backgrounds
    LightContent     = 1, // Light content, for use on dark backgrounds
};

enum class CAInterfaceOrientation
{
    Unknown        = 0,
    Portrait       = 1,
    Landscape      = 2,
};

static const char* CAApplicationDidChangeStatusBarOrientationNotification = "CAApplicationDidChangeStatusBarOrientationNotification";
static const char* CROSSAPP_CCLOG_NOTIFICATION = "CROSSAPP_CCLOG_NOTIFICATION";
//script
namespace script
{
    static int viewDidLoad = 0x11;
    static int viewDidUnload = 0x12;
    static int viewSizeDidChanged = 0x13;
    static int viewDidAppear = 0x14;
    static int viewDidDisappear = 0x15;
}



#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#ifdef _WIN64
typedef signed __int64    ssize_t;
#else  /* _WIN64 */
typedef _W64 signed int   ssize_t;
#endif  /* _WIN64 */
#endif

NS_CC_END

#endif //__CCTYPES_H__
