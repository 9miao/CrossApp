//
//  CAImageView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-18.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CAIMAGEVIEW__
#define __CAIMAGEVIEW__

#include "CAView.h"

NS_CC_BEGIN

class CAImageFrame;
class CAAnimation;

class CC_DLL CAImageView : public CAView
{
public:

    static CAImageView* create();

    static CAImageView* createWithImage(CAImage* image);

    static CAImageView* createWithSpriteFrame(CAImageFrame *pSpriteFrame);

    static CAImageView* createWithFrame(const CCRect& rect);
    
    static CAImageView* createWithCenter(const CCRect& rect);
    
    CAImageView(void);

    virtual ~CAImageView(void);

    virtual bool init(void);

    virtual bool initWithImage(CAImage* image);
    
    virtual bool initWithSpriteFrame(CAImageFrame *pSpriteFrame);

    virtual void setImage(CAImage* image);
    
    virtual CAImage* getImage(void);
    
    virtual void setDisplayFrame(CAImageFrame *pNewFrame);

    virtual bool isFrameDisplayed(CAImageFrame *pFrame);

    virtual CAImageFrame* displayFrame(void);

    virtual void setDisplayFrameWithAnimationName(const char *animationName, int frameIndex);

    virtual void setImageAsyncWithFile(const char* fileName);
    
    using CAView::setImageRect;
    
protected:

    void asyncFinish(CAObject* var);
    
    virtual void updateImageRect();
    
    virtual bool initWithFrame(const CCRect& rect, const CAColor4B& color4B);
  
    virtual bool initWithCenter(const CCRect& rect, const CAColor4B& color4B);
    
    using CAView::initWithColor;
};

NS_CC_END

#endif
