#include "CAAutoCollectionView.h"
#include "CACollectionView.h"
#include "basics/CAApplication.h"
#include "control/CAButton.h"
#include "basics/CAPointExtension.h"
#include "dispatcher/CATouch.h"
#include "animation/CAViewAnimation.h"

NS_CC_BEGIN

#pragma CACollectionView

CAAutoCollectionView::CAAutoCollectionView()
: m_pCollectionViewDataSource(nullptr)
, m_pCollectionViewDelegate(nullptr)
, m_pCollectionHeaderView(nullptr)
, m_pCollectionFooterView(nullptr)
, m_bAllowsSelection(false)
, m_bAllowsMultipleSelection(false)
, m_pHighlightedCollectionCells(nullptr)
, m_nCollectionHeaderHeight(0)
, m_nCollectionFooterHeight(0)
, m_nHoriCellInterval(5)
, m_nVertCellInterval(5)
, m_iHoriMargins(0)
, m_iVertMargins(0)
, m_bAlwaysTopSectionHeader(true)
, m_bAlwaysBottomSectionFooter(true)
, m_eOrientation(Orientation::Vertical)
, m_eCellHoriAlign(CellHoriAlign::Center)
, m_eCellVertAlign(CellVertAlign::Center)
, m_bLoadData(true)
{
    
}


CAAutoCollectionView::~CAAutoCollectionView()
{
	CC_SAFE_RELEASE_NULL(m_pCollectionHeaderView);
	CC_SAFE_RELEASE_NULL(m_pCollectionFooterView);
	m_pCollectionViewDataSource = nullptr;
	m_pCollectionViewDelegate = nullptr;
}

void CAAutoCollectionView::onEnterTransitionDidFinish()
{
	CAScrollView::onEnterTransitionDidFinish();
}

void CAAutoCollectionView::onExitTransitionDidStart()
{
	CAScrollView::onExitTransitionDidStart();
}

CAAutoCollectionView* CAAutoCollectionView::createWithFrame(const DRect& rect)
{
	CAAutoCollectionView* collectionView = new CAAutoCollectionView();
	if (collectionView && collectionView->initWithFrame(rect))
	{
		collectionView->autorelease();
		return collectionView;
	}
	CC_SAFE_DELETE(collectionView);
	return nullptr;
}

CAAutoCollectionView* CAAutoCollectionView::createWithCenter(const DRect& rect)
{
	CAAutoCollectionView* collectionView = new CAAutoCollectionView();
	if (collectionView && collectionView->initWithCenter(rect))
	{
		collectionView->autorelease();
		return collectionView;
	}
	CC_SAFE_DELETE(collectionView);
	return nullptr;
}

CAAutoCollectionView* CAAutoCollectionView::createWithLayout(const CrossApp::DLayout &layout)
{
    CAAutoCollectionView* collectionView = new CAAutoCollectionView();
    if (collectionView && collectionView->initWithLayout(layout))
    {
        collectionView->autorelease();
        return collectionView;
    }
    CC_SAFE_DELETE(collectionView);
    return nullptr;
}

bool CAAutoCollectionView::init()
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

void CAAutoCollectionView::setContentSize(const CrossApp::DSize &var)
{
    CAScrollView::setContentSize(var);
    
    if (!m_mpUsedCollectionCells.empty())
    {
        this->reloadData();
    }
}

void CAAutoCollectionView::setAllowsSelection(bool var)
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

void CAAutoCollectionView::setAllowsMultipleSelection(bool var)
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

void CAAutoCollectionView::setSelectRowAtIndexPath(unsigned int section, unsigned int item)
{
	CC_RETURN_IF(section >= m_rCollectionViewSection.size());

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

	CAIndexPath3E indexPath = CAIndexPath3E(section, 0, item);
	if (CACollectionViewCell* cell = m_mpUsedCollectionCells.at(indexPath))
	{
		cell->setControlState(CAControl::State::Selected);
	}
	m_pSelectedCollectionCells.insert(indexPath);
}

void CAAutoCollectionView::setUnSelectRowAtIndexPath(unsigned int section, unsigned int item)
{
	CC_RETURN_IF(section >= m_rCollectionViewSection.size());

	CAIndexPath3E indexPath = CAIndexPath3E(section, 0, item);
	CC_RETURN_IF(m_pSelectedCollectionCells.find(indexPath) == m_pSelectedCollectionCells.end());
	if (CACollectionViewCell* cell = m_mpUsedCollectionCells.at(indexPath))
	{
		cell->setControlState(CAControl::State::Normal);
	}
	m_pSelectedCollectionCells.erase(indexPath);
}

void CAAutoCollectionView::setShowsScrollIndicators(bool var)
{
    this->setShowsVerticalScrollIndicator(var);
    m_bShowsScrollIndicators = var;
}

CACollectionViewCell* CAAutoCollectionView::cellForRowAtIndexPath(unsigned int section, unsigned int item)
{
    CAIndexPath3E indexPath = CAIndexPath3E(section, 0, item);
    if (m_mpUsedCollectionCells.find(indexPath) == m_mpUsedCollectionCells.end())
    {
        return nullptr;
    }
    return m_mpUsedCollectionCells[indexPath];
}

const CAVector<CACollectionViewCell*>& CAAutoCollectionView::displayingCollectionCell()
{
    return m_vpUsedCollectionCells;
}


void CAAutoCollectionView::setOrientation(CAAutoCollectionView::Orientation var)
{
	m_eOrientation = var;
    bool bVertScroll = m_eOrientation == CAAutoCollectionView::Orientation::Vertical;

	setShowsVerticalScrollIndicator(bVertScroll);
	setBounceVertical(bVertScroll);
	setShowsHorizontalScrollIndicator(!bVertScroll);
	setBounceHorizontal(!bVertScroll);
	setHorizontalScrollEnabled(!bVertScroll);
	setVerticalScrollEnabled(bVertScroll);
    
    if(!m_mpUsedCollectionCells.empty())
    {
        this->reloadData();
    }
}

CAAutoCollectionView::Orientation CAAutoCollectionView::getOrientation()
{
	return m_eOrientation;
}

bool CAAutoCollectionView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
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

void CAAutoCollectionView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
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

void CAAutoCollectionView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
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
                m_obDidDeselectCellAtIndexPath(deselectedIndexPath.section, deselectedIndexPath.item);
            }
            else if (m_pCollectionViewDelegate)
			{
				m_pCollectionViewDelegate->collectionViewDidDeselectCellAtIndexPath(this, deselectedIndexPath.section, deselectedIndexPath.item);
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
                m_obDidSelectCellAtIndexPath(selectedIndexPath.section, selectedIndexPath.item);
            }
			else if (m_pCollectionViewDelegate)
			{
				m_pCollectionViewDelegate->collectionViewDidSelectCellAtIndexPath(this, selectedIndexPath.section, selectedIndexPath.item);
			}
		}
        
        m_pHighlightedCollectionCells = nullptr;
	}
}

void CAAutoCollectionView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
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

void CAAutoCollectionView::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
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
                                                        0,
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

void CAAutoCollectionView::mouseMovedOutSide(CATouch* pTouch, CAEvent* pEvent)
{
    if (m_pHighlightedCollectionCells)
    {
        CAIndexPath3E index = CAIndexPath3E(m_pHighlightedCollectionCells->getSection(),
                                            0,
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

void CAAutoCollectionView::switchPCMode(bool var)
{
    CAScrollView::switchPCMode(var);
    this->setMouseMovedEnabled(true);
}

bool CAAutoCollectionView::fillSectionRowData(CollectionViewRow& r, DSize rSize)
{
    if (m_eOrientation == CAAutoCollectionView::Orientation::Vertical)
	{
		int dw = m_obContentSize.width - 2 * m_iHoriMargins;

		if (r.iIniValue + rSize.width > dw && r.iMaxValue > 0)
			return false;

		if (r.iMaxValue < rSize.height)
			r.iMaxValue = rSize.height;
		
		if (r.iIniValue > 0)
		{
			r.iIniValue += m_nHoriCellInterval;
		}
		r.rItemRects.push_back(DRect(m_iHoriMargins + r.iIniValue, 0, rSize.width, rSize.height));

		r.iIniValue += rSize.width;
	}
	else
	{
		int dw = m_obContentSize.height - 2 * m_iVertMargins;

		if (r.iIniValue + rSize.height > dw && r.iMaxValue > 0)
			return false;

		if (r.iMaxValue < rSize.width)
			r.iMaxValue = rSize.width;

		if (r.iIniValue > 0)
		{
			r.iIniValue += m_nVertCellInterval;
		}
		r.rItemRects.push_back(DRect (0, m_iVertMargins + r.iIniValue, rSize.width, rSize.height));

		r.iIniValue += rSize.height;
	}
	
	return true;
}

void CAAutoCollectionView::reloadViewSizeData()
{    
	clearData();

    unsigned int nSections = 1;
    if (m_obNumberOfSections)
    {
        nSections = m_obNumberOfSections();
    }
    else if (m_pCollectionViewDataSource)
    {
        nSections = m_pCollectionViewDataSource->numberOfSections(this);
    }
	m_rCollectionViewSection.resize(nSections);
    
	CollectionViewRow r;
	for (unsigned int i = 0; i<nSections; i++)
    {
		CollectionViewSection& cvs = m_rCollectionViewSection[i];
        cvs.nSectionHeaderHeight = 0;
        if (m_obHeightForHeaderInSection)
        {
            cvs.nSectionHeaderHeight = m_obHeightForHeaderInSection(i);
        }
        else if (m_pCollectionViewDataSource)
        {
            cvs.nSectionHeaderHeight = m_pCollectionViewDataSource->collectionViewHeightForHeaderInSection(this, i);
        }
        
        cvs.nSectionFooterHeight = 0;
        if (m_obHeightForFooterInSection)
        {
            cvs.nSectionFooterHeight = m_obHeightForFooterInSection(i);
        }
        else if (m_pCollectionViewDataSource)
        {
            cvs.nSectionFooterHeight = m_pCollectionViewDataSource->collectionViewHeightForFooterInSection(this, i);
        }
        
        unsigned int itemCount = 0;
        if (m_obNumberOfItemsAtIndexPath)
        {
            itemCount = m_obNumberOfItemsAtIndexPath(i);
        }
        else if (m_pCollectionViewDataSource)
        {
            itemCount = m_pCollectionViewDataSource->numberOfItemsInSection(this, i);
        }
		for (int j = 0; j < itemCount; j++)
		{
            DSize rSize = DSizeZero;
            if (m_obCellSizeAtIndexPath)
            {
                rSize = m_obCellSizeAtIndexPath(i, j);
            }
            else if (m_pCollectionViewDataSource)
            {
                rSize = m_pCollectionViewDataSource->collectionViewCellSizeAtIndexPath(this, i, j);
            }
            
			if (!fillSectionRowData(r, rSize))
			{
				cvs.CollectionViewRows.push_back(r);
				r = CollectionViewRow();
				j--;
			}
		}
		if (r.iMaxValue > 0)
		{
			cvs.CollectionViewRows.push_back(r);
			r = CollectionViewRow();
		}
    }
    
	unsigned int cellXValue = (m_eOrientation == CAAutoCollectionView::Orientation::Vertical) ? m_nVertCellInterval : m_nHoriCellInterval;
	unsigned int viewHeight = (m_eOrientation == CAAutoCollectionView::Orientation::Vertical) ? (2 * m_iVertMargins) : (2 * m_iHoriMargins);

	for (int i = 0; i < nSections; i++)
	{
		CollectionViewSection& cvs = m_rCollectionViewSection[i];

		unsigned int sectionHeight = 0;
		if (cvs.nSectionHeaderHeight>0)
		{
			sectionHeight += cvs.nSectionHeaderHeight;
		}
		sectionHeight += cellXValue;
	
		for (unsigned int j = 0; j < cvs.CollectionViewRows.size(); j++)
		{
			sectionHeight += cvs.CollectionViewRows[j].iMaxValue;
			sectionHeight += cellXValue;
		}
		
		if (cvs.nSectionFooterHeight>0)
		{
			sectionHeight += cvs.nSectionFooterHeight;
		}
		sectionHeight += cellXValue;

		viewHeight += sectionHeight;
	}

	viewHeight += m_nCollectionHeaderHeight;
	viewHeight += m_nCollectionFooterHeight;

	DSize size = m_obContentSize;
	if (m_eOrientation == CAAutoCollectionView::Orientation::Vertical)
	{
		size.height = viewHeight;
	}
	else
	{
		size.width = viewHeight;
	}
	this->setViewSize(size);
}

void CAAutoCollectionView::clearData()
{
	m_mpUsedCollectionCells.clear();
	m_rUsedCollectionCellRects.clear();
	m_rCollectionViewSection.clear();

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


int CAAutoCollectionView::calculateAllCellsLength(CollectionViewSection& cvs)
{
	int iMaxLengthValue = 0;
	for (int i = 0; i < cvs.CollectionViewRows.size(); i++)
	{
		CollectionViewRow& r = cvs.CollectionViewRows[i];

		if (r.iIniValue > iMaxLengthValue)
		{
			iMaxLengthValue = r.iIniValue;
		}
	}
	return iMaxLengthValue;
}

int CAAutoCollectionView::calculateAllCells(CollectionViewSection& cvs, int index, int dd, int dv, int dw)
{
	int iMaxLengthValue = calculateAllCellsLength(cvs);

	for (int j = 0, l = 0; j < cvs.CollectionViewRows.size(); j++)
	{
		CollectionViewRow& r = cvs.CollectionViewRows[j];

		if (j>0)
		{
			dd += dv;
		}
		for (int k = 0; k < r.rItemRects.size(); k++, l++)
		{
			CAIndexPath3E indexPath = CAIndexPath3E(index, 0, l);

			DRect& cellRect = r.rItemRects[k];

            if (m_eOrientation == CAAutoCollectionView::Orientation::Vertical)
			{
				cellRect.origin.y = dd;
				if (m_eCellVertAlign == CAAutoCollectionView::CellVertAlign::Center)
				{
					int d = (r.iMaxValue - r.rItemRects[k].size.height) / 2;
					cellRect.origin.y += d;
				}
				if (m_eCellVertAlign == CAAutoCollectionView::CellVertAlign::Bottom)
				{
					int d = r.iMaxValue - r.rItemRects[k].size.height;
					cellRect.origin.y += d;
				}
				if (m_eCellHoriAlign == CAAutoCollectionView::CellHoriAlign::Center)
				{
					int d = (dw - iMaxLengthValue) / 2;
					cellRect.origin.x += d;
				}
				if (m_eCellHoriAlign == CAAutoCollectionView::CellHoriAlign::Right)
				{
					int d = dw - iMaxLengthValue;
					cellRect.origin.x += d;
				}
			}
			else
			{
				if (m_eCellVertAlign == CAAutoCollectionView::CellVertAlign::Center)
				{
					int d = (dw - iMaxLengthValue) / 2;
					cellRect.origin.y += d;
				}
				if (m_eCellVertAlign == CAAutoCollectionView::CellVertAlign::Bottom)
				{
					int d = dw - iMaxLengthValue;
					cellRect.origin.y += d;
				}

				cellRect.origin.x = dd;
				if (m_eCellHoriAlign == CAAutoCollectionView::CellHoriAlign::Center)
				{
					int d = (r.iMaxValue - r.rItemRects[k].size.width) / 2;
					cellRect.origin.x += d;
				}
				if (m_eCellHoriAlign == CAAutoCollectionView::CellHoriAlign::Right)
				{
					int d = r.iMaxValue - r.rItemRects[k].size.width;
					cellRect.origin.x += d;
				}
			}
			m_rUsedCollectionCellRects[indexPath] = cellRect;

			m_mpUsedCollectionCells.insert(std::make_pair(indexPath, (CACollectionViewCell*)nullptr));
		}
		
		dd += r.iMaxValue;
	}
	return dd;
}

int CAAutoCollectionView::calculateAllRects()
{
	int dw = 0, dd = 0, dv = 0;
    if (m_eOrientation == CAAutoCollectionView::Orientation::Vertical)
	{
		dw = m_obContentSize.width - 2 * m_iHoriMargins;
		dd = m_iVertMargins + m_nCollectionHeaderHeight;
		dv = m_nVertCellInterval;
	}
	else
	{
		dw = m_obContentSize.height - 2 * m_iVertMargins;
		dd = m_iHoriMargins + m_nCollectionHeaderHeight;
		dv = m_nHoriCellInterval;
	}

	for (int i = 0; i < m_rCollectionViewSection.size(); i++)
	{
		CollectionViewSection& cvs = m_rCollectionViewSection[i];

		unsigned int iSectionHeaderHeight = cvs.nSectionHeaderHeight;

		DRect sectionHeaderRect;
		if (m_eOrientation == CAAutoCollectionView::Orientation::Vertical)
		{
			sectionHeaderRect = DRect(m_iHoriMargins, dd, dw, iSectionHeaderHeight);
		}
		else
		{
			sectionHeaderRect = DRect(dd, m_iVertMargins, iSectionHeaderHeight, dw);
		}

		if (iSectionHeaderHeight > 0)
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
				insertSubview(pSectionHeaderView, 1);
			}
			dd += iSectionHeaderHeight;
			cvs.pSectionHeaderView = pSectionHeaderView;
		}
		dd += dv;
		dd = calculateAllCells(cvs, i, dd, dv, dw);

		unsigned int iSectionFooterHeight = cvs.nSectionFooterHeight;

		DRect sectionFooterRect;
		if (m_eOrientation == CAAutoCollectionView::Orientation::Vertical)
		{
			sectionFooterRect = DRect(m_iHoriMargins, dd, dw, iSectionFooterHeight);
		}
		else
		{
			sectionFooterRect = DRect(dd, m_iVertMargins, iSectionFooterHeight, dw);
		}

		if (iSectionFooterHeight > 0)
		{
            CAView* pSectionFooterView = nullptr;
            if (m_obSectionViewForFooterInSection)
            {
                pSectionFooterView = m_obSectionViewForFooterInSection(sectionHeaderRect.size, i);
            }
            else if (m_pCollectionViewDataSource)
            {
                pSectionFooterView = m_pCollectionViewDataSource->collectionViewSectionViewForFooterInSection(this, sectionFooterRect.size, i);
                
            }
			if (pSectionFooterView != nullptr)
			{
				pSectionFooterView->setDisplayRange(true);
				pSectionFooterView->setFrame(sectionFooterRect);
				insertSubview(pSectionFooterView, 1);
			}
			dd += iSectionFooterHeight;
			cvs.pSectionFooterView = pSectionFooterView;
		}
		dd += dv;

		DRect sectionRect = sectionHeaderRect;
		if (m_eOrientation == CAAutoCollectionView::Orientation::Vertical)
		{
			sectionRect.size.height = sectionFooterRect.origin.y + sectionFooterRect.size.height - sectionHeaderRect.origin.y;
		}
		else
		{
			sectionRect.size.width = sectionFooterRect.origin.x + sectionFooterRect.size.width - sectionHeaderRect.origin.x;
		}
		cvs.rSectionRect = sectionRect;
	}
	
	return dd;
}

void CAAutoCollectionView::reloadData()
{
    m_bLoadData = true;
    this->updateDraw();
}

void CAAutoCollectionView::recoveryCollectionCell()
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

void CAAutoCollectionView::loadCollectionCell()
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
            cell = m_obCellAtIndexPath(cellRect.size, r.section, r.item);
        }
        else if (m_pCollectionViewDataSource)
        {
            cell = m_pCollectionViewDataSource->collectionCellAtIndex(this, cellRect.size, r.section, r.item);
        }
		if (cell)
		{
			cell->m_nSection = r.section;
			cell->m_nRow = 0;
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
                m_obWillDisplayCellAtIndexPath(cell, r.section, r.item);
            }
            else if (m_pCollectionViewDataSource)
            {
                m_pCollectionViewDataSource->collectionViewWillDisplayCellAtIndex(this, cell, r.section, r.item);
            }
		}
	}
}

void CAAutoCollectionView::updateSectionHeaderAndFooterRects()
{
	DRect rect = this->getBounds();
	rect.origin = getContentOffset();

	if (m_eOrientation == CAAutoCollectionView::Orientation::Vertical)
	{
		rect.origin.x = m_iHoriMargins;
	}
	else
	{
		rect.origin.x = m_iVertMargins;
	}

	for (int i = 0; i < m_rCollectionViewSection.size(); i++)
	{
		CollectionViewSection& cvs = m_rCollectionViewSection[i];

		CC_CONTINUE_IF(!rect.intersectsRect(cvs.rSectionRect));

		CAView* header = cvs.pSectionHeaderView;
		CAView* footer = cvs.pSectionFooterView;
		
		float headerHeight = cvs.nSectionHeaderHeight;
		float footerHeight = cvs.nSectionFooterHeight;

		if (header && m_bAlwaysTopSectionHeader)
		{
			DPoint p1 = rect.origin;
			p1.y = MAX(p1.y, cvs.rSectionRect.origin.y);
			p1.y = MIN(p1.y, cvs.rSectionRect.origin.y + cvs.rSectionRect.size.height
				- headerHeight - footerHeight);
			header->setFrameOrigin(p1);
		}
		if (footer && m_bAlwaysBottomSectionFooter)
		{
			DPoint p2 = rect.origin;
			p2.y = MIN(rect.origin.y + m_obContentSize.height - footerHeight,
				cvs.rSectionRect.origin.y + cvs.rSectionRect.size.height - footerHeight);
			p2.y = MAX(p2.y, cvs.rSectionRect.origin.y + headerHeight);
			footer->setFrameOrigin(p2);
		}
	}
}

void CAAutoCollectionView::update(float dt)
{
	CAScrollView::update(dt);
    
	recoveryCollectionCell();
    
	loadCollectionCell();
    
	updateSectionHeaderAndFooterRects();
}

void CAAutoCollectionView::visitEve()
{
    if (m_bLoadData)
    {
        m_bLoadData = false;
        
        this->reloadViewSizeData();
        
        this->removeAllSubviews();
        
        DRect winRect = this->getBounds();
        winRect.origin = getContentOffset();
        
        int dw = winRect.size.width - 2 * ((m_eOrientation == CAAutoCollectionView::Orientation::Vertical) ? m_iHoriMargins : m_iVertMargins);
        
        if (m_nCollectionHeaderHeight > 0 && m_pCollectionHeaderView)
        {
            if (m_eOrientation == CAAutoCollectionView::Orientation::Vertical)
            {
                m_pCollectionHeaderView->setFrame(DRect(m_iHoriMargins, m_iVertMargins, dw, m_nCollectionHeaderHeight));
            }
            else
            {
                m_pCollectionHeaderView->setFrame(DRect(m_iHoriMargins, m_iVertMargins, m_nCollectionHeaderHeight, dw));
            }
            this->addSubview(m_pCollectionHeaderView);
        }
        
        int dwValue = calculateAllRects();
        
        if (m_nCollectionFooterHeight > 0 && m_pCollectionFooterView)
        {
            if (m_eOrientation == CAAutoCollectionView::Orientation::Vertical)
            {
                m_pCollectionFooterView->setFrame(DRect(m_iHoriMargins, dwValue, dw, m_nCollectionFooterHeight));
            }
            else
            {
                m_pCollectionFooterView->setFrame(DRect(dwValue, m_iVertMargins, m_nCollectionFooterHeight, dw));
            }
            this->addSubview(m_pCollectionFooterView);
        }
        
        this->loadCollectionCell();
        this->updateSectionHeaderAndFooterRects();
        this->layoutPullToRefreshView();
        
        if (this->isScrollWindowNotOutSide())
        {
            this->startDeaccelerateScroll();
        }
    }
    CAScrollView::visitEve();
}

float CAAutoCollectionView::maxSpeed(float dt)
{
    return (128 * 60 * dt);
}

float CAAutoCollectionView::decelerationRatio(float dt)
{
    return 1.8f * dt;
}

CACollectionViewCell* CAAutoCollectionView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
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

CACollectionViewCell* CAAutoCollectionView::getHighlightCollectionCell()
{
    return m_pHighlightedCollectionCells;
}

NS_CC_END
