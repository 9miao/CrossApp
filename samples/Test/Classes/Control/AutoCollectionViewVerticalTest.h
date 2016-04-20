
#ifndef __Test__AutoCollectionViewVerticalTest__
#define __Test__AutoCollectionViewVerticalTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class CDEAutoCollectionViewVerticalDelegate
{
public:
    virtual ~CDEAutoCollectionViewVerticalDelegate(){};
    
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item){};
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item){};
};

class ECollectionViewVerticalCell : public CACollectionViewCell
{
public:
    
    ECollectionViewVerticalCell();
    
    virtual ~ECollectionViewVerticalCell();
    
    static ECollectionViewVerticalCell* create(const std::string& reuseIdentifier);
    
    virtual void highlightedCollectionViewCell();
    
    virtual void selectedCollectionViewCell();
    
public:
    
    void initWithCell();
    
};

class EAutoCollectionViewVertical : public CAView, CAAutoCollectionViewDataSource,CAAutoCollectionViewDelegate
{
public:
    
    EAutoCollectionViewVertical();
    
    virtual ~EAutoCollectionViewVertical();
    
    static EAutoCollectionViewVertical* createWithLayout(const DLayout& layout);
    virtual bool init();
    
    void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
public:
    
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item);
    virtual DSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item);
    virtual unsigned int numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section);
    virtual unsigned int numberOfSections(CAAutoCollectionView *collectionView);
    
    virtual CAView* collectionViewSectionViewForHeaderInSection(CAAutoCollectionView *collectionView, const DSize& viewSize, unsigned int section);
    virtual unsigned int collectionViewHeightForHeaderInSection(CAAutoCollectionView *collectionView, unsigned int section);
    virtual CAView* collectionViewSectionViewForFooterInSection(CAAutoCollectionView *collectionView, const DSize& viewSize, unsigned int section);
    virtual unsigned int collectionViewHeightForFooterInSection(CAAutoCollectionView *collectionView, unsigned int section);
    
    CC_SYNTHESIZE(CDEAutoCollectionViewVerticalDelegate*, m_pEAutoCollectionViewVerticalDelegate, EAutoCollectionViewVerticalDelegate);
    
private:
    DSize size;
    
    CAAutoCollectionView* p_AutoCollection;
    
};



class AutoCollectionViewVerticalTest: public CAViewController, public CAScrollViewDelegate, CAAutoCollectionViewDataSource,CAAutoCollectionViewDelegate, public CDEAutoCollectionViewVerticalDelegate
{
    
public:
    
    AutoCollectionViewVerticalTest();
    
    virtual ~AutoCollectionViewVerticalTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAAutoCollectionView* p_AutoCollection;
    
    CAPullToRefreshView* headerRefreshView;
    CAPullToRefreshView* footerRefreshView;
    
public:
    
    void refreshData1(float interval);
    
    void refreshData2(float interval);
    
    void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item);
    
    virtual DSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item);
    
    virtual unsigned int numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section);
    
    virtual unsigned int numberOfSections(CAAutoCollectionView *collectionView);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
