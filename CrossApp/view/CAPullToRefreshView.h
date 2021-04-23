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
    
    enum class Type : int
    {
        Header = 0,
        Footer,
        Custom
    };
    
    CAPullToRefreshView(CAPullToRefreshView::Type type);
    
    virtual ~CAPullToRefreshView();
    
    static CAPullToRefreshView* create(CAPullToRefreshView::Type type);
    
    virtual bool init();
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sPullToRefreshText, PullToRefreshText);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sReleaseToRefreshText, ReleaseToRefreshText);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sRefreshingText, RefreshingText);
    
    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_cLabelColor, LabelColor);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pPullToImage, PullToImage);
    
    CC_SYNTHESIZE_RETAIN(CAActivityIndicatorView*, m_pLoadingView, LoadingView);
        
    CC_SYNTHESIZE_READONLY(CAPullToRefreshView::Type, m_eType, Type);
    
protected:
    
    enum class State : int
    {
        Normal = 1,
        Pulling,
        Refreshing,
        None
    };
        
    bool isLayoutFinish();
    
    void startLayout();
    
    void updateLayout();
    
    void setState(CAPullToRefreshView::State stateType);
    
    bool isCanRefresh();
    
    bool isRefreshing();
    
    virtual void setContentSize(const DSize& contentSize);
    
    friend class CAScrollView;
    
protected:
    
    CALabel* m_pPullToRefreshLabel;
    
    CALabel* m_pReleaseToRefreshLabel;
    
    CALabel* m_pRefreshingLabel;
    
    CAImageView* m_pPullToImageView;
    
    CAPullToRefreshView::State m_eState;
};

NS_CC_END

#endif /* defined(__CrossApp__CAPullToReView__) */
