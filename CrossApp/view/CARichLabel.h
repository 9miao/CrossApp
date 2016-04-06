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

typedef struct LabelElement
{
	LabelElement()
		: nHyperlink(0)
	{

	}
	LabelElement(const std::string& text, const CAFont& ft)
		: cszText(text)
		, font(ft)
		, nHyperlink(0)
	{

	}
	LabelElement(const std::string& text, const CAFont& ft, short link)
		: cszText(text)
		, font(ft)
		, nHyperlink(link)
	{

	}
	std::string cszText;
	CAFont font;
	short nHyperlink;
	std::vector<DRect> vHyperlinkRects;

} LabelElement;

class CC_DLL CARichLabel : public CAView
{
public:
    
	CARichLabel();
    
	virtual ~CARichLabel();

	static CARichLabel* create();

	static CARichLabel* createWithFrame(const DRect& rect);

	static CARichLabel* createWithCenter(const DRect& rect);

    static CARichLabel* createWithLayout(const DRectLayout& layout);
    
	virtual bool initWithFrame(const DRect& rect);

	virtual bool initWithCenter(const DRect& rect);

	virtual void visitEve();

	virtual bool init();

	void appendText(const std::string& text);
	void appendText(const std::string& text, const CAFont& font);
    
    void clear();

	bool getAutoLinkMask();
	void setAutoLinkMask(bool var);

	bool getLinksClickable();
	void setLinksClickable(bool var);

	const CAColor4B& getLinkTextColor();
	void setLinkTextColor(const CAColor4B& col);

	const CAColor4B& getLinkVisitedTextColor();
	void setLinkVisitedTextColor(const CAColor4B& col);

	std::vector<std::string> getUrls();
    
protected:
	virtual void setContentSize(const DSize& var);

	virtual void updateImageRect();

	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);

	void updateImageDraw();

	void updateImage();

	float getMaxFontHeight();

	void splitUrlStrings(const std::string& text, std::vector<std::pair<int, int> >& vIntVect);

	void appendTextEx(const std::string& text, const CAFont& font);

private:
	bool m_bUpdateImage;
	float m_fTextHeight;
	bool m_bAutoLinkMask;
	bool m_bLinksClickable;
	CAColor4B m_linkColor;
	CAColor4B m_linkVisitedColor;
	std::vector<LabelElement> m_vLabelFontVect;
};


NS_CC_END

#endif
