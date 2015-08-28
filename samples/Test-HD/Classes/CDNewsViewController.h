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

class CDNewsViewController : public CAViewController, CAScrollViewDelegate, CAAutoCollectionViewDelegate, CAAutoCollectionViewDataSource
{
public:
    CDNewsViewController(int index,string newsType);
    virtual ~CDNewsViewController();
    
    void viewDidLoad();
    
    void viewDidUnload();
public:
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);

public:
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CAAutoCollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int item);
    
    virtual CCSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item);
    virtual unsigned int numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section);
    virtual unsigned int numberOfSections(CAAutoCollectionView *collectionView);
public:
    
    void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    void onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    void initNewsView();
    void initNewsPageView();
    void pageControlCallBack(CrossApp::CAControl *btn, CrossApp::CCPoint point);
    void showAlert();
    void buttonCallBack(CAControl* btn,CCPoint point);
    void tempCallBack();
public:
    CADipSize winSize;
    std::vector<std::string> tempList;
    std::vector<newsMsg > m_msg;
    std::vector<newsImage > m_ImageMsg;
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
    string m_pNewsType;
};

#endif /* defined(__Test__CDNewsViewController__) */
