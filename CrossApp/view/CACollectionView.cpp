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

float CACollectionView::maxSpeed(float dt)
{
    return (CCPoint(m_obContentSize).getLength() * 6 * dt);
}

float CACollectionView::maxSpeedCache(float dt)
{
    return (maxSpeed(dt) * 3.0f);
}

float CACollectionView::decelerationRatio(float dt)
{
    return 3.0f * dt;
}

CCPoint CACollectionView::maxBouncesLenght()
{
    //PULL SHOW VIEW
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

	if (m_nCollectionHeaderHeight > 0 && m_pCollectionHeaderView)
	{
		m_pCollectionHeaderView->setDisplayRange(true);
        m_pCollectionHeaderView->setFrame(CCRect(0, y, width, m_nCollectionHeaderHeight));
        addSubview(m_pCollectionHeaderView);
        y += m_nCollectionHeaderHeight;
	}

	unsigned int sectionCount = m_pCollectionViewDataSource->numberOfSectionsInCollectioView();
	for (int i = 0; i < sectionCount; i++)
	{
		unsigned int iSectionHeaderHeight = m_pCollectionViewDataSource->collectionViewHeightForHeaderInSection(this, i);
		if (iSectionHeaderHeight>0)
		{
            CCRect sectionHeaderRect = CCRect(0, y, width, iSectionHeaderHeight);
			CAView* pSectionHeaderView = m_pCollectionViewDataSource->collectionViewSectionViewForHeaderInSection(this, sectionHeaderRect.size, i);
			if (pSectionHeaderView!=NULL)
			{
				pSectionHeaderView->setDisplayRange(true);
				pSectionHeaderView->setFrame(sectionHeaderRect);
				addSubview(pSectionHeaderView);
				y += iSectionHeaderHeight;
			}
		}

		unsigned int rowCount = m_pCollectionViewDataSource->numberOfRowsInSectionCollectionView(this, i);
		for (int j = 0; j < rowCount; j++)
		{
			int iHeight = m_pCollectionViewDataSource->collectionViewHeightForRowAtIndexPath(this, i, j);

			unsigned int itemCount = m_pCollectionViewDataSource->numberOfItemsInRowsInSection(this, i, j);
			for (int k = 0; k < itemCount; k++)
			{
				CCRect cellRect = CCRect(width*k / itemCount, y, width / itemCount, iHeight);
				CACollectionViewCell* pCellView = m_pCollectionViewDataSource->collectionCellAtIndex(this, cellRect.size, i, j, k);
				pCellView->setFrame(cellRect);
				addSubview(pCellView);
                pCellView->m_nSection = i;
				pCellView->m_nRow = j;
				pCellView->m_nItem = k;
				m_pCollectionCells.push_back(pCellView);
			}
			y += iHeight;
		}

		unsigned int iSectionFooterHeight = m_pCollectionViewDataSource->collectionViewHeightForFooterInSection(this, i);
		if (iSectionFooterHeight>0)
		{
            CCRect sectionFooterRect = CCRectMake(0, y, width, iSectionFooterHeight);
			CAView* pSectionFooterView = m_pCollectionViewDataSource->collectionViewSectionViewForFooterInSection(this, sectionFooterRect.size, i);
			if (pSectionFooterView != NULL)
			{
				pSectionFooterView->setDisplayRange(true);
				pSectionFooterView->setFrame(sectionFooterRect);
				addSubview(pSectionFooterView);
				y += iSectionFooterHeight;
			}
		}
	}

	if (m_nCollectionFooterHeight > 0 && m_pCollectionHeaderView)
	{
		m_pCollectionFooterView->setDisplayRange(true);
        m_pCollectionFooterView->setFrame(CCRect(0, y, width, m_nCollectionFooterHeight));
        addSubview(m_pCollectionFooterView);
        y += m_nCollectionFooterHeight;
	}

    this->setViewSize(CCSize(width, y));
}


void CACollectionView::setSelectRowAtIndexPath(unsigned int section, unsigned int row, unsigned int item)
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
		cell = m_pCollectionCells[i];
		CC_BREAK_IF(cell->getSection() == section && cell->getRow() == row && cell->getItem() == item);
	}

	if (cell)
	{
		cell->setControlState(CAControlStateSelected);
        m_pSelectedCollectionCells.insert(cell);
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
				m_pCollectionViewDelegate->collectionViewDidDeselectCellAtIndexPath(this,
                                                                                    deselectedCell->getSection(),
                                                                                    deselectedCell->getRow(),
                                                                                    deselectedCell->getItem());
			}
		}

		if (selectedCell)
		{
			selectedCell->setControlStateSelected();
			if (m_pCollectionViewDelegate)
			{
				m_pCollectionViewDelegate->collectionViewDidSelectCellAtIndexPath(this,
                                                                                  deselectedCell->getSection(),
                                                                                  selectedCell->getRow(),
                                                                                  selectedCell->getItem());
			}
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

void CACollectionView::update(float dt)
{
    CAScrollView::update(dt);
    
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
: m_nSection(0)
, m_nRow(0)
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