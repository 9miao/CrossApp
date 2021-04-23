#include "CATextEditHelper.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "control/CAButton.h"
#include "dispatcher/CATouchDispatcher.h"
#include "CAAlertView.h"
#include "support/ccUTF8.h"
#include "platform/CCPlatformMacros.h"
#include "CAWindow.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
#include "view/CAClippingView.h"
#include "view/CAScale9ImageView.h"

NS_CC_BEGIN

CATextToolBarView::CATextToolBarView()
: m_pBackView(nullptr)
, m_pControlView(nullptr)
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

void CATextToolBarView::addButton(const std::string& strBtnText, const std::function<void()>& callback)
{
	m_CallbackTargets.push_back(CallbackTarget(callback, strBtnText));
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

    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CAAlertView");
	CAScale9ImageView *BackgroundImageView = CAScale9ImageView::createWithFrame(m_pBackView->getBounds());
	BackgroundImageView->setImage(CAImage::create(map.at("backgroundView")));
	m_pBackView->addSubview(BackgroundImageView);
	m_pBackView->setStencil(BackgroundImageView->copy());

	size_t btnCount = m_CallbackTargets.size();

	for (int i = 0; i < btnCount; i++) 
	{
        CAButton* btn = CAButton::create(CAButton::Type::Custom);
		btn->setTitleForState(CAControl::State::Normal, m_CallbackTargets[i].cszButtonText.c_str());
        btn->setTitleColorForState(CAControl::State::Normal, CAColor4B(3, 100, 255, 255));
        btn->setBackgroundImageForState(CAControl::State::Normal, CAImage::CLEAR_IMAGE(), false);
        btn->setBackgroundImageForState(CAControl::State::Highlighted, CAImage::createWithColor4B(CAColor4B(226, 226, 226, 225)), false);
		btn->setTag(i);
        btn->addTarget([=]()
        {
        
            int btnIndex = btn->getTag();
            if (btnIndex>=0 && btnIndex<m_CallbackTargets.size())
            {
                m_CallbackTargets[btnIndex].function();
            }
            CATextToolBarView::hideTextToolBar();
            
        }, CAButton::Event::TouchUpInSide);
		btn->setFrame(DRect(i*alertViewWidth / btnCount, 0, alertViewWidth / btnCount, alertViewButtonHeight));
		m_pBackView->addSubview(btn);

		if (i>0)
		{
            addGrayLine(alertViewWidth/btnCount * i);
		}
	}

	if (CAWindow *rootWindow = CAApplication::getApplication()->getRootWindow())
	{
		rootWindow->insertSubview(this, CAWindowZOrderTop);
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

	this->setColor(CAColor4B(135, 135, 135, 190));
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
	line->setColor(CAColor4B(206, 206, 211, 255));
	m_pBackView->addSubview(line);
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
	CAApplication::getApplication()->getTouchDispatcher()->setFirstResponder(nullptr);
}


NS_CC_END
