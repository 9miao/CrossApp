#include "CAWaterfallView.h"
#include "basics/CAApplication.h"
#include "control/CAButton.h"
#include "basics/CAPointExtension.h"
#include "dispatcher/CATouch.h"
#include "animation/CAViewAnimation.h"

NS_CC_BEGIN

CAWaterfallView::CAWaterfallView()
: m_pWaterfallViewDataSource(nullptr)
, m_pWaterfallViewDelegate(nullptr)
, m_pWaterfallHeaderView(nullptr)
, m_pWaterfallFooterView(nullptr)
, m_bAllowsSelection(false)
, m_bAllowsMultipleSelection(false)
, m_pHighlightedWaterfallCells(nullptr)
, m_nWaterfallHeaderHeight(0)
, m_nWaterfallFooterHeight(0)
, m_nColumnCount(2)
, m_nItemMargin(4)
, m_nColumnMargin(4)
, m_bAlwaysTopSectionHeader(true)
, m_bAlwaysBottomSectionFooter(true)
, m_bLoadData(true)
{
}


CAWaterfallView::~CAWaterfallView()
{
    m_mpUsedWaterfallCells.clear();
	CC_SAFE_RELEASE_NULL(m_pWaterfallHeaderView);
	CC_SAFE_RELEASE_NULL(m_pWaterfallFooterView);
	m_pWaterfallViewDataSource = nullptr;
	m_pWaterfallViewDelegate = nullptr;
}

void CAWaterfallView::onEnterTransitionDidFinish()
{
	CAScrollView::onEnterTransitionDidFinish();
}

void CAWaterfallView::onExitTransitionDidStart()
{
	CAScrollView::onExitTransitionDidStart();
}

CAWaterfallView* CAWaterfallView::createWithFrame(const DRect& rect)
{
	CAWaterfallView* pWaterfallView = new CAWaterfallView();
	if (pWaterfallView && pWaterfallView->initWithFrame(rect))
	{
		pWaterfallView->autorelease();
		return pWaterfallView;
	}
	CC_SAFE_DELETE(pWaterfallView);
	return nullptr;
}

CAWaterfallView* CAWaterfallView::createWithCenter(const DRect& rect)
{
	CAWaterfallView* pWaterfallView = new CAWaterfallView();
	if (pWaterfallView && pWaterfallView->initWithCenter(rect))
	{
		pWaterfallView->autorelease();
		return pWaterfallView;
	}
	CC_SAFE_DELETE(pWaterfallView);
	return nullptr;
}

CAWaterfallView* CAWaterfallView::createWithLayout(const CrossApp::DLayout &layout)
{
    CAWaterfallView* pWaterfallView = new CAWaterfallView();
    if (pWaterfallView && pWaterfallView->initWithLayout(layout))
    {
        pWaterfallView->autorelease();
        return pWaterfallView;
    }
    CC_SAFE_DELETE(pWaterfallView);
    return nullptr;
}

bool CAWaterfallView::init()
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

void CAWaterfallView::setContentSize(const CrossApp::DSize &var)
{
    CAScrollView::setContentSize(var);
    
    if (!m_mpUsedWaterfallCells.empty())
    {
        this->reloadData();
    }
}

void CAWaterfallView::setAllowsSelection(bool var)
{
	m_bAllowsSelection = var;
	CC_RETURN_IF(!m_bAllowsSelection);
	CC_RETURN_IF(m_pSelectedWaterfallCells.empty());
	std::set<int>::iterator itr;
	for (itr = m_pSelectedWaterfallCells.begin(); itr != m_pSelectedWaterfallCells.end(); itr++)
	{
		if (CAWaterfallViewCell* cell = m_mpUsedWaterfallCells[(*itr)])
		{
			cell->setControlState(CAControl::State::Normal);
		}
	}
	m_pSelectedWaterfallCells.clear();
}

void CAWaterfallView::setAllowsMultipleSelection(bool var)
{
	m_bAllowsMultipleSelection = var;
	CC_RETURN_IF(m_pSelectedWaterfallCells.empty());
	std::set<int>::iterator itr;
	for (itr = m_pSelectedWaterfallCells.begin(); itr != m_pSelectedWaterfallCells.end(); itr++)
	{
		if (CAWaterfallViewCell* cell = m_mpUsedWaterfallCells[(*itr)])
		{
			cell->setControlState(CAControl::State::Normal);
		}
	}
	m_pSelectedWaterfallCells.clear();
}

void CAWaterfallView::setSelectRowAtIndexPath(unsigned int itemIndex)
{
	if (!m_pSelectedWaterfallCells.empty() && m_bAllowsMultipleSelection == false)
	{
		std::set<int>::iterator itr;
		for (itr = m_pSelectedWaterfallCells.begin(); itr != m_pSelectedWaterfallCells.end(); itr++)
		{
			if (CAWaterfallViewCell* cell = m_mpUsedWaterfallCells[(*itr)])
			{
				cell->setControlState(CAControl::State::Normal);
			}
		}
		m_pSelectedWaterfallCells.clear();
	}

	if (CAWaterfallViewCell* cell = m_mpUsedWaterfallCells.at(itemIndex))
	{
		cell->setControlState(CAControl::State::Selected);
	}
	m_pSelectedWaterfallCells.insert(itemIndex);
}

void CAWaterfallView::setUnSelectRowAtIndexPath(unsigned int itemIndex)
{
	CC_RETURN_IF(m_pSelectedWaterfallCells.find(itemIndex) == m_pSelectedWaterfallCells.end());
	if (CAWaterfallViewCell* cell = m_mpUsedWaterfallCells.at(itemIndex))
	{
		cell->setControlState(CAControl::State::Normal);
	}
	m_pSelectedWaterfallCells.erase(itemIndex);
}

void CAWaterfallView::setShowsScrollIndicators(bool var)
{
	this->setShowsVerticalScrollIndicator(var);
	m_bShowsScrollIndicators = var;
}

CAWaterfallViewCell* CAWaterfallView::cellForRowAtIndexPath(unsigned int itemIndex)
{
    if (m_mpUsedWaterfallCells.find(itemIndex) == m_mpUsedWaterfallCells.end())
    {
        return nullptr;
    }
	return m_mpUsedWaterfallCells[itemIndex];
}

const CAVector<CAWaterfallViewCell*>& CAWaterfallView::displayingWaterfallCell()
{
	return m_vpUsedWaterfallCells;
}

bool CAWaterfallView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	bool isInertia = m_tInertia.getLength() < 1.0f;
	if (!CAScrollView::ccTouchBegan(pTouch, pEvent))
		return false;

	if (m_pContainer->isTouchEnabled() && m_bAllowsSelection && this->isScrollWindowNotOutSide() == false && isInertia)
	{
		DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);
		
        for (auto& pair : m_mpUsedWaterfallCells)
		{
			CAWaterfallViewCell* cell = pair.second;
			CC_CONTINUE_IF(cell == nullptr);

			if (cell->getFrame().containsPoint(point) && cell->isVisible())
			{
				CC_BREAK_IF(cell->getControlState() == CAControl::State::Disabled);

				if (m_pHighlightedWaterfallCells != cell)
				{
					if (m_pHighlightedWaterfallCells)
					{
						m_pHighlightedWaterfallCells->setControlState(CAControl::State::Normal);
					}

					m_pHighlightedWaterfallCells = cell;
				}

				CC_BREAK_IF(cell->getControlState() == CAControl::State::Selected);

				CAViewAnimation::beginAnimations(m_s__StrID);
				CAViewAnimation::setAnimationDuration(0.05f);
                CAViewAnimation::setAnimationDidStopSelector(std::bind(&CAWaterfallViewCell::setControlStateHighlighted, cell));
				CAViewAnimation::commitAnimations();
				break;
			}
		}
	}
	return true;
}

void CAWaterfallView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchMoved(pTouch, pEvent);

	if (m_pHighlightedWaterfallCells)
	{
		CAViewAnimation::removeAnimations(m_s__StrID);

		if (m_pHighlightedWaterfallCells->getControlState() == CAControl::State::Highlighted)
		{
			m_pHighlightedWaterfallCells->setControlState(CAControl::State::Normal);
		}

		m_pHighlightedWaterfallCells = nullptr;
	}

}

void CAWaterfallView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
	CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchEnded(pTouch, pEvent);

	if (m_pHighlightedWaterfallCells)
	{
		CAViewAnimation::removeAnimations(m_s__StrID);

		int deselectedIndex = -1;
		int selectedIndex = m_pHighlightedWaterfallCells->getItem();

		if (m_pSelectedWaterfallCells.count(selectedIndex) > 0 && m_bAllowsMultipleSelection)
		{
			deselectedIndex = selectedIndex;
			selectedIndex = -1;
			m_pSelectedWaterfallCells.erase(deselectedIndex);
		}
		else
		{
			if (m_pSelectedWaterfallCells.size() > 0 && m_bAllowsMultipleSelection == false)
			{
				deselectedIndex = *m_pSelectedWaterfallCells.begin();
				m_pSelectedWaterfallCells.clear();
			}
			m_pSelectedWaterfallCells.insert(selectedIndex);
		}

		if (deselectedIndex != -1)
		{
			if (CAWaterfallViewCell* cell = m_mpUsedWaterfallCells[deselectedIndex])
			{
				cell->setControlState(CAControl::State::Normal);
			}
            if (m_obDidDeselectCellAtIndexPath)
            {
                m_obDidDeselectCellAtIndexPath(deselectedIndex);
            }
            else if (m_pWaterfallViewDelegate)
			{
				m_pWaterfallViewDelegate->waterfallViewDidDeselectCellAtIndexPath(this, deselectedIndex);
			}
		}

		if (selectedIndex != -1)
		{
			if (CAWaterfallViewCell* cell = m_mpUsedWaterfallCells[selectedIndex])
			{
				cell->setControlState(CAControl::State::Selected);
			}
            if (m_obDidSelectCellAtIndexPath)
            {
                m_obDidSelectCellAtIndexPath(selectedIndex);
            }
			else if (m_pWaterfallViewDelegate)
			{
				m_pWaterfallViewDelegate->waterfallViewDidSelectCellAtIndexPath(this, selectedIndex);
			}
		}

		m_pHighlightedWaterfallCells = nullptr;
	}
}

void CAWaterfallView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
	CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchCancelled(pTouch, pEvent);

	if (m_pHighlightedWaterfallCells)
	{
		CAViewAnimation::removeAnimations(m_s__StrID);

		if (m_pHighlightedWaterfallCells->getControlState() == CAControl::State::Highlighted)
		{
			m_pHighlightedWaterfallCells->setControlState(CAControl::State::Normal);
		}
		m_pHighlightedWaterfallCells = nullptr;
	}
}

void CAWaterfallView::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
{
	if (m_bAllowsSelection)
	{
		DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);
		
        for (auto& pair : m_mpUsedWaterfallCells)
		{
			CAWaterfallViewCell* cell = pair.second;
			CC_CONTINUE_IF(cell == nullptr);
			if (cell->getFrame().containsPoint(point) && cell->isVisible())
			{
				CC_BREAK_IF(cell->getControlState() == CAControl::State::Disabled);

				if (m_pHighlightedWaterfallCells)
				{
					int index = m_pHighlightedWaterfallCells->getItem();
					if (m_pSelectedWaterfallCells.count(index))
					{
						m_pHighlightedWaterfallCells->setControlState(CAControl::State::Highlighted);
					}
					else
					{
						m_pHighlightedWaterfallCells->setControlState(CAControl::State::Normal);
					}
				}

				m_pHighlightedWaterfallCells = cell;
				cell->setControlState(CAControl::State::Highlighted);

				break;
			}
		}
	}
}

void CAWaterfallView::mouseMovedOutSide(CATouch* pTouch, CAEvent* pEvent)
{
	if (m_pHighlightedWaterfallCells)
	{
		int index = m_pHighlightedWaterfallCells->getItem();
		if (m_pSelectedWaterfallCells.count(index))
		{
			m_pHighlightedWaterfallCells->setControlState(CAControl::State::Selected);
		}
		else
		{
			m_pHighlightedWaterfallCells->setControlState(CAControl::State::Normal);
		}
		m_pHighlightedWaterfallCells = nullptr;
	}
}

void CAWaterfallView::switchPCMode(bool var)
{
	CAScrollView::switchPCMode(var);
	this->setMouseMovedEnabled(true);
}

void CAWaterfallView::reloadViewSizeData()
{
	clearData();

	DSize size = this->getBounds().size;
	unsigned int nColumnWidth = (size.width - (m_nColumnCount + 1) * m_nColumnMargin) / m_nColumnCount;
	
	unsigned int viewHeight = 0;

    int nItemCount = 0;
    if (m_obNumberOfItemsAtIndexPath)
    {
        nItemCount = m_obNumberOfItemsAtIndexPath();
    }
    else if (m_pWaterfallViewDataSource)
    {
        nItemCount = m_pWaterfallViewDataSource->numberOfItems(this);
    }
    
	for (int i = 0; i < nItemCount; i++)
	{
        unsigned int nColumnHeight = 0;
        if (m_obCellHeightAtIndexPath)
        {
            nColumnHeight = m_obCellHeightAtIndexPath(i);
        }
        else if (m_pWaterfallViewDataSource)
        {
            nColumnHeight = m_pWaterfallViewDataSource->waterfallViewHeightForItemAtIndex(this, i);
        }

		unsigned int index = getCurColumnIndex();

		if (m_nColumnHeightVect[index] != 0)
		{
			m_nColumnHeightVect[index] += m_nItemMargin;
		}

		int x = index*(nColumnWidth + m_nColumnMargin) + m_nColumnMargin;
		int y = m_nColumnHeightVect[index];

		m_rUsedWaterfallCellRects[i] = DRect(x, y + viewHeight, nColumnWidth, nColumnHeight);
		m_mpUsedWaterfallCells[i] = nullptr;

		m_nColumnHeightVect[index] += nColumnHeight;
	}
	viewHeight += getMaxColumnValue();

	size.height = viewHeight;
	this->setViewSize(size);
}

void CAWaterfallView::clearData()
{
    m_mpUsedWaterfallCells.clear();
    
    for (auto& cell : m_vpUsedWaterfallCells)
	{
		CC_CONTINUE_IF(cell == nullptr);
		m_mpFreedWaterfallCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetCell();
	}
	m_vpUsedWaterfallCells.clear();
    
    m_rUsedWaterfallCellRects.clear();
    m_nColumnHeightVect.clear();
    
	m_pHighlightedWaterfallCells = nullptr;
}

unsigned int CAWaterfallView::getCurColumnIndex()
{
	if (m_nColumnHeightVect.empty())
	{
		for (int i = 0; i < m_nColumnCount; i++)
		{
			m_nColumnHeightVect.push_back(0);
		}
	}
	
	unsigned int iMinValue = 0xffffff;
	unsigned int iColumnEx = 0;
	for (int i = 0; i < m_nColumnCount; i++)
	{
		if (m_nColumnHeightVect[i] < iMinValue)
		{
			iColumnEx = i;
			iMinValue = m_nColumnHeightVect[i];
		}
	}
	return iColumnEx;
}

unsigned int CAWaterfallView::getMaxColumnValue()
{
	unsigned int nColumnHeightMax = 0;
	for (int i = 0; i < m_nColumnHeightVect.size(); i++)
	{
		if (m_nColumnHeightVect[i] > nColumnHeightMax)
		{
			nColumnHeightMax = m_nColumnHeightVect[i];
		}
	}
	return nColumnHeightMax;
}

void CAWaterfallView::reloadData()
{
    m_bLoadData = true;
    this->updateDraw();
}

void CAWaterfallView::recoveryWaterfallCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

    for (auto& pair : m_mpUsedWaterfallCells)
	{
		CAWaterfallViewCell* cell = pair.second;
		CC_CONTINUE_IF(cell == nullptr);

		DRect cellRect = cell->getFrame();
		CC_CONTINUE_IF(rect.intersectsRect(cellRect));

		m_mpFreedWaterfallCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetCell();
		pair.second = nullptr;
		m_vpUsedWaterfallCells.eraseObject(cell);
	}
}

void CAWaterfallView::loadWaterfallCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

	for (auto& pair : m_mpUsedWaterfallCells)
	{
		CC_CONTINUE_IF(pair.second != nullptr);

		int index = pair.first;
		DRect cellRect = m_rUsedWaterfallCellRects[index];
		CC_CONTINUE_IF(!rect.intersectsRect(cellRect));

        CAWaterfallViewCell* cell = nullptr;
        if (m_obCellAtIndexPath)
        {
            cell = m_obCellAtIndexPath(cellRect.size, index);
        }
        else if (m_pWaterfallViewDataSource)
        {
            cell = m_pWaterfallViewDataSource->waterfallCellAtIndex(this, cellRect.size, index);
        }
        
		if (cell)
		{
			cell->m_nItem = index;
            cell->updateDisplayedAlpha(_realAlpha);
            cell->setFrame(cellRect);
			this->addSubview(cell);
            m_mpUsedWaterfallCells[index] = cell;
			m_vpUsedWaterfallCells.pushBack(cell);

			if (m_pSelectedWaterfallCells.count(index))
			{
				cell->setControlState(CAControl::State::Selected);
			}

            if (m_obWillDisplayCellAtIndexPath)
            {
                m_obWillDisplayCellAtIndexPath(cell, index);
            }
			else if (m_pWaterfallViewDataSource)
			{
				m_pWaterfallViewDataSource->waterfallViewWillDisplayCellAtIndex(this, cell, index);
			}
		}
	}
}

void CAWaterfallView::updateSectionHeaderAndFooterRects()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
	

	CAView* header = m_pWaterfallHeaderView;
	CAView* footer = m_pWaterfallFooterView;

	float headerHeight = m_nWaterfallHeaderHeight;
	float footerHeight = m_nWaterfallFooterHeight;

	DRect rRect;
	rRect.size = this->getViewSize();

	if (header && m_bAlwaysTopSectionHeader)
	{
		DPoint p1 = rect.origin;
		p1.y = MAX(p1.y, rRect.origin.y);
		p1.y = MIN(p1.y, rRect.origin.y + rRect.size.height - headerHeight - footerHeight);
		header->setFrameOrigin(p1);
	}
	if (footer && m_bAlwaysBottomSectionFooter)
	{
		DPoint p2 = DPointZero;
		p2.y = MIN(rect.origin.y + this->getBounds().size.height - footerHeight, rRect.origin.y + rRect.size.height - footerHeight);
		p2.y = MAX(p2.y, rRect.origin.y + headerHeight);
		footer->setFrameOrigin(p2);
	}
}

void CAWaterfallView::update(float dt)
{
	CAScrollView::update(dt);

	recoveryWaterfallCell();

	loadWaterfallCell();

	updateSectionHeaderAndFooterRects();
}

void CAWaterfallView::visitEve()
{
    if (m_bLoadData)
    {
        m_bLoadData = false;
        
        this->reloadViewSizeData();
        
        this->removeAllSubviews();
        
        DRect winRect = this->getBounds();
        winRect.origin = getContentOffset();
        
        if (m_nWaterfallHeaderHeight > 0 && m_pWaterfallHeaderView)
        {
            m_pWaterfallHeaderView->setDisplayRange(true);
            m_pWaterfallHeaderView->setFrame(DRect(0, 0, winRect.size.width, m_nWaterfallHeaderHeight));
            this->addSubview(m_pWaterfallHeaderView);
        }
        
        int dwValue = getMaxColumnValue();
        dwValue += m_nWaterfallHeaderHeight;
        dwValue += 2 * m_nItemMargin;
        
        if (m_nWaterfallFooterHeight > 0 && m_pWaterfallFooterView)
        {
            m_pWaterfallFooterView->setFrame(DRect(0, dwValue, winRect.size.width, m_nWaterfallFooterHeight));
            this->addSubview(m_pWaterfallFooterView);
        }
        
        this->loadWaterfallCell();
        this->updateSectionHeaderAndFooterRects();
        this->layoutPullToRefreshView();
        
        if (this->isScrollWindowNotOutSide())
        {
            this->startDeaccelerateScroll();
        }
    }
    CAScrollView::visitEve();
}

float CAWaterfallView::maxSpeed(float dt)
{
	return (128 * 60 * dt);
}

float CAWaterfallView::decelerationRatio(float dt)
{
	return 1.8f * dt;
}

CAWaterfallViewCell* CAWaterfallView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
{
	CAWaterfallViewCell* cell = nullptr;

	if (reuseIdentifier && !m_mpFreedWaterfallCells[reuseIdentifier].empty())
	{
		cell = m_mpFreedWaterfallCells[reuseIdentifier].back();
		cell->retain()->autorelease();
		m_mpFreedWaterfallCells[reuseIdentifier].popBack();
	}

	return cell;
}

CAWaterfallViewCell* CAWaterfallView::getHighlightWaterfallCell()
{
	return m_pHighlightedWaterfallCells;
}


#pragma CAWaterfallViewCell

CAWaterfallViewCell::CAWaterfallViewCell()
: m_nItem(UINT_NONE)
{

}


CAWaterfallViewCell::~CAWaterfallViewCell()
{

}

CAWaterfallViewCell* CAWaterfallViewCell::create(const std::string& reuseIdentifier)
{
	CAWaterfallViewCell* cell = new CAWaterfallViewCell();
	if (cell && cell->initWithReuseIdentifier(reuseIdentifier))
	{
		cell->autorelease();
		return cell;
	}
	CC_SAFE_DELETE(cell);
	return nullptr;
}

void CAWaterfallViewCell::normalCell()
{
	CC_RETURN_IF(m_pBackgroundView == nullptr);
	m_pBackgroundView->setColor(CAColor4B(255, 255, 255, 255));
}

void CAWaterfallViewCell::highlightedCell()
{
	CC_RETURN_IF(m_pBackgroundView == nullptr);
	m_pBackgroundView->setColor(CAColor4B(240, 240, 240, 255));
}


void CAWaterfallViewCell::selectedCell()
{
	CC_RETURN_IF(m_pBackgroundView == nullptr);
	m_pBackgroundView->setColor(CAColor4B(50, 193, 255, 255));
}


void CAWaterfallViewCell::disabledCell()
{
	CC_RETURN_IF(m_pBackgroundView == nullptr);
	m_pBackgroundView->setColor(CAColor4B(127, 127, 127, 255));
}

NS_CC_END
