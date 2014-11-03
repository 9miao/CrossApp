//
//  CATableView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-28.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CrossAppx__CATableView__
#define __CrossAppx__CATableView__

#include <iostream>
#include "CAScrollView.h"
#include <set>
#include "basics/CASTLContainer.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCArray.h"
#include "control/CAControl.h"
#include "basics/CAIndexPath.h"

NS_CC_BEGIN


class CATableViewCell;
class CATableView;

class CATableViewDelegate
{
public:
    
    virtual ~CATableViewDelegate(){};

    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row){};
    
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row){};
};

class CATableViewDataSource
{
public:
    
    virtual ~CATableViewDataSource(){};
    
    //Necessary
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
    {
        return NULL;
    }
    
    //Necessary
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
    {
        return 0;
    }
    
    //Necessary
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section)
    {
        return 0;
    }
    
    virtual unsigned int numberOfSections(CATableView *table)
    {
        return 1;
    }
    
    virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
    {
        return NULL;
    }
    
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
    {
        return 0;
    }
    
    virtual CAView* tableViewSectionViewForFooterInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
    {
        return NULL;
    }
    
    virtual unsigned int tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
    {
        return 0;
    }
    
    CC_DEPRECATED_ATTRIBUTE virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, unsigned int section)
    {
        return NULL;
    }
    
    CC_DEPRECATED_ATTRIBUTE virtual CAView* tableViewSectionViewForFooterInSection(CATableView* table, unsigned int section)
    {
        return NULL;
    }
};


class CC_DLL CATableView: public CAScrollView
{
    
public:
    
    CATableView();
    
    virtual ~CATableView();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();

    static CATableView* createWithFrame(const CCRect& rect);
    
    static CATableView* createWithCenter(const CCRect& rect);
    
    virtual bool init();
    
    void reloadData();
    
    CATableViewCell* dequeueReusableCellWithIdentifier(const char* reuseIdentifier);
    
    virtual void setAllowsSelection(bool var);
    
    virtual void setAllowsMultipleSelection(bool var);
    
    void setSelectRowAtIndexPath(unsigned int section, unsigned int row);
    
    void setUnSelectRowAtIndexPath(unsigned int section, unsigned int row);
    
    CATableViewCell* cellForRowAtIndexPath(unsigned int section, unsigned int row);
    
    CC_SYNTHESIZE(CATableViewDataSource*, m_pTableViewDataSource, TableViewDataSource);
    
    CC_SYNTHESIZE(CATableViewDelegate*, m_pTableViewDelegate, TableViewDelegate);
    
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
    
public:
    
    unsigned int getNumberOfSections();
    
    unsigned int getNumberOfRowsInSection(unsigned int section);
    
    float getSectionHeightInSection(unsigned int section);
    
    float getSectionHeaderHeightInSection(unsigned int section);
    
    float getSectionFooterHeightInSection(unsigned int section);
    
    float getRowHeightInSectionInRow(unsigned int section, unsigned int row);

protected:

    inline virtual float maxSpeed(float dt);
    
    inline virtual float maxSpeedCache(float dt);
    
    inline virtual float decelerationRatio(float dt);
    
    void clearData();
    
    void reloadViewSizeData();
    
    virtual void setContentSize(const CCSize& var);
    
    virtual void update(float dt);
    
    void recoveryTableCell();
    
    void loadTableCell();
    
    CAView* dequeueReusableLine();
    
    void updateSectionHeaderAndFooterRects();

    void firstReloadData();
    
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
    
    using CAResponder::setTouchMovedListenHorizontal;
    
protected:
    
    unsigned int m_nSections;
    
    std::vector<unsigned int> m_nRowsInSections;
    
    std::vector<unsigned int> m_nSectionHeights;
    
    std::vector<unsigned int> m_nSectionHeaderHeights;
    
    std::vector<unsigned int> m_nSectionFooterHeights;
    
    std::vector<std::vector<unsigned int> > m_nRowHeightss;
    
    std::vector<CCRect> m_rSectionRects;
    
    std::map<int, CAView*> m_pSectionHeaderViews;
    
    std::map<int, CAView*> m_pSectionFooterViews;
    
    std::vector<std::vector<CCRect> > m_rTableCellRectss;

    std::map<CAIndexPath2E, CATableViewCell*> m_pUsedTableCells;
    
    std::map<std::string, CAVector<CATableViewCell*> > m_pFreedTableCells;
    
    std::set<CAIndexPath2E> m_pSelectedTableCells;
    
    CATableViewCell* m_pHighlightedTableCells;
    
    std::vector<std::vector<CCRect> > m_rLineRectss;
    
    std::map<CAIndexPath2E, CAView*> m_pUsedLines;
    
    CAList<CAView*> m_pFreedLines;
};

class CC_DLL CATableViewCell: public CAControl
{
    
public:
    
    CATableViewCell();
    
    virtual ~CATableViewCell();
    
    static CATableViewCell* create(const std::string& reuseIdentifier);
    
    virtual bool initWithReuseIdentifier(const std::string& reuseIdentifier);
    
    CC_PROPERTY(CAView*, m_pBackgroundView, BackgroundView);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sReuseIdentifier, ReuseIdentifier);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nSection, Section);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nRow, Row);
    
    CC_SYNTHESIZE_IS(bool, m_bControlStateEffect, ControlStateEffect);
    
    CC_SYNTHESIZE_IS(bool, m_bAllowsSelected, AllowsSelected);
    
protected:
    
    CC_DEPRECATED_ATTRIBUTE virtual bool initWithReuseIdentifier(const char* reuseIdentifier);
    
protected:

    virtual void normalTableViewCell();
    
    virtual void highlightedTableViewCell();
    
    virtual void selectedTableViewCell();
    
    virtual void disabledTableViewCell();

    virtual void recoveryTableViewCell(){};
    
    void setControlState(const CAControlState& var);
    
    void setContentSize(const CCSize& var);
    
private:
    
    void resetTableViewCell();
    
    using CAView::init;
    
    using CAView::initWithCenter;
    
    using CAView::initWithFrame;
    
    using CAView::initWithColor;
    
    friend class CATableView;

};

NS_CC_END;

#endif /* defined(__CrossAppx__CATableView__) */
