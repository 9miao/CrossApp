//
//  CACollectionView.h
//  CrossApp
//
//  Created by Zhujian on 14-6-27.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CACollectionView__
#define __CrossApp__CACollectionView__

#include "view/CAView.h"
#include "view/CAScale9ImageView.h"
#include "controller/CABarItem.h"
#include "view/CATableView.h"
#include "view/CALabel.h"

NS_CC_BEGIN

class CACollectionViewCell;
class CACollectionView;

class CACollectionViewDelegate
{
public:
	virtual ~CACollectionViewDelegate(){};

	virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item){};

	virtual void collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item){};
};


class CACollectionViewDataSource
{
public:
	virtual ~CACollectionViewDataSource(){};

    //Necessary
	virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
    {
        return NULL;
    }

    //Necessary
    virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
    {
        return 0;
    }
    
	//Necessary
    virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
    {
        return 0;
    }
    
    //Necessary
	virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
    {
        return 0;
    }
    
    virtual unsigned int numberOfSections(CACollectionView *collectionView)
    {
        return 1;
    }
    
	virtual CAView* collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section)
    {
        return NULL;
    }

    virtual unsigned int collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section)
    {
        return 0;
    }
    
	virtual CAView* collectionViewSectionViewForFooterInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section)
    {
        return NULL;
    }
    
	virtual unsigned int collectionViewHeightForFooterInSection(CACollectionView *collectionView, unsigned int section)
    {
        return 0;
    }
    
    virtual void collectionViewWillDisplayCellAtIndex(CACollectionView* table, CACollectionViewCell* cell, unsigned int section, unsigned int row, unsigned int item) {};
};


class CC_DLL CACollectionView : public CAScrollView
{
public:
	CACollectionView();
	virtual ~CACollectionView();

	virtual void onEnterTransitionDidFinish();

	virtual void onExitTransitionDidStart();

	static CACollectionView* createWithFrame(const CCRect& rect);

	static CACollectionView* createWithCenter(const CCRect& rect);

	virtual bool init();

	void clearData();
	void reloadData();

	CACollectionViewCell* dequeueReusableCellWithIdentifier(const char* reuseIdentifier);
    
    virtual void setAllowsSelection(bool var);
    
    virtual void setAllowsMultipleSelection(bool var);
    
	void setSelectRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item);

    void setUnSelectRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item);
    
    virtual void setShowsScrollIndicators(bool var);
    
    CACollectionViewCell* cellForRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item);
    
    const CAVector<CACollectionViewCell*>& displayingCollectionCell();
    
    CC_SYNTHESIZE(CACollectionViewDataSource*, m_pCollectionViewDataSource, CollectionViewDataSource);
    
	CC_SYNTHESIZE(CACollectionViewDelegate*, m_pCollectionViewDelegate, CollectionViewDelegate);
    
	CC_SYNTHESIZE_RETAIN(CAView*, m_pCollectionHeaderView, CollectionHeaderView);
    
	CC_SYNTHESIZE_RETAIN(CAView*, m_pCollectionFooterView, CollectionFooterView);
    
    CC_SYNTHESIZE(unsigned int, m_nCollectionHeaderHeight, CollectionHeaderHeight);
    
    CC_SYNTHESIZE(unsigned int, m_nCollectionFooterHeight, CollectionFooterHeight);

	CC_SYNTHESIZE(unsigned int, m_nHoriInterval, HoriInterval);

	CC_SYNTHESIZE(unsigned int, m_nVertInterval, VertInterval);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsSelection, AllowsSelection);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsMultipleSelection, AllowsMultipleSelection);
    
    CC_SYNTHESIZE_IS(bool, m_bAlwaysTopSectionHeader, AlwaysTopSectionHeader);
    
    CC_SYNTHESIZE_IS(bool, m_bAlwaysBottomSectionFooter, AlwaysBottomSectionFooter);
    
    CACollectionViewCell* getHighlightCollectionCell();
    
    virtual void switchPCMode(bool var);
    
protected:
    
    inline virtual float maxSpeed(float dt);
    
    inline virtual float maxSpeedCache(float dt);
    
    inline virtual float decelerationRatio(float dt);
    
    void reloadViewSizeData();
    
    virtual void update(float dt);

	void recoveryCollectionCell();

	void loadCollectionCell();
    
    void updateSectionHeaderAndFooterRects();
    
    void firstReloadData();
    
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
    
    unsigned int m_nSections;
    
    std::vector<unsigned int> m_nRowsInSections;
    
    std::vector<unsigned int> m_nSectionHeights;
    
    std::vector<unsigned int> m_nSectionHeaderHeights;
    
    std::vector<unsigned int> m_nSectionFooterHeights;
    
    std::vector<std::vector<unsigned int> > m_nRowHeightss;
    
    std::vector<CCRect> m_rSectionRects;
    
    std::map<CAIndexPath3E, CCRect> m_rUsedCollectionCellRects;
    
    std::map<int, CAView*> m_pSectionHeaderViews;
    
    std::map<int, CAView*> m_pSectionFooterViews;
    
	std::set<CAIndexPath3E> m_pSelectedCollectionCells;

	CACollectionViewCell* m_pHighlightedCollectionCells;

	std::map<CAIndexPath3E, CACollectionViewCell*> m_mpUsedCollectionCells;

    CAVector<CACollectionViewCell*> m_vpUsedCollectionCells;
    
	std::map<std::string, CAVector<CACollectionViewCell*> > m_mpFreedCollectionCells;
};

class CC_DLL CACollectionViewCell : public CAControl
{
	friend class CAAutoCollectionView;
public:
    
	CACollectionViewCell();
    
	virtual ~CACollectionViewCell();

	static CACollectionViewCell* create(const std::string& reuseIdentifier);

	virtual bool initWithReuseIdentifier(const std::string& reuseIdentifier);

    CC_SYNTHESIZE_READONLY(CAView*, m_pContentView, ContentView);
    
    CC_PROPERTY(CAView*, m_pBackgroundView, BackgroundView);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sReuseIdentifier, ReuseIdentifier);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nSection, Section);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nRow, Row);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nItem, Item);
    
    CC_SYNTHESIZE_IS(bool, m_bControlStateEffect, ControlStateEffect);
    
    CC_SYNTHESIZE_IS(bool, m_bAllowsSelected, AllowsSelected);
    
protected:
    
	virtual void normalCollectionViewCell();
    
	virtual void highlightedCollectionViewCell();
    
	virtual void selectedCollectionViewCell();
    
	virtual void disabledCollectionViewCell();

    virtual void recoveryCollectionViewCell(){};
    
    void setControlState(const CAControlState& var);
    
    void setContentSize(const CCSize& var);
    
private:
    
    void resetCollectionViewCell();
    
    using CAView::init;
    
    using CAView::initWithCenter;
    
    using CAView::initWithFrame;
    
    using CAView::initWithColor;
    
    friend class CACollectionView;
};

NS_CC_END

#endif