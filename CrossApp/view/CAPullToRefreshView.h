//
//  CAPullToReView.h
//  CrossApp
//
//  Created by LiYuanfeng on 14-9-5.
//  Copyright (c) 2014年 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CAPullToReView__
#define __CrossApp__CAPullToReView__

#include <iostream>
#include "CAActivityIndicatorView.h"
#include "CALabel.h"
#include "CAImageView.h"

NS_CC_BEGIN

class CAScrollView;
class CC_DLL CAPullToRefreshView: public CAView
{
    
public:
    
    typedef enum
    {
        Header = 0,
        Footer,
        Custom
    }PullToRefreshType;
    
    CAPullToRefreshView(const PullToRefreshType& type);
    
    virtual ~CAPullToRefreshView();
    
    static CAPullToRefreshView* create(const PullToRefreshType& type);
    
    virtual bool init();
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sPullToRefreshText, PullToRefreshText);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sReleaseToRefreshText, ReleaseToRefreshText);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sRefreshingText, RefreshingText);
    
    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_cLabelColor, LabelColor);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pPullToImage, PullToImage);
    
    CC_SYNTHESIZE_RETAIN(CAActivityIndicatorView*, m_pLoadingView, LoadingView);
    
    CC_SYNTHESIZE_PASS_BY_REF(CALayoutLinearType, m_eLayoutLinearType, LayoutLinearType);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(PullToRefreshType, m_eType, Type);
    
protected:
    
    bool isLayoutFinish();
    
    void startLayout();
    
    void updateLayout();
    
    typedef enum
    {
        Normal = 1,
        Pulling,
        Refreshing,
        None
    }
    PullToRefreshStateType;
    
    void setPullToRefreshStateType(const PullToRefreshStateType& stateType);
    
    bool isCanRefresh();
    
    bool isRefreshing();
    
    virtual void setContentSize(const DSize& contentSize);
    
    friend class CAScrollView;
    
protected:
    
    CALabel* m_pPullToRefreshLabel;
    
    CALabel* m_pReleaseToRefreshLabel;
    
    CALabel* m_pRefreshingLabel;
    
    CAImageView* m_pPullToImageView;
    
    PullToRefreshStateType m_eStateType;
};

#define CAPullToRefreshTypeHeader Header
#define CAPullToRefreshTypeFooter Footer
#define CAPullToRefreshTypeCustom Custom

NS_CC_END

#endif /* defined(__CrossApp__CAPullToReView__) */
