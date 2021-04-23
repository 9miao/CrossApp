//
//  CACheckbox.h
//  CrossApp
//
//  Created by Li Yuanfeng on 17-2-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//


#ifndef __CACheckbox__
#define __CACheckbox__

#include <iostream>
#include "CAControl.h"
NS_CC_BEGIN

class CAImageView;
class CALabel;

class CC_DLL CACheckbox : public CAControl
{
public:
    
    enum class Type : int
    {
        Custom = 0,
        SquareRect,
        RoundedRect
    };
    
    CACheckbox(const CACheckbox::Type& type);
    virtual ~CACheckbox(void);

    static CACheckbox* create(const CACheckbox::Type& type);
    static CACheckbox* createWithFrame(const DRect& rect, const CACheckbox::Type& type);
    static CACheckbox* createWithCenter(const DRect& rect, const CACheckbox::Type& type);
    static CACheckbox* createWithLayout(const DLayout& layout, const CACheckbox::Type& type);
    
public:
    
    virtual bool init() override;

    void setImageStateNormal(CAImage* var);
    CAImage* getImageStateNormal();
    
    void setImageStateSelected(CAImage* var);
    CAImage* getImageStateSelected();
    
    void setTitleStateNormal(const std::string& var);
    const std::string& getTitleStateNormal();
    
    void setTitleStateSelected(const std::string& var);
    const std::string& getTitleStateSelected();
    
    void setImageColorStateNormal(const CAColor4B& var);
    void setImageColorStateSelected(const CAColor4B& var);
    
    void setTitleColorStateNormal(const CAColor4B& var);
    void setTitleColorStateSelected(const CAColor4B& var);
    
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
        
    void setTarget(const std::function<void(bool on)>& function);

    bool isOn();
    void setIsOn(bool on);
    
public:
    
    virtual void onExitTransitionDidStart() override;
    virtual void onEnterTransitionDidFinish() override;
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent) override;
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent) override;
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent) override;
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent) override;
    
protected:

    CACheckbox::Type m_eType;
    
    CAImage* m_pImageNormal;
    CAImage* m_pImageSelected;
    
    CAColor4B m_cImageColorNormal;
    CAColor4B m_cImageColorSelected;
    
    std::string m_sTitleNormal;
    std::string m_sTitleSelected;
    
    CAColor4B m_cTitleColorNormal;
    CAColor4B m_cTitleColorSelected;

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

    bool m_bIsOn;
    
    std::function<void(bool)> m_function;
    
protected:
    
    void setHighlightedState(bool var);
    
    void updateCheckboxState();
    
    void updateWithPreferredSize();
    
    void setContentSize(const DSize& var) override;
};

NS_CC_END

#endif /* defined(__CACheckbox__) */
