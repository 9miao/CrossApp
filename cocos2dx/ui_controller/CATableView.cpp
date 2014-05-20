//
//  CATableView.cpp
//  cocos2dx
//
//  Created by Li Yuanfeng on 14-4-28.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#include "CATableView.h"
#include "sprite_nodes/CCSprite.h"
#include "sprite_nodes/CCScale9Sprite.h"
#include "CCDirector.h"
#include "CCScheduler.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionInterval.h"
#include "touch_dispatcher/CCTouch.h"
#include "support/CCPointExtension.h"
NS_CC_BEGIN

#pragma CATableView

CATableView::CATableView()
:m_pTableHeaderView(NULL)
,m_pTableFooterView(NULL)
,m_pTablePullDownView(NULL)
,m_pTablePullUpView(NULL)
,m_separatorColor(ccc4(127, 127, 127, 255))
,m_nTableHeaderHeight(0)
,m_nTableFooterHeight(0)
,m_nTablePullViewHeight(0)
,m_pTableViewDataSource(NULL)
,m_pTableViewDelegate(NULL)
,m_pBackGroundView(NULL)
,m_pHighlightedTableCells(NULL)
,m_bAllowsSelection(false)
,m_bAllowsMultipleSelection(false)
{
    m_pCellDict = new CCDictionary();
}

CATableView::~CATableView()
{
    CC_SAFE_DELETE(m_pCellDict);
    CC_SAFE_DELETE(m_pTableHeaderView);
    CC_SAFE_DELETE(m_pTableFooterView);
    CC_SAFE_DELETE(m_pTablePullDownView);
    CC_SAFE_DELETE(m_pTablePullUpView);
//    m_pTableViewDataSource = NULL;
//    m_pTableViewDelegate = NULL;
    m_pTableViewDelegate->removeDelegate(this);
    m_pTableViewDataSource->removeDelegate(this);
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

bool CATableView::initWithFrame(const cocos2d::CCRect &rect)
{
    if (!CAScrollView::initWithFrame(rect))
    {
        return false;
    }
    
    this->setViewSize(rect.size);
    this->setShowsHorizontalScrollIndicator(false);
    this->setBounceHorizontal(false);
    
    return true;
}

void CATableView::setViewSize(CCSize var)
{
    CAScrollView::setViewSize(var);
}

void CATableView::setContentSize(const cocos2d::CCSize &var)
{
    CAScrollView::setContentSize(var);
    if (m_pBackGroundView)
    {
        m_pBackGroundView->setFrame(CCRect(0, 0, var.width, var.height));
    }
}

bool CATableView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pTouches->count() > 0)
        return false;
    
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
                CC_BREAK_IF(cell->isDisabled());
                
                m_pHighlightedTableCells = cell;

                CC_BREAK_IF(cell->isSelected());
                
                CCDelayTime* delayTime = CCDelayTime::create(0.1f);
                CCCallFunc* func = CCCallFunc::create(cell, callfunc_selector(CATableViewCell::setControlStateHighlighted));
                CCSequence* actions = CCSequence::create(delayTime, func, NULL);
                actions->setTag(0xffff);
                this->runAction(actions);
                break;
            }
        }
    }
    
    m_tBeginPoint = pTouch->getLocation();
    
    return true;
}

void CATableView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CAScrollView::ccTouchMoved(pTouch, pEvent);
    
    do
    {
        CC_BREAK_IF(ccpDistance(m_tBeginPoint, pTouch->getLocation()) < 8.0f);
        
        if (m_pHighlightedTableCells)
        {
            this->stopActionByTag(0xffff);
            
            if (m_pHighlightedTableCells->isHighlighted())
            {
                m_pHighlightedTableCells->setControlStateNormal();
            }
            
            m_pHighlightedTableCells = NULL;
        }
    }
    while (0);
}

void CATableView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
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
    
    if (m_pTableViewDelegate)
    {
        if (m_pTablePullDownView && m_pContainer->getFrame().origin.y > m_nTablePullViewHeight)
        {
            m_pTableViewDelegate->tableViewDidShowPullDownView(this);
        }
        
        if (m_pTablePullUpView && m_pContainer->getFrame().origin.y + m_pContainer->getFrame().size.height - this->getBounds().size.height
            < -m_nTablePullViewHeight)
        {
            m_pTableViewDelegate->tableViewDidShowPullUpView(this);
        }
    }
}

void CATableView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CAScrollView::ccTouchCancelled(pTouch, pEvent);
}

float CATableView::maxSpeed(float delay)
{
    return (CCPoint(m_obContentSize).getLength() * 6 * delay);
}

float CATableView::maxSpeedCache(float delay)
{
    return (maxSpeed(delay) * 3.0f);
}

float CATableView::decelerationRatio(float delay)
{
    return 3.0f * delay;
}

CCPoint CATableView::maxBouncesLenght()
{
    if (m_pTablePullDownView || m_pTablePullUpView)
    {
        return CCPoint(0, MIN(m_obContentSize.height/2, m_nTablePullViewHeight * 0.8f));
    }
    
    return CCPoint(0, m_obContentSize.height/2);
}

void CATableView::setBackGroundImage(CAImage* image)
{
    if (m_pBackGroundView)
    {
        m_pChildInThis->removeObject(m_pBackGroundView);
        m_pBackGroundView->removeFromSuperview();
    }
    m_pBackGroundView = CAImageView::createWithImage(image);
    m_pBackGroundView->setFrame(this->getBounds());
    m_pChildInThis->addObject(m_pBackGroundView);
    this->insertSubview(m_pBackGroundView, -1);
}

void CATableView::setBackGroundScale9Image(CAImage* image)
{
    if (m_pBackGroundView)
    {
        m_pChildInThis->removeObject(m_pBackGroundView);
        m_pBackGroundView->removeFromSuperview();
    }
    m_pBackGroundView = CCScale9Sprite::createWithImage(image);
    m_pBackGroundView->setFrame(this->getBounds());
    m_pChildInThis->addObject(m_pBackGroundView);
    this->insertSubview(m_pBackGroundView, -1);
}

CATableViewCell* CATableView::dequeueReusableCellWithIdentifier(const char* reuseIdentifier)
{
    
    return NULL;
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
            sectionHeight += 1;
        }
        sectionHeight -= 1;
        m_nSectionHeights[i] = sectionHeight;
        viewHeight += sectionHeight;
    }
    
    viewHeight += m_nTableHeaderHeight;
    viewHeight += m_nTableFooterHeight;

    CCSize size = this->getContentSize();
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
    
    if (m_pTablePullUpView)
    {
        m_pTablePullUpView->setFrame(CCRect(0, this->getBounds().size.height, width, m_nTablePullViewHeight));
        this->addSubview(m_pTablePullUpView);
    }
    
    if (m_pTableHeaderView)
    {
        m_pTableHeaderView->setFrame(CCRect(0, y, width, m_nTableHeaderHeight));
        this->addSubview(m_pTableHeaderView);
        y += m_nTableHeaderHeight;
    }
    
    
    unsigned int sectionCount = m_pTableViewDataSource->numberOfSections(this);
    
    m_rTableCellRectss.resize(sectionCount);
    for (unsigned int i=0; i<sectionCount; i++)
    {
        CAView* sectionHeaderView = m_pTableViewDataSource->tableViewSectionViewForHeaderInSection(this, i);
        if (sectionHeaderView)
        {
            sectionHeaderView->setFrame(CCRect(0, y, width, m_nSectionHeaderHeights.at(i)));
            this->addSubview(sectionHeaderView);
        }
         y += m_nSectionHeaderHeights[i];
        
        m_rTableCellRectss[i].resize(m_nRowHeightss[i].size());
        for (unsigned int j=0; j<m_rTableCellRectss[i].size(); j++)
        {
            m_rTableCellRectss[i][j] = CCRect(0, y, width, m_nRowHeightss[i][j]);
            
            CATableViewCell* cell = m_pTableViewDataSource->tableCellAtIndex(this, i, j);
            cell->setFrame(m_rTableCellRectss[i][j]);
            this->addSubview(cell);
            cell->setSection(i);
            cell->setRow(j);
            m_pTableCells.push_back(cell);
            
             y += m_nRowHeightss[i][j];
            
            if (j < m_nRowHeightss.at(i).size() - 1)
            {
                CAView* view = CAView::createWithFrame(CCRect(0, y, width, 1), m_separatorColor);
                this->addSubview(view);
                y += 1;
            }
        }
        
        CAView* sectionFooterView = m_pTableViewDataSource->tableViewSectionViewForFooterInSection(this, i);
        if (sectionFooterView)
        {
            sectionFooterView->setFrame(CCRect(0, y, width, m_nSectionFooterHeights.at(i)));
            this->addSubview(sectionFooterView);
        }
        y += m_nSectionFooterHeights.at(i);
    }
    
    if (m_pTableFooterView)
    {
        m_pTableFooterView->setFrame(CCRect(0, y, width, m_nTableFooterHeight));
        this->addSubview(m_pTableFooterView);
        y += m_nTableFooterHeight;
    }
    
}

void CATableView::update(float fDelta)
{
    CAScrollView::update(fDelta);
    
    CCRect rect = this->getBounds();
    rect.origin.y -= rect.size.height/2;
    rect.size.height *= 2;
    
    std::deque<CATableViewCell*>::iterator itr;
    for (itr=m_pTableCells.begin(); itr!=m_pTableCells.end(); itr++)
    {
        CATableViewCell* cell = *itr;
        CCPoint point = cell->getCenterOrigin();
        point = m_pContainer->convertToWorldSpace(point);
        point = this->convertToNodeSpace(point);
        
        if (rect.containsPoint(point))
        {
            cell->setVisible(true);
        }
        else
        {
            cell->setVisible(false);
        }
    }
}

#pragma CATableViewCell

CATableViewCell::CATableViewCell()
:m_nSection(0)
,m_nRow(0)
{

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

    this->setReuseIdentifier(reuseIdentifier);
    
    this->setBackGroundView(CAView::createWithFrame(this->getBounds(), ccc4(255, 255, 255, 255)));

    this->setHighlightedBackGroundView(CAView::createWithFrame(this->getBounds(), ccc4(50, 50, 200, 255)));
    
    this->setSelectedBackGroundView(CAView::createWithFrame(this->getBounds(), ccc4(50, 50, 200, 255)));
    
    this->setControlStateNormal();
    
    return true;
}

void CATableViewDataSource::removeDelegate(cocos2d::CATableView *table)
{
    table->setTableViewDataSource(NULL);
}

void CATableViewDelegate::removeDelegate(cocos2d::CATableView *table)
{
    table->setTableViewDelegate(NULL);
}

NS_CC_END