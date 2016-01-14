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
    
    virtual bool initWithFrame(const DRect& rect);
    
    virtual bool initWithCenter(const DRect& rect);
    
    virtual void onEnterTransitionDidFinish();
    
    CC_PROPERTY_PASS_BY_REF(string, m_nText, Text);
    
    CC_PROPERTY(unsigned int, m_nfontSize, FontSize);
    
	CC_PROPERTY(bool, m_bWordWrap, WordWrap);

	CC_PROPERTY(bool, m_bBold, Bold);

	CC_PROPERTY(bool, m_bUnderLine, UnderLine);

	CC_PROPERTY(bool, m_bDeleteLine, DeleteLine);

	CC_PROPERTY(bool, m_bItalics, Italics);

	CC_PROPERTY(int, m_iLineSpacing, LineSpacing);
    
    CC_PROPERTY_PASS_BY_REF(string, m_nfontName, FontName);
    
    CC_PROPERTY_PASS_BY_REF(CAVerticalTextAlignment, m_nVerticalTextAlignmet, VerticalTextAlignmet);
    
    CC_PROPERTY_PASS_BY_REF(CATextAlignment, m_nTextAlignment, TextAlignment);
    
    CC_PROPERTY_PASS_BY_REF(DSize, m_nDimensions, Dimensions);
    
    CC_PROPERTY(unsigned int, m_nNumberOfLine, NumberOfLine);

	CC_SYNTHESIZE(bool, m_bEnableCopy, EnableCopy);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(DSize, m_cLabelSize, LabelSize);

    virtual void visitEve();
    
    void sizeToFit();
    
    void unsizeToFit();
    
    void setColor(const CAColor4B& var);
    
    const CAColor4B& getColor();
    
	void applyStyle(const string& sStyleName);

	void applyStyle(const CALabelStyle* pLabelStyle);
    
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
    
    CAColor4B m_cFontColor;
};

NS_CC_END
#endif /* defined(__project__CALabel__) */
