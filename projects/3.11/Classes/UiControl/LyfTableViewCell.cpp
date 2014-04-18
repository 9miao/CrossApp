//  出自九秒社区 http://www.9miao.com
//
//  LyfTableViewCell.cpp
//  LTabelView
//
//  Created by LI YUANFENG on 13-3-11.
//
//

#include "LyfTableViewCell.h"
#include "SimpleAudioEngine.h"

#define DELBTNICON "ui/indicator.png"
#define DELBTNPOINT CCPoint(this->getContentSize().width-m_delBtn->getContentSize().height/2, this->getContentSize().height/2)

LyfTableViewCell::LyfTableViewCell()
:m_sign(LyfIndexPathZero)
,m_textTag("")
,m_imageView(NULL)
,m_textLabel(NULL)
,m_detailTextLabel(NULL)
,m_touchBeginUIPoint(CCPointZero)
,m_bIsTouchEnabled(true)
,m_pitchOn(false)
,m_touchBegin(false)
,m_specially(false)
,m_delBtn(NULL)
,m_delegate(NULL)
,m_delRect(CCRectZero)
,m_touchModel(tCell)
,m_nTouchPriority(0)
,m_IsRehisterTouchDispatcher(false)
{
}

LyfTableViewCell::~LyfTableViewCell()
{

}

void LyfTableViewCell::onExit()
{
    
    if (m_delegate)
    {
        m_delegate->removeDelegate(this);
        m_delegate=NULL;
    }
    
    this->unRegisterWithTouchDispatcher();
    CCSprite::onExit();
}

void LyfTableViewCell::onEnter()
{
    
    CCSprite::onEnter();
    this->registerWithTouchDispatcher();
}

void LyfTableViewCellDelegate::removeDelegate(LyfTableViewCell *cell)
{
    
    cell->setDelegate(NULL);
}

LyfTableViewCell* LyfTableViewCell::create(const char *fileName)
{
    
    LyfTableViewCell* tableViewCell=new LyfTableViewCell();
    if (tableViewCell && tableViewCell->initWithFile(fileName))
    {
        tableViewCell->autorelease();
        return tableViewCell;
    }
    
    CC_SAFE_DELETE(tableViewCell);
    
    return NULL;
}

LyfTableViewCell* LyfTableViewCell::createWithSpriteFrameName(const char *fileName)
{
    
    LyfTableViewCell* tableViewCell=new LyfTableViewCell();
    if (tableViewCell && tableViewCell->initWithSpriteFrameName(fileName))
    {
        tableViewCell->autorelease();
        return tableViewCell;
    }
    
    CC_SAFE_DELETE(tableViewCell);
    
    return NULL;
}

LyfTableViewCell* LyfTableViewCell::createTmp()
{
    
    LyfTableViewCell* tableViewCell=new LyfTableViewCell();
    if (tableViewCell && tableViewCell->init())
    {
        tableViewCell->autorelease();
        return tableViewCell;
    }
    
    CC_SAFE_DELETE(tableViewCell);
    
    return NULL;
}

LyfTableViewCell* LyfTableViewCell::createWithTexture(cocos2d::CCTexture2D *pTexture)
{
    
    LyfTableViewCell *tableViewCell = new LyfTableViewCell();
    if (tableViewCell && tableViewCell->initWithTexture(pTexture))
    {
        tableViewCell->autorelease();
        return tableViewCell;
    }
    
    CC_SAFE_DELETE(tableViewCell);
    
    return NULL;
}

void LyfTableViewCell::setImageView(CCSprite* sprite)
{
    
    if (sprite==NULL) return;

    if (m_imageView!=NULL)
    {
        m_imageView->removeFromParentAndCleanup(true);
    }
    
    m_imageView=sprite;
    
    if (m_imageView->getContentSize().height>this->getContentSize().height)
    {
        m_imageView->setScale(this->getContentSize().height/m_imageView->getContentSize().height);
    }
    
    m_imageView->setPosition(CCPoint(m_imageView->getContentSize().width/2*m_imageView->getScale(),
                                    this->getContentSize().height/2));
    this->addChild(m_imageView);
}

CCSprite* LyfTableViewCell::getImageView(){

    return m_imageView;
}

void LyfTableViewCell::setTextLabel(CCLabelTTF* textLabel)
{
    
    if (m_textLabel)
    {
        m_textLabel->removeFromParentAndCleanup(true);
    }
    
    m_textLabel=textLabel;
    
    this->addChild(m_textLabel);
}

CCLabelTTF* LyfTableViewCell::getTextLabel()
{
    
    return m_textLabel;
}

void LyfTableViewCell::setDetailTextLabel(CCLabelTTF* detailTextLabel)
{
    
    if (m_detailTextLabel)
    {
        m_detailTextLabel->removeFromParentAndCleanup(true);
    }
    
    m_detailTextLabel=detailTextLabel;
    
    this->addChild(m_detailTextLabel);
}

CCLabelTTF* LyfTableViewCell::getDetailTextLabel()
{
    
    return m_detailTextLabel;
}

void LyfTableViewCell::setTouchModel(tTouchModel tEnum)
{
    
    m_touchModel=tEnum;
}

CCRect LyfTableViewCell::getDelRect()
{
    
    if (!m_delBtn)
        return CCRectZero;

    CCRect rect=CCRectZero;
    float x=m_delBtn->getPositionX()-m_delRect.size.width;
    float y=m_delBtn->getPositionY()-m_delRect.size.height/2;
    CCPoint point=CCPoint(x, y);
    CCSize size=CCSize(m_delRect.size.width, m_delRect.size.height);
    rect.origin=point;
    rect.size=size;
    return rect;
    
}

CCRect LyfTableViewCell::getBtnRect(CCSprite* sprite)
{
    
    if (!sprite) return CCRectZero;
    
    return CCRect( 1, 1, sprite->getContentSize().width-2, sprite->getContentSize().height-2);
}

CCRect LyfTableViewCell::getRect(){
    
    return CCRect( 1, 1, getContentSize().width-2, getContentSize().height-2);
}

bool LyfTableViewCell::isOverstep(CCPoint point)
{
    
    if (!this->getParent()->getParent()) return false;

    if (!this->getParent()) return false;

    if ((this->getPositionY()+this->getParent()->getPositionY()-this->getContentSize().height/2+point.y)<0)
    {
        return true;
    }
    
    if ((this->getPositionY()+this->getParent()->getPositionY()
         -this->getContentSize().height/2+point.y)
        > (this->getParent()->getParent()->getContentSize().height))
    {
        return true;
    }
    
    return false;
}

bool LyfTableViewCell::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    m_touchBeginUIPoint=pTouch->getLocationInView();
    
    CCPoint uipoint=pTouch->getLocationInView();
    CCPoint point=CCDirector::sharedDirector()->convertToGL(uipoint);
    point=this->convertToNodeSpace(point);
    
    if (isOverstep(point)) return false;

    if (!this->isVisible()) return false;

    if (m_delegate && !m_delegate->isCanSelected()) return false;
    
    if (m_delegate && m_delegate->isCanEdit())
    {
        if (getDelRect().containsPoint(point))
        {
            
            m_touchModel=tDel;
            return true;
        }
        else {
            if (m_delBtn)
            {
                this->removeDelBtnParent();
                return false;
            }
        }
    }
    
    if (!getRect().containsPoint(point)) return false;
    
    if (getBtnRect(m_imageView).containsPoint(point))
    {
        m_touchModel=tLead;
        return true;
    }
    
    if (!m_bIsTouchEnabled) return false;

    m_touchModel = tCell;
    
    do
    {
        CC_BREAK_IF(m_touchBegin == true);
        
        m_touchBegin = true;
        
        CCDelayTime* delayTime = CCDelayTime::create(0.1f);
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(LyfTableViewCell::setTouchedEffect));
        CCSequence* actions = CCSequence::create(delayTime, func, NULL);
        this->runAction(actions);
        actions->setTag(0xffff);
    }
    while (0);
    
    return true;
}

void LyfTableViewCell::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    float lenght=ccpDistance(pTouch->getLocationInView(), m_touchBeginUIPoint);
    if (lenght<TOUCH_PROTECTION_ACCURACY) return;
    
    CCPoint uipoint=pTouch->getLocationInView();
    CCPoint point=CCDirector::sharedDirector()->convertToGL(uipoint);
    point=this->convertToNodeSpace(point); 
    
    CCPoint uipointpre=pTouch->getPreviousLocationInView();
    CCPoint pointpre=CCDirector::sharedDirector()->convertToGL(uipointpre);
    pointpre=this->convertToNodeSpace(pointpre);
    
    if (m_delegate && m_delegate->isEditing()) return;

    if (m_touchModel==tCell && fabsf(point.y-pointpre.y)/ccpDistance(point, pointpre)<0.4f)
    {
        m_delegate->joinEditing(this);
    }

    m_touchModel = tNone;
    
    do
    {
        CC_BREAK_IF(m_touchBegin == false);
        
        m_touchBegin = false;
        
        if (this->getActionByTag(0xffff) == NULL)
        {
            CCDelayTime* delayTime = CCDelayTime::create(0.1f);
            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(LyfTableViewCell::setNoTouchedEffect));
            CCSequence* actions = CCSequence::create(delayTime, func, NULL);
            this->runAction(actions);
            actions->setTag(0xffff);
        }
        else
        {
            this->stopActionByTag(0xffff);
        }
    }
    while (0);
}

void LyfTableViewCell::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    m_touchBeginUIPoint=CCPointZero;
    
    CCPoint uipoint=pTouch->getLocationInView();
    CCPoint point=CCDirector::sharedDirector()->convertToGL(uipoint);
    point=this->convertToNodeSpace(point);
    
    do
    {
        CC_BREAK_IF(m_touchBegin == false);
        
        m_touchBegin = false;
        
        CCDelayTime* delayTime = CCDelayTime::create(0.1f);
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(LyfTableViewCell::setNoTouchedEffect));
        CCSequence* actions = CCSequence::create(delayTime, func, NULL);
        this->runAction(actions);
        actions->setTag(0xffff);
    }
    while (0);
    
    if (getRect().containsPoint(point) && m_touchModel==tCell)
    {
        m_delegate->setTouchPitchOn(this);
    }
    
    if (getBtnRect(m_imageView).containsPoint(point) && m_touchModel==tLead)
    {
        m_delegate->setTouchImageView(this);
    }
    
    if (getDelRect().containsPoint(point) && m_touchModel==tDel)
    {
        m_delBtn->setVisible(false);
        this->removeThisAndActions();
    }
}

void LyfTableViewCell::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    this->ccTouchEnded(pTouch, pEvent);
}

void LyfTableViewCell::registerWithTouchDispatcher()
{
    
    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    touchDispatcher->addTargetedDelegate(this, m_nTouchPriority, false);
}

void LyfTableViewCell::unRegisterWithTouchDispatcher()
{
    
    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    touchDispatcher->removeDelegate(this);
    m_IsRehisterTouchDispatcher=false;
}

void LyfTableViewCell::setPriority(int nPriority)
{
    
    m_nTouchPriority=nPriority;
    
    if (m_IsRehisterTouchDispatcher)
    {
        CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        touchDispatcher->setPriority(m_nTouchPriority, this);
    }
}

void LyfTableViewCell::setDelegate(LyfTableViewCellDelegate *tableView)
{
    
    m_delegate=tableView;
}

void LyfTableViewCell::setTouchEnabled(bool enabled)
{
    
    m_bIsTouchEnabled=enabled;
    
    if (m_bIsTouchEnabled)
    {
        this->setClickEffect();
    }
    else
    {
        this->setNoClickEffect();
    }
}

bool LyfTableViewCell::getTouchEnabled()
{
    return m_bIsTouchEnabled;
}


#pragma mark -
#pragma mark 选中特效模块

void LyfTableViewCell::setPitchOn(bool var)
{
    do
    {
        CC_BREAK_IF(m_pitchOn == var);
        m_pitchOn = var;
        
        if (m_pitchOn)
        {
            this->setPitchOnEffect();
        }
        else
        {
            this->setNoPitchOnEffect();
        }
        
    }
    while (0);
}

bool LyfTableViewCell::getPitchOn()
{
    return m_pitchOn;
}

#pragma mark -
#pragma mark 删除按钮模块

void LyfTableViewCell::showDelBtn()
{
    
    if (m_delBtn) return;
    
    m_delBtn=CCSprite::create(DELBTNICON);
    m_delBtn->setAnchorPoint(CCPoint(1.0f, 0.5f));
    m_delBtn->setPosition(DELBTNPOINT);
    this->addChild(m_delBtn);
    
    m_delRect=m_delBtn->getTextureRect();
    m_delBtn->setTextureRect(CCRect(m_delRect.origin.x, m_delRect.origin.y, 0, m_delRect.size.height));
    CCNode* node=CCNode::create();
    node->setPosition(CCPointZero);
    m_delBtn->addChild(node);
    
    CCMoveTo* moveTo=CCMoveTo::create(0.3f, CCPoint(m_delRect.size.width, 0));
    node->runAction(moveTo);
    this->schedule(schedule_selector(LyfTableViewCell::delShowAction));
}

void LyfTableViewCell::delShowAction()
{
    
    float width=((CCNode*)m_delBtn->getChildren()->objectAtIndex(0))->getPositionX();
    
    CCRect rect=CCRectZero;
    CCPoint point=CCPoint(m_delRect.size.width-width+m_delRect.origin.x, m_delRect.origin.y);
    CCSize size=CCSize(width, m_delRect.size.height);
    rect.origin=point;
    rect.size=size;
    m_delBtn->setTextureRect(rect);
    
    if (width==m_delRect.size.width)
    {
        m_delBtn->setContentSize(CCSize(m_delRect.size.width, m_delRect.size.height));
        this->unschedule(schedule_selector(LyfTableViewCell::delShowAction));
        return;
    }
}

void LyfTableViewCell::delHideAction()
{
    
    float width=((CCNode*)m_delBtn->getChildren()->objectAtIndex(0))->getPositionX();
    
    CCRect rect=CCRectZero;
    CCPoint point=CCPoint(m_delRect.size.width-width+m_delRect.origin.x,m_delRect.origin.y);
    CCSize size=CCSize(width,m_delRect.size.height);
    rect.origin=point;
    rect.size=size;
    m_delBtn->setTextureRect(rect);
    
    if (width==0)
    {
        m_delBtn->setContentSize(CCSizeZero);
        m_delBtn->removeFromParentAndCleanup(true);
        m_delBtn=NULL;
        m_delegate->closeEditing();
        this->unschedule(schedule_selector(LyfTableViewCell::delHideAction));
        return;
    }
}

void LyfTableViewCell::removeDelBtnParent()
{

    if (!m_delBtn) return;

    CCNode* node=((CCNode*)m_delBtn->getChildren()->objectAtIndex(0));
    node->stopAllActions();
    CCMoveTo* moveTo=CCMoveTo::create(0.3f*node->getPositionX()/m_delRect.size.width, CCPointZero);
    node->runAction(moveTo);
    this->unschedule(schedule_selector(LyfTableViewCell::delShowAction));
    this->schedule(schedule_selector(LyfTableViewCell::delHideAction));
}

void LyfTableViewCell::removeThis(){
    
    m_delegate->removeTableCell(this);
}

void LyfTableViewCell::removeThisAndActions()
{
    
    CCMoveBy* moveby=CCMoveBy::create(0.15f, CCPoint(-this->getContentSize().width, 0));
    this->runAction(moveby);
    CCFadeOut* fadeOut=CCFadeOut::create(0.15f);
    this->runAction(fadeOut);

    this->scheduleOnce(schedule_selector(LyfTableViewCell::removeThis), 0.25f);
}

void LyfTableViewCell::setOpacity(GLubyte opacity)
{
    
    CCSprite::setOpacity(opacity);
    
    if (m_imageView)
    {
        m_imageView->setOpacity(opacity);
    }
    
    if (m_textLabel)
    {
        m_textLabel->setOpacity(opacity);
    }
    
    if (m_detailTextLabel)
    {
        m_detailTextLabel->setOpacity(opacity);
    }
}

bool LyfTableViewCell::isTextTagEqual(const char *text)
{
    
    return (strcmp(m_textTag.c_str(),text)==0);
}

bool LyfTableViewCell::isInTheRangeOfTableView(CCPoint point)
{
    
    CCRect rect=CCRectZero;
    float width=this->getContentSize().width;
    float height=this->getParent()->getParent()->getContentSize().height;
    rect.size=CCSize(width, height);
    
    float point_X=point.x;
    float point_Y=point.y+this->getPositionY()+this->getParent()->getPositionY()-this->getContentSize().height/2;
    
    return rect.containsPoint(CCPoint(point_X, point_Y));
}