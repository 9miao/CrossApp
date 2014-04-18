//
//  TextShow.h
//  project
//
//  Created by 栗元峰 on 14-3-11.
//
//

#ifndef __project__TextShow__
#define __project__TextShow__

#include <iostream>
#include "MainInterfaceData.h"
#include "PostReviewData.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LMenu.h"
#include "LyfTableView.h"
#include "LTableBarController.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TextShow
: public LRootLayer
, public MysocketDelegate
, public LyfTableViewDataSource
{
    MainInterfaceData* m_pData;

    CCArray* m_pArray;
    
    vector<PostReviewData> m_nPostReviewVec;
    
    vector<string> m_nTextVec;
    
    vector<string> m_nUrlVec;
    
    vector<string> m_nHyperlinkVec;
    
    LyfTableView* m_pTableView;
    
    unsigned int m_nUrlIndex;
    
    unsigned int m_nHyperlinkIndex;
    
public:
    
    TextShow(MainInterfaceData* data);
    
    ~TextShow();
    
    void onEnterTransitionDidFinish();
    
    void onExitTransitionDidStart();
    
    static TextShow* create(MainInterfaceData* data);
    
    bool init();
    
    virtual void callBackWithSocket(int msgID, const CSJson::Value& root);
    
    virtual void show();
    
    //初始化cell回调
    virtual LyfTableViewCell* tableCellAtIndex(LyfTableView *table, unsigned int idx);
    
    //获取cell总个数回调
    virtual unsigned int numberfRowsInTableView(LyfTableView *table);
};


#endif /* defined(__project__TextShow__) */
