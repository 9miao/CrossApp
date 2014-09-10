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
        CAPullToRefreshTypeHeader = 0,
        CAPullToRefreshTypeFooter,
        CAPullToRefreshTypeLeftHeader,
        CAPullToRefreshTypeRightFooter,
        CAPullToRefreshTypeCustom
    }CAPullToRefreshType;
    
    CAPullToRefreshView(const CAPullToRefreshType& type);
    
    virtual ~CAPullToRefreshView();
    
    static CAPullToRefreshView* create(const CAPullToRefreshType& type);
    
    virtual bool init();
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sPullToRefreshText, PullToRefreshText);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sReleaseToRefreshText, ReleaseToRefreshText);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sRefreshingText, RefreshingText);
    
    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_cLabelColor, LabelColor);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pPullToImage, PullToImage);
    
    CC_SYNTHESIZE_RETAIN(CAActivityIndicatorView*, m_pLoadingView, LoadingView);
    
    CC_SYNTHESIZE_PASS_BY_REF(CALayoutLinearType, m_eLayoutLinearType, LayoutLinearType);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAPullToRefreshType, m_eType, PullToRefreshType);
    
protected:
    
    bool isLayoutFinish();
    
    void startLayout();
    
    typedef enum
    {
        CAPullToRefreshStateNormal = 1, // 普通状态
        CAPullToRefreshStatePulling, // 松开就可以进行刷新的状态
        CAPullToRefreshStateRefreshing, // 正在刷新中的状态
        CAPullToRefreshStateNone
    }
    CAPullToRefreshStateType;
    
    void setPullToRefreshStateType(const CAPullToRefreshStateType& stateType);
    
    bool isCanRefresh();
    
    bool isRefreshing();
    
    friend class CAScrollView;
    
protected:
    
    CALabel* m_pPullToRefreshLabel;
    
    CALabel* m_pReleaseToRefreshLabel;
    
    CALabel* m_pRefreshingLabel;
    
    CAImageView* m_pPullToImageView;
    
    CAPullToRefreshStateType m_eStateType;
};

NS_CC_END

#endif /* defined(__CrossApp__CAPullToReView__) */
