//
//  CARichLabel.h
//  CrossApp
//
//  Created by ÷ÏΩ® on 16-2-22.
//  Copyright (c) 2016 http://www.9miao.com All rights reserved.
//


#ifndef __CrossApp__CARichLabel__
#define __CrossApp__CARichLabel__

#include "CAView.h"
#include "controller/CAFont.h"

NS_CC_BEGIN

class CC_DLL CARichLabel : public CAView
{
public:
    
    struct CC_DLL Element
    {
        Element(const std::string& text, const CAFont& font);

        std::string text;
        CAFont font;
    };
    
public:
    
	CARichLabel();
    
	virtual ~CARichLabel();

	static CARichLabel* create();

	static CARichLabel* createWithFrame(const DRect& rect);

	static CARichLabel* createWithCenter(const DRect& rect);

    static CARichLabel* createWithLayout(const DLayout& layout);
    
	void appendText(const std::string& text, const CAFont& font);

    void clear();

    void setVerticalTextAlignmet(const CAVerticalTextAlignment& var);
    
    void setTextAlignment(const CATextAlignment& var);
    
    void setNumberOfLine(unsigned int var);
    
    const DSize& getLabelSize();
    
    virtual void visitEve();

protected:
    
	virtual void setContentSize(const DSize& var);

	virtual void updateImageRect();

    void updateImageDraw();
    
    void updateImage();

private:
    
	bool m_bUpdateImage;

    CATextAlignment         m_eTextAlignment;
    
    CAVerticalTextAlignment m_eVerticalTextAlignment;
    
    DSize                   m_obPadding;
    
    bool                    m_bFitFlag;
    
    unsigned int m_iNumberOfLine;
    
    DSize m_obLabelSize;
    
    std::vector<Element> m_vElements;
    
};


NS_CC_END

#endif
