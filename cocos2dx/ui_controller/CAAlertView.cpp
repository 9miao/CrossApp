
#include "CAAlertView.h"
#include "sprite_nodes/CAScale9ImageView.h"
#include "CCDirector.h"
#include "CAButton.h"
#include "label_nodes/CCLabelTTF.h"
#include "support/CCPointExtension.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionEase.h"
#include "touch_dispatcher/CCTouch.h"
#include "sprite_nodes/CAWindow.h"


NS_CC_BEGIN

CAAlertView::CAAlertView()
: m_pContentLabel(NULL)
, m_pTitleLabel(NULL)
, m_pTitleImage(NULL)
, m_pContentBkImage(NULL)
, m_pCAlertBtnEvent(NULL)
, m_pCAlertTarget(NULL)
, m_pBtnTableView(NULL)
, m_sMsgFontName("fonts/arial.ttf")
{

}


CAAlertView::~CAAlertView()
{
	this->removeAllSubviews();
	for (int i = 0; i < m_vAllBtn.size(); i++)
	{
		CC_SAFE_RELEASE_NULL(m_vAllBtn[i]);
	}
	CC_SAFE_DELETE(m_pTitleLabel);
	CC_SAFE_DELETE(m_pContentLabel);
	CC_SAFE_DELETE(m_pTitleImage);
	CC_SAFE_DELETE(m_pContentBkImage);
	CC_SAFE_DELETE(m_pBtnTableView);
}

bool CAAlertView::init()
{
	if (!CAView::init())
	{
		return false;
	}

	CCSize sz = CCDirector::sharedDirector()->getWinSize();
	setCenter(CCRectMake(sz.width / 2, sz.height / 2, sz.width, sz.height));
	return true;
}

CAAlertView *CAAlertView::create()
{
	CAAlertView *pAlert = new CAAlertView();
	if (pAlert && pAlert->init())
	{
		pAlert->setColor(ccc4(0, 0, 0, 0));
		pAlert->autorelease();
		return pAlert;
	}
	CC_SAFE_DELETE(pAlert);
	return NULL;
}

void CAAlertView::setMessageFontName(std::string var)
{
	if (m_pContentLabel==NULL)
	{
		return;
	}
	if (m_sMsgFontName.compare(var))
	{
		m_sMsgFontName = var;
		m_pContentLabel->setFontName(m_sMsgFontName.c_str());
	}
}

void CAAlertView::setTitle(std::string var, CAColor4B col)
{
	setLabel(m_pTitleLabel, var, col);
}

void CAAlertView::setTitleImage(std::string szImage)
{
	setCtrlImage(m_pTitleImage, CAImage::create(szImage.c_str()));
}

void CAAlertView::setTitleImage(CAImage* image)
{
	setCtrlImage(m_pTitleImage, image);
}

void CAAlertView::setAlertMessage(std::string var, CAColor4B col)
{
	setLabel(m_pContentLabel, var, col);
}

void CAAlertView::setContentBackGroundImage(std::string szImage)
{
	setCtrlImage(m_pContentBkImage, CAImage::create(szImage.c_str()));
}

void CAAlertView::setContentBackGroundImage(CAImage* image)
{
	setCtrlImage(m_pContentBkImage, image);
}


void CAAlertView::initAllButton(std::vector<CAButton*>& vbtns)
{
	if (!m_vAllBtn.empty())
	{
		return;
	}

	for (int i = 0; i < vbtns.size(); i++)
	{
		vbtns[i]->retain();
		m_vAllBtn.push_back(vbtns[i]);
		vbtns[i]->setAllowsSelected(true);
		vbtns[i]->addTarget(this, CAControl_selector(CAAlertView::onClickButton), CAControlEventTouchUpInSide);
	}
}

void CAAlertView::initAllButton(std::vector<std::string>& vBtnText)
{
	std::vector<CAButton*> vbtns;
	for (int i = 0; i < vBtnText.size(); i++)
	{
		CAButton* btn = CAButton::create(CAButtonTypeSquareRect);
		CCAssert(btn, "");
		btn->setTitleForState(CAControlStateAll, vBtnText[i].c_str());
		vbtns.push_back(btn);
	}
	initAllButton(vbtns);
}

void CAAlertView::setAllBtnBackGroundImage(CAControlState controlState, std::string szImage)
{
	setAllBtnBackGroundImage(controlState, CAImage::create(szImage.c_str()));
}

void CAAlertView::setAllBtnBackGroundImage(CAControlState controlState, CAImage* image)
{
	for (int i = 0; i < m_vAllBtn.size(); i++)
	{
		m_vAllBtn[i]->setBackGroundViewForState(controlState, CAScale9ImageView::createWithImage(image));
	}
}

void CAAlertView::setAllBtnTextColor(CAColor4B col)
{
	for (int i = 0; i < m_vAllBtn.size(); i++)
	{
		m_vAllBtn[i]->setTitleColorForState(CAControlStateAll, col);
	}
}


void CAAlertView::setCtrlImage(CAScale9ImageView*& pImageView, CAImage* image)
{
	if (pImageView)
	{
		removeSubview(pImageView);
	}
	CC_SAFE_DELETE(pImageView);

	pImageView = new CAScale9ImageView();
	if (pImageView == NULL)
	{
		return;
	}

	if (!pImageView->initWithImage(image))
	{
		CC_SAFE_DELETE(pImageView);
		return;
	}
	addSubview(pImageView);
}

void CAAlertView::setLabel(CCLabelTTF*& pLabel, std::string& szTitle, CAColor4B col)
{
	if (pLabel == NULL)
	{
		pLabel = new CCLabelTTF();
		if (pLabel == NULL)
		{
			return;
		}
		pLabel->initWithString("", m_sMsgFontName.c_str(), 20);
		pLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
		pLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		insertSubview(pLabel, 1);
	}
	pLabel->setString(szTitle);
	pLabel->setColor(col);
}

CCSize CAAlertView::getAlertWinSize()
{
	CCSize sz = CCDirector::sharedDirector()->getWinSize();
	return CCSizeMake(sz.width*ALERT_WINDOW_SIZEX, sz.height*ALERT_WINDOW_SIZEY);
}

void CAAlertView::calcuCtrlsSize()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize ertSize = getAlertWinSize();

	bool isHoriBtnArray = m_vAllBtn.size() <= MAX_BUTTON_COUNT_ROW;
	if (!isHoriBtnArray)
	{
		ertSize.height += ertSize.height / 5;
	}
	if (m_pTitleImage)
	{
		if (isHoriBtnArray)
		{
			m_pTitleImage->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2 - ertSize.height * 2 / 5, ertSize.width, ertSize.height / 5));
		}
		else
		{
			m_pTitleImage->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2 - ertSize.height * 5 / 12, ertSize.width, ertSize.height / 6));
		}
	}
	if (m_pTitleLabel)
	{
		if (isHoriBtnArray)
		{
			m_pTitleLabel->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2 - ertSize.height * 2 / 5, 0, 0));
		}
		else
		{
			m_pTitleLabel->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2 - ertSize.height * 5 / 12, 0, 0));
		}
	}

	
	if (m_pContentBkImage)
	{
		if (isHoriBtnArray)
		{
			m_pContentBkImage->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2, ertSize.width, ertSize.height * 3 / 5));
		}
		else
		{
			m_pContentBkImage->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2 - ertSize.height/6, ertSize.width, ertSize.height * 1 / 3));
		}
		
	}
	if (m_pContentLabel)
	{
		if (isHoriBtnArray)
		{
			m_pContentLabel->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2, 0, 0));
		}
		else
		{
			m_pContentLabel->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2 - ertSize.height / 6, 0, 0));
		}
	}

	if (!isHoriBtnArray)
	{
		m_pBtnTableView = new CATableView();
		CCAssert(m_pBtnTableView != NULL, "");

		m_pBtnTableView->initWithFrame(CCRectMake(0, 0, 0, 0));
		m_pBtnTableView->setCenter(CCRectMake(winSize.width/2, winSize.height/2+ertSize.height/4, ertSize.width, ertSize.height/2));
		m_pBtnTableView->setTableViewDataSource(this);
		m_pBtnTableView->setTableViewDelegate(this);
		addSubview(m_pBtnTableView);
	}

	for (int i = 0; i < m_vAllBtn.size(); i++)
	{
		CAButton* pButton = m_vAllBtn[i];
		CCAssert(pButton, "");

		if (isHoriBtnArray)
		{
			int iStartPosX = (winSize.width - ertSize.width) / 2;
			int iDtBtnWidth = ertSize.width / m_vAllBtn.size();
			pButton->setCenter(CCRectMake(iStartPosX + iDtBtnWidth*i + iDtBtnWidth / 2, winSize.height / 2 + ertSize.height * 2 / 5, iDtBtnWidth, ertSize.height / 5));
			addSubview(pButton);
		}
		else
		{
			pButton->setCenter(CCRectMake(0, 0, ertSize.width, ertSize.height / 6));
		}
	}
}

void CAAlertView::addTarget(CCObject* target, SEL_CAlertBtnEvent selector)
{
	CCAssert(target != NULL, "");
	m_pCAlertTarget = target;
	CCAssert(selector != NULL, "");
	m_pCAlertBtnEvent = selector;
}

void CAAlertView::onClickButton(CAControl* btn, CCPoint point)
{
	int iBtnIndex = -1;
	for (int i = 0; i < m_vAllBtn.size(); i++)
	{
		if (btn == m_vAllBtn[i])
		{
			iBtnIndex = i;
			break;
		}
	}
	if (m_pCAlertBtnEvent && m_pCAlertTarget)
	{
		((CCObject*)m_pCAlertTarget->*m_pCAlertBtnEvent)(iBtnIndex);
	}
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCAssert(pDirector != NULL, "");
	pDirector->getRootWindow()->removeSubview(this);
	autorelease();
}


void CAAlertView::showAlertMsg(std::string title, std::string alertMsg, std::vector<std::string>& vBtnText)
{
	setTitleImage("alert_title.png");
	setContentBackGroundImage("alert_content.png");
	setAlertMessage("Hello World");
	setTitle(title.c_str(), CAColor_white);
	setAlertMessage(alertMsg.c_str());
	
	initAllButton(vBtnText);
	setAllBtnBackGroundImage(CAControlStateNormal, "alert_btn.png");
	setAllBtnBackGroundImage(CAControlStateHighlighted, "alert_btn_sel.png");
	setAllBtnTextColor();

	calcuCtrlsSize();

	CCDirector* pDirector = CCDirector::sharedDirector();
	CCAssert(pDirector != NULL, "");
	pDirector->getRootWindow()->insertSubview(this, 1);
}

CATableViewCell* CAAlertView::tableCellAtIndex(CATableView* table, unsigned int section, unsigned int row)
{
	CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("aaa");
	if (cell == NULL)
	{
		cell = CATableViewCell::create("aaa");

		cell->addSubview(m_vAllBtn[row]);
		m_vAllBtn[row]->setCenterOrigin(m_vAllBtn[row]->getBounds().size / 2);
	}
	return cell;
}

unsigned int CAAlertView::numberOfRowsInSection(CATableView *table, unsigned int section)
{
	return m_vAllBtn.size();
}

unsigned int CAAlertView::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	CCSize ertSize = getAlertWinSize();
	return ertSize.height / 5;
}

NS_CC_END