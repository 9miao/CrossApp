//
//  CALabel.h
//  project
//
//  Created by lh on 14-5-12.
//
//

#ifndef __project__CALabel__
#define __project__CALabel__

#include <iostream>

#include "cocos2d.h"
#include "sprite_nodes/CAView.h"
USING_NS_CC;
using namespace std;
typedef struct _caFontDefinition
{
public:
    
    _caFontDefinition():  m_alignment(kCCTextAlignmentCenter),
    m_vertAlignment(kCCVerticalTextAlignmentTop),
    m_fontFillColor(ccc4(255, 255, 255, 255))
    { m_dimensions = CCSizeMake(0,0); }
    
    // font name
    std::string             m_fontName;
    // font size
    int                     m_fontSize;
    // horizontal alignment
    CCTextAlignment         m_alignment;
    // vertical alignment
    CCVerticalTextAlignment m_vertAlignment;
    // renering box
    CCSize                  m_dimensions;
    // font color
    ccColor4B               m_fontFillColor;
    // font shadow
    
} caFontDefinition;

class CALabel:public CAView
{
public:
    CALabel();
    
    virtual ~CALabel();
    
public:
    static CALabel *create(CCRect frame);
    
    virtual bool initWithFrame(CCRect frame);
    
    CC_PROPERTY(string, m_nText, Text);
    
    CC_SYNTHESIZE(unsigned int, m_nfontSize, fontSize);
    
    CC_SYNTHESIZE(string, m_nfontName, fontName);
    
    CC_PROPERTY(ccColor4B, m_nTextcolor, Textcolor);
    
    CC_PROPERTY(ccColor4B, m_nShadowcolor, Shadowcolor);

    CC_PROPERTY(CCSize, m_nShadowOffset, ShadowOffset);
    
    CC_SYNTHESIZE(CCVerticalTextAlignment, m_nvTextAlignmet, vTextAlignmet);
    
    CC_PROPERTY(CCTextAlignment, m_nTextAlignment, TextAlignment);
    
    CC_SYNTHESIZE(CCSize, m_nDimensions, Dimensions);
    
    CC_PROPERTY(unsigned int, m_nNumberOfLine, NumberOfLine);
    
    CCSprite *m_pTextTexture;
private:
    
    bool updataTexture();
    
    virtual void onEnterTransitionDidFinish();
    
    caFontDefinition setFontDefiniton(bool flag);
};

#endif /* defined(__project__CALabel__) */
