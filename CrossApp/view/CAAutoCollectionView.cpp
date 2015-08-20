#include "CAAutoCollectionView.h"
#include "basics/CAApplication.h"
#include "support/CCPointExtension.h"
#include "dispatcher/CATouch.h"
#include "animation/CAViewAnimation.h"

NS_CC_BEGIN

#pragma CAAutoCollectionView

CAAutoCollectionView::CAAutoCollectionView()
: m_pCollectionViewDataSource(NULL)
, m_pCollectionViewDelegate(NULL)
, m_pCollectionHeaderView(NULL)
, m_pCollectionFooterView(NULL)
, m_bAllowsSelection(false)
, m_bAllowsMultipleSelection(false)
, m_pHighlightedCollectionCells(NULL)
, m_nCollectionHeaderHeight(0)
, m_nCollectionFooterHeight(0)
, m_nHoriInterval(0)
, m_nVertInterval(0)
, m_bAlwaysTopSectionHeader(true)
, m_bAlwaysBottomSectionFooter(true)
, m_pCollectionViewOrientation(CACollectionViewOrientationVertical)
{
    
}


CAAutoCollectionView::~CAAutoCollectionView()
{
	CC_SAFE_RELEASE_NULL(m_pCollectionHeaderView);
	CC_SAFE_RELEASE_NULL(m_pCollectionFooterView);
	m_pCollectionViewDataSource = NULL;
	m_pCollectionViewDelegate = NULL;
}

void CAAutoCollectionView::onEnterTransitionDidFinish()
{
	CAScrollView::onEnterTransitionDidFinish();

    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0);
	CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CAAutoCollectionView::firstReloadData));
    CAViewAnimation::commitAnimations();
}

void CAAutoCollectionView::onExitTransitionDidStart()
{
	CAScrollView::onExitTransitionDidStart();
}

CAAutoCollectionView* CAAutoCollectionView::createWithFrame(const CCRect& rect)
{
	CAAutoCollectionView* collectionView = new CAAutoCollectionView();
	if (collectionView && collectionView->initWithFrame(rect))
	{
		collectionView->autorelease();
		return collectionView;
	}
	CC_SAFE_DELETE(collectionView);
	return NULL;
}

CAAutoCollectionView* CAAutoCollectionView::createWithCenter(const CCRect& rect)
{
	CAAutoCollectionView* collectionView = new CAAutoCollectionView();
	if (collectionView && collectionView->initWithCenter(rect))
	{
		collectionView->autorelease();
		return collectionView;
	}
	CC_SAFE_DELETE(collectionView);
	return NULL;
}

bool CAAutoCollectionView::init()
{
	if (!CAScrollView::init())
	{
		return false;
	}

	this->setShowsHorizontalScrollIndicator(false);
	this->setBounceHorizontal(false);
    this->setHorizontalScrollEnabled(false);
	return true;
}

void CAAutoCollectionView::setAllowsSelection(bool var)
{
	m_bAllowsSelection = var;
	CC_RETURN_IF(!m_bAllowsSelection);
	CC_RETURN_IF(m_pSelectedCollectionCells.empty());
	std::set<CAIndexPath3E>::iterator itr;
	for (itr = m_pSelectedCollectionCells.begin(); itr != m_pSelectedCollectionCells.end(); itr++)
	{
		if (CACollectionViewCell* cell = m_mpUsedCollectionCells[(*itr)])
		{
			cell->setControlState(CAControlStateNormal);
		}
	}
	m_pSelectedCollectionCells.clear();
}

void CAAutoCollectionView::setAllowsMultipleSelection(bool var)
{
	m_bAllowsMultipleSelection = var;
	CC_RETURN_IF(m_pSelectedCollectionCells.empty());
	std::set<CAIndexPath3E>::iterator itr;
	for (itr = m_pSelectedCollectionCells.begin(); itr != m_pSelectedCollectionCells.end(); itr++)
	{
		if (CACollectionViewCell* cell = m_mpUsedCollectionCells[(*itr)])
		{
			cell->setControlState(CAControlStateNormal);
		}
	}
	m_pSelectedCollectionCells.clear();
}

void CAAutoCollectionView::setSelectRowAtIndexPath(unsigned int section, unsigned int item)
{
	CC_RETURN_IF(section >= m_rCollectionViewSection.size());

	if (!m_pSelectedCollectionCells.empty() && m_bAllowsMultipleSelection == false)
	{
		std::set<CAIndexPath3E>::iterator itr;
		for (itr = m_pSelectedCollectionCells.begin(); itr != m_pSelectedCollectionCells.end(); itr++)
		{
			if (CACollectionViewCell* cell = m_mpUsedCollectionCells[(*itr)])
			{
				cell->setControlState(CAControlStateNormal);
			}
		}
		m_pSelectedCollectionCells.clear();
	}

	CAIndexPath3E indexPath = CAIndexPath3E(section, 0, item);
	if (CACollectionViewCell* cell = m_mpUsedCollectionCells.at(indexPath))
	{
		cell->setControlStateSelected();
	}
	m_pSelectedCollectionCells.insert(indexPath);
}

void CAAutoCollectionView::setUnSelectRowAtIndexPath(unsigned int section, unsigned int item)
{
	CC_RETURN_IF(section >= m_rCollectionViewSection.size());

	CAIndexPath3E indexPath = CAIndexPath3E(section, 0, item);
	CC_RETURN_IF(m_pSelectedCollectionCells.find(indexPath) == m_pSelectedCollectionCells.end());
	if (CACollectionViewCell* cell = m_mpUsedCollectionCells.at(indexPath))
	{
		cell->setControlStateNormal();
	}
	m_pSelectedCollectionCells.erase(indexPath);
}

void CAAutoCollectionView::setShowsScrollIndicators(bool var)
{
    this->setShowsVerticalScrollIndicator(var);
    m_bShowsScrollIndicators = var;
}

CACollectionViewCell* CAAutoCollectionView::cellForRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item)
{
    return m_mpUsedCollectionCells[CAIndexPath3E(section, row, item)];
}

const CAVector<CACollectionViewCell*>& CAAutoCollectionView::displayingCollectionCell()
{
    return m_vpUsedCollectionCells;
}

void CAAutoCollectionView::setCollectionViewOrientation(CACollectionViewOrientation var)
{
	m_pCollectionViewOrientation = var;
}

CACollectionViewOrientation CAAutoCollectionView::getCollectionViewOrientation()
{
	return m_pCollectionViewOrientation;
}

bool CAAutoCollectionView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    if (!m_vTouches.empty())
    {
        m_vTouches.replace(0, pTouch);
        return true;
    }
	bool isInertia = m_tInertia.getLength() < 1.0f;
	if (!CAScrollView::ccTouchBegan(pTouch, pEvent))
		return false;

	if (m_bAllowsSelection && this->isScrollWindowNotOutSide() == false && isInertia)
	{
		CCPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);

		std::map<CAIndexPath3E, CACollectionViewCell*>::iterator itr;
		for (itr = m_mpUsedCollectionCells.begin(); itr != m_mpUsedCollectionCells.end(); ++itr)
		{
			CACollectionViewCell* pCell = itr->second;
			CC_CONTINUE_IF(pCell == NULL);

			if (pCell->getFrame().containsPoint(point) && pCell->isVisible())
			{
				CC_BREAK_IF(pCell->getControlState() == CAControlStateDisabled);

				if (m_pHighlightedCollectionCells)
				{
					m_pHighlightedCollectionCells->setControlStateNormal();
				}

				m_pHighlightedCollectionCells = pCell;

				CC_BREAK_IF(pCell->getControlState() == CAControlStateSelected);

                CAViewAnimation::beginAnimations(m_s__StrID, NULL);
                CAViewAnimation::setAnimationDuration(0.05f);
                CAViewAnimation::setAnimationDidStopSelector(pCell, CAViewAnimation0_selector(CACollectionViewCell::setControlStateHighlighted));
                CAViewAnimation::commitAnimations();
				break;
			}
		}
	}
	return true;
}

void CAAutoCollectionView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchMoved(pTouch, pEvent);

	if (m_pHighlightedCollectionCells)
	{
		CAViewAnimation::removeAnimations(m_s__StrID);

		if (m_pHighlightedCollectionCells->getControlState() == CAControlStateHighlighted)
		{
			m_pHighlightedCollectionCells->setControlStateNormal();
		}

		m_pHighlightedCollectionCells = NULL;
	}

}

void CAAutoCollectionView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    //CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchEnded(pTouch, pEvent);

	if (m_pHighlightedCollectionCells)
	{
		CAViewAnimation::removeAnimations(m_s__StrID);

		CAIndexPath3E deselectedIndexPath = CAIndexPath3EZero;
		CAIndexPath3E selectedIndexPath = CAIndexPath3E(m_pHighlightedCollectionCells->getSection(), 0, m_pHighlightedCollectionCells->getItem());

		if (m_pSelectedCollectionCells.count(selectedIndexPath) > 0 && m_bAllowsMultipleSelection)
		{
			deselectedIndexPath = selectedIndexPath;
			selectedIndexPath = CAIndexPath3EZero;
			m_pSelectedCollectionCells.erase(deselectedIndexPath);
		}
		else
		{
			if (m_pSelectedCollectionCells.size() > 0 && m_bAllowsMultipleSelection == false)
			{
				deselectedIndexPath = *m_pSelectedCollectionCells.begin();
				m_pSelectedCollectionCells.clear();
			}
			m_pSelectedCollectionCells.insert(selectedIndexPath);
		}

		if (deselectedIndexPath != CAIndexPath3EZero)
		{
			if (CACollectionViewCell* cell = m_mpUsedCollectionCells[deselectedIndexPath])
			{
				cell->setControlStateNormal();
			}
			if (m_pCollectionViewDelegate)
			{
				m_pCollectionViewDelegate->collectionViewDidDeselectCellAtIndexPath(this, deselectedIndexPath.section, deselectedIndexPath.item);
			}
		}

		if (selectedIndexPath != CAIndexPath3EZero)
		{
			if (CACollectionViewCell* cell = m_mpUsedCollectionCells[selectedIndexPath])
			{
				cell->setControlStateSelected();
			}
			if (m_pCollectionViewDelegate)
			{
				m_pCollectionViewDelegate->collectionViewDidSelectCellAtIndexPath(this, selectedIndexPath.section, selectedIndexPath.item);
			}
		}
        
        m_pHighlightedCollectionCells = NULL;
	}
}

void CAAutoCollectionView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchCancelled(pTouch, pEvent);

	if (m_pHighlightedCollectionCells)
	{
		CAViewAnimation::removeAnimations(m_s__StrID);

        if (m_pHighlightedCollectionCells->getControlState() == CAControlStateHighlighted)
        {
            m_pHighlightedCollectionCells->setControlStateNormal();
        }
		m_pHighlightedCollectionCells = NULL;
	}
}

void CAAutoCollectionView::reloadViewSizeData()
{    
	clearData();

    int nSections = m_pCollectionViewDataSource->numberOfSections(this);
	m_rCollectionViewSection.resize(nSections);
    
	int dw = 0;
	if (m_pCollectionViewOrientation == CACollectionViewOrientationVertical)
	{
		dw = this->getBounds().size.width - 2 * m_nHoriInterval;
	}
	else
	{
		dw = this->getBounds().size.height - 2 * m_nVertInterval;
	}
	

	CollectionViewRow r;
	for (unsigned int i = 0; i<nSections; i++)
    {
		CollectionViewSection& cvs = m_rCollectionViewSection[i];
		cvs.nSectionHeaderHeight = m_pCollectionViewDataSource->collectionViewHeightForHeaderInSection(this, i);
		cvs.nSectionFooterHeight = m_pCollectionViewDataSource->collectionViewHeightForFooterInSection(this, i);

		int iCurW = 0;

		unsigned int itemCount = m_pCollectionViewDataSource->numberOfItemsInSection(this, i);
		for (int j = 0; j < itemCount; j++)
		{
			CCSize rSize = m_pCollectionViewDataSource->collectionViewSizeForItemAtIndexPath(this, i, j);

			int d1 = 0, d2 = 0, d3 = 0;
			if (m_pCollectionViewOrientation == CACollectionViewOrientationVertical)
			{
				d1 = rSize.width;
				d2 = rSize.height;
				d3 = m_nHoriInterval;
			}
			else
			{
				d1 = rSize.height;
				d2 = rSize.width;
				d3 = m_nVertInterval;
			}

			if (iCurW + d1 > dw && r.iHeight > 0)
			{
				iCurW = 0;
				cvs.CollectionViewRows.push_back(r);
				r = CollectionViewRow();
			}

			iCurW += (d1 + d3);
			if (r.iHeight < d2)
			{
				r.iHeight = d2;
			}
			r.rItemSizes.push_back(rSize);
		}
		if (r.iHeight > 0)
		{
			cvs.CollectionViewRows.push_back(r);
		}
    }
    
	unsigned int viewHeight = 0;
	for (int i = 0; i < nSections; i++)
	{
		CollectionViewSection& cvs = m_rCollectionViewSection[i];

		unsigned int sectionHeight = 0;
		sectionHeight += cvs.nSectionHeaderHeight;
		sectionHeight += cvs.nSectionFooterHeight;
		sectionHeight += (m_pCollectionViewOrientation == CACollectionViewOrientationVertical) ? m_nVertInterval : m_nHoriInterval;
		for (unsigned int j = 0; j < cvs.CollectionViewRows.size(); j++)
		{
			sectionHeight += cvs.CollectionViewRows[j].iHeight;
			sectionHeight += (m_pCollectionViewOrientation == CACollectionViewOrientationVertical) ? m_nVertInterval : m_nHoriInterval;
		}
		viewHeight += sectionHeight;
	}

	viewHeight += m_nCollectionHeaderHeight;
	viewHeight += m_nCollectionFooterHeight;

	CCSize size = this->getBounds().size;
	if (m_pCollectionViewOrientation == CACollectionViewOrientationVertical)
	{
		size.height = viewHeight;
	}
	else
	{
		size.width = viewHeight;
	}
	this->setViewSize(size);
}

void CAAutoCollectionView::clearData()
{
	m_mpUsedCollectionCells.clear();
	m_rUsedCollectionCellRects.clear();
	m_pSelectedCollectionCells.clear();
	m_rCollectionViewSection.clear();

	for (int i = 0; i < m_vpUsedCollectionCells.size(); i++)
	{
		CACollectionViewCell* cell = m_vpUsedCollectionCells.at(i);
		CC_CONTINUE_IF(cell == NULL);
		m_mpFreedCollectionCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetCollectionViewCell();
	}
	m_vpUsedCollectionCells.clear();

	m_pHighlightedCollectionCells = NULL;
}

void CAAutoCollectionView::reloadData()
{
	if (m_pCollectionViewDataSource == NULL)
		return;
    
	this->reloadViewSizeData();
    
	this->removeAllSubviews();
    
	CCRect winRect = this->getBounds();
	winRect.origin = getContentOffset();
	
	int x = 0, y = 0, z = 0;
    
	if (m_nCollectionHeaderHeight > 0 && m_pCollectionHeaderView)
	{
		m_pCollectionHeaderView->setDisplayRange(true);
		if (m_pCollectionViewOrientation == CACollectionViewOrientationVertical)
		{
			m_pCollectionHeaderView->setFrame(CCRect(0, y, winRect.size.width, m_nCollectionHeaderHeight));
			y += m_nCollectionHeaderHeight;
		}
		else
		{
			m_pCollectionHeaderView->setFrame(CCRect(x, 0, m_nCollectionHeaderHeight, winRect.size.height));
			x += m_nCollectionHeaderHeight;
		}
		addSubview(m_pCollectionHeaderView);
	}
    
	for (int i = 0; i < m_rCollectionViewSection.size(); i++)
	{
		CollectionViewSection& cvs = m_rCollectionViewSection[i];

		unsigned int iSectionHeaderHeight = cvs.nSectionHeaderHeight;
		CCRect sectionHeaderRect;
		if (m_pCollectionViewOrientation == CACollectionViewOrientationVertical)
		{
			sectionHeaderRect = CCRect(0, y, winRect.size.width, iSectionHeaderHeight);
		}
		else
		{
			sectionHeaderRect = CCRect(x, 0, iSectionHeaderHeight, winRect.size.height);
		}
		
		if (iSectionHeaderHeight>0)
		{
			CAView* pSectionHeaderView = m_pCollectionViewDataSource->collectionViewSectionViewForHeaderInSection(this, sectionHeaderRect.size, i);
			if (pSectionHeaderView != NULL)
			{
				pSectionHeaderView->setDisplayRange(true);
				pSectionHeaderView->setFrame(sectionHeaderRect);
				insertSubview(pSectionHeaderView, 1);
				if (m_pCollectionViewOrientation == CACollectionViewOrientationVertical)
				{
					y += iSectionHeaderHeight;
				}
				else
				{
					x += iSectionHeaderHeight;
				}
			}
			cvs.pSectionHeaderView = pSectionHeaderView;
		}

		if (m_pCollectionViewOrientation == CACollectionViewOrientationVertical)
		{
			y += m_nVertInterval;
		}
		else
		{
			x += m_nHoriInterval;
		}

		for (int j = 0, l=0; j < cvs.CollectionViewRows.size(); j++)
		{
			CollectionViewRow& r = cvs.CollectionViewRows[j];
			int iHeight = r.iHeight;

			if (m_pCollectionViewOrientation == CACollectionViewOrientationVertical)
			{
				z = m_nHoriInterval;
			}
			else
			{
				z = m_nVertInterval;
			}
			for (int k = 0; k < r.rItemSizes.size(); k++, l++)
			{
				CAIndexPath3E indexPath = CAIndexPath3E(i, 0, l);

				CCRect cellRect;
				if (m_pCollectionViewOrientation == CACollectionViewOrientationVertical)
				{
					int d = (iHeight - r.rItemSizes[k].height) / 2;
					cellRect = CCRect(z, y + d, r.rItemSizes[k].width, r.rItemSizes[k].height);
					z += (r.rItemSizes[k].width + m_nHoriInterval);
				}
				else
				{
					int d = (iHeight - r.rItemSizes[k].width) / 2;
					cellRect = CCRect(x + d, z, r.rItemSizes[k].width, r.rItemSizes[k].height);
					z += (r.rItemSizes[k].height + m_nVertInterval);
				}
				m_rUsedCollectionCellRects[indexPath] = cellRect;

				std::pair<std::map<CAIndexPath3E, CACollectionViewCell*>::iterator, bool> itrResult =
					m_mpUsedCollectionCells.insert(std::make_pair(indexPath, (CACollectionViewCell*)NULL));

				CC_CONTINUE_IF(!winRect.intersectsRect(cellRect));

				CACollectionViewCell* cell = m_pCollectionViewDataSource->collectionCellAtIndex(this, cellRect.size, i, l);
				if (cell)
				{
					addSubview(cell);
					cell->setFrame(cellRect);
					cell->m_nSection = i;
					cell->m_nItem = l;
					itrResult.first->second = cell;
					m_vpUsedCollectionCells.pushBack(cell);

					if (m_pCollectionViewDataSource)
					{
						m_pCollectionViewDataSource->collectionViewWillDisplayCellAtIndex(this, cell, i, l);
					}
				}
			}
			if (m_pCollectionViewOrientation == CACollectionViewOrientationVertical)
			{
				y += (iHeight + m_nVertInterval);
			}
			else
			{
				x += (iHeight + m_nHoriInterval);
			}
		}


		unsigned int iSectionFooterHeight = cvs.nSectionFooterHeight;
		CCRect sectionFooterRect = CCRectMake(0, y, winRect.size.width, iSectionFooterHeight);
		if (iSectionFooterHeight > 0)
		{
			CAView* pSectionFooterView = m_pCollectionViewDataSource->collectionViewSectionViewForFooterInSection(this, sectionFooterRect.size, i);
			if (pSectionFooterView != NULL)
			{
				pSectionFooterView->setDisplayRange(true);
				pSectionFooterView->setFrame(sectionFooterRect);
				insertSubview(pSectionFooterView, 1);
				y += iSectionFooterHeight;
			}
			cvs.pSectionHeaderView = pSectionFooterView;
		}
        
		CCRect sectionRect = sectionHeaderRect;
		sectionRect.size.height = sectionFooterRect.origin.y + sectionFooterRect.size.height  - sectionHeaderRect.origin.y;
		cvs.rSectionRect = sectionRect;
	}
    
	if (m_nCollectionFooterHeight > 0 && m_pCollectionHeaderView)
	{
		if (m_pCollectionViewOrientation == CACollectionViewOrientationVertical)
		{
			m_pCollectionFooterView->setFrame(CCRect(0, y, winRect.size.width, m_nCollectionFooterHeight));
			y += m_nCollectionFooterHeight;
		}
		else
		{
			m_pCollectionFooterView->setFrame(CCRect(x, 0, m_nCollectionFooterHeight, winRect.size.height));
			x += m_nCollectionFooterHeight;
		}
		addSubview(m_pCollectionFooterView);
	}
    
	this->updateSectionHeaderAndFooterRects();
	this->layoutPullToRefreshView();
	this->startDeaccelerateScroll();
}

void CAAutoCollectionView::firstReloadData()
{
	CC_RETURN_IF(!m_mpUsedCollectionCells.empty());
	this->reloadData();
}

void CAAutoCollectionView::recoveryCollectionCell()
{
	CCRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

	std::map<CAIndexPath3E, CACollectionViewCell*>::iterator itr;
	for (itr = m_mpUsedCollectionCells.begin(); itr != m_mpUsedCollectionCells.end(); itr++)
	{
		CACollectionViewCell* cell = itr->second;
		CC_CONTINUE_IF(cell == NULL);

		CCRect cellRect = cell->getFrame();
		CC_CONTINUE_IF(rect.intersectsRect(cellRect));

		m_mpFreedCollectionCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetCollectionViewCell();
		itr->second = NULL;
        m_vpUsedCollectionCells.eraseObject(cell);
	}
}

void CAAutoCollectionView::loadCollectionCell()
{
	CCRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

	std::map<CAIndexPath3E, CACollectionViewCell*>::iterator itr;
	for (itr = m_mpUsedCollectionCells.begin(); itr != m_mpUsedCollectionCells.end(); itr++)
	{
		CC_CONTINUE_IF(itr->second != NULL);

		CAIndexPath3E r = itr->first;
		CCRect cellRect = m_rUsedCollectionCellRects[r];
		CC_CONTINUE_IF(!rect.intersectsRect(cellRect));

		CACollectionViewCell* cell = m_pCollectionViewDataSource->collectionCellAtIndex(this, cellRect.size, r.section, r.item);
		if (cell)
		{
			cell->m_nSection = r.section;
			cell->m_nItem = r.item;
			cell->updateDisplayedAlpha(this->getAlpha());
			this->addSubview(cell);
			cell->setFrame(cellRect);
			itr->second = cell;
            m_vpUsedCollectionCells.pushBack(cell);
            
			if (m_pSelectedCollectionCells.count(r))
			{
				cell->setControlStateSelected();
			}
            
            if (m_pCollectionViewDataSource)
            {
                m_pCollectionViewDataSource->collectionViewWillDisplayCellAtIndex(this, cell, r.section, r.item);
            }
		}
	}
}

void CAAutoCollectionView::updateSectionHeaderAndFooterRects()
{
	CCRect rect = this->getBounds();
	rect.origin = getContentOffset();

	for (int i = 0; i < m_rCollectionViewSection.size(); i++)
	{
		CollectionViewSection& cvs = m_rCollectionViewSection[i];

		CC_CONTINUE_IF(!rect.intersectsRect(cvs.rSectionRect));

		CAView* header = cvs.pSectionHeaderView;
		CAView* footer = cvs.pSectionFooterView;
		
		float headerHeight = cvs.nSectionHeaderHeight;
		float footerHeight = cvs.nSectionFooterHeight;

		if (header && m_bAlwaysTopSectionHeader)
		{
			CCPoint p1 = rect.origin;
			p1.y = MAX(p1.y, cvs.rSectionRect.origin.y);
			p1.y = MIN(p1.y, cvs.rSectionRect.origin.y + cvs.rSectionRect.size.height
				- headerHeight - footerHeight);
			header->setFrameOrigin(p1);
		}
		if (footer && m_bAlwaysBottomSectionFooter)
		{
			CCPoint p2 = CCPointZero;
			p2.y = MIN(rect.origin.y + this->getBounds().size.height - footerHeight,
				cvs.rSectionRect.origin.y + cvs.rSectionRect.size.height - footerHeight);
			p2.y = MAX(p2.y, cvs.rSectionRect.origin.y + headerHeight);
			footer->setFrameOrigin(p2);
		}
	}
}

void CAAutoCollectionView::update(float dt)
{
	CAScrollView::update(dt);
    
	recoveryCollectionCell();
    
	loadCollectionCell();
    
	updateSectionHeaderAndFooterRects();
}

float CAAutoCollectionView::maxSpeed(float dt)
{
    return (_px(128) * 60 * dt);
}

float CAAutoCollectionView::maxSpeedCache(float dt)
{
    return (maxSpeed(dt) * 2.0f);
}

float CAAutoCollectionView::decelerationRatio(float dt)
{
    return 1.8f * dt;
}

CACollectionViewCell* CAAutoCollectionView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
{
    CACollectionViewCell* cell = NULL;
    
    if (reuseIdentifier && !m_mpFreedCollectionCells[reuseIdentifier].empty())
    {
        cell = m_mpFreedCollectionCells[reuseIdentifier].back();
        cell->retain()->autorelease();
        m_mpFreedCollectionCells[reuseIdentifier].popBack();
    }
    
    return cell;
}

CACollectionViewCell* CAAutoCollectionView::getHighlightCollectionCell()
{
    return m_pHighlightedCollectionCells;
}

NS_CC_END