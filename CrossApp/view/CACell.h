//
//  CATableView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 16-4-20.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CrossApp_CACell__
#define __CrossApp_CACell__

#include <iostream>
#include "control/CAControl.h"

NS_CC_BEGIN
class CC_DLL CACell: public CAView
{
    
public:
    
    CACell();
    
    virtual ~CACell();
    
    static CACell* create(const std::string& reuseIdentifier);
    
    virtual bool initWithReuseIdentifier(const std::string& reuseIdentifier);
    
    CC_SYNTHESIZE_READONLY(CAView*, m_pContentView, ContentView);
    
    CC_PROPERTY(CAView*, m_pBackgroundView, BackgroundView);

    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sReuseIdentifier, ReuseIdentifier);

    CC_SYNTHESIZE_IS(bool, m_bControlStateEffect, ControlStateEffect);
    
    CC_SYNTHESIZE_IS(bool, m_bAllowsSelected, AllowsSelected);

protected:

    virtual void normalCell();
    
    virtual void highlightedCell();
    
    virtual void selectedCell();
    
    virtual void disabledCell();

    virtual void recoveryCell(){};
    
    void setControlState(const CAControlState& var);

    const CAControlState& getControlState();
    
    void setControlStateNormal();
    
    void setControlStateHighlighted();
    
    void setControlStateDisabled();
    
    void setControlStateSelected();
    
    void resetCell();
    
private:

    using CAView::init;
    
    using CAView::initWithCenter;
    
    using CAView::initWithFrame;
    
    using CAView::initWithLayout;
    
    CAControlState m_eControlState;
};

NS_CC_END;

#endif /* defined(__CrossApp_CACell__) */
