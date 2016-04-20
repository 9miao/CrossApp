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
    
    int showImageViewNavigationBar;
    CANavigationBarItem* ImageViewNavigationBar;
    
    int showActivityIndicatorNavigationBar;
    CANavigationBarItem* ActivityIndicatorNavigationBar;
    
    int showSliderNavigationBar;
    CANavigationBarItem* SliderNavigationBar;
    
    int showLabelNavigationBar;
    CANavigationBarItem* LabelNavigationBar;
    
    int showTabBarNavigationBar;
    CANavigationBarItem* TabBarNavigationBar;
    
    int showPickerViewNavigationBar;
    CANavigationBarItem* PickerViewNavigationBar;
    
    int showViewAnimationNavigationBar;
    CANavigationBarItem* ViewAnimationNavigationBar;
    
    int showAutoCollectionVerticalNavigationBar;
    CANavigationBarItem* AutoCollectionVerticalNavigationBar;
    
    int showAutoCollectionHorizontalNavigationBar;
    CANavigationBarItem* AutoCollectionHorizontalNavigationBar;
    
public:
    
    void SliderRightBtnRightcallback(CAControl* control, DPoint point);
    
    void LabelRightBtnRightcallback(CAControl* control, DPoint point);
    
    void ImageViewRightBtnRightcallback(CAControl* control, DPoint point);
    
    void ActivityIndicatorRightBtnRightcallback(CAControl* control, DPoint point);
    
    void TabBarRightBtnRightcallback(CAControl* control, DPoint point);
    
    void PickerViewRightBtnRightcallback(CAControl* control, DPoint point);
    
    void ViewAnimationRightBtnRightcallback(CAControl* control, DPoint point);
    
    void AutoCollectionVerticalRightBtnRightcallback(CAControl* control, DPoint point);
    
    void AutoCollectionHorizontalRightBtnRightcallback(CAControl* control, DPoint point);
    
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