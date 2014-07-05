//
//  CATableView.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-28.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CATableView.h"
#include "CAImageView.h"
#include "CAScale9ImageView.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionInterval.h"
#include "dispatcher/CATouch.h"
#include "support/CCPointExtension.h"
#include "CCEGLView.h"
NS_CC_BEGIN

#pragma CATableView

CATableView::CATableView()
:m_pTableHeaderView(NULL)
,m_pTableFooterView(NULL)
,m_pTablePullDownView(NULL)
,m_pTablePullUpView(NULL)
,m_separatorColor(CAColor_gray)
,m_nSeparatorViewHeight(1)
,m_nTableHeaderHeight(0)
,m_nTableFooterHeight(0)
,m_nTablePullViewHeight(0)
,m_pTableViewDataSource(NULL)
,m_pTableViewDelegate(NULL)
,m_pHighlightedTableCells(NULL)
,m_bAllowsSelection(false)
,m_bAllowsMultipleSelection(false)
,m_bToUpdate(CATableViewToUpdateNone)
{
    m_pCellDict = new CCDictionary();
}

CATableView::~CATableView()
{
    CC_SAFE_DELETE(m_pCellDict);
    CC_SAFE_RELEASE_NULL(m_pTableHeaderView);
    CC_SAFE_RELEASE_NULL(m_pTableFooterView);
    CC_SAFE_RELEASE_NULL(m_pTablePullDownView);
    CC_SAFE_RELEASE_NULL(m_pTablePullUpView);
    m_pTableViewDataSource = NULL;
    m_pTableViewDelegate = NULL;
}

void CATableView::onEnterTransitionDidFinish()
{
    CAScrollView::onEnterTransitionDidFinish();
    
    if (m_pTableCells.empty())
    {
        this->runAction(CCSequence::create(CCDelayTime::create(1/60.0f),
                                           CCCallFunc::create(this, callfunc_selector(CATableView::reloadData)), NULL));
    }
}

void CATableView::onExitTransitionDidStart()
{
    CAScrollView::onExitTransitionDidStart();
}

CATableView* CATableView::createWithFrame(const CCRect& rect)
{
    CATableView* tableView = new CATableView();
    if (tableView && tableView->initWithFrame(rect))
    {
        tableView->autorelease();
        return tableView;
    }
    CC_SAFE_DELETE(tableView);
    return NULL;
}

CATableView* CATableView::createWithCenter(const CCRect& rect)
{
    CATableView* tableView = new CATableView();
    if (tableView && tableView->initWithCenter(rect))
    {
        tableView->autorelease();
        return tableView;
    }
    CC_SAFE_DELETE(tableView);
    return NULL;
}

bool CATableView::init()
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

void CATableView::setContentSize(const CrossApp::CCSize &var)
{
    CAScrollView::setContentSize(var);
}

bool CATableView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
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
        
        std::deque<CATableViewCell*>::iterator itr;
        for (itr=m_pTableCells.begin(); itr!=m_pTableCells.end(); itr++)
        {
            CATableViewCell* cell = *itr;
            if (cell->getFrame().containsPoint(point))
            {
                CC_BREAK_IF(cell->getControlState() == CAControlStateDisabled);
                
                if (m_pHighlightedTableCells)
                {
                    m_pHighlightedTableCells->setControlStateNormal();
                }
                
                m_pHighlightedTableCells = cell;

                CC_BREAK_IF(cell->getControlState() == CAControlStateSelected);
                
                CCDelayTime* delayTime = CCDelayTime::create(0.1f);
                CCCallFunc* func = CCCallFunc::create(cell, callfunc_selector(CATableViewCell::setControlStateHighlighted));
                CCSequence* actions = CCSequence::create(delayTime, func, NULL);
                actions->setTag(0xffff);
                this->runAction(actions);
                break;
            }
        }
    }
    return true;
}

void CATableView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_bscrollEnabled == false);
    
    CAScrollView::ccTouchMoved(pTouch, pEvent);
    
    if (m_pHighlightedTableCells)
    {
        this->stopActionByTag(0xffff);
        
        if (m_pHighlightedTableCells->getControlState() == CAControlStateHighlighted)
        {
            m_pHighlightedTableCells->setControlStateNormal();
        }
        
        m_pHighlightedTableCells = NULL;
    }
    
}

void CATableView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CAScrollView::ccTouchEnded(pTouch, pEvent);
    
    if (m_pHighlightedTableCells)
    {
        this->stopActionByTag(0xffff);
        
        CATableViewCell* deselectedCell = NULL;
        CATableViewCell* selectedCell = m_pHighlightedTableCells;
        m_pHighlightedTableCells = NULL;
        
        if (m_pSelectedTableCells.count(selectedCell) > 0 && m_bAllowsMultipleSelection)
        {
            deselectedCell = selectedCell;
            selectedCell = NULL;
            m_pSelectedTableCells.erase(deselectedCell);
        }
        else
        {
            if (m_pSelectedTableCells.size() > 0 && m_bAllowsMultipleSelection == false)
            {
                deselectedCell = *m_pSelectedTableCells.begin();
                m_pSelectedTableCells.clear();
            }
            m_pSelectedTableCells.insert(selectedCell);
        }
        
        if (deselectedCell)
        {
            deselectedCell->setControlStateNormal();
            if (m_pTableViewDelegate)
            {
                m_pTableViewDelegate->tableViewDidDeselectRowAtIndexPath(this, deselectedCell->getSection(), deselectedCell->getRow());
            }
        }
        
        if (selectedCell)
        {
            selectedCell->setControlStateSelected();
            if (m_pTableViewDelegate)
            {
                m_pTableViewDelegate->tableViewDidSelectRowAtIndexPath(this, selectedCell->getSection(), selectedCell->getRow());
            }
        }
    }
    
    if (m_pTableViewDelegate && m_nTablePullViewHeight > 0)
    {
        CCPoint point  = m_pContainer->getFrameOrigin();
        
        if (m_pTablePullDownView
            && point.y
            > m_nTablePullViewHeight)
        {
            m_bToUpdate     =   CATableViewToUpdatePullDown;
            
            point.y         =   m_nTablePullViewHeight;
            
            this->setContentOffset(ccpMult(point, -1), true);
        }
        
        if (m_pTablePullUpView
            && point.y
            < this->getBounds().size.height
                - m_pContainer->getFrame().size.height
                - m_nTablePullViewHeight)
        {
            m_bToUpdate     =   CATableViewToUpdatePullUp;
            
            point.y         =   this->getBounds().size.height
                                - m_pContainer->getFrame().size.height
                                - m_nTablePullViewHeight;
            
            this->setContentOffset(ccpMult(point, -1), true);
        }
    }
}

void CATableView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    CAScrollView::ccTouchCancelled(pTouch, pEvent);
    
    if (m_pHighlightedTableCells)
    {
        m_pHighlightedTableCells->setControlStateNormal();
        m_pHighlightedTableCells = NULL;
    }
}

float CATableView::maxSpeed(float dt)
{
    return (CCPoint(m_obContentSize).getLength() * 6 * dt);
}

float CATableView::maxSpeedCache(float dt)
{
    return (maxSpeed(dt) * 3.0f);
}

float CATableView::decelerationRatio(float dt)
{
    return 3.0f * dt;
}

CCPoint CATableView::maxBouncesLenght()
{
    if (m_pTablePullDownView || m_pTablePullUpView)
    {
        return CCPoint(0, MIN(m_obContentSize.height/2, m_nTablePullViewHeight * 0.8f));
    }
    
    return CCPoint(0, m_obContentSize.height/2);
}

void CATableView::contentOffsetFinish()
{
    CC_RETURN_IF(m_bToUpdate == CATableViewToUpdateNone);
    switch (m_bToUpdate)
    {
        case CATableViewToUpdatePullDown:
            m_pTableViewDelegate->tableViewDidShowPullDownView(this);
            break;
        case CATableViewToUpdatePullUp:
            m_pTableViewDelegate->tableViewDidShowPullUpView(this);
            break;
        default:
            break;
    }
    m_bToUpdate = CATableViewToUpdateNone;
    this->reloadData();
    CAScheduler::schedule(schedule_selector(CATableView::deaccelerateScrolling), this, 1/60.0f, kCCRepeatForever, 0.3f);
}

CATableViewCell* CATableView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
{
    
    return NULL;
}

void CATableView::setSelectRowAtIndexPath(unsigned int section, unsigned int row)
{
    if (!m_pSelectedTableCells.empty())
    {
        std::set<CATableViewCell*>::iterator itr;
        for (itr=m_pSelectedTableCells.begin(); itr!=m_pSelectedTableCells.end(); itr++)
        {
            (*itr)->setControlState(CAControlStateNormal);
        }
        m_pSelectedTableCells.clear();
    }
    
    CATableViewCell* cell = NULL;
    std::deque<CATableViewCell*>::iterator itr;
    for (itr=m_pTableCells.begin(); itr!=m_pTableCells.end(); itr++)
    {
        CATableViewCell* cell = *itr;
        CC_BREAK_IF(cell->getSection() == section && cell->getRow() == row);
    }
    
    if (cell)
    {
        cell->setControlState(CAControlStateSelected);
    }
}

void CATableView::clearData()
{
    m_nRowsInSections.clear();
    m_nSectionHeaderHeights.clear();
    m_nSectionFooterHeights.clear();
    std::vector<std::vector<unsigned int> >::iterator itr;
    for (itr=m_nRowHeightss.begin(); itr!=m_nRowHeightss.end(); itr++)
    {
        itr->clear();
    }
    m_nRowHeightss.clear();
    
    m_rSectionRectss.clear();
    m_rTableCellRectss.clear();
    
    std::set<CATableViewCell*>::iterator itr2;
    for (itr2=m_pSelectedTableCells.begin(); itr2!=m_pSelectedTableCells.end(); itr2++)
    {
        CATableViewCell* cell = *itr2;
        cell->setControlStateNormal();
    }
    m_pSelectedTableCells.clear();
    
    m_pTableCells.clear();
}

void CATableView::reloadViewSizeData()
{
    this->clearData();
    
    unsigned int sectionCount = m_pTableViewDataSource->numberOfSections(this);
    m_nRowsInSections.resize(sectionCount);
    for (unsigned int i=0; i<sectionCount; i++)
    {
        unsigned int rowsInSection = m_pTableViewDataSource->numberOfRowsInSection(this, i);
        m_nRowsInSections[i] = rowsInSection;
    }
    
    m_nSectionHeaderHeights.resize(sectionCount);
    for (unsigned int i=0; i<sectionCount; i++)
    {
        unsigned int sectionHeaderHeight = m_pTableViewDataSource->tableViewHeightForHeaderInSection(this, i);
        m_nSectionHeaderHeights[i] = sectionHeaderHeight;
    }
    
    m_nSectionFooterHeights.resize(sectionCount);
    for (unsigned int i=0; i<sectionCount; i++)
    {
        unsigned int sectionFooterHeight = m_pTableViewDataSource->tableViewHeightForFooterInSection(this, i);
        m_nSectionFooterHeights[i] = sectionFooterHeight;
    }
    
    m_nRowHeightss.resize(sectionCount);
    for (unsigned int i=0; i<sectionCount; i++)
    {
        std::vector<unsigned int> rowHeights(m_nRowsInSections.at(i));
        for (unsigned int j=0; j<m_nRowsInSections.at(i); j++)
        {
            unsigned int rowHeight = m_pTableViewDataSource->tableViewHeightForRowAtIndexPath(this, i, j);
            rowHeights[j] = rowHeight;
        }
        m_nRowHeightss[i] = rowHeights;
    }
    
    unsigned int viewHeight = 0;
    
    m_nSectionHeights.resize(sectionCount);
    for (unsigned int i=0; i<sectionCount; i++)
    {
        unsigned int sectionHeight = 0;
        sectionHeight += m_nSectionHeaderHeights.at(i);
        sectionHeight += m_nSectionFooterHeights.at(i);
        for (unsigned int j=0; j<m_nRowHeightss.at(i).size(); j++)
        {
            sectionHeight += m_nRowHeightss.at(i).at(j);
            sectionHeight += m_nSeparatorViewHeight;
        }
        sectionHeight -= m_nSeparatorViewHeight;
        m_nSectionHeights[i] = sectionHeight;
        viewHeight += sectionHeight;
    }
    
    viewHeight += m_nTableHeaderHeight;
    viewHeight += m_nTableFooterHeight;

    CCSize size = this->getBounds().size;
    size.height = viewHeight;
    this->setViewSize(size);
}

void CATableView::reloadData()
{
    this->reloadViewSizeData();

    this->removeAllSubviews();
    
    float width = this->getBounds().size.width;
    int y = 0;
    
    if (m_pTablePullDownView)
    {
        m_pTablePullDownView->setFrame(CCRect(0, -(float)m_nTablePullViewHeight, width, m_nTablePullViewHeight));
        this->addSubview(m_pTablePullDownView);
    }
    
    if (m_pTableHeaderView)
    {
        m_pTableHeaderView->setDisplayRange(true);
        m_pTableHeaderView->setFrame(CCRect(0, y, width, m_nTableHeaderHeight));
        this->addSubview(m_pTableHeaderView);
        y += m_nTableHeaderHeight;
    }
    
    unsigned int sectionCount = m_pTableViewDataSource->numberOfSections(this);
    
    m_rTableCellRectss.resize(sectionCount);
    for (unsigned int i=0; i<sectionCount; i++)
    {
        CCRect sectionHeaderRect = CCRect(0, y, width, m_nSectionHeaderHeights.at(i));
        CAView* sectionHeaderView = m_pTableViewDataSource->tableViewSectionViewForHeaderInSection(this, sectionHeaderRect.size, i);
        
        //CC_DEPRECATED_ATTRIBUTE
        if (sectionHeaderView == NULL)
        {
            sectionHeaderView = m_pTableViewDataSource->tableViewSectionViewForHeaderInSection(this, i);
        }
        
        if (sectionHeaderView)
        {
            sectionHeaderView->setFrame(sectionHeaderRect);
            this->addSubview(sectionHeaderView);
        }
         y += m_nSectionHeaderHeights[i];
        
        m_rTableCellRectss[i].resize(m_nRowHeightss[i].size());
        for (unsigned int j=0; j<m_rTableCellRectss[i].size(); j++)
        {
            m_rTableCellRectss[i][j] = CCRect(0, y, width, m_nRowHeightss[i][j]);
            
            CATableViewCell* cell = m_pTableViewDataSource->tableCellAtIndex(this, m_rTableCellRectss[i][j].size, i, j);
            
            //CC_DEPRECATED_ATTRIBUTE
            if (cell == NULL)
            {
                cell = m_pTableViewDataSource->tableCellAtIndex(this, i, j);
            }
            
            cell->setFrame(m_rTableCellRectss[i][j]);
            this->addSubview(cell);
            cell->m_nSection = i;
            cell->m_nRow = j;
            m_pTableCells.push_back(cell);
            
             y += m_nRowHeightss[i][j];
            
            if (j < m_nRowHeightss.at(i).size() - 1)
            {
                CAView* view = CAView::createWithFrame(CCRect(0, y, width, m_nSeparatorViewHeight), m_separatorColor);
                this->addSubview(view);
                y += m_nSeparatorViewHeight;
            }
        }
        
        CCRect sectionFooterRect = CCRect(0, y, width, m_nSectionFooterHeights.at(i));
        
        CAView* sectionFooterView = m_pTableViewDataSource->tableViewSectionViewForFooterInSection(this, sectionFooterRect.size, i);
        
        //CC_DEPRECATED_ATTRIBUTE
        if (sectionFooterView == NULL)
        {
            sectionFooterView = m_pTableViewDataSource->tableViewSectionViewForFooterInSection(this, i);
        }
        
        if (sectionFooterView)
        {
            sectionFooterView->setFrame(sectionFooterRect);
            this->addSubview(sectionFooterView);
        }
        y += m_nSectionFooterHeights.at(i);
    }
    
    if (m_pTableFooterView)
    {
        m_pTableFooterView->setDisplayRange(true);
        m_pTableFooterView->setFrame(CCRect(0, y, width, m_nTableFooterHeight));
        this->addSubview(m_pTableFooterView);
        y += m_nTableFooterHeight;
    }
    
    if (m_pTablePullUpView)
    {
        m_pTablePullUpView->setFrame(CCRect(0, m_obViewSize.height, width, m_nTablePullViewHeight));
        this->addSubview(m_pTablePullUpView);
    }
}

void CATableView::update(float dt)
{
    CAScrollView::update(dt);
    
    CCRect rect = this->getBounds();
    rect.origin.y -= rect.size.height * 0.1f;
    rect.size.height *= 1.2f;
    
    std::deque<CATableViewCell*>::iterator itr;
    for (itr=m_pTableCells.begin(); itr!=m_pTableCells.end(); itr++)
    {
        CATableViewCell* cell = *itr;
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

void CATableView::setTablePullDownView(CAView* var)
{
    CC_SAFE_RELEASE(m_pTablePullDownView);
    CC_SAFE_RETAIN(var);
    m_pTablePullDownView = var;
}

void CATableView::setTablePullUpView(CAView* var)
{
    CC_SAFE_RELEASE(m_pTablePullUpView);
    CC_SAFE_RETAIN(var);
    m_pTablePullUpView = var;
}

void CATableView::setTablePullViewHeight(unsigned int var)
{
    m_nTablePullViewHeight = var;
}

#pragma CATableViewCell

CATableViewCell::CATableViewCell()
:m_nSection(0)
,m_nRow(0)
{
    m_bControl = false;
}

CATableViewCell::~CATableViewCell()
{

}

CATableViewCell* CATableViewCell::create(const char* reuseIdentifier)
{
    CATableViewCell* cell = new CATableViewCell();
    if (cell && cell->initWithReuseIdentifier(reuseIdentifier))
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

bool CATableViewCell::initWithReuseIdentifier(const char* reuseIdentifier)
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

void CATableViewCell::setControlState(CAControlState var)
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