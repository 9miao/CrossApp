//
//  CAListMenuView.cpp
//  CrossApp
//
//  Created by QiaoXin on 15-8-27.
//  Email:qiaoxin265@126.com
//  Copyright (c) 2015 http://9miao.com All rights reserved.
//

#include "CAListMenuView.h"

#define MENU_LABEL_TAG 100
#define MENU_BG_COLOR_NORMAL CAColor_black
#define MENU_BG_COLOR_SELECTED CAColor_white
#define MENU_LABEL_COLOR_NORMAL CAColor_white
#define MENU_LABEL_COLOR_SELECTED CAColor_black

#define LABEL_FONT_SIZE 14
#define NEXT_MENU_ITEM_IMAGE "source_material/btn_right_blue.png"


CAListMenuView::CAListMenuView()
:m_pDelegate(NULL)
,m_mMenuInfo(NULL)
{

}

CAListMenuView::~CAListMenuView()
{
    m_pMenuItemSelectedNow = NULL;
    m_vMenuItemTableView.clear();
    m_vMenuItemInfoWithTableView.clear();
}

void CAListMenuView::cleanMenuInfo(MAP_MENUINFO menuInfo)
{
    MAP_MENUINFO::iterator itr = menuInfo.begin();
    for(; itr != menuInfo.end(); itr++)
    {
        if( (itr->second)->m_mChildMenuInfo.size() > 0 )
        {
            cleanMenuInfo((itr->second)->m_mChildMenuInfo);
        }
        CC_SAFE_DELETE(itr->second);
    }
}

CAListMenuView* CAListMenuView::create()
{
    CAListMenuView* obj = new CAListMenuView();
    if(obj && obj->init())
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return obj;
}

bool CAListMenuView::init()
{
    if(CAControl::init())
    {
        CAApplication* pDirector = CAApplication::getApplication();
        this->setFrame(CCRect(0,0,pDirector->getWinSize().width,pDirector->getWinSize().height));
        if(!m_mMenuInfo)
        {
            m_mMenuInfo = new MenuInfo();
        }
        this->setColor(CAColor_clear);
        return true;
    }
    return false;
}

void CAListMenuView::setMenuItemWithIndex(std::string title, CCRect rect, int tag, int menuItemIndex, ...)
{
   	va_list args;
    va_start(args, menuItemIndex);
    MenuInfo* menuItemInfo = m_mMenuInfo->getChildMenuInfoWithIndex(menuItemIndex);
    int index = va_arg(args, int);
    while (index)
    {
        menuItemInfo = menuItemInfo->getChildMenuInfoWithIndex(index);
        index = va_arg(args, int);
    }
    menuItemInfo->m_sTitle = title;
    menuItemInfo->m_pRect = rect;
    menuItemInfo->m_iTag = tag;
    va_end(args);
}

void CAListMenuView::setMenuItemWithIndexForCAMenuView(std::string title, CCRect rect, int tag, int menuItemIndex, va_list argp)
{
    MenuInfo* menuItemInfo = m_mMenuInfo->getChildMenuInfoWithIndex(menuItemIndex);
    int index = va_arg(argp, int);
    while (index)
    {
        menuItemInfo = menuItemInfo->getChildMenuInfoWithIndex(index);
        index = va_arg(argp, int);
    }
    menuItemInfo->m_sTitle = title;
    menuItemInfo->m_pRect = rect;
    menuItemInfo->m_iTag = tag;
    va_end(argp);
}

void CAListMenuView::setDelegate(CAListMenuViewDelegate* delegate)
{
    m_pDelegate = delegate;
}

void CAListMenuView::showListMenuView(CCPoint point)
{
    this->setVisible(true);
    m_pMenuItemSelectedNow = m_mMenuInfo;
    MAP_MENUINFO::iterator itr_info = m_pMenuItemSelectedNow->m_mChildMenuInfo.begin();
    float tableViewWidth = 0;
    float tableViewHeight = 0;
    for(; itr_info != m_pMenuItemSelectedNow->m_mChildMenuInfo.end(); ++itr_info)
    {
        MenuInfo* menuinfo = itr_info->second;
        tableViewWidth = menuinfo->m_pRect.size.width;
        tableViewHeight += menuinfo->m_pRect.size.height;
    }
    m_vMenuItemInfoWithTableView.push_back(m_mMenuInfo);
    createMenuItemView(CCRect(point.x , point.y, tableViewWidth, tableViewHeight));
}

#pragma mark -- Protected
void CAListMenuView::createMenuItemView(CCRect rect)
{
    CATableView* tableView = CATableView::createWithFrame(rect);
    tableView->setTableViewDataSource(this);
    tableView->setTableViewDelegate(this);
    tableView->setAllowsSelection(true);
    tableView->setScrollEnabled(false);
    tableView->setBackGroundColor(CAColor_clear);
    this->addSubview(tableView);
    m_vMenuItemTableView.push_back(tableView);
}

void CAListMenuView::cleanMenuItemView()
{
    for(int i=m_vMenuItemTableView.size()-1; i>=0; i--)
    {
        m_vMenuItemTableView.at(i)->removeFromSuperview();
        m_vMenuItemTableView.pop_back();
    }
}


#pragma mark -- Touch

bool CAListMenuView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    cleanMenuItemView();
    if(CrossApp::EventType::rightMouseEvent == pEvent->getEventType())
    {
        CCPoint point = pTouch->getLocation();
        this->showListMenuView(point);
    }
    else if(CrossApp::EventType::leftMouseEvent == pEvent->getEventType())
    {
        this->setVisible(false);
        if(m_pDelegate)
        {
            m_pDelegate->selectedOver();
        }
    }
    return true;
}

void CAListMenuView::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{

}

void CAListMenuView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{

}

void CAListMenuView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{

}

#pragma mark -- TableView

#pragma mark TableViewDelegate
void CAListMenuView::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    if(m_pDelegate)
    {
        for(int i=m_vMenuItemTableView.size()-1; i>=0; i--)
        {
            if(m_vMenuItemTableView.at(i) != table)
            {
                m_vMenuItemInfoWithTableView.pop_back();
                m_vMenuItemTableView.at(i)->removeFromSuperview();
                m_vMenuItemTableView.pop_back();
            }
            else
            {
                m_pMenuItemSelectedNow = m_vMenuItemInfoWithTableView.back();
                break;
            }
        }

        std::map<int, MenuInfo*>::iterator itr = m_pMenuItemSelectedNow->m_mChildMenuInfo.begin();
        for(; itr != m_pMenuItemSelectedNow->m_mChildMenuInfo.end(); itr++)
        {
            if( row+1 == itr->first)
            {
                CCRect tableRect = table->getFrame();
                std::map<int, MenuInfo*> menuInfoItem = (*(itr->second)).m_mChildMenuInfo;
                std::map<int, MenuInfo*>::iterator itrItem = menuInfoItem.begin();
                if(menuInfoItem.size()>0)
                {
                    m_pMenuItemSelectedNow = (itr->second);
                    float tableViewWidth = 0;
                    float tableViewHeight = 0;
                    for(; itrItem != menuInfoItem.end(); ++itrItem)
                    {
                        MenuInfo* menuinfo = itrItem->second;
                        tableViewWidth = menuinfo->m_pRect.size.width;
                        tableViewHeight += menuinfo->m_pRect.size.height;
                    }
                    m_vMenuItemInfoWithTableView.push_back((itr->second));
                    createMenuItemView(CCRect(tableRect.origin.x+tableRect.size.width,
                                              tableRect.origin.y+(*(itr->second)).m_pRect.origin.y,
                                              tableViewWidth,
                                              tableViewHeight));
                }
                else
                {
                    m_pDelegate->selectedListMenuViewItem(this, (*(itr->second)).m_iTag, (*(itr->second)).m_sTitle);
                    m_pDelegate->selectedOver();
                    this->setVisible(false);
                   // selecteMenuWithIndex(-1);
                    cleanMenuItemView();
                }
                break;
            }
        }
    }
}

void CAListMenuView::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* CAListMenuView::tableCellAtIndex(CATableView* table, const CCSize& cellsize, unsigned int section, unsigned int row)
{
    std::map<int, MenuInfo*>::iterator itr = m_pMenuItemSelectedNow->m_mChildMenuInfo.begin();
    for(; itr != m_pMenuItemSelectedNow->m_mChildMenuInfo.end(); itr++)
    {
        if( row+1 == itr->first)
            break;
    }
    CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CAMenuItemCell");
    if(cell == NULL)
    {
        cell = CATableViewCell::create("CAMenuItemCell");
        cell->setFrame(itr->second->m_pRect);
        
        CCRect labelRect;
        labelRect.origin.x = cellsize.width*0.5f;
        labelRect.origin.y = cellsize.height*0.5f;
        labelRect.size.width = cellsize.width-20;
        labelRect.size.height = cellsize.height;
        CALabel* label = CALabel::createWithCenter(labelRect);
        label->setText(itr->second->m_sTitle);
        label->setFontSize(LABEL_FONT_SIZE);
        cell->addSubview(label);
        if(itr->second->m_mChildMenuInfo.size()>0)
        {
            CAImageView* imageView = CAImageView::createWithCenter(CCRect(itr->second->m_pRect.size.width - 20, itr->second->m_pRect.size.height*0.5f, 25, 25));
            imageView->setImage(CAImage::create(NEXT_MENU_ITEM_IMAGE));
            cell->addSubview(imageView);
        }
    }
    return cell;
}

/*在section之间添加一个view，就是UI中section之间的灰色地带*/
CAView* CAListMenuView::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(ccc4(230, 230, 230, 255));
    return view;
}

/*设置每个section含有的cell个数*/
unsigned int CAListMenuView::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    int num = m_pMenuItemSelectedNow->m_mChildMenuInfo.size();
    return num;
}

/*设置含有几个section*/
unsigned int CAListMenuView::numberOfSections(CATableView *table)
{
    int num = 1;
    return num;
}

unsigned int CAListMenuView::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{	/*section的大小设置*/
    float height = 0;
    MAP_MENUINFO::iterator itr = m_pMenuItemSelectedNow->m_mChildMenuInfo.begin();
    for(; itr !=  m_pMenuItemSelectedNow->m_mChildMenuInfo.end(); itr++)
    {
        if(row+1 == itr->first)
        {
            height = itr->second->m_pRect.size.height;
            break;
        }
    }
    return _px(height);
}

unsigned int CAListMenuView::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    return _px(0);
}

