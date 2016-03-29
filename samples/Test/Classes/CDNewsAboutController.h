//
//  CDNewsAboutController.h
//  Test
//
//  Created by renhongguang on 15/4/17.
//
//

#ifndef __Test__CDNewsAboutController__
#define __Test__CDNewsAboutController__

#include "RootWindow.h"

class CDNewsAboutTableCell : public CATableViewCell
{
public:
    CDNewsAboutTableCell();
    virtual ~CDNewsAboutTableCell();
    static CDNewsAboutTableCell* create(const std::string& identifier, const DRect& _rect = DRectZero);
    virtual void highlightedTableViewCell();
    virtual void selectedTableViewCell();
public:
    void initWithCell();
};

class CDNewsAboutController : public CAViewController,CATableViewDelegate,CATableViewDataSource
{
public:
    CDNewsAboutController();
    virtual ~CDNewsAboutController();
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    CREATE_FUNC(CDNewsAboutController);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section);
    virtual CAView* tableViewSectionViewForFooterInSection(CATableView* table, const DSize& viewSize, unsigned int section);
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int numberOfSections(CATableView *table);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    virtual unsigned int tableViewHeightForFooterInSection(CATableView* table, unsigned int section);
    
public:
    
    void deleteCallBack(float dt);
    CAView* _waitview;
    CAActivityIndicatorView* p_pLoading;
    CATableView* p_TableView;
    float _filesize;
    float _tempfilesize;
    string temp_Path;
    pthread_t start()
    {
        pthread_t tid;
        pthread_create(&tid, NULL, hook, this);
        return tid;
    }
private:
    static void* hook(void* args)
    
    {
        reinterpret_cast<CDNewsAboutController*>(args)->worker();
        return NULL;
    }
protected:
    void worker();
};

#endif /* defined(__Test__CDNewsAboutController__) */
