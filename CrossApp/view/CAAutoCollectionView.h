//
//  CAAutoCollectionView.h
//  CrossApp
//
//  Created by Zhujian on 15-8-10.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CAAutoCollectionView__
#define __CrossApp__CAAutoCollectionView__

#include "view/CAView.h"
#include "view/CAScale9ImageView.h"
#include "controller/CABarItem.h"
#include "view/CATableView.h"
#include "view/CALabel.h"

NS_CC_BEGIN

typedef enum
{
	CACollectionViewOrientationHorizontal,
	CACollectionViewOrientationVertical
}
CACollectionViewOrientation;

typedef enum
{
	eCollectionViewCellHoriAlignLeft,
	eCollectionViewCellHoriAlignCenter,
	eCollectionViewCellHoriAlignRight,
}
CACollectionViewCellHoriAlign;

typedef enum
{
	eCollectionViewCellVertAlignTop,
	eCollectionViewCellVertAlignCenter,
	eCollectionViewCellVertAlignBottom,
}
CACollectionViewCellVertAlign;

class CACollectionViewCell;
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
	virtual CACollectionViewCell* collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item)
    {
        return NULL;
    }

    //Necessary
	virtual DSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
    {
        return DSizeZero;
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
    
	virtual CAView* collectionViewSectionViewForHeaderInSection(CAAutoCollectionView *collectionView, const DSize& viewSize, unsigned int section)
    {
        return NULL;
    }

	virtual unsigned int collectionViewHeightForHeaderInSection(CAAutoCollectionView *collectionView, unsigned int section)
    {
        return 0;
    }
    
	virtual CAView* collectionViewSectionViewForFooterInSection(CAAutoCollectionView *collectionView, const DSize& viewSize, unsigned int section)
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
		CollectionViewRow() : iIniValue(0), iMaxValue(0) {}
		unsigned int iIniValue;
		unsigned int iMaxValue;
		std::vector<DRect> rItemRects;
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

		DRect rSectionRect;
	};

public:
	CAAutoCollectionView();
	virtual ~CAAutoCollectionView();

	virtual void onEnterTransitionDidFinish();

	virtual void onExitTransitionDidStart();

	static CAAutoCollectionView* createWithFrame(const DRect& rect);

	static CAAutoCollectionView* createWithCenter(const DRect& rect);

    static CAAutoCollectionView* createWithLayout(const DRectLayout& layout);
    
	virtual bool init();

	void reloadData();

	CACollectionViewCell* dequeueReusableCellWithIdentifier(const char* reuseIdentifier);
    
    virtual void setAllowsSelection(bool var);
    
    virtual void setAllowsMultipleSelection(bool var);
    
	void setSelectRowAtIndexPath(unsigned int section, unsigned int item);

    void setUnSelectRowAtIndexPath(unsigned int section, unsigned int item);
    
    virtual void setShowsScrollIndicators(bool var);
    
	CACollectionViewCell* cellForRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item);
    
	const CAVector<CACollectionViewCell*>& displayingCollectionCell();
    
    CC_SYNTHESIZE(CAAutoCollectionViewDataSource*, m_pCollectionViewDataSource, CollectionViewDataSource);
    
	CC_SYNTHESIZE(CAAutoCollectionViewDelegate*, m_pCollectionViewDelegate, CollectionViewDelegate);
    
	CC_SYNTHESIZE_RETAIN(CAView*, m_pCollectionHeaderView, CollectionHeaderView);
    
	CC_SYNTHESIZE_RETAIN(CAView*, m_pCollectionFooterView, CollectionFooterView);
    
    CC_SYNTHESIZE(unsigned int, m_nCollectionHeaderHeight, CollectionHeaderHeight);
    
    CC_SYNTHESIZE(unsigned int, m_nCollectionFooterHeight, CollectionFooterHeight);

	CC_PROPERTY(CACollectionViewOrientation, m_pCollectionViewOrientation, CollectionViewOrientation);
	CC_SYNTHESIZE(CACollectionViewCellHoriAlign, m_pCollectionViewCellHoriAlign, CollectionViewCellHoriAlign);
	CC_SYNTHESIZE(CACollectionViewCellVertAlign, m_pCollectionViewCellVertAlign, CollectionViewCellVertAlign);

	CC_SYNTHESIZE(unsigned int, m_nHoriCellInterval, HoriCellInterval);
	CC_SYNTHESIZE(unsigned int, m_nVertCellInterval, VertCellInterval);

	CC_SYNTHESIZE(unsigned int, m_iHoriMargins, HoriMargins);
	CC_SYNTHESIZE(unsigned int, m_iVertMargins, VertMargins);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsSelection, AllowsSelection);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bAllowsMultipleSelection, AllowsMultipleSelection);
    
    CC_SYNTHESIZE_IS(bool, m_bAlwaysTopSectionHeader, AlwaysTopSectionHeader);
    
    CC_SYNTHESIZE_IS(bool, m_bAlwaysBottomSectionFooter, AlwaysBottomSectionFooter);
    
	CACollectionViewCell* getHighlightCollectionCell();
    
    virtual void switchPCMode(bool var);
    
protected:
    
    inline virtual float maxSpeed(float dt);
    
    inline virtual float decelerationRatio(float dt);
    
	void clearData();
	int calculateAllCellsLength(CollectionViewSection& cvs);
	int calculateAllCells(CollectionViewSection& cvs, int index, int dd, int dv, int dw);
	int calculateAllRects();
	bool fillSectionRowData(CollectionViewRow& r, DSize rSize);
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
	std::vector<CollectionViewSection> m_rCollectionViewSection;
    
    std::map<CAIndexPath3E, DRect> m_rUsedCollectionCellRects;
    
	std::set<CAIndexPath3E> m_pSelectedCollectionCells;

	CACollectionViewCell* m_pHighlightedCollectionCells;

	std::map<CAIndexPath3E, CACollectionViewCell*> m_mpUsedCollectionCells;

	CAVector<CACollectionViewCell*> m_vpUsedCollectionCells;
    
	std::map<std::string, CAVector<CACollectionViewCell*> > m_mpFreedCollectionCells;
};

NS_CC_END

#endif