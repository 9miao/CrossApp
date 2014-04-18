//
//  MainInterface.cpp
//  project
//
//  Created by 栗元峰 on 14-3-11.
//
//
#define WINSIZE CCDirector::sharedDirector()->getWinSize()

#include "MainInterface.h"
#include "TextShow.h"
#include "CCScrollViewX.h"

MainInterface::MainInterface()
:m_nCurrPage(0)
,m_nCount(0)
{
    m_pDict = new CCDictionary();
    
    
}

MainInterface::~MainInterface()
{
    delete m_pDict;
}

void MainInterface::onEnterTransitionDidFinish()
{
    LRootLayer::onEnterTransitionDidFinish();
    
    MySocket::shareMySocket()->addDelegate(101, this);
    MySocket::shareMySocket()->addDelegate(102, this);
}

void MainInterface::onExitTransitionDidStart()
{
    LRootLayer::onExitTransitionDidStart();
    
    MySocket::shareMySocket()->removeDelegate(101, this);
    MySocket::shareMySocket()->removeDelegate(102, this);
}

bool MainInterface::init()
{
    if (!LRootLayer::init())
    {
        return false;
    }
    this->setDisplayRange(false);
    m_pTableView = LyfTableView::create(m_obContentSize.height);
    m_pTableView->setTableViewDataSource(this);
    m_pTableView->setTableViewDelegate(this);
    m_pTableView->setFrame(CCRectZero);
    this->addChild(m_pTableView);
    m_pTableView->setIndicatorPointOff(-15);
    m_pTableView->setMaxNumberOfPages(20);
    m_pTableView->setSelectedCount(1);

    CCLayerColor* layerColor = CCLayerColor::createWithFrame(CCRect(0, 0, m_obContentSize.width, m_obContentSize.height));
    layerColor->setColor(ccc3(127, 127, 127));
    m_pTableView->addChild(layerColor, -1);
    
    return true;
}

void MainInterface::settype(int type)
{
    MySocket::shareMySocket()->send(NULL, 102);
}

void MainInterface::reLoadDate()
{
    m_pTableView->setPage(m_nCurrPage);
    m_pTableView->readData(false);
}

void MainInterface::sendRequest_101()
{
    do
    {
        int currPage = m_pTableView->getPage();
        int count = m_nCount - m_nCurrPage * 20;
        count = MIN(20, count);
        
        bool flag = true;
        
        for (unsigned int i=0; i<count; i++)
        {
            if (m_deque.at(i+m_nCurrPage*20) == NULL)
            {
                flag = false;
            }
        }
        
        CC_BREAK_IF(flag == true);
        
        map<string, string> m;
        char pageStr[2];
        sprintf(pageStr, "%d", currPage);
        m.insert(map<string, string>::value_type("page", pageStr));
        
        char countStr[2];
        sprintf(countStr, "%d", count);
        m.insert(map<string, string>::value_type("count", countStr));
        
        MySocket::shareMySocket()->send(&m, 101);
    }
    while (0);
}

void MainInterface::show()
{

}

//初始化cell回调
LyfTableViewCell* MainInterface::tableCellAtIndex(LyfTableView *table, unsigned int idx)
{
    LyfTableViewCell* cell = NULL;
    
    if (idx == 0)
    {
        cell = LyfTableViewCell::createTmp();
        CCScrollViewX* scrollView = CCScrollViewX::create();
        scrollView->setFrame(CCRectZero);
        cell->addChild(scrollView);
        cell->setContentSize(scrollView->getViewSize());
        
    }
    else
    {
        cell = MainInterfaceCell::create(m_deque.at(idx - 1));
    }

    return cell;
}

//获取cell总个数回调
unsigned int MainInterface::numberfRowsInTableView(LyfTableView *table)
{
    return m_nCount + 1;
}

void MainInterface::tableCellTouched(LyfTableView* table, LyfTableViewCell* cell, unsigned int idx)
{
    if (idx == 0)
    {
        return;
    }
    
    TABBAR_CONTROLLER->pushLayer(TextShow::create(m_deque.at(idx - 1)));
    
    table->setNoPitchOnOfArray(cell);
}

void MainInterface::tableViewUpdate(LyfTableView* table)
{
    do
    {
        CC_BREAK_IF(m_nCurrPage == table->getPage() - 1);
        m_nCurrPage = table->getPage() - 1;
        this->sendRequest_101();
    }
    while (0);
}

void MainInterface::callBackWithSocket(int msgID, const CSJson::Value& root)
{
    switch (msgID) {
        case 101:
        {
            CCArray* array = new CCArray();
            
            do
            {
                int size = root.size();
                for (int i=0; i<size; i++)
                {
                    MainInterfaceData* data = new MainInterfaceData();
                    data->ID = root[i]["id"].asInt();
                    data->catID = root[i]["catid"].asInt();
                    data->title = root[i]["title"].asString();
                    data->text = root[i]["keywords"].asString();
                    data->username = root[i]["username"].asString();
                    array->addObject(data);
                    data->release();
                }
            }
            while (0);
            
            for (unsigned int i=0; i<array->count(); i++)
            {
                MainInterfaceData* data = (MainInterfaceData*)array->objectAtIndex(i);
                m_pDict->setObject(data, data->ID);
                m_deque.at(m_nCurrPage*20+i) = data;
            }
            
            this->scheduleOnce(schedule_selector(MainInterface::reLoadDate), 1/60.0f);
        }
            break;
        case 102:
        {
            m_nCount = root.asInt();
            m_deque = deque<MainInterfaceData*>(m_nCount);
            for (deque<MainInterfaceData*>::iterator itr=m_deque.begin(); itr!=m_deque.end(); itr++)
            {
                *itr = NULL;
            }
            this->sendRequest_101();
        }
            break;
        default:
            break;
    }
}