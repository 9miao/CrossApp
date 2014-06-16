//
//  CAImageView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-18.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CAIMAGEVIEW__
#define __CAIMAGEVIEW__

#include "sprite_nodes/CAView.h"

NS_CC_BEGIN

class CCSpriteFrame;
class CCAnimation;
class CAScale9ImageView;

class CC_DLL CAImageView : public CAView
{
public:

    static CAImageView* create();

    static CAImageView* createWithImage(CAImage* image);

    static CAImageView* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);

    static CAImageView* createWithFrame(const CCRect& rect);
    
    static CAImageView* createWithCenter(const CCRect& rect);
    
    CAImageView(void);

    virtual ~CAImageView(void);

    virtual bool init(void);

    virtual bool initWithImage(CAImage* image);
    
    virtual bool initWithSpriteFrame(CCSpriteFrame *pSpriteFrame);

    virtual void setImage(CAImage* image);
    
    virtual void setDisplayFrame(CCSpriteFrame *pNewFrame);

    virtual bool isFrameDisplayed(CCSpriteFrame *pFrame);

    virtual CCSpriteFrame* displayFrame(void);

    virtual void setDisplayFrameWithAnimationName(const char *animationName, int frameIndex);

protected:

    virtual void updateImageRect();
    
    friend class CAScale9ImageView;
};

NS_CC_END

#endif
