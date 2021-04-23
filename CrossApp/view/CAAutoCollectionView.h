//
//  CAAutoCollectionView.h
//  CrossApp
//
//  Created by Zhujian on 15-8-10.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CAAutoCollectionView__
#define __CrossApp__CAAutoCollectionView__

#include <iostream>
#include <set>
#include "view/CAScrollView.h"
#include "view/CACell.h"
#include "basics/CASTLContainer.h"
#include "basics/CAIndexPath.h"

NS_CC_BEGIN


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
	virtual DSize collectionViewCellSizeAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
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
public:
    // event listeners. If these functions are set, the corresponding function of CAAutoCollectionViewDataSource will fail.
    
    CC_LISTENING_FUNCTION(CACollectionViewCell*(DSize cellSize, unsigned int section, unsigned int item), CellAtIndexPath);
    
    CC_LISTENING_FUNCTION(DSize(unsigned int section, unsigned int item), CellSizeAtIndexPath);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int section), NumberOfItemsAtIndexPath);
    
    CC_LISTENING_FUNCTION(unsigned int(), NumberOfSections);
    
    CC_LISTENING_FUNCTION(CAView*(DSize viewSize, unsigned int section), SectionViewForHeaderInSection);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int section), HeightForHeaderInSection);
    
    CC_LISTENING_FUNCTION(CAView*(DSize viewSize, unsigned int section), SectionViewForFooterInSection);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int section), HeightForFooterInSection);
    
    CC_LISTENING_FUNCTION(void(CACollectionViewCell* cell, unsigned int section, unsigned int item), WillDisplayCellAtIndexPath);
    
    // event listeners. If these functions are set, the corresponding function of CAAutoCollectionViewDelegate will fail.
    
    CC_LISTENING_FUNCTION(void(unsigned int section, unsigned int item), DidSelectCellAtIndexPath);
    
    CC_LISTENING_FUNCTION(void(unsigned int section, unsigned int item), DidDeselectCellAtIndexPath);
    
protected:
    
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
		: pSectionHeaderView(nullptr)
		, pSectionFooterView(nullptr)
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
    
    enum class Orientation
    {
        Vertical = 0,
        Horizontal
    };
    
    enum class CellHoriAlign
    {
        Left,
        Center,
        Right
    };
    
    enum class CellVertAlign
    {
        Top,
        Center,
        Bottom
    };
    
public:
    
	CAAutoCollectionView();
    
	virtual ~CAAutoCollectionView();

	virtual void onEnterTransitionDidFinish();

	virtual void onExitTransitionDidStart();

	static CAAutoCollectionView* createWithFrame(const DRect& rect);

	static CAAutoCollectionView* createWithCenter(const DRect& rect);

    static CAAutoCollectionView* createWithLayout(const DLayout& layout);
    
	virtual bool init();

	void reloadData();

	CACollectionViewCell* dequeueReusableCellWithIdentifier(const char* reuseIdentifier);
    
    virtual void setAllowsSelection(bool var);
    
    virtual void setAllowsMultipleSelection(bool var);
    
	void setSelectRowAtIndexPath(unsigned int section, unsigned int item);

    void setUnSelectRowAtIndexPath(unsigned int section, unsigned int item);
    
    virtual void setShowsScrollIndicators(bool var);
    
	CACollectionViewCell* cellForRowAtIndexPath(unsigned int section, unsigned int item);
    
	const CAVector<CACollectionViewCell*>& displayingCollectionCell();
    
    CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CAAutoCollectionViewDataSource*, m_pCollectionViewDataSource, CollectionViewDataSource);
    
	CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CAAutoCollectionViewDelegate*, m_pCollectionViewDelegate, CollectionViewDelegate);
    
	CC_SYNTHESIZE_RETAIN(CAView*, m_pCollectionHeaderView, CollectionHeaderView);
    
	CC_SYNTHESIZE_RETAIN(CAView*, m_pCollectionFooterView, CollectionFooterView);
    
    CC_SYNTHESIZE(unsigned int, m_nCollectionHeaderHeight, CollectionHeaderHeight);
    
    CC_SYNTHESIZE(unsigned int, m_nCollectionFooterHeight, CollectionFooterHeight);

    CC_PROPERTY(CAAutoCollectionView::Orientation, m_eOrientation, Orientation);
    
    CC_SYNTHESIZE(CAAutoCollectionView::CellHoriAlign, m_eCellHoriAlign, CellHoriAlign);
	CC_SYNTHESIZE(CAAutoCollectionView::CellVertAlign, m_eCellVertAlign, CellVertAlign);

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
    
    virtual void visitEve();
    
protected:
    
    virtual void setContentSize(const DSize& var);
    
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
    
    bool m_bLoadData;
};

NS_CC_END

#endif
