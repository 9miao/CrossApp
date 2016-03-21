
#ifndef __Test__ScrollViewTest__
#define __Test__ScrollViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class ScrollViewTest: public CAViewController, public CAScrollViewDelegate
{
    
public:

	ScrollViewTest();
    
	virtual ~ScrollViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAPageView* p_PageViewVec;
    
    int showIndex;
    int showNum;
    int pageViewIndex;
    
    CAVector<CAView* > VIEWLIST;
    
    CAScrollView* p_ScrollView;
    CAImageView* p_imageView;
    std::vector<CAColor4B> colorArr;
    CACollectionView* p_Conllection;
    
public:
    
    void refreshData(float interval);
    
    //触摸滚动时调用
    virtual void scrollViewDidMoved(CAScrollView* view);
    
    //触摸滚动停止时调用
    virtual void scrollViewStopMoved(CAScrollView* view);
    
    //滚动时调用（包括惯性滚动时）
    virtual void scrollViewDidScroll(CAScrollView* view);
    
    //开始滚动时调用
    virtual void scrollViewWillBeginDragging(CAScrollView* view);
    
    //结束滚动时调用
    virtual void scrollViewDidEndDragging(CAScrollView* view);
    
    //缩放时调用
    virtual void scrollViewDidZoom(CAScrollView* view);
    
    //头部开始刷新时调用
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    //尾巴开始刷新时调用
    virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
