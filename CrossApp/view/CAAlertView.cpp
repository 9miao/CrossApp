
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
    CCRect rect = CCRectZero;
    rect.size = CAApplication::getApplication()->getWinSize();
    this->setFrame(rect);

	setBackGroundImage(CAImage::create("source_material/alert_content.png"));
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
	return pAlert;
}


bool CAAlertView::initWithText(const char* szTitle, const char* szAlertMsg, const char* pszBtnText, ...)
{
	if (!this->init())
	{
		return false;
	}

	va_list args;
	va_start(args, pszBtnText);

	addButton(pszBtnText);
	const char* pszText = va_arg(args, const char*);
	while (pszText)
	{
		addButton(pszText);
		pszText = va_arg(args, const char*);
	}
	va_end(args);

	setTitle(szTitle, CAColor_white);
	setAlertMessage(szAlertMsg);
	return true;
}


CAAlertView *CAAlertView::createWithText(const char* pszTitle, const char* pszAlertMsg, const char* pszBtnText, ...)
{
	va_list args;
	va_start(args, pszBtnText);

	CAAlertView *pAlert = new CAAlertView();
	if (pAlert && pAlert->init())
	{
		pAlert->addButton(pszBtnText);
		const char* pszText = va_arg(args, const char*);
		while (pszText)
		{
			pAlert->addButton(pszText);
			pszText = va_arg(args, const char*);
		}
		va_end(args);

		pAlert->setTitle(pszTitle, CAColor_white);
		pAlert->setAlertMessage(pszAlertMsg);

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
    CC_RETURN_IF(var.compare("") == 0);
	setLabel(m_pTitleLabel, var, col);
    if (m_pTitleImage == NULL)
    {
        setTitleImage(CAImage::create("source_material/alert_title.png"));
    }
}

void CAAlertView::setTitleImage(CAImage* image)
{
	setCtrlImage(m_pTitleImage, image);
}

void CAAlertView::setAlertMessage(std::string var, CAColor4B col)
{
	setLabel(m_pContentLabel, var, col);
}

void CAAlertView::setBackGroundImage(CAImage* image)
{
	setCtrlImage(m_pContentBkImage, image);
}

void CAAlertView::addButton(const std::string& btnText, CAColor4B col, CAImage* pNormalImage, CAImage* pHighlightedImage)
{
	CAButton* btn = CAButton::create(CAButtonTypeSquareRect);
	CCAssert(btn, "");
	btn->setTitleForState(CAControlStateAll, btnText.c_str());
	btn->setTitleColorForState(CAControlStateAll, col);
	if (pNormalImage == NULL)
	{
		pNormalImage = CAImage::create("source_material/alert_btn.png");
	}
	btn->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(pNormalImage));
	if (pHighlightedImage == NULL)
	{
		pHighlightedImage = CAImage::create("source_material/alert_btn_sel.png");
	}
	btn->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(pHighlightedImage));
	addButton(btn);
}

void CAAlertView::addButton(CAButton* pBtn)
{
	CCAssert(pBtn != NULL, "");
	m_vAllBtn.pushBack(pBtn);
	pBtn->setTag(m_vAllBtn.size() - 1);
	pBtn->addTarget(this, CAControl_selector(CAAlertView::onClickButton), CAControlEventTouchUpInSide);
}


void CAAlertView::setAllBtnBackGroundImage(CAControlState controlState, CAImage* image)
{
	for (size_t i = 0; i < m_vAllBtn.size(); i++)
	{
		m_vAllBtn.at(i)->setBackGroundViewForState(controlState, CAScale9ImageView::createWithImage(image));
	}
}

void CAAlertView::setAllBtnTextColor(CAColor4B col)
{
	for (size_t i = 0; i < m_vAllBtn.size(); i++)
	{
		m_vAllBtn.at(i)->setTitleColorForState(CAControlStateAll, col);
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
	return CADipSize(640 * ALERT_WINDOW_SIZEX, 960 * ALERT_WINDOW_SIZEY);
}

void CAAlertView::calcuCtrlsSize()
{
	CCSize winSize = this->getBounds().size;
	CCSize ertSize = getAlertWinSize();

	bool isHoriBtnArray = m_vAllBtn.size() <= MAX_BUTTON_COUNT_ROW;
	if (!isHoriBtnArray)
	{
		ertSize.height += ertSize.height / 5;
	}
	if (m_pTitleLabel && m_pTitleImage)
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
			m_pContentBkImage->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2 - ertSize.height / 6, ertSize.width, ertSize.height * 1 / 3));
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
		float size = ertSize.height / 10;
		m_pContentLabel->setFontSize(size);
	}

	if (!isHoriBtnArray)
	{
		m_pBtnTableView = new CATableView();
		CCAssert(m_pBtnTableView != NULL, "");

		m_pBtnTableView->initWithFrame(CCRectMake(0, 0, 0, 0));
		m_pBtnTableView->setCenter(CCRectMake(winSize.width / 2, winSize.height / 2 + ertSize.height / 4, ertSize.width, ertSize.height / 2));
		m_pBtnTableView->setTableViewDataSource(this);
		m_pBtnTableView->setTableViewDelegate(this);
		addSubview(m_pBtnTableView);
	}

	for (size_t i = 0; i < m_vAllBtn.size(); i++)
	{
		CAButton* pButton = m_vAllBtn.at(i);
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

void CAAlertView::show()
{
	if (getSuperview() != NULL)
		return;

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

		cell->addSubview(m_vAllBtn.at(row));
		m_vAllBtn.at(row)->setCenterOrigin(m_vAllBtn.at(row)->getBounds().size / 2);
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