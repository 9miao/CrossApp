//
//  CDNewsViewController.h
//  Test
//
//  Created by renhongguang on 15/4/16.
//
//

#ifndef __Test__CDNewsViewController__
#define __Test__CDNewsViewController__

#include "RootWindow.h"
#include "view/CAAutoCollectionView.h"


class CDNewsTableCell : public CATableViewCell
{
public:
    CDNewsTableCell();
    virtual ~CDNewsTableCell();
    static CDNewsTableCell* create(const std::string& identifier, const CADipRect& _rect = CADipRectZero);
    virtual void highlightedTableViewCell();
    virtual void selectedTableViewCell();
public:
    void initWithCell();
    void setModel(const newsMsg& cellmodel);
private:
    CALabel* theTitle;
    CALabel* theDesc;
    CommonUrlImageView* theImage;
};

class CDNewsViewController : public CAViewController,CATableViewDelegate,CATableViewDataSource, CAScrollViewDelegate,CAPageViewDelegate, CAAutoCollectionViewDelegate, CAAutoCollectionViewDataSource
{
public:
    CDNewsViewController(int index);
    virtual ~CDNewsViewController();
    
    void viewDidLoad();
    
    void viewDidUnload();
public:
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewWillDisplayCellAtIndex(CATableView* table, CATableViewCell* cell, unsigned int section, unsigned int row);
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int numberOfSections(CATableView *table);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);

    virtual void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const CCPoint& point);
    
    virtual void pageViewDidBeginTurning(CAPageView* pageView);
    virtual void pageViewDidEndTurning(CAPageView* pageView);
public:
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int item);
    
    virtual CCSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item);
    virtual unsigned int numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section);
    virtual unsigned int numberOfSections(CAAutoCollectionView *collectionView);
    
//    virtual CAView* collectionViewSectionViewForHeaderInSection(CAAutoCollectionView *collectionView, const CCSize& viewSize, unsigned int section);
//    virtual unsigned int collectionViewHeightForHeaderInSection(CAAutoCollectionView *collectionView, unsigned int section);
//    virtual CAView* collectionViewSectionViewForFooterInSection(CAAutoCollectionView *collectionView, const CCSize& viewSize, unsigned int section);
//    virtual unsigned int collectionViewHeightForFooterInSection(CAAutoCollectionView *collectionView, unsigned int section);
//    virtual void collectionViewWillDisplayCellAtIndex(CAAutoCollectionView* table, CACollectionViewCell* cell, unsigned int section, unsigned int item);
public:
    
    void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    void onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    void initNewsTableView();
    void initNewsPageView();
    void pageControlCallBack(CrossApp::CAControl *btn, CrossApp::CCPoint point);
    void showAlert();
    void buttonCallBack(CAControl* btn,CCPoint point);
    void tempCallBack();
public:
    CADipSize winSize;
    std::vector<std::string> tempList;
    std::vector<newsMsg > m_msg;
    //std::vector<std::vector<newsMsg > > m_MSG;
    std::vector<newsPage > m_page;
    CATableView* p_TableView;
    CAPageView* p_PageView;
    CAPageControl* pageControl;
    CALabel* pageViewTitle;
    CAActivityIndicatorView* p_pLoading;
    CAView* p_alertView;
    int urlID;
    int p_section;
    CAAutoCollectionView* p_Conllection;
};

#endif /* defined(__Test__CDNewsViewController__) */
