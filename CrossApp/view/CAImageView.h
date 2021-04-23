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

class CC_DLL CAImageView : public CAView
{
public:

    enum class ScaleType
    {
        FitImageXY = 0,         //塞满
        FitImageCrop,           //塞满裁剪，不变形, 居中
        FitImageInside,         //显示全部，不变形, 居中
        FitViewByHorizontal,    //显示全部，不变形，横向适应图片
        FitViewByVertical       //显示全部，不变形，纵横适应图片
    };
    
public:
    
    static CAImageView* create();

    static CAImageView* createWithImage(CAImage* image);

    static CAImageView* createWithFrame(const DRect& rect);
    
    static CAImageView* createWithCenter(const DRect& rect);
    
    static CAImageView* createWithLayout(const DLayout& layout);
    
    CAImageView(void);

    virtual ~CAImageView(void);

    virtual bool init(void);

    virtual bool initWithImage(CAImage* image);
    
    virtual void setImage(CAImage* image);
    
    virtual CAImage* getImage(void);

    virtual void setImageAsyncWithFile(const std::string& path);
    
    virtual void setImageRect(const DRect& rect);
    
    CC_PROPERTY(CAImageView::ScaleType, m_eScaleType, ScaleType);
    
    CC_SYNTHESIZE(CAVector<CAImage*>, m_vAnimationImages, AnimationImages);
    
    CC_SYNTHESIZE(float, m_fAnimationDuration, AnimationDuration);//default is number of images * 1/30th
    
    CC_SYNTHESIZE(unsigned int, m_iAnimationRepeatCount, AnimationRepeatCount);//(default is 0)
    
    void startAnimating();
    void stopAnimating();
    bool isAnimating();
    
    virtual CAView* copy();
    
protected:

    void update(float dt);
    
    virtual void setContentSize(const DSize& contentSize);
    
    virtual void updateByScaleType();
    
    virtual void updateImageRect();
    
    using CAView::initWithColor;
    
    bool m_bUpdateByScaleType;
    
    float m_fLeft;
    
    float m_fRight;
    
    float m_fTop;
    
    float m_fBottom;
    
    int m_fAnimationRunTime;
    
    bool m_bAnimating;
};

NS_CC_END

#endif
