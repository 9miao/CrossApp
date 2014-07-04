#include "CACollectionView.h"
#include "basics/CAApplication.h"
#include "control/CAButton.h"
#include "support/CCPointExtension.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionEase.h"
#include "dispatcher/CATouch.h"
#include "view/CAWindow.h"

NS_CC_BEGIN

#pragma CACollectionView

CACollectionView::CACollectionView()
: m_pCollectionViewDataSource(NULL)
, m_pCollectionViewDelegate(NULL)
, m_pCollectionHeaderView(NULL)
, m_pCollectionFooterView(NULL)
, m_pCollectionPullDownView(NULL)
, m_pCollectionPullUpView(NULL)
, m_nCollectionPullViewHeight(0)
, m_bAllowsSelection(false)
, m_bAllowsMultipleSelection(false)
, m_pHighlightedCollectionCells(NULL)
, m_nCollectionHeaderHeight(0)
, m_nCollectionFooterHeight(0)
{
}


CACollectionView::~CACollectionView()
{
}

void CACollectionView::onEnterTransitionDidFinish()
{
	CAScrollView::onEnterTransitionDidFinish();

	if (m_pCollectionCells.empty())
	{
		this->runAction(CCSequence::create(CCDelayTime::create(1 / 60.0f),
			CCCallFunc::create(this, callfunc_selector(CACollectionView::reloadData)), NULL));
	}
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
    this->setTouchSidingDirection(CATouchSidingDirectionVertical);
	return true;
}

float CACollectionView::maxSpeed(float delay)
{
    return (CCPoint(m_obContentSize).getLength() * 6 * delay);
}

float CACollectionView::maxSpeedCache(float delay)
{
    return (maxSpeed(delay) * 3.0f);
}

float CACollectionView::decelerationRatio(float delay)
{
    return 3.0f * delay;
}

CCPoint CACollectionView::maxBouncesLenght()
{
    if (m_pCollectionPullDownView || m_pCollectionPullUpView)
    {
        return CCPoint(0, MIN(m_obContentSize.height/2, m_nCollectionPullViewHeight * 0.8f));
    }
    
    return CCPoint(0, m_obContentSize.height/2);
}

void CACollectionView::reloadData()
{
	if (m_pCollectionViewDataSource == NULL)
		return;

	m_pCollectionCells.clear();

	removeAllSubviews();

	float width = this->getBounds().size.width;
	int y = 0;

	if (m_pCollectionPullDownView)
	{
		m_pCollectionPullDownView->setFrame(CCRect(0, -(float)m_nCollectionPullViewHeight, width, m_nCollectionPullViewHeight));
		addSubview(m_pCollectionPullDownView);
	}

	if (m_nCollectionHeaderHeight > 0 && m_pCollectionHeaderView)
	{
		m_pCollectionHeaderView->setDisplayRange(true);
        m_pCollectionHeaderView->setFrame(CCRect(0, y, width, m_nCollectionHeaderHeight));
        addSubview(m_pCollectionHeaderView);
        y += m_nCollectionHeaderHeight;
	}

	unsigned int rowCount = m_pCollectionViewDataSource->numberOfRowsInCollectionView(this);
	for (int i = 0; i < rowCount; i++)
	{
		int iHeight = m_pCollectionViewDataSource->collectionViewHeightForRowAtIndexPath(this, i);

		unsigned int itemCount = m_pCollectionViewDataSource->numberOfItemsInRows(this, i);
		for (int j = 0; j < itemCount; j++)
		{
			CCRect cellRect = CCRectMake(width*j / itemCount, y, width / itemCount, iHeight);
			CACollectionViewCell* pCellView = m_pCollectionViewDataSource->collectionCellAtIndex(this, cellRect.size, i, j);
			pCellView->setFrame(cellRect);
			addSubview(pCellView);
            pCellView->m_nRow = i;
            pCellView->m_nItem = j;
            m_pCollectionCells.push_back(pCellView);
		}
		y += iHeight;
	}

	if (m_nCollectionFooterHeight > 0 && m_pCollectionHeaderView)
	{
		m_pCollectionFooterView->setDisplayRange(true);
        m_pCollectionFooterView->setFrame(CCRect(0, y, width, m_nCollectionFooterHeight));
        addSubview(m_pCollectionFooterView);
        y += m_nCollectionFooterHeight;
	}

	if (m_pCollectionPullUpView)
	{
		m_pCollectionPullUpView->setFrame(CCRect(0, m_obViewSize.height, width, m_nCollectionPullViewHeight));
		addSubview(m_pCollectionPullUpView);
	}
    
    this->setViewSize(CCSize(width, y));
}


void CACollectionView::setSelectItemAtRow(unsigned int row, unsigned int item)
{
	if (!m_pSelectedCollectionCells.empty())
	{
		std::set<CACollectionViewCell*>::iterator itr;
		for (itr = m_pSelectedCollectionCells.begin(); itr != m_pSelectedCollectionCells.end(); itr++)
		{
			(*itr)->setControlState(CAControlStateNormal);
		}
		m_pSelectedCollectionCells.clear();
	}

	CACollectionViewCell* cell = NULL;
	for (int i = 0; i < m_pCollectionCells.size(); i++)
	{
		CACollectionViewCell* cell = m_pCollectionCells[i];
		CC_BREAK_IF(cell->getRow() == row && cell->getItem() == item);
	}

	if (cell)
	{
		cell->setControlState(CAControlStateSelected);
	}
}

bool CACollectionView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	if (m_pTouches->count() > 0)
	{
		m_pTouches->replaceObjectAtIndex(0, pTouch);
		return true;
	}

	if (!CAScrollView::ccTouchBegan(pTouch, pEvent))
		return false;

	if (m_bAllowsSelection && this->isScrollWindowNotOutSide() == false)
	{
		CCPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);

		for (int i = 0; i < m_pCollectionCells.size(); i++)
		{
			CACollectionViewCell* pCell = m_pCollectionCells[i];
			CCAssert(pCell != NULL, "");

			if (pCell->getFrame().containsPoint(point))
			{
				CC_BREAK_IF(pCell->getControlState() == CAControlStateDisabled);

				if (m_pHighlightedCollectionCells)
				{
					m_pHighlightedCollectionCells->setControlStateNormal();
				}

				m_pHighlightedCollectionCells = pCell;

				CC_BREAK_IF(pCell->getControlState() == CAControlStateSelected);

				CCDelayTime* delayTime = CCDelayTime::create(0.1f);
				CCCallFunc* func = CCCallFunc::create(pCell, callfunc_selector(CACollectionViewCell::setControlStateHighlighted));
				CCSequence* actions = CCSequence::create(delayTime, func, NULL);
				actions->setTag(0xffff);
				this->runAction(actions);
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
		this->stopActionByTag(0xffff);

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
		this->stopActionByTag(0xffff);

		CACollectionViewCell* deselectedCell = NULL;
		CACollectionViewCell* selectedCell = m_pHighlightedCollectionCells;
		m_pHighlightedCollectionCells = NULL;

		if (m_pSelectedCollectionCells.count(selectedCell) > 0 && m_bAllowsMultipleSelection)
		{
			deselectedCell = selectedCell;
			selectedCell = NULL;
			m_pSelectedCollectionCells.erase(deselectedCell);
		}
		else
		{
			if (m_pSelectedCollectionCells.size() > 0 && m_bAllowsMultipleSelection == false)
			{
				deselectedCell = *m_pSelectedCollectionCells.begin();
				m_pSelectedCollectionCells.clear();
			}
			m_pSelectedCollectionCells.insert(selectedCell);
		}

		if (deselectedCell)
		{
			deselectedCell->setControlStateNormal();
			if (m_pCollectionViewDelegate)
			{
				m_pCollectionViewDelegate->collectionViewDidDeselectCellAtIndexPath(this, deselectedCell->getRow(), deselectedCell->getItem());
			}
		}

		if (selectedCell)
		{
			selectedCell->setControlStateSelected();
			if (m_pCollectionViewDelegate)
			{
				m_pCollectionViewDelegate->collectionViewDidSelectCellAtIndexPath(this, selectedCell->getRow(), selectedCell->getItem());
			}
		}
	}

	if (m_pCollectionViewDelegate)
	{
		if (m_pCollectionViewDelegate && m_pContainer->getFrame().origin.y > m_nCollectionPullViewHeight)
		{
			m_pCollectionViewDelegate->collectionViewDidShowPullDownView(this);
		}

		if (m_pCollectionViewDelegate && this->getBounds().size.height - (m_pContainer->getFrame().origin.y + m_pContainer->getFrame().size.height) > m_nCollectionPullViewHeight)
		{
			m_pCollectionViewDelegate->collectionViewDidShowPullUpView(this);
		}
	}
}

void CACollectionView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
	CAScrollView::ccTouchCancelled(pTouch, pEvent);

	if (m_pHighlightedCollectionCells)
	{
		m_pHighlightedCollectionCells->setControlStateNormal();
		m_pHighlightedCollectionCells = NULL;
	}
}

void CACollectionView::update(float fDelta)
{
    CAScrollView::update(fDelta);
    
    CCRect rect = this->getBounds();
    rect.origin.y -= rect.size.height * 0.1f;
    rect.size.height *= 1.2f;
    
    std::vector<CACollectionViewCell*>::iterator itr;
    for (itr=m_pCollectionCells.begin(); itr!=m_pCollectionCells.end(); itr++)
    {
        CACollectionViewCell* cell = *itr;
        CCRect cellRect = cell->getFrame();
        cellRect.origin = m_pContainer->convertToWorldSpace(cellRect.origin);
        cellRect.origin = this->convertToNodeSpace(cellRect.origin);
        
        if (rect.intersectsRect(cellRect))
        {
            cell->setVisible(true);
        }
        else
        {
            cell->setVisible(false);
        }
    }
}

#pragma CACollectionViewCell

CACollectionViewCell::CACollectionViewCell()
: m_nRow(0)
, m_nItem(0)
{
	m_bControl = false;
}


CACollectionViewCell::~CACollectionViewCell()
{
}

CACollectionViewCell* CACollectionViewCell::create(const char* reuseIdentifier)
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

bool CACollectionViewCell::initWithReuseIdentifier(const char* reuseIdentifier)
{
	if (!CAControl::init())
	{
		return false;
	}
	this->setColor(CAColor_clear);
	this->setReuseIdentifier(reuseIdentifier);

	this->setBackGroundViewForState(CAControlStateNormal,
		CAView::createWithFrame(this->getBounds(),
		ccc4(255, 255, 255, 255)));

	this->setBackGroundViewForState(CAControlStateHighlighted,
		CAView::createWithFrame(this->getBounds(),
		ccc4(50, 193, 255, 255)));

	this->setBackGroundViewForState(CAControlStateSelected,
		CAView::createWithFrame(this->getBounds(),
		ccc4(50, 193, 255, 255)));

	this->setControlStateNormal();

	return true;
}

void CACollectionViewCell::setControlState(CAControlState var)
{
    CAControl::setControlState(var);
    
    switch (m_eControlState)
    {
        case CAControlStateNormal:
            this->normalTableViewCell();
            break;
        case CAControlStateHighlighted:
            this->highlightedTableViewCell();
            break;
        case CAControlStateSelected:
            this->selectedTableViewCell();
            break;
        case CAControlStateDisabled:
            this->disabledTableViewCell();
            break;
        default:
            break;
    }
}

NS_CC_END