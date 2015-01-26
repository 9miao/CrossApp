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
class CALabelStyle;

class CC_DLL CALabel: public CAView
{
public:
    
    CALabel();
    
    virtual ~CALabel();
    
    static CALabel* createWithFrame(const CCRect& rect);
    
    static CALabel* createWithCenter(const CCRect& rect);
    
    virtual bool initWithFrame(const CCRect& rect);
    
    virtual bool initWithCenter(const CCRect& rect);
    
    virtual void onEnterTransitionDidFinish();
    
    CC_PROPERTY_PASS_BY_REF(string, m_nText, Text);
    
    CC_PROPERTY(unsigned int, m_nfontSize, FontSize);
    
	CC_PROPERTY(bool, m_bWordWrap, WordWrap);

	CC_PROPERTY(bool, m_bBold, Bold);

	CC_PROPERTY(bool, m_bUnderLine, UnderLine);

	CC_PROPERTY(bool, m_bItalics, Italics);

	CC_PROPERTY(int, m_iLineSpacing, LineSpacing);
    
    CC_PROPERTY_PASS_BY_REF(string, m_nfontName, FontName);
    
    CC_PROPERTY_PASS_BY_REF(CAVerticalTextAlignment, m_nVerticalTextAlignmet, VerticalTextAlignmet);
    
    CC_PROPERTY_PASS_BY_REF(CATextAlignment, m_nTextAlignment, TextAlignment);
    
    CC_PROPERTY_PASS_BY_REF(CCSize, m_nDimensions, Dimensions);
    
    CC_PROPERTY(unsigned int, m_nNumberOfLine, NumberOfLine);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCSize, m_cLabelSize, LabelSize);
    
    virtual void visit();
    
    void sizeToFit();
    
    void unsizeToFit();

	void applyStyle(const string& sStyleName);

	void applyStyle(const CALabelStyle* pLabelStyle);
    
    void updateImage();
    
protected:
    
    virtual void setContentSize(const CCSize& var);
    
    virtual void updateImageRect();
        
    bool m_bUpdateImage;
    
    float pTextHeight;
    
    bool m_bFitFlag;
};

NS_CC_END
#endif /* defined(__project__CALabel__) */
