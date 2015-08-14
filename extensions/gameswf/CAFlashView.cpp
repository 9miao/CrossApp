//
//  extension::CAFlashView.cpp
//  CrossApp
//
//  Created by qiaoxin on 15-05-12.
//

#include "CrossApp.h"
#include "CAFlashView.h"


NS_CC_EXT_BEGIN

extension::CAFlashView::CAFlashView()
:m_pFlash(NULL)
,m_bIsRepeatForever(false)
,m_bIsRunning(false)
{
    this->setDisplayRange(false);
}

extension::CAFlashView::~CAFlashView()
{
    this->stopAnimation();
    CC_SAFE_DELETE(m_pFlash);
}

extension::CAFlashView* extension::CAFlashView::createWithFlash(CAFlash* flash)
{
    extension::CAFlashView* view = new extension::CAFlashView();
    if ( view && view->initWithFlash(flash)) {
        view->autorelease();
        return view;
    }
    return NULL;
}

bool extension::CAFlashView::initWithFlash(CAFlash* flash)
{
    if (!this->init())
    {
        return false;
    }
    
    this->setFlash(flash);
    return true;
}

bool extension::CAFlashView::init()
{
    return true;
}

void extension::CAFlashView::setFlash(CAFlash* flash)
{
    CC_SAFE_RETAIN(flash);
    CC_SAFE_DELETE(m_pFlash);
    m_pFlash = flash;
}

void extension::CAFlashView::onEnter()
{
    CAView::onEnter();
}

void extension::CAFlashView::onExit()
{
    CAView::onExit();
    if(!this->isRunning())
        return;
    CAScheduler::unschedule(schedule_selector(extension::CAFlashView::update), this);
    this->setRunning(false);
}

void extension::CAFlashView::runAnimation()
{
    if(this->isRunning())
        return;
    this->setRunning(true);
    m_pFlash->setIndex(0);
    m_pFlash->playRun();
    CAScheduler::schedule(schedule_selector(extension::CAFlashView::update), this, 0.01f);
}

void extension::CAFlashView::stopAnimation()
{
    if(! this->isRunning())
        return;
    CAScheduler::unschedule(schedule_selector(extension::CAFlashView::update), this);
    this->setRunning(false);
}

void extension::CAFlashView::update(float dt)
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

void extension::CAFlashView::draw()
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
    float localScaleX = size.width/ m_pFlash->getWidth();
    float localScaleY = -(size.height / m_pFlash->getHeight());
    float localScale = localScaleX > -localScaleY ? -localScaleY : localScaleX;
    kmMat4 matrixs;
    kmMat4Scaling(&matrixs, localScale, -localScale, getZOrder());
    kmMat4Multiply(&matrixMVP, &matrixMVP, &matrixs);
    m_pFlash->display(&matrixMVP);
    CHECK_GL_ERROR_DEBUG();
}

void extension::CAFlashView::setRepeatForever(bool repeatForever)
{
    m_bIsRepeatForever = repeatForever;
}

bool extension::CAFlashView::isRepeatForever()
{
    return m_bIsRepeatForever;
}

void extension::CAFlashView::setRunning(bool running)
{
    m_bIsRunning = running;
}

bool extension::CAFlashView::isRunning()
{
    return m_bIsRunning;
}

NS_CC_EXT_END
