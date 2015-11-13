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
#include "dispatcher/CATouch.h"
#include "support/CAPointExtension.h"
#include "CCEGLView.h"
#include "animation/CAViewAnimation.h"
#include "platform/CADensityDpi.h"
NS_CC_BEGIN

#pragma CATableView

CATableView::CATableView()
:m_pTableHeaderView(NULL)
,m_pTableFooterView(NULL)
,m_obSeparatorColor(CAColor_gray)
,m_nSeparatorViewHeight(s_px_to_dip(2.0f))
,m_nTableHeaderHeight(0)
,m_nTableFooterHeight(0)
,m_nSections(0)
,m_pTableViewDataSource(NULL)
,m_pTableViewDelegate(NULL)
,m_pHighlightedTableCells(NULL)
,m_bAllowsSelection(false)
,m_bAllowsMultipleSelection(false)
,m_bAlwaysTopSectionHeader(true)
,m_bAlwaysBottomSectionFooter(true)
{

}

CATableView::~CATableView()
{
    std::map<std::string, CAVector<CATableViewCell*> >::iterator itr;
    for (itr=m_mpFreedTableCells.begin(); itr!=m_mpFreedTableCells.end(); itr++)
    {
        itr->second.clear();
    }
    m_vpUsedTableCells.clear();
    m_mpFreedTableCells.clear();
    m_pFreedLines.clear();
    this->clearData();
    
    CC_SAFE_RELEASE_NULL(m_pTableHeaderView);
    CC_SAFE_RELEASE_NULL(m_pTableFooterView);
    m_pTableViewDataSource = NULL;
    m_pTableViewDelegate = NULL;
}

void CATableView::onEnterTransitionDidFinish()
{
    CAScrollView::onEnterTransitionDidFinish();
    
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0);
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATableView::firstReloadData));
    CAViewAnimation::commitAnimations();
}

void CATableView::onExitTransitionDidStart()
{
    CAScrollView::onExitTransitionDidStart();
}

CATableView* CATableView::createWithFrame(const DRect& rect)
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

CATableView* CATableView::createWithCenter(const DRect& rect)
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
    this->setHorizontalScrollEnabled(false);
    return true;
}

void CATableView::setContentSize(const CrossApp::DSize &var)
{
    CAScrollView::setContentSize(var);
}

bool CATableView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    bool isInertia = m_tInertia.getLength() < 1.0f;
    if (!CAScrollView::ccTouchBegan(pTouch, pEvent))
        return false;
    
    if (m_bAllowsSelection && this->isScrollWindowNotOutSide() == false && isInertia)
    {
        DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);
        
        std::map<CAIndexPath2E, CATableViewCell*>::iterator itr;
        for (itr=m_mpUsedTableCells.begin(); itr!=m_mpUsedTableCells.end(); itr++)
        {
            CATableViewCell* cell = itr->second;
            CC_CONTINUE_IF(cell == NULL);
            if (cell->getFrame().containsPoint(point) && cell->isVisible())
            {
                CC_BREAK_IF(cell->getControlState() == CAControlStateDisabled);
                
                if (m_pHighlightedTableCells != cell)
                {
                    if (m_pHighlightedTableCells)
                    {
                        m_pHighlightedTableCells->setControlStateNormal();
                    }
                    m_pHighlightedTableCells = cell;
                }

                CC_BREAK_IF(cell->getControlState() == CAControlStateSelected);
                
                CAViewAnimation::beginAnimations(m_s__StrID, NULL);
                CAViewAnimation::setAnimationDuration(0.05f);
                CAViewAnimation::setAnimationDidStopSelector(cell, CAViewAnimation0_selector(CATableViewCell::setControlStateHighlighted));
                CAViewAnimation::commitAnimations();
                break;
            }
        }
    }
    return true;
}

void CATableView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
    CAScrollView::ccTouchMoved(pTouch, pEvent);
    
    if (m_pHighlightedTableCells)
    {
        CAViewAnimation::removeAnimations(m_s__StrID);
        
        if (m_pHighlightedTableCells->getControlState() == CAControlStateHighlighted)
        {
            m_pHighlightedTableCells->setControlStateNormal();
        }
        
        m_pHighlightedTableCells = NULL;
    }
}

void CATableView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
    CAScrollView::ccTouchEnded(pTouch, pEvent);
    
    if (m_pHighlightedTableCells)
    {
        CAViewAnimation::removeAnimations(m_s__StrID);
        
        CAIndexPath2E deselectedIndexPath = CAIndexPath2EZero;
        CAIndexPath2E selectedIndexPath = CAIndexPath2E(m_pHighlightedTableCells->getSection(), m_pHighlightedTableCells->getRow());
        m_pHighlightedTableCells = NULL;
        
        if (m_pSelectedTableCells.count(selectedIndexPath) > 0 && m_bAllowsMultipleSelection)
        {
            deselectedIndexPath = selectedIndexPath;
            selectedIndexPath = CAIndexPath2EZero;
            m_pSelectedTableCells.erase(deselectedIndexPath);
        }
        else
        {
            if (!m_pSelectedTableCells.empty() && m_bAllowsMultipleSelection == false)
            {
                deselectedIndexPath = *m_pSelectedTableCells.begin();
                m_pSelectedTableCells.clear();
                
            }
            m_pSelectedTableCells.insert(selectedIndexPath);
            
        }

        if (deselectedIndexPath != CAIndexPath2EZero)
        {
            if (CATableViewCell* cell = m_mpUsedTableCells[deselectedIndexPath])
            {
                cell->setControlStateNormal();
            }
            if (m_pTableViewDelegate)
            {
                m_pTableViewDelegate->tableViewDidDeselectRowAtIndexPath(this,
                                                                         deselectedIndexPath.section,
                                                                         deselectedIndexPath.row);
            }
        }
        
        if (selectedIndexPath != CAIndexPath2EZero)
        {
            if (CATableViewCell* cell = m_mpUsedTableCells[selectedIndexPath])
            {
                cell->setControlStateSelected();
            }
            if (m_pTableViewDelegate)
            {
                m_pTableViewDelegate->tableViewDidSelectRowAtIndexPath(this,
                                                                       selectedIndexPath.section,

																	   selectedIndexPath.row);
            }
        }
    }
}

void CATableView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
    CAScrollView::ccTouchCancelled(pTouch, pEvent);
    
    if (m_pHighlightedTableCells)
    {
        CAViewAnimation::removeAnimations(m_s__StrID);
        
        if (m_pHighlightedTableCells->getControlState() == CAControlStateHighlighted)
        {
            m_pHighlightedTableCells->setControlStateNormal();
        }
        m_pHighlightedTableCells = NULL;
    }
}

void CATableView::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
{
    if (m_bAllowsSelection)
    {
        DPoint point = m_pContainer->convertTouchToNodeSpace(pTouch);
        
        std::map<CAIndexPath2E, CATableViewCell*>::iterator itr;
        for (itr=m_mpUsedTableCells.begin(); itr!=m_mpUsedTableCells.end(); itr++)
        {
            CATableViewCell* cell = itr->second;
            CC_CONTINUE_IF(cell == NULL);
            if (cell->getFrame().containsPoint(point) && cell->isVisible())
            {
                CC_BREAK_IF(cell->getControlState() == CAControlStateDisabled);
                
                if (m_pHighlightedTableCells)
                {
                    CAIndexPath2E index = CAIndexPath2E(m_pHighlightedTableCells->getSection(), m_pHighlightedTableCells->getRow());
                    if (m_pSelectedTableCells.count(index))
                    {
                        m_pHighlightedTableCells->setControlStateHighlighted();
                    }
                    else
                    {
                        m_pHighlightedTableCells->setControlStateNormal();
                    }
                    
                }
                
                m_pHighlightedTableCells = cell;
                cell->setControlStateHighlighted();

                break;
            }
        }
    }
}

void CATableView::mouseMovedOutSide(CATouch* pTouch, CAEvent* pEvent)
{
    if (m_pHighlightedTableCells)
    {
        CAIndexPath2E index = CAIndexPath2E(m_pHighlightedTableCells->getSection(), m_pHighlightedTableCells->getRow());
        if (m_pSelectedTableCells.count(index))
        {
            m_pHighlightedTableCells->setControlStateSelected();
        }
        else
        {
            m_pHighlightedTableCells->setControlStateNormal();
        }
        m_pHighlightedTableCells = NULL;
    }
}

void CATableView::switchPCMode(bool var)
{
    CAScrollView::switchPCMode(var);
    this->setMouseMovedEnabled(true);
}

float CATableView::maxSpeed(float dt)
{
    return (128 * 60 * dt);
}

float CATableView::maxSpeedCache(float dt)
{
    return (maxSpeed(dt) * 2.0f);
}

float CATableView::decelerationRatio(float dt)
{
    return 1.8f * dt;
}

CATableViewCell* CATableView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
{
    CATableViewCell* cell = NULL;
    
    if (!m_mpFreedTableCells[reuseIdentifier].empty())
    {
        cell = m_mpFreedTableCells[reuseIdentifier].back();
        cell->retain()->autorelease();
        m_mpFreedTableCells[reuseIdentifier].eraseObject(cell);
    }
    return cell;
}

void CATableView::setAllowsSelection(bool var)
{
    m_bAllowsSelection = var;
    CC_RETURN_IF(!m_bAllowsSelection);
    CC_RETURN_IF(m_pSelectedTableCells.empty());
    std::set<CAIndexPath2E>::iterator itr;
    for (itr=m_pSelectedTableCells.begin(); itr!=m_pSelectedTableCells.end(); itr++)
    {
        if (CATableViewCell* cell = m_mpUsedTableCells[(*itr)])
        {
            cell->setControlState(CAControlStateNormal);
        }
    }
    m_pSelectedTableCells.clear();
}

void CATableView::setAllowsMultipleSelection(bool var)
{
    m_bAllowsMultipleSelection = var;
    CC_RETURN_IF(m_pSelectedTableCells.empty());
    std::set<CAIndexPath2E>::iterator itr;
    for (itr=m_pSelectedTableCells.begin(); itr!=m_pSelectedTableCells.end(); itr++)
    {
        if (CATableViewCell* cell = m_mpUsedTableCells[(*itr)])
        {
            cell->setControlState(CAControlStateNormal);
        }
    }
    m_pSelectedTableCells.clear();
}

void CATableView::setSelectRowAtIndexPath(unsigned int section, unsigned int row)
{
    CC_RETURN_IF(section >= m_nSections);
    CC_RETURN_IF(row >= m_nRowsInSections[section]);
    
    if (!m_pSelectedTableCells.empty() && m_bAllowsMultipleSelection == false)
    {
        std::set<CAIndexPath2E>::iterator itr;
        for (itr=m_pSelectedTableCells.begin(); itr!=m_pSelectedTableCells.end(); itr++)
        {
            if (CATableViewCell* cell = m_mpUsedTableCells[(*itr)])
            {
                cell->setControlState(CAControlStateNormal);
            }
        }
        m_pSelectedTableCells.clear();
    }
    
    CAIndexPath2E indexPath = CAIndexPath2E(section, row);
    if (CATableViewCell* cell = m_mpUsedTableCells[indexPath])
    {
        cell->setControlStateSelected();
    }
    
    m_pSelectedTableCells.insert(indexPath);
}

void CATableView::setUnSelectRowAtIndexPath(unsigned int section, unsigned int row)
{
    CC_RETURN_IF(section >= m_rSectionRects.size());
    
    CAIndexPath2E indexPath = CAIndexPath2E(section, row);
    CC_RETURN_IF(m_pSelectedTableCells.find(indexPath) == m_pSelectedTableCells.end());
    if (CATableViewCell* cell = m_mpUsedTableCells.at(indexPath))
    {
        cell->setControlStateNormal();
    }
    m_pSelectedTableCells.erase(indexPath);
}

void CATableView::setShowsScrollIndicators(bool var)
{
    this->setShowsVerticalScrollIndicator(var);
    m_bShowsScrollIndicators = var;
}

CATableViewCell* CATableView::cellForRowAtIndexPath(unsigned int section, unsigned int row)
{
    return m_mpUsedTableCells[CAIndexPath2E(section, row)];
}

const CAVector<CATableViewCell*>& CATableView::displayingTableCell()
{
    return m_vpUsedTableCells;
}

void CATableView::clearData()
{
    m_nSections = 0;
    m_nRowsInSections.clear();
    m_nSectionHeaderHeights.clear();
    m_nSectionFooterHeights.clear();
    std::vector<std::vector<unsigned int> >::iterator itr;
    for (itr=m_nRowHeightss.begin(); itr!=m_nRowHeightss.end(); itr++)
    {
        itr->clear();
    }
    m_nRowHeightss.clear();
    
    std::vector<std::vector<DRect> >::iterator itr2;
    for (itr2=m_rTableCellRectss.begin(); itr2!=m_rTableCellRectss.end(); itr2++)
    {
        itr2->clear();
    }
    m_rTableCellRectss.clear();
    
    m_rSectionRects.clear();
    
    std::vector<std::vector<DRect> >::iterator itr3;
    for (itr3=m_rLineRectss.begin(); itr3!=m_rLineRectss.end(); itr3++)
    {
        itr3->clear();
    }
    m_rLineRectss.clear();
    
    m_pSelectedTableCells.clear();
    m_pUsedLines.clear();
    
    std::map<CAIndexPath2E, CATableViewCell*>::iterator itr4;
    for (itr4=m_mpUsedTableCells.begin(); itr4!=m_mpUsedTableCells.end(); itr4++)
    {
        CATableViewCell* cell = itr4->second;
        CC_CONTINUE_IF(cell == NULL);
        m_mpFreedTableCells[cell->getReuseIdentifier()].pushBack(cell);
        itr4->second = NULL;
        cell->removeFromSuperview();
        cell->resetTableViewCell();
    }
    m_mpUsedTableCells.clear();
    m_vpUsedTableCells.clear();
    m_pSectionHeaderViews.clear();
    m_pSectionHeaderViews.clear();
}

void CATableView::reloadViewSizeData()
{
    this->clearData();
    
    m_nSections = m_pTableViewDataSource->numberOfSections(this);
    m_nRowsInSections.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        unsigned int rowsInSection = m_pTableViewDataSource->numberOfRowsInSection(this, i);
        m_nRowsInSections[i] = rowsInSection;
    }
    
    m_nSectionHeaderHeights.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        unsigned int sectionHeaderHeight = m_pTableViewDataSource->tableViewHeightForHeaderInSection(this, i);
        m_nSectionHeaderHeights[i] = sectionHeaderHeight;
    }
    
    m_nSectionFooterHeights.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        unsigned int sectionFooterHeight = m_pTableViewDataSource->tableViewHeightForFooterInSection(this, i);
        m_nSectionFooterHeights[i] = sectionFooterHeight;
    }
    
    m_nRowHeightss.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
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
    
    m_nSectionHeights.resize(m_nSections);
    for (unsigned int i=0; i<m_nSections; i++)
    {
        unsigned int sectionHeight = 0;
        sectionHeight += m_nSectionHeaderHeights.at(i);
        sectionHeight += m_nSectionFooterHeights.at(i);
        for (unsigned int j=0; j<m_nRowHeightss.at(i).size(); j++)
        {
            sectionHeight += m_nRowHeightss.at(i).at(j);
            sectionHeight += m_nSeparatorViewHeight;
        }
        m_nSectionHeights[i] = sectionHeight;
        viewHeight += sectionHeight;
    }
    
    viewHeight += m_nTableHeaderHeight;
    viewHeight += m_nTableFooterHeight;

    DSize size = this->getBounds().size;
    size.height = viewHeight;
    this->setViewSize(size);
}

void CATableView::reloadData()
{
    this->reloadViewSizeData();

    this->removeAllSubviews();
    
    float width = this->getBounds().size.width;
    unsigned int y = 0;
    
    if (m_pTableHeaderView)
    {
        m_pTableHeaderView->setFrame(DRect(0, y, width, m_nTableHeaderHeight));
        this->addSubview(m_pTableHeaderView);
        y += m_nTableHeaderHeight;
    }
    
    unsigned int sectionCount = m_pTableViewDataSource->numberOfSections(this);
    
    m_rTableCellRectss.resize(sectionCount);
    m_rLineRectss.resize(sectionCount);
    for (unsigned int i=0; i<sectionCount; i++)
    {
        DRect sectionHeaderRect = DRect(0, y, width, m_nSectionHeaderHeights.at(i));
        CAView* sectionHeaderView = m_pTableViewDataSource->tableViewSectionViewForHeaderInSection(this, sectionHeaderRect.size, i);
        
        if (sectionHeaderView)
        {
            sectionHeaderView->setFrame(sectionHeaderRect);
            this->insertSubview(sectionHeaderView, 2);
            m_pSectionHeaderViews[i] = sectionHeaderView;
        }
         y += m_nSectionHeaderHeights[i];
        
        m_rTableCellRectss[i].resize(m_nRowHeightss[i].size());
        m_rLineRectss[i].resize(m_nRowHeightss[i].size());
        for (unsigned int j=0; j<m_rTableCellRectss[i].size(); j++)
        {
            m_rTableCellRectss[i][j] = DRect(0, y, width, m_nRowHeightss[i][j]);
            y += m_nRowHeightss[i][j];
            
            m_rLineRectss[i][j] = DRect(0, y, width, m_nSeparatorViewHeight);
            y += m_nSeparatorViewHeight;
        }
        
        DRect sectionFooterRect = DRect(0, y, width, m_nSectionFooterHeights.at(i));
        
        CAView* sectionFooterView = m_pTableViewDataSource->tableViewSectionViewForFooterInSection(this, sectionFooterRect.size, i);
        
        if (sectionFooterView)
        {
            sectionFooterView->setFrame(sectionFooterRect);
            this->insertSubview(sectionFooterView, 2);
            m_pSectionFooterViews[i] = sectionFooterView;
        }
        
        DRect sectionRect = sectionHeaderRect;
        sectionRect.size.height = sectionFooterRect.origin.y
                                + sectionFooterRect.size.height
                                - sectionHeaderRect.origin.y;
        m_rSectionRects.push_back(sectionRect);
        
        y += m_nSectionFooterHeights.at(i);
    }
    
    if (m_pTableFooterView)
    {
        m_pTableFooterView->setFrame(DRect(0, y, width, m_nTableFooterHeight));
        this->addSubview(m_pTableFooterView);
        y += m_nTableFooterHeight;
    }

    this->loadTableCell();
    this->updateSectionHeaderAndFooterRects();
    this->layoutPullToRefreshView();
    this->startDeaccelerateScroll();
}

void CATableView::firstReloadData()
{
    CC_RETURN_IF(!m_mpUsedTableCells.empty());
    this->reloadData();
}

void CATableView::loadTableCell()
{
    DRect rect = this->getBounds();
	rect.origin = getContentOffset();
    rect.origin.y -= rect.size.height * 0.1f;
    rect.size.height *= 1.2f;
    
    for (unsigned int i=0; i<(unsigned int)m_rTableCellRectss.size(); i++)
    {
        for (unsigned int j=0; j<(unsigned int)m_rTableCellRectss.at(i).size(); j++)
        {
            CAIndexPath2E indexPath = CAIndexPath2E(i, j);
            CC_CONTINUE_IF(m_mpUsedTableCells.count(indexPath) && m_mpUsedTableCells[indexPath]);
            DRect cellRect = m_rTableCellRectss[i][j];
            CC_CONTINUE_IF(!rect.intersectsRect(cellRect));
            CATableViewCell* cell = m_pTableViewDataSource->tableCellAtIndex(this, m_rTableCellRectss[i][j].size, i, j);
            CC_CONTINUE_IF(cell == NULL);
            cell->m_nSection = i;
            cell->m_nRow = j;
            cell->updateDisplayedAlpha(this->getAlpha());
            m_pContainer->addSubview(cell);
            cell->setFrame(m_rTableCellRectss[i][j]);
            m_mpUsedTableCells[indexPath] = cell;
            m_vpUsedTableCells.pushBack(cell);
            if (m_pSelectedTableCells.count(indexPath))
            {
                cell->setControlStateSelected();
            }
            if (m_pTableViewDataSource)
            {
                m_pTableViewDataSource->tableViewWillDisplayCellAtIndex(this, cell, i, j);
            }
            
            CAView* view = this->dequeueReusableLine();
            DRect lineRect = m_rLineRectss[i][j];
            if (view == NULL)
            {
                view = CAView::createWithFrame(lineRect, m_obSeparatorColor);
            }
            m_pUsedLines[indexPath] = view;
            this->insertSubview(view, 1);
            view->setFrame(lineRect);
        }
    }
}

void CATableView::recoveryTableCell()
{
    DRect rect = this->getBounds();
	rect.origin = getContentOffset();
    rect.origin.y -= rect.size.height * 0.1f;
    rect.size.height *= 1.2f;
    
    std::map<CAIndexPath2E, CATableViewCell*>::iterator itr;
    for (itr=m_mpUsedTableCells.begin();
         itr!=m_mpUsedTableCells.end();
         itr++)
    {
        CATableViewCell* cell = itr->second;
        CC_CONTINUE_IF(cell == NULL);
        DRect cellRect = cell->getFrame();

        CC_CONTINUE_IF(rect.intersectsRect(cellRect));
        m_mpFreedTableCells[cell->getReuseIdentifier()].pushBack(cell);
        cell->removeFromSuperview();
        cell->resetTableViewCell();
        itr->second = NULL;
        m_vpUsedTableCells.eraseObject(cell);
        
        CAView* line = m_pUsedLines[itr->first];
        CC_CONTINUE_IF(line == NULL);
        m_pFreedLines.pushBack(line);
        line->removeFromSuperview();
        m_pUsedLines[itr->first] = NULL;
    }
}

CAView* CATableView::dequeueReusableLine()
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

void CATableView::updateSectionHeaderAndFooterRects()
{
    DRect rect = this->getBounds();
	rect.origin = getContentOffset();
    
    std::vector<DRect>::iterator itr;
    for (itr=m_rSectionRects.begin(); itr!=m_rSectionRects.end(); itr++)
    {
        CC_CONTINUE_IF(!rect.intersectsRect(*itr));
        int i = (int)(itr - m_rSectionRects.begin());
        CAView* header = NULL;
        CAView* footer = NULL;
        float headerHeight = m_nSectionHeaderHeights[i];
        float footerHeight = m_nSectionFooterHeights[i];
        if (m_pSectionHeaderViews.find(i) != m_pSectionHeaderViews.end())
        {
            header = m_pSectionHeaderViews[i];
        }
        if (m_pSectionFooterViews.find(i) != m_pSectionFooterViews.end())
        {
            footer = m_pSectionFooterViews[i];
        }
        if (header && m_bAlwaysTopSectionHeader)
        {
            DPoint p1 = rect.origin;
            p1.y = MAX(p1.y, itr->origin.y);
            p1.y = MIN(p1.y, itr->origin.y + itr->size.height
                       - headerHeight - footerHeight);
            header->setFrameOrigin(p1);
        }
        if (footer && m_bAlwaysBottomSectionFooter)
        {
            DPoint p2 = DPointZero;
            p2.y = MIN(rect.origin.y + this->getBounds().size.height - footerHeight,
                       itr->origin.y + itr->size.height - footerHeight);
            p2.y = MAX(p2.y, itr->origin.y + headerHeight);
            footer->setFrameOrigin(p2);
        }
    }
}

void CATableView::update(float dt)
{
    CAScrollView::update(dt);
    
    this->recoveryTableCell();
    this->loadTableCell();
    this->updateSectionHeaderAndFooterRects();
}

unsigned int CATableView::getNumberOfSections()
{
    return m_nSections;
}

unsigned int CATableView::getNumberOfRowsInSection(unsigned int section)
{
    if (section >= m_nRowsInSections.size())
    {
        return 0;
    }
    return m_nRowsInSections.at(section);
}

float CATableView::getSectionHeightInSection(unsigned int section)
{
    if (section >= m_nSectionHeights.size())
    {
        return 0.0f;
    }
    return (float)m_nSectionHeights.at(section);
}

float CATableView::getSectionHeaderHeightInSection(unsigned int section)
{
    if (section >= m_nSectionHeaderHeights.size())
    {
        return 0.0f;
    }
    return (float)m_nSectionHeaderHeights.at(section);
}

float CATableView::getSectionFooterHeightInSection(unsigned int section)
{
    if (section >= m_nSectionFooterHeights.size())
    {
        return 0.0f;
    }
    return (float)m_nSectionFooterHeights.at(section);
}

float CATableView::getRowHeightInSectionInRow(unsigned int section, unsigned int row)
{
    if (section >= m_nRowHeightss.size())
    {
        return 0.0f;
    }
    if (row >= m_nRowHeightss[section].size())
    {
        return 0.0f;
    }
    return (float)m_nRowHeightss[section][row];
}

#pragma CATableViewCell

CATableViewCell::CATableViewCell()
:m_pBackgroundView(NULL)
,m_pContentView(NULL)
,m_nSection(0xffffffff)
,m_nRow(0xffffffff)
,m_bControlStateEffect(true)
,m_bAllowsSelected(true)
{
    this->setHaveNextResponder(true);
    this->setDisplayRange(false);
    this->setColor(CAColor_clear);
}

CATableViewCell::~CATableViewCell()
{
    CC_SAFE_RELEASE_NULL(m_pContentView);
    CC_SAFE_RELEASE_NULL(m_pBackgroundView);
}

CATableViewCell* CATableViewCell::create(const std::string& reuseIdentifier)
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

bool CATableViewCell::initWithReuseIdentifier(const std::string& reuseIdentifier)
{
    m_pContentView = new CAView();
    this->addSubview(m_pContentView);
    
    this->setBackgroundView(CAView::create());
    this->setReuseIdentifier(reuseIdentifier);
    this->normalTableViewCell();
    
    return true;
}

void CATableViewCell::setBackgroundView(CrossApp::CAView *var)
{
    CC_SAFE_RETAIN(var);
    this->removeSubview(m_pBackgroundView);
    CC_SAFE_RELEASE(m_pBackgroundView);
    m_pBackgroundView = var;
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setFrame(this->getBounds());
    m_pContentView->insertSubview(m_pBackgroundView, -1);
}

CAView* CATableViewCell::getBackgroundView()
{
    return m_pBackgroundView;
}

void CATableViewCell::setContentSize(const CrossApp::DSize &var)
{
    CAView::setContentSize(var);
    
    m_pContentView->setFrame(this->getBounds());
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(m_pContentView->getBounds());
    }
}

void CATableViewCell::setControlState(const CAControlState& var)
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

void CATableViewCell::normalTableViewCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(255, 255, 255, 255));
}

void CATableViewCell::highlightedTableViewCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(50, 193, 255, 255));
}


void CATableViewCell::selectedTableViewCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(50, 193, 255, 255));
}


void CATableViewCell::disabledTableViewCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(127, 127, 127, 255));
}

void CATableViewCell::resetTableViewCell()
{
    this->setVisible(true);
    this->normalTableViewCell();
    this->recoveryTableViewCell();
    m_pContentView->setScale(1.0f);
    m_pContentView->setFrame(this->getBounds());
    m_pContentView->setRotation(0);
}

NS_CC_END