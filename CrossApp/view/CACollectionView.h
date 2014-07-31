//
//  CACollectionView.h
//  CrossApp
//
//  Created by Zhujian on 14-6-27.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __cocos2dx__CACollectionView__
#define __cocos2dx__CACollectionView__

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

	virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item) {};

	virtual void collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item) {};
};


class CACollectionViewDataSource
{
public:
	virtual ~CACollectionViewDataSource(){};

	virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item) = 0;

	virtual unsigned int numberOfSectionsInCollectioView() { return 1; }

	virtual unsigned int numberOfRowsInSectionCollectionView(CACollectionView *collectionView, unsigned int section) = 0;

	virtual unsigned int collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section) { return 0; }

	virtual unsigned int collectionViewHeightForFooterInSection(CACollectionView *collectionView, unsigned int section) { return 0; }

	virtual CAView* collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section){ return NULL; }

	virtual CAView* collectionViewSectionViewForFooterInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section){ return NULL; }

	virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row) = 0;

	virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row) { return 0; }
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

	void reloadData();

	CACollectionViewCell* dequeueReusableCellWithIdentifier(const char* reuseIdentifier);
    
    virtual void setAllowsSelection(bool var);
    
    virtual void setAllowsMultipleSelection(bool var);
    
	void setSelectRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item);

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
    
protected:
    
    inline virtual float maxSpeed(float dt);
    
    inline virtual float maxSpeedCache(float dt);
    
    inline virtual float decelerationRatio(float dt);
    
    inline virtual CCPoint maxBouncesLenght();
    
    virtual void update(float dt);

	void recoveryCollectionCell();

	void loadCollectionCell();
    
public:

	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);

	virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);

	virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);

	virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);

private:
	std::set<CAIndexPath3E> m_pSelectedCollectionCells;

	CACollectionViewCell* m_pHighlightedCollectionCells;

	std::map<CAIndexPath3E, CACollectionViewCell*> m_pUsedCollectionCells;

	std::map<CAIndexPath3E, CCRect> m_pUsedCollectionCellRects;

	std::map<std::string, CAVector<CACollectionViewCell*> > m_pFreedCollectionCells;
};

class CC_DLL CACollectionViewCell : public CAControl
{
public:
    
	CACollectionViewCell();
    
	virtual ~CACollectionViewCell();

	static CACollectionViewCell* create(const char* reuseIdentifier);

	virtual bool initWithReuseIdentifier(const char* reuseIdentifier);

	virtual void draw(void);

	virtual void visit(void);

    CC_PROPERTY(CAView*, m_pBackgroundView, BackgroundView);
    
    CC_SYNTHESIZE(std::string, m_sReuseIdentifier, ReuseIdentifier);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nSection, Section);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nRow, Row);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nItem, Item);
    
public:
    
    virtual CAResponder* nextResponder();
    
protected:
    
    virtual void setControlState(CAControlState var);
    
	virtual void normalCollectionViewCell();
    
	virtual void highlightedCollectionViewCell();
    
	virtual void selectedCollectionViewCell();
    
	virtual void disabledCollectionViewCell();

	virtual void resetCollectionViewCell();
    
    virtual void setContentSize(const CCSize& var);
    
    using CAView::init;
    
    using CAView::initWithCenter;
    
    using CAView::initWithFrame;
    
    using CAView::initWithColor;
    
    friend class CACollectionView;
};

NS_CC_END

#endif