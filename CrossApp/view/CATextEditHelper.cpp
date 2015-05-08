#include "CATextEditHelper.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "control/CAButton.h"
#include "CAAlertView.h"
#include "support/ccUTF8.h"
#include "platform/CCPlatformMacros.h"
#include "CAWindow.h"


NS_CC_BEGIN


CATouchView::CATouchView()
: m_pCurTouch(NULL)
, m_pCurEvent(NULL)
{
}

bool CATouchView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	CAScheduler::schedule(schedule_selector(CATouchView::ccTouchTimer), this, 0, 0, 1.5f);

	m_pCurTouch = pTouch;
	m_pCurEvent = pEvent;
	return true;
}

void CATouchView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	CAScheduler::unschedule(schedule_selector(CATouchView::ccTouchTimer), this);
}

void CATouchView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
	CAScheduler::unschedule(schedule_selector(CATouchView::ccTouchTimer), this);
}

void CATouchView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
	CAScheduler::unschedule(schedule_selector(CATouchView::ccTouchTimer), this);
}


void CATouchView::ccTouchTimer(float interval)
{
	CAScheduler::unschedule(schedule_selector(CATouchView::ccTouchTimer), this);
	ccTouchPress(m_pCurTouch, m_pCurEvent);
}



////////////////////////////////////////////////////////////////////////////////////////////
CATextToolBarView::CATextToolBarView()
: m_pBackView(NULL)
{
}


CATextToolBarView::~CATextToolBarView()
{
}

void CATextToolBarView::addButton(const std::string& strBtnText, CAObject* target, SEL_CATextTooBarBtnEvent selector)
{
	m_CallbackTargets.push_back(CallbackTarget(target, selector, strBtnText));
}

void CATextToolBarView::show()
{
	CADipSize winSize = this->getBounds().size;

	float alertViewButtonHeight = 88;
	float alertViewWidth = winSize.width * 2 / 3;

	CADipRect rect = CADipRect(winSize.width / 2, winSize.height / 2, alertViewWidth, alertViewButtonHeight*m_CallbackTargets.size());

	m_pBackView = CAClippingView::create();
	m_pBackView->setCenter(rect);
	this->addSubview(m_pBackView);
	m_pBackView->setAlphaThreshold(0.5f);

	CAScale9ImageView *backgroundImageView = CAScale9ImageView::createWithFrame(m_pBackView->getBounds());
	backgroundImageView->setImage(CAImage::create("source_material/alert_back.png"));
	m_pBackView->addSubview(backgroundImageView);
	m_pBackView->setStencil(backgroundImageView->copy());

	size_t btnCount = m_CallbackTargets.size();

	for (int i = 0; i < btnCount; i++) 
	{
		CAButton* btn = CAButton::create(CAButtonTypeSquareRect);
		btn->setTitleForState(CAControlStateAll, m_CallbackTargets[i].cszButtonText.c_str());
		btn->setTitleColorForState(CAControlStateAll, ccc4(3, 100, 255, 255));
		btn->setBackGroundViewForState(CAControlStateNormal, CAView::createWithColor(CAColor_clear));
		btn->setBackGroundViewForState(CAControlStateHighlighted, CAView::createWithColor(ccc4(226, 226, 226, 225)));
		btn->setTag(i);
		btn->addTarget(this, CAControl_selector(CATextToolBarView::alertViewCallback), CAControlEventTouchUpInSide);
		btn->setFrame(CADipRect(0, alertViewButtonHeight * i, alertViewWidth, alertViewButtonHeight));
		m_pBackView->addSubview(btn);

		if (i>0)
		{
			addGrayLine(alertViewButtonHeight * i);
		}
	}

	if (CAWindow *rootWindow = CAApplication::getApplication()->getRootWindow())
	{
		rootWindow->insertSubview(this, CAWindowZoderTop);
	}
	becomeFirstResponder();
}

bool CATextToolBarView::init()
{
	if (!CAView::init())
	{
		return false;
	}

	this->setColor(ccc4(135, 135, 135, 190));
	CCRect rect = CCRectZero;
	rect.size = CAApplication::getApplication()->getWinSize();
	this->setFrame(rect);

	return true;
}

CATextToolBarView *CATextToolBarView::create()
{
	CATextToolBarView *pAlert = new CATextToolBarView();
	if (pAlert && pAlert->init())
	{
		pAlert->autorelease();
		return pAlert;
	}
	CC_SAFE_DELETE(pAlert);
	return pAlert;
}


void CATextToolBarView::addGrayLine(int y) 
{
	CCSize size = CAApplication::getApplication()->getWinSize();
	CAView *line = createWithFrame(CADipRect(0, y, size.width, 1));
	line->setColor(ccc4(206, 206, 211, 255));
	m_pBackView->addSubview(line);
}

void CATextToolBarView::alertViewCallback(CAControl* btn, CCPoint point)
{
	int btnIndex = btn->getTag();
	CCAssert(btnIndex >= 0 && btnIndex < m_CallbackTargets.size());
	((CAObject*)m_CallbackTargets[btnIndex].target->*m_CallbackTargets[btnIndex].selector)();
	removeFromSuperview();
}

bool CATextToolBarView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	resignFirstResponder();
	removeFromSuperview();
	return false;
}

NS_CC_END