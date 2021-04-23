#include "CAListView.h"
#include "basics/CAApplication.h"
#include "control/CAButton.h"
#include "basics/CAPointExtension.h"
#include "basics/CAScheduler.h"
#include "dispatcher/CATouch.h"
#include "animation/CAViewAnimation.h"
#include "platform/CADensityDpi.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN

#pragma CAListView

CAListView::CAListView()
: m_pListViewDataSource(nullptr)
, m_pListViewDelegate(nullptr)
, m_bAllowsSelection(false)
, m_bAllowsMultipleSelection(false)
, m_pHighlightedListCells(nullptr)
, m_eOrientation(CAListView::Orientation::Vertical)
, m_pListHeaderView(nullptr)
, m_pListFooterView(nullptr)
, m_nListHeaderHeight(0)
, m_nListFooterHeight(0)
, m_obSeparatorColor(ccc4Int(0xffefeef4))
, m_nSeparatorViewHeight(1)
, m_bLoadData(true)
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CACell");
    m_obSeparatorColor = ccc4Int(CrossApp::hex2Int(map.at("separatorColor")));
}

CAListView::~CAListView()
{
	m_mpFreedListCells.clear();
	CC_SAFE_RELEASE_NULL(m_pListHeaderView);
	CC_SAFE_RELEASE_NULL(m_pListFooterView);
    m_pListViewDataSource = nullptr;
    m_pListViewDelegate = nullptr;
}

void CAListView::onEnterTransitionDidFinish()
{
	CAScrollView::onEnterTransitionDidFinish();
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
	return nullptr;
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
	return nullptr;
}

CAListView* CAListView::createWithLayout(const CrossApp::DLayout &layout)
{
    CAListView* listView = new CAListView();
    if (listView && listView->initWithLayout(layout))
    {
        listView->autorelease();
        return listView;
    }
    CC_SAFE_DELETE(listView);
    return nullptr;
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

void CAListView::setContentSize(const CrossApp::DSize &var)
{
    CAScrollView::setContentSize(var);
    
    if (!m_mpUsedListCells.empty())
    {
        this->reloadData();
    }
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
			cell->setControlState(CAControl::State::Normal);
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
			cell->setControlState(CAControl::State::Normal);
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
				cell->setControlState(CAControl::State::Normal);
			}
		}
		m_pSelectedListCells.clear();
	}

	if (CAListViewCell* cell = m_mpUsedListCells[index])
	{
		cell->setControlState(CAControl::State::Selected);
	}
	m_pSelectedListCells.insert(index);
}

void CAListView::setUnSelectAtIndex(unsigned int index)
{
    CC_RETURN_IF(index >= m_rIndexRects.size());
    
    CC_RETURN_IF(m_pSelectedListCells.find(index) == m_pSelectedListCells.end());
    if (CAListViewCell* cell = m_mpUsedListCells.at(index))
    {
        cell->setControlState(CAControl::State::Normal);
    }
    m_pSelectedListCells.erase(index);
}

void CAListView::setShowsScrollIndicators(bool var)
{
    bool bVertScroll = m_eOrientation == CAListView::Orientation::Vertical;
    this->setShowsHorizontalScrollIndicator(var && !bVertScroll);
    this->setShowsVerticalScrollIndicator(var && bVertScroll);
    m_bShowsScrollIndicators = var;
}

CAListViewCell* CAListView::cellForRowAtIndex(unsigned int index)
{
    if (m_mpUsedListCells.find(index) == m_mpUsedListCells.end())
    {
        return nullptr;
    }
    return m_mpUsedListCells[index];
}

const CAVector<CAListViewCell*>& CAListView::displayingListCell()
{
    return m_vpUsedListCells;
}

void CAListView::setOrientation(CAListView::Orientation var)
{
	m_eOrientation = var;
	bool bVertScroll = m_eOrientation == CAListView::Orientation::Vertical;
	
	setShowsVerticalScrollIndicator(bVertScroll);
	setBounceVertical(bVertScroll);
	setShowsHorizontalScrollIndicator(!bVertScroll);
	setBounceHorizontal(!bVertScroll);
	setHorizontalScrollEnabled(!bVertScroll);
	setVerticalScrollEnabled(bVertScroll);
    
    if(!m_mpUsedListCells.empty())
    {
        this->reloadData();
    }
}

CAListView::Orientation CAListView::getOrientation()
{
	return m_eOrientation;
}

bool CAListView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    bool isInertia = m_tInertia.getLength() < 1.0f;
	if (!CAScrollView::ccTouchBegan(pTouch, pEvent))
		return false;

	if (m_pContainer->isTouchEnabled() && m_bAllowsSelection && this->isScrollWindowNotOutSide() == false && isInertia)
	{
		DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);

        for (auto pair : m_mpUsedListCells)
		{
			CAListViewCell* cell = pair.second;
			CC_CONTINUE_IF(cell == nullptr);

			if (cell->getFrame().containsPoint(point) && cell->isVisible())
			{
				CC_BREAK_IF(cell->getControlState() == CAControl::State::Disabled);

                if (m_pHighlightedListCells != cell)
                {
                    if (m_pHighlightedListCells)
                    {
                        m_pHighlightedListCells->setControlState(CAControl::State::Normal);
                    }
                    m_pHighlightedListCells = cell;
                }

				CC_BREAK_IF(cell->getControlState() == CAControl::State::Selected);

                CAViewAnimation::beginAnimations(m_s__StrID);
                CAViewAnimation::setAnimationDuration(0.05f);
                CAViewAnimation::setAnimationDidStopSelector(std::bind(&CAListViewCell::setControlStateHighlighted, cell));
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

		if (m_pHighlightedListCells->getControlState() == CAControl::State::Highlighted)
		{
			m_pHighlightedListCells->setControlState(CAControl::State::Normal);
		}

		m_pHighlightedListCells = nullptr;
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
        
		m_pHighlightedListCells = nullptr;

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
				cell->setControlState(CAControl::State::Normal);
			}
            if (m_obDidDeselectCellAtIndexPath)
            {
                m_obDidDeselectCellAtIndexPath(iDeSelectIndex);
            }
            else if (m_pListViewDelegate)
			{
				m_pListViewDelegate->listViewDidDeselectCellAtIndex(this, iDeSelectIndex);
			}
		}

		if (iSelectIndex != -1)
		{
			if (CAListViewCell* cell = m_mpUsedListCells[iSelectIndex])
			{
				cell->setControlState(CAControl::State::Selected);
			}
            if (m_obDidSelectCellAtIndexPath)
            {
                m_obDidSelectCellAtIndexPath(iSelectIndex);
            }
			else if (m_pListViewDelegate)
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
        
        if (m_pHighlightedListCells->getControlState() == CAControl::State::Highlighted)
        {
            m_pHighlightedListCells->setControlState(CAControl::State::Normal);
        }
		m_pHighlightedListCells = nullptr;
	}
}

void CAListView::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
{
    if (m_bAllowsSelection)
    {
        DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);
        
        for (auto pair : m_mpUsedListCells)
        {
            CAListViewCell* cell = pair.second;
            CC_CONTINUE_IF(cell == nullptr);
            
            if (cell->getFrame().containsPoint(point) && cell->isVisible())
            {
                CC_BREAK_IF(cell->getControlState() == CAControl::State::Disabled);
                
                if (m_pHighlightedListCells)
                {
                    unsigned int index = m_pHighlightedListCells->getIndex();
                    if (m_pSelectedListCells.count(index))
                    {
                        m_pHighlightedListCells->setControlState(CAControl::State::Highlighted);
                    }
                    else
                    {
                        m_pHighlightedListCells->setControlState(CAControl::State::Normal);
                    }
                    
                }
                
                m_pHighlightedListCells = cell;
                cell->setControlState(CAControl::State::Highlighted);

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
            m_pHighlightedListCells->setControlState(CAControl::State::Selected);
        }
        else
        {
            m_pHighlightedListCells->setControlState(CAControl::State::Normal);
        }
        m_pHighlightedListCells = nullptr;
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
    
	this->clearData();
    
    int iStartPosition = 0;
    if (m_nListHeaderHeight > 0)
    {
        m_rHeaderRect = (m_eOrientation == CAListView::Orientation::Vertical)
        ? DRect(0, iStartPosition, width, m_nListHeaderHeight)
        : DRect(iStartPosition, 0, m_nListHeaderHeight, height);
        iStartPosition += m_nListHeaderHeight;
    }
    
    m_nIndexs = 0;
    
    if (m_obNumberOfRowsAtIndexPath)
    {
        m_nIndexs = m_obNumberOfRowsAtIndexPath();
    }
    else if (m_pListViewDataSource)
    {
        m_nIndexs = m_pListViewDataSource->numberOfIndex(this);
    }
    
    m_rIndexRects.resize(m_nIndexs);
    m_rLineRects.resize(m_nIndexs);
    for (unsigned i = 0; i < m_nIndexs; i++)
    {
        unsigned int cellHeight = 0;
        
        if (m_obCellHeightAtIndexPath)
        {
           cellHeight = m_obCellHeightAtIndexPath(i);
        }
        else if (m_pListViewDataSource)
        {
           cellHeight = m_pListViewDataSource->listViewHeightForIndex(this, i);
        }
        
        if (cellHeight > 0)
        {
            DRect cellRect = (m_eOrientation == CAListView::Orientation::Vertical)
            ? DRect(0, iStartPosition, width, cellHeight)
            : DRect(iStartPosition, 0, cellHeight, height);
            m_rIndexRects[i] = cellRect;
            iStartPosition += cellHeight;
        }
        if (m_nSeparatorViewHeight > 0)
        {
            m_rLineRects[i] = (m_eOrientation == CAListView::Orientation::Vertical)
            ? DRect(0, iStartPosition, width, s_px_to_dip(m_nSeparatorViewHeight))
            : DRect(iStartPosition, 0, s_px_to_dip(m_nSeparatorViewHeight), height);
            iStartPosition += s_px_to_dip(m_nSeparatorViewHeight);
        }
    }
    
    if (m_nListFooterHeight > 0)
    {
        m_rFooterRect = (m_eOrientation == CAListView::Orientation::Vertical)
        ? DRect(0, iStartPosition, width, m_nListFooterHeight)
        : DRect(iStartPosition, 0, m_nListFooterHeight, height);
        iStartPosition += m_nListFooterHeight;
    }
    
    if (m_eOrientation == CAListView::Orientation::Vertical)
    {
        this->setViewSize(DSize(0, iStartPosition));
    }
    else
    {
        this->setViewSize(DSize(iStartPosition, 0));
    }
}

void CAListView::clearData()
{
    for (auto& pair : m_pUsedLines)
	{
		CAView* view = pair.second;
		CC_CONTINUE_IF(view == nullptr);
		m_pFreedLines.pushBack(view);
		view->removeFromSuperview();
	}
	m_pUsedLines.clear();

	m_mpUsedListCells.clear();

	for (auto& cell : m_vpUsedListCells)
	{
		CC_CONTINUE_IF(cell == nullptr);
		m_mpFreedListCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetCell();
	}
	m_vpUsedListCells.clear();
	
	m_nIndexs = 0;
	m_rIndexRects.clear();
	m_rLineRects.clear();
	m_rHeaderRect = m_rFooterRect = DRectZero;
	m_pHighlightedListCells = nullptr;
}

void CAListView::reloadData()
{
    m_bLoadData = true;
    this->updateDraw();
}

void CAListView::recoveryCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
    rect.origin.y -= rect.size.height * 0.1f;
    rect.origin.x -= rect.size.width * 0.1f;
    rect.size.width *= 1.2f;
    rect.size.height *= 1.2f;
    
    for (auto& pair : m_mpUsedListCells)
	{
		CAListViewCell* cell = pair.second;
		CC_CONTINUE_IF(cell == nullptr);

		DRect cellRect = cell->getFrame();
		CC_CONTINUE_IF(rect.intersectsRect(cellRect));
		
		m_mpFreedListCells[cell->getReuseIdentifier()].pushBack(cell);
		cell->removeFromSuperview();
		cell->resetCell();
		pair.second = nullptr;
        m_vpUsedListCells.eraseObject(cell);
		
        CAView* line = m_pUsedLines[pair.first];
        CC_CONTINUE_IF(line == nullptr);
        m_pFreedLines.pushBack(line);
        line->removeFromSuperview();
        m_pUsedLines[pair.first] = nullptr;
	}
}

void CAListView::loadCell()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();
    rect.origin.y -= rect.size.height * 0.1f;
    rect.origin.x -= rect.size.width * 0.1f;
    rect.size.width *= 1.2f;
    rect.size.height *= 1.2f;
    
    for (auto& pair : m_mpUsedListCells)
	{
		CC_CONTINUE_IF(pair.second != nullptr);

		unsigned int index = pair.first;
		DRect cellRect = m_rIndexRects[index];
		CC_CONTINUE_IF(!rect.intersectsRect(cellRect));

        CAListViewCell* cell = nullptr;
        if (m_obCellAtIndexPath)
        {
            cell = m_obCellAtIndexPath(cellRect.size, index);
        }
        else if (m_pListViewDataSource)
        {
            cell = m_pListViewDataSource->listViewCellAtIndex(this, cellRect.size, index);
        }
        
		if (cell)
		{
            cell->m_nIndex = index;
            cell->updateDisplayedAlpha(_realAlpha);
			this->addSubview(cell);
			cell->setFrame(cellRect);
            m_mpUsedListCells[index] = cell;
            m_vpUsedListCells.pushBack(cell);
		}

		if (m_pSelectedListCells.count(index))
		{
			cell->setControlState(CAControl::State::Selected);
		}
        
        if (m_obWillDisplayCellAtIndexPath)
        {
            m_obWillDisplayCellAtIndexPath(cell, index);
        }
        else if (m_pListViewDataSource)
        {
            m_pListViewDataSource->listViewWillDisplayCellAtIndex(this, cell, index);
        }
        
        CAView* view = this->dequeueReusableLine();
        DRect lineRect = m_rLineRects[index];
        if (view == nullptr)
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

    recoveryCell();
    
    loadCell();
}

void CAListView::visitEve()
{
    if (m_bLoadData)
    {
        m_bLoadData = false;
        
        this->reloadViewSizeData();
        
        this->removeAllSubviews();
        
        DRect winRect = this->getBounds();
        winRect.origin = this->getContentOffset();
        
        if (m_pListHeaderView && m_nListHeaderHeight > 0)
        {
            m_pListHeaderView->setFrame(m_rHeaderRect);
            this->addSubview(m_pListHeaderView);
        }
        
        for (unsigned int i = 0; i < m_nIndexs; i++)
        {
            m_mpUsedListCells.insert(std::make_pair(i, (CAListViewCell*)nullptr));
        }
        
        if (m_pListFooterView && m_nListFooterHeight > 0)
        {
            m_pListFooterView->setFrame(m_rFooterRect);
            this->addSubview(m_pListFooterView);
        }
        
        this->loadCell();
        this->layoutPullToRefreshView();
        
        if (this->isScrollWindowNotOutSide())
        {
            this->startDeaccelerateScroll();
        }
    }
    CAScrollView::visitEve();
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
	CAListViewCell* cell = nullptr;

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
        return nullptr;
    }
    CAView* view = m_pFreedLines.front();
    view->retain()->autorelease();
    m_pFreedLines.popFront();
    return view;
}


#pragma CAListViewCell

CAListViewCell::CAListViewCell()
:m_nIndex(UINT_NONE)
{
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
	return nullptr;
}

void CAListViewCell::normalCell()
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CACell");
    CC_RETURN_IF(m_pBackgroundView == nullptr);
    m_pBackgroundView->setColor(ccc4Int(CrossApp::hex2Int(map.at("backgroundColor_normal"))));
}

void CAListViewCell::highlightedCell()
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CACell");
    CC_RETURN_IF(m_pBackgroundView == nullptr);
    m_pBackgroundView->setColor(ccc4Int(CrossApp::hex2Int(map.at("backgroundColor_highlighted"))));
}

void CAListViewCell::selectedCell()
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CACell");
    CC_RETURN_IF(m_pBackgroundView == nullptr);
    m_pBackgroundView->setColor(ccc4Int(CrossApp::hex2Int(map.at("backgroundColor_selected"))));
}

void CAListViewCell::disabledCell()
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CACell");
    CC_RETURN_IF(m_pBackgroundView == nullptr);
    m_pBackgroundView->setColor(ccc4Int(CrossApp::hex2Int(map.at("backgroundColor_disabled"))));
}

NS_CC_END
