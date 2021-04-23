//
//  CAWaterfallViewCell.h
//  CrossApp
//
//  Created by Zhujian on 15-11-09.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CAWaterfallView__
#define __CrossApp__CAWaterfallView__

#include <iostream>
#include <set>
#include "view/CAScrollView.h"
#include "view/CACell.h"
#include "basics/CASTLContainer.h"
#include "basics/CAIndexPath.h"

NS_CC_BEGIN

class CAWaterfallView;
class CAWaterfallViewCell;

class CAWaterfallViewDelegate
{
public:
    virtual ~CAWaterfallViewDelegate(){};
    
    virtual void waterfallViewDidSelectCellAtIndexPath(CAWaterfallView *waterfallView, unsigned int itemIndex){};
    
    virtual void waterfallViewDidDeselectCellAtIndexPath(CAWaterfallView *waterfallView, unsigned int itemIndex){};
};

class CAWaterfallViewDataSource
{
public:
	virtual ~CAWaterfallViewDataSource(){};

	//Necessary
	virtual CAWaterfallViewCell* waterfallCellAtIndex(CAWaterfallView *waterfallView, const DSize& cellSize, unsigned int itemIndex)
	{
		return NULL;
	}
	//Necessary
	virtual unsigned int waterfallViewHeightForItemAtIndex(CAWaterfallView *waterfallView, unsigned int itemIndex)
	{
		return 0;
	}
	//Necessary
	virtual unsigned int numberOfItems(CAWaterfallView *waterfallView)
	{
		return 0;
	}

	virtual void waterfallViewWillDisplayCellAtIndex(CAWaterfallView* waterfallView, CAWaterfallViewCell* cell, unsigned int itemIndex) {};
};

class CC_DLL CAWaterfallView : public CAScrollView
{
public:
    
    // event listeners. If these functions are set, the corresponding function of CAWaterfallViewDataSource will fail.
    CC_LISTENING_FUNCTION(CAWaterfallViewCell*(DSize cellSize, unsigned int index), CellAtIndexPath);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int index), CellHeightAtIndexPath);
    
    CC_LISTENING_FUNCTION(unsigned int(), NumberOfItemsAtIndexPath);
    
    CC_LISTENING_FUNCTION(void(CAWaterfallViewCell* cell, unsigned int index), WillDisplayCellAtIndexPath);
    
    // event listeners. If these functions are set, the corresponding function of CAWaterfallViewDelegate will fail.
    CC_LISTENING_FUNCTION(void(unsigned int index), DidSelectCellAtIndexPath);
    
    CC_LISTENING_FUNCTION(void(unsigned int index), DidDeselectCellAtIndexPath);
    
public:
	CAWaterfallView();
    
	virtual ~CAWaterfallView();

	virtual void onEnterTransitionDidFinish();

	virtual void onExitTransitionDidStart();

	static CAWaterfallView* createWithFrame(const DRect& rect);

	static CAWaterfallView* createWithCenter(const DRect& rect);

    static CAWaterfallView* createWithLayout(const DLayout& layout);
    
	virtual bool init();

	void reloadData();

	CAWaterfallViewCell* dequeueReusableCellWithIdentifier(const char* reuseIdentifier);

	virtual void setAllowsSelection(bool var);

	virtual void setAllowsMultipleSelection(bool var);

	void setSelectRowAtIndexPath(unsigned int itemIndex);

	void setUnSelectRowAtIndexPath(unsigned int itemIndex);

	virtual void setShowsScrollIndicators(bool var);

	CAWaterfallViewCell* cellForRowAtIndexPath(unsigned int itemIndex);

	const CAVector<CAWaterfallViewCell*>& displayingWaterfallCell();

	CAWaterfallViewCell* getHighlightWaterfallCell();

	virtual void switchPCMode(bool var);

    virtual void visitEve();
    
protected:
    
	CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CAWaterfallViewDataSource*, m_pWaterfallViewDataSource, WaterfallViewDataSource);
	
	CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CAWaterfallViewDelegate*, m_pWaterfallViewDelegate, WaterfallViewDelegate);

	CC_SYNTHESIZE_RETAIN(CAView*, m_pWaterfallHeaderView, WaterfallHeaderView);

	CC_SYNTHESIZE_RETAIN(CAView*, m_pWaterfallFooterView, WaterfallFooterView);

	CC_SYNTHESIZE(unsigned int, m_nWaterfallHeaderHeight, WaterfallHeaderHeight);

	CC_SYNTHESIZE(unsigned int, m_nWaterfallFooterHeight, WaterfallFooterHeight);

	CC_SYNTHESIZE(unsigned int, m_nColumnCount, ColumnCount);

	CC_SYNTHESIZE(unsigned int, m_nItemMargin, ItemMargin);
	CC_SYNTHESIZE(unsigned int, m_nColumnMargin, ColumnMargin);
	
	CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsSelection, AllowsSelection);

	CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsMultipleSelection, AllowsMultipleSelection);

	CC_SYNTHESIZE_IS(bool, m_bAlwaysTopSectionHeader, AlwaysTopSectionHeader);

	CC_SYNTHESIZE_IS(bool, m_bAlwaysBottomSectionFooter, AlwaysBottomSectionFooter);
	
protected:

    virtual void setContentSize(const DSize& var);
    
	inline virtual float maxSpeed(float dt);

	inline virtual float decelerationRatio(float dt);

	void clearData();
	unsigned int getCurColumnIndex();
	unsigned int getMaxColumnValue();
	void reloadViewSizeData();

	virtual void update(float dt);

	void recoveryWaterfallCell();

	void loadWaterfallCell();

	void updateSectionHeaderAndFooterRects();

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
	std::map<int, DRect> m_rUsedWaterfallCellRects;

	std::map<int, CAWaterfallViewCell*> m_mpUsedWaterfallCells;

	std::set<int> m_pSelectedWaterfallCells;

	CAWaterfallViewCell* m_pHighlightedWaterfallCells;

	CAVector<CAWaterfallViewCell*> m_vpUsedWaterfallCells;

	std::map<std::string, CAVector<CAWaterfallViewCell*> > m_mpFreedWaterfallCells;

	std::vector<unsigned int> m_nColumnHeightVect;
    
    bool m_bLoadData;
};

class CC_DLL CAWaterfallViewCell : public CACell
{
public:
    
    CAWaterfallViewCell();
    
    virtual ~CAWaterfallViewCell();
    
    static CAWaterfallViewCell* create(const std::string& reuseIdentifier);

    CC_SYNTHESIZE_READONLY(unsigned int, m_nItem, Item);

protected:
    
    virtual void normalCell();
    
    virtual void highlightedCell();
    
    virtual void selectedCell();
    
    virtual void disabledCell();
    
    virtual void recoveryCell(){}
    
    friend class CAWaterfallView;
};

NS_CC_END

#endif //__CrossApp__CAWaterfallView__
