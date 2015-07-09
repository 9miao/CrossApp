//
//  CAListView.h
//  CrossApp
//
//  Created by Zhujian on 14-8-21.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CAListView__
#define __CrossApp__CAListView__

#include "view/CAView.h"
#include "view/CAScrollView.h"
#include "view/CAScale9ImageView.h"
#include "controller/CABarItem.h"
#include "view/CALabel.h"
#include "basics/CASTLContainer.h"
#include <set>

NS_CC_BEGIN

typedef enum
{
    CAListViewOrientationHorizontal,
	CAListViewOrientationVertical
}
CAListViewOrientation;

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

	virtual CAListViewCell* listViewCellAtIndex(CAListView *listView, const CCSize& cellSize, unsigned int index) = 0;
};


class CC_DLL CAListView : public CAScrollView
{
public:
	CAListView();
	virtual ~CAListView();

	virtual void onEnterTransitionDidFinish();

	virtual void onExitTransitionDidStart();

	static CAListView* createWithFrame(const CCRect& rect);

	static CAListView* createWithCenter(const CCRect& rect);

	virtual bool init();

	void reloadViewSizeData();

	void reloadData();

	CAListViewCell* dequeueReusableCellWithIdentifier(const char* reuseIdentifier);
    
    virtual void setAllowsSelection(bool var);
    
    virtual void setAllowsMultipleSelection(bool var);
    
	void setSelectAtIndex(unsigned int index);

    void setUnSelectAtIndex(unsigned int index);
    
    virtual void setShowsScrollIndicators(bool var);
    
    CAListViewCell* cellForRowAtIndex(unsigned int index);
    
    const CAVector<CAListViewCell*>& displayingListCell();
    
	CC_PROPERTY(CAListViewOrientation, m_pListViewOrientation, ListViewOrientation);

    CC_SYNTHESIZE(CAListViewDataSource*, m_pListViewDataSource, ListViewDataSource);
    
	CC_SYNTHESIZE(CAListViewDelegate*, m_pListViewDelegate, ListViewDelegate);
    
	CC_SYNTHESIZE_RETAIN(CAView*, m_pListHeaderView, ListHeaderView);

	CC_SYNTHESIZE_RETAIN(CAView*, m_pListFooterView, ListFooterView);

    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_obSeparatorColor, SeparatorColor);
    
	CC_SYNTHESIZE(unsigned int, m_nListHeaderHeight, ListHeaderHeight);

	CC_SYNTHESIZE(unsigned int, m_nListFooterHeight, ListFooterHeight);

    CC_SYNTHESIZE(unsigned int, m_nSeparatorViewHeight, SeparatorViewHeight);
    
	CC_SYNTHESIZE(bool, m_bAllowsHeadAndFootHover, AllowsHeadAndFootHover);

	CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsSelection, AllowsSelection);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsMultipleSelection, AllowsMultipleSelection);

protected:
    
    inline virtual float maxSpeed(float dt);
    
    inline virtual float maxSpeedCache(float dt);
    
    inline virtual float decelerationRatio(float dt);
    
    virtual void update(float dt);

	void recoveryCollectionCell();

	void loadCollectionCell();
    
    CAView* dequeueReusableLine();
    
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
    
	CCRect m_rHeaderRect;
    
	CCRect m_rFooterRect;

    unsigned int m_nIndexs;
    
    std::vector<CCRect> m_rIndexRects;

    std::vector<CCRect> m_rLineRects;
    
	std::map<unsigned int, CAListViewCell*> m_mpUsedListCells;

    CAVector<CAListViewCell*> m_vpUsedListCells;
    
	std::map<std::string, CAVector<CAListViewCell*> > m_mpFreedListCells;
    
    std::map<unsigned int, CAView*> m_pUsedLines;
    
    CAList<CAView*> m_pFreedLines;
    
    CAListViewCell* m_pHighlightedListCells;
    
    std::set<unsigned int> m_pSelectedListCells;
};

class CC_DLL CAListViewCell : public CAControl
{
public:
    
	CAListViewCell();
    
	virtual ~CAListViewCell();

	static CAListViewCell* create(const std::string& reuseIdentifier);

	virtual bool initWithReuseIdentifier(const std::string& reuseIdentifier);

    CC_PROPERTY(CAView*, m_pBackgroundView, BackgroundView);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sReuseIdentifier, ReuseIdentifier);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nIndex, Index);
    
    CC_SYNTHESIZE_IS(bool, m_bControlStateEffect, ControlStateEffect);
    
    CC_SYNTHESIZE_IS(bool, m_bAllowsSelected, AllowsSelected);
    
protected:
    
    virtual void normalListViewCell();
    
    virtual void highlightedListViewCell();
    
    virtual void selectedListViewCell();
    
    virtual void disabledListViewCell();
    
    virtual void recoveryListViewCell(){};
    
    void setControlState(const CAControlState& var);
    
    void setContentSize(const CCSize& var);
    
private:
    
    void resetListViewCell();
    
    using CAView::init;
    
    using CAView::initWithCenter;
    
    using CAView::initWithFrame;
    
    using CAView::initWithColor;
    
    friend class CAListView;
};

NS_CC_END

#endif