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
#include "support/CCPointExtension.h"
#include "basics/CAApplication.h"
#include "animation/CAViewAnimation.h"

NS_CC_BEGIN

CAProgress::CAProgress()
:m_fProgress(0.0f)
,m_cProgressTintColor(CAColor_white)
,m_cProgressTrackColor(CAColor_white)
,m_pProgressTintImage(NULL)
,m_pProgressTrackImage(NULL)
,m_pProgressImageView(NULL)
,m_pTarckImageView(NULL)
{
    m_pIndicator = CAView::create();
    m_pIndicator->retain();
	m_pIndicator->setFrameOrigin(CCPointZero);
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

bool CAProgress::init()
{
    if (!CAView::init())
    {
        return false;
    }
    this->setColor(CAColor_clear);
    
    
    m_pTarckImageView = CAScale9ImageView::createWithImage(NULL);
    m_pTarckImageView->setFrame(this->getBounds());
    this->insertSubview(m_pTarckImageView, -1);
    
    m_pProgressImageView = CAScale9ImageView::createWithImage(NULL);
    m_pProgressImageView->setFrame(this->getBounds());
    this->addSubview(m_pProgressImageView);
    
    CCRect rect = m_pProgressImageView->getBounds();
    rect.size.width *= m_fProgress;
    m_pProgressImageView->setFrame(rect);
    
    this->addSubview(m_pIndicator);
    
    return true;
}

void CAProgress::onEnterTransitionDidFinish()
{ 
    CAView::onEnterTransitionDidFinish();
    
    if (m_pProgressTrackImage == NULL)
    {
        this->setProgressTrackImage(CAImage::create("source_material/btn_square_disabled.png"));
    }
    
    if (m_pProgressTintImage == NULL)
    {
        this->setProgressTintImage(CAImage::create("source_material/btn_square_highlighted.png"));
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
    CC_SAFE_RELEASE_NULL(m_pProgressTintImage);
    CC_SAFE_RETAIN(var);
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
    CC_SAFE_RELEASE_NULL(m_pProgressTrackImage);
    CC_SAFE_RETAIN(var);
    m_pProgressTrackImage = var;
    m_pTarckImageView->setImage(m_pProgressTrackImage);
}

CAImage* CAProgress::getProgressTrackImage()
{
    return m_pProgressTrackImage;
}


void CAProgress::setProgress(float progress, bool animated)
{
	progress = MIN(1.0f, progress);
	progress = MAX(0.0f, progress);

	CCPoint point = CCPoint(this->getBounds().size.width * progress, 0);

	if (animated)
	{
        this->animatedBegin();
		float time = fabsf(progress - m_fProgress) * 0.3f;
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(time);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CAProgress::animatedFinish));
        m_pIndicator->setFrameOrigin(point);
        CAViewAnimation::commitAnimations();
	}
	else
	{
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
	CCRect rect = CCRect(0, 0, m_pIndicator->getFrameOrigin().x, this->getBounds().size.height);

	m_pProgressImageView->setFrame(rect);
    
	m_fProgress = rect.size.width / this->getBounds().size.width;
}

void CAProgress::animatedBegin()
{
    CAScheduler::unschedule(schedule_selector(CAProgress::update), this);
	CAScheduler::schedule(schedule_selector(CAProgress::update), this, 1 / 60.0f);
}

void CAProgress::animatedFinish()
{
	CAScheduler::unschedule(schedule_selector(CAProgress::update), this);
}

void CAProgress::setContentSize(const CCSize & var)
{
    CAView::setContentSize(CCSize(var.width, MAX(var.height, _px(6))));
    
    m_pTarckImageView->setFrame(this->getBounds());
    CCRect rect = CCRect(0, 0, m_pIndicator->getFrameOrigin().x, this->getBounds().size.height);
    m_pProgressImageView->setFrame(rect);
}

NS_CC_END
