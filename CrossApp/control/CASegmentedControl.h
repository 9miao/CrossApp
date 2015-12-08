//
//  CASegmentedControl.h
//  CrossApp
//
//  Created by qiaoxin on 15-1-20.
//  Copyright (c) 2015年 CrossApp. All rights reserved.
//

#ifndef CrossApp_CASegmentedControl_h
#define CrossApp_CASegmentedControl_h

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
    
    static CASegmentedControl* create(unsigned int itemsCount);
    
    static CASegmentedControl* createWithFrame(const DRect& rect, unsigned int itemsCount);
    
    static CASegmentedControl* createWithCenter(const DRect& rect, unsigned int itemsCount);
  
    virtual bool init();
    
    unsigned int getNumberOfSegments();
    
    void setSelectedAtIndex(int index);
    
    int getSelectedAtIndex();
    
    void setBackgroundImage(CAImage* image);

    void setSegmentItemBackgroundImage(CAImage* image);
 
    void setTitleColor(const CAColor4B& color);
    const CAColor4B& getTitleColor();
    
    void setTitleSelectedColor(const CAColor4B& color);
    const CAColor4B& getTitleSelectedColor();
    
    void setImageColor(const CAColor4B& color);
    const CAColor4B& getImageColor();
    
    void setImageSelectedColor(const CAColor4B& color);
    const CAColor4B& getImageSelectedColor();
    
    void setTintColor(const CAColor4B& color);
    
    void setImageSizeAtIndex(DSize size, int index);
    
    void setTitleFontName(std::string titleName);
    
    void setTitleFontSize(float titleSize);
    
    void addTarget(CAObject* target, SEL_CASegmentedControl selector);

    void setTitleForSegmentAtIndex(const std::string& title, int index);
    std::string getTitleForSegmentAtIndex(int index);
    
    void setImageForSegmentAtIndex(CAImage* image, int index, CAControlState controlState);
    CAImage* getImageForSegmentAtIndex(int index);
    
    void setContentOffsetForSegmentAtIndex(DSize offset, int index);
    DSize getContentOffsetForSegmentAtIndex(int index);
    
    void setEnabledForSegmentAtIndex(bool isEnable, int index);
    bool isEnabledForSegmentAtIndex(int index);


protected:

    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    void setContentSize(const DSize & var);
    
    void callFunc(CAObject* object, int index);
    
    void printWithSegmentItemBackground();

    void initWithData();
    
    void initWithView();
    
    int pickWithPoint(const DPoint& point);
    
    void touchVithIndex(int index);
    
protected:
    
    unsigned int                    m_nItemsCount;

    int                             m_iSelectedIndex;
    
    int                             m_iTouchIndex;
    
    std::vector<std::string>        m_vTitles;
    
    CAVector<CAImage*>              m_vNormalImages;
    
    CAVector<CAImage*>              m_vSelectedImages;
    
    float                           m_fSegmentWidth;
    
    std::vector<bool>               m_vItemTouchEnableds;
    
    std::vector<DSize>              m_vItemContentOffsets;
    
    std::vector<DSize>              m_vItemImageSizes;
    
    CAScale9ImageView*              m_pBackgroundView;
    
    CAVector<CAImageView*>          m_vItemSelectedBackgrounds;
    
    CAImage*                        m_pSegmentItemBackgroundImage;
    
    CAImage*                        m_pNewSegmentItemBackgroundImage;
    
    CAVector<CAView*>               m_vSegmentItems;
    
    CAVector<CAView*>               m_vSegmentItemsTitles;
    
    CAVector<CAView*>               m_vSeparateViews;
    
    float                           m_fSeparateWidth;
    
    float                           m_fTitleFontSize;
    
    std::string                     m_sTitleFontName;
    
    CAColor4B                       m_cTextColor;
    
    CAColor4B                       m_cTextSelectedColor;
    
    CAColor4B                       m_cImageColor;
    
    CAColor4B                       m_cImageSelectedColor;
    
    CAColor4B                       m_cTintColor;
    
    CAObject*                       m_pTarget;
    
    SEL_CASegmentedControl          m_pCallFunc;

};

NS_CC_END


#endif
