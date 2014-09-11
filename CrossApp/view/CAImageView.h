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

typedef enum
{
    CAImageViewScaleTypeFitImageXY = 0,         //塞满
    CAImageViewScaleTypeFitImageCrop,           //塞满裁剪，不变形, 居中
    CAImageViewScaleTypeFitImageInside,         //显示全部，不变形, 居中
    CAImageViewScaleTypeFitViewByHorizontal,    //显示全部，不变形，横向适应图片
    CAImageViewScaleTypeFitViewByVertical       //显示全部，不变形，纵横适应图片
}
CAImageViewScaleType;

class CC_DLL CAImageView : public CAView
{
public:

    static CAImageView* create();

    static CAImageView* createWithImage(CAImage* image);

    static CAImageView* createWithFrame(const CCRect& rect);
    
    static CAImageView* createWithCenter(const CCRect& rect);
    
    CAImageView(void);

    virtual ~CAImageView(void);

    virtual bool init(void);

    virtual bool initWithImage(CAImage* image);
    
    virtual void setImage(CAImage* image);
    
    virtual CAImage* getImage(void);

    virtual void setImageAsyncWithFile(const char* fileName);
    
    using CAView::setImageRect;
    
    CC_SYNTHESIZE_PASS_BY_REF(CAImageViewScaleType, m_eImageViewScaleType, ImageViewScaleType);
    
protected:

    void asyncFinish(CAObject* var);
    
    virtual void setContentSize(const CCSize& size);
    
    virtual void updateByImageViewScaleType();
    
    virtual void updateImageRect();
    
    virtual bool initWithFrame(const CCRect& rect, const CAColor4B& color4B);
  
    virtual bool initWithCenter(const CCRect& rect, const CAColor4B& color4B);
    
    using CAView::initWithColor;
    
    float m_fLeft;
    
    float m_fRight;
    
    float m_fTop;
    
    float m_fBottom;
};

NS_CC_END

#endif
