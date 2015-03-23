#include "CATextEditHelper.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "view/CAWindow.h"
#include "actions/CCActionInterval.h"
#include "CCEGLView.h"
#include "support/ccUTF8.h"


NS_CC_BEGIN

CATextToolBar::CATextToolBar()
: m_pCATextTarget(NULL)
, m_pCAEditBtnEvent(NULL)
{
}


CATextToolBar::~CATextToolBar()
{
}

CATextToolBar *CATextToolBar::createWithText(const char* pszBtnText, ...)
{
	va_list args;
	va_start(args, pszBtnText);

	CATextToolBar *pTextEditHelper = new CATextToolBar();
	if (pTextEditHelper && pTextEditHelper->init())
	{
		pTextEditHelper->addButton(pszBtnText);
		const char* pszText = va_arg(args, const char*);
		while (pszText)
		{
			pTextEditHelper->addButton(pszText);
			pszText = va_arg(args, const char*);
		}
		va_end(args);

		pTextEditHelper->autorelease();
		return pTextEditHelper;
	}
	CC_SAFE_DELETE(pTextEditHelper);
	return NULL;
}

void CATextToolBar::addButton(const std::string& btnText, CAColor4B col, CAImage* pNormalImage, CAImage* pHighlightedImage)
{
	CAButton* btn = CAButton::create(CAButtonTypeSquareRect);
	CCAssert(btn, "");
	btn->setTitleForState(CAControlStateAll, btnText.c_str());
	btn->setTitleColorForState(CAControlStateAll, col);
	if (pNormalImage == NULL)
	{
		btn->setBackGroundViewForState(CAControlStateNormal, CAView::createWithColor(CAColor_gray));
	}
	else 
	{
		btn->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(pNormalImage));
	}

	if (pHighlightedImage == NULL)
	{
		btn->setBackGroundViewForState(CAControlStateHighlighted, CAView::createWithColor(CAColor_yellow));
	}
	else
	{
		btn->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(pHighlightedImage));
	}

	m_vAllBtn.pushBack(btn);
	btn->setTag(m_vAllBtn.size() - 1);
	btn->addTarget(this, CAControl_selector(CATextToolBar::onClickButton), CAControlEventTouchUpInSide);
}

void CATextToolBar::onClickButton(CAControl* btn, CCPoint point)
{
	CC_RETURN_IF(m_bRunning == false);
	if (m_pCAEditBtnEvent && m_pCATextTarget)
	{
		((CAObject*)m_pCATextTarget->*m_pCAEditBtnEvent)(btn->getTag());
	}
	hideTextEditView();
}


bool CATextToolBar::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	CCPoint point = this->convertTouchToNodeSpace(pTouch);
	if (!this->getBounds().containsPoint(point))
	{
		if (resignFirstResponder())
		{
			hideTextEditView();
		}
		return true;
	}
	return CAView::ccTouchBegan(pTouch, pEvent);
}

void CATextToolBar::setTarget(CAObject* target, SEL_CATextEditBtnEvent selector)
{
	m_pCATextTarget = target;
	m_pCAEditBtnEvent = selector;
}

void CATextToolBar::showTextEditView(const CCPoint& point)
{
	const float fBtnWidth = 80;
	const float fBtnHeight= 50;

	if (getSuperview() != NULL)
		return;

	setFrame(CADipRect(point.x, point.y, fBtnWidth*m_vAllBtn.size(), fBtnHeight));
	setColor(CAColor_red);

	for (int i = 0; i < m_vAllBtn.size(); i++) 
	{
		m_vAllBtn.at(i)->setFrame(CADipRect(fBtnWidth * i, 0, fBtnWidth, fBtnHeight));
		addSubview(m_vAllBtn.at(i));
	}
	

	if (CAView *rootWindow = CAApplication::getApplication()->getRootWindow()) 
	{
		rootWindow->addSubview(this);
	}
	becomeFirstResponder();
}

void CATextToolBar::hideTextEditView()
{
	//
	resignFirstResponder();
	removeFromSuperview();

	CAApplication::getApplication()->updateDraw();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CATextSelectView::CATextSelectView()
: m_pDotViewL(NULL)
, m_pDotViewR(NULL)
, m_pCursorMarkL(NULL)
, m_pCursorMarkR(NULL)
, m_pTextMask(NULL)
, m_iSelViewTouchPos(0)
{

}

CATextSelectView::~CATextSelectView()
{

}

bool CATextSelectView::init()
{
	if (!CAView::init())
		return false;

	m_pDotViewL = CAImageView::createWithImage(CAImage::create("source_material/dian.png"));
	addSubview(m_pDotViewL);
	m_pDotViewL->setVisible(false);

	m_pDotViewR = CAImageView::createWithImage(CAImage::create("source_material/dian.png"));
	addSubview(m_pDotViewR);
	m_pDotViewR->setVisible(false);

	m_pCursorMarkL = CAView::create();
	m_pCursorMarkL->setColor(ccc4(19,112,215,255));
	addSubview(m_pCursorMarkL);
	m_pCursorMarkL->setVisible(false);

	m_pCursorMarkR = CAView::create();
	m_pCursorMarkR->setColor(ccc4(19, 112, 215, 255));
	addSubview(m_pCursorMarkR);
	m_pCursorMarkR->setVisible(false);

	m_pTextMask = CAImageView::createWithImage(CAImage::create("source_material/zhezhao.png"));
	addSubview(m_pTextMask);
	m_pTextMask->setVisible(false);

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


void CATextSelectView::showTextSelView(const CCRect& rect, bool showLeft, bool showRight)
{
	CCSize winSize = CAApplication::getApplication()->getWinSize();
	setFrame(CCRect(0, 0, winSize.width, winSize.height));
	setColor(CAColor_clear);

	CCRect newRect = rect;
	if (showLeft)
	{
		m_pDotViewL->setFrame(CCRect(newRect.origin.x - ZZSELECT_VIEW_SIZE / 2, newRect.origin.y - ZZSELECT_VIEW_SIZE, ZZSELECT_VIEW_SIZE, ZZSELECT_VIEW_SIZE));
		m_pDotViewL->setVisible(true);
		m_pCursorMarkL->setFrame(CCRect(newRect.origin.x, newRect.origin.y, 2, newRect.size.height));
		m_pCursorMarkL->setVisible(true);
	}
	
	if (showRight)
	{
		m_pDotViewR->setFrame(CCRect(newRect.origin.x + newRect.size.width - ZZSELECT_VIEW_SIZE / 2, newRect.origin.y + newRect.size.height, ZZSELECT_VIEW_SIZE, ZZSELECT_VIEW_SIZE));
		m_pDotViewR->setVisible(true);
		m_pCursorMarkR->setFrame(CCRect(newRect.origin.x + newRect.size.width - 1, newRect.origin.y, 2, newRect.size.height));
		m_pCursorMarkR->setVisible(true);
	}

	m_pTextMask->setFrame(newRect);
	m_pTextMask->setVisible(true);
	

	if (CAView *rootWindow = CAApplication::getApplication()->getRootWindow())
	{
		rootWindow->addSubview(this);
	}
	becomeFirstResponder();
}


bool CATextSelectView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	m_TouchPoint = this->convertTouchToNodeSpace(pTouch);
	
	CCRect newRectL = m_pDotViewL->getFrame();
	newRectL.InflateRect(5);
	CCRect newRectR = m_pDotViewR->getFrame();
	newRectR.InflateRect(5);

	m_iSelViewTouchPos = 0;
	if (newRectL.containsPoint(m_TouchPoint))
	{
		m_iSelViewTouchPos = 1;
		return true;
	}
	
	if (newRectR.containsPoint(m_TouchPoint))
	{
		m_iSelViewTouchPos = 2;
		return true;
	}

	if (resignFirstResponder())
	{
		hideTextSelView();
	}
	else
	{
		becomeFirstResponder();
	}
	return false;
}

void CATextSelectView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	if (m_iSelViewTouchPos != 1 && m_iSelViewTouchPos != 2)
		return;
	
	CAIMEDispatcher::sharedDispatcher()->dispatchMoveSelectChars(m_iSelViewTouchPos == 1, pTouch->getLocation());

	CAView::ccTouchMoved(pTouch, pEvent);
}

void CATextSelectView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
	CAIMEDispatcher::sharedDispatcher()->dispatchMoveSelectCharsCancel(pTouch->getLocation());

	CAView::ccTouchEnded(pTouch, pEvent);
}

void CATextSelectView::hideTextSelView()
{
	resignFirstResponder();
	removeFromSuperview();

	CAApplication::getApplication()->updateDraw();
}

NS_CC_END