//
//  CAFlashView.cpp
//  CrossApp
//
//  Created by qiaoxin on 15-05-12.
//

#include "CrossApp.h"
#include "CAFlashView.h"


NS_CC_BEGIN

CAFlashView::CAFlashView()
:m_pFlash(NULL)
,m_bIsRepeatForever(false)
,m_bIsRunning(false)
{
    this->setDisplayRange(false);
}

CAFlashView::~CAFlashView()
{
    this->stopAnimation();
    CC_SAFE_DELETE(m_pFlash);
}

CAFlashView* CAFlashView::createWithFlash(CAFlash* flash)
{
    CAFlashView* view = new CAFlashView();
    if ( view && view->initWithFlash(flash)) {
        view->autorelease();
        return view;
    }
    return NULL;
}

bool CAFlashView::initWithFlash(CAFlash* flash)
{
    if (!this->init())
    {
        return false;
    }
    
    this->setFlash(flash);
    return true;
}

bool CAFlashView::init()
{
    return true;
}

void CAFlashView::setFlash(CAFlash* flash)
{
    CC_SAFE_RETAIN(flash);
    CC_SAFE_DELETE(m_pFlash);
    m_pFlash = flash;
}

void CAFlashView::onEnter()
{
    CAView::onEnter();
}

void CAFlashView::onExit()
{
    CAView::onExit();
    if(!this->isRunning())
        return;
    CAScheduler::unschedule(schedule_selector(CAFlashView::update), this);
    this->setRunning(false);
}

void CAFlashView::runAnimation()
{
    if(this->isRunning())
        return;
    this->setRunning(true);
    m_pFlash->setIndex(0);
    m_pFlash->playRun();
    CAScheduler::schedule(schedule_selector(CAFlashView::update), this, 0.01f);
}

void CAFlashView::stopAnimation()
{
    if(! this->isRunning())
        return;
    CAScheduler::unschedule(schedule_selector(CAFlashView::update), this);
    this->setRunning(false);
}

void CAFlashView::update(float dt)
{
    CAView::update(dt);
    int index = m_pFlash->getIndex();
    int counts = m_pFlash->getCounts();

    bool tobestop = false;
    if(! m_bIsRepeatForever)
    {
        if (index == counts - 1)
        {
            tobestop = true;
        }
    }
    m_pFlash->advance(dt);
    if(tobestop)
    {
        this->stopAnimation();
    }
    this->updateDraw();
}

void CAFlashView::draw()
{
    CAView::draw();
    ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
    kmMat4 matrixP;
    kmMat4 matrixMV;
    kmMat4 matrixMVP;
    kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
    kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );
    matrixMV.mat[13] = this->getFrame().size.height + matrixMV.mat[13];
    kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
#if CC_ENABLE_GL_STATE_CACHE
    ccGLUseProgram(0);//valid program is NON_ZERO unsigned int
#endif
    CCSize size = this->getFrame().size;
    float localScaleX = size.width/ m_pFlash->getWidth()/this->getScaleX();
    float localScaleY = -(size.height / m_pFlash->getHeight()/this->getScaleY());
    float localScale = localScaleX > -localScaleY ? -localScaleY : localScaleX;
    kmMat4 matrixs;
    kmMat4Scaling(&matrixs, localScale, -localScale, getZOrder());
    kmMat4Multiply(&matrixMVP, &matrixMVP, &matrixs);
    m_pFlash->display(&matrixMVP);
    CHECK_GL_ERROR_DEBUG();
}

void CAFlashView::setRepeatForever(bool repeatForever)
{
    m_bIsRepeatForever = repeatForever;
}

bool CAFlashView::isRepeatForever()
{
    return m_bIsRepeatForever;
}

void CAFlashView::setRunning(bool running)
{
    m_bIsRunning = running;
}

bool CAFlashView::isRunning()
{
    return m_bIsRunning;
}

NS_CC_END
