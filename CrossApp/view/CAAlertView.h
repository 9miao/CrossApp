//
//  CAAlertView.h
//  CrossApp
//
//  Created by Zhujian on 14-6-10.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//


#ifndef __CrossApp__CAAlertView__
#define __CrossApp__CAAlertView__

#include "view/CAView.h"
#include "view/CAScale9ImageView.h"
#include "controller/CABarItem.h"
#include "view/CATableView.h"
#include "view/CALabel.h"
#include <string>
#include <vector>

NS_CC_BEGIN

class CAButton;
class CAObject;
class CATableView;

#define ALERT_WINDOW_SIZEX 0.8
#define ALERT_WINDOW_SIZEY 0.4
#define MAX_BUTTON_COUNT_ROW 3


typedef void (CAObject::*SEL_CAAlertBtnEvent)(int iButtonIndex);
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

    bool init();
    
    void showMessage(const std::string &title, const std::string &alertMsg, const std::vector<std::string> &vBtnText);
    
    void setTarget(CAObject* target, SEL_CAAlertBtnEvent selector);
    
	void setMessageFontName(const std::string &var);

	void setTitle(const std::string &var, const CAColor4B &col = CAColor_black);

	void setTitleImage(CAImage* image);

	void setAlertMessage(const std::string &var, const CAColor4B &col = CAColor_black);

	void setContentBackGroundImage(CAImage* image);

	void initAllButton(const std::vector<CAButton*> &vbtns);

	void initAllButton(const std::vector<std::string> &vBtnText);

	void setAllBtnBackGroundImage(CAControlState controlState, CAImage* image);
    
	void setAllBtnTextColor(const CAColor4B &col = CAColor_white);

	void calcuCtrlsSize();

protected:
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
	void onClickButton(CAControl* btn, const CCPoint &point);

	void setCtrlImage(CAScale9ImageView*& pImageView, CAImage* image);

	void setLabel(CALabel*& pLabel, const std::string& szTitle, const CAColor4B &col);

	CCSize getAlertWinSize();

private:
    
	std::string m_sMsgFontName;

	CALabel* m_pTitleLabel;

	CALabel* m_pContentLabel;

	CAScale9ImageView* m_pTitleImage;

	CAScale9ImageView* m_pContentBkImage;

	std::vector<CAButton*> m_vAllBtn;

	CATableView* m_pBtnTableView;

	SEL_CAAlertBtnEvent m_pCAlertBtnEvent;
    
	CAObject* m_pCAlertTarget;
};

NS_CC_END

#endif /* defined(__cocos2dx__CAAlertView__) */