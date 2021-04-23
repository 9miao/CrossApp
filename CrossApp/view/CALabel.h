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

class CALabelStyle;

class CC_DLL CALabel : public CAView
{
public:
    
    CALabel();
    
    virtual ~CALabel();
    
    static CALabel* create();
    
    static CALabel* createWithFrame(const DRect& rect);
    
    static CALabel* createWithCenter(const DRect& rect);
    
    static CALabel* createWithLayout(const DLayout& layout);
    
    virtual bool init();

    CC_PROPERTY_PASS_BY_REF(string, m_sText, Text);
    
    CC_PROPERTY_PASS_BY_REF(CAFont, m_obFont, Font);
    
    CC_PROPERTY(unsigned int, m_nNumberOfLine, NumberOfLine);
    
    CC_SYNTHESIZE(bool, m_bEnableCopy, EnableCopy);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(DSize, m_obLabelSize, LabelSize);
    
    virtual void setColor(const CAColor4B& var);
    
    virtual const CAColor4B& getColor();
    
    void setFontSize(float var);
    
    void setFontName(const std::string& var);
    
    void setBold(bool var);

    void setUnderLine(bool var);

    void setDeleteLine(bool var);
    
    void setItalics(bool var);
    
    void setWordWrap(bool var);
    
    void setVerticalTextAlignmet(const CAVerticalTextAlignment& var);
    
    void setTextAlignment(const CATextAlignment& var);
    
    void setLineSpacing(float var);
    
    virtual void visitEve();
    
    void sizeToFit();
    
    void unsizeToFit();
    
protected:

    virtual void setContentSize(const DSize& var);

	virtual void updateImageRect();
    
    void updateImageDraw();
    
    void updateImage();

	void copySelectText();
    
    CATextAlignment         m_eTextAlignment;
    
    CAVerticalTextAlignment m_eVerticalTextAlignment;
    
    DSize                   m_obPadding;
    
    bool                    m_bFitFlag;
    
    bool                    m_bUpdateImage;
};

NS_CC_END
#endif /* defined(__project__CALabel__) */
