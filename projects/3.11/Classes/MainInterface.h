//
//  MainInterface.h
//  project
//
//  Created by 栗元峰 on 14-3-11.
//
//

#ifndef __project__MainInterface__
#define __project__MainInterface__

#include <iostream>
#include "LyfTableView.h"
#include "MainInterfaceCell.h"
#include "LTableBarController.h"
#include "cocos2d.h"

USING_NS_CC;

class MainInterface
: public LRootLayer
, public MysocketDelegate
, public LyfTableViewDelegate
, public LyfTableViewDataSource
{
    
    CCDictionary* m_pDict;
    
    deque<MainInterfaceData*> m_deque;
    
    LyfTableView* m_pTableView;
    
    int m_nType;
    
    CC_SYNTHESIZE(unsigned int, m_nCount, Count);
    
    int m_nCurrPage;
    
public:
    
    MainInterface();
    
    ~MainInterface();
    
    void onEnterTransitionDidFinish();
    
    void onExitTransitionDidStart();
    
    CREATE_FUNC(MainInterface);
    
    bool init();

    void settype(int type);
    
    void reLoadDate();
    
    void sendRequest_101();
    
    virtual void show();
    
protected:
    
    //初始化cell回调
    virtual LyfTableViewCell* tableCellAtIndex(LyfTableView *table, unsigned int idx);
    
    //获取cell总个数回调
    virtual unsigned int numberfRowsInTableView(LyfTableView *table);
    
    //cell选中代理回调
    virtual void tableCellTouched(LyfTableView* table, LyfTableViewCell* cell, unsigned int idx);
    
    //多选时，点击已经选中的cell时取消选中状态代理回调
    virtual void tableCellNoTouched(LyfTableView* table, LyfTableViewCell* cell, unsigned int idx){};
    
    //点击cell的ImageView时代理会掉
    virtual void tableCellImageViewTouchUpInSide(LyfTableView* table, LyfTableViewCell* cell, unsigned int idx){};
    
    //翻页是回调，可用此回调方法更新显示页
    virtual void tableViewUpdate(LyfTableView* table);
    
    virtual void callBackWithSocket(int msgID, const CSJson::Value& root);

};

#endif /* defined(__project__MainInterface__) */
