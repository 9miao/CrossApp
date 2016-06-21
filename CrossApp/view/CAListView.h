//
//  CAListView.h
//  CrossApp
//
//  Created by Zhujian on 14-8-21.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CAListView__
#define __CrossApp__CAListView__

#include <iostream>
#include <set>
#include "view/CAScrollView.h"
#include "view/CACell.h"
#include "basics/CASTLContainer.h"
#include "basics/CAIndexPath.h"

NS_CC_BEGIN

class CAListViewCell;
class CAListView;

class CAListViewDelegate
{
public:
	virtual ~CAListViewDelegate(){};

	virtual void listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index) {};

	virtual void listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index) {};
};


class CAListViewDataSource
{
public:
	virtual ~CAListViewDataSource(){};

	virtual unsigned int numberOfIndex(CAListView *listView) = 0;

	virtual unsigned int listViewHeightForIndex(CAListView *listView, unsigned int index) = 0;

	virtual CAListViewCell* listViewCellAtIndex(CAListView *listView, const DSize& cellSize, unsigned int index) = 0;
    
    virtual void listViewWillDisplayCellAtIndex(CAListView* table, CAListViewCell* cell, unsigned int index) {};
};


class CC_DLL CAListView : public CAScrollView
{
public:
    
    typedef enum
    {
        Horizontal = 0,
        Vertical
    }
    Orientation;
    
public:
    
	CAListView();
    
	virtual ~CAListView();

	virtual void onEnterTransitionDidFinish();

	virtual void onExitTransitionDidStart();

	static CAListView* createWithFrame(const DRect& rect);

	static CAListView* createWithCenter(const DRect& rect);

    static CAListView* createWithLayout(const DLayout& layout);
    
	virtual bool init();

	void reloadData();

	CAListViewCell* dequeueReusableCellWithIdentifier(const char* reuseIdentifier);
    
    virtual void setAllowsSelection(bool var);
    
    virtual void setAllowsMultipleSelection(bool var);
    
	void setSelectAtIndex(unsigned int index);

    void setUnSelectAtIndex(unsigned int index);
    
    virtual void setShowsScrollIndicators(bool var);
    
    CAListViewCell* cellForRowAtIndex(unsigned int index);
    
    const CAVector<CAListViewCell*>& displayingListCell();
    
	CC_PROPERTY_PASS_BY_REF(Orientation, m_eOrientation, Orientation);

    CC_SYNTHESIZE(CAListViewDataSource*, m_pListViewDataSource, ListViewDataSource);
    
	CC_SYNTHESIZE(CAListViewDelegate*, m_pListViewDelegate, ListViewDelegate);
    
	CC_SYNTHESIZE_RETAIN(CAView*, m_pListHeaderView, ListHeaderView);

	CC_SYNTHESIZE_RETAIN(CAView*, m_pListFooterView, ListFooterView);

    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_obSeparatorColor, SeparatorColor);
    
	CC_SYNTHESIZE(unsigned int, m_nListHeaderHeight, ListHeaderHeight);

	CC_SYNTHESIZE(unsigned int, m_nListFooterHeight, ListFooterHeight);

    CC_SYNTHESIZE(unsigned int, m_nSeparatorViewHeight, SeparatorViewHeight);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsSelection, AllowsSelection);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsMultipleSelection, AllowsMultipleSelection);

    virtual void switchPCMode(bool var);
    
protected:
    
    virtual void setContentSize(const DSize& var);
    
    inline virtual float maxSpeed(float dt);
    
    inline virtual float decelerationRatio(float dt);
    
    virtual void update(float dt);

	void recoveryCell();

	void loadCell();
    
    CAView* dequeueReusableLine();
    
    void firstReloadData();
    
    void reloadViewSizeData();
    
    void clearData();
    
public:

	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);

	virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);

	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);

	virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);

    virtual void mouseMoved(CATouch* pTouch, CAEvent* pEvent);
    
    virtual void mouseMovedOutSide(CATouch* pTouch, CAEvent* pEvent);
    
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
    
private:
    
	DRect m_rHeaderRect;
    
	DRect m_rFooterRect;

    unsigned int m_nIndexs;
    
    std::vector<DRect> m_rIndexRects;

    std::vector<DRect> m_rLineRects;
    
	std::map<unsigned int, CAListViewCell*> m_mpUsedListCells;

    CAVector<CAListViewCell*> m_vpUsedListCells;
    
	std::map<std::string, CAVector<CAListViewCell*> > m_mpFreedListCells;
    
    std::map<unsigned int, CAView*> m_pUsedLines;
    
    CAList<CAView*> m_pFreedLines;
    
    CAListViewCell* m_pHighlightedListCells;
    
    std::set<unsigned int> m_pSelectedListCells;
};

class CC_DLL CAListViewCell : public CACell
{
public:
    
	CAListViewCell();
    
	virtual ~CAListViewCell();

	static CAListViewCell* create(const std::string& reuseIdentifier);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nIndex, Index);

protected:
    
    virtual void normalCell();
    
    virtual void highlightedCell();
    
    virtual void selectedCell();
    
    virtual void disabledCell();
    
    virtual void recoveryCell();
    
    CC_DEPRECATED_ATTRIBUTE virtual void normalListViewCell();
    
    CC_DEPRECATED_ATTRIBUTE virtual void highlightedListViewCell();
    
    CC_DEPRECATED_ATTRIBUTE virtual void selectedListViewCell();
    
    CC_DEPRECATED_ATTRIBUTE virtual void disabledListViewCell();
    
    CC_DEPRECATED_ATTRIBUTE virtual void recoveryListViewCell(){};
    
    friend class CAListView;
};

#define setListViewOrientation(_var_) setOrientation(_var_)
#define getListViewOrientation(_var_) getOrientation(_var_)
#define CAListViewOrientationHorizontal CAListView::Horizontal
#define CAListViewOrientationVertical CAListView::Vertical


NS_CC_END

#endif