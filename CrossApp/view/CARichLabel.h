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


NS_CC_BEGIN


class CC_DLL CAFont
{
public:
	CAFont();

	CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sFontName, FontName);

	CC_SYNTHESIZE(unsigned int, m_nfontSize, FontSize);

	CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_cTextColor, TextColor);
	
	CC_SYNTHESIZE(bool, m_bItalics, Italics);

	CC_SYNTHESIZE(bool, m_bBold, Bold);

	CC_SYNTHESIZE(bool, m_bUnderLine, UnderLine);

	CC_SYNTHESIZE(bool, m_bDeleteLine, DeleteLine);
};


class CC_DLL CARichLabel : public CAView
{
public:
    
	CARichLabel();
    
	virtual ~CARichLabel();

	static CARichLabel* create();

	static CARichLabel* createWithFrame(const DRect& rect);

	static CARichLabel* createWithCenter(const DRect& rect);

	virtual bool initWithFrame(const DRect& rect);

	virtual bool initWithCenter(const DRect& rect);

	virtual void visitEve();

	virtual bool init();

	void appendText(const std::string& cszText);
	void appendText(const std::string& text, const CAFont& font);

protected:
	virtual void setContentSize(const DSize& var);

	virtual void updateImageRect();

	void updateImageDraw();

	void updateImage();

	float getMaxFontHeight();

private:
	bool m_bUpdateImage;
	float m_fTextHeight;
	std::vector<std::pair<std::string, CAFont> > m_vLabelFontVect;
};


NS_CC_END

#endif
