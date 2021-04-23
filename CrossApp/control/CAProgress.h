//
//  CASlider.h
//  project
//
//  Created by lh on 14-4-16.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __project__CASlider__
#define __project__CASlider__

#include <iostream>
#include "control/CAControl.h"
#include "view/CAScale9ImageView.h"
#include "view/CARenderImage.h"

NS_CC_BEGIN

class CC_DLL CAProgress : public CAControl
{
public:
    
    CAProgress();
    
    virtual ~CAProgress();
    
    static CAProgress* create();
    static CAProgress* createWithFrame(const DRect& rect);
    static CAProgress* createWithCenter(const DRect& rect);
    static CAProgress* createWithLayout(const DLayout& layout);
    
    bool init();
    
	virtual void setColor(const CAColor4B& color);
    
	void setProgress(float progress, bool animated = false);
	float getProgress();

public:
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cProgressTintColor, ProgressTintColor);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cProgressTrackColor, ProgressTrackColor);
    
	CC_PROPERTY(CAImage*, m_pProgressTintImage, ProgressTintImage);
    
	CC_PROPERTY(CAImage*, m_pProgressTrackImage, ProgressTrackImage);

public:
    
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
    
protected:

    void setContentSize(const DSize & var);
    
	void update(float dt);
    
	void animatedBegin();

	void animatedFinish();

    CAScale9ImageView* m_pTarckImageView;
    
	CAScale9ImageView* m_pProgressImageView;
    
	float m_fProgress;

    CAView* m_pIndicator;
};

NS_CC_END
#endif /* defined(__project__CASlider__) */
