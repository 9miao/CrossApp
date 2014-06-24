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

#include "CAView.h"

NS_CC_BEGIN
using namespace std;

class CC_DLL CALabel: public CAView
{
public:
    
    CALabel();
    
    virtual ~CALabel();
    
    static CALabel* createWithFrame(const CCRect& rect);
    
    static CALabel* createWithCenter(const CCRect& rect);
    
    virtual bool initWithFrame(const CCRect& rect);
    
    virtual bool initWithCenter(const CCRect& rect);
    
    virtual void setImageRect(const CCRect& rect);
    
    virtual void updateImageRect();
    
    CC_PROPERTY(string, m_nText, Text);
    
    CC_PROPERTY(unsigned int, m_nfontSize, FontSize);
    
    CC_PROPERTY(string, m_nfontName, FontName);
    
    CC_PROPERTY(CAVerticalTextAlignment, m_nVerticalTextAlignmet, VerticalTextAlignmet);
    
    CC_PROPERTY(CATextAlignment, m_nTextAlignment, TextAlignment);
    
    CC_PROPERTY(CCSize, m_nDimensions, Dimensions);
    
    CC_PROPERTY(unsigned int, m_nNumberOfLine, NumberOfLine);
    
    CC_SYNTHESIZE_READONLY(CCSize, m_cLabelSize, LabelSize);
    
    void updateImage();

    bool m_bUpdateImage;
    
    float pTextHeight;
    
    virtual void visit();
    
private:
    virtual void onEnterTransitionDidFinish();
};

NS_CC_END
#endif /* defined(__project__CALabel__) */
