//
//  CAListMenuView.h
//  CrossApp
//
//  Created by QiaoXin on 15-8-27.
//  Email:qiaoxin265@126.com
//  Copyright (c) 2015 http://9miao.com All rights reserved.
//

#ifndef __CrossApp__CAListMenuView__
#define __CrossApp__CAListMenuView__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class CAListMenuView;
class CAListMenuViewDelegate
{
public:
    virtual void selectedListMenuViewItem(CAListMenuView* menuView, int tag, std::string titleName){};
    
    virtual void selectedOver(){};
};

class MenuInfo
{
public:
    MenuInfo(){
        m_sTitle = "";
        m_pRect  = CCRectZero;
        m_iTag   = 0;
    }
    ~MenuInfo(){
        m_mChildMenuInfo.clear();
    }
    
    int getMenuTag(){
        return m_iTag;
    }
    
    void setChildMenuInfoWithIndex(std::string title, CCRect rect, int index)
    {
        std::map<int, MenuInfo*>::iterator itr = m_mChildMenuInfo.begin();
        for(; itr != m_mChildMenuInfo.end(); itr++)
        {
            if(index == itr->first)
            {
                (itr->second)->m_sTitle = title;
                (itr->second)->m_pRect = rect;
            }
        }
    
        MenuInfo* menuInfo = new MenuInfo();
        menuInfo->m_sTitle = title;
        menuInfo->m_pRect = rect;
        m_mChildMenuInfo.insert(make_pair(index, menuInfo));
    }

    MenuInfo* getChildMenuInfoWithIndex(int index)
    {
        std::map<int, MenuInfo*>::iterator itr = m_mChildMenuInfo.begin();
        for(; itr != m_mChildMenuInfo.end(); itr++)
        {
            if(index == itr->first)
            {
                return (itr->second);
            }
        }
        MenuInfo* menuInfo = new MenuInfo();
        pair<std::map<int, MenuInfo*>::iterator, bool> rest = m_mChildMenuInfo.insert(make_pair(index, menuInfo));
        return rest.first->second;
    }
    
    std::string m_sTitle;
    CCRect      m_pRect;
    int         m_iTag;
    std::map<int, MenuInfo*> m_mChildMenuInfo;
};

typedef std::map<int, MenuInfo*>  MAP_MENUINFO;
typedef std::map<int, CAView*>  MAP_MENU;

class CAListMenuView: public CAControl, public CATableViewDelegate, public CATableViewDataSource
{
public:

	CAListMenuView();
    
	virtual ~CAListMenuView();
    
    static CAListMenuView* create();
    
    void setMenuItemWithIndex(std::string title, CCRect rect, int tag, int menuItemIndex, ... );

    void refreshMenuView();

    void setDelegate(CAListMenuViewDelegate* delegate);
    
    void showListMenuView(CCPoint point);
    
    bool init();
    
    void setMenuItemWithIndexForCAMenuView(std::string title, CCRect rect, int tag, int menuItemIndex, va_list argp);
    
    void cleanMenuItemView();

protected:

    void createMenuItemView(CCRect rect);
    
    void cleanMenuInfo(MAP_MENUINFO menuInfo);
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    /*CATableViewDelegate*/
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    /*CATableViewDataSource*/
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& viewSize, unsigned int section, unsigned int row);
    virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& cellsize, unsigned int section);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int numberOfSections(CATableView *table);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    
protected:
    MenuInfo*           m_mMenuInfo;
    int                 m_iSelectedIndex;
    CAListMenuViewDelegate* m_pDelegate;
    
    MenuInfo*           m_pMenuItemSelectedNow;
    std::vector<CATableView*>     m_vMenuItemTableView;
    std::vector<MenuInfo*>        m_vMenuItemInfoWithTableView;
};


#endif /* defined(__CrossApp__CAListMenuView__) */
