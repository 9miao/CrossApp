#include "CACollectionView.h"
#include "basics/CAApplication.h"
#include "control/CAButton.h"
#include "basics/CAPointExtension.h"
#include "dispatcher/CATouch.h"
#include "animation/CAViewAnimation.h"

NS_CC_BEGIN

#pragma CACollectionView

CACollectionView::CACollectionView()
: m_pCollectionViewDataSource(nullptr)
, m_pCollectionViewDelegate(nullptr)
, m_pCollectionHeaderView(nullptr)
, m_pCollectionFooterView(nullptr)
, m_bAllowsSelection(false)
, m_bAllowsMultipleSelection(false)
, m_pHighlightedCollectionCells(nullptr)
, m_nCollectionHeaderHeight(0)
, m_nCollectionFooterHeight(0)
, m_nHoriInterval(0)
, m_nVertInterval(0)
, m_bAlwaysTopSectionHeader(true)
, m_bAlwaysBottomSectionFooter(true)
, m_bLoadData(true)
{
    
}


CACollectionView::~CACollectionView()
{
	CC_SAFE_RELEASE_NULL(m_pCollectionHeaderView);
	CC_SAFE_RELEASE_NULL(m_pCollectionFooterView);
	m_pCollectionViewDataSource = nullptr;
	m_pCollectionViewDelegate = nullptr;
}

void CACollectionView::onEnterTransitionDidFinish()
{
	CAScrollView::onEnterTransitionDidFinish();
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
	return nullptr;
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
	return nullptr;
}

CACollectionView* CACollectionView::createWithLayout(const CrossApp::DLayout &layout)
{
    CACollectionView* collectionView = new CACollectionView();
    if (collectionView && collectionView->initWithLayout(layout))
    {
        collectionView->autorelease();
        return collectionView;
    }
    CC_SAFE_DELETE(collectionView);
    return nullptr;
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

void CACollectionView::setContentSize(const CrossApp::DSize &var)
{
    CAScrollView::setContentSize(var);
    
    if (!m_mpUsedCollectionCells.empty())
    {
        this->reloadData();
    }
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
			cell->setControlState(CAControl::State::Normal);
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
			cell->setControlState(CAControl::State::Normal);
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
				cell->setControlState(CAControl::State::Normal);
			}
		}
		m_pSelectedCollectionCells.clear();
	}

	CAIndexPath3E indexPath = CAIndexPath3E(section, row, item);
	if (CACollectionViewCell* cell = m_mpUsedCollectionCells.at(indexPath))
	{
		cell->setControlState(CAControl::State::Selected);
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
		cell->setControlState(CAControl::State::Normal);
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
    CAIndexPath3E indexPath = CAIndexPath3E(section, row, item);
    if (m_mpUsedCollectionCells.find(indexPath) == m_mpUsedCollectionCells.end())
    {
        return nullptr;
    }
    return m_mpUsedCollectionCells[indexPath];
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

	if (m_pContainer->isTouchEnabled() && m_bAllowsSelection && this->isScrollWindowNotOutSide() == false && isInertia)
	{
		DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);

        for (auto& pair : m_mpUsedCollectionCells)
		{
			CACollectionViewCell* cell = pair.second;
			CC_CONTINUE_IF(cell == nullptr);

			if (cell->getFrame().containsPoint(point) && cell->isVisible())
			{
				CC_BREAK_IF(cell->getControlState() == CAControl::State::Disabled);

                if (m_pHighlightedCollectionCells != cell)
                {
                    if (m_pHighlightedCollectionCells)
                    {
                        m_pHighlightedCollectionCells->setControlState(CAControl::State::Normal);
                    }
                    
                    m_pHighlightedCollectionCells = cell;
                }

				CC_BREAK_IF(cell->getControlState() == CAControl::State::Selected);

                CAViewAnimation::beginAnimations(m_s__StrID);
                CAViewAnimation::setAnimationDuration(0.05f);
                CAViewAnimation::setAnimationDidStopSelector(std::bind(&CACollectionViewCell::setControlStateHighlighted, cell));
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

		if (m_pHighlightedCollectionCells->getControlState() == CAControl::State::Highlighted)
		{
			m_pHighlightedCollectionCells->setControlState(CAControl::State::Normal);
		}

		m_pHighlightedCollectionCells = nullptr;
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
				cell->setControlState(CAControl::State::Normal);
			}
            if (m_obDidDeselectCellAtIndexPath)
            {
                m_obDidDeselectCellAtIndexPath(deselectedIndexPath.section, deselectedIndexPath.row, deselectedIndexPath.item);
            }
            else if (m_pCollectionViewDelegate)
			{
				m_pCollectionViewDelegate->collectionViewDidDeselectCellAtIndexPath(this,
					deselectedIndexPath.section, deselectedIndexPath.row, deselectedIndexPath.item);
			}
		}

		if (selectedIndexPath != CAIndexPath3EZero)
		{
			if (CACollectionViewCell* cell = m_mpUsedCollectionCells[selectedIndexPath])
			{
				cell->setControlState(CAControl::State::Selected);
			}
            if (m_obDidSelectCellAtIndexPath)
            {
                m_obDidSelectCellAtIndexPath(selectedIndexPath.section, selectedIndexPath.row, selectedIndexPath.item);
            }
			else if (m_pCollectionViewDelegate)
			{
				m_pCollectionViewDelegate->collectionViewDidSelectCellAtIndexPath(this,
					selectedIndexPath.section, selectedIndexPath.row, selectedIndexPath.item);
			}
		}
        
        m_pHighlightedCollectionCells = nullptr;
	}
}

void CACollectionView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchCancelled(pTouch, pEvent);

	if (m_pHighlightedCollectionCells)
	{
        CAViewAnimation::removeAnimations(m_s__StrID);

        if (m_pHighlightedCollectionCells->getControlState() == CAControl::State::Highlighted)
        {
            m_pHighlightedCollectionCells->setControlState(CAControl::State::Normal);
        }
		m_pHighlightedCollectionCells = nullptr;
	}
}

void CACollectionView::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
{
    if (m_bAllowsSelection)
    {
        DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);
        
        for (auto& pair : m_mpUsedCollectionCells)
        {
            CACollectionViewCell* cell = pair.second;
            CC_CONTINUE_IF(cell == nullptr);
            if (cell->getFrame().containsPoint(point) && cell->isVisible())
            {
                CC_BREAK_IF(cell->getControlState() == CAControl::State::Disabled);
                
                if (m_pHighlightedCollectionCells)
                {
                    CAIndexPath3E index = CAIndexPath3E(m_pHighlightedCollectionCells->getSection(),
                                                        m_pHighlightedCollectionCells->getRow(),
                                                        m_pHighlightedCollectionCells->getItem());
                    if (m_pSelectedCollectionCells.count(index))
                    {
                        m_pHighlightedCollectionCells->setControlState(CAControl::State::Highlighted);
                    }
                    else
                    {
                        m_pHighlightedCollectionCells->setControlState(CAControl::State::Normal);
                    }
                    
                }
                
                m_pHighlightedCollectionCells = cell;
                cell->setControlState(CAControl::State::Highlighted);
                
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
            m_pHighlightedCollectionCells->setControlState(CAControl::State::Selected);
        }
        else
        {
            m_pHighlightedCollectionCells->setControlState(CAControl::State::Normal);
        }
        m_pHighlightedCollectionCells = nullptr;
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

    m_nSections = 1;
    if (m_obNumberOfSections)
    {
        m_nSections = m_obNumberOfSections();
    }
    else if (m_pCollectionViewDataSource)
    {
        m_nSections = m_pCollectionViewDataSource->numberOfSections(this);
    }
    
    m_nRowsInSections.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        unsigned int rowsInSection = 0;
        if (m_obNumberOfRowsAtIndexPath)
        {
            rowsInSection = m_obNumberOfRowsAtIndexPath(i);
        }
        else if (m_pCollectionViewDataSource)
        {
            rowsInSection = m_pCollectionViewDataSource->numberOfRowsInSection(this, i);
        }
        m_nRowsInSections[i] = rowsInSection;
    }
    
    m_nSectionHeaderHeights.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        unsigned int sectionHeaderHeight = 0;
        if (m_obHeightForHeaderInSection)
        {
            sectionHeaderHeight = m_obHeightForHeaderInSection(i);
        }
        else if (m_pCollectionViewDataSource)
        {
            sectionHeaderHeight = m_pCollectionViewDataSource->collectionViewHeightForHeaderInSection(this, i);
        }
        m_nSectionHeaderHeights[i] = sectionHeaderHeight;
    }
    
    m_nSectionFooterHeights.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        unsigned int sectionFooterHeight = 0;
        if (m_obHeightForFooterInSection)
        {
            sectionFooterHeight = m_obHeightForFooterInSection(i);
        }
        else if (m_pCollectionViewDataSource)
        {
            sectionFooterHeight = m_pCollectionViewDataSource->collectionViewHeightForFooterInSection(this, i);
        }
        m_nSectionFooterHeights[i] = sectionFooterHeight;
    }
    
    m_nRowHeightss.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        std::vector<unsigned int> rowHeights(m_nRowsInSections.at(i));
        for (unsigned int j=0; j<m_nRowsInSections.at(i); j++)
        {
            unsigned int rowHeight = 0;
            if (m_obCellHeightAtIndexPath)
            {
                rowHeight = m_obCellHeightAtIndexPath(i, j);
            }
            else if (m_pCollectionViewDataSource)
            {
                rowHeight = m_pCollectionViewDataSource->collectionViewHeightForRowAtIndexPath(this, i, j);

            }
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
	m_pSectionHeaderViews.clear();
	m_pSectionFooterViews.clear();
	m_rSectionRects.clear();

	for (int i = 0; i < m_vpUsedCollectionCells.size(); i++)
	{
		CACollectionViewCell* cell = m_vpUsedCollectionCells.at(i);
		CC_CONTINUE_IF(cell == nullptr);
		m_mpFreedCollectionCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetCell();
	}
	m_vpUsedCollectionCells.clear();

	m_pHighlightedCollectionCells = nullptr;
}

void CACollectionView::reloadData()
{    
    m_bLoadData = true;
    this->updateDraw();
}

void CACollectionView::recoveryCollectionCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

    for (auto& pair : m_mpUsedCollectionCells)
	{
		CACollectionViewCell* cell = pair.second;
		CC_CONTINUE_IF(cell == nullptr);

		DRect cellRect = cell->getFrame();
		CC_CONTINUE_IF(rect.intersectsRect(cellRect));

		m_mpFreedCollectionCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetCell();
		pair.second = nullptr;
        m_vpUsedCollectionCells.eraseObject(cell);
	}
}

void CACollectionView::loadCollectionCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

	for (auto& pair : m_mpUsedCollectionCells)
	{
		CC_CONTINUE_IF(pair.second != nullptr);

		CAIndexPath3E r = pair.first;
		DRect cellRect = m_rUsedCollectionCellRects[r];
		CC_CONTINUE_IF(!rect.intersectsRect(cellRect));

        CACollectionViewCell* cell = nullptr;
        if (m_obCellAtIndexPath)
        {
            cell = m_obCellAtIndexPath(cellRect.size, r.section, r.row, r.item);
        }
        else if (m_pCollectionViewDataSource)
        {
            cell = m_pCollectionViewDataSource->collectionCellAtIndex(this, cellRect.size, r.section, r.row, r.item);
        }
        
		if (cell)
		{
			cell->m_nSection = r.section;
			cell->m_nRow = r.row;
			cell->m_nItem = r.item;
            cell->updateDisplayedAlpha(_realAlpha);
			this->addSubview(cell);
			cell->setFrame(cellRect);
			pair.second = cell;
            m_vpUsedCollectionCells.pushBack(cell);
            
			if (m_pSelectedCollectionCells.count(r))
			{
				cell->setControlState(CAControl::State::Selected);
			}
            if (m_obWillDisplayCellAtIndexPath)
            {
                m_obWillDisplayCellAtIndexPath(cell, r.section, r.row, r.item);
            }
            else if (m_pCollectionViewDataSource)
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

    int i = 0;
    for (auto& r : m_rSectionRects)
	{
		CC_CONTINUE_IF(!rect.intersectsRect(r));
		CAView* header = nullptr;
		CAView* footer = nullptr;
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
			p1.y = MAX(p1.y, r.origin.y);
			p1.y = MIN(p1.y, r.origin.y + r.size.height
				- headerHeight - footerHeight);
			header->setFrameOrigin(p1);
		}
		if (footer && m_bAlwaysBottomSectionFooter)
		{
			DPoint p2 = DPointZero;
			p2.y = MIN(rect.origin.y + this->getBounds().size.height - footerHeight,
				r.origin.y + r.size.height - footerHeight);
			p2.y = MAX(p2.y, r.origin.y + headerHeight);
			footer->setFrameOrigin(p2);
		}
        ++i;
	}
}

void CACollectionView::update(float dt)
{
	CAScrollView::update(dt);
    
	recoveryCollectionCell();
    
	loadCollectionCell();
    
	updateSectionHeaderAndFooterRects();
}

void CACollectionView::visitEve()
{
    if (m_bLoadData)
    {
        m_bLoadData = false;
        
        this->reloadViewSizeData();
        
        this->removeAllSubviews();
        
        DRect winRect = this->getBounds();
        winRect.origin = getContentOffset();
        float width = winRect.size.width;
        int y = 0;
        
        if (m_nCollectionHeaderHeight > 0 && m_pCollectionHeaderView)
        {
            m_pCollectionHeaderView->setFrame(DRect(0, y, width, m_nCollectionHeaderHeight));
            this->addSubview(m_pCollectionHeaderView);
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
                CAView* pSectionHeaderView = nullptr;
                if (m_obSectionViewForHeaderInSection)
                {
                    pSectionHeaderView = m_obSectionViewForHeaderInSection(sectionHeaderRect.size, i);
                }
                else if (m_pCollectionViewDataSource)
                {
                    pSectionHeaderView = m_pCollectionViewDataSource->collectionViewSectionViewForHeaderInSection(this, sectionHeaderRect.size, i);
                }
                
                if (pSectionHeaderView != nullptr)
                {
                    pSectionHeaderView->setDisplayRange(true);
                    pSectionHeaderView->setFrame(sectionHeaderRect);
                    this->insertSubview(pSectionHeaderView, 1);
                    m_pSectionHeaderViews[i] = pSectionHeaderView;
                    y += iSectionHeaderHeight;
                }
            }
            
            y += m_nVertInterval;
            unsigned int rowCount = m_nRowsInSections.at(i);
            for (int j = 0; j < rowCount; j++)
            {
                int iHeight = m_nRowHeightss.at(i).at(j);
                
                unsigned int itemCount = 0;
                if (m_obNumberOfItemsAtIndexPath)
                {
                    itemCount = m_obNumberOfItemsAtIndexPath(i, j);
                }
                else if (m_pCollectionViewDataSource)
                {
                    itemCount = m_pCollectionViewDataSource->numberOfItemsInRowsInSection(this, i, j);
                }
                
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
                    m_mpUsedCollectionCells.insert(std::make_pair(indexPath, (CACollectionViewCell*)nullptr));
                    
                    CC_CONTINUE_IF(!winRect.intersectsRect(cellRect));
                    
                    CACollectionViewCell* cell = nullptr;
                    if (m_obCellAtIndexPath)
                    {
                        cell = m_obCellAtIndexPath(cellRect.size, i, j, k);
                    }
                    else if (m_pCollectionViewDataSource)
                    {
                        cell = m_pCollectionViewDataSource->collectionCellAtIndex(this, cellRect.size, i, j, k);
                    }
                    if (cell)
                    {
                        this->addSubview(cell);
                        cell->setFrame(cellRect);
                        cell->m_nSection = i;
                        cell->m_nRow = j;
                        cell->m_nItem = k;
                        itrResult.first->second = cell;
                        m_vpUsedCollectionCells.pushBack(cell);
                        
                        if (m_obWillDisplayCellAtIndexPath)
                        {
                            m_obWillDisplayCellAtIndexPath(cell, i, j, k);
                        }
                        else if (m_pCollectionViewDataSource)
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
                CAView* pSectionFooterView = nullptr;
                if (m_obSectionViewForFooterInSection)
                {
                    pSectionFooterView = m_obSectionViewForFooterInSection(sectionFooterRect.size, i);
                }
                else if (m_pCollectionViewDataSource)
                {
                    pSectionFooterView = m_pCollectionViewDataSource->collectionViewSectionViewForFooterInSection(this, sectionFooterRect.size, i);
                }
                if (pSectionFooterView != nullptr)
                {
                    pSectionFooterView->setDisplayRange(true);
                    pSectionFooterView->setFrame(sectionFooterRect);
                    this->insertSubview(pSectionFooterView, 1);
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
        
        if (m_nCollectionFooterHeight > 0 && m_pCollectionFooterView)
        {
            m_pCollectionFooterView->setFrame(DRect(0, y, width, m_nCollectionFooterHeight));
            this->addSubview(m_pCollectionFooterView);
            y += m_nCollectionFooterHeight;
        }
        
        this->updateSectionHeaderAndFooterRects();
        this->layoutPullToRefreshView();
        
        if (this->isScrollWindowNotOutSide())
        {
            this->startDeaccelerateScroll();
        }
    }
    CAScrollView::visitEve();
}

float CACollectionView::maxSpeed(float dt)
{
    return (128 * 60 * dt);
}

float CACollectionView::decelerationRatio(float dt)
{
    return 1.8f * dt;
}

CACollectionViewCell* CACollectionView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
{
    CACollectionViewCell* cell = nullptr;
    
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
: m_nSection(UINT_NONE)
, m_nRow(UINT_NONE)
, m_nItem(UINT_NONE)
{

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
	return nullptr;
}

void CACollectionViewCell::normalCell()
{
	CC_RETURN_IF(m_pBackgroundView == nullptr);
	m_pBackgroundView->setColor(CAColor4B(255, 255, 255, 255));
}

void CACollectionViewCell::highlightedCell()
{
	CC_RETURN_IF(m_pBackgroundView == nullptr);
	m_pBackgroundView->setColor(CAColor4B(240, 240, 240, 255));
}


void CACollectionViewCell::selectedCell()
{
	CC_RETURN_IF(m_pBackgroundView == nullptr);
	m_pBackgroundView->setColor(CAColor4B(50, 193, 255, 255));
}


void CACollectionViewCell::disabledCell()
{
	CC_RETURN_IF(m_pBackgroundView == nullptr);
	m_pBackgroundView->setColor(CAColor4B(127, 127, 127, 255));
}

NS_CC_END
