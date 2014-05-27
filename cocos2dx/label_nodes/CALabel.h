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

#include "sprite_nodes/CAImageView.h"
NS_CC_BEGIN
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

class CC_DLL CALabel: public CAView
{
public:
    
    CALabel();
    
    virtual ~CALabel();
    
    static CALabel *create(CCRect frame);
    
    virtual bool initWithFrame(CCRect frame);
    
    CC_PROPERTY(string, m_nText, Text);
    
    CC_SYNTHESIZE(unsigned int, m_nfontSize, fontSize);
    
    CC_SYNTHESIZE(string, m_nfontName, fontName);
    
    CC_PROPERTY(ccColor4B, m_nTextcolor, Textcolor);
    
//    CC_PROPERTY(ccColor4B, m_nShadowcolor, Shadowcolor);
//
//    CC_PROPERTY(CCSize, m_nShadowOffset, ShadowOffset);
    
    CC_SYNTHESIZE(CCVerticalTextAlignment, m_nVerticalTextAlignmet, VerticalTextAlignmet);
    
    CC_PROPERTY(CCTextAlignment, m_nTextAlignment, TextAlignment);
    
    CC_PROPERTY(CCSize, m_nDimensions, Dimensions);
    
    CC_PROPERTY(unsigned int, m_nNumberOfLine, NumberOfLine);
    
    CC_SYNTHESIZE_READONLY(CAImageView*, m_pTextImage, TextImage);

    CC_SYNTHESIZE_READONLY(CCSize, m_cLabelSize, LabelSize);
    
    bool updateImage();
    
private:
    
    virtual void onEnterTransitionDidFinish();
    
    caFontDefinition setFontDefiniton(bool flag);
};

NS_CC_END
#endif /* defined(__project__CALabel__) */
