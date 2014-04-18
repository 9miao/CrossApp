//  出自九秒社区 http://www.9miao.com
//
//  LyfTableView.cpp
//  LTabelView
//
//  Created by LI YUANFENG on 13-3-11.
//
//

#include "LyfTableView.h"

#define MAX_SPEED (64)
#define MAXPULLINGSPEED MAX_SPEED*2.0
#define OBSTRUCTION m_nInertiaLength/15.0f
#define SELETED_CURRENT (this->getMaxNumberOfPages()*m_page)


#define MAIN_HANZI_FONT "Helvetica-Bold"
#define ON_ONE_PAGE_LABEL "向上滑动翻页"
#define THE_NEXT_PAGE_LABEL "向下滑动翻页"
#define LOOSEN_ON_ONE_PAGE_LABEL "松开翻下页"
#define LOOSEN_THE_NEXT_PAGE_LABEL "松开翻上页"
#define PULL_SHOW_IMAGE "pull_show_image.png"

#define UPDATE_TIME 0.3f

class LPullShowSprite: public CCSprite
{
  
private:

    CCSprite* pullShowSprite;
    
    CCSprite* pullShowLabel;
    
    CCSprite* waitingSprite;
    
    bool m_isTop;
    
public:
    
    LPullShowSprite(bool isTop)
    :pullShowSprite(NULL)
    ,pullShowLabel(NULL)
    ,waitingSprite(NULL)
    {
        m_isTop=isTop;
    }
    
    virtual ~LPullShowSprite()
    {
        
    }
    
    void onEnter()
    {
        CCSprite::onEnter();
        
        this->refurbishPoint();
    }
    
    void onExit()
    {
        CCSprite::onExit();
    }
    
    static LPullShowSprite* create(bool isTop)
    {
        
        LPullShowSprite* sprite=new LPullShowSprite(isTop);
        if (sprite && sprite->init())
        {
            sprite->autorelease();
            return sprite;
        }
        
        CC_SAFE_DELETE(sprite);
        return NULL;
        
    }
    
    bool init()
    {
        if (!CCSprite::init())
        {
            return false;
        }
        
        if (m_isTop)
        {
            pullShowSprite=CCSprite::create(PULL_SHOW_IMAGE);
            this->addChild(pullShowSprite);
            pullShowSprite->setRotation(180);
            
            pullShowLabel=CCLabelTTF::create(ON_ONE_PAGE_LABEL, MAIN_HANZI_FONT, dvalue(11));
            this->addChild(pullShowLabel);
        }
        else
        {
            pullShowSprite=CCSprite::create(PULL_SHOW_IMAGE);
            this->addChild(pullShowSprite);
            
            pullShowLabel=CCLabelTTF::create(THE_NEXT_PAGE_LABEL, MAIN_HANZI_FONT, dvalue(11));
            this->addChild(pullShowLabel);
        }
        
        waitingSprite=CCLabelTTF::create("正在加载", MAIN_HANZI_FONT, dvalue(11));
        this->addChild(waitingSprite);
        waitingSprite->setVisible(false);
        
        CCSize size=CCSize(pullShowSprite->getContentSize().width+pullShowLabel->getContentSize().width, pullShowSprite->getContentSize().height);
        
        this->setContentSize(size);
        
        return true;
    }
    
    void loosen()
    {
        if (m_isTop)
        {
            pullShowSprite->setRotation(0);
            ((CCLabelTTF*)pullShowLabel)->setString(LOOSEN_ON_ONE_PAGE_LABEL);
        }
        else
        {
            pullShowSprite->setRotation(180);
            ((CCLabelTTF*)pullShowLabel)->setString(LOOSEN_THE_NEXT_PAGE_LABEL);
        }
    }
    
    void pullWhack()
    {
        if (m_isTop)
        {
            pullShowSprite->setRotation(180);
            ((CCLabelTTF*)pullShowLabel)->setString(ON_ONE_PAGE_LABEL);
        }
        else
        {
            pullShowSprite->setRotation(0);
            ((CCLabelTTF*)pullShowLabel)->setString(THE_NEXT_PAGE_LABEL);
        }
    }
    
    
    void inWaiting()
    {
        this->pullWhack();
        
        pullShowSprite->setVisible(false);
        pullShowLabel->setVisible(false);
        waitingSprite->setVisible(true);
        this->inWaitingActions();
        
    }
    
    void inWaitingActions()
    {
        this->unschedule(schedule_selector(LPullShowSprite::inWaitingActions));
        
        string str=string(((CCLabelTTF*)waitingSprite)->getString());
        
        if (strcmp(str.c_str(), "正在加载......") == 0)
        {
            ((CCLabelTTF*)waitingSprite)->setString("正在加载");
            pullShowSprite->setVisible(true);
            pullShowLabel->setVisible(true);
            waitingSprite->setVisible(false);
            return;
        }
        else
        {
            str+=".";
            
            ((CCLabelTTF*)waitingSprite)->setString(str.c_str());
            
            this->schedule(schedule_selector(LPullShowSprite::inWaitingActions), 0.05f);
        }
    }
    
    void refurbishPoint()
    {
        CCPoint pullShowSpritePoint=CCPoint(pullShowSprite->getContentSize().width/2, this->getContentSize().height/2);
        
        pullShowSprite->setPosition(pullShowSpritePoint);
        
        CCPoint pullShowLabelPoint=CCPoint(pullShowSprite->getContentSize().width+pullShowLabel->getContentSize().width/2, this->getContentSize().height/2);
        
        pullShowLabel->setPosition(pullShowLabelPoint);
        
        CCPoint waitingSpritePoint=CCPoint(this->getContentSize().width/2, this->getContentSize().height/2);
        
        waitingSprite->setPosition(waitingSpritePoint);
        
    }
    
    void setOpacity(GLubyte opacity)
    {
        pullShowSprite->setOpacity(opacity);
        pullShowLabel->setOpacity(opacity);
        waitingSprite->setOpacity(opacity);
    }
    
};



LyfTableView::LyfTableView()
:m_pDataSource(NULL)
,m_pTableViewDelegate(NULL)
,m_indicator(NULL)
,m_numOfRows(0)
,m_sizeHeight(0)
,m_maxNumberOfPages(MAXCELLCOUNT)
,m_selectedCount(0)
,m_page(0)
,m_maxPage(0)
,m_nInertiaLength(0)
,pullUpShowSprite(NULL)
,pullDownShowSprite(NULL)
,m_nIndicator_offX(0)
,m_nIndicator_scaleX(1.0f)
,m_nEdit(false)
,m_CellEditing(-1)
,m_IsRunning(false)
,m_backNode(NULL)
,m_IsCanSkip(true)
,m_IsTouchMoved(true)
,m_touchBeginUIPoint(CCPointZero)
,m_nTouchPriority(0)
,m_IsRehisterTouchDispatcher(false)
{

}

LyfTableView::~LyfTableView()
{
    m_TableViewCells.clear();
    m_SelectedIndexPaths.clear();
    m_DisabledIndexPaths.clear();
}

void LyfTableViewDataSource::removeDelegate(LyfTableView* table){
    
    table->setTableViewDataSource(NULL);
    table->setTableViewDelegate(NULL);
}

void LyfTableView::onExit(){

    CCLayerColor::onExit();
    
    m_pDataSource->removeDelegate(this);
    
    this->unRegisterWithTouchDispatcher();

    m_TableViewCells.clear();
}

void LyfTableView::onEnter(){
    
    CCLayerColor::onEnter();
    
    this->registerWithTouchDispatcher();
    
    this->setTableViewCells();
}

void LyfTableView::registerWithTouchDispatcher(){
    
    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    touchDispatcher->addTargetedDelegate(this, m_nTouchPriority, false);
    m_IsRehisterTouchDispatcher=true;
}

void LyfTableView::unRegisterWithTouchDispatcher(){
    
    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    touchDispatcher->removeDelegate(this);
    m_IsRehisterTouchDispatcher=false;
}

void LyfTableView::setPriority(int nPriority) {
    
    m_nTouchPriority=nPriority;
    
    if (m_IsRehisterTouchDispatcher)
    {
        CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        touchDispatcher->setPriority(m_nTouchPriority, this);
        
        for (int i=0; i<m_TableViewCells.size(); i++)
        {
            m_TableViewCells.at(i)->setPriority(m_nTouchPriority);
        }
    }
}

LyfTableView* LyfTableView::create(float contentSizeHeight){
    
    LyfTableView* tabelView=new LyfTableView();
    
    if (tabelView && tabelView->initWithTableView(contentSizeHeight))
    {
        tabelView->autorelease();
        return tabelView;
    }
    
    CC_SAFE_DELETE(tabelView);
    
    return NULL;
}

bool LyfTableView::initWithTableView(float contentSizeHeight){

    if (!CCLayerColor::init())
    {
        return false;
    }
    CCLayerColor::setOpacity(0);
    m_obAnchorPoint = CCPointZero;
    
    this->setDisplayRange(false);
    this->setTouchMoved(true);
    this->setContentSize(CCSize(0, contentSizeHeight));

    m_backNode=CCNode::create();
    m_backNode->setPosition(CCPoint(0, this->getContentSize().height));
    this->addChild(m_backNode);

    this->setIndicator();
    this->setPullShow();
    this->scheduleUpdate();
    
    return true;
}

void LyfTableView::setContentSizeHeight(float contentSizeHeight)
{
    this->setContentSize(CCSize(m_obContentSize.width, contentSizeHeight));
    m_backNode->setPosition(ccp(0, contentSizeHeight));
    this->updateTableViewCellPos();
    this->updateIndicator();
    this->updatePullShow();
    this->updatePitchOn();
}

float LyfTableView::getRowHight(unsigned int idx){
    
    return m_TableViewCells.at(idx)->getContentSize().height;
}

float LyfTableView::getTableViewHight(){
    
    return m_sizeHeight;
}

int LyfTableView::getNumberOfRows(){
    
    if (m_pDataSource)
    {
        m_numOfRows=m_pDataSource->numberfRowsInTableView(this);
    }

    return m_numOfRows;
}

CCRect LyfTableView::getRect(){
    
    return CCRect(0,0,getContentSize().width,getContentSize().height);
}

bool LyfTableView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    m_touchBeginUIPoint = pTouch->getLocationInView();
    
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (!this->isVisible()) return false;
    
    if (!getRect().containsPoint(point)) return false;

    if (!m_IsTouchMoved) return false;

    if (isEditing()) return false;

    if (m_IsRunning) return false;
    
    m_backNode->stopAllActions();

    m_nInertiaLength = 0;
    
    return true;
}

void LyfTableView::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    float lenght=ccpDistance(pTouch->getLocationInView(), m_touchBeginUIPoint);
    
    CCPoint point_last = pTouch->getPreviousLocation();
    point_last = this->convertToNodeSpace(point_last);
    
    CCPoint point_this = pTouch->getLocation();
    point_this = this->convertToNodeSpace(point_this);
    
    if (isEditing()) return;
    
    float relustPointY = m_backNode->getPosition().y;
    float topCutOff = this->getHomeFromCellPos().y;
    float bottomCutOff = this->getFirmFromCellPos().y;
    float rop = 1.0f;
    float height=this->getContentSize().height;
    
    if (relustPointY<topCutOff)
    {
        rop = 0.5 - (topCutOff-m_backNode->getPositionY())/height;
        this->pullDownShow();
    } 
    else if (relustPointY>bottomCutOff)
    {
        rop = 0.5 - (m_backNode->getPositionY() - bottomCutOff)/height;
        this->pullUpShow();
    }
    relustPointY = m_backNode->getPosition().y + (point_this.y-point_last.y)*rop;
    
    if (!m_IsCanSkip)
    {
        if (relustPointY <= topCutOff)
        {
            point_last = point_this;
            relustPointY = topCutOff;
        }
        if (relustPointY >= bottomCutOff)
        {
            point_last = point_this;
            relustPointY = bottomCutOff;
        }
    }
    m_backNode->setPositionY(relustPointY);
    
    if (lenght < TOUCH_PROTECTION_ACCURACY) return;
    
    m_nPointOffset.push_back(ccpSub(point_this, point_last));

    if (m_nPointOffset.size() > 3)
    {
        m_nPointOffset.pop_front();
    }
}

void LyfTableView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchBeginUIPoint = CCPointZero;
    
    CCPoint point=pTouch->getLocation();
    point=this->convertToNodeSpace(point);

    this->cellGoHome();
    
    CCPoint p = CCPointZero;
    
    if (m_nPointOffset.size() > 0)
    {
        for (unsigned int i=0; i<m_nPointOffset.size(); i++)
        {
            p = ccpAdd(p, m_nPointOffset.at(i));
        }
        p = p/m_nPointOffset.size();
    }
    
    if (this->isCanSelected() && fabsf(p.y) >= sqrtf(TOUCH_PROTECTION_ACCURACY))
    {
        m_nInertiaLength = p.y;
        this->schedule(schedule_selector(LyfTableView::inertia));
    }
    
    m_nPointOffset.clear();
}

void LyfTableView::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    this->ccTouchEnded(pTouch, pEvent);
}

void LyfTableView::inertia(){
    
    if (m_backNode->getPositionY() <= getHomeFromCellPos().y)
    {
        m_backNode->setPosition(getHomeFromCellPos());
        m_nInertiaLength=0;
    }
    else if (m_backNode->getPositionY() > getFirmFromCellPos().y)
    {
        m_backNode->setPosition(getFirmFromCellPos());
        m_nInertiaLength=0;
    }
    
    if (fabsf(m_nInertiaLength)<1.0f)
    {
        this->unschedule(schedule_selector(LyfTableView::inertia));
    }
    
    if (fabsl(m_nInertiaLength)>MAXPULLINGSPEED)
    {
        m_nInertiaLength=MAXPULLINGSPEED * fabsl(m_nInertiaLength)/m_nInertiaLength;
    }
    
    float speed;
    if (fabsl(m_nInertiaLength)>MAX_SPEED) {
        speed=MAX_SPEED *m_nInertiaLength/fabsl(m_nInertiaLength);
    }
    else
    {
        speed=m_nInertiaLength;
    }
    
    m_backNode->setPositionY(m_backNode->getPositionY()+speed);
    m_nInertiaLength-=OBSTRUCTION;
}

void LyfTableView::readDataInTurnLastPage(){

    this->reLoadData();
    m_backNode->setPosition(CCPoint(0, m_sizeHeight+this->getContentSize().height));
    CCCallFunc* callFunc=CCCallFunc::create(this, callfunc_selector(LyfTableView::PullShowHideFalse));
    CCSequence* finish=CCSequence::create(cellAction(getFirmFromCellPos(),0.3f), callFunc, NULL);
    m_backNode->runAction(finish);
}

void LyfTableView::readDataInTurnNextpage(){
    
    this->reLoadData();
    m_backNode->setPosition(CCPointZero);
    CCCallFunc* callFunc=CCCallFunc::create(this, callfunc_selector(LyfTableView::PullShowHideFalse));
    CCSequence* finish=CCSequence::create(cellAction(getHomeFromCellPos(),0.3f), callFunc, NULL);
    m_backNode->runAction(finish);
}

void LyfTableView::turnLastpage(){
    
    if (m_IsRunning) return;

    m_IsRunning=true;
    m_page--;
    ((LPullShowSprite*)pullDownShowSprite)->inWaiting();
    pullUpShowSprite->setOpacity(0);
    
    m_indicator->setVisible(false);
    this->scheduleOnce(schedule_selector(LyfTableView::readDataInTurnLastPage), UPDATE_TIME);
}

void LyfTableView::turnNextPage(){

    if (m_IsRunning) return;
    
    m_IsRunning=true;
    m_page++;
    ((LPullShowSprite*)pullUpShowSprite)->inWaiting();
    pullDownShowSprite->setOpacity(0);
    
    m_indicator->setVisible(false);
    this->scheduleOnce(schedule_selector(LyfTableView::readDataInTurnNextpage), UPDATE_TIME);
}

void LyfTableView::cellGoHome(){
    
    if (!m_IsCanSkip) return;

    if (!pullUpShowSprite || !pullDownShowSprite) return;
    
    if (isTurnLastPage())
    {
        this->turnLastpage();
    }
    else if (isTurnNextPage())
    {
        this->turnNextPage();
    }
    else
    {
        float topCutOff=this->getHomeFromCellPos().y;
        float bottomCutOff=this->getFirmFromCellPos().y;
        
        if (m_backNode->getPositionY()<topCutOff)
        {
            m_backNode->runAction(cellAction(getHomeFromCellPos()));
        }
        
        if (m_backNode->getPositionY()>bottomCutOff) {
            m_backNode->runAction(cellAction(getFirmFromCellPos()));
        }
    }
}

void LyfTableView::setSelectedArray(cocos2d::CCArray *array){
    
    m_SelectedIndexPaths.clear();
    for (int i=0; i<array->count(); i++)
    {
        LyfIndexPath indexPath=LyfIndexPath(*(LyfIndexPath*)array->objectAtIndex(i));
        m_SelectedIndexPaths.push_back(indexPath);
    }
    
    this->updatePitchOn();
}

void LyfTableView::setNoClickOtherArray(cocos2d::CCArray *array){
    
    for (int i=0; i<m_TableViewCells.size(); i++)
    {
        LyfTableViewCell* tableViewCell=m_TableViewCells.at(i);
        tableViewCell->setTouchEnabled(true);
    }
    
    m_DisabledIndexPaths.clear();
    
    if (array==NULL) return;

    for (int i=0; i<array->count(); i++)
    {
        LyfIndexPath indexPath=*(LyfIndexPath*)array->objectAtIndex(i);
        m_DisabledIndexPaths.push_back(LyfIndexPath(indexPath));
    }
    
    for (int i=0; i<m_DisabledIndexPaths.size(); i++)
    {
        LyfIndexPath indexPath=m_DisabledIndexPaths.at(i);
        if (indexPath.page==m_page && indexPath.row<m_TableViewCells.size())
        {
            m_TableViewCells.at(indexPath.row)->setTouchEnabled(false);
        }
    }
}

void LyfTableView::readData(bool flag){
    
    if (flag)
    {
        m_page=0;
        m_backNode->setPosition(ccp(0, this->getContentSize().height));
    }
    
    m_SelectedIndexPaths.clear();
    
    if (m_TableViewCells.size()>0)
    {
        for (int i=0; i<m_TableViewCells.size(); i++)
        {
            LyfTableViewCell* cell=m_TableViewCells.at(i);
            cell->removeFromParentAndCleanup(true);
        }
        m_TableViewCells.clear();
    }
    
    if (getNumberOfRows()==0)
    {
        if (m_indicator)
        {
            m_indicator->setVisible(false);
        }
        return;
    }
    
    this->setTableViewCells();
    
    if (flag)
    {
        this->cellGoHome();
    }
}

void LyfTableView::reLoadData(){
    
    if (m_TableViewCells.size()>0)
    {
        for (int i=0; i<m_TableViewCells.size(); i++)
        {
            LyfTableViewCell* cell=m_TableViewCells.at(i);
            cell->removeFromParentAndCleanup(true);
        }
        m_TableViewCells.clear();
    }
    
    this->setTableViewCells();
}

void LyfTableView::setPage(const int page){
    
    if (page<0) return;

    m_page=page;
    
    this->readData(false);
}

int LyfTableView::getPage(){
    
    return (m_page+1);
}

int LyfTableView::getMaxPage(){
    
    return (m_maxPage+1);
}

void LyfTableView::setIndicatorPointOff(float offX){
    
    m_nIndicator_offX=offX;
    
    if (m_indicator)
    {
        m_indicator->setPointOff(CCPoint(m_nIndicator_offX, 0));
    }
}

void LyfTableView::setIndicatorScaleX(float scaleX){
    
    m_nIndicator_scaleX=scaleX;
    
    if (m_indicator)
    {
        m_indicator->setScaleX(m_nIndicator_scaleX);
    }
}

void LyfTableView::setNodePosY(float y){
    
    this->unschedule(schedule_selector(LyfTableView::inertia));
    m_nInertiaLength=0;
    
    if (y<getHomeFromCellPos().y)
    {
        y=getHomeFromCellPos().y;
    }
    else if (y>getFirmFromCellPos().y)
    {
        y=getFirmFromCellPos().y;
    }
    
    m_backNode->setPositionY(y);
}

void LyfTableView::setIndicator(){
    
    if (m_indicator) return;
    
    float indScale=this->getContentSize().height/m_sizeHeight;
    
    m_indicator=LyfIndicator::createWithAddChild(this, indScale);
}

void LyfTableView::updateIndicator(){
    
    float indScale=this->getContentSize().height/m_sizeHeight;
    
    m_indicator->refresh();
    m_indicator->setScale(indScale);
    m_indicator->setPointOff(CCPoint(m_nIndicator_offX, 0));
    m_indicator->setScaleX(m_nIndicator_scaleX);

    if (indScale>=1)
    {
        m_indicator->setVisible(false);
    }
    else
    {
        m_indicator->setVisible(true);
    }
    
}

void LyfTableView::setPullShow(){
    
    LPullShowSprite* upShow=LPullShowSprite::create(true);
    m_backNode->addChild(upShow);
    pullUpShowSprite=upShow;
    pullUpShowSprite->setVisible(false);
    
    LPullShowSprite* downShow=LPullShowSprite::create(false);
    m_backNode->addChild(downShow);
    pullDownShowSprite=downShow;
    pullDownShowSprite->setVisible(false);
}

void LyfTableView::updatePullShow(){

    if (m_numOfRows<=m_maxNumberOfPages)
    {
        pullUpShowSprite->setVisible(false);
        pullDownShowSprite->setVisible(false);
        return;
    }

    CCPoint pullUpSpritePoint=CCPoint(this->getContentSize().width/2,
                                      -m_sizeHeight-pullUpShowSprite->getContentSize().height/2);
    CCPoint pullDownSpritePoint=CCPoint(this->getContentSize().width/2,
                                        pullDownShowSprite->getContentSize().height/2);
    
    pullUpShowSprite->setPosition(pullUpSpritePoint);
    pullDownShowSprite->setPosition(pullDownSpritePoint);
    
    pullUpShowSprite->setVisible(true);
    pullDownShowSprite->setVisible(true);
    
    if (m_page==m_maxPage)
    {
        pullUpShowSprite->setVisible(false);
    }
    
    if (m_page==0)
    {
        pullDownShowSprite->setVisible(false);
    }

}

void LyfTableView::pullDownShow(){
    
    if (isTurnLastPage()) {
        
        ((LPullShowSprite*)pullDownShowSprite)->loosen();
    }
    else
    {
        ((LPullShowSprite*)pullDownShowSprite)->pullWhack();
    }
}

void LyfTableView::pullUpShow(){
    
    if (isTurnNextPage())
    {
        ((LPullShowSprite*)pullUpShowSprite)->loosen();
    }
    else
    {
        ((LPullShowSprite*)pullUpShowSprite)->pullWhack();
    }
}

bool LyfTableView::isTurnLastPage(){
    
    if (!pullDownShowSprite->isVisible()) return false;
    
    return (pullDownShowSprite->getPositionY()+m_backNode->getPositionY()+pullDownShowSprite->getContentSize().height/2+dvalue(10)<this->getContentSize().height);
}   

bool LyfTableView::isTurnNextPage(){
    
    if (!pullUpShowSprite->isVisible()) return false;
    
    return (pullUpShowSprite->getPositionY()+m_backNode->getPositionY()-pullUpShowSprite->getContentSize().height/2-dvalue(10)>0);
}

void LyfTableView::PullShowHideFalse(){
    
    if (pullUpShowSprite)
    {
        pullUpShowSprite->setOpacity(255);
    }
    
    if (pullDownShowSprite)
    {
        pullDownShowSprite->setOpacity(255);
    }
    
    m_indicator->setVisible(true);
    
    m_IsRunning=false;
}

void LyfTableView::updatePitchOn(){
    
    if (m_TableViewCells.empty()) return;

    for (int i=0; i<m_SelectedIndexPaths.size(); i++)
    {
        LyfIndexPath indexPath=m_SelectedIndexPaths.at(i);
        if (indexPath.page==m_page)
        {
            LyfTableViewCell* cell= m_TableViewCells.at(indexPath.row);
            cell->setPitchOn(true);
        }
    }
}

void LyfTableView::setTableViewCells(){
    
    if (getNumberOfRows()==0)
    {
        m_indicator->setVisible(false);
        return;
    }
    
    m_indicator->setVisible(true);
    
    unsigned int cellNumber=this->getMaxNumberOfPages();
    
    if (getNumberOfRows()<this->getMaxNumberOfPages())
    {
        cellNumber=getNumberOfRows();
    }
    
    if (getNumberOfRows()-SELETED_CURRENT<this->getMaxNumberOfPages())
    {
        cellNumber=getNumberOfRows()-SELETED_CURRENT;
    }
    
    for ( int i=0; i<cellNumber; i++)
    {
        LyfTableViewCell* cell=m_pDataSource->tableCellAtIndex(this, i+SELETED_CURRENT);
        cell->setDelegate(this);
        cell->setPriority(m_nTouchPriority);
        m_backNode->addChild(cell);
        m_TableViewCells.push_back(cell);

    }
    
    for (int i=0; i<m_DisabledIndexPaths.size(); i++)
    {
        LyfIndexPath indexPath=m_DisabledIndexPaths.at(i);
        if (indexPath.page==m_page && indexPath.row<m_TableViewCells.size())
        {
            m_TableViewCells.at(indexPath.row)->setTouchEnabled(false);
        }
    }
    
    this->updateTableViewCellPos();
    this->updateIndicator();
    this->updatePullShow();
    this->updatePitchOn();
    
}

void LyfTableView::updateTableViewCellPos(){
    
    m_maxPage=(getNumberOfRows()-1)/this->getMaxNumberOfPages();
    
    if (m_page>m_maxPage)
    {
        m_page=m_maxPage;
    }

    float sizeHeight = 0;
    
    if (m_TableViewCells.size()>0 && this->getContentSize().width==0)
    {
        LyfTableViewCell* cell=m_TableViewCells.front();
        this->setContentSize(CCSizeMake(cell->getContentSize().width, this->getContentSize().height));
    }
    
    for (int i=0; i<m_TableViewCells.size(); i++)
    {
        LyfTableViewCell* cell=m_TableViewCells.at(i);
        
        cell->setSign(LyfIndexPath(i, m_page, m_maxNumberOfPages));
        
        cell->setAnchorPoint(CCPoint(0.5f, 0.5f));
        
        cell->setPosition(CCPoint(cell->getContentSize().width/2,
                                  -(cell->getContentSize().height/2+sizeHeight)));
        
        sizeHeight=cell->getContentSize().height/2-cell->getPositionY();
    }
    
    m_sizeHeight=sizeHeight;
    
    if (!m_pTableViewDelegate) return;
    
    m_pTableViewDelegate->tableViewUpdate(this);

}

LyfTableViewCell* LyfTableView::insertTableCellAtIndex(unsigned int idx){
    
    int index=idx-m_page*m_maxNumberOfPages;
    
    if (index>=m_maxNumberOfPages) return NULL;
    
    if (index<0) index=0;
    
    LyfTableViewCell* cell=m_pDataSource->tableCellAtIndex(this, idx);
    if (!cell) return NULL;
    cell->setDelegate(this);
    m_backNode->addChild(cell);
    
    vector<LyfTableViewCell*>::iterator theIterator=m_TableViewCells.begin()+index;
    m_TableViewCells.insert(theIterator, cell);
    
    while (m_TableViewCells.size()>m_maxNumberOfPages)
    {
        LyfTableViewCell* rCell=m_TableViewCells.back();
        m_TableViewCells.pop_back();
        rCell->removeFromParent();
    }
    
    this->updateTableViewCellPos();
    this->updateIndicator();
    this->updatePullShow();
    this->updatePitchOn();
    
//    this->setPosition(getHomeFromCellPos());
    
    return cell;
}

LyfTableViewCell* LyfTableView::insertFirstTableCell(){
    
    return this->insertTableCellAtIndex(0);
}

LyfTableViewCell* LyfTableView::insertLastTableCell(){
    
    return this->insertTableCellAtIndex(m_TableViewCells.size());
}

void LyfTableView::removeTableCellAtIndex(unsigned int idx, bool actions){
    
    if (m_TableViewCells.empty()) return;
    
    int index=idx-m_page*m_maxNumberOfPages;
    
    if (index>=m_maxNumberOfPages) return;
    
    if (index<0) index=0;
    
    LyfTableViewCell* cell=m_TableViewCells.at(index);
    if (actions)
    {
        cell->removeThisAndActions();
    }
    else
    {
        cell->removeThis();
    }
}

void LyfTableView::removeFirstTableCell(bool actions){

    this->removeTableCellAtIndex(0, actions);
}

void LyfTableView::removeLastTableCell(bool actions){

    this->removeTableCellAtIndex(m_TableViewCells.size()-1, actions);
}

void LyfTableView::replaceTableCellAtIndex(unsigned int idx){

    int index=idx-m_page*m_maxNumberOfPages;
    
    if (index>=m_maxNumberOfPages) return;
    
    if (index<0) index=0;
    
    LyfTableViewCell* rCell=m_TableViewCells.at(index);
    vector<LyfTableViewCell*>::iterator oldIterator=m_TableViewCells.begin()+index;
    m_TableViewCells.erase(oldIterator);
    rCell->removeFromParent();
    
    LyfTableViewCell* cell=m_pDataSource->tableCellAtIndex(this, idx);
    cell->setDelegate(this);
    m_backNode->addChild(cell);
    
    vector<LyfTableViewCell*>::iterator newIterator=m_TableViewCells.begin()+index;
    m_TableViewCells.insert(newIterator, cell);
    
    
    this->updateTableViewCellPos();
    this->updateIndicator();
    this->updatePullShow();
    this->updatePitchOn();
}

void LyfTableView::exchangeTableCellAtIndex(unsigned int idx1, unsigned int idx2){
    
    int index1=idx1-m_page*m_maxNumberOfPages;
    
    int index2=idx2-m_page*m_maxNumberOfPages;

    bool infraMetasIndex1=false;
    bool infraMetasIndex2=false;
    
    if (index1>=0 && index1<m_TableViewCells.size())
    {
        infraMetasIndex1=true;
    }
    if (index2>=0 && index2<m_TableViewCells.size())
    {
        infraMetasIndex2=true;
    }
    
    if (infraMetasIndex1 && infraMetasIndex2)
    {
        swap(m_TableViewCells[index1], m_TableViewCells[index2]);
    }
    else if (infraMetasIndex1 && !infraMetasIndex2)
    {
        this->replaceTableCellAtIndex(idx1);
    }
    else if (!infraMetasIndex1 && infraMetasIndex2)
    {
        this->replaceTableCellAtIndex(idx2);
    }
    else
    {
        return;
    }
    
    this->updateTableViewCellPos();
    this->updateIndicator();
    this->updatePullShow();
    this->updatePitchOn();
}

CCPoint LyfTableView::getHomeFromCellPos()
{
    
    return CCPoint( 0, this->getContentSize().height);
}

CCPoint LyfTableView::getFirmFromCellPos()
{
    
    if (m_sizeHeight < this->getContentSize().height)
        return CCPoint( 0, this->getContentSize().height);

    return CCPoint( 0, m_sizeHeight);
}

CCActionInterval* LyfTableView::cellAction(CCPoint point, float time)
{
    
    CCMoveTo* moveto=CCMoveTo::create(time, point);
    CCEaseSineInOut* easeBack=CCEaseSineInOut::create(moveto);
    return easeBack;
}

void LyfTableView::update(float delta)
{
    if (!m_indicator) return;

    float size = getHomeFromCellPos().y;
    float contentSize = getFirmFromCellPos().y-getHomeFromCellPos().y;
    float lenght = m_backNode->getPositionY()-getHomeFromCellPos().y;
    m_indicator->setIndicator(size,contentSize,lenght);
}

void LyfTableView::setTableViewDataSource(LyfTableViewDataSource* node)
{
    
    m_pDataSource=node;
}

void LyfTableView::setTableViewDelegate(LyfTableViewDelegate* node)
{
    
    m_pTableViewDelegate=node;
}

void LyfTableView::setSelectedCount(int var)
{
    
    if (var<0) var=0;

    if (m_selectedCount==var) return;

    m_selectedCount=var;
    
    m_SelectedIndexPaths.clear();
    
    for (int i=0; i<m_TableViewCells.size(); i++)
    {
        LyfTableViewCell* cell=m_TableViewCells.at(i);
        cell->setPitchOn(false);
    }
}

int LyfTableView::getSelectedCount()
{
    
    return m_selectedCount;
}

int LyfTableView::getSelectedCountInCurrent()
{
    
    return m_SelectedIndexPaths.size();
}

void LyfTableView::setTouchPitchOn(LyfTableViewCell *cell)
{
    if (m_selectedCount == 0) return;

    if (!m_pTableViewDelegate) return;
    
    if (m_selectedCount == 1)
    {
        if (!m_SelectedIndexPaths.empty())
        {
            LyfIndexPath indexPath=*m_SelectedIndexPaths.begin();
            
            if (indexPath.isEqual(cell->getSign())) return;
            
            LyfTableViewCell* otherCell;
            
            if (indexPath.page==m_page)
            {
                otherCell=m_TableViewCells.at(indexPath.row);
                otherCell->setPitchOn(false);
            }
            else
            {
                otherCell=LyfTableViewCell::createTmp();
                otherCell->setSign(indexPath);
            }
            
            m_pTableViewDelegate->tableCellNoTouched(this, otherCell, otherCell->getSign().getIndex());
            m_SelectedIndexPaths.clear();
        }
    }
    
    else if (m_selectedCount>1)
        
    {
        if (cell->getPitchOn())
        {
            cell->setPitchOn(false);
            m_pTableViewDelegate->tableCellNoTouched(this, cell, cell->getSign().getIndex());
            
            for (int i=0; i<m_SelectedIndexPaths.size(); i++)
            {
                LyfIndexPath otherPath=m_SelectedIndexPaths.at(i);
                if (cell->getSign().isEqual(otherPath))
                {
                    m_SelectedIndexPaths.erase(m_SelectedIndexPaths.begin()+i);
                }
            }
            return;
        }
        
        if (m_SelectedIndexPaths.size()>=m_selectedCount) return;

    }
    
    cell->setPitchOn(true);
    m_SelectedIndexPaths.push_back(cell->getSign());
    
    m_pTableViewDelegate->tableCellTouched(this, cell, cell->getSign().getIndex());

}

void LyfTableView::setNoPitchOnOfArray(LyfTableViewCell *cell)
{
    
    LyfIndexPath indexPath=cell->getSign();
    for (int i=0; i<m_SelectedIndexPaths.size(); i++)
    {
        LyfIndexPath otherPath=m_SelectedIndexPaths.at(i);
        if (indexPath.isEqual(otherPath))
        {
            m_SelectedIndexPaths.erase(m_SelectedIndexPaths.begin()+i);
        }
    }
    
    cell->setPitchOn(false);
    
    if (!m_pTableViewDelegate) return;
    
    m_pTableViewDelegate->tableCellNoTouched(this, cell, cell->getSign().getIndex());
}

void LyfTableView::setTouchImageView(LyfTableViewCell* cell)
{
    
    if (!m_pTableViewDelegate) return;
    
    m_pTableViewDelegate->tableCellImageViewTouchUpInSide(this, cell, cell->getSign().getIndex());
}

void LyfTableView::removeTableCell(LyfTableViewCell *cell)
{
    
    if (!m_pTableViewDelegate) return;
    
    bool isRemoveTableCell=m_pTableViewDelegate->removeTableCell(this, cell, cell->getSign().getIndex());
    
    if (!isRemoveTableCell)
    {
        cell->setOpacity(255);
        cell->setPositionX(cell->getContentSize().width/2);
        return;
    }
    
    this->closeEditing();
    
    int lastIdx=m_TableViewCells.back()->getSign().getIndex();
    vector<LyfTableViewCell*>::iterator theIterator=m_TableViewCells.begin()+cell->getSign().row;
    m_TableViewCells.erase(theIterator);
    cell->removeFromParent();

    if (this->getNumberOfRows()-lastIdx>0)
    {
        LyfTableViewCell* rCell=m_pDataSource->tableCellAtIndex(this, lastIdx);
        rCell->setDelegate(this);
        m_backNode->addChild(rCell);
        
        m_TableViewCells.push_back(rCell);
    }

    this->updateTableViewCellPos();
    this->updateIndicator();
    this->updatePullShow();
    this->updatePitchOn();
    
    if (m_TableViewCells.empty() && m_page>0)
    {
        m_page--;
        this->readData();
    } 
    this->scheduleOnce(schedule_selector(LyfTableView::cellGoHome), 0.033f);

}

void LyfTableView::joinEditing(LyfTableViewCell *cell)
{
    
    if (!m_nEdit) return;
    
    if (m_CellEditing>-1) return;

    m_CellEditing=cell->getSign().row;
    m_SelectedIndexPaths.clear();
    cell->showDelBtn();
    this->unschedule(schedule_selector(LyfTableView::inertia));
}

bool LyfTableView::isEditing(){

    return (m_nEdit && m_CellEditing>-1);
}

bool LyfTableView::isCanEdit(){
    
    return m_nEdit;
}

void LyfTableView::closeEditing(){
    
    m_CellEditing=-1;
}

bool LyfTableView::isCanSelected()
{
    float topCutOff=this->getHomeFromCellPos().y;
    float bottomCutOff=this->getFirmFromCellPos().y;
    
    return (m_backNode->getPositionY()>=topCutOff && m_backNode->getPositionY()<=bottomCutOff);
}

void LyfTableView::setOpacity(GLubyte opacity)
{
    
    //CCLayerColor::setOpacity(opacity);
    
    for (int i=0; i<m_TableViewCells.size(); i++)
    {
        LyfTableViewCell* cell=m_TableViewCells.at(i);
        cell->setOpacity(opacity);
    }
    
    if (m_indicator)
    {
        m_indicator->setOpacity(opacity);
    }

}

CCNode* LyfTableView::getNode()
{
    
    return m_backNode;
}

LyfTableViewCell* LyfTableView::getTableViewCellAtIndex(int idx)
{
    
    LyfIndexPath indexPath=LyfIndexPath(idx, m_maxNumberOfPages);
    
    if (indexPath.page!=m_page) return NULL;
    
    LyfTableViewCell* cell=m_TableViewCells.at(indexPath.row);
    
    return cell;
}