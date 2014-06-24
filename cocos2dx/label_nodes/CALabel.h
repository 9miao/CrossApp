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
    _caFontDefinition()
    : m_alignment(CATextAlignmentCenter),
    m_vertAlignment(CAVerticalTextAlignmentTop),
    m_fontFillColor(ccc4(255, 255, 255, 255))
    {
        m_dimensions = CCSizeMake(0,0);
    }
    
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
    
} caFontDefinition;

class CC_DLL CALabel: public CAView
{
public:
    
    CALabel();
    
    virtual ~CALabel();
    
    static CALabel* createWithFrame(const CCRect& rect);
    
    static CALabel* createWithCenter(const CCRect& rect);
    
    virtual bool init();
    
    virtual bool initWithFrame(const CCRect& rect);
    
    virtual bool initWithCenter(const CCRect& rect);
    
    virtual void setImageRect(const CCRect& rect);
    
    virtual void updateImageRect();
    
    CC_PROPERTY(string, m_nText, Text);
    
    CC_SYNTHESIZE(unsigned int, m_nfontSize, FontSize);
    
    CC_SYNTHESIZE(string, m_nfontName, FontName);
    
    CC_SYNTHESIZE(CAVerticalTextAlignment, m_nVerticalTextAlignmet, VerticalTextAlignmet);
    
    CC_PROPERTY(CATextAlignment, m_nTextAlignment, TextAlignment);
    
    CC_PROPERTY(CCSize, m_nDimensions, Dimensions);
    
    CC_PROPERTY(unsigned int, m_nNumberOfLine, NumberOfLine);
    
    CC_SYNTHESIZE_READONLY(CAImageView*, m_pTextImage, TextImage);

    CC_SYNTHESIZE_READONLY(CCSize, m_cLabelSize, LabelSize);
    
    bool isInput;
    
    bool updateImage();
    
private:
    CCRect m_rRect;
    
    virtual void onEnterTransitionDidFinish();
    
    caFontDefinition setFontDefiniton(bool flag);
};

NS_CC_END
#endif /* defined(__project__CALabel__) */
