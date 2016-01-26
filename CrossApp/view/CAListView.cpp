#include "CAListView.h"
#include "basics/CAApplication.h"
#include "control/CAButton.h"
#include "support/CAPointExtension.h"
#include "basics/CAScheduler.h"
#include "dispatcher/CATouch.h"
#include "animation/CAViewAnimation.h"
#include "platform/CADensityDpi.h"
NS_CC_BEGIN

#pragma CAListView

CAListView::CAListView()
: m_pListViewDataSource(NULL)
, m_pListViewDelegate(NULL)
, m_bAllowsSelection(false)
, m_bAllowsMultipleSelection(false)
, m_pHighlightedListCells(NULL)
, m_pListViewOrientation(CAListViewOrientationVertical)
, m_bAllowsHeadAndFootHover(true)
, m_pListHeaderView(NULL)
, m_pListFooterView(NULL)
, m_nListHeaderHeight(0)
, m_nListFooterHeight(0)
, m_obSeparatorColor(CAColor_gray)
, m_nSeparatorViewHeight(1)
{
    
}


CAListView::~CAListView()
{
	m_mpFreedListCells.clear();
	CC_SAFE_RELEASE_NULL(m_pListHeaderView);
	CC_SAFE_RELEASE_NULL(m_pListFooterView);
    m_pListViewDataSource = NULL;
    m_pListViewDelegate = NULL;
}

void CAListView::onEnterTransitionDidFinish()
{
	CAScrollView::onEnterTransitionDidFinish();

	if (m_mpUsedListCells.empty())
	{
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CAListView::firstReloadData));
        CAViewAnimation::commitAnimations();
	}
}

void CAListView::onExitTransitionDidStart()
{
	CAScrollView::onExitTransitionDidStart();
}

CAListView* CAListView::createWithFrame(const DRect& rect)
{
	CAListView* listView = new CAListView();
	if (listView && listView->initWithFrame(rect))
	{
		listView->autorelease();
		return listView;
	}
	CC_SAFE_DELETE(listView);
	return NULL;
}

CAListView* CAListView::createWithCenter(const DRect& rect)
{
	CAListView* listView = new CAListView();
	if (listView && listView->initWithCenter(rect))
	{
		listView->autorelease();
		return listView;
	}
	CC_SAFE_DELETE(listView);
	return NULL;
}

bool CAListView::init()
{
	if (!CAScrollView::init())
	{
		return false;
	}

	this->setShowsHorizontalScrollIndicator(false);
    this->setBounceHorizontal(false);
	return true;
}

void CAListView::setAllowsSelection(bool var)
{
    m_bAllowsSelection = var;
    CC_RETURN_IF(!m_bAllowsSelection);
	CC_RETURN_IF(m_pSelectedListCells.empty());
	std::set<unsigned int>::iterator itr;
	for (itr = m_pSelectedListCells.begin(); itr != m_pSelectedListCells.end(); itr++)
    {
		if (CAListViewCell* cell = m_mpUsedListCells[(*itr)])
		{
			cell->setControlState(CAControlStateNormal);
		}
    }
	m_pSelectedListCells.clear();
}

void CAListView::setAllowsMultipleSelection(bool var)
{
    m_bAllowsMultipleSelection = var;
	CC_RETURN_IF(m_pSelectedListCells.empty());
	std::set<unsigned int>::iterator itr;
	for (itr = m_pSelectedListCells.begin(); itr != m_pSelectedListCells.end(); itr++)
    {
		if (CAListViewCell* cell = m_mpUsedListCells[(*itr)])
		{
			cell->setControlState(CAControlStateNormal);
		}
    }
	m_pSelectedListCells.clear();
}

void CAListView::setSelectAtIndex(unsigned int index)
{
	if (!m_pSelectedListCells.empty() && m_bAllowsMultipleSelection == false)
	{
		std::set<unsigned int>::iterator itr;
		for (itr = m_pSelectedListCells.begin(); itr != m_pSelectedListCells.end(); itr++)
		{
			if (CAListViewCell* cell = m_mpUsedListCells[(*itr)])
			{
				cell->setControlState(CAControlStateNormal);
			}
		}
		m_pSelectedListCells.clear();
	}

	if (CAListViewCell* cell = m_mpUsedListCells[index])
	{
		cell->setControlStateSelected();
	}
	m_pSelectedListCells.insert(index);
}

void CAListView::setUnSelectAtIndex(unsigned int index)
{
    CC_RETURN_IF(index >= m_rIndexRects.size());
    
    CC_RETURN_IF(m_pSelectedListCells.find(index) == m_pSelectedListCells.end());
    if (CAListViewCell* cell = m_mpUsedListCells.at(index))
    {
        cell->setControlStateNormal();
    }
    m_pSelectedListCells.erase(index);
}

void CAListView::setShowsScrollIndicators(bool var)
{
    bool bVertScroll = m_pListViewOrientation == CAListViewOrientationVertical;
    this->setShowsHorizontalScrollIndicator(var && !bVertScroll);
    this->setShowsVerticalScrollIndicator(var && bVertScroll);
    m_bShowsScrollIndicators = var;
}

CAListViewCell* CAListView::cellForRowAtIndex(unsigned int index)
{
    return m_mpUsedListCells[index];
}

const CAVector<CAListViewCell*>& CAListView::displayingListCell()
{
    return m_vpUsedListCells;
}

void CAListView::setListViewOrientation(CAListViewOrientation var)
{
	m_pListViewOrientation = var;
	bool bVertScroll = m_pListViewOrientation == CAListViewOrientationVertical;
	
	setShowsVerticalScrollIndicator(bVertScroll);
	setBounceVertical(bVertScroll);
	setShowsHorizontalScrollIndicator(!bVertScroll);
	setBounceHorizontal(!bVertScroll);
	setHorizontalScrollEnabled(!bVertScroll);
	setVerticalScrollEnabled(bVertScroll);
}

CAListViewOrientation CAListView::getListViewOrientation()
{
	return m_pListViewOrientation;
}

bool CAListView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    bool isInertia = m_tInertia.getLength() < 1.0f;
	if (!CAScrollView::ccTouchBegan(pTouch, pEvent))
		return false;

	if (m_pContainer->isTouchEnabled() && m_bAllowsSelection && this->isScrollWindowNotOutSide() == false && isInertia)
	{
		DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);

		std::map<unsigned int, CAListViewCell*>::iterator itr;
		for (itr = m_mpUsedListCells.begin(); itr != m_mpUsedListCells.end(); ++itr)
		{
			CAListViewCell* pCell = itr->second;
			CC_CONTINUE_IF(pCell == NULL);

			if (pCell->getFrame().containsPoint(point) && pCell->isVisible())
			{
				CC_BREAK_IF(pCell->getControlState() == CAControlStateDisabled);

                if (m_pHighlightedListCells != pCell)
                {
                    if (m_pHighlightedListCells)
                    {
                        m_pHighlightedListCells->setControlStateNormal();
                    }
                    m_pHighlightedListCells = pCell;
                }

				CC_BREAK_IF(pCell->getControlState() == CAControlStateSelected);

                CAViewAnimation::beginAnimations(m_s__StrID, NULL);
                CAViewAnimation::setAnimationDuration(0.05f);
                CAViewAnimation::setAnimationDidStopSelector(pCell, CAViewAnimation0_selector(CAListViewCell::setControlStateHighlighted));
                CAViewAnimation::commitAnimations();
				break;
			}
		}
	}
	return true;
}

void CAListView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchMoved(pTouch, pEvent);

	if (m_pHighlightedListCells)
	{
		CAViewAnimation::removeAnimations(m_s__StrID);

		if (m_pHighlightedListCells->getControlState() == CAControlStateHighlighted)
		{
			m_pHighlightedListCells->setControlStateNormal();
		}

		m_pHighlightedListCells = NULL;
	}

}

void CAListView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchEnded(pTouch, pEvent);

	if (m_pHighlightedListCells)
	{
		CAViewAnimation::removeAnimations(m_s__StrID);

		unsigned int iDeSelectIndex = -1;
		unsigned int iSelectIndex = m_pHighlightedListCells->getIndex();
        
		m_pHighlightedListCells = NULL;

		if (m_pSelectedListCells.count(iSelectIndex) > 0 && m_bAllowsMultipleSelection)
		{
			iDeSelectIndex = iSelectIndex;
			iSelectIndex = -1;
			m_pSelectedListCells.erase(iDeSelectIndex);
		}
		else
		{
			if (m_pSelectedListCells.size() > 0 && m_bAllowsMultipleSelection == false)
			{
				iDeSelectIndex = *m_pSelectedListCells.begin();
				m_pSelectedListCells.clear();
			}
			m_pSelectedListCells.insert(iSelectIndex);
		}

		if (iDeSelectIndex != -1)
		{
			if (CAListViewCell* cell = m_mpUsedListCells[iDeSelectIndex])
			{
				cell->setControlStateNormal();
			}
			if (m_pListViewDelegate)
			{
				m_pListViewDelegate->listViewDidDeselectCellAtIndex(this, iDeSelectIndex);
			}
		}

		if (iSelectIndex != -1)
		{
			if (CAListViewCell* cell = m_mpUsedListCells[iSelectIndex])
			{
				cell->setControlStateSelected();
			}
			if (m_pListViewDelegate)
			{
				m_pListViewDelegate->listViewDidSelectCellAtIndex(this, iSelectIndex);
			}
		}
	}
}

void CAListView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
	CAScrollView::ccTouchCancelled(pTouch, pEvent);

	if (m_pHighlightedListCells)
	{
        CAViewAnimation::removeAnimations(m_s__StrID);
        
        if (m_pHighlightedListCells->getControlState() == CAControlStateHighlighted)
        {
            m_pHighlightedListCells->setControlStateNormal();
        }
		m_pHighlightedListCells = NULL;
	}
}

void CAListView::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
{
    if (m_bAllowsSelection)
    {
        DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);
        
        std::map<unsigned int, CAListViewCell*>::iterator itr;
        for (itr = m_mpUsedListCells.begin(); itr != m_mpUsedListCells.end(); ++itr)
        {
            CAListViewCell* pCell = itr->second;
            CC_CONTINUE_IF(pCell == NULL);
            
            if (pCell->getFrame().containsPoint(point) && pCell->isVisible())
            {
                CC_BREAK_IF(pCell->getControlState() == CAControlStateDisabled);
                
                if (m_pHighlightedListCells)
                {
                    unsigned int index = m_pHighlightedListCells->getIndex();
                    if (m_pSelectedListCells.count(index))
                    {
                        m_pHighlightedListCells->setControlStateHighlighted();
                    }
                    else
                    {
                        m_pHighlightedListCells->setControlStateNormal();
                    }
                    
                }
                
                m_pHighlightedListCells = pCell;
                pCell->setControlStateHighlighted();

                break;
            }
        }
    }
}

void CAListView::mouseMovedOutSide(CATouch* pTouch, CAEvent* pEvent)
{
    if (m_pHighlightedListCells)
    {
        unsigned int index = m_pHighlightedListCells->getIndex();
        if (m_pSelectedListCells.count(index))
        {
            m_pHighlightedListCells->setControlStateSelected();
        }
        else
        {
            m_pHighlightedListCells->setControlStateNormal();
        }
        m_pHighlightedListCells = NULL;
    }
}

void CAListView::switchPCMode(bool var)
{
    CAScrollView::switchPCMode(var);
    this->setMouseMovedEnabled(true);
}

void CAListView::reloadViewSizeData()
{
    DRect winRect = this->getBounds();
    winRect.origin = getContentOffset();
    float width = winRect.size.width;
    float height = winRect.size.height;
    
	clearData();
    
    int iStartPosition = 0;
    if (m_nListHeaderHeight > 0)
    {
        m_rHeaderRect = (m_pListViewOrientation == CAListViewOrientationVertical)
        ? DRect(0, iStartPosition, width, m_nListHeaderHeight)
        : DRect(iStartPosition, 0, m_nListHeaderHeight, height);
        iStartPosition += m_nListHeaderHeight;
    }
    
    m_nIndexs = m_pListViewDataSource->numberOfIndex(this);
    m_rIndexRects.resize(m_nIndexs);
    m_rLineRects.resize(m_nIndexs);
    for (unsigned i = 0; i < m_nIndexs; i++)
    {
        unsigned int cellHeight = m_pListViewDataSource->listViewHeightForIndex(this, i);
        if (cellHeight > 0)
        {
            DRect cellRect = (m_pListViewOrientation == CAListViewOrientationVertical)
            ? DRect(0, iStartPosition, width, cellHeight)
            : DRect(iStartPosition, 0, cellHeight, height);
            m_rIndexRects[i] = cellRect;
            iStartPosition += cellHeight;
        }
        if (m_nSeparatorViewHeight > 0)
        {
            m_rLineRects[i] = (m_pListViewOrientation == CAListViewOrientationVertical)
            ? DRect(0, iStartPosition, width, s_px_to_dip(m_nSeparatorViewHeight))
            : DRect(iStartPosition, 0, s_px_to_dip(m_nSeparatorViewHeight), height);
            iStartPosition += m_nSeparatorViewHeight;
        }
    }
    
    if (m_nListFooterHeight > 0)
    {
        m_rFooterRect = (m_pListViewOrientation == CAListViewOrientationVertical)
        ? DRect(0, iStartPosition, width, m_nListFooterHeight)
        : DRect(iStartPosition, 0, m_nListFooterHeight, height);
        iStartPosition += m_nListFooterHeight;
    }
    
    if (m_pListViewOrientation == CAListViewOrientationVertical)
    {
        this->setViewSize(DSize(width, iStartPosition));
    }
    else
    {
        this->setViewSize(DSize(iStartPosition, height));
    }
}

void CAListView::clearData()
{
	std::map<unsigned int, CAView*>::iterator it = m_pUsedLines.begin();
	for (; it != m_pUsedLines.end(); ++it)
	{
		CAView* view = it->second;
		CC_CONTINUE_IF(view == NULL);
		m_pFreedLines.pushBack(view);
		view->removeFromSuperview();
	}
	m_pUsedLines.clear();

	m_mpUsedListCells.clear();

	for (int i = 0; i < m_vpUsedListCells.size(); i++)
	{
		CAListViewCell* cell = m_vpUsedListCells.at(i);
		CC_CONTINUE_IF(cell == NULL);
		m_mpFreedListCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetListViewCell();
	}
	m_vpUsedListCells.clear();
	
	m_nIndexs = 0;
	m_rIndexRects.clear();
	m_rLineRects.clear();
	m_rHeaderRect = m_rFooterRect = DRectZero;
	m_pHighlightedListCells = NULL;
}

void CAListView::reloadData()
{
    CC_RETURN_IF(m_pListViewDataSource == NULL);
    
    this->reloadViewSizeData();
    
	this->removeAllSubviews();
    
    DRect winRect = this->getBounds();
    winRect.origin = this->getContentOffset();
    
    if (m_nListHeaderHeight > 0)
    {
        if (m_pListHeaderView)
        {
            m_pListHeaderView->setFrame(m_rHeaderRect);
            addSubview(m_pListHeaderView);
        }
    }
	
    unsigned int cellCount = m_pListViewDataSource->numberOfIndex(this);
    for (unsigned i = 0; i < cellCount; i++)
    {
		if (m_nIndexs > 0)
		{
			m_mpUsedListCells.insert(std::make_pair(i, (CAListViewCell*)NULL));
		}
    }
    
    if (m_nListFooterHeight > 0)
    {
        if (m_pListFooterView)
        {
            m_pListFooterView->setFrame(m_rFooterRect);
            addSubview(m_pListFooterView);
        }
    }
	loadCollectionCell();
    this->layoutPullToRefreshView();
    this->startDeaccelerateScroll();
}

void CAListView::firstReloadData()
{
    CC_RETURN_IF(!m_mpUsedListCells.empty());
    this->reloadData();
}

void CAListView::recoveryCollectionCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
    rect.origin.y -= rect.size.height * 0.1f;
    rect.size.height *= 1.2f;
    
	std::map<unsigned int, CAListViewCell*>::iterator itr;
	for (itr = m_mpUsedListCells.begin(); itr != m_mpUsedListCells.end(); itr++)
	{
		CAListViewCell* cell = itr->second;
		CC_CONTINUE_IF(cell == NULL);

		DRect cellRect = cell->getFrame();
		CC_CONTINUE_IF(rect.intersectsRect(cellRect));
		
		m_mpFreedListCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetListViewCell();
		itr->second = NULL;
        m_vpUsedListCells.eraseObject(cell);
		
        CAView* line = m_pUsedLines[itr->first];
        CC_CONTINUE_IF(line == NULL);
        m_pFreedLines.pushBack(line);
        line->removeFromSuperview();
        m_pUsedLines[itr->first] = NULL;
	}
}

void CAListView::loadCollectionCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
    rect.origin.y -= rect.size.height * 0.1f;
    rect.size.height *= 1.2f;
    
	std::map<unsigned int, CAListViewCell*>::iterator itr;
	for (itr = m_mpUsedListCells.begin(); itr != m_mpUsedListCells.end(); itr++)
	{
		CC_CONTINUE_IF(itr->second != NULL);

		unsigned int index = itr->first;
		DRect cellRect = m_rIndexRects[index];
		CC_CONTINUE_IF(!rect.intersectsRect(cellRect));

		CAListViewCell* cell = m_pListViewDataSource->listViewCellAtIndex(this, cellRect.size, index);
		if (cell)
		{
            cell->m_nIndex = index;
            cell->updateDisplayedAlpha(this->getAlpha());
			addSubview(cell);
			cell->setFrame(cellRect);
            m_mpUsedListCells[index] = cell;
            m_vpUsedListCells.pushBack(cell);
		}

		if (m_pSelectedListCells.count(index))
		{
			cell->setControlStateSelected();
		}
        
        if (m_pListViewDataSource)
        {
            m_pListViewDataSource->listViewWillDisplayCellAtIndex(this, cell, index);
        }
        
        CAView* view = this->dequeueReusableLine();
        DRect lineRect = m_rLineRects[index];
        if (view == NULL)
        {
            view = CAView::createWithFrame(lineRect, m_obSeparatorColor);
        }
        m_pUsedLines[index] = view;
        this->insertSubview(view, 1);
		view->setFrame(lineRect);
	}
}

void CAListView::update(float dt)
{
    CAScrollView::update(dt);

    recoveryCollectionCell();
    
    loadCollectionCell();
}

float CAListView::maxSpeed(float dt)
{
    return (128 * 60 * dt);
}

float CAListView::decelerationRatio(float dt)
{
    return 1.8f * dt;
}

CAListViewCell* CAListView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
{
	CAListViewCell* cell = NULL;

	if (reuseIdentifier && !m_mpFreedListCells[reuseIdentifier].empty())
	{
		cell = m_mpFreedListCells[reuseIdentifier].back();
		cell->retain()->autorelease();
		m_mpFreedListCells[reuseIdentifier].popBack();
	}
    
	return cell;
}

CAView* CAListView::dequeueReusableLine()
{
    if (m_pFreedLines.empty())
    {
        return NULL;
    }
    CAView* view = m_pFreedLines.front();
    view->retain()->autorelease();
    m_pFreedLines.popFront();
    return view;
}


#pragma CAListViewCell

CAListViewCell::CAListViewCell()
:m_pBackgroundView(NULL)
,m_pContentView(NULL)
,m_nIndex(0xffffffff)
,m_bControlStateEffect(true)
,m_bAllowsSelected(true)
{
    this->setHaveNextResponder(true);
    this->setDisplayRange(false);
    this->setColor(CAColor_clear);
}


CAListViewCell::~CAListViewCell()
{
    CC_SAFE_RELEASE_NULL(m_pContentView);
    CC_SAFE_RELEASE_NULL(m_pBackgroundView);
}

CAListViewCell* CAListViewCell::create(const std::string& reuseIdentifier)
{
	CAListViewCell* cell = new CAListViewCell();
	if (cell && cell->initWithReuseIdentifier(reuseIdentifier))
	{
		cell->autorelease();
		return cell;
	}
	CC_SAFE_DELETE(cell);
	return NULL;
}

bool CAListViewCell::initWithReuseIdentifier(const std::string& reuseIdentifier)
{
    m_pContentView = new CAView();
    this->addSubview(m_pContentView);
    
    this->setBackgroundView(CAView::create());
    this->setReuseIdentifier(reuseIdentifier);
    this->normalListViewCell();
    
    return true;
}

void CAListViewCell::setBackgroundView(CrossApp::CAView *var)
{
    CC_SAFE_RETAIN(var);
    this->removeSubview(m_pBackgroundView);
    CC_SAFE_RELEASE(m_pBackgroundView);
    m_pBackgroundView = var;
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setFrame(this->getBounds());
    m_pContentView->insertSubview(m_pBackgroundView, -1);
}

CAView* CAListViewCell::getBackgroundView()
{
    return m_pBackgroundView;
}

void CAListViewCell::setContentSize(const CrossApp::DSize &var)
{
    CAView::setContentSize(var);
    
    m_pContentView->setFrame(this->getBounds());
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(m_pContentView->getBounds());
    }
}

void CAListViewCell::setControlState(const CAControlState& var)
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
    switch (var)
    {
        case CAControlStateNormal:
			this->normalListViewCell();
            break;
        case CAControlStateHighlighted:
			this->highlightedListViewCell();
            break;
        case CAControlStateSelected:
			this->selectedListViewCell();
            break;
        case CAControlStateDisabled:
			this->disabledListViewCell();
            break;
        default:
            break;
    }
}

void CAListViewCell::normalListViewCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(255, 255, 255, 255));
}

void CAListViewCell::highlightedListViewCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(50, 193, 255, 255));
}


void CAListViewCell::selectedListViewCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(50, 193, 255, 255));
}


void CAListViewCell::disabledListViewCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(127, 127, 127, 255));
}

void CAListViewCell::resetListViewCell()
{
    this->setVisible(true);
    this->normalListViewCell();
    this->recoveryListViewCell();
    m_pContentView->setScale(1.0f);
    m_pContentView->setFrame(this->getBounds());
    m_pContentView->setRotation(0);
}

NS_CC_END