/****************************************************************************
Copyright (c) 2010-2012  cocos2d-x.org
Copyright (c) 2011 ForzeField Studios S.L.

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
#ifndef __CCMOTION_STREAK_H__
#define __CCMOTION_STREAK_H__

#include "CCProtocols.h"
#include "images/CAImage.h"
#include "ccTypes.h"
#include "sprite_nodes/CAView.h"
#ifdef EMSCRIPTEN
#include "base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

/**
 * @addtogroup misc_nodes
 * @{
 */

/** MotionStreak.
 Creates a trailing path.
 */
class CC_DLL CCMotionStreak : public CAView, public CCTextureProtocol
#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
public:
    /**
     * @js ctor
     */
    CCMotionStreak();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCMotionStreak();

    /** creates and initializes a motion streak with fade in seconds, minimum segments, stroke's width, color, Image filename */
    static CCMotionStreak* create(float fade, float minSeg, float stroke, const CAColor4B& color, const char* path);
    /** creates and initializes a motion streak with fade in seconds, minimum segments, stroke's width, color, Image */
    static CCMotionStreak* create(float fade, float minSeg, float stroke, const CAColor4B& color, CAImage* image);

    /** initializes a motion streak with fade in seconds, minimum segments, stroke's width, color and Image filename */
    bool initWithFade(float fade, float minSeg, float stroke, const CAColor4B& color, const char* path);
    /** initializes a motion streak with fade in seconds, minimum segments, stroke's width, color and Image  */
    bool initWithFade(float fade, float minSeg, float stroke, const CAColor4B& color, CAImage* image);

    /** color used for the tint */
    void tintWithColor(CAColor4B colors);

    /** Remove all living segments of the ribbon */
    void reset();

    /** Override super methods */
    virtual void setPoint(const CCPoint& position);
    virtual void draw();
    virtual void update(float delta);

    /* Implement interfaces */
    virtual CAImage* getImage(void);
    virtual void setImage(CAImage* image);
    /**
     * @js NA
     */
    virtual void setBlendFunc(ccBlendFunc blendFunc);
    /**
     * @js NA
     */
    virtual ccBlendFunc getBlendFunc(void);
    
    virtual float getAlpha();
    virtual void setAlpha(float alpha);

    /** When fast mode is enabled, new points are added faster but with lower precision */
    inline bool isFastMode() { return m_bFastMode; }
    inline void setFastMode(bool bFastMode) { m_bFastMode = bFastMode; }

    inline bool isStartingPositionInitialized() { return m_bStartingPositionInitialized; }
    inline void setStartingPositionInitialized(bool bStartingPositionInitialized) 
    { 
        m_bStartingPositionInitialized = bStartingPositionInitialized; 
    }
protected:
    bool m_bFastMode;
    bool m_bStartingPositionInitialized;
private:
    /** Image used for the motion streak */
    CAImage* m_pImage;
    ccBlendFunc m_tBlendFunc;
    CCPoint m_tPositionR;

    float m_fStroke;
    float m_fFadeDelta;
    float m_fMinSeg;

    unsigned int m_uMaxPoints;
    unsigned int m_uNuPoints;
    unsigned int m_uPreviousNuPoints;

    /** Pointers */
    CCPoint* m_pPointVertexes;
    float* m_pPointState;

    // Opengl
    ccVertex2F* m_pVertices;
    GLubyte* m_pColorPointer;
    ccTex2F* m_pTexCoords;
};

// end of misc_nodes group
/// @}

NS_CC_END

#endif //__CCMOTION_STREAK_H__
