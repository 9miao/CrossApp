//
//  CGSprite.h
//  CrossApp
//
//  Created by Li Yuanfeng on 16-7-19.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CGSPRITE_H__
#define __CGSPRITE_H__

#include "view/CAView.h"
#include "ccTypes.h"
#include "ccMacros.h"
#include "CCGL.h"
#include "dispatcher/CAProtocols.h"
#include "images/CAImageCache.h"

NS_CC_BEGIN

class Action;
class ActionManager;
class CGSpriteFrame;
class CGAnimation;

class CC_DLL CGSprite : public CAView
{
public:

    static CGSprite* create();

    static CGSprite* create(const std::string& pszFileName);

    static CGSprite* create(const std::string& pszFileName, const DRect& rect);

    static CGSprite* createWithImage(CAImage *image);

    static CGSprite* createWithImage(CAImage *image, const DRect& rect);

    static CGSprite* createWithSpriteFrame(CGSpriteFrame *pSpriteFrame);

    static CGSprite* createWithSpriteFrameName(const std::string& pszSpriteFrameName);
    
    CGSprite(void);

    virtual ~CGSprite(void);

    virtual bool init() override;

    virtual bool initWithImage(CAImage *image);

    virtual bool initWithImage(CAImage *image, const DRect& rect);

    virtual bool initWithImage(CAImage *image, const DRect& rect, bool rotated);

    virtual bool initWithSpriteFrame(CGSpriteFrame *pSpriteFrame);

    virtual bool initWithSpriteFrameName(const std::string& pszSpriteFrameName);

    virtual bool initWithFile(const std::string& pszFilename);

    virtual bool initWithFile(const std::string& pszFilename, const DRect& rect);

    virtual void setImage(CAImage *image) override;
    
    virtual CAImage* getImage() override;

    virtual void setImageRect(const DRect& rect, bool rotated, const DSize& untrimmedSize);

    virtual void setSpriteFrame(CGSpriteFrame *pNewFrame);

    virtual bool isFrameDisplayed(CGSpriteFrame *pFrame);

    virtual CGSpriteFrame* getSpriteFrame(void);

    virtual void setDisplayFrameWithAnimationName(const std::string& animationName, int frameIndex);

    inline const DPoint& getOffsetPosition(void) { return m_obOffsetPosition; }
    
    virtual void setRotation3D(const DPoint3D& rotation);
    
    virtual DPoint3D getRotation3D() const;

    virtual void setPosition(const DPoint &point);
    
    virtual const DPoint& getPosition();
    
    virtual void setNormalizedPosition(const DPoint &position);
    
    virtual const DPoint& getNormalizedPosition() const;
    
    virtual void  setPositionX(float x);
    
    virtual float getPositionX(void) const;
    
    virtual void  setPositionY(float y);
    
    virtual float getPositionY(void) const;
    
    virtual void setPositionZ(float positionZ);
    
    virtual float getPositionZ() const;
    
    virtual void setPosition3D(const DPoint3D& position);
    
    virtual DPoint3D getPosition3D() const;
    
    virtual void setRotationQuat(const Quaternion& quat);
    
    Quaternion getRotationQuat() const;
    
    virtual const DSize& getContentSize();
    
    virtual void setContentSize(const DSize& contentSize) override;
    
    virtual void setScaleZ(float scaleZ);
    
    virtual float getScaleZ() const;
    
    virtual Action* runAction(Action* action);
    
    void stopAllActions();
    
    void stopAction(Action* action);
    
    void stopActionByTag(int tag);
    
    void stopAllActionsByTag(int tag);
    
    void stopActionsByFlags(unsigned int flags);
    
    Action* getActionByTag(int tag);
    
    size_t getNumberOfRunningActions() const;

    virtual DRect boundingBox();
    
    virtual const Mat4& getViewToSuperviewTransform() const override;
    
    virtual void onEnter() override;
    
    virtual void onExit() override;
    
protected:

    void updateRotation3D();
    
    DPoint  m_obNormalizedPosition;
    bool    m_bUsingNormalizedPosition;
    // Offset Position (used by Zwoptex)
    DPoint  m_obOffsetPosition;
    DPoint  m_obUnflippedOffsetPositionFromCenter;

    bool    m_obAutoContentSize;
};


// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __SPITE_NODE_CGSprite_H__
