//
//  CAButton.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-3-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//


#ifndef __CAButton__
#define __CAButton__

#include <iostream>
#include "CAControl.h"
NS_CC_BEGIN

class CAImageView;
class CALabel;

class CC_DLL CAButton : public CAControl
{
    
public:

    enum class Event
    {
        TouchDown,
        TouchDownRepeat,
        TouchMoved,
        TouchMovedOutSide,
        TouchUpInSide,
        TouchUpOutSide,
        TouchLongPress,
        TouchCancelled
    };
    
    enum class Type : int
    {
        Custom = 0,
        SquareRect,
        RoundedRect,
    };
    
public:
    
    CAButton(const CAButton::Type& buttonType);
    virtual ~CAButton(void);

    static CAButton* create(const CAButton::Type& buttonType);
    static CAButton* createWithFrame(const DRect& rect, const CAButton::Type& buttonType);
    static CAButton* createWithCenter(const DRect& rect, const CAButton::Type& buttonType);
    static CAButton* createWithLayout(const DLayout& layout, const CAButton::Type& buttonType);
    
public:
    
    virtual bool init() override;
    
    CC_DEPRECATED_ATTRIBUTE void setBackgroundViewForState(CAControl::State state, CAView *var);
    CC_DEPRECATED_ATTRIBUTE CAView* getBackgroundViewForState(CAControl::State state);
    
    void setBackgroundImageForState(CAControl::State state, CAImage* image, bool isScale9);
    CAImage* getBackgroundImageForState(CAControl::State state);
        
    void setImageForState(CAControl::State state, CAImage* var);
    CAImage* getImageForState(CAControl::State state);
    
    void setTitleForState(CAControl::State state, const std::string& var);
    const std::string& getTitleForState(CAControl::State state);
    
    void setImageColorForState(CAControl::State state, const CAColor4B& var);

    void setTitleColorForState(CAControl::State state, const CAColor4B& var);
        
    void setImageOffset(const DSize& offset);
    void setImageSize(const DSize& size);
        
    void setTitleFontName(const std::string& var);
    void setTitleOffset(const DSize& offset);
    void setTitleLabelSize(const DSize& size);
    void setTitleFontSize(float fontSize);
    void setTitleBold(bool bold);
    void setTitleTextAlignment(const CATextAlignment& var);
    void setTitleFont(const CAFont& font);
    const CAFont& getTitleFont();
        
    void addTarget(const std::function<void()>& function, CAButton::Event event);

    virtual void setControlState(CAControl::State state) override;
    
public:
    
	void interruptTouchState();

public:
    
    virtual void onExitTransitionDidStart() override;
    virtual void onEnterTransitionDidFinish() override;
        
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent) override;
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent) override;
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent) override;
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent) override;
    
protected:

    CAButton::Type m_eButtonType;

    CAMap<CAControl::State, CAImage*> m_mImages;
    std::map<CAControl::State, CAColor4B> m_mImageColors;
    std::map<CAControl::State, std::string> m_mTitles;
    std::map<CAControl::State, CAColor4B> m_mTitleColors;
    CAMap<CAControl::State, CAView*> m_mBackgroundViews;
    
    CAImageView* m_pImageView;
    CALabel* m_pLabel;
    
    DSize m_pImageSize;
    bool m_bDefineImageSize;
    DSize m_pImageOffset;
    bool m_bDefineImageOffset;
        
    CAFont m_obTitleFont;
    DSize m_pTitleLabelSize;
    bool m_bDefineTitleLabelSize;
    DSize m_pTitleOffset;
    bool m_bDefineTitleOffset;
        
    std::map<CAButton::Event, std::function<void()>> m_mFunctions;
    
    bool m_bTouchClick;
    
protected:
    
    void updateWithPreferredSize();
    
    void setContentSize(const DSize& var) override;

    void setBackgroundViewSquareRect();
    void setBackgroundViewRoundedRect();
    
    void callBackFunction(CAButton::Event event);
};

NS_CC_END

#endif /* defined(__CAButton__) */
