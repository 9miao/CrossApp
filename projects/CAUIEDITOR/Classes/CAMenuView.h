//
//  CAMenuView.h
//  CrossApp
//
//  Created by QiaoXin on 15-8-25.
//  Email:qiaoxin265@126.com
//  Copyright (c) 2015 http://9miao.com All rights reserved.
//

#ifndef __CrossApp__CAMenuView__
#define __CrossApp__CAMenuView__

#include <iostream>
#include "CrossApp.h"
#include "CAListMenuView.h"

USING_NS_CC;
NS_CC_BEGIN
class CAMenuView;
class CAMenuViewDelegate
{
public:
    virtual void selectedMenuViewItem(CAMenuView* menuView, int tag, std::string titleName){};
};

typedef std::map<int, MenuInfo*>  MAP_MENUINFO;
typedef std::map<int, CAView*>  MAP_MENU;

typedef void (CAObject::*SEL_CAControl)(CAControl*, CCPoint);
#define CAControl_selector(_SELECTOR) (SEL_CAControl)(&_SELECTOR)

class CAMenuView: public CAControl, public CAListMenuViewDelegate
{
public:

	CAMenuView();
    
	virtual ~CAMenuView();
    
    static CAMenuView* create(CCRect rect, int num);
    
    void setMenuWithIndex(std::string title, CCRect rect, int index);
    
    void setMenuItemWithIndex(std::string title, CCRect rect, int tag, int menuIndex, int menuItemIndex, ... );

    int getMenuCounts();
    
    void setDelegate(CAMenuViewDelegate* delegate);
    
protected:
    
    bool init();
    
    void setMenuCounts(int num);
    
    MenuInfo* findMenuInfoWithIndex(int index);
    
    CAView* findMenuWithIndex(int index);
    
    int getMenuIndexWithPoint(CCPoint point);
    
    void selecteMenuWithIndex(int index);
    
    void cleanMenuItemView();
    
    void cleanMenuInfo(MAP_MENUINFO menuInfo);
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void selectedListMenuViewItem(CAListMenuView* menuView, int tag, std::string titleName);
    virtual void selectedOver();
    
protected:
    MAP_MENUINFO        m_mMenuInfo;
    MAP_MENU            m_vMenu;
    int                 m_iMenuCounts;
    int                 m_iSelectedIndex;
    CAMenuViewDelegate* m_pDelegate;
    std::vector<CAListMenuView*> m_vListMenuView;
};
NS_CC_END

#endif /* defined(__CrossApp__CAMenuView__) */
