#include "CAWaterfallView.h"
#include "basics/CAApplication.h"
#include "control/CAButton.h"
#include "support/CAPointExtension.h"
#include "dispatcher/CATouch.h"
#include "animation/CAViewAnimation.h"

NS_CC_BEGIN

CAWaterfallView::CAWaterfallView()
: m_pWaterfallViewDataSource(NULL)
, m_pWaterfallViewDelegate(NULL)
, m_pWaterfallHeaderView(NULL)
, m_pWaterfallFooterView(NULL)
, m_bAllowsSelection(false)
, m_bAllowsMultipleSelection(false)
, m_pHighlightedWaterfallCells(NULL)
, m_nWaterfallHeaderHeight(0)
, m_nWaterfallFooterHeight(0)
, m_nColumnCount(3)
, m_nItemMargin(4)
, m_nColumnMargin(4)
, m_bAlwaysTopSectionHeader(true)
, m_bAlwaysBottomSectionFooter(true)
{
}


CAWaterfallView::~CAWaterfallView()
{
	CC_SAFE_RELEASE_NULL(m_pWaterfallHeaderView);
	CC_SAFE_RELEASE_NULL(m_pWaterfallFooterView);
	m_pWaterfallViewDataSource = NULL;
	m_pWaterfallViewDelegate = NULL;
}

void CAWaterfallView::onEnterTransitionDidFinish()
{
	CAScrollView::onEnterTransitionDidFinish();

	CAViewAnimation::beginAnimations("", NULL);
	CAViewAnimation::setAnimationDuration(0);
	CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CAWaterfallView::firstReloadData));
	CAViewAnimation::commitAnimations();
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
	return NULL;
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
	return NULL;
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
			cell->setControlState(CAControlStateNormal);
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
			cell->setControlState(CAControlStateNormal);
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
				cell->setControlState(CAControlStateNormal);
			}
		}
		m_pSelectedWaterfallCells.clear();
	}

	if (CAWaterfallViewCell* cell = m_mpUsedWaterfallCells.at(itemIndex))
	{
		cell->setControlStateSelected();
	}
	m_pSelectedWaterfallCells.insert(itemIndex);
}

void CAWaterfallView::setUnSelectRowAtIndexPath(unsigned int itemIndex)
{
	CC_RETURN_IF(m_pSelectedWaterfallCells.find(itemIndex) == m_pSelectedWaterfallCells.end());
	if (CAWaterfallViewCell* cell = m_mpUsedWaterfallCells.at(itemIndex))
	{
		cell->setControlStateNormal();
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

	if (m_bAllowsSelection && this->isScrollWindowNotOutSide() == false && isInertia)
	{
		DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);
		
		std::map<int, CAWaterfallViewCell*>::iterator itr;
		for (itr = m_mpUsedWaterfallCells.begin(); itr != m_mpUsedWaterfallCells.end(); ++itr)
		{
			CAWaterfallViewCell* pCell = itr->second;
			CC_CONTINUE_IF(pCell == NULL);

			if (pCell->getFrame().containsPoint(point) && pCell->isVisible())
			{
				CC_BREAK_IF(pCell->getControlState() == CAControlStateDisabled);

				if (m_pHighlightedWaterfallCells != pCell)
				{
					if (m_pHighlightedWaterfallCells)
					{
						m_pHighlightedWaterfallCells->setControlStateNormal();
					}

					m_pHighlightedWaterfallCells = pCell;
				}

				CC_BREAK_IF(pCell->getControlState() == CAControlStateSelected);

				CAViewAnimation::beginAnimations(m_s__StrID, NULL);
				CAViewAnimation::setAnimationDuration(0.05f);
				CAViewAnimation::setAnimationDidStopSelector(pCell, CAViewAnimation0_selector(CAWaterfallViewCell::setControlStateHighlighted));
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

		if (m_pHighlightedWaterfallCells->getControlState() == CAControlStateHighlighted)
		{
			m_pHighlightedWaterfallCells->setControlStateNormal();
		}

		m_pHighlightedWaterfallCells = NULL;
	}

}

void CAWaterfallView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
	CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchEnded(pTouch, pEvent);

	if (m_pHighlightedWaterfallCells)
	{
		CAViewAnimation::removeAnimations(m_s__StrID);

		int deselectedIndex = 0;
		int selectedIndex = m_pHighlightedWaterfallCells->getItemIndex();

		if (m_pSelectedWaterfallCells.count(selectedIndex) > 0 && m_bAllowsMultipleSelection)
		{
			deselectedIndex = selectedIndex;
			selectedIndex = 0;
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

		if (deselectedIndex != 0)
		{
			if (CAWaterfallViewCell* cell = m_mpUsedWaterfallCells[deselectedIndex])
			{
				cell->setControlStateNormal();
			}
			if (m_pWaterfallViewDelegate)
			{
				m_pWaterfallViewDelegate->waterfallViewDidDeselectCellAtIndexPath(this, deselectedIndex);
			}
		}

		if (selectedIndex != 0)
		{
			if (CAWaterfallViewCell* cell = m_mpUsedWaterfallCells[selectedIndex])
			{
				cell->setControlStateSelected();
			}
			if (m_pWaterfallViewDelegate)
			{
				m_pWaterfallViewDelegate->waterfallViewDidSelectCellAtIndexPath(this, selectedIndex);
			}
		}

		m_pHighlightedWaterfallCells = NULL;
	}
}

void CAWaterfallView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
	CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchCancelled(pTouch, pEvent);

	if (m_pHighlightedWaterfallCells)
	{
		CAViewAnimation::removeAnimations(m_s__StrID);

		if (m_pHighlightedWaterfallCells->getControlState() == CAControlStateHighlighted)
		{
			m_pHighlightedWaterfallCells->setControlStateNormal();
		}
		m_pHighlightedWaterfallCells = NULL;
	}
}

void CAWaterfallView::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
{
	if (m_bAllowsSelection)
	{
		DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);
		
		std::map<int, CAWaterfallViewCell*>::iterator itr;
		for (itr = m_mpUsedWaterfallCells.begin(); itr != m_mpUsedWaterfallCells.end(); ++itr)
		{
			CAWaterfallViewCell* cell = itr->second;
			CC_CONTINUE_IF(cell == NULL);
			if (cell->getFrame().containsPoint(point) && cell->isVisible())
			{
				CC_BREAK_IF(cell->getControlState() == CAControlStateDisabled);

				if (m_pHighlightedWaterfallCells)
				{
					int index = m_pHighlightedWaterfallCells->getItemIndex();
					if (m_pSelectedWaterfallCells.count(index))
					{
						m_pHighlightedWaterfallCells->setControlStateHighlighted();
					}
					else
					{
						m_pHighlightedWaterfallCells->setControlStateNormal();
					}
				}

				m_pHighlightedWaterfallCells = cell;
				cell->setControlStateHighlighted();

				break;
			}
		}
	}
}

void CAWaterfallView::mouseMovedOutSide(CATouch* pTouch, CAEvent* pEvent)
{
	if (m_pHighlightedWaterfallCells)
	{
		int index = m_pHighlightedWaterfallCells->getItemIndex();
		if (m_pSelectedWaterfallCells.count(index))
		{
			m_pHighlightedWaterfallCells->setControlStateSelected();
		}
		else
		{
			m_pHighlightedWaterfallCells->setControlStateNormal();
		}
		m_pHighlightedWaterfallCells = NULL;
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

	int iHeaderHeight = m_pWaterfallViewDataSource->waterfallViewHeightForHeader(this);
	if (iHeaderHeight > 0)
	{
		viewHeight += iHeaderHeight;
		viewHeight += m_nItemMargin;
	}

	int nItemCount = m_pWaterfallViewDataSource->numberOfItems(this);
	for (int i = 0; i < nItemCount; i++)
	{
		unsigned int index = getCurColumnIndex();

		int x = index*(nColumnWidth + m_nColumnMargin) + m_nColumnMargin;
		int y = m_nColumnHeightVect[index];

		unsigned int nColumnHeight = m_pWaterfallViewDataSource->waterfallViewHeightForItemAtIndex(this, i);

		m_rUsedWaterfallCellRects[i] = DRect(x, y + viewHeight, nColumnWidth, nColumnHeight);
		m_mpUsedWaterfallCells[i] = NULL;

		if (m_nColumnHeightVect[index] != 0)
		{
			m_nColumnHeightVect[index] += m_nItemMargin;
		}
		m_nColumnHeightVect[index] += nColumnHeight;
	}
	viewHeight += getMaxColumnValue();

	int iFooterHeight = m_pWaterfallViewDataSource->waterfallViewHeightForFooter(this);
	if (iFooterHeight > 0)
	{
		viewHeight += iFooterHeight;
		viewHeight += m_nItemMargin;
	}

	size.height = viewHeight;
	this->setViewSize(size);
}

void CAWaterfallView::clearData()
{
	m_mpUsedWaterfallCells.clear();
	m_rUsedWaterfallCellRects.clear();
	m_pSelectedWaterfallCells.clear();
	m_nColumnHeightVect.clear();

	for (int i = 0; i < m_vpUsedWaterfallCells.size(); i++)
	{
		CAWaterfallViewCell* cell = m_vpUsedWaterfallCells.at(i);
		CC_CONTINUE_IF(cell == NULL);
		m_mpFreedWaterfallCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetWaterfallViewCell();
	}
	m_vpUsedWaterfallCells.clear();

	m_pHighlightedWaterfallCells = NULL;
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
	CC_RETURN_IF(m_pWaterfallViewDataSource == NULL);

	this->reloadViewSizeData();

	this->removeAllSubviews();

	DRect winRect = this->getBounds();
	winRect.origin = getContentOffset();

	if (m_nWaterfallHeaderHeight > 0 && m_pWaterfallHeaderView)
	{
		m_pWaterfallHeaderView->setDisplayRange(true);
		m_pWaterfallHeaderView->setFrame(DRect(0, 0, winRect.size.width, m_nWaterfallHeaderHeight));
		addSubview(m_pWaterfallHeaderView);
	}

	int dwValue = getMaxColumnValue();
	dwValue += m_nWaterfallHeaderHeight;
	dwValue += 2 * m_nItemMargin;

	if (m_nWaterfallFooterHeight > 0 && m_pWaterfallFooterView)
	{
		m_pWaterfallFooterView->setFrame(DRect(0, dwValue, winRect.size.width, m_nWaterfallFooterHeight));
		addSubview(m_pWaterfallFooterView);
	}

	this->updateSectionHeaderAndFooterRects();
	this->layoutPullToRefreshView();
	this->startDeaccelerateScroll();
	this->loadWaterfallCell();
}

void CAWaterfallView::firstReloadData()
{
	CC_RETURN_IF(!m_mpUsedWaterfallCells.empty());
	this->reloadData();
}

void CAWaterfallView::recoveryWaterfallCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

	std::map<int, CAWaterfallViewCell*>::iterator itr;
	for (itr = m_mpUsedWaterfallCells.begin(); itr != m_mpUsedWaterfallCells.end(); itr++)
	{
		CAWaterfallViewCell* cell = itr->second;
		CC_CONTINUE_IF(cell == NULL);

		DRect cellRect = cell->getFrame();
		CC_CONTINUE_IF(rect.intersectsRect(cellRect));

		m_mpFreedWaterfallCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetWaterfallViewCell();
		itr->second = NULL;
		m_vpUsedWaterfallCells.eraseObject(cell);
	}
}

void CAWaterfallView::loadWaterfallCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
	rect.origin.y -= rect.size.height * 0.1f;
	rect.size.height *= 1.2f;

	std::map<int, CAWaterfallViewCell*>::iterator itr;
	for (itr = m_mpUsedWaterfallCells.begin(); itr != m_mpUsedWaterfallCells.end(); itr++)
	{
		CC_CONTINUE_IF(itr->second != NULL);

		int r = itr->first;
		DRect cellRect = m_rUsedWaterfallCellRects[r];
		CC_CONTINUE_IF(!rect.intersectsRect(cellRect));

		CAWaterfallViewCell* cell = m_pWaterfallViewDataSource->waterfallCellAtIndex(this, cellRect.size, r);
		if (cell)
		{
			cell->m_nItemIndex = r;
			cell->updateDisplayedAlpha(this->getAlpha());
			this->addSubview(cell);
			cell->setFrame(cellRect);
			itr->second = cell;
			m_vpUsedWaterfallCells.pushBack(cell);

			if (m_pSelectedWaterfallCells.count(r))
			{
				cell->setControlStateSelected();
			}

			if (m_pWaterfallViewDataSource)
			{
				m_pWaterfallViewDataSource->waterfallViewWillDisplayCellAtIndex(this, cell, r);
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

float CAWaterfallView::maxSpeed(float dt)
{
	return (128 * 60 * dt);
}

float CAWaterfallView::maxSpeedCache(float dt)
{
	return (maxSpeed(dt) * 2.0f);
}

float CAWaterfallView::decelerationRatio(float dt)
{
	return 1.8f * dt;
}

CAWaterfallViewCell* CAWaterfallView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
{
	CAWaterfallViewCell* cell = NULL;

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
:m_pBackgroundView(NULL)
, m_nItemIndex(0xffffffff)
, m_bControlStateEffect(true)
, m_bAllowsSelected(true)
{
	this->setHaveNextResponder(true);
	this->setDisplayRange(false);
	this->setColor(CAColor_clear);
}


CAWaterfallViewCell::~CAWaterfallViewCell()
{
	CC_SAFE_RELEASE_NULL(m_pContentView);
	CC_SAFE_RELEASE_NULL(m_pBackgroundView);
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
	return NULL;
}

bool CAWaterfallViewCell::initWithReuseIdentifier(const std::string& reuseIdentifier)
{
	m_pContentView = new CAView();
	this->addSubview(m_pContentView);

	this->setBackgroundView(CAView::create());
	this->setReuseIdentifier(reuseIdentifier);
	this->normalWaterfallViewCell();

	return true;
}

void CAWaterfallViewCell::setBackgroundView(CrossApp::CAView *var)
{
	CC_SAFE_RETAIN(var);
	this->removeSubview(m_pBackgroundView);
	CC_SAFE_RELEASE(m_pBackgroundView);
	m_pBackgroundView = var;
	CC_RETURN_IF(m_pBackgroundView == NULL);
	m_pBackgroundView->setFrame(this->getBounds());
	this->insertSubview(m_pBackgroundView, -1);
}

CAView* CAWaterfallViewCell::getBackgroundView()
{
	return m_pBackgroundView;
}

void CAWaterfallViewCell::setContentSize(const CrossApp::DSize &var)
{
	CAView::setContentSize(var);

	m_pContentView->setFrame(this->getBounds());
	if (m_pBackgroundView)
	{
		m_pBackgroundView->setFrame(m_pContentView->getBounds());
	}
}

void CAWaterfallViewCell::setControlState(const CAControlState& var)
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
		this->normalWaterfallViewCell();
		break;
	case CAControlStateHighlighted:
		this->highlightedWaterfallViewCell();
		break;
	case CAControlStateSelected:
		this->selectedWaterfallViewCell();
		break;
	case CAControlStateDisabled:
		this->disabledWaterfallViewCell();
		break;
	default:
		break;
	}
}

void CAWaterfallViewCell::normalWaterfallViewCell()
{
	CC_RETURN_IF(m_pBackgroundView == NULL);
	m_pBackgroundView->setColor(ccc4(255, 255, 255, 255));
}

void CAWaterfallViewCell::highlightedWaterfallViewCell()
{
	CC_RETURN_IF(m_pBackgroundView == NULL);
	m_pBackgroundView->setColor(ccc4(50, 193, 255, 255));
}


void CAWaterfallViewCell::selectedWaterfallViewCell()
{
	CC_RETURN_IF(m_pBackgroundView == NULL);
	m_pBackgroundView->setColor(ccc4(50, 193, 255, 255));
}


void CAWaterfallViewCell::disabledWaterfallViewCell()
{
	CC_RETURN_IF(m_pBackgroundView == NULL);
	m_pBackgroundView->setColor(ccc4(127, 127, 127, 255));
}

void CAWaterfallViewCell::resetWaterfallViewCell()
{
	this->setVisible(true);
	this->normalWaterfallViewCell();
	this->recoveryWaterfallViewCell();
	m_pContentView->setScale(1.0f);
	m_pContentView->setFrame(this->getBounds());
	m_pContentView->setRotation(0);
}


NS_CC_END