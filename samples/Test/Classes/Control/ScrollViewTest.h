
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
    
    virtual void scrollViewDidMoved(CAScrollView* view);
    
    virtual void scrollViewStopMoved(CAScrollView* view);
    
    virtual void scrollViewDidScroll(CAScrollView* view);
    
    virtual void scrollViewWillBeginDragging(CAScrollView* view);
    
    virtual void scrollViewDidEndDragging(CAScrollView* view);
    
    virtual void scrollViewDidZoom(CAScrollView* view);
    
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
