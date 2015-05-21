//
//  CDShowNewsImage.h
//  Test
//
//  Created by renhongguang on 15/5/6.
//
//

#ifndef __Test__CDShowNewsImage__
#define __Test__CDShowNewsImage__

#include "CrossApp.h"
#include "CDData.h"

class CDShowNewsImage : public CAViewController,
public CAPageViewDelegate
{
public:
    
    CDShowNewsImage();
    
    virtual ~CDShowNewsImage();
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void initNewsImageView(newsImage _image);
    
public:
    
    virtual void pageViewDidEndTurning(CAPageView* pageView);
    
    virtual void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const CCPoint& point);
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
