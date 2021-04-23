//
//  CATableView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-28.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CrossApp_CATableView__
#define __CrossApp_CATableView__

#include <iostream>
#include <set>
#include "view/CAScrollView.h"
#include "view/CACell.h"
#include "basics/CASTLContainer.h"
#include "basics/CAIndexPath.h"

NS_CC_BEGIN


class CATableViewCell;
class CATableView;

class CC_DLL CATableViewDelegate
{
public:
    
    virtual ~CATableViewDelegate(){};
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row){};
    
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row){};
};

class CC_DLL CATableViewDataSource
{
public:
    
    virtual ~CATableViewDataSource(){};
    
    //Necessary
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row) = 0;
    
    //Necessary
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row) = 0;
    
    //Necessary
    virtual unsigned int numberOfRowsInSection(CATableView* table, unsigned int section) = 0;
    
    virtual unsigned int numberOfSections(CATableView* table)
    {
        return 1;
    }
    
    virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section)
    {
        return NULL;
    }
    
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
    {
        return 0;
    }
    
    virtual CAView* tableViewSectionViewForFooterInSection(CATableView* table, const DSize& viewSize, unsigned int section)
    {
        return NULL;
    }
    
    virtual unsigned int tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
    {
        return 0;
    }
    
    virtual void tableViewWillDisplayCellAtIndex(CATableView* table, CATableViewCell* cell, unsigned int section, unsigned int row) {};
};


class CC_DLL CATableView: public CAScrollView
{
public:
    
    // event listeners. If these functions are set, the corresponding function of CATableViewDataSource will fail.
    CC_LISTENING_FUNCTION(CATableViewCell*(DSize cellSize, unsigned int section, unsigned int row), CellAtIndexPath);

    CC_LISTENING_FUNCTION(unsigned int(unsigned int section, unsigned int row), CellHeightAtIndexPath);

    CC_LISTENING_FUNCTION(unsigned int(unsigned int section), NumberOfRowsAtIndexPath);
    
    CC_LISTENING_FUNCTION(unsigned int(), NumberOfSections);
    
    CC_LISTENING_FUNCTION(CAView*(DSize viewSize, unsigned int section), SectionViewForHeaderInSection);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int section), HeightForHeaderInSection);
    
    CC_LISTENING_FUNCTION(CAView*(DSize viewSize, unsigned int section), SectionViewForFooterInSection);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int section), HeightForFooterInSection);
    
    CC_LISTENING_FUNCTION(void(CATableViewCell* cell, unsigned int section, unsigned int row), WillDisplayCellAtIndexPath);
    
    // event listeners. If these functions are set, the corresponding function of CATableViewDelegate will fail.
    
    CC_LISTENING_FUNCTION(void(unsigned int section, unsigned int row), DidSelectCellAtIndexPath);
    
    CC_LISTENING_FUNCTION(void(unsigned int section, unsigned int row), DidDeselectCellAtIndexPath);

public:
    
    CATableView();
    
    virtual ~CATableView();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    static CATableView* createWithFrame(const DRect& rect);
    
    static CATableView* createWithCenter(const DRect& rect);
    
    static CATableView* createWithLayout(const DLayout& layout);
    
    virtual bool init();
    
    void reloadData();
    
    CATableViewCell* dequeueReusableCellWithIdentifier(const char* reuseIdentifier);
    
    virtual void setAllowsSelection(bool var);
    
    virtual void setAllowsMultipleSelection(bool var);
    
    void setSelectRowAtIndexPath(unsigned int section, unsigned int row);
    
    void setUnSelectRowAtIndexPath(unsigned int section, unsigned int row);
    
    virtual void setShowsScrollIndicators(bool var);
    
    CATableViewCell* cellForRowAtIndexPath(unsigned int section, unsigned int row);
    
    const CAVector<CATableViewCell*>& displayingTableCell();
    
    CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CATableViewDataSource*, m_pTableViewDataSource, TableViewDataSource);
    
    CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CATableViewDelegate*, m_pTableViewDelegate, TableViewDelegate);
    
    CC_SYNTHESIZE_RETAIN(CAView*, m_pTableHeaderView, TableHeaderView);
    
    CC_SYNTHESIZE_RETAIN(CAView*, m_pTableFooterView, TableFooterView);
    
    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_obSeparatorColor, SeparatorColor);
    
    CC_SYNTHESIZE(unsigned int, m_nTableHeaderHeight, TableHeaderHeight);
    
    CC_SYNTHESIZE(unsigned int, m_nTableFooterHeight, TableFooterHeight);
    
    CC_SYNTHESIZE(unsigned int, m_nSeparatorViewHeight, SeparatorViewHeight);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsSelection, AllowsSelection);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsMultipleSelection, AllowsMultipleSelection);
    
    CC_SYNTHESIZE_IS(bool, m_bAlwaysTopSectionHeader, AlwaysTopSectionHeader);
    
    CC_SYNTHESIZE_IS(bool, m_bAlwaysBottomSectionFooter, AlwaysBottomSectionFooter);
    
    virtual void switchPCMode(bool var);
    
    virtual void visitEve();
    
public:
    
    unsigned int getNumberOfSections();
    
    unsigned int getNumberOfRowsInSection(unsigned int section);
    
    float getSectionHeightInSection(unsigned int section);
    
    float getSectionHeaderHeightInSection(unsigned int section);
    
    float getSectionFooterHeightInSection(unsigned int section);
    
    float getRowHeightInSectionInRow(unsigned int section, unsigned int row);
    
protected:
    
    virtual void setContentSize(const DSize& var);
    
    inline virtual float maxSpeed(float dt);
    
    inline virtual float decelerationRatio(float dt);
    
    void clearData();
    
    void reloadViewSizeData();
    
    virtual void update(float dt);
    
    void recoveryTableCell();
    
    void loadTableCell();
    
    CAView* dequeueReusableLine();
    
    void updateSectionHeaderAndFooterRects();

public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
private:
    
    using CAScrollView::setBounceHorizontal;
    
    using CAScrollView::isBounceHorizontal;
    
    using CAScrollView::setBounceVertical;
    
    using CAScrollView::isBounceVertical;
    
    using CAScrollView::setShowsHorizontalScrollIndicator;
    
    using CAScrollView::isShowsHorizontalScrollIndicator;
    
    using CAScrollView::setShowsVerticalScrollIndicator;
    
    using CAScrollView::isShowsVerticalScrollIndicator;
    
    using CAScrollView::setViewSize;
    
    using CAScrollView::setMinimumZoomScale;
    
    using CAScrollView::getMinimumZoomScale;
    
    using CAScrollView::setMaximumZoomScale;
    
    using CAScrollView::getMaximumZoomScale;
    
    using CAScrollView::getZoomScale;
    
    using CAScrollView::isZooming;
    
    using CAScrollView::addSubview;
    
    using CAScrollView::insertSubview;
    
    using CAScrollView::removeAllSubviews;
    
    using CAScrollView::removeSubview;
    
    using CAScrollView::removeSubviewByTag;
    
    using CAScrollView::getSubviewByTag;
    
protected:
    
    unsigned int m_nSections;
    
    std::vector<unsigned int> m_nRowsInSections;
    
    std::vector<unsigned int> m_nSectionHeights;
    
    std::vector<unsigned int> m_nSectionHeaderHeights;
    
    std::vector<unsigned int> m_nSectionFooterHeights;
    
    std::vector<std::vector<unsigned int> > m_nRowHeightss;
    
    std::vector<DRect> m_rSectionRects;
    
    std::map<int, CAView*> m_pSectionHeaderViews;
    
    std::map<int, CAView*> m_pSectionFooterViews;
    
    std::vector<std::vector<DRect> > m_rTableCellRectss;
    
    std::map<CAIndexPath2E, CATableViewCell*> m_mpUsedTableCells;
    
    CAVector<CATableViewCell*> m_vpUsedTableCells;
    
    std::map<std::string, CAVector<CATableViewCell*> > m_mpFreedTableCells;
    
    std::set<CAIndexPath2E> m_pSelectedTableCells;
    
    std::vector<std::vector<DRect> > m_rLineRectss;
    
    std::map<CAIndexPath2E, CAView*> m_pUsedLines;
    
    CAList<CAView*> m_pFreedLines;
    
    CATableViewCell* m_pDraggingOutCell;
    
    bool m_bLoadData;
    
    friend class CATableViewCell;
};

class CC_DLL CATableViewCell: public CACell
{
    
public:
    
    CATableViewCell();
    
    virtual ~CATableViewCell();
    
    static CATableViewCell* create(const std::string& reuseIdentifier);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nSection, Section);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nRow, Row);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nDraggingLength, DraggingLength);
    
    void setDraggingLength(unsigned int var);
    
protected:
    
    virtual void normalCell();
    
    virtual void highlightedCell();
    
    virtual void selectedCell();
    
    virtual void disabledCell();
    
    virtual void dragInAtCell(){};
    
    virtual void dragOutAtCell(){};
    
    virtual void recoveryCell(){};
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    void dragInAnimation();
    
    void dragOutAnimation();
    
    void setDragging(bool var);
    
    bool m_bDragging;
    
    CATableView* m_pTarget;
    
    friend class CATableView;
};

NS_CC_END;

#endif /* defined(__CrossApp_CATableView__) */
