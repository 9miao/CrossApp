#include "CATextEditHelper.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "control/CAButton.h"
#include "dispatcher/CATouchDispatcher.h"
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
	CAScheduler::schedule(schedule_selector(CATouchView::ccTouchTimer), this, 0, 0, 1.0f);

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
, m_pControlView(NULL)
{
}


CATextToolBarView::~CATextToolBarView()
{
    if (m_pControlView)
    {
        m_pControlView->becomeFirstResponder();
    }
    m_pControlView=NULL;
    m_CallbackTargets.clear();
}

void CATextToolBarView::addButton(const std::string& strBtnText, CAObject* target, SEL_CallFunc selector)
{
	m_CallbackTargets.push_back(CallbackTarget(target, selector, strBtnText));
}


void CATextToolBarView::show(CAView* pView)
{
	DSize winSize = CAApplication::getApplication()->getWinSize();

	float alertViewButtonHeight = 88;
	float alertViewWidth = winSize.width * 2 / 3;

	DRect rect = DRect(winSize.width / 2, winSize.height / 2 - alertViewButtonHeight, alertViewWidth, alertViewButtonHeight);

	m_pBackView = CAClippingView::create();
	m_pBackView->setCenter(rect);
	this->addSubview(m_pBackView);
	this->setTextTag("CATextToolBarView");
	m_pBackView->setAlphaThreshold(0.5f);

	CAScale9ImageView *BackgroundImageView = CAScale9ImageView::createWithFrame(m_pBackView->getBounds());
	BackgroundImageView->setImage(CAImage::create("source_material/alert_back.png"));
	m_pBackView->addSubview(BackgroundImageView);
	m_pBackView->setStencil(BackgroundImageView->copy());

	size_t btnCount = m_CallbackTargets.size();

	for (int i = 0; i < btnCount; i++) 
	{
		CAButton* btn = CAButton::create(CAButtonTypeSquareRect);
		btn->setTitleForState(CAControlStateAll, m_CallbackTargets[i].cszButtonText.c_str());
		btn->setTitleColorForState(CAControlStateAll, ccc4(3, 100, 255, 255));
		btn->setBackgroundViewForState(CAControlStateNormal, CAView::createWithColor(CAColor_clear));
		btn->setBackgroundViewForState(CAControlStateHighlighted, CAView::createWithColor(ccc4(226, 226, 226, 225)));
		btn->setTag(i);
		btn->addTarget(this, CAControl_selector(CATextToolBarView::alertViewCallback), CAControlEventTouchUpInSide);
		btn->setFrame(DRect(i*alertViewWidth / btnCount, 0, alertViewWidth / btnCount, alertViewButtonHeight));
		m_pBackView->addSubview(btn);

		if (i>0)
		{
            addGrayLine(alertViewWidth/btnCount * i);
		}
	}

	if (CAWindow *rootWindow = CAApplication::getApplication()->getRootWindow())
	{
		rootWindow->insertSubview(this, CAWindowZOderTop);
	}
	becomeFirstResponder();
    m_pControlView = pView;
}

bool CATextToolBarView::init()
{
	if (!CAView::init())
	{
		return false;
	}

	this->setColor(ccc4(135, 135, 135, 190));
	DRect rect = DRectZero;
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

bool CATextToolBarView::isTextToolBarShow()
{
	bool isShow = false;
	if (CAWindow *rootWindow = CAApplication::getApplication()->getRootWindow())
	{
		isShow = rootWindow->getSubviewByTextTag("CATextToolBarView")!=NULL;
	}
	return isShow;
}


void CATextToolBarView::hideTextToolBar()
{
	CATextToolBarView* pToolBarView = NULL;
	if (CAWindow *rootWindow = CAApplication::getApplication()->getRootWindow())
	{
		pToolBarView = (CATextToolBarView*)rootWindow->getSubviewByTextTag("CATextToolBarView");
	}
	if (pToolBarView)
	{
		pToolBarView->resignFirstResponder();
        pToolBarView->removeFromSuperview();
	}
}

bool CATextToolBarView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    return true;
}

void CATextToolBarView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    resignFirstResponder();
    removeFromSuperview();
}

void CATextToolBarView::addGrayLine(int x)
{
	DSize size = CAApplication::getApplication()->getWinSize();
	CAView *line = createWithFrame(DRect(x, 0, 1, size.height));
	line->setColor(ccc4(206, 206, 211, 255));
	m_pBackView->addSubview(line);
}

void CATextToolBarView::alertViewCallback(CAControl* btn, DPoint point)
{
	int btnIndex = btn->getTag();
	if (btnIndex>=0 && btnIndex<m_CallbackTargets.size())
	{
		((CAObject*)m_CallbackTargets[btnIndex].target->*m_CallbackTargets[btnIndex].selector)();
	}
    CATextToolBarView::hideTextToolBar();
}


std::vector<CATextResponder*> CATextResponder::s_AllTextResponder;

CATextResponder::CATextResponder()
{
	s_AllTextResponder.push_back(this);
}

CATextResponder::~CATextResponder()
{
	std::vector<CATextResponder*>::iterator it = std::find(s_AllTextResponder.begin(), s_AllTextResponder.end(), this);
	if (it != s_AllTextResponder.end())
	{
		s_AllTextResponder.erase(it);
	}
}

void CATextResponder::resignAllResponder(CATextResponder* pCurExcept)
{
	for (int i = 0; i < s_AllTextResponder.size(); i++)
	{
		if (s_AllTextResponder[i] == pCurExcept)
			continue;

		s_AllTextResponder[i]->resignResponder();
	}
	CAApplication::getApplication()->getTouchDispatcher()->setFirstResponder(NULL);
}


NS_CC_END