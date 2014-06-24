//
//  CASlider.cpp
//  project
//
//  Created by lh on 14-4-16.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAProgress.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"
#include "view/CAScale9ImageView.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "support/CCPointExtension.h"
NS_CC_BEGIN

CAProgress::CAProgress()
:m_fProgress(0.0f)
,m_progressTintColor(CAColor_white)
,m_progresstrackColor(CAColor_white)
,m_pProgressStyle(CAProgressStyleDefault)
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
    return true;
}

void CAProgress::onEnterTransitionDidFinish()
{ 
    CAView::onEnterTransitionDidFinish();
    
    if (m_pTarckImageView == NULL ) 
	{
		if (m_pProgressTrackImage == NULL)
		{
            this->setProgressTrackImage(CAImage::create("source_material/btn_square_disabled.png"));
		}
		
		m_pTarckImageView = CAImageView::createWithImage(this->getImage(m_pProgressTrackImage));
		m_pTarckImageView->setFrame(CCRectZero);
        this->insertSubview(m_pTarckImageView, -1);
		m_pTarckImageView->setColor(m_progresstrackColor);

		if (m_pProgressTintImage == NULL)
		{
            this->setProgressTintImage(CAImage::create("source_material/btn_square_highlighted.png"));
		}
        
		m_pProgressImageView = CAImageView::createWithImage(this->getImage(m_pProgressTintImage));
		m_pProgressImageView->setAnchorPoint(CCPointZero);
		m_pProgressImageView->setFrame(CCRectZero);
        this->addSubview(m_pProgressImageView);
		m_pProgressImageView->setColor(m_progressTintColor);

		CCRect rect = m_pProgressImageView->getBounds();
		rect.size.width *= m_fProgress;
		m_pProgressImageView->setImageRect(rect, false, rect.size);

        this->addSubview(m_pIndicator);
    }
}

void CAProgress::onExitTransitionDidStart()
{
	CAView::onExitTransitionDidStart();
}

CAImage* CAProgress::getImage(CAImage* image)
{
	CAScale9ImageView *scale9Image = CAScale9ImageView::createWithImage(image);
    scale9Image->setAnchorPoint(CCPointZero);
	scale9Image->setFrame(this->getBounds());
    this->addSubview(scale9Image);
    
	CARenderImage* render = CARenderImage::create(this->getBounds().size.width, this->getBounds().size.height, kCAImagePixelFormat_RGBA8888);
	render->beginWithClear(0, 0, 0, 0);
    scale9Image->visit();
	render->end();
    scale9Image->removeFromSuperview();
	return render->getSprite()->getImage();
}

void CAProgress::setProgress(float progress, bool animated)
{

	progress = MIN(1.0f, progress);
	progress = MAX(0.0f, progress);

	CCPoint point = CCPoint(this->getBounds().size.width * progress, 0);

	if (animated)
	{
		float time = fabsf(progress - m_fProgress) * 0.3f;

		if (m_pIndicator->getActionByTag(0xffff))
		{
			m_pIndicator->stopAllActions();
			this->animatedFinish();
		}
		
		CCFrameOrginTo *moveTo = CCFrameOrginTo::create(time, point);
		CCCallFunc* begin = CCCallFunc::create(this, callfunc_selector(CAProgress::animatedBegin));
		CCCallFunc* finish = CCCallFunc::create(this, callfunc_selector(CAProgress::animatedFinish));
		CCSequence* animateds = CCSequence::create(begin, moveTo, finish, NULL);
		animateds->setTag(0xffff);
		m_pIndicator->runAction(animateds);
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

	m_pProgressImageView->setImageRect(rect, false, rect.size);
    
	m_fProgress = rect.size.width / this->getBounds().size.width;
}

void CAProgress::animatedBegin()
{
	CAScheduler::schedule(schedule_selector(CAProgress::update), this, 1 / 60.0f, false);
}

void CAProgress::animatedFinish()
{
	CAScheduler::unschedule(schedule_selector(CAProgress::update), this);
}

void CAProgress::setContentSize(const CCSize & var)
{
    CAView::setContentSize(CCSize(var.width, 6.0f));
}

NS_CC_END
