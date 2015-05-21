//
//  CDNewsViewController.h
//  Test
//
//  Created by renhongguang on 15/4/16.
//
//

#ifndef __Test__CDNewsViewController__
#define __Test__CDNewsViewController__

#include "CrossApp.h"
#include "CommonHttpManager.h"
#include "CDData.h"

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
};

class CDNewsViewController : public CAViewController,
public CATableViewDelegate,
public CATableViewDataSource,
public CAScrollViewDelegate,
public CAPageViewDelegate
{
public:
    
    CDNewsViewController(int index);
    
    virtual ~CDNewsViewController();
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
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
    
    void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void initNewsTableView();
    
    void initNewsPageView();
    
    void pageControlCallBack(CrossApp::CAControl *btn, CrossApp::CCPoint point);
    
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
    int urlID;
    int p_section;
};

#endif /* defined(__Test__CDNewsViewController__) */
