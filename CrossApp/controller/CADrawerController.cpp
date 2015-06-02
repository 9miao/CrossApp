//
//  CADrawerController.cpp
//  CartoonHouse
//
//  Created by 栗元峰 on 14-8-20.
//
//

#include "CADrawerController.h"
#include "animation/CAAnimation.h"
#include "basics/CAApplication.h"
#include "dispatcher/CATouchDispatcher.h"

NS_CC_BEGIN

CADrawerController::CADrawerController()
:m_pLeftViewController(NULL)
,m_pRightViewController(NULL)
,m_fDivision(0)
,m_fCurrDivision(0)
,m_bShow(false)
,m_bEffect3D(false)
,m_fOffX(0)
,m_pBackgroundView(NULL)
{
    this->getView()->setColor(CAColor_clear);
    this->setTouchMovedListenVertical(false);
    this->setTouchMoved(true);
}

CADrawerController::~CADrawerController()
{
    CC_SAFE_RELEASE(m_pLeftViewController);
    CC_SAFE_RELEASE(m_pRightViewController);
}

bool CADrawerController::initWithController(CAViewController* leftViewController, CAViewController* rightViewController, float division)
{
    if (!CAViewController::init())
    {
        return false;
    }
    m_fDivision = division;
    this->setLeftViewController(leftViewController);
    this->setRightViewController(rightViewController);
    
    return true;
}

void CADrawerController::replaceRightViewController(CAViewController* rightViewController)
{

}

void CADrawerController::setLeftViewController(CrossApp::CAViewController *leftViewController)
{
    CC_SAFE_RETAIN(leftViewController);
    CC_SAFE_RELEASE(m_pLeftViewController);
    m_pLeftViewController = leftViewController;
    
    if (this->isViewRunning())
    {
        
    }
}

CAViewController* CADrawerController::getLeftViewController()
{
    return m_pLeftViewController;
}

void CADrawerController::setRightViewController(CrossApp::CAViewController *rightViewController)
{
    CC_SAFE_RETAIN(rightViewController);
    CC_SAFE_RELEASE(m_pRightViewController);
    m_pRightViewController = rightViewController;
    
    if (this->isViewRunning())
    {
        
    }
}

CAViewController* CADrawerController::getRightViewController()
{
    return m_pRightViewController;
}

void CADrawerController::viewDidLoad()
{    
    m_rHideFrame[0] = CCRect(-m_fDivision, 0, m_fDivision, this->getView()->getBounds().size.height);
    m_rHideFrame[1] = CCRect(0 , 0, this->getView()->getBounds().size.width, this->getView()->getBounds().size.height);
    m_rShowFrame[0] = CCRect(0, 0, m_fDivision, this->getView()->getBounds().size.height);
    m_rShowFrame[1] = CCRect(m_fDivision , 0, this->getView()->getBounds().size.width, this->getView()->getBounds().size.height);
    
    
    for (int i=0; i<2; i++)
    {
        m_pContainer[i] = new CAView();
        m_pContainer[i]->setFrame(m_rHideFrame[i]);
        this->getView()->addSubview(m_pContainer[i]);
        m_pContainer[i]->release();
    }
    
    m_pContainer[0]->setAnchorPoint(CCPoint(1.0f, 0.5f));
    m_pContainer[1]->setAnchorPoint(CCPoint(0.0f, 0.5f));
    
    m_pLeftViewController->addViewFromSuperview(m_pContainer[0]);
    m_pRightViewController->addViewFromSuperview(m_pContainer[1]);
    
    m_bShow = true;
    this->hideLeftViewController(false);
    
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getView()->getBounds());
    }
}

void CADrawerController::viewDidUnload()
{

}

void CADrawerController::viewDidAppear()
{
    if (m_bShow)
    {
        m_pLeftViewController->viewDidAppear();
    }
    else
    {
        m_pRightViewController->viewDidAppear();
    }
}

void CADrawerController::viewDidDisappear()
{
    if (m_bShow)
    {
        m_pLeftViewController->viewDidDisappear();
    }
    else
    {
        m_pRightViewController->viewDidDisappear();
    }
}

void CADrawerController::reshapeViewRectDidFinish()
{
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getView()->getBounds());
    }
    
    CCRect* rect;
    if (m_bShow)
    {
        rect = m_rShowFrame;
    }
    else
    {
        rect = m_rHideFrame;
    }
    
    for (int i=0; i<2; i++)
    {
        m_pContainer[i]->setFrame(rect[i]);
    }
    
    m_pLeftViewController->getSuperViewRect(m_pContainer[0]->getBounds());
    m_pRightViewController->getSuperViewRect(m_pContainer[1]->getBounds());
}

void CADrawerController::showLeftViewController(bool animated)
{
    m_bShow = true;

    if (animated)
    {
        this->showBegin();
        m_fCurrDivision = m_pContainer[1]->getFrameOrigin().x;
        CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
        CAScheduler::unscheduleAllForTarget(this);
        CAScheduler::schedule(schedule_selector(CADrawerController::scheduleShowAction), this, 1/60.0f);
    }
    else
    {
        m_fCurrDivision = m_fDivision;
        this->updateViewFrame();
        this->hideEnded();
    }
    m_bSlidingMaxX = false;
    m_bSlidingMinX = true;
}

void CADrawerController::hideLeftViewController(bool animated)
{
    m_bShow = false;
    
    if (animated)
    {
        m_fCurrDivision = m_pContainer[1]->getFrameOrigin().x;
        CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
        CAScheduler::unscheduleAllForTarget(this);
        CAScheduler::schedule(schedule_selector(CADrawerController::scheduleHideAction), this, 1/60.0f);
    }
    else
    {
        m_fCurrDivision = 0;
        this->updateViewFrame();
        this->hideEnded();
    }
    m_bSlidingMaxX = true;
    m_bSlidingMinX = false;
}

bool CADrawerController::isShowLeftViewController()
{
    return m_pContainer[0]->isVisible();
}

void CADrawerController::showBegin()
{
    m_pContainer[0]->setVisible(true);
    m_pContainer[1]->setTouchEnabled(false);
    m_pLeftViewController->viewDidAppear();
    m_pRightViewController->viewDidDisappear();
}

void CADrawerController::hideEnded()
{
    m_pContainer[0]->setVisible(false);
    m_pContainer[1]->setTouchEnabled(true);
    m_pLeftViewController->viewDidDisappear();
    m_pRightViewController->viewDidAppear();
}

void CADrawerController::updateViewFrame()
{
    CCPoint point[2] =
    {
        CCPoint(m_fCurrDivision - m_fDivision, 0),
        CCPoint(m_fCurrDivision, 0)
    };
    
    if (m_bEffect3D)
    {
        float scale0 = 0.5f + 0.5f * m_fCurrDivision / m_fDivision;
        float scale1 = 1.0f - powf(m_fCurrDivision / m_fDivision, 2) * 0.2f;
        
        m_pContainer[0]->setAlpha(m_fCurrDivision / m_fDivision);
        m_pContainer[0]->setScale(scale0);
        m_pContainer[1]->setScale(scale1);
        point[0].x = (point[1].x - m_pContainer[0]->getFrame().size.width) / 3;
        point[0].y = this->getView()->getBounds().size.height * (1.0f - scale0) / 2;
        point[1].y = this->getView()->getBounds().size.height * (1.0f - scale1) / 2;
    }
    
    m_pContainer[0]->setFrameOrigin(point[0]);
    m_pContainer[1]->setFrameOrigin(point[1]);

}

void CADrawerController::scheduleShowAction(float dt)
{
    if (m_fDivision - m_fCurrDivision < FLT_EPSILON)
    {
        CAScheduler::unschedule(schedule_selector(CADrawerController::scheduleShowAction), this);
        CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
    }
    else
    {
        m_fCurrDivision = MIN(m_fCurrDivision + m_fDivision/12.0f * 60 * dt, m_fDivision) ;
        this->updateViewFrame();
    }
}

void CADrawerController::scheduleHideAction(float dt)
{
    if (m_fCurrDivision < FLT_EPSILON)
    {
        CAScheduler::unschedule(schedule_selector(CADrawerController::scheduleHideAction), this);
        CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
        this->hideEnded();
    }
    else
    {
        m_fCurrDivision = MAX(m_fCurrDivision - m_fDivision/12.0f * 60 * dt, 0);
        this->updateViewFrame();
    }
}

bool CADrawerController::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    return true;
}

void CADrawerController::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_bTouchMoved == false);
    float offDis = pTouch->getLocation().x - pTouch->getPreviousLocation().x;
    m_fCurrDivision += offDis;
    m_fCurrDivision = MIN(m_fCurrDivision, m_fDivision);
    m_fCurrDivision = MAX(m_fCurrDivision, 0);
    
    this->updateViewFrame();
    
    if (m_bShow == false)
    {
        m_pContainer[0]->setVisible(true);
        m_pContainer[1]->setTouchEnabled(false);
    }

    m_fOffX = offDis;
}

void CADrawerController::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    if (m_bShow
        && !(m_fOffX > FLT_EPSILON)
        && m_pContainer[1]->getFrame().containsPoint(this->getView()->convertTouchToNodeSpace(pTouch)))
    {
        this->hideLeftViewController(true);
    }
    else if (std::abs(m_fOffX) > FLT_EPSILON)
    {
        if (m_fOffX > 0)
        {
            this->showLeftViewController(true);
        }
        else
        {
            this->hideLeftViewController(true);
        }
    }
    else
    {
        if (m_fCurrDivision > m_fDivision / 2)
        {
            this->showLeftViewController(true);
        }
        else
        {
            this->hideLeftViewController(true);
        }
    }
    
    m_fOffX = 0;
}

void CADrawerController::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}

void CADrawerController::setBackgroundView(CrossApp::CAView *var)
{
    this->getView()->removeSubview(m_pBackgroundView);
    m_pBackgroundView = var;
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getView()->getBounds());
        this->getView()->insertSubview(m_pBackgroundView, -1);
    }
}

CAView* CADrawerController::getBackgroundView()
{
    return m_pBackgroundView;
}

void CADrawerController::setTouchMoved(bool var)
{
    m_bTouchMovedStopSubviews = m_bTouchMoved = var;
}

bool CADrawerController::isTouchMoved()
{
    return m_bTouchMoved;
}

NS_CC_END