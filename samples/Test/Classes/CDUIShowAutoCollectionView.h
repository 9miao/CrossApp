#ifndef __Test__CDUIShowAutoCollectionView__
#define __Test__CDUIShowAutoCollectionView__

#include "RootWindow.h"

class CDUIShowAutoCollectionView : public CAViewController, CAAutoCollectionViewDataSource, CAAutoCollectionViewDelegate, CAScrollViewDelegate
{
    
public:
    
    CDUIShowAutoCollectionView();
    
    virtual ~CDUIShowAutoCollectionView();
    //CREATE_FUNC(CDUIShowCollectionView);
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAAutoCollectionView* p_AutoCollection;
    
public:
    
    void refreshData(float interval);
    
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item);
    
    virtual DSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item);
    
    virtual unsigned int numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section);
    
    virtual unsigned int numberOfSections(CAAutoCollectionView *collectionView);
    
private:
    
    std::vector<CAColor4B> colorArr;
    
    std::vector<std::string> m_vTitle;
};
#endif /* defined(__Test__CDUIShowCollectionView__) */