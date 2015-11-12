#include "CACollectionView.h"
#include "basics/CAApplication.h"
#include "control/CAButton.h"
#include "support/CAPointExtension.h"
#include "dispatcher/CATouch.h"
#include "animation/CAViewAnimation.h"

NS_CC_BEGIN

#pragma CACollectionView

CACollectionView::CACollectionView()
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
{
    
}


CACollectionView::~CACollectionView()
{
	CC_SAFE_RELEASE_NULL(m_pCollectionHeaderView);
	CC_SAFE_RELEASE_NULL(m_pCollectionFooterView);
	m_pCollectionViewDataSource = NULL;
	m_pCollectionViewDelegate = NULL;
}

void CACollectionView::onEnterTransitionDidFinish()
{
	CAScrollView::onEnterTransitionDidFinish();

    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0);
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CACollectionView::firstReloadData));
    CAViewAnimation::commitAnimations();
}

void CACollectionView::onExitTransitionDidStart()
{
	CAScrollView::onExitTransitionDidStart();
}

CACollectionView* CACollectionView::createWithFrame(const DRect& rect)
{
	CACollectionView* collectionView = new CACollectionView();
	if (collectionView && collectionView->initWithFrame(rect))
	{
		collectionView->autorelease();
		return collectionView;
	}
	CC_SAFE_DELETE(collectionView);
	return NULL;
}

CACollectionView* CACollectionView::createWithCenter(const DRect& rect)
{
	CACollectionView* collectionView = new CACollectionView();
	if (collectionView && collectionView->initWithCenter(rect))
	{
		collectionView->autorelease();
		return collectionView;
	}
	CC_SAFE_DELETE(collectionView);
	return NULL;
}

bool CACollectionView::init()
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

void CACollectionView::setAllowsSelection(bool var)
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

void CACollectionView::setAllowsMultipleSelection(bool var)
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

void CACollectionView::setSelectRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item)
{
	CC_RETURN_IF(section >= m_rSectionRects.size());

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

	CAIndexPath3E indexPath = CAIndexPath3E(section, row, item);
	if (CACollectionViewCell* cell = m_mpUsedCollectionCells.at(indexPath))
	{
		cell->setControlStateSelected();
	}
	m_pSelectedCollectionCells.insert(indexPath);
}

void CACollectionView::setUnSelectRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item)
{
	CC_RETURN_IF(section >= m_rSectionRects.size());
    
	CAIndexPath3E indexPath = CAIndexPath3E(section, row, item);
    CC_RETURN_IF(m_pSelectedCollectionCells.find(indexPath) == m_pSelectedCollectionCells.end());
	if (CACollectionViewCell* cell = m_mpUsedCollectionCells.at(indexPath))
	{
		cell->setControlStateNormal();
	}
	m_pSelectedCollectionCells.erase(indexPath);
}

void CACollectionView::setShowsScrollIndicators(bool var)
{
    this->setShowsVerticalScrollIndicator(var);
    m_bShowsScrollIndicators = var;
}

CACollectionViewCell* CACollectionView::cellForRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item)
{
    return m_mpUsedCollectionCells[CAIndexPath3E(section, row, item)];
}

const CAVector<CACollectionViewCell*>& CACollectionView::displayingCollectionCell()
{
    return m_vpUsedCollectionCells;
}

bool CACollectionView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	bool isInertia = m_tInertia.getLength() < 1.0f;
	if (!CAScrollView::ccTouchBegan(pTouch, pEvent))
		return false;

	if (m_bAllowsSelection && this->isScrollWindowNotOutSide() == false && isInertia)
	{
		DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);

		std::map<CAIndexPath3E, CACollectionViewCell*>::iterator itr;
		for (itr = m_mpUsedCollectionCells.begin(); itr != m_mpUsedCollectionCells.end(); ++itr)
		{
			CACollectionViewCell* pCell = itr->second;
			CC_CONTINUE_IF(pCell == NULL);

			if (pCell->getFrame().containsPoint(point) && pCell->isVisible())
			{
				CC_BREAK_IF(pCell->getControlState() == CAControlStateDisabled);

                if (m_pHighlightedCollectionCells != pCell)
                {
                    if (m_pHighlightedCollectionCells)
                    {
                        m_pHighlightedCollectionCells->setControlStateNormal();
                    }
                    
                    m_pHighlightedCollectionCells = pCell;
                }

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

void CACollectionView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
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

void CACollectionView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    //CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchEnded(pTouch, pEvent);

	if (m_pHighlightedCollectionCells)
	{
        CAViewAnimation::removeAnimations(m_s__StrID);

		CAIndexPath3E deselectedIndexPath = CAIndexPath3EZero;
		CAIndexPath3E selectedIndexPath = CAIndexPath3E(m_pHighlightedCollectionCells->getSection(), m_pHighlightedCollectionCells->getRow(), m_pHighlightedCollectionCells->getItem());

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
				m_pCollectionViewDelegate->collectionViewDidDeselectCellAtIndexPath(this,
					deselectedIndexPath.section, deselectedIndexPath.row, deselectedIndexPath.item);
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
				m_pCollectionViewDelegate->collectionViewDidSelectCellAtIndexPath(this,
					selectedIndexPath.section, selectedIndexPath.row, selectedIndexPath.item);
			}
		}
        
        m_pHighlightedCollectionCells = NULL;
	}
}

void CACollectionView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
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

void CACollectionView::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
{
    if (m_bAllowsSelection)
    {
        DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);
        
        std::map<CAIndexPath3E, CACollectionViewCell*>::iterator itr;
        for (itr = m_mpUsedCollectionCells.begin(); itr != m_mpUsedCollectionCells.end(); ++itr)
        {
            CACollectionViewCell* cell = itr->second;
            CC_CONTINUE_IF(cell == NULL);
            if (cell->getFrame().containsPoint(point) && cell->isVisible())
            {
                CC_BREAK_IF(cell->getControlState() == CAControlStateDisabled);
                
                if (m_pHighlightedCollectionCells)
                {
                    CAIndexPath3E index = CAIndexPath3E(m_pHighlightedCollectionCells->getSection(),
                                                        m_pHighlightedCollectionCells->getRow(),
                                                        m_pHighlightedCollectionCells->getItem());
                    if (m_pSelectedCollectionCells.count(index))
                    {
                        m_pHighlightedCollectionCells->setControlStateHighlighted();
                    }
                    else
                    {
                        m_pHighlightedCollectionCells->setControlStateNormal();
                    }
                    
                }
                
                m_pHighlightedCollectionCells = cell;
                cell->setControlStateHighlighted();
                
                break;
            }
        }
    }
}

void CACollectionView::mouseMovedOutSide(CATouch* pTouch, CAEvent* pEvent)
{
    if (m_pHighlightedCollectionCells)
    {
        CAIndexPath3E index = CAIndexPath3E(m_pHighlightedCollectionCells->getSection(),
                                            m_pHighlightedCollectionCells->getRow(),
                                            m_pHighlightedCollectionCells->getItem());
        if (m_pSelectedCollectionCells.count(index))
        {
            m_pHighlightedCollectionCells->setControlStateSelected();
        }
        else
        {
            m_pHighlightedCollectionCells->setControlStateNormal();
        }
        m_pHighlightedCollectionCells = NULL;
    }
}

void CACollectionView::switchPCMode(bool var)
{
    CAScrollView::switchPCMode(var);
    this->setMouseMovedEnabled(true);
}

void CACollectionView::reloadViewSizeData()
{    
	clearData();

    m_nSections = m_pCollectionViewDataSource->numberOfSections(this);
    m_nRowsInSections.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        unsigned int rowsInSection = m_pCollectionViewDataSource->numberOfRowsInSection(this, i);
        m_nRowsInSections[i] = rowsInSection;
    }
    
    m_nSectionHeaderHeights.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        unsigned int sectionHeaderHeight = m_pCollectionViewDataSource->collectionViewHeightForHeaderInSection(this, i);
        m_nSectionHeaderHeights[i] = sectionHeaderHeight;
    }
    
    m_nSectionFooterHeights.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        unsigned int sectionFooterHeight = m_pCollectionViewDataSource->collectionViewHeightForFooterInSection(this, i);
        m_nSectionFooterHeights[i] = sectionFooterHeight;
    }
    
    m_nRowHeightss.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        std::vector<unsigned int> rowHeights(m_nRowsInSections.at(i));
        for (unsigned int j=0; j<m_nRowsInSections.at(i); j++)
        {
            unsigned int rowHeight = m_pCollectionViewDataSource->collectionViewHeightForRowAtIndexPath(this, i, j);
            rowHeights[j] = rowHeight;
        }
        m_nRowHeightss[i] = rowHeights;
    }
    
    unsigned int viewHeight = 0;
    
    m_nSectionHeights.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        unsigned int sectionHeight = 0;
        sectionHeight += m_nSectionHeaderHeights.at(i);
        sectionHeight += m_nSectionFooterHeights.at(i);
        sectionHeight += m_nVertInterval;
        for (unsigned int j=0; j<m_nRowHeightss.at(i).size(); j++)
        {
            sectionHeight += m_nRowHeightss.at(i).at(j);
            sectionHeight += m_nVertInterval;
        }
        m_nSectionHeights[i] = sectionHeight;
        viewHeight += sectionHeight;
    }
    
    viewHeight += m_nCollectionHeaderHeight;
    viewHeight += m_nCollectionFooterHeight;
    
    DSize size = this->getBounds().size;
    size.height = viewHeight;
    this->setViewSize(size);

}

void CACollectionView::clearData()
{
	m_rUsedCollectionCellRects.clear();

	m_mpUsedCollectionCells.clear();
	m_rUsedCollectionCellRects.clear();

	m_pSelectedCollectionCells.clear();

	m_pSectionHeaderViews.clear();
	m_pSectionFooterViews.clear();
	m_rSectionRects.clear();

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

void CACollectionView::reloadData()
{
	if (m_pCollectionViewDataSource == NULL)
		return;
    
	this->reloadViewSizeData();
    
	this->removeAllSubviews();
    
	DRect winRect = this->getBounds();
	winRect.origin = getContentOffset();
	float width = winRect.size.width;
	int y = 0;
    
	if (m_nCollectionHeaderHeight > 0 && m_pCollectionHeaderView)
	{
		m_pCollectionHeaderView->setDisplayRange(true);
		m_pCollectionHeaderView->setFrame(DRect(0, y, width, m_nCollectionHeaderHeight));
		addSubview(m_pCollectionHeaderView);
		y += m_nCollectionHeaderHeight;
	}
    
;
    int begin = (int)m_rSectionRects.size();
    m_rSectionRects.resize(m_rSectionRects.size() + m_nSections);
	for (int i = 0; i < m_nSections; i++)
	{
		unsigned int iSectionHeaderHeight = m_nSectionHeaderHeights.at(i);
		DRect sectionHeaderRect = DRect(0, y, width, iSectionHeaderHeight);
		if (iSectionHeaderHeight>0)
		{
			CAView* pSectionHeaderView = m_pCollectionViewDataSource->collectionViewSectionViewForHeaderInSection(this, sectionHeaderRect.size, i);
			if (pSectionHeaderView != NULL)
			{
				pSectionHeaderView->setDisplayRange(true);
				pSectionHeaderView->setFrame(sectionHeaderRect);
				insertSubview(pSectionHeaderView, 1);
				m_pSectionHeaderViews[i] = pSectionHeaderView;
				y += iSectionHeaderHeight;
			}
		}
        
		y += m_nVertInterval;
		unsigned int rowCount = m_nRowsInSections.at(i);
		for (int j = 0; j < rowCount; j++)
		{
			int iHeight = m_nRowHeightss.at(i).at(j);
            
			unsigned int itemCount = m_pCollectionViewDataSource->numberOfItemsInRowsInSection(this, i, j);
            
			unsigned int cellWidth = 0;
			if (itemCount>0)
			{
				cellWidth = (width - m_nHoriInterval) / itemCount - m_nHoriInterval;
			}
			for (int k = 0; k < itemCount; k++)
			{
				CAIndexPath3E indexPath = CAIndexPath3E(i, j, k);
				DRect cellRect = DRect(m_nHoriInterval + (cellWidth + m_nHoriInterval)*k, y, cellWidth, iHeight);
				m_rUsedCollectionCellRects[indexPath] = cellRect;
                
				std::pair<std::map<CAIndexPath3E, CACollectionViewCell*>::iterator, bool> itrResult =
                m_mpUsedCollectionCells.insert(std::make_pair(indexPath, (CACollectionViewCell*)NULL));
                
				CC_CONTINUE_IF(!winRect.intersectsRect(cellRect));
                
				CACollectionViewCell* cell = m_pCollectionViewDataSource->collectionCellAtIndex(this, cellRect.size, i, j, k);
				if (cell)
				{
					addSubview(cell);
					cell->setFrame(cellRect);
					cell->m_nSection = i;
					cell->m_nRow = j;
					cell->m_nItem = k;
					itrResult.first->second = cell;
                    m_vpUsedCollectionCells.pushBack(cell);
                    
                    if (m_pCollectionViewDataSource)
                    {
                        m_pCollectionViewDataSource->collectionViewWillDisplayCellAtIndex(this, cell, i, j, k);
                    }
				}
			}
			y += (iHeight + m_nVertInterval);
		}
        
		unsigned int iSectionFooterHeight = m_nSectionFooterHeights.at(i);
		DRect sectionFooterRect = DRect(0, y, width, iSectionFooterHeight);
		if (iSectionFooterHeight > 0)
		{
			CAView* pSectionFooterView = m_pCollectionViewDataSource->collectionViewSectionViewForFooterInSection(this, sectionFooterRect.size, i);
			if (pSectionFooterView != NULL)
			{
				pSectionFooterView->setDisplayRange(true);
				pSectionFooterView->setFrame(sectionFooterRect);
				insertSubview(pSectionFooterView, 1);
				m_pSectionFooterViews[i] = pSectionFooterView;
				y += iSectionFooterHeight;
			}
		}
        
		DRect sectionRect = sectionHeaderRect;
		sectionRect.size.height = sectionFooterRect.origin.y
        + sectionFooterRect.size.height
        - sectionHeaderRect.origin.y;
        m_rSectionRects[begin + i] = sectionRect;
	}
    
	if (m_nCollectionFooterHeight > 0 && m_pCollectionHeaderView)
	{
		m_pCollectionFooterView->setFrame(DRect(0, y, width, m_nCollectionFooterHeight));
		addSubview(m_pCollectionFooterView);
		y += m_nCollectionFooterHeight;
	}
    
	this->updateSectionHeaderAndFooterRects();
	this->layoutPullToRefreshView();
	this->startDeaccelerateScroll();
}

void CACollectionView::firstReloadData()
{
	CC_RETURN_IF(!m_mpUsedCollectionCells.empty());
	this->reloadData();
}

void CACollectionView::recoveryCollectionCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

	std::map<CAIndexPath3E, CACollectionViewCell*>::iterator itr;
	for (itr = m_mpUsedCollectionCells.begin(); itr != m_mpUsedCollectionCells.end(); itr++)
	{
		CACollectionViewCell* cell = itr->second;
		CC_CONTINUE_IF(cell == NULL);

		DRect cellRect = cell->getFrame();
		CC_CONTINUE_IF(rect.intersectsRect(cellRect));

		m_mpFreedCollectionCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetCollectionViewCell();
		itr->second = NULL;
        m_vpUsedCollectionCells.eraseObject(cell);
	}
}

void CACollectionView::loadCollectionCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

	std::map<CAIndexPath3E, CACollectionViewCell*>::iterator itr;
	for (itr = m_mpUsedCollectionCells.begin(); itr != m_mpUsedCollectionCells.end(); itr++)
	{
		CC_CONTINUE_IF(itr->second != NULL);

		CAIndexPath3E r = itr->first;
		DRect cellRect = m_rUsedCollectionCellRects[r];
		CC_CONTINUE_IF(!rect.intersectsRect(cellRect));

		CACollectionViewCell* cell = m_pCollectionViewDataSource->collectionCellAtIndex(this, cellRect.size, r.section, r.row, r.item);
		if (cell)
		{
			cell->m_nSection = r.section;
			cell->m_nRow = r.row;
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
                m_pCollectionViewDataSource->collectionViewWillDisplayCellAtIndex(this, cell, r.section, r.row, r.item);
            }
		}
	}
}

void CACollectionView::updateSectionHeaderAndFooterRects()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();

	std::vector<DRect>::iterator itr;
	for (itr = m_rSectionRects.begin(); itr != m_rSectionRects.end(); itr++)
	{
		CC_CONTINUE_IF(!rect.intersectsRect(*itr));
		int i = (int)(itr - m_rSectionRects.begin());
		CAView* header = NULL;
		CAView* footer = NULL;
		float headerHeight = 0;
		float footerHeight = 0;
		if (m_pSectionHeaderViews.find(i) != m_pSectionHeaderViews.end())
		{
			header = m_pSectionHeaderViews[i];
			headerHeight = m_pSectionHeaderViews[i]->getFrame().size.height;
		}
		if (m_pSectionFooterViews.find(i) != m_pSectionFooterViews.end())
		{
			footer = m_pSectionFooterViews[i];
			footerHeight = m_pSectionFooterViews[i]->getFrame().size.height;
		}
		if (header && m_bAlwaysTopSectionHeader)
		{
			DPoint p1 = rect.origin;
			p1.y = MAX(p1.y, itr->origin.y);
			p1.y = MIN(p1.y, itr->origin.y + itr->size.height
				- headerHeight - footerHeight);
			header->setFrameOrigin(p1);
		}
		if (footer && m_bAlwaysBottomSectionFooter)
		{
			DPoint p2 = DPointZero;
			p2.y = MIN(rect.origin.y + this->getBounds().size.height - footerHeight,
				itr->origin.y + itr->size.height - footerHeight);
			p2.y = MAX(p2.y, itr->origin.y + headerHeight);
			footer->setFrameOrigin(p2);
		}
	}
}

void CACollectionView::update(float dt)
{
	CAScrollView::update(dt);
    
	recoveryCollectionCell();
    
	loadCollectionCell();
    
	updateSectionHeaderAndFooterRects();
}

float CACollectionView::maxSpeed(float dt)
{
    return (128 * 60 * dt);
}

float CACollectionView::maxSpeedCache(float dt)
{
    return (maxSpeed(dt) * 2.0f);
}

float CACollectionView::decelerationRatio(float dt)
{
    return 1.8f * dt;
}

CACollectionViewCell* CACollectionView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
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

CACollectionViewCell* CACollectionView::getHighlightCollectionCell()
{
    return m_pHighlightedCollectionCells;
}

#pragma CACollectionViewCell

CACollectionViewCell::CACollectionViewCell()
:m_pBackgroundView(NULL)
, m_nSection(0xffffffff)
, m_nRow(0xffffffff)
, m_nItem(0xffffffff)
, m_bControlStateEffect(true)
, m_bAllowsSelected(true)
{
    this->setHaveNextResponder(true);
    this->setDisplayRange(false);
    this->setColor(CAColor_clear);
}


CACollectionViewCell::~CACollectionViewCell()
{
    CC_SAFE_RELEASE_NULL(m_pContentView);
    CC_SAFE_RELEASE_NULL(m_pBackgroundView);
}

CACollectionViewCell* CACollectionViewCell::create(const std::string& reuseIdentifier)
{
	CACollectionViewCell* cell = new CACollectionViewCell();
	if (cell && cell->initWithReuseIdentifier(reuseIdentifier))
	{
		cell->autorelease();
		return cell;
	}
	CC_SAFE_DELETE(cell);
	return NULL;
}

bool CACollectionViewCell::initWithReuseIdentifier(const std::string& reuseIdentifier)
{
    m_pContentView = new CAView();
    this->addSubview(m_pContentView);
    
	this->setBackgroundView(CAView::create());
	this->setReuseIdentifier(reuseIdentifier);
	this->normalCollectionViewCell();

	return true;
}

void CACollectionViewCell::setBackgroundView(CrossApp::CAView *var)
{
	CC_SAFE_RETAIN(var);
	this->removeSubview(m_pBackgroundView);
	CC_SAFE_RELEASE(m_pBackgroundView);
	m_pBackgroundView = var;
	CC_RETURN_IF(m_pBackgroundView == NULL);
	m_pBackgroundView->setFrame(this->getBounds());
	this->insertSubview(m_pBackgroundView, -1);
}

CAView* CACollectionViewCell::getBackgroundView()
{
	return m_pBackgroundView;
}

void CACollectionViewCell::setContentSize(const CrossApp::DSize &var)
{
	CAView::setContentSize(var);
    
    m_pContentView->setFrame(this->getBounds());
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(m_pContentView->getBounds());
    }
}

void CACollectionViewCell::setControlState(const CAControlState& var)
{
	if (m_bAllowsSelected == false && var == CAControlStateSelected)
	{
		CAControl::setControlState(CAControlStateNormal);
	}
	else
	{
		CAControl::setControlState(var);
	}

	CC_RETURN_IF(m_bControlStateEffect == false);
	switch (m_eControlState)
	{
	case CAControlStateNormal:
		this->normalCollectionViewCell();
		break;
	case CAControlStateHighlighted:
		this->highlightedCollectionViewCell();
		break;
	case CAControlStateSelected:
		this->selectedCollectionViewCell();
		break;
	case CAControlStateDisabled:
		this->disabledCollectionViewCell();
		break;
	default:
		break;
	}
}

void CACollectionViewCell::normalCollectionViewCell()
{
	CC_RETURN_IF(m_pBackgroundView == NULL);
	m_pBackgroundView->setColor(ccc4(255, 255, 255, 255));
}

void CACollectionViewCell::highlightedCollectionViewCell()
{
	CC_RETURN_IF(m_pBackgroundView == NULL);
	m_pBackgroundView->setColor(ccc4(50, 193, 255, 255));
}


void CACollectionViewCell::selectedCollectionViewCell()
{
	CC_RETURN_IF(m_pBackgroundView == NULL);
	m_pBackgroundView->setColor(ccc4(50, 193, 255, 255));
}


void CACollectionViewCell::disabledCollectionViewCell()
{
	CC_RETURN_IF(m_pBackgroundView == NULL);
	m_pBackgroundView->setColor(ccc4(127, 127, 127, 255));
}

void CACollectionViewCell::resetCollectionViewCell()
{
	this->setVisible(true);
	this->normalCollectionViewCell();
	this->recoveryCollectionViewCell();
    m_pContentView->setScale(1.0f);
    m_pContentView->setFrame(this->getBounds());
    m_pContentView->setRotation(0);
}

NS_CC_END