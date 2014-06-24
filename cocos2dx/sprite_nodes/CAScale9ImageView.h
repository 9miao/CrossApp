//
//  CAScale9ImageView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-6-17.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CAScale9ImageView_H__
#define __CAScale9ImageView_H__

#include "CAImageView.h"

NS_CC_BEGIN

class CC_DLL CAScale9ImageView : public CAView
{
public:
    
    CAScale9ImageView();
    
    virtual ~CAScale9ImageView();

    static CAScale9ImageView* create();
    
    static CAScale9ImageView* createWithImage(CAImage* image);
    
    virtual bool init();
    
	virtual bool initWithImage(CAImage* image);
    
    virtual void setCapInsets(const CCRect& capInsets);
    
    virtual void setInsetLeft(float insetLeft);
    
    virtual void setInsetTop(float insetTop);
    
    virtual void setInsetRight(float insetRight);
    
    virtual void setInsetBottom(float insetBottom);
    
    virtual void setColor(const CAColor4B& color);
    
    virtual const CAColor4B& getColor();
    
    CAScale9ImageView* copy();
    
public:

    CC_SYNTHESIZE_READONLY(CCSize, m_obOriginalSize, OriginalSize);

    CC_SYNTHESIZE_READONLY(CCRect, m_obCapInsets, CapInsets);

    CC_SYNTHESIZE_READONLY(float, m_fInsetLeft, InsetLeft);

    CC_SYNTHESIZE_READONLY(float, m_fInsetTop, InsetTop);

    CC_SYNTHESIZE_READONLY(float, m_fInsetRight, InsetRight);

    CC_SYNTHESIZE_READONLY(float, m_fInsetBottom, InsetBottom);
    
protected:
    
    void updateCapInset();
    
    void updatePositions();

    virtual void setContentSize(const CCSize & size);
    
    bool updateWithImage(CABatchView* batch, CCRect rect,const CCRect& capInsets);
    
    virtual void updateDisplayedColor(const cocos2d::CAColor4B& parentColor);
    
protected:

    CABatchView* m_pScale9ImageView;
    
    CAImageView* m_pImageView[9];
    
    CCRect m_rFrame[9];
    
public:
    
    CC_DEPRECATED_ATTRIBUTE bool initWithImage(CAImage* image, CCRect rect, CCRect capInsets);
    
	CC_DEPRECATED_ATTRIBUTE static CAScale9ImageView* createWithImage(CAImage* image, CCRect rect, CCRect capInsets);
    
	CC_DEPRECATED_ATTRIBUTE bool initWithImage(CAImage* image, CCRect rect);
    
	CC_DEPRECATED_ATTRIBUTE static CAScale9ImageView* createWithImage(CAImage* image, CCRect rect);
    
	CC_DEPRECATED_ATTRIBUTE bool initWithImage(CCRect capInsets, CAImage* image);
    
	CC_DEPRECATED_ATTRIBUTE CAScale9ImageView* createWithImage(CCRect capInsets, CAImage* image);
    
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CAScale9ImageView_H__
