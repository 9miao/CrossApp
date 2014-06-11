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

    static CAImageView* createWithImage(CAImage* image, const CCRect& rect);

    static CAImageView* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);

    CAImageView(void);

    virtual ~CAImageView(void);

    virtual bool init(void);

    virtual bool initWithImage(CAImage* image);

    virtual bool initWithImage(CAImage* image, const CCRect& rect);

    virtual bool initWithImage(CAImage* image, const CCRect& rect, bool rotated);

    virtual bool initWithSpriteFrame(CCSpriteFrame *pSpriteFrame);

    virtual void setDisplayFrame(CCSpriteFrame *pNewFrame);

    virtual bool isFrameDisplayed(CCSpriteFrame *pFrame);

    virtual CCSpriteFrame* displayFrame(void);

	void setFrame(const CCRect& rect);
    
    void setFrameOrigin(const CCPoint& point);

    void setCenter(const CCRect& rect);
    
    CCRect getCenter();
    
    void setCenterOrigin(const CCPoint& point);
    
    CCPoint getCenterOrigin();
    
	void setBounds(const CCRect& rect);

    CCRect getBounds() const;

    virtual void setDisplayFrameWithAnimationName(const char *animationName, int frameIndex);

    virtual void setImage(CAImage* image);
    
    friend class CAScale9ImageView;
};

NS_CC_END

#endif
