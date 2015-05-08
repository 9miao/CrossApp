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
{
}


CATextToolBarView::~CATextToolBarView()
{
}

void CATextToolBarView::addButton(const std::string& strBtnText, CAObject* target, SEL_CallFunc selector)
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
		setTextTag("CATextToolBarView");
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

bool CATextToolBarView::IsTextToolBarShow()
{
	bool isShow = false;
	if (CAWindow *rootWindow = CAApplication::getApplication()->getRootWindow())
	{
		isShow = rootWindow->getSubviewByTextTag("CATextToolBarView")!=NULL;
	}
	return isShow;
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
	((CAObject*)m_CallbackTargets[btnIndex].target->*m_CallbackTargets[btnIndex].selector)();
	removeFromSuperview();
}

bool CATextToolBarView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	resignFirstResponder();
	removeFromSuperview();
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
CATextSelectView::CATextSelectView()
: m_pCursorMarkL(NULL)
, m_pCursorMarkR(NULL)
, m_pTextViewMask(NULL)
, m_pControlView(NULL)
{

}

CATextSelectView::~CATextSelectView()
{

}

bool CATextSelectView::init()
{
	if (!CAView::init())
		return false;

	m_pTextViewMask = CAView::createWithColor(ccc4(60, 120, 240, 127));
	addSubview(m_pTextViewMask);

	m_pCursorMarkL = CAImageView::createWithImage(CAImage::create("source_material/text_pos_l.png"));
	addSubview(m_pCursorMarkL);
	m_pCursorMarkL->setVisible(false);

	m_pCursorMarkR = CAImageView::createWithImage(CAImage::create("source_material/text_pos_r.png"));
	addSubview(m_pCursorMarkR);
	m_pCursorMarkR->setVisible(false);

	return true;
}

CATextSelectView *CATextSelectView::create()
{
	CATextSelectView *pTextSelView = new CATextSelectView();
	if (pTextSelView && pTextSelView->init())
	{
		pTextSelView->autorelease();
		return pTextSelView;
	}
	CC_SAFE_DELETE(pTextSelView);
	return pTextSelView;
}

void CATextSelectView::hideTextSelectView()
{
	CATextSelectView* pSelTextView = NULL;
	if (CAWindow *rootWindow = CAApplication::getApplication()->getRootWindow())
	{
		pSelTextView = (CATextSelectView*)rootWindow->getSubviewByTextTag("CATextSelectView");
	}
	if (pSelTextView)
	{
		pSelTextView->hideTextSelView();
	}
}

void CATextSelectView::showTextSelView(const CCRect& rect, CAView* pControlView, bool showLeft, bool showRight)
{
	if (getSuperview() != NULL)
		return;

	CCSize winSize = CAApplication::getApplication()->getWinSize();
	setFrame(CCRect(0, 0, winSize.width, winSize.height));
	setColor(CAColor_clear);
	setTextTag("CATextSelectView");

	CADipRect newRect = rect;
	if (showLeft)
	{
		m_pCursorMarkL->setFrame(CADipRect(newRect.origin.x - 40, newRect.origin.y + newRect.size.height, 40, 64));
		m_pCursorMarkL->setVisible(true);
	}

	if (showRight)
	{
		m_pCursorMarkR->setFrame(CADipRect(newRect.origin.x + newRect.size.width, newRect.origin.y + newRect.size.height, 40, 64));
		m_pCursorMarkR->setVisible(true);
	}

	m_pTextViewMask->setFrame(newRect);
	m_pTextViewMask->setVisible(true);


	if (CAView *rootWindow = CAApplication::getApplication()->getRootWindow())
	{
		rootWindow->removeSubviewByTextTag("CATextSelectView");
		rootWindow->addSubview(this);
	}
	becomeFirstResponder();
	m_pControlView = pControlView;
}


void CATextSelectView::hideTextSelView()
{
	resignFirstResponder();

	if (m_pControlView)
	{
		m_pControlView->becomeFirstResponder();
		m_pControlView = NULL;
	}

	removeFromSuperview();

	CAApplication::getApplication()->updateDraw();
}

bool CATextSelectView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	CCPoint cTouchPoint = this->convertTouchToNodeSpace(pTouch);

	CCRect newRectL = m_pCursorMarkL->getFrame();
	newRectL.InflateRect(5);
	CCRect newRectR = m_pCursorMarkR->getFrame();
	newRectR.InflateRect(5);

	m_iSelViewTouchPos = 0;
	if (newRectL.containsPoint(cTouchPoint))
	{
		m_iSelViewTouchPos = 1;
		return true;
	}

	if (newRectR.containsPoint(cTouchPoint))
	{
		m_iSelViewTouchPos = 2;
		return true;
	}

	CCPoint point = this->convertTouchToNodeSpace(pTouch);

	CCRect ccTextRect = m_pTextViewMask->getFrame();
	if (ccTextRect.containsPoint(point))
	{
		CATextToolBarView *pToolBar = CATextToolBarView::create();
		pToolBar->addButton(UTF8("\u526a\u5207"), this, callfunc_selector(CATextSelectView::ccCutToClipboard));
		pToolBar->addButton(UTF8("\u590d\u5236"), this, callfunc_selector(CATextSelectView::ccCopyToClipboard));
		pToolBar->addButton(UTF8("\u7c98\u8d34"), this, callfunc_selector(CATextSelectView::ccPasteFromClipboard));
		pToolBar->show();
	}
	else
	{
		if (resignFirstResponder())
		{
			hideTextSelView();
		}
		else
		{
			becomeFirstResponder();
		}
	}
	return false;
}

void CATextSelectView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	if (m_iSelViewTouchPos != 1 && m_iSelViewTouchPos != 2)
		return;

	CAIMEDispatcher::sharedDispatcher()->dispatchMoveSelectChars(m_iSelViewTouchPos == 1, pTouch->getLocation());
}

void CATextSelectView::ccCopyToClipboard()
{
	hideTextSelView();
	CAIMEDispatcher::sharedDispatcher()->dispatchCopyToClipboard();
}

void CATextSelectView::ccCutToClipboard()
{
	hideTextSelView();
	CAIMEDispatcher::sharedDispatcher()->dispatchCutToClipboard();
}

void CATextSelectView::ccPasteFromClipboard()
{
	hideTextSelView();
	CAIMEDispatcher::sharedDispatcher()->dispatchPasteFromClipboard();
}

NS_CC_END