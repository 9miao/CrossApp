//
//  CAAutoCollectionView.h
//  CrossApp
//
//  Created by Zhujian on 15-8-10.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CAAutoCollectionView__
#define __CrossApp__CAAutoCollectionView__

#include "view/CACollectionView.h"

NS_CC_BEGIN

typedef enum
{
	CACollectionViewOrientationHorizontal,
	CACollectionViewOrientationVertical
}
CACollectionViewOrientation;

class CAAutoCollectionView;

class CAAutoCollectionViewDelegate
{
public:
	virtual ~CAAutoCollectionViewDelegate(){};

	virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item){};

	virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item){};
};


class CAAutoCollectionViewDataSource
{
public:
	virtual ~CAAutoCollectionViewDataSource(){};

    //Necessary
	virtual CACollectionViewCell* collectionCellAtIndex(CAAutoCollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int item)
    {
        return NULL;
    }

    //Necessary
	virtual CCSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
    {
        return CCSizeZero;
    }
    
    //Necessary
	virtual unsigned int numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section)
    {
        return 0;
    }
    
    virtual unsigned int numberOfSections(CAAutoCollectionView *collectionView)
    {
        return 1;
    }
    
	virtual CAView* collectionViewSectionViewForHeaderInSection(CAAutoCollectionView *collectionView, const CCSize& viewSize, unsigned int section)
    {
        return NULL;
    }

	virtual unsigned int collectionViewHeightForHeaderInSection(CAAutoCollectionView *collectionView, unsigned int section)
    {
        return 0;
    }
    
	virtual CAView* collectionViewSectionViewForFooterInSection(CAAutoCollectionView *collectionView, const CCSize& viewSize, unsigned int section)
    {
        return NULL;
    }
    
	virtual unsigned int collectionViewHeightForFooterInSection(CAAutoCollectionView *collectionView, unsigned int section)
    {
        return 0;
    }
    
	virtual void collectionViewWillDisplayCellAtIndex(CAAutoCollectionView* table, CACollectionViewCell* cell, unsigned int section, unsigned int item) {};
};


class CC_DLL CAAutoCollectionView : public CAScrollView
{
	struct CollectionViewRow
	{
		CollectionViewRow() : iHeight(0) {}
		unsigned int iHeight;
		std::vector<CCSize> rItemSizes;
	};

	struct CollectionViewSection
	{
		CollectionViewSection()
		: pSectionHeaderView(NULL)
		, pSectionFooterView(NULL)
		{

		}
		CAView* pSectionHeaderView;
		CAView* pSectionFooterView;

		unsigned int nSectionHeaderHeight;
		unsigned int nSectionFooterHeight;

		std::vector<CollectionViewRow> CollectionViewRows;

		CCRect rSectionRect;
	};

public:
	CAAutoCollectionView();
	virtual ~CAAutoCollectionView();

	virtual void onEnterTransitionDidFinish();

	virtual void onExitTransitionDidStart();

	static CAAutoCollectionView* createWithFrame(const CCRect& rect);

	static CAAutoCollectionView* createWithCenter(const CCRect& rect);

	virtual bool init();

	void clearData();
	void reloadData();

	CACollectionViewCell* dequeueReusableCellWithIdentifier(const char* reuseIdentifier);
    
    virtual void setAllowsSelection(bool var);
    
    virtual void setAllowsMultipleSelection(bool var);
    
	void setSelectRowAtIndexPath(unsigned int section, unsigned int item);

    void setUnSelectRowAtIndexPath(unsigned int section, unsigned int item);
    
    virtual void setShowsScrollIndicators(bool var);
    
	CACollectionViewCell* cellForRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item);
    
	const CAVector<CACollectionViewCell*>& displayingCollectionCell();
    
	CC_PROPERTY(CACollectionViewOrientation, m_pCollectionViewOrientation, CollectionViewOrientation);

    CC_SYNTHESIZE(CAAutoCollectionViewDataSource*, m_pCollectionViewDataSource, CollectionViewDataSource);
    
	CC_SYNTHESIZE(CAAutoCollectionViewDelegate*, m_pCollectionViewDelegate, CollectionViewDelegate);
    
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
	std::vector<CollectionViewSection> m_rCollectionViewSection;
    
    std::map<CAIndexPath3E, CCRect> m_rUsedCollectionCellRects;
    
	std::set<CAIndexPath3E> m_pSelectedCollectionCells;

	CACollectionViewCell* m_pHighlightedCollectionCells;

	std::map<CAIndexPath3E, CACollectionViewCell*> m_mpUsedCollectionCells;

	CAVector<CACollectionViewCell*> m_vpUsedCollectionCells;
    
	std::map<std::string, CAVector<CACollectionViewCell*> > m_mpFreedCollectionCells;
};

NS_CC_END

#endif