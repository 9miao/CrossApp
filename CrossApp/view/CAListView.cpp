#include "CAListView.h"
#include "basics/CAApplication.h"
#include "control/CAButton.h"
#include "support/CCPointExtension.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionEase.h"
#include "basics/CAScheduler.h"
#include "dispatcher/CATouch.h"
#include "view/CAWindow.h"
#include <utility>

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
	CC_SAFE_RELEASE_NULL(m_pListHeaderView);
	CC_SAFE_RELEASE_NULL(m_pListFooterView);
    m_pListViewDataSource = NULL;
    m_pListViewDelegate = NULL;
}

void CAListView::onEnterTransitionDidFinish()
{
	CAScrollView::onEnterTransitionDidFinish();

	if (m_pUsedListCells.empty())
	{
		this->runAction(CCSequence::create(CCDelayTime::create(1 / 60.0f),
			CCCallFunc::create(this, callfunc_selector(CAListView::reloadData)), NULL));
	}
}

void CAListView::onExitTransitionDidStart()
{
	CAScrollView::onExitTransitionDidStart();
}

CAListView* CAListView::createWithFrame(const CCRect& rect)
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

CAListView* CAListView::createWithCenter(const CCRect& rect)
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
    this->setTouchMovedListenHorizontal(false);
	return true;
}

float CAListView::maxSpeed(float dt)
{
    return (CCPoint(m_obContentSize).getLength() * 8 * dt);
}

float CAListView::maxSpeedCache(float dt)
{
    return (maxSpeed(dt) * 3.0f);
}

float CAListView::decelerationRatio(float dt)
{
    return 2.0f * dt;
}

void CAListView::reloadViewSizeData()
{
	CCRect winRect = this->getBounds();
	winRect.origin = getContentOffset();
	float width = winRect.size.width;
	float height = winRect.size.height;

    m_nIndexs = 0;
	m_rIndexRects.clear();
	m_rLineRects.clear();
	m_rHeaderRect = m_rFooterRect = CCRectZero;

	int iStartPosition = 0;
	if (m_nListHeaderHeight > 0)
	{
		m_rHeaderRect = (m_pListViewOrientation == CAListViewOrientationVertical)
                        ? CCRect(0, iStartPosition, width, m_nListHeaderHeight)
                        : CCRect(iStartPosition, 0, m_nListHeaderHeight, height);
		iStartPosition += m_nListHeaderHeight;
	}

	m_nIndexs = m_pListViewDataSource->numberOfIndex(this);
    m_rIndexRects.resize(m_nIndexs);
    m_rLineRects.resize(m_nIndexs);
	for (int i = 0; i < m_nIndexs; i++)
	{
		unsigned int cellHeight = m_pListViewDataSource->listViewHeightForIndex(this, i);
		if (cellHeight > 0)
		{
			CCRect cellRect = (m_pListViewOrientation == CAListViewOrientationVertical)
                            ? CCRect(0, iStartPosition, width, cellHeight)
                            : CCRect(iStartPosition, 0, cellHeight, height);
			m_rIndexRects[i] = cellRect;
			iStartPosition += cellHeight;
		}
        if (m_nSeparatorViewHeight > 0)
        {
            m_rLineRects[i] = (m_pListViewOrientation == CAListViewOrientationVertical)
            ? CCRect(0, iStartPosition, width, m_nSeparatorViewHeight)
            : CCRect(iStartPosition, 0, m_nSeparatorViewHeight, height);
            iStartPosition += m_nSeparatorViewHeight;
        }
	}

	if (m_nListFooterHeight > 0)
	{
		m_rFooterRect = (m_pListViewOrientation == CAListViewOrientationVertical)
                        ? CCRect(0, iStartPosition, width, m_nListFooterHeight)
                        : CCRect(iStartPosition, 0, m_nListFooterHeight, height);
		iStartPosition += m_nListFooterHeight;
	}

	if (m_pListViewOrientation == CAListViewOrientationVertical)
	{
		this->setViewSize(CCSize(width, iStartPosition));
	}
	else
	{
		this->setViewSize(CCSize(iStartPosition, height));
	}
}

void CAListView::reloadData()
{
	if (m_pListViewDataSource == NULL)
		return;
    
    this->reloadViewSizeData();
    
    this->removeAllSubviews();
    
    m_pUsedLines.clear();
	m_pUsedListCells.clear();
	m_pFreedListCells.clear();
    m_pSelectedListCells.clear();
	
	CCRect winRect = this->getBounds();
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
	for (int i = 0; i < cellCount; i++)
	{
		if (m_nIndexs > 0)
		{
			std::pair<std::map<unsigned int, CAListViewCell*>::iterator, bool> itrResult =
				m_pUsedListCells.insert(std::make_pair(i, (CAListViewCell*)NULL));

			CC_CONTINUE_IF(!winRect.intersectsRect(m_rIndexRects[i]));

			CAListViewCell* pCellView = m_pListViewDataSource->listViewCellAtIndex(this, m_rIndexRects[i].size, i);
			if (pCellView)
			{
				pCellView->m_nIndex = i;
				pCellView->setFrame(m_rIndexRects[i]);
				addSubview(pCellView);
				itrResult.first->second = pCellView;
			}
		}
        
		if (m_nSeparatorViewHeight > 0)
		{
			CAView* view = CAView::createWithFrame(m_rLineRects[i], m_obSeparatorColor);
			addSubview(view);
            m_pUsedLines[i] = view;
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
    this->layoutPullToRefreshView();
    this->startDeaccelerateScroll();
}

void CAListView::setAllowsSelection(bool var)
{
    m_bAllowsSelection = var;
    CC_RETURN_IF(!m_bAllowsSelection);
	CC_RETURN_IF(m_pSelectedListCells.empty());
	std::set<unsigned int>::iterator itr;
	for (itr = m_pSelectedListCells.begin(); itr != m_pSelectedListCells.end(); itr++)
    {
		if (CAListViewCell* cell = m_pUsedListCells[(*itr)])
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
		if (CAListViewCell* cell = m_pUsedListCells[(*itr)])
		{
			cell->setControlState(CAControlStateNormal);
		}
    }
	m_pSelectedListCells.clear();
}

void CAListView::setSelectAtIndex(unsigned int index)
{
	if (!m_pSelectedListCells.empty())
	{
		std::set<unsigned int>::iterator itr;
		for (itr = m_pSelectedListCells.begin(); itr != m_pSelectedListCells.end(); itr++)
		{
			if (CAListViewCell* cell = m_pUsedListCells[(*itr)])
			{
				cell->setControlState(CAControlStateNormal);
			}
		}
		m_pSelectedListCells.clear();
	}

	if (CAListViewCell* cell = m_pUsedListCells[index])
	{
		cell->setControlStateSelected();
	}
	m_pSelectedListCells.insert(index);
}

void CAListView::setListViewOrientation(CAListViewOrientation var)
{
	m_pListViewOrientation = var;
	bool bVertScroll = m_pListViewOrientation == CAListViewOrientationVertical;
	
	setShowsVerticalScrollIndicator(bVertScroll);
	setBounceVertical(bVertScroll);
	setShowsHorizontalScrollIndicator(!bVertScroll);
	setBounceHorizontal(!bVertScroll);
	setTouchMovedListenHorizontal(!bVertScroll);
	setTouchMovedListenVertical(bVertScroll);
}

CAListViewOrientation CAListView::getListViewOrientation()
{
	return m_pListViewOrientation;
}

bool CAListView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
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

		std::map<unsigned int, CAListViewCell*>::iterator itr;
		for (itr = m_pUsedListCells.begin(); itr != m_pUsedListCells.end(); ++itr)
		{
			CAListViewCell* pCell = itr->second;
			CC_CONTINUE_IF(pCell == NULL);

			if (pCell->getFrame().containsPoint(point) && pCell->isVisible())
			{
				CC_BREAK_IF(pCell->getControlState() == CAControlStateDisabled);

				if (m_pHighlightedListCells)
				{
					m_pHighlightedListCells->setControlStateNormal();
				}

				m_pHighlightedListCells = pCell;

				CC_BREAK_IF(pCell->getControlState() == CAControlStateSelected);

				CCDelayTime* delayTime = CCDelayTime::create(0.05f);
				CCCallFunc* func = CCCallFunc::create(pCell, callfunc_selector(CAListViewCell::setControlStateHighlighted));
				CCSequence* actions = CCSequence::create(delayTime, func, NULL);
				m_pContainer->runAction(actions);
				break;
			}
		}
	}
	return true;
}

void CAListView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	CC_RETURN_IF(m_bscrollEnabled == false);

	CAScrollView::ccTouchMoved(pTouch, pEvent);

	if (m_pHighlightedListCells)
	{
		m_pContainer->stopAllActions();

		if (m_pHighlightedListCells->getControlState() == CAControlStateHighlighted)
		{
			m_pHighlightedListCells->setControlStateNormal();
		}

		m_pHighlightedListCells = NULL;
	}

}

void CAListView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
	CAScrollView::ccTouchEnded(pTouch, pEvent);

	if (m_pHighlightedListCells)
	{
		m_pContainer->stopAllActions();

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
			if (CAListViewCell* cell = m_pUsedListCells[iDeSelectIndex])
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
			if (CAListViewCell* cell = m_pUsedListCells[iSelectIndex])
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
	CAScrollView::ccTouchCancelled(pTouch, pEvent);

	if (m_pHighlightedListCells)
	{
        m_pContainer->stopAllActions();
        
		m_pHighlightedListCells->setControlStateNormal();
		m_pHighlightedListCells = NULL;
	}
}

void CAListView::update(float dt)
{
    CAScrollView::update(dt);
    
	recoveryCollectionCell();

	loadCollectionCell();
}


void CAListView::recoveryCollectionCell()
{
	CCRect rect = this->getBounds();
	rect.origin = getContentOffset();
    rect.origin.y -= rect.size.height * 0.1f;
    rect.size.height *= 1.2f;
    
	std::map<unsigned int, CAListViewCell*>::iterator itr;
	for (itr = m_pUsedListCells.begin(); itr != m_pUsedListCells.end(); itr++)
	{
		CAListViewCell* cell = itr->second;
		CC_CONTINUE_IF(cell == NULL);

		CCRect cellRect = cell->getFrame();
		CC_CONTINUE_IF(rect.intersectsRect(cellRect));

		m_pFreedListCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetListViewCell();
		itr->second = NULL;
        
        
        CAView* line = m_pUsedLines[itr->first];
        CC_CONTINUE_IF(line == NULL);
        m_pFreedLines.pushBack(line);
        line->removeFromSuperview();
        m_pUsedLines[itr->first] = NULL;
	}
}

void CAListView::loadCollectionCell()
{
	CCRect rect = this->getBounds();
	rect.origin = getContentOffset();
    rect.origin.y -= rect.size.height * 0.1f;
    rect.size.height *= 1.2f;
    
	std::map<unsigned int, CAListViewCell*>::iterator itr;
	for (itr = m_pUsedListCells.begin(); itr != m_pUsedListCells.end(); itr++)
	{
		CC_CONTINUE_IF(itr->second != NULL);

		unsigned int index = itr->first;
		CCRect cellRect = m_rIndexRects[index];
		CC_CONTINUE_IF(!rect.intersectsRect(cellRect));

		CAListViewCell* cell = m_pListViewDataSource->listViewCellAtIndex(this, cellRect.size, index);
		if (cell)
		{
            cell->m_nIndex = index;
            cell->updateDisplayedAlpha(this->getAlpha());
			addSubview(cell);
			cell->setFrame(cellRect);
            m_pUsedListCells[index] = cell;
		}

		if (m_pSelectedListCells.count(index))
		{
			cell->setControlStateSelected();
		}
        
        CAView* view = this->dequeueReusableLine();
        CCRect lineRect = m_rLineRects[index];
        if (view == NULL)
        {
            view = CAView::createWithFrame(lineRect, m_obSeparatorColor);
        }
        m_pUsedLines[index] = view;
        this->insertSubview(view, 1);
        view->setFrame(lineRect);
	}
}

CAListViewCell* CAListView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
{
	CAListViewCell* cell = NULL;

	if (reuseIdentifier && !m_pFreedListCells[reuseIdentifier].empty())
	{
		cell = m_pFreedListCells[reuseIdentifier].back();
		cell->retain()->autorelease();
		m_pFreedListCells[reuseIdentifier].popBack();
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
,m_nIndex(0xffffffff)
,m_bControlStateEffect(true)
,m_bAllowsSelected(true)
{
    this->setHaveNextResponder(true);
}


CAListViewCell::~CAListViewCell()
{
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
    this->setDisplayRange(false);
    this->setBackgroundView(CAView::create());
    this->setColor(CAColor_clear);
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
    this->insertSubview(m_pBackgroundView, -1);
}

CAView* CAListViewCell::getBackgroundView()
{
    return m_pBackgroundView;
}

void CAListViewCell::setContentSize(const CrossApp::CCSize &var)
{
    CAView::setContentSize(var);
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getBounds());
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
}

NS_CC_END