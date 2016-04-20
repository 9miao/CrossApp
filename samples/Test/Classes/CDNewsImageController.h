//
//  CDNewsImageController.h
//  Test
//
//  Created by renhongguang on 15/4/17.
//
//

#ifndef __Test__CDNewsImageController__
#define __Test__CDNewsImageController__

#include "RootWindow.h"

class CDNewsImagecollectionCell : public CACollectionViewCell
{
public:
    CDNewsImagecollectionCell();
    virtual ~CDNewsImagecollectionCell();
    static CDNewsImagecollectionCell* create(const std::string& identifier);
    virtual void highlightedTableViewCell();
    virtual void selectedTableViewCell();
    
    CC_SYNTHESIZE_READONLY(CommonUrlImageView*, m_pImage1, Image1);
    CC_SYNTHESIZE_READONLY(CommonUrlImageView*, m_pImage2, Image2);
    CC_SYNTHESIZE_READONLY(CommonUrlImageView*, m_pImage3, Image3);
    CC_SYNTHESIZE_READONLY(CommonUrlImageView*, m_pImage4, Image4);
    
public:
    void initWithCell(int num);
    int cell_tag;
};

class CDNewsImageController : public CAViewController, CAScrollViewDelegate, CAAutoCollectionViewDataSource, CAAutoCollectionViewDelegate

{
public:
    CDNewsImageController(int index);
    virtual ~CDNewsImageController();
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    //CREATE_FUNC(CDNewsImageController);
public:
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item);
    virtual DSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item);
    virtual unsigned int numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section);
    virtual unsigned int numberOfSections(CAAutoCollectionView *collectionView);    
    
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);
    virtual void scrollViewStopMoved(CAScrollView* view);
public:
    void initImageTableView();
    void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    void onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    float getRandNum();
    void showAlert();
    void buttonCallBack(CAControl* btn,DPoint point);
public:
    string getSign(std::map<std::string,std::string> key_value);
    DSize winSize;
    CAAutoCollectionView* p_AutoCollectionView;
    CAActivityIndicatorView* p_pLoading;
    std::vector<newsImage > m_ImageMsg;
    int urlID;
    std::vector<int > m_ImageNum;
    int p_section;
    CAView* p_alertView;
};

#endif /* defined(__Test__CDNewsImageController__) */
