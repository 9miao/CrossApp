//
//  CASlider.cpp
//  project
//
//  Created by lh on 14-4-16.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#include "CAProgress.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"
#include "sprite_nodes/CCScale9Sprite.h"
#include "CCDirector.h"
#include "CCScheduler.h"
#include "support/CCPointExtension.h"
NS_CC_BEGIN

CAProgress::CAProgress()
:m_fProgress(0.0f)
,m_progressTintColor(ccBLUE)
,m_progresstrackColor(ccGRAY)
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
    return true;

}

void CAProgress::onEnterTransitionDidFinish()
{ 
    CAView::onEnterTransitionDidFinish();
    
    if (m_pTarckImageView == NULL ) 
	{
		if (m_pProgressTrackImage == NULL)
		{
            this->setProgressTrackImage(CAImage::create("WhiteDots.png"));
		}
		
		m_pTarckImageView = CAImageView::createWithImage(this->getImage(m_pProgressTrackImage));
		m_pTarckImageView->setFrame(CCRectZero);
        this->insertSubview(m_pTarckImageView, -1);
		m_pTarckImageView->setColor(m_progresstrackColor);

		if (m_pProgressTintImage == NULL)
		{
            this->setProgressTintImage(CAImage::create("WhiteDots.png"));
		}
        
		m_pProgressImageView = CAImageView::createWithImage(this->getImage(m_pProgressTintImage));
		m_pProgressImageView->setAnchorPoint(CCPointZero);
		m_pProgressImageView->setFrame(CCRectZero);
        this->addSubview(m_pProgressImageView);
		m_pProgressImageView->setColor(m_progressTintColor);

		CCRect rect = m_pProgressImageView->getBounds();
		rect.size.width *= m_fProgress;
		m_pProgressImageView->setTextureRect(rect);

        this->addSubview(m_pIndicator);
    }
}

void CAProgress::onExitTransitionDidStart()
{
	CAView::onExitTransitionDidStart();
}

CAImage* CAProgress::getImage(CAImage* image)
{
	CCScale9Sprite *scale9Image = CCScale9Sprite::createWithImage(image);
    CCRect rect;
    rect.origin = scale9Image->getFrame().size/2;
    rect.origin = ccpSub(rect.origin, CCPoint(0.5f, 0.5f));
    rect.size = CCSize(1, 1);
    scale9Image->setCapInsets(rect);
	scale9Image->setPreferredSize(this->getBounds().size);
    scale9Image->setAnchorPoint(CCPointZero);
    this->addSubview(scale9Image);
    
	CCRenderTexture* render = CCRenderTexture::create(this->getBounds().size.width, this->getBounds().size.height, kCCTexture2DPixelFormat_RGBA8888);
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
		
		CCMoveTo *moveTo = CCMoveTo::create(time, point);
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

	m_pProgressImageView->setTextureRect(rect);
    
	m_fProgress = rect.size.width / this->getBounds().size.width;
}

void CAProgress::animatedBegin()
{
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CAProgress::update), this, 1 / 60.0f, false);
}

void CAProgress::animatedFinish()
{
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(CAProgress::update), this);
}

NS_CC_END
