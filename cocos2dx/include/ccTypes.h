/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CCTYPES_H__
#define __CCTYPES_H__

#include <string>
#include "cocoa/CCGeometry.h"
#include "CCGL.h"


NS_CC_BEGIN

/** RGBA color composed of 4 bytes
@since v0.8
*/
typedef struct _Color4B
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} CAColor4B;
//! helper macro that creates an CAColor4B type
static inline CAColor4B
ccc4(const GLubyte r, const GLubyte g, const GLubyte b, const GLubyte o)
{
    CAColor4B c = {r, g, b, o};
    return c;
}

//CAColor4B predefined colors
//! White color (255,255,255,255)
static const CAColor4B CAColor_white   = {255, 255, 255, 255};
//! Yellow color (255,255,0,255)
static const CAColor4B CAColor_yellow  = {255, 255,   0, 255};
//! Magenta Color (255,0,255,255)
static const CAColor4B CAColor_magenta = {255,   0, 255, 255};
//! Red Color (255,0,0,255)
static const CAColor4B CAColor_red     = {255,   0,   0, 255};
//! Green Color (0,255,0,255)
static const CAColor4B CAColor_green   = {  0, 255,   0, 255};
//! Blue color (0,0,255,255)
static const CAColor4B CAColor_blue    = {  0,   0, 255, 255};
//! Black Color (0,0,0,255)
static const CAColor4B CAColor_black   = {  0,   0,   0, 255};
//! Orange Color (255,127,0,255)
static const CAColor4B CAColor_orange  = {255, 127,   0, 255};
//! Gray Color (166,166,166,255)
static const CAColor4B CAColor_gray    = {166, 166, 166, 255};
//! Gray Color (0,0,0,0)
static const CAColor4B CAColor_clear   = {  0,   0,   0,   0};

/** RGBA color composed of 4 floats
@since v0.8
*/
typedef struct _Color4F {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
} CAColor4F;


//! helper that creates a CAColor4F type
static inline CAColor4F 
ccc4f(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a)
{
    CAColor4F c4 = {r, g, b, a};
    return c4;
}

/** Returns a CAColor4F from a CAColor4B.
 @since v0.99.1
 */
static inline CAColor4F ccc4FFromccc4B(CAColor4B c)
{
    CAColor4F c4 = {c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f};
    return c4;
}

static inline CAColor4B ccc4BFromccc4F(CAColor4F c)
{
    CAColor4B ret = {(GLubyte)(c.r*255), (GLubyte)(c.g*255), (GLubyte)(c.b*255), (GLubyte)(c.a*255)};
	return ret;
}


/** returns YES if both CAColor4F are equal. Otherwise it returns NO.
 @since v0.99.1
 */
static inline bool ccc4FEqual(CAColor4F a, CAColor4F b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

/** A vertex composed of 2 floats: x, y
 @since v0.8
 */
typedef struct _ccVertex2F
{
    GLfloat x;
    GLfloat y;
} ccVertex2F;

static inline ccVertex2F vertex2(const float x, const float y)
{
    ccVertex2F c = {x, y};
    return c;
}


/** A vertex composed of 2 floats: x, y
 @since v0.8
 */
typedef struct _ccVertex3F
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
} ccVertex3F;

static inline ccVertex3F vertex3(const float x, const float y, const float z)
{
    ccVertex3F c = {x, y, z};
    return c;
}
        
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
typedef struct _ccPointSprite
{
    ccVertex2F    pos;        // 8 bytes
    CAColor4B    color;        // 4 bytes
    GLfloat        size;        // 4 bytes
} ccPointSprite;

//!    A 2D Quad. 4 * 2 floats
typedef struct _ccQuad2 {
    ccVertex2F        tl;
    ccVertex2F        tr;
    ccVertex2F        bl;
    ccVertex2F        br;
} ccQuad2;


//!    A 3D Quad. 4 * 3 floats
typedef struct _ccQuad3 {
    ccVertex3F        bl;
    ccVertex3F        br;
    ccVertex3F        tl;
    ccVertex3F        tr;
} ccQuad3;

//! a Point with a vertex point, a tex coord point and a color 4B
typedef struct _ccV2F_C4B_T2F
{
    //! vertices (2F)
    ccVertex2F        vertices;
    //! colors (4B)
    CAColor4B        colors;
    //! tex coords (2F)
    ccTex2F            texCoords;
} ccV2F_C4B_T2F;

//! a Point with a vertex point, a tex coord point and a color 4F
typedef struct _ccV2F_C4F_T2F
{
    //! vertices (2F)
    ccVertex2F        vertices;
    //! colors (4F)
    CAColor4F        colors;
    //! tex coords (2F)
    ccTex2F            texCoords;
} ccV2F_C4F_T2F;

//! a Point with a vertex point, a tex coord point and a color 4B
typedef struct _ccV3F_C4B_T2F
{
    //! vertices (3F)
    ccVertex3F        vertices;            // 12 bytes
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

//! 4 ccVertex3FTex2FColor4B
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

//! 4 ccVertex2FTex2FColor4F Quad
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

//! Blend Function used for textures
typedef struct _ccBlendFunc
{
    //! source blend function
    GLenum src;
    //! destination blend function
    GLenum dst;
} ccBlendFunc;

static const ccBlendFunc kCCBlendFuncDisable = {GL_ONE, GL_ZERO};

// XXX: If any of these enums are edited and/or reordered, update CAImage.m
//! Vertical text alignment type
typedef enum
{
    CAVerticalTextAlignmentTop,
    CAVerticalTextAlignmentCenter,
    CAVerticalTextAlignmentBottom,
} CAVerticalTextAlignment;

// XXX: If any of these enums are edited and/or reordered, update CAImage.m
//! Horizontal text alignment type
typedef enum
{
    CATextAlignmentLeft,
    CATextAlignmentCenter,
    CATextAlignmentRight,
} CATextAlignment;

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
    CCSize size; 
} ccAnimationFrameData;



/**
 types used for defining fonts properties (i.e. font name, size, stroke or shadow)
 */

// shadow attributes
typedef struct _ccFontShadow
{
public:
    
    // shadow is not enabled by default
    _ccFontShadow(): m_shadowEnabled(false) {}
    
    // true if shadow enabled
    bool   m_shadowEnabled;
    // shadow x and y offset
	CCSize m_shadowOffset;
    // shadow blurrines
	float  m_shadowBlur;
    // shadow opacity
	float  m_shadowOpacity;
    
} ccFontShadow;

// stroke attributes
typedef struct _ccFontStroke
{
public:
    
    // stroke is disabled by default
    _ccFontStroke(): m_strokeEnabled(false) {}
    
    // true if stroke enabled
    bool        m_strokeEnabled;
    // stroke color
	CAColor4B   m_strokeColor;
    // stroke size
    float       m_strokeSize;
    
} ccFontStroke;

// font attributes
/**
 *  @js NA
 *  @lua NA
 */
typedef struct _ccFontDefinition
{
public:
    
    _ccFontDefinition():  m_alignment(CATextAlignmentCenter),
    m_vertAlignment(CAVerticalTextAlignmentTop),
    m_fontFillColor(CAColor_white)
    { m_dimensions = CCSizeMake(0,0); }
    
    // font name
    std::string             m_fontName;
    // font size
    int                     m_fontSize;
    // horizontal alignment
    CATextAlignment         m_alignment;
    // vertical alignment
    CAVerticalTextAlignment m_vertAlignment;
    // renering box
    CCSize                  m_dimensions;
    // font color
    CAColor4B               m_fontFillColor;
    // font shadow
    ccFontShadow            m_shadow;
    // font stroke
    ccFontStroke            m_stroke;
    
} ccFontDefinition;


NS_CC_END

#endif //__CCTYPES_H__
