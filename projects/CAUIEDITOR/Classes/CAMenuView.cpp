//
//  CAMenuView.cpp
//  CrossApp
//
//  Created by QiaoXin on 15-8-25.
//  Email:qiaoxin265@126.com
//  Copyright (c) 2015 http://9miao.com All rights reserved.
//

#include "CAMenuView.h"
#include "CAListMenuView.h"

#define MENU_LABEL_TAG 100
#define MENU_BG_COLOR_NORMAL CAColor_clear
#define MENU_BG_COLOR_SELECTED CAColor_white
#define MENU_LABEL_COLOR_NORMAL CAColor_white
#define MENU_LABEL_COLOR_SELECTED CAColor_black

#define LABEL_FONT_SIZE 18
#define NEXT_MENU_ITEM_IMAGE "source_material/btn_right_blue.png"

NS_CC_BEGIN

CAMenuView::CAMenuView()
:m_iMenuCounts(0)
,m_iSelectedIndex(-1)
,m_pDelegate(NULL)
{

}

CAMenuView::~CAMenuView()
{
    cleanMenuInfo(m_mMenuInfo);
    
    m_vListMenuView.clear();
}

void CAMenuView::cleanMenuInfo(MAP_MENUINFO menuInfo)
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

CAMenuView* CAMenuView::create(CCRect rect, int num)
{
    CAMenuView* obj = new CAMenuView();
    if(obj && obj->init())
    {
        CAApplication* pDirector = CAApplication::getApplication();
        obj->setMenuCounts(num);
        obj->setFrame(CCRect(0,0,pDirector->getWinSize().width,pDirector->getWinSize().height));
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return obj;
}

bool CAMenuView::init()
{
    if(CAControl::init())
    {
        this->setColor(CAColor_clear);
        this->setHaveNextResponder(true);
        return true;
    }
    return false;
}

int CAMenuView::getMenuCounts()
{
    return m_iMenuCounts;
}

void CAMenuView::setMenuCounts(int num)
{
    m_iMenuCounts = num;
    for(int i=0; i<num; i++)
    {
        CAListMenuView* listMenuView =new CAListMenuView();
        listMenuView->init();
        listMenuView->setDelegate(this);
        listMenuView->setVisible(false);
        this->insertSubview(listMenuView,10);
        m_vListMenuView.push_back(listMenuView);
    }
}

void CAMenuView::setMenuWithIndex(std::string title, CCRect rect, int index)
{
    MenuInfo* menuInfo = findMenuInfoWithIndex(index);
    menuInfo->m_sTitle = title;
    menuInfo->m_pRect = rect;
    findMenuWithIndex(index);
}

void CAMenuView::setMenuItemWithIndex(std::string title, CCRect rect, int tag, int menuIndex, int menuItemIndex, ...)
{
    va_list args;
    
    va_start(args, menuItemIndex);
    
    CAListMenuView* listMenuView = m_vListMenuView.at(menuIndex-1);
    listMenuView->setMenuItemWithIndexForCAMenuView(title, rect, tag, menuItemIndex, args);
    va_end(args);
}

void CAMenuView::setDelegate(CAMenuViewDelegate* delegate)
{
    m_pDelegate = delegate;
}

#pragma mark -- Protected

MenuInfo* CAMenuView::findMenuInfoWithIndex(int index)
{
    if(index<1 || index > m_iMenuCounts)
        return NULL;
    MAP_MENUINFO::iterator itr = m_mMenuInfo.begin();
    for(; itr != m_mMenuInfo.end(); itr++)
    {
        if(index == itr->first)
        {
            return (itr->second);
        }
    }
    MenuInfo* menuInfo = new MenuInfo();
    pair<MAP_MENUINFO::iterator, bool> rest = m_mMenuInfo.insert(make_pair(index, menuInfo));
    return rest.first->second;
}

CAView* CAMenuView::findMenuWithIndex(int index)
{
    if(index<1 || index > m_iMenuCounts)
        return NULL;
    MAP_MENU::iterator itr = m_vMenu.begin();
    for(; itr != m_vMenu.end(); itr++)
    {
        if(index == itr->first)
        {
            return (itr->second);
        }
    }
    MenuInfo* menuInfo = findMenuInfoWithIndex(index);
    CCRect menuRect = menuInfo->m_pRect;
    CAView* menu = CAView::createWithFrame(menuRect);
    menu->setColor(MENU_BG_COLOR_NORMAL);
    CALabel* menuLabel = CALabel::createWithFrame(CCRect(0,0,menuRect.size.width,menuRect.size.height));
    menuLabel->setText(menuInfo->m_sTitle);
    menuLabel->setFontSize(LABEL_FONT_SIZE);
    menuLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    menuLabel->setTextAlignment(CATextAlignmentCenter);
    menuLabel->setColor(MENU_LABEL_COLOR_NORMAL);
    menuLabel->setTag(MENU_LABEL_TAG);
    menuLabel->setBold(true);
    menu->addSubview(menuLabel);
    this->addSubview(menu);
    pair<MAP_MENU::iterator, bool> rest = m_vMenu.insert(make_pair(index, menu));
    return rest.first->second;
}

int CAMenuView::getMenuIndexWithPoint(CCPoint point)
{
    float length = 0;
    MAP_MENU::iterator itr = m_vMenu.begin();
    for(; itr != m_vMenu.end(); ++itr)
    {
        CCRect menuRect = (itr->second)->getFrame();
        length = (itr->second)->getFrame().size.width + (itr->second)->getFrame().origin.x;
        if(point.x > menuRect.origin.x && point.x < ( menuRect.origin.x + menuRect.size.width)
           && point.y > menuRect.origin.y && point.y < ( menuRect.origin.y + menuRect.size.height))
        {
            return itr->first;
        }
    }
    return -1;
}

void CAMenuView::selecteMenuWithIndex(int index)
{
    CAView* menuOld = findMenuWithIndex(m_iSelectedIndex);
    if(menuOld)
    {
        menuOld->setColor(MENU_BG_COLOR_NORMAL);
        menuOld->getSubviewByTag(MENU_LABEL_TAG)->setColor(MENU_LABEL_COLOR_NORMAL);
    }

    CAView* menuSelected = findMenuWithIndex(index);
    if(menuSelected)
    {
        menuSelected->setColor(MENU_BG_COLOR_SELECTED);
        menuSelected->getSubviewByTag(MENU_LABEL_TAG)->setColor(MENU_LABEL_COLOR_SELECTED);
    }
    m_iSelectedIndex = index;
}

void CAMenuView::cleanMenuItemView()
{
    for(int i=0; i<m_vListMenuView.size(); i++)
    {
        CAListMenuView* listMenuView = m_vListMenuView.at(i);
        listMenuView->cleanMenuItemView();
    }
}

void CAMenuView::selectedListMenuViewItem(CAListMenuView* menuView, int tag, std::string titleName)
{
    if(m_pDelegate)
    {
        m_pDelegate->selectedMenuViewItem(this, tag, titleName);
    }
}

void CAMenuView::selectedOver()
{
    cleanMenuItemView();
    selecteMenuWithIndex(-1);
}

#pragma mark -- Touch

bool CAMenuView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    cleanMenuItemView();
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    int index = getMenuIndexWithPoint(point);
    selecteMenuWithIndex(index);

    if(index == -1)
        return true;
    
    return true;
}

void CAMenuView::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{

}

void CAMenuView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    int index = getMenuIndexWithPoint(point);
    
    if(index == -1)
        return;
    selecteMenuWithIndex(index);
    CAView* menu = findMenuWithIndex(index);
    CCRect menuRect = menu->getFrame();
    menuRect.origin.y += menuRect.size.height;
    CAListMenuView* listMenuView = m_vListMenuView.at(index-1);

    listMenuView->showListMenuView(menuRect.origin);
}

void CAMenuView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{

}

NS_CC_END
