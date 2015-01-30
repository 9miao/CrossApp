//
//  CASegmentedControl.h
//  CrossApp
//
//  Created by qiaoxin on 15-1-20.
//  Copyright (c) 2015年 CrossApp. All rights reserved.
//

#ifndef CrossAppx_CASegmentedControl_h
#define CrossAppx_CASegmentedControl_h

#include "CAControl.h"
#include "basics/CASTLContainer.h"
#include <set>
NS_CC_BEGIN

class CAButton;
class CAImage;
class CAImageView;
class CAScale9ImageView;
class CALabel;


class CASegmentedControl;

typedef void (CAObject::*SEL_CASegmentedControl)(CASegmentedControl*, int index);
#define CASegmentedControl_selector(_SELECTOR) (SEL_CASegmentedControl)(&_SELECTOR)

class CC_DLL CASegmentedControl : public CAControl
{
public:
    
    CASegmentedControl(unsigned int itemsCount);
    
    virtual ~CASegmentedControl();
    
    virtual void onExitTransitionDidStart();
    
    virtual void onEnterTransitionDidFinish();
    
    static CASegmentedControl* createWithFrame(const CCRect& rect, unsigned int itemsCount);
    
    static CASegmentedControl* createWithCenter(const CCRect& rect, unsigned int itemsCount);
  
    void setBackgroundImage(CAImage* image);
 
    void addTarget(CAObject* target, SEL_CASegmentedControl selector);
    
    void setSelectedAtIndex(int index);
    int  getSelectedAtIndex();
    
    int  getNumberOfSegments();
    
    void setTitleColor(const CAColor4B& color);
    const CAColor4B& getTitleColor();
    
    void insertSegmentWithTitleAtIndex(const std::string title, int index);
    void insertSegmentWithImageAtIndex(CAImage* image, int index);
    
    void removeSegmentAtIndex(int index);
    void removeAllSegments();
    
    void setTitleForSegmentAtIndex(const std::string title, int index);
    std::string getTitleForSegmentAtIndex(int index);
    
    void setImageForSegmentAtIndex(CAImage* image, int index);
    CAImage* getImageForSegmentAtIndex(int index);
    
    void setWidthForSegmentAtIndex(float width, int index);
    float getWidthForSegmentAtIndex(int index);
    
    void setContentOffsetForSegmentAtIndex(CCSize offset, int index);
    CCSize getContentOffsetForSegmentAtIndex(int index);
    
    void setEnabledForSegmentAtIndex(bool isEnable, int index);
    bool isEnabledForSegmentAtIndex(int index);
    
    CC_PROPERTY(CAView*, m_pBackgroundView, BackgroundView);

protected:
    virtual bool initWithFrame(const CCRect& rect);
    
    virtual bool initWithCenter(const CCRect& rect);
    
    virtual CAView* createDefaultSegment(int index);
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    void setContentSize(const CCSize & var);
    
    void createSeparate();
    
    void cleanAllSeparate();

    void removeAllSeparateImage();
    
    void refreshAllSegmentItemBound();
    
    void refreshAllSegmentItemPosition();
    
    void refreshSegmentItemByPoint(CCPoint point, CAControlState controlState);
    
    void refreshSegmentItemByIndex(int index, CAControlState controlState);
    
    void refreshAllSegmentItemByState(CAControlState controlState);
    
    int  getSegmentItemIndexByPoint(CCPoint point);
    
    CAObject* getObjectByIndex(int index, CAControlState controlState);
    
    void setSegmentStateWithColor( CAControlState controlState, int index);
    
    void callFunc(CAObject* object, int index);
    
protected:
    unsigned int                     m_nItemsCount;

    CAVector<CALabel*>               m_vTitles;
    
    CAVector<CAImage*>               m_vNormalImages;
    
    std::vector<bool>                m_vIsEnabled;
    
    std::vector<CCSize>              m_vContentOffset;
    
    std::vector<float>               m_vSegmentWidth;
    
    CAVector<CAView*>                m_vSegments;
    
    CAVector<CAView*>                m_vSeparateView;
    
    float                            m_fSeparateWidth;
    
    int                              m_iSelectedIndex;
    
    int                              m_iTouchIndex;
    
    CAColor4B                        m_cTextColor;
    
    CAObject*                        m_pTarget;
    
    SEL_CASegmentedControl           m_pCallFunc;
    
    
};

NS_CC_END


#endif
