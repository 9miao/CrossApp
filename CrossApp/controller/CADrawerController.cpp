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
#include "support/CANotificationCenter.h"
NS_CC_BEGIN

CADrawerController::CADrawerController()
:m_pLeftViewController(NULL)
,m_pRightViewController(NULL)
,m_fDivision(0)
,m_fCurrDivision(0)
,m_bShow(false)
,m_bEffect3D(false)
,m_fOffX(0)
,m_bAnimation(false)
,m_pBackgroundView(NULL)
{
    this->getView()->setColor(CAColor_clear);
    this->setTouchMoved(true);
    this->setVerticalScrollEnabled(false);
    
    memset(m_pContainer, NULL, sizeof(CAView*) * 2);
    
    CANotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CADrawerController::changeStatusBarOrientation), CAApplicationDidChangeStatusBarOrientationNotification, NULL);
}

CADrawerController::~CADrawerController()
{
    CC_SAFE_RELEASE(m_pLeftViewController);
    CC_SAFE_RELEASE(m_pRightViewController);
    
    CANotificationCenter::sharedNotificationCenter()->removeObserver(this, CAApplicationDidChangeStatusBarOrientationNotification);
}

bool CADrawerController::initWithController(CAViewController* leftViewController, CAViewController* rightViewController)
{
    if (!CAViewController::init())
    {
        return false;
    }
    this->setLeftViewController(leftViewController);
    this->setRightViewController(rightViewController);
    
    return true;
}

bool CADrawerController::initWithController(CAViewController* leftViewController, CAViewController* rightViewController, float division)
{
    return this->initWithController(leftViewController, rightViewController);
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
    for (int i=0; i<2; i++)
    {
        m_pContainer[i] = new CAView();

        this->getView()->addSubview(m_pContainer[i]);
        m_pContainer[i]->release();
    }
    
    this->achieveLayout();
    
    m_pContainer[0]->setLayout(m_rHideLayout[0]);
    m_pContainer[0]->setAnchorPoint(DPoint(0.5f, 0.5f));
    m_pContainer[1]->setLayout(m_rHideLayout[1]);
    m_pContainer[1]->setAnchorPoint(DPoint(0.0f, 0.5f));
    
    m_pLeftViewController->addViewFromSuperview(m_pContainer[0]);
    m_pRightViewController->addViewFromSuperview(m_pContainer[1]);
    
    m_bShow = true;
    this->hideLeftViewController(false);
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

void CADrawerController::achieveLayout()
{
    const CAInterfaceOrientation& orientation = CAApplication::getApplication()->getStatusBarOrientation();
    if (orientation == CAInterfaceOrientationPortrait || orientation == CAInterfaceOrientationPortraitUpsideDown)
    {
        m_fDivision = this->getView()->getBounds().size.width - 120;
        
        m_rHideLayout[0].horizontal.left = -m_fDivision;
        m_rHideLayout[0].horizontal.width = m_fDivision;
        m_rHideLayout[0].vertical = DVerticalLayoutFill;
        
        m_rShowLayout[0].horizontal.left = 0;
        m_rShowLayout[0].horizontal.width = m_fDivision;
        m_rShowLayout[0].vertical = DVerticalLayoutFill;
        
        
        m_rHideLayout[1].horizontal = DHorizontalLayoutFill;
        m_rHideLayout[1].vertical = DVerticalLayoutFill;
        
        m_rShowLayout[1].horizontal = DHorizontalLayout_L_R(m_fDivision, -m_fDivision);
        m_rShowLayout[1].vertical = DVerticalLayoutFill;
    }
    else if (orientation == CAInterfaceOrientationLandscapeLeft || CAInterfaceOrientationLandscapeRight)
    {
        m_fDivision = this->getView()->getBounds().size.width - 120;
        
        m_rHideLayout[0].horizontal.left = -m_fDivision;
        m_rHideLayout[0].horizontal.width = m_fDivision;
        m_rHideLayout[0].vertical = DVerticalLayoutFill;
        
        m_rShowLayout[0].horizontal.left = 0;
        m_rShowLayout[0].horizontal.width = m_fDivision;
        m_rShowLayout[0].vertical = DVerticalLayoutFill;
        
        
        m_rHideLayout[1].horizontal = DHorizontalLayoutFill;
        m_rHideLayout[1].vertical = DVerticalLayoutFill;
        
        m_rShowLayout[1].horizontal = DHorizontalLayout_L_R(m_fDivision, -m_fDivision);
        m_rShowLayout[1].vertical = DVerticalLayoutFill;
    }
}

void CADrawerController::changeStatusBarOrientation(CAObject* obj)
{
    this->achieveLayout();
    
    if (m_bShow)
    {
        m_fCurrDivision = m_fDivision;
        this->updateViewFrame();
        this->showEnded();
    }
    else
    {
        m_fCurrDivision = 0;
        this->updateViewFrame();
        this->hideEnded();
    }
}

void CADrawerController::showLeftViewController(bool animated)
{
    CC_RETURN_IF(m_bAnimation);
    m_bShow = true;

    this->begin();
    
    if (animated)
    {
        m_fCurrDivision = m_pContainer[1]->getFrameOrigin().x;
        CAScheduler::schedule(schedule_selector(CADrawerController::scheduleShowAction), this, 1/60.0f);
        m_bAnimation = true;
    }
    else
    {
        m_fCurrDivision = m_fDivision;
        this->updateViewFrame();
        this->showEnded();
    }
}

void CADrawerController::hideLeftViewController(bool animated)
{
    CC_RETURN_IF(m_bAnimation);
    m_bShow = false;
    
    this->begin();
    
    if (animated)
    {
        m_fCurrDivision = m_pContainer[1]->getFrameOrigin().x;
        m_pContainer[0]->setTouchEnabled(false);
        m_pContainer[1]->setTouchEnabled(false);
        CAScheduler::schedule(schedule_selector(CADrawerController::scheduleHideAction), this, 1/60.0f);
        m_bAnimation = true;
    }
    else
    {
        m_fCurrDivision = 0;
        this->updateViewFrame();
        this->hideEnded();
    }
}

bool CADrawerController::isShowLeftViewController()
{
    return m_pContainer[0]->isVisible();
}

void CADrawerController::begin()
{
    m_pContainer[0]->setVisible(true);
    m_pContainer[0]->setTouchEnabled(false);
    m_pContainer[1]->setTouchEnabled(false);
}

void CADrawerController::showEnded()
{
    m_pContainer[0]->setTouchEnabled(true);
    m_pRightViewController->viewDidDisappear();
    m_pLeftViewController->viewDidAppear();
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
    DPoint point[2] =
    {
        DPoint(m_fCurrDivision - m_fDivision, 0),
        DPoint(m_fCurrDivision, 0)
    };
    
    if (m_bEffect3D)
    {
        float scale0 = 0.5f + 0.5f * m_fCurrDivision / m_fDivision;
        float scale1 = 1.0f - powf(m_fCurrDivision / m_fDivision, 2) * 0.2f;
        
        m_pContainer[0]->setAlpha(m_fCurrDivision / m_fDivision);
        m_pContainer[0]->setScale(scale0);
        m_pContainer[1]->setScale(scale1);
        point[0].x = (m_fCurrDivision - m_fDivision) / 3;
    }
    else
    {
        m_pContainer[0]->setScale(1);
        m_pContainer[1]->setScale(1);
    }
    
    DLayout layout[2];
    layout[0] = m_rHideLayout[0];
    layout[0].horizontal.left = point[0].x;
    
    layout[1] = m_rHideLayout[1];
    layout[1].horizontal.left = point[1].x;
    layout[1].horizontal.right = -point[1].x;
    
    for (int i=0; i<2; i++)
    {
        m_pContainer[i]->setLayout(layout[i]);
    }
}

void CADrawerController::scheduleShowAction(float dt)
{
    if (m_fDivision - m_fCurrDivision < FLT_EPSILON)
    {
        CAScheduler::unschedule(schedule_selector(CADrawerController::scheduleShowAction), this);
        this->showEnded();
        m_bAnimation = false;
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
        this->hideEnded();
        m_bAnimation = false;
    }
    else
    {
        m_fCurrDivision = MAX(m_fCurrDivision - m_fDivision/12.0f * 60 * dt, 0);
        this->updateViewFrame();
    }
}

bool CADrawerController::isReachBoundaryLeft()
{
    return this->isShowLeftViewController();
}

bool CADrawerController::isReachBoundaryRight()
{
    return !this->isShowLeftViewController();
}

bool CADrawerController::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    return m_vTouches.size() == 1;
}

void CADrawerController::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_bTouchMoved == false);
    float offDis = pTouch->getLocation().x - pTouch->getPreviousLocation().x;
    m_fCurrDivision += offDis;
    m_fCurrDivision = MIN(m_fCurrDivision, m_fDivision);
    m_fCurrDivision = MAX(m_fCurrDivision, 0);
    
    this->begin();
    this->updateViewFrame();

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
        m_pBackgroundView->setLayout(DLayoutFill);
        this->getView()->insertSubview(m_pBackgroundView, -1);
    }
}

CAView* CADrawerController::getBackgroundView()
{
    return m_pBackgroundView;
}

void CADrawerController::setBackgroundImage(CAImage* var)
{
    if (m_pBackgroundView == NULL)
    {
        m_pBackgroundView = CAImageView::createWithLayout(DLayoutFill);
        this->getView()->insertSubview(m_pBackgroundView, -1);
    }
    if (CAImageView* imageView = dynamic_cast<CAImageView*>(m_pBackgroundView))
    {
        imageView->setImage(var);
    }
}

void CADrawerController::setTouchMoved(bool var)
{
    m_bTouchMoved = var;
    this->setPriorityScroll(var);
}

bool CADrawerController::isTouchMoved()
{
    return m_bTouchMoved;
}

NS_CC_END