#include "CACollectionView.h"
#include "basics/CAApplication.h"
#include "control/CAButton.h"
#include "support/CCPointExtension.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionEase.h"
#include "dispatcher/CATouch.h"
#include "view/CAWindow.h"
#include <utility>

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

	this->runAction(CCSequence::create(CCDelayTime::create(1 / 60.0f),
		CCCallFunc::create(this, callfunc_selector(CACollectionView::firstReloadData)), NULL));
}

void CACollectionView::onExitTransitionDidStart()
{
	CAScrollView::onExitTransitionDidStart();
}

CACollectionView* CACollectionView::createWithFrame(const CCRect& rect)
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

CACollectionView* CACollectionView::createWithCenter(const CCRect& rect)
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
	this->setTouchMovedListenHorizontal(false);
	return true;
}

float CACollectionView::maxSpeed(float dt)
{
	return (CCPoint(m_obContentSize).getLength() * 8 * dt);
}

float CACollectionView::maxSpeedCache(float dt)
{
	return (maxSpeed(dt) * 3.0f);
}

float CACollectionView::decelerationRatio(float dt)
{
	return 2.0f * dt;
}

void CACollectionView::setAllowsSelection(bool var)
{
	m_bAllowsSelection = var;
	CC_RETURN_IF(!m_bAllowsSelection);
	CC_RETURN_IF(m_pSelectedCollectionCells.empty());
	std::set<CAIndexPath3E>::iterator itr;
	for (itr = m_pSelectedCollectionCells.begin(); itr != m_pSelectedCollectionCells.end(); itr++)
	{
		if (CACollectionViewCell* cell = m_pUsedCollectionCells[(*itr)])
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
		if (CACollectionViewCell* cell = m_pUsedCollectionCells[(*itr)])
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
			if (CACollectionViewCell* cell = m_pUsedCollectionCells[(*itr)])
			{
				cell->setControlState(CAControlStateNormal);
			}
		}
		m_pSelectedCollectionCells.clear();
	}

	CAIndexPath3E indexPath = CAIndexPath3E(section, row, item);
	if (CACollectionViewCell* cell = m_pUsedCollectionCells.at(indexPath))
	{
		cell->setControlStateSelected();
	}
	m_pSelectedCollectionCells.insert(indexPath);
}

void CACollectionView::setUnSelectRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item)
{
	CC_RETURN_IF(section >= m_rSectionRects.size());

	CAIndexPath3E indexPath = CAIndexPath3E(section, row, item);
	if (CACollectionViewCell* cell = m_pUsedCollectionCells.at(indexPath))
	{
		cell->setControlStateNormal();
	}
	m_pSelectedCollectionCells.erase(indexPath);
}

bool CACollectionView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	if (m_pTouches->count() > 0)
	{
		m_pTouches->replaceObjectAtIndex(0, pTouch);
		return true;
	}
	bool isInertia = m_tInertia.getLength() < 1.0f;
	if (!CAScrollView::ccTouchBegan(pTouch, pEvent))
		return false;

	if (m_bAllowsSelection && this->isScrollWindowNotOutSide() == false && isInertia)
	{
		CCPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);

		std::map<CAIndexPath3E, CACollectionViewCell*>::iterator itr;
		for (itr = m_pUsedCollectionCells.begin(); itr != m_pUsedCollectionCells.end(); ++itr)
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

				CCDelayTime* delayTime = CCDelayTime::create(0.05f);
				CCCallFunc* func = CCCallFunc::create(pCell, callfunc_selector(CACollectionViewCell::setControlStateHighlighted));
				CCSequence* actions = CCSequence::create(delayTime, func, NULL);
				m_pContainer->runAction(actions);
				break;
			}
		}
	}
	return true;
}

void CACollectionView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	CC_RETURN_IF(m_bscrollEnabled == false);

	CAScrollView::ccTouchMoved(pTouch, pEvent);

	if (m_pHighlightedCollectionCells)
	{
		m_pContainer->stopAllActions();

		if (m_pHighlightedCollectionCells->getControlState() == CAControlStateHighlighted)
		{
			m_pHighlightedCollectionCells->setControlStateNormal();
		}

		m_pHighlightedCollectionCells = NULL;
	}

}

void CACollectionView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
	CAScrollView::ccTouchEnded(pTouch, pEvent);

	if (m_pHighlightedCollectionCells)
	{
		m_pContainer->stopAllActions();

		CAIndexPath3E deselectedIndexPath = CAIndexPath3EZero;
		CAIndexPath3E selectedIndexPath = CAIndexPath3E(m_pHighlightedCollectionCells->getSection(), m_pHighlightedCollectionCells->getRow(), m_pHighlightedCollectionCells->getItem());
		m_pHighlightedCollectionCells = NULL;

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
			if (CACollectionViewCell* cell = m_pUsedCollectionCells[deselectedIndexPath])
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
			if (CACollectionViewCell* cell = m_pUsedCollectionCells[selectedIndexPath])
			{
				cell->setControlStateSelected();
			}
			if (m_pCollectionViewDelegate)
			{
				m_pCollectionViewDelegate->collectionViewDidSelectCellAtIndexPath(this,
					selectedIndexPath.section, selectedIndexPath.row, selectedIndexPath.item);
			}
		}
	}
}

void CACollectionView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
	CAScrollView::ccTouchCancelled(pTouch, pEvent);

	if (m_pHighlightedCollectionCells)
	{
		m_pContainer->stopAllActions();

		m_pHighlightedCollectionCells->setControlStateNormal();
		m_pHighlightedCollectionCells = NULL;
	}
}

CACollectionViewCell* CACollectionView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
{
	CACollectionViewCell* cell = NULL;
    
	if (reuseIdentifier && !m_pFreedCollectionCells[reuseIdentifier].empty())
	{
		cell = m_pFreedCollectionCells[reuseIdentifier].back();
		cell->retain()->autorelease();
		m_pFreedCollectionCells[reuseIdentifier].popBack();
	}
    
	return cell;
}

void CACollectionView::reloadViewSizeData()
{
    m_rUsedCollectionCellRects.clear();
    
	m_pUsedCollectionCells.clear();
	m_rUsedCollectionCellRects.clear();
    
	m_pSelectedCollectionCells.clear();
    
	m_pSectionFooterViews.clear();
	m_pSectionFooterViews.clear();
	m_rSectionRects.clear();
    
    
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
    
    CCSize size = this->getBounds().size;
    size.height = viewHeight;
    this->setViewSize(size);

}

void CACollectionView::reloadData()
{
	if (m_pCollectionViewDataSource == NULL)
		return;
    
	this->reloadViewSizeData();
    
	this->removeAllSubviews();
    
	CCRect winRect = this->getBounds();
	winRect.origin = getContentOffset();
	float width = winRect.size.width;
	int y = 0;
    
	if (m_nCollectionHeaderHeight > 0 && m_pCollectionHeaderView)
	{
		m_pCollectionHeaderView->setDisplayRange(true);
		m_pCollectionHeaderView->setFrame(CCRect(0, y, width, m_nCollectionHeaderHeight));
		addSubview(m_pCollectionHeaderView);
		y += m_nCollectionHeaderHeight;
	}
    
;
	for (int i = 0; i < m_nSections; i++)
	{
		unsigned int iSectionHeaderHeight = m_nSectionHeaderHeights.at(i);
		CCRect sectionHeaderRect = CCRect(0, y, width, iSectionHeaderHeight);
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
				CCRect cellRect = CCRect(m_nHoriInterval + (cellWidth + m_nHoriInterval)*k, y, cellWidth, iHeight);
				m_rUsedCollectionCellRects[indexPath] = cellRect;
                
				std::pair<std::map<CAIndexPath3E, CACollectionViewCell*>::iterator, bool> itrResult =
                m_pUsedCollectionCells.insert(std::make_pair(indexPath, (CACollectionViewCell*)NULL));
                
				CC_CONTINUE_IF(!winRect.intersectsRect(cellRect));
                
				CACollectionViewCell* pCellView = m_pCollectionViewDataSource->collectionCellAtIndex(this, cellRect.size, i, j, k);
				if (pCellView)
				{
					addSubview(pCellView);
					pCellView->setFrame(cellRect);
					pCellView->m_nSection = i;
					pCellView->m_nRow = j;
					pCellView->m_nItem = k;
					itrResult.first->second = pCellView;
				}
			}
			y += (iHeight + m_nVertInterval);
		}
        
		unsigned int iSectionFooterHeight = m_nSectionFooterHeights.at(i);
		CCRect sectionFooterRect = CCRectMake(0, y, width, iSectionFooterHeight);
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
        
		CCRect sectionRect = sectionHeaderRect;
		sectionRect.size.height = sectionFooterRect.origin.y
        + sectionFooterRect.size.height
        - sectionHeaderRect.origin.y;
		m_rSectionRects.push_back(sectionRect);
	}
    
	if (m_nCollectionFooterHeight > 0 && m_pCollectionHeaderView)
	{
		m_pCollectionFooterView->setFrame(CCRect(0, y, width, m_nCollectionFooterHeight));
		addSubview(m_pCollectionFooterView);
		y += m_nCollectionFooterHeight;
	}
    
	this->updateSectionHeaderAndFooterRects();
	this->layoutPullToRefreshView();
	this->startDeaccelerateScroll();
}

void CACollectionView::firstReloadData()
{
	CC_RETURN_IF(!m_pUsedCollectionCells.empty());
	this->reloadData();
}

void CACollectionView::recoveryCollectionCell()
{
	CCRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

	std::map<CAIndexPath3E, CACollectionViewCell*>::iterator itr;
	for (itr = m_pUsedCollectionCells.begin(); itr != m_pUsedCollectionCells.end(); itr++)
	{
		CACollectionViewCell* cell = itr->second;
		CC_CONTINUE_IF(cell == NULL);

		CCRect cellRect = cell->getFrame();
		CC_CONTINUE_IF(rect.intersectsRect(cellRect));

		CAIndexPath3E r = itr->first;
		m_pFreedCollectionCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetCollectionViewCell();
		itr->second = NULL;
	}
}

void CACollectionView::loadCollectionCell()
{
	CCRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

	std::map<CAIndexPath3E, CACollectionViewCell*>::iterator itr;
	for (itr = m_pUsedCollectionCells.begin(); itr != m_pUsedCollectionCells.end(); itr++)
	{
		CC_CONTINUE_IF(itr->second != NULL);

		CAIndexPath3E r = itr->first;
		CCRect cellRect = m_rUsedCollectionCellRects[r];
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

			if (m_pSelectedCollectionCells.count(r))
			{
				cell->setControlStateSelected();
			}
		}
	}
}

void CACollectionView::updateSectionHeaderAndFooterRects()
{
	CCRect rect = this->getBounds();
	rect.origin = getContentOffset();

	std::vector<CCRect>::iterator itr;
	for (itr = m_rSectionRects.begin(); itr != m_rSectionRects.end(); itr++)
	{
		CC_CONTINUE_IF(!rect.intersectsRect(*itr));
		int i = itr - m_rSectionRects.begin();
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
			CCPoint p1 = rect.origin;
			p1.y = MAX(p1.y, itr->origin.y);
			p1.y = MIN(p1.y, itr->origin.y + itr->size.height
				- headerHeight - footerHeight);
			header->setFrameOrigin(p1);
		}
		if (footer && m_bAlwaysBottomSectionFooter)
		{
			CCPoint p2 = CCPointZero;
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
}


CACollectionViewCell::~CACollectionViewCell()
{
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
	this->setBackgroundView(CAView::create());
	this->setColor(CAColor_clear);
	this->setReuseIdentifier(reuseIdentifier);
	this->normalCollectionViewCell();

	return true;
}

bool CACollectionViewCell::initWithReuseIdentifier(const char* reuseIdentifier)
{
	this->setBackgroundView(CAView::create());
	this->setColor(CAColor_clear);
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

void CACollectionViewCell::setContentSize(const CrossApp::CCSize &var)
{
	CAView::setContentSize(var);
	if (m_pBackgroundView)
	{
		m_pBackgroundView->setFrame(this->getBounds());
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
}

NS_CC_END