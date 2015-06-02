//
//  CAAlertView.h
//  CrossApp
//
//  Created by Zhujian on 14-6-10.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//


#ifndef __CrossAppx__CAAlertView__
#define __CrossAppx__CAAlertView__

#include <iostream>
#include "view/CAView.h"
#include "view/CAScale9ImageView.h"
#include "controller/CABarItem.h"
#include "view/CATableView.h"
#include "view/CALabel.h"
#include "view/CAClippingView.h"
#include <string>
#include <vector>

NS_CC_BEGIN

class CAButton;
class CAObject;
class CATableView;
class CAAlertViewDelegate;
class CAAlertView;


#define ALERT_VIEW_TITLE_FONT 35
#define ALERT_VIEW_MESG_FONT 30
#define ALERT_VIEW_WIDTH 540
#define ALERT_VIEW_MESG_WIDTH 440

typedef void (CAObject::*SEL_CAAlertBtnEvent)(int index);
#define CAAlertView_selector(_SELECTOR) (SEL_CAAlertBtnEvent)(&_SELECTOR)

class CC_DLL CAAlertView 
	: public CAView
	, public CATableViewDataSource
	, public CATableViewDelegate
{
public:

	CAAlertView();
	virtual ~CAAlertView();

	static CAAlertView *create();

	static CAAlertView *createWithText(const char* pszTitle, const char* pszAlertMsg, const char* pszBtnText, ...);
    
	bool initWithText(const char* szTitle, const char* szAlertMsg, const char* pszBtnText, ...);

	void addButton(const std::string& btnText, CAColor4B col = ccc4(3, 100, 255, 255), CAImage* pNormalImage = NULL, CAImage* pHighlightedImage = NULL);

	void addButton(CAButton* pBtn);

    void setTarget(CAObject* target, SEL_CAAlertBtnEvent selector);
	
	void show();

	//optional
	void setMessageFontName(std::string &var);

	void setTitle(std::string var, CAColor4B col = CAColor_black);

	void setAlertMessage(std::string var, CAColor4B col = CAColor_black);
   
protected:

	bool init();

    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
	void onClickButton(CAControl* btn, CCPoint point);

	void setLabel(CALabel*& pLabel, const char* szTitle, const char* fontName, const CAColor4B& col);

private:

	void showAlertView();

	void adjustButtonView();

	void addGrayLine(int y);

	void calcuAlerViewSize();

private:
    
	std::string m_sMsgFontName;

	CALabel* m_pTitleLabel;

	CALabel* m_pContentLabel;

	CAVector<CAButton*> m_vAllBtn;

	CATableView* m_pBtnTableView;

	SEL_CAAlertBtnEvent m_pCAlertBtnEvent;
    
	CAObject* m_pCAlertTarget;

	float m_fAlertViewHeight;

	float m_fAlertViewTitleHeight;

	float m_fAlertViewMessageHeight;

	float m_fAlertViewLineHeight;

	CAClippingView *m_pBackView;
};

NS_CC_END

#endif /* defined(__cocos2dx__CAAlertView__) */