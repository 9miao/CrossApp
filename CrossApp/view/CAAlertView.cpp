
#include "CAAlertView.h"
#include "view/CAScale9ImageView.h"
#include "basics/CAApplication.h"
#include "control/CAButton.h"
#include "support/CCPointExtension.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionEase.h"
#include "dispatcher/CATouch.h"
#include "view/CAWindow.h"


NS_CC_BEGIN

CAAlertView::CAAlertView()
: m_pContentLabel(NULL)
, m_pTitleLabel(NULL)
, m_pTitleImage(NULL)
, m_pContentBkImage(NULL)
, m_pCAlertBtnEvent(NULL)
, m_pCAlertTarget(NULL)
, m_pBtnTableView(NULL)
, m_sMsgFontName("")
{

}


CAAlertView::~CAAlertView()
{
	for (int i = 0; i < m_vAllBtn.size(); i++)
	{
		CC_SAFE_RELEASE_NULL(m_vAllBtn[i]);
	}
	CC_SAFE_RELEASE_NULL(m_pTitleLabel);
	CC_SAFE_RELEASE_NULL(m_pContentLabel);
	CC_SAFE_RELEASE_NULL(m_pTitleImage);
	CC_SAFE_RELEASE_NULL(m_pContentBkImage);
	CC_SAFE_RELEASE_NULL(m_pBtnTableView);
}

bool CAAlertView::init()
{
	if (!CAView::init())
	{
		return false;
	}

    this->setColor(CAColor_clear);
	CCSize sz = CAApplication::getApplication()->getWinSize();
	setCenter(CCRectMake(sz.width / 2, sz.height / 2, sz.width, sz.height));
	return true;
}

CAAlertView *CAAlertView::create()
{
	CAAlertView *pAlert = new CAAlertView();
	if (pAlert && pAlert->init())
	{
		pAlert->autorelease();
		return pAlert;
	}
	CC_SAFE_DELETE(pAlert);
	return NULL;
}

void CAAlertView::setMessageFontName(std::string var)
{
    CC_RETURN_IF(m_pContentLabel == NULL);
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

void CAAlertView::setTitleImage(CAImage* image)
{
	setCtrlImage(m_pTitleImage, image);
}

void CAAlertView::setAlertMessage(std::string var, CAColor4B col)
{
	setLabel(m_pContentLabel, var, col);
}

void CAAlertView::setContentBackGroundImage(CAImage* image)
{
	setCtrlImage(m_pContentBkImage, image);
}

void CAAlertView::initAllButton(std::vector<CAButton*>& vbtns)
{
    CC_RETURN_IF(!m_vAllBtn.empty());
	for (int i = 0; i < vbtns.size(); i++)
	{
		vbtns[i]->retain();
		m_vAllBtn.push_back(vbtns[i]);
		vbtns[i]->setTag(i);
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

    CC_RETURN_IF(pImageView == NULL);
    
	if (!pImageView->initWithImage(image))
	{
		CC_SAFE_DELETE(pImageView);
		return;
	}
	addSubview(pImageView);
}

void CAAlertView::setLabel(CALabel*& pLabel, std::string& szTitle, CAColor4B col)
{
	if (pLabel == NULL)
	{
		pLabel = new CALabel();
        CC_RETURN_IF(pLabel == NULL);
		pLabel->init();
		pLabel->setTextAlignment(CATextAlignmentCenter);
		pLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        pLabel->setFontName(m_sMsgFontName.c_str());
        pLabel->setFontSize(_px(32));
		insertSubview(pLabel, 1);
	}
	pLabel->setText(szTitle);
	pLabel->setColor(col);
}

CCSize CAAlertView::getAlertWinSize()
{
	CCSize sz = CAApplication::getApplication()->getWinSize();
	return CCSizeMake(sz.width*ALERT_WINDOW_SIZEX, sz.height*ALERT_WINDOW_SIZEY);
}

void CAAlertView::calcuCtrlsSize()
{
	CCSize winSize = CAApplication::getApplication()->getWinSize();
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
        if (m_pTitleLabel)
        {
            m_pTitleLabel->setBounds(CCRectZero);
            
        }
	}
	if (m_pTitleLabel)
	{
		if (isHoriBtnArray)
		{
			m_pTitleLabel->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2 - ertSize.height * 2 / 5, ertSize.width, ertSize.height / 5));
		}
		else
		{
			m_pTitleLabel->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2 - ertSize.height * 5 / 12, ertSize.width, ertSize.height / 6));
		}
        float size = m_pTitleImage->getBounds().size.height / 2;
        m_pTitleLabel->setFontSize(size);
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
			m_pContentLabel->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2, ertSize.width, ertSize.height * 3 / 5));
		}
		else
		{
			m_pContentLabel->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2 - ertSize.height / 6, ertSize.width, ertSize.height * 1 / 3));
		}
        float size = m_pTitleImage->getBounds().size.height / 2;
        m_pContentLabel->setFontSize(size);
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

void CAAlertView::setTarget(CAObject* target, SEL_CAAlertBtnEvent selector)
{
	m_pCAlertTarget = target;
	m_pCAlertBtnEvent = selector;
}

void CAAlertView::onClickButton(CAControl* btn, CCPoint point)
{
    CC_RETURN_IF(m_bRunning == false);
	if (m_pCAlertBtnEvent && m_pCAlertTarget)
	{
		((CAObject*)m_pCAlertTarget->*m_pCAlertBtnEvent)(btn->getTag());
	}
    this->removeFromSuperview();
	autorelease();
}


void CAAlertView::showMessage(std::string title, std::string alertMsg, std::vector<std::string>& vBtnText)
{
	setTitleImage(CAImage::create("source_material/alert_title.png"));
	setContentBackGroundImage(CAImage::create("source_material/alert_content.png"));
	setTitle(title.c_str(), CAColor_white);
	setAlertMessage(alertMsg.c_str());
	
	initAllButton(vBtnText);
	setAllBtnBackGroundImage(CAControlStateNormal, CAImage::create("source_material/alert_btn.png"));
	setAllBtnBackGroundImage(CAControlStateHighlighted, CAImage::create("source_material/alert_btn_sel.png"));
	setAllBtnTextColor();

	calcuCtrlsSize();

	CAApplication* pApplication = CAApplication::getApplication();
	CCAssert(pApplication != NULL, "");
	pApplication->getRootWindow()->insertSubview(this, CAWindowZoderCenter);
	retain();
}

CATableViewCell* CAAlertView::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
	CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("cellID");
	if (cell == NULL)
	{
		cell = CATableViewCell::create("cellID");

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