
#ifndef __Test__CollectionViewTest__
#define __Test__CollectionViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class CollectionViewTest: public CAViewController, CACollectionViewDelegate, CACollectionViewDataSource, public CAScrollViewDelegate
{
    
public:

	CollectionViewTest();
    
	virtual ~CollectionViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAPullToRefreshView* headerRefreshView;
    CAPullToRefreshView* footerRefreshView;
    
    CAPageView* p_PageViewVec;
    
    int showIndex;
    int showNum;
    int pageViewIndex;
    
    CAVector<CAView* > VIEWLIST;
    
public:
    
    void refreshData(float interval);
    
    void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
    //选中item时调用
    virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    //取消item是调用
    virtual void collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    //获取指定cell
    virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
    //section的个数
    virtual unsigned int numberOfSections(CACollectionView *collectionView);
    
    //section中的cell个数
    virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section);
    
    //每个cell中Item的个数
    virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
    
    //cell的高度
    virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);
    
public:
    
    //用于获得屏幕的size
    DSize size;
    
    //CACollectionView
    CACollectionView* p_Conllection;
    
    //颜色容器
    std::vector<CAColor4B> colorArr;
    
};


#endif /* defined(__HelloCpp__ViewController__) */
