//
//  CASlider.cpp
//  project
//
//  Created by lh on 14-4-16.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAProgress.h"
#include "view/CAScale9ImageView.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "basics/CAPointExtension.h"
#include "basics/CAApplication.h"
#include "animation/CAViewAnimation.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN

CAProgress::CAProgress()
:m_fProgress(0.0f)
,m_cProgressTintColor(CAColor4B::WHITE)
,m_cProgressTrackColor(CAColor4B::WHITE)
,m_pProgressTintImage(nullptr)
,m_pProgressTrackImage(nullptr)
,m_pProgressImageView(nullptr)
,m_pTarckImageView(nullptr)
{
    this->setHaveNextResponder(true);
    this->setMultipleTouchEnabled(true);
    
    m_pIndicator = CAView::create();
    m_pIndicator->retain();
	m_pIndicator->setFrameOrigin(DPointZero);
}

CAProgress::~CAProgress()
{
    CC_SAFE_RELEASE_NULL(m_pIndicator);
	CC_SAFE_RELEASE_NULL(m_pProgressTrackImage);
	CC_SAFE_RELEASE_NULL(m_pProgressTintImage);
}

CAProgress* CAProgress::create()
{
    CAProgress *progress = new CAProgress();
    if (progress && progress->init())
    {
        progress->autorelease();
        return progress;
    }
    CC_SAFE_DELETE(progress);
    return NULL;
}

CAProgress* CAProgress::createWithFrame(const DRect& rect)
{
    CAProgress *progress = new CAProgress();
    if (progress && progress->initWithFrame(rect))
    {
        progress->autorelease();
        return progress;
    }
    CC_SAFE_DELETE(progress);
    return NULL;
}

CAProgress* CAProgress::createWithCenter(const DRect& rect)
{
    CAProgress *progress = new CAProgress();
    if (progress && progress->initWithCenter(rect))
    {
        progress->autorelease();
        return progress;
    }
    CC_SAFE_DELETE(progress);
    return NULL;
}

CAProgress* CAProgress::createWithLayout(const DLayout& layout)
{
    CAProgress *progress = new CAProgress();
    if (progress && progress->initWithLayout(layout))
    {
        progress->autorelease();
        return progress;
    }
    CC_SAFE_DELETE(progress);
    return NULL;
}

bool CAProgress::init()
{
    if (!CAView::init())
    {
        return false;
    }
    CAView::setColor(CAColor4B::CLEAR);
    
    m_pTarckImageView = CAScale9ImageView::createWithImage(nullptr);
    m_pTarckImageView->setFrame(this->getBounds());
    this->insertSubview(m_pTarckImageView, -1);
    
    m_pProgressImageView = CAScale9ImageView::createWithImage(nullptr);
    m_pProgressImageView->setFrame(this->getBounds());
    this->addSubview(m_pProgressImageView);
    
    DRect rect = m_pProgressImageView->getBounds();
    rect.size.width *= m_fProgress;
    m_pProgressImageView->setFrame(rect);
    
    this->addSubview(m_pIndicator);
    
    return true;
}

void CAProgress::onEnterTransitionDidFinish()
{ 
    CAView::onEnterTransitionDidFinish();
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CAProgress");
    if (m_pProgressTrackImage == NULL)
    {
        this->setProgressTrackImage(CAImage::create(map.at("trackImage")));
    }
    
    if (m_pProgressTintImage == NULL)
    {
        this->setProgressTintImage(CAImage::create(map.at("tintImage")));
    }
}

void CAProgress::onExitTransitionDidStart()
{
	CAView::onExitTransitionDidStart();
}

void CAProgress::setProgressTintColor(const CAColor4B &var)
{
    m_cProgressTintColor = var;
    m_pProgressImageView->setColor(m_cProgressTintColor);
}

const CAColor4B& CAProgress::getProgressTintColor()
{
    return m_cProgressTintColor;
}

void CAProgress::setProgressTintImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pProgressTintImage);
    m_pProgressTintImage = var;
    m_pProgressImageView->setImage(m_pProgressTintImage);
}

CAImage* CAProgress::getProgressTintImage()
{
    return m_pProgressTintImage;
}

void CAProgress::setProgressTrackColor(const CAColor4B &var)
{
    m_cProgressTrackColor = var;
    m_pTarckImageView->setColor(m_cProgressTrackColor);
}

const CAColor4B& CAProgress::getProgressTrackColor()
{
    return m_cProgressTrackColor;
}

void CAProgress::setProgressTrackImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pProgressTrackImage);
    m_pProgressTrackImage = var;
    m_pTarckImageView->setImage(m_pProgressTrackImage);
}

CAImage* CAProgress::getProgressTrackImage()
{
    return m_pProgressTrackImage;
}

void CAProgress::setColor(const CAColor4B& color)
{
	setProgressTintColor(color);
	setProgressTrackColor(color);
}

void CAProgress::setProgress(float progress, bool animated)
{
	progress = MIN(1.0f, progress);
	progress = MAX(0.0f, progress);
    
	DPoint point = DPoint(m_obContentSize.width * progress, 0);

	if (animated)
	{
        this->animatedBegin();
		float time = fabsf(progress - m_fProgress) * 0.5f;
        CAViewAnimation::beginAnimations("");
        CAViewAnimation::setAnimationDuration(time);
        CAViewAnimation::setAnimationDidStopSelector(std::bind(&CAProgress::animatedFinish, this));
        m_pIndicator->setFrameOrigin(point);
        CAViewAnimation::commitAnimations();
	}
	else
	{
        m_fProgress = progress;
		m_pIndicator->setFrameOrigin(point);

		this->update(0);
	}
}

float CAProgress::getProgress()
{
	return m_fProgress;
}

void CAProgress::update(float dt)
{
    m_fProgress = m_pIndicator->getFrameOrigin().x / m_obContentSize.width;
    
	DRect rect = DRect(0, 0, m_pIndicator->getFrameOrigin().x, m_obContentSize.height);

	m_pProgressImageView->setFrame(rect);
}

void CAProgress::animatedBegin()
{
	CAScheduler::getScheduler()->scheduleUpdate(this, CAScheduler::PRIORITY_SYSTEM, false);
}

void CAProgress::animatedFinish()
{
    this->update(0);
    CAScheduler::getScheduler()->unscheduleUpdate(this);
}

void CAProgress::setContentSize(const DSize & var)
{
    DSize size = var;
    if (m_bRecSpe)
    {
        const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CAProgress");
        int h = atoi(map.at("height").c_str());
        size.height = (h == 0) ? size.height : h;
    }
    CAView::setContentSize(size);
    
    m_pTarckImageView->setFrame(this->getBounds());
    DRect rect = DRect(0, 0, m_pIndicator->getFrameOrigin().x, this->getBounds().size.height);
    m_pProgressImageView->setFrame(rect);
    this->setProgress(m_fProgress);
}

NS_CC_END
