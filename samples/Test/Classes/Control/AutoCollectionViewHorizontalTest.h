
#ifndef __Test__AutoCollectionViewHorizontalTest__
#define __Test__AutoCollectionViewHorizontalTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class CDEAutoCollectionViewHorizontalDelegate
{
public:
    virtual ~CDEAutoCollectionViewHorizontalDelegate(){};
    
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item){};
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item){};
};

class ECollectionViewHorizontalCell : public CACollectionViewCell
{
public:
    
    ECollectionViewHorizontalCell();
    
    virtual ~ECollectionViewHorizontalCell();
    
    static ECollectionViewHorizontalCell* create(const std::string& reuseIdentifier);
    
    virtual void highlightedCollectionViewCell();
    
    virtual void selectedCollectionViewCell();
    
public:
    
    void initWithCell();
    
};

class EAutoCollectionViewHorizontal : public CAView, CAAutoCollectionViewDataSource,CAAutoCollectionViewDelegate
{
public:
    
    EAutoCollectionViewHorizontal();
    
    virtual ~EAutoCollectionViewHorizontal();
    
    static EAutoCollectionViewHorizontal* createWithLayout(const DLayout& layout);
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

    CC_SYNTHESIZE(CDEAutoCollectionViewHorizontalDelegate*, m_pEAutoCollectionViewHorizontalDelegate, EAutoCollectionViewHorizontalDelegate);
    
private:
    DSize size;
    
    CAAutoCollectionView* p_AutoCollection;
    
};




class AutoCollectionViewHorizontalTest: public CAViewController, public CAScrollViewDelegate, CAAutoCollectionViewDataSource,CAAutoCollectionViewDelegate, public CDEAutoCollectionViewHorizontalDelegate
{
    
public:
    
    AutoCollectionViewHorizontalTest();
    
    virtual ~AutoCollectionViewHorizontalTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAAutoCollectionView* p_AutoCollection;
    
public:
    
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item);

    virtual DSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item);

    virtual unsigned int numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section);

    virtual unsigned int numberOfSections(CAAutoCollectionView *collectionView);

};


#endif /* defined(__HelloCpp__ViewController__) */
