//
//  CAPickerView.cpp
//  CrossApp
//
//  Created by dai xinping on 14-7-22.
//  Copyright (c) 2014年 www.9miao.com. All rights reserved.
//

#include "CAPickerView.h"
#include "CALabel.h"

NS_CC_BEGIN

CAPickerView::CAPickerView()
: m_tableViews(NULL)
, m_delegate(NULL)
, m_dataSource(NULL)
, m_fontSizeNormal(20)
, m_fontSizeSelected(22)
, m_fontColorNormal(ccc4(0, 0, 0, 255))
, m_fontColorSelected(ccc4(0, 0, 0, 255))
, m_separateColor(ccc4(127, 127, 127, 127))
, m_displayRow(7)
{
    setDisplayRange(false);
}

CAPickerView::~CAPickerView()
{
    CC_SAFE_RELEASE(m_tableViews);
}

CAPickerView* CAPickerView::create()
{
    CAPickerView* view = new CAPickerView();
    if (view && view->init()) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CAPickerView* CAPickerView::createWithFrame(const CCRect& rect)
{
    CAPickerView* view = new CAPickerView();
    if (view && view->initWithFrame(rect)) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CAPickerView* CAPickerView::createWithCenter(const CCRect& rect)
{
    CAPickerView* view = new CAPickerView();
    if (view && view->initWithCenter(rect)) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

bool CAPickerView::init()
{
    if (!CAView::init()) {
        return false;
    }
    
    return true;
}

bool CAPickerView::initWithCenter(const CrossApp::CCRect &rect)
{
    if (!CAView::initWithCenter(rect)) {
        return false;
    }
    
    return true;
}

bool CAPickerView::initWithFrame(const CrossApp::CCRect &rect)
{
    if (!CAView::initWithFrame(rect)) {
        return false;
    }
    
    return true;
}

void CAPickerView::onEnter()
{
    CAView::onEnter();
}

void CAPickerView::onExit()
{
    CAView::onExit();
}

int CAPickerView::numberOfComponents()
{
    if (m_dataSource) {
        m_dataSource->numberOfComponentsInPickerView(this);
    }
    return -1;
}

int CAPickerView::numberOfRowsInComponent(unsigned int component)
{
    if (m_dataSource) {
        return m_dataSource->numberOfRowsInComponent(this, component);
    }
    return -1;
}

CCSize CAPickerView::rowSizeForComponent(unsigned int component)
{
    if (m_dataSource) {
        float width = m_dataSource->widthForComponent(this, component);
        float height = m_dataSource->rowHeightForComponent(this, component);
        return CCSizeMake(width, height);
    }
    return CCSizeMake(0, 0);
}

CAView* CAPickerView::viewForRow(unsigned int row, unsigned int component)
{
    if (m_dataSource) {
        return m_dataSource->viewForRow(this, row, component);
    }
    
    return NULL;
}

float CAPickerView::calcTotalWidth(unsigned int component)
{
    float total = 0;
    for (int i=0; i<component; i++) {
        total += m_dataSource->widthForComponent(this, i);
    }
    return total;
}

void CAPickerView::reloadAllComponents()
{
    if (m_delegate && m_dataSource) {
        
        // clear old data        
        CC_SAFE_RELEASE_NULL(m_tableViews);
        m_tableViews = CCArray::create();
        m_tableViews->retain();
        
        m_selected.clear();
        m_componentsIndex.clear();
        m_displayRow.clear();
        
        // clear all tableviews
        removeAllSubviews();
        
        // reload data
        int component = m_dataSource->numberOfComponentsInPickerView(this);
        float total_width = calcTotalWidth(component);
        m_componentsIndex.resize(component);
        m_componentOffsetX.resize(component);
        m_displayRow.resize(component);
        float start_x = getFrame().size.width/2 - total_width/2;
        for (int i=0; i<component; i++) {
            m_selected.push_back(0);
            m_componentsIndex[i] = std::vector<int>();
            m_componentOffsetX[i] = start_x;
            m_displayRow[i] = getFrame().size.height/m_dataSource->rowHeightForComponent(this, i);
            if (m_displayRow[i] % 2 == 0) {
                m_displayRow[i] += 1;
            }

            // create tableview
            float tableWidth = m_dataSource->widthForComponent(this, i);
            float tableHeight = m_dataSource->rowHeightForComponent(this, i) * m_displayRow[i];
            float start_y = getFrame().size.height/2 - tableHeight/2;
            CATableView* tableView = CATableView::createWithFrame(CCRectMake(start_x, start_y, tableWidth, tableHeight));
            tableView->setTableViewDataSource(this);
            tableView->setSeparatorViewHeight(0);
            tableView->setSeparatorColor(CAColor_clear);
            tableView->setShowsVerticalScrollIndicator(false);
            m_tableViews->addObject(tableView);
            addSubview(tableView);
            
            // create highlight
            CCSize selectSize = CCSizeMake(tableWidth, m_dataSource->rowHeightForComponent(this, i));
            CAView* select = m_dataSource->viewForSelect(this, i, selectSize);
            if (!select) {
                CCRect sepRect = CCRectMake(start_x, getFrame().size.height/2 - m_dataSource->rowHeightForComponent(this, i)/2, tableWidth, 1);
                addSubview(CAView::createWithFrame(sepRect, m_separateColor));               
                sepRect.origin.y += m_dataSource->rowHeightForComponent(this, i);
                addSubview(CAView::createWithFrame(sepRect, m_separateColor));
            } else {
                select->setCenter(CCRectMake(start_x, getFrame().size.height/2, selectSize.width, selectSize.height));
                addSubview(select);
            }

            reloadComponent(i, false);
            
            start_x += m_dataSource->widthForComponent(this, i);
        }
    }
}


void CAPickerView::reloadComponent(unsigned int component, bool bReloadData)
{
    // reload component
    int row = m_dataSource->numberOfRowsInComponent(this, component);
    int head = m_displayRow[component]/2;
    int foot = m_displayRow[component]/2;
    if (row <= m_displayRow[component]) {
        row += (head + foot);
        m_componentsIndex[component].resize(row);
        for (int i=0; i<row; i++) {
            if (i < head) {
                m_componentsIndex[component][i] = -1;
            } else if (i >= row - foot) {
                m_componentsIndex[component][i] = -1;
            } else {
                m_componentsIndex[component][i] = i - head;
            }
        }
    } else {
        int cycle = 3;
        m_componentsIndex[component].resize(row*3);
        while (cycle--) {
            for (int i=0; i<row; i++) {
                m_componentsIndex[component][i + cycle*row] = i;
            }
        }
    }
        
    // reset selected index
    selectRow(0, component, false);
    
    if (bReloadData) {
        // reload table view
        CATableView* view = (CATableView*)m_tableViews->objectAtIndex(component);
        view->reloadData();        
    }
}

CAView* CAPickerView::viewForRowInComponent(int component, int row, CCSize size)
{
    int index = m_componentsIndex[component][row];
    if (index == -1) {
        return NULL;
    }
    CAView* view = m_dataSource->viewForRow(this, index, component);
    if (!view) { // create view
        CCString* title = m_dataSource->titleForRow(this, index, component);
        if (title && title->length() > 0) {
            CCRect rect = CCRectMake(0, 0, size.width, size.height);
            CALabel* label = CALabel::createWithFrame(rect);
            label->setText(title->getCString());
            label->setColor(m_fontColorNormal);
            label->setFontSize(m_fontSizeNormal);
            label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label->setTextAlignment(CATextAlignmentCenter);
            
            return label;
        }
    }
    
    return view;
}

CATableViewCell* CAPickerView::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    if (m_dataSource && m_tableViews) {
        
        CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CrossApp");
        if (cell == NULL) {
            cell = CATableViewCell::create("CrossApp");
            cell->setBackgroundView(NULL);
        } else {
            cell->removeSubviewByTag(100);            
        }
        
        int component = m_tableViews->indexOfObject(table);
        
        CAView* view = viewForRowInComponent(component, row, cellSize);
        if (view) {
            view->setTag(100);
            cell->addSubview(view);
        }
        
        return cell;
    }
    
    return NULL;
}

unsigned int CAPickerView::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    if (m_dataSource && m_tableViews) {
        int component = m_tableViews->indexOfObject(table);

        return m_componentsIndex[component].size();
    }
    return 0;
}

unsigned int CAPickerView::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    if (m_dataSource && m_tableViews) {
        int component = m_tableViews->indexOfObject(table);
        return m_dataSource->rowHeightForComponent(this, component);
    }
    return 0;
}

void CAPickerView::selectRow(unsigned int row, unsigned int component, bool animated)
{
    if (!m_tableViews || !m_dataSource) {
        return;
    }
    
    CATableView* tableView = (CATableView*)m_tableViews->objectAtIndex(component);
    if (tableView) {
        int maxRow = m_dataSource->numberOfRowsInComponent(this, component);
        float height = m_dataSource->rowHeightForComponent(this, component);
        if (row < maxRow) {
            CCPoint offset = CCPointZero;
            if (maxRow <= m_displayRow[component]) {
                m_selected[component] = row + m_displayRow[component]/2;
                offset.y = row * height;
                tableView->setContentOffset(offset, animated);
            } else {
                m_selected[component] = maxRow + row;
                offset.y = m_selected[component] * height + height/2 - tableView->getFrame().size.height/2;
                tableView->setContentOffset(offset, animated);
            }
        }
    }
}

int CAPickerView::selectedRowInComponent(unsigned int component)
{
    return m_selected[component];
}

void CAPickerView::visit()
{
    if (m_dataSource) {
        CAObject* obj = NULL;
        CCARRAY_FOREACH(m_tableViews, obj) {
            
            // cycle data
            CATableView* tableView = (CATableView*)obj;
            CCPoint offset = tableView->getContentOffset();
            int component = m_tableViews->indexOfObject(obj);
            int row = m_dataSource->numberOfRowsInComponent(this, component);
            int row_height = m_dataSource->rowHeightForComponent(this, component);
            if (row > m_displayRow[component]) {
                if (offset.y < 0) {
                    offset.y += row * 2 * row_height;
                    tableView->setContentOffset(offset, false);
                } else if (offset.y > row_height * m_componentsIndex[component].size() - tableView->getFrame().size.height) {
                    offset.y -= row * 2 * row_height;
                    tableView->setContentOffset(offset, false);
                }
            }
            
            // set opacity
            int offset_y = abs((int)offset.y);
            int remainder = offset_y % row_height;
            int index = offset_y / row_height;
            for (int i=index-1; i<index + m_displayRow[component] + 2; i++) {
                CATableViewCell* cell = tableView->cellForRowAtIndexPath(0, i);
                if (cell) {
                    float y = cell->getFrameOrigin().y - offset_y + cell->getFrame().size.height/2;
                    float mid = tableView->getFrame().size.height/2;
                    float length = fabs(mid - y);
                    float op = (fabs(length - mid))/mid;
                    op = powf(op, 2);
                    if (op < 0.1f) {
                        op = 0.1f;
                    }
                    
                    cell->setAlpha(op);
                    
//                    float scale = length/mid * 0.1;
//                    cell->setScale(1.0 - scale);
                }
            }
            
            // fixed position in the middle 
            if (!tableView->isDecelerating() && !tableView->isTracking()) {
                if (remainder > row_height/2) {
                    index++;
                    offset.y = index * row_height;
                    tableView->setContentOffset(offset, true);
                } else if (remainder > 0) {
                    offset.y = index * row_height;
                    tableView->setContentOffset(offset, true);
                } else {
                    // set selected when stop scrolling.
                    if (m_selected[component] != index + m_displayRow[component]/2) {
                        m_selected[component] = index + m_displayRow[component]/2;
                        if (m_delegate) {
                            m_delegate->didSelectRow(this, m_componentsIndex[component][m_selected[component]], component);
                        }                    
                    }                    
                }
            }
        }
    }
    
    CAView::visit();
}

NS_CC_END