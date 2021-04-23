//
//  CACollectionView.h
//  CrossApp
//
//  Created by Zhujian on 14-6-27.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CrossApp__CACollectionView__
#define __CrossApp__CACollectionView__

#include <iostream>
#include <set>
#include "view/CAScrollView.h"
#include "view/CACell.h"
#include "basics/CASTLContainer.h"
#include "basics/CAIndexPath.h"

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
	virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
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
    
	virtual CAView* collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const DSize& viewSize, unsigned int section)
    {
        return NULL;
    }

    virtual unsigned int collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section)
    {
        return 0;
    }
    
	virtual CAView* collectionViewSectionViewForFooterInSection(CACollectionView *collectionView, const DSize& viewSize, unsigned int section)
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
    
    // event listeners. If these functions are set, the corresponding function of CACollectionViewDataSource will fail.
    
    CC_LISTENING_FUNCTION(CACollectionViewCell*(DSize cellSize, unsigned int section, unsigned int row, unsigned int item), CellAtIndexPath);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int section, unsigned int row), CellHeightAtIndexPath);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int section, unsigned int row), NumberOfItemsAtIndexPath);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int section), NumberOfRowsAtIndexPath);
    
    CC_LISTENING_FUNCTION(unsigned int(), NumberOfSections);
    
    CC_LISTENING_FUNCTION(CAView*(DSize viewSize, unsigned int section), SectionViewForHeaderInSection);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int section), HeightForHeaderInSection);
    
    CC_LISTENING_FUNCTION(CAView*(DSize viewSize, unsigned int section), SectionViewForFooterInSection);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int section), HeightForFooterInSection);
    
    CC_LISTENING_FUNCTION(void(CACollectionViewCell* cell, unsigned int section, unsigned int row, unsigned int item), WillDisplayCellAtIndexPath);
    
    // event listeners. If these functions are set, the corresponding function of CACollectionViewDelegate will fail.
    
    CC_LISTENING_FUNCTION(void(unsigned int section, unsigned int row, unsigned int item), DidSelectCellAtIndexPath);
    
    CC_LISTENING_FUNCTION(void(unsigned int section, unsigned int row, unsigned int item), DidDeselectCellAtIndexPath);
    
public:
	CACollectionView();
    
	virtual ~CACollectionView();

	virtual void onEnterTransitionDidFinish();

	virtual void onExitTransitionDidStart();

	static CACollectionView* createWithFrame(const DRect& rect);

	static CACollectionView* createWithCenter(const DRect& rect);

    static CACollectionView* createWithLayout(const DLayout& layout);
    
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
    
    CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CACollectionViewDataSource*, m_pCollectionViewDataSource, CollectionViewDataSource);
    
	CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CACollectionViewDelegate*, m_pCollectionViewDelegate, CollectionViewDelegate);
    
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
    
    virtual void visitEve();
    
protected:
    
    virtual void setContentSize(const DSize& var);
    
    inline virtual float maxSpeed(float dt);

    inline virtual float decelerationRatio(float dt);
    
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
    
    unsigned int m_nSections;
    
    std::vector<unsigned int> m_nRowsInSections;
    
    std::vector<unsigned int> m_nSectionHeights;
    
    std::vector<unsigned int> m_nSectionHeaderHeights;
    
    std::vector<unsigned int> m_nSectionFooterHeights;
    
    std::vector<std::vector<unsigned int> > m_nRowHeightss;
    
    std::vector<DRect> m_rSectionRects;
    
    std::map<CAIndexPath3E, DRect> m_rUsedCollectionCellRects;
    
    std::map<int, CAView*> m_pSectionHeaderViews;
    
    std::map<int, CAView*> m_pSectionFooterViews;
    
	std::set<CAIndexPath3E> m_pSelectedCollectionCells;

	CACollectionViewCell* m_pHighlightedCollectionCells;

	std::map<CAIndexPath3E, CACollectionViewCell*> m_mpUsedCollectionCells;

    CAVector<CACollectionViewCell*> m_vpUsedCollectionCells;
    
	std::map<std::string, CAVector<CACollectionViewCell*> > m_mpFreedCollectionCells;
    
    bool m_bLoadData;
};

class CC_DLL CACollectionViewCell : public CACell
{
	
public:
    
	CACollectionViewCell();
    
	virtual ~CACollectionViewCell();

	static CACollectionViewCell* create(const std::string& reuseIdentifier);

    CC_SYNTHESIZE_READONLY(unsigned int, m_nSection, Section);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nRow, Row);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nItem, Item);
    
protected:
    
    virtual void normalCell();
    
    virtual void highlightedCell();
    
    virtual void selectedCell();
    
    virtual void disabledCell();
    
    virtual void recoveryCell(){}

    friend class CACollectionView;
    
    friend class CAAutoCollectionView;
};

NS_CC_END

#endif
