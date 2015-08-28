//
//  CDShowNewsImage.h
//  Test
//
//  Created by renhongguang on 15/5/6.
//
//

#ifndef __Test__CDShowNewsImage__
#define __Test__CDShowNewsImage__

#include "RootWindow.h"

class CDShowNewsImage : public CAViewController, CAPageViewDelegate, CAScrollViewDelegate
{
public:
    CDShowNewsImage();
    
    virtual ~CDShowNewsImage();
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void initNewsImageView(newsImage _image);
public:
    virtual void pageViewDidEndTurning(CAPageView* pageView);
    virtual void scrollViewTouchUpWithoutMoved(CAScrollView* view, const CCPoint& point);
public:
    CADipSize winSize;
    newsImage m_image;
    CAPageView* p_PageView;
    CALabel* p_title;
    CALabel* p_des;
    CALabel* p_index;
    bool m_bMoved;
    bool m_isShow;
    CAView* p_bg;
};

#endif /* defined(__Test__CDShowNewsImage__) */
