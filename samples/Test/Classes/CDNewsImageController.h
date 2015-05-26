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

class CDNewsImageTableCell : public CATableViewCell
{
public:
    CDNewsImageTableCell();
    virtual ~CDNewsImageTableCell();
    static CDNewsImageTableCell* create(const std::string& identifier, const CADipRect& _rect = CADipRectZero);
    virtual void highlightedTableViewCell();
    virtual void selectedTableViewCell();
public:
    void initWithCell(int num);
    int cell_tag;
};

class CDNewsImageController : public CAViewController,CATableViewDelegate,CATableViewDataSource, CAScrollViewDelegate
{
public:
    CDNewsImageController(int index);
    virtual ~CDNewsImageController();
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    //CREATE_FUNC(CDNewsImageController);
public:
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);
public:
    void initImageTableView();
    void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    void onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    float getRandNum();
    void showAlert();
    void buttonCallBack(CAControl* btn,CCPoint point);
public:
    CADipSize winSize;
    CATableView* p_TableView;
    CAActivityIndicatorView* p_pLoading;
    std::vector<newsImage > m_ImageMsg;
    int urlID;
    std::vector<int > m_ImageNum;
    int p_section;
    CAView* p_alertView;
};

#endif /* defined(__Test__CDNewsImageController__) */
