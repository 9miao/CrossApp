
#include "CAWindow.h"
#include "basics/CAPointExtension.h"
#include "basics/CAApplication.h"
#include "animation/CAViewAnimation.h"
#include "dispatcher/CATouchDispatcher.h"
#include "basics/CACamera.h"
#include "script_support/CAScriptSupport.h"

NS_CC_BEGIN

CAWindow::CAWindow()
:m_pRootViewController(nullptr)
,m_pModalViewController(nullptr)
,m_bCameraOrderDirty(true)
{
    this->setDisplayRange(false);
    
    CACamera::_visitingCamera = nullptr;
    
    m_pDefaultCamera = CACamera::create();
    this->addSubview(m_pDefaultCamera);

}

CAWindow::~CAWindow()
{
    CC_SAFE_RELEASE_NULL(m_pModalViewController);
    CC_SAFE_RELEASE_NULL(m_pRootViewController);

#if CC_ENABLE_SCRIPT_BINDING
	if (CAScriptEngineManager::getScriptEngineManager())
	{
		CAScriptEngineManager::getScriptEngineManager()->getScriptEngine()->releaseAllSubviewsRecursive(this);
	}
#endif
}

bool CAWindow::init()
{
    CAView::init();
    bool bRet = false;
    if (CAApplication* application = CAApplication::getApplication())
    {
        this->setFrame(DRect(DPointZero, application->getWinSize()));
        bRet = true;
    }
    return bRet;
}

CAWindow *CAWindow::create()
{
    CAWindow *pRet = new CAWindow();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void CAWindow::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
}

void CAWindow::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

void CAWindow::setRootViewController(CrossApp::CAViewController *var)
{
    if (m_pRootViewController)
    {
        m_pRootViewController->removeViewFromSuperview();
        CC_SAFE_RELEASE_NULL(m_pRootViewController);
    }
    
    if (var)
    {
        var->retain();
        m_pRootViewController = var;
        m_pRootViewController->addViewFromSuperview(this);
        m_pRootViewController->getView()->setZOrder(CAWindowZOrderBottom);
    }
}

CAViewController* CAWindow::getRootViewController()
{
    return m_pRootViewController;
}

void CAWindow::presentModalViewController(CAViewController* controller, bool animated)
{
    CC_RETURN_IF(controller == nullptr);
    CC_RETURN_IF(m_pModalViewController);
    CC_SAFE_RETAIN(controller);
    m_pModalViewController = controller;
    
    m_pModalViewController->addViewFromSuperview(this);
    m_pModalViewController->getView()->setZOrder(CAWindowZOrderCenter);
    m_pModalViewController->setViewVisibleTrue();
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    if (animated)
    {
        CAView* view = m_pModalViewController->getView();
        DLayout layout = view->getLayout();
        float y = m_obContentSize.height;
        layout.vertical = DVerticalLayout_T_B(y, -y);
        view->setLayout(layout);

        CAViewAnimation::beginAnimations("");
        CAViewAnimation::setAnimationDuration(0.3f);
        CAViewAnimation::setAnimationDelay(0.1f);
        CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
        CAViewAnimation::setAnimationDidStopSelector([&]()
        {
            if (m_pRootViewController)
            {
                m_pRootViewController->setViewVisibleFalse();
            }
            CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
        });
        view->setLayout(DLayoutFill);
        CAViewAnimation::commitAnimations();
    }
    else
    {
        if (m_pRootViewController)
        {
            m_pRootViewController->setViewVisibleFalse();
        }
        CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
    }
}

void CAWindow::dismissModalViewController(bool animated)
{
    CC_RETURN_IF(m_pModalViewController == NULL);
    
    if (m_pRootViewController)
    {
        m_pRootViewController->setViewVisibleTrue();
    }
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    if (animated)
    {
        CAView* view = m_pModalViewController->getView();

        CAViewAnimation::beginAnimations("");
        CAViewAnimation::setAnimationDuration(0.3f);
        CAViewAnimation::setAnimationDelay(0.1f);
        CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
        CAViewAnimation::setAnimationDidStopSelector([&]()
        {
            m_pModalViewController->setViewVisibleFalse();
            m_pModalViewController->removeViewFromSuperview();
            CC_SAFE_RELEASE_NULL(m_pModalViewController);
            CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
        });
        DLayout layout = view->getLayout();
        float y = m_obContentSize.height;
        layout.vertical = DVerticalLayout_T_B(y, -y);
        view->setLayout(layout);
        CAViewAnimation::commitAnimations();
    }
    else
    {
        m_pModalViewController->setViewVisibleFalse();
        m_pModalViewController->removeViewFromSuperview();
        CC_SAFE_RELEASE_NULL(m_pModalViewController);
        CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
    }
    
}

void CAWindow::render(Renderer* renderer, const Mat4& eyeTransform, const Mat4* eyeProjection)
{
    auto application = CAApplication::getApplication();
    CACamera* defaultCamera = nullptr;
    const auto& transform = getViewToSuperviewTransform();

    for (const auto& camera : getCameras())
    {
        if (!camera->isVisible())
            continue;
        
        CACamera::_visitingCamera = camera;
        if (CACamera::_visitingCamera->getCameraFlag() == CACameraFlag::DEFAULT)
        {
            defaultCamera = CACamera::_visitingCamera;
        }
        
        if (eyeProjection)
            camera->setAdditionalProjection(*eyeProjection * camera->getProjectionMatrix().getInversed());
        
        Mat4 eyeMat4 = eyeTransform.getInversed();
        camera->setAdditionalTransform(&eyeMat4);
        
        application->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
        application->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, CACamera::_visitingCamera->getViewProjectionMatrix());
        camera->apply();
        //clear background with max depth
        camera->clearBackground();
        //visit the scene
        visit(renderer, transform, 0);
        
        renderer->render();
        camera->restore();
        
        application->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
        
        // we shouldn't restore the transform matrix since it could be used
        // from "update" or other parts of the game to calculate culling or something else.
        //        camera->setNodeToParentTransform(eyeCopy);
    }
    
    CACamera::_visitingCamera = nullptr;
}

void CAWindow::removeAllSubviews()
{
    if (m_pDefaultCamera)
    {
        m_pDefaultCamera->retain();
    }
    CAView::removeAllSubviews();
    
    if (m_pDefaultCamera)
    {
        addSubview(m_pDefaultCamera);
        m_pDefaultCamera->release();
    }
}

static bool camera_cmp(const CACamera* a, const CACamera* b)
{
    return a->getRenderOrder() < b->getRenderOrder();
}

const std::vector<CACamera*>& CAWindow::getCameras()
{
    if (m_bCameraOrderDirty)
    {
        stable_sort(m_vCameras.begin(), m_vCameras.end(), camera_cmp);
        m_bCameraOrderDirty = false;
    }
    return m_vCameras;
}

NS_CC_END
