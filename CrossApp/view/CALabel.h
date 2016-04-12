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
#include "view/CATextEditHelper.h"
#include "controller/CAFont.h"

NS_CC_BEGIN
using namespace std;
class CALabelStyle;

class CC_DLL CALabel : public CATouchView
{
public:
    
    CALabel();
    
    virtual ~CALabel();
    
    static CALabel* create();
    
    static CALabel* createWithFrame(const DRect& rect);
    
    static CALabel* createWithCenter(const DRect& rect);
    
    static CALabel* createWithLayout(const DLayout& layout);
    
    CC_PROPERTY_PASS_BY_REF(string, m_nText, Text);

    CC_PROPERTY_PASS_BY_REF(CAFont, m_obFont, Font);
    
    void setFontSize(unsigned int var);
    
    unsigned int getFontSize();
    
    void setFontName(const std::string& var);
    
    const std::string& getFontName();
    
    void setBold(bool var);
    
    bool isBold();

    void setUnderLine(bool var);
    
    bool isUnderLine();

    void setDeleteLine(bool var);
    
    bool isDeleteLine();
    
    void setItalics(bool var);
    
    bool isItalics();
    
    void setColor(const CAColor4B& var);
    
    const CAColor4B& getColor();

	CC_PROPERTY(int, m_iLineSpacing, LineSpacing);
    
    CC_PROPERTY_IS(bool, m_bWordWrap, WordWrap);
    
    CC_PROPERTY_PASS_BY_REF(CAVerticalTextAlignment, m_nVerticalTextAlignmet, VerticalTextAlignmet);
    
    CC_PROPERTY_PASS_BY_REF(CATextAlignment, m_nTextAlignment, TextAlignment);
    
    CC_PROPERTY_PASS_BY_REF(DSize, m_nDimensions, Dimensions);
    
    CC_PROPERTY(unsigned int, m_nNumberOfLine, NumberOfLine);

	CC_SYNTHESIZE(bool, m_bEnableCopy, EnableCopy);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(DSize, m_cLabelSize, LabelSize);

    virtual void visitEve();
    
    void sizeToFit();
    
    void unsizeToFit();
    
protected:

    virtual void setContentSize(const DSize& var);

	virtual void updateImageRect();

	virtual void ccTouchPress(CATouch *pTouch, CAEvent *pEvent);
    
    void updateImageDraw();
    
    void updateImage();

	void copySelectText();
    
    float pTextHeight;
    
    bool m_bFitFlag;
    
    bool m_bUpdateImage;
};

NS_CC_END
#endif /* defined(__project__CALabel__) */
