//
//  CCScrollViewX.cpp
//  scrollView
//
//  Created by mac on 13-10-23.
//
//

#include "CCScrollViewX.h"
#include "LUrlSprite.h"

CCScrollViewX::CCScrollViewX()
:_number(0)
,_isNext(0)
,isActions(false)
,m_touchBeginPoint(CCPointZero)
,_isMoved(false)
{
    
}

CCScrollViewX::~CCScrollViewX()
{

}

CCScrollViewX* CCScrollViewX::create(CCSize size, CCNode* container/* = NULL*/)
{
    CCScrollViewX* pRet = new CCScrollViewX();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCScrollViewX* CCScrollViewX::create()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCLayerColor* layer=CCLayerColor::createWithFrame(CCRect(0, 0, size.width*3, 390));
    layer->setOpacity(0);
    
    CCScrollViewX* pRet = CCScrollViewX::create(CCSize(size.width, 390), layer);
    pRet->setBounceable(true);
    pRet->setDirection(kCCScrollViewDirectionHorizontal);
    pRet->setDelegate(pRet);
    pRet->initWithBg();

    return pRet;
}

void CCScrollViewX::initWithBg()
{
    std::vector<std::string> vec;
    vec.push_back("http://a2.att.hudong.com/33/03/01300000260386125024033626341.jpg");
    vec.push_back("http://a2.att.hudong.com/33/03/01300000260386125024033626341.jpg");
    vec.push_back("http://img7.9158.com/200708/23/23/32/2007082308159.jpg");
    vec.push_back("http://img7.9158.com/200708/23/23/32/2007082308159.jpg");
    vec.push_back("http://a3.att.hudong.com/80/95/300000577412133467952994064.jpg");
    vec.push_back("http://a3.att.hudong.com/80/95/300000577412133467952994064.jpg");

    for (unsigned int i=0; i<vec.size(); i++)
    {
        if (i%2 == 0)
        {
            m_array.push_back(vec.at(i));
        }
        else
        {
            m_Urlarray.push_back(vec.at(i));
        }
        
    }

    for (int i=0; i<3; i++)
    {
        CCSprite* image = CCSprite::create();
        image->setPosition(CCPoint(this->getViewSize().width * (i+0.5), this->getViewSize().height/2));
        this->getContainer()->addChild(image);
        
        LUrlSprite* sprite = LUrlSprite::create(m_array.at(i).c_str());
        sprite->setPosition(CCPointZero);
        image->addChild(sprite, 0, 0xcccc);
        
        m_nImage[i]=image;
    }
    
    this->getContainer()->setPosition(CCPoint(-this->getViewSize().width, 0));
    this->startPlay();
    this->initWithInstructsContainer();
    this->actionsFinish();
}

void CCScrollViewX::initWithInstructsContainer()
{
    CCSize size=CCSize(25, 25);
    
    m_nInstructsContainer=CCSprite::create();
    m_nInstructsContainer->setAnchorPoint(CCPointZero);
    m_nInstructsContainer->setPosition(CCPoint(10, 10));
    this->addChild(m_nInstructsContainer, 10);
    
    m_nInstruct=CCSprite::create("WhiteDots.png");
    m_nInstructsContainer->addChild(m_nInstruct);
    
    for (unsigned int i=0; i<m_array.size(); i++)
    {
        CCSprite* instructs=CCSprite::create("GreyDots.png");
        m_nInstructsContainer->addChild(instructs);
        m_nInstructs.push_back(instructs);
    }
    
    m_nInstructsContainer->setContentSize(CCSize(m_nInstructs.size()*size.width, size.height));
    
    for (int i=0; i<m_nInstructs.size(); i++)
    {
        m_nInstructs.at(i)->setPosition(CCPoint((0.5+i)*size.width, size.height/2));
    }
    
    m_nInstruct->setPosition(m_nInstructs.front()->getPosition());
}

void CCScrollViewX::startPlay()
{
    _isNext = 1;
    this->schedule(schedule_selector(CCScrollViewX::scrollViewActions), 3.0f);
}

void CCScrollViewX::addChild(CCNode * child, int zOrder, int tag)
{
    child->ignoreAnchorPointForPosition(false);
    child->setAnchorPoint(ccp(0.0f, 0.0f));
    
    if (m_pContainer != child && m_nInstructsContainer != child)
    {
        m_pContainer->addChild(child, zOrder, tag);
    }
    else
    {
        CCLayerColor::addChild(child, zOrder, tag);
    }
}

void CCScrollViewX::addChild(CCNode * child, int zOrder)
{
    this->addChild(child, zOrder, child->getTag());
}

void CCScrollViewX::addChild(CCNode * child)
{
    this->addChild(child, child->getZOrder(), child->getTag());
}

void CCScrollViewX::registerWithTouchDispatcher(void)
{
    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    touchDispatcher->addTargetedDelegate(this, 0, true);
}

bool CCScrollViewX::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pTouches->count() > 2 ||
        m_bTouchMoved           ||
        !this->getViewRect().containsPoint(m_pContainer->convertToWorldSpace(m_pContainer->convertTouchToNodeSpace(pTouch))))
    {
        return false;
    }
    
    if (isActions)
    {
        return false;
    }
    
    m_touchBeginPoint=pTouch->getLocation();
    
    _isNext = 0;
    this->unschedule(schedule_selector(CCScrollViewX::scrollViewActions));
    return CCScrollView::ccTouchBegan(pTouch, pEvent);
}


void CCScrollViewX::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCScrollView::ccTouchMoved(pTouch, pEvent);
    
    CCPoint point=pTouch->getLocation();
    float lenght=ccpDistance(point, m_touchBeginPoint);
    if (lenght<6) return;
    
    _isMoved = true;
    
    if (pTouch->getPreviousLocation().x-pTouch->getLocation().x > 5)
    {
        _isNext=1;
        return;
    }
    if (pTouch->getLocation().x-pTouch->getPreviousLocation().x > 5)
    {
        _isNext=-1;
        return;
    }
    _isNext=0;
}

void CCScrollViewX::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    if (m_pTouches->containsObject(pTouch))
    {
        m_pTouches->removeObject(pTouch);
    }
    
    if (m_pTouches->count() == 0)
    {
        m_bDragging = false;
        m_bTouchMoved = false;
    }
    
    if (_isNext==0)
    {
        if (this->getContainer()->getPositionX()>-this->getViewSize().width/2)
        {
            _isNext=-1;
        }
        else if (this->getContainer()->getPositionX()<-this->getViewSize().width/2*3)
        {
            _isNext=1;
        }
    }
    
    isActions=true;
    
    this->scrollViewActions();
    
    this->scheduleOnce(schedule_selector(CCScrollViewX::startPlay), 2.0f);
    
    if (!_isMoved)
    {
        this->touchUpInSide();
    }
    _isMoved=false;
}

void CCScrollViewX::scrollViewActions()
{

    if (_isNext > 0)
    {
        _number++;
        
        if (_number >= m_array.size())
        {
            _number = 0;
        }
    }
    if (_isNext < 0)
    {
        _number--;
        
        if (_number < 0)
        {
            _number = m_array.size() - 1;
        }
    }

    CCMoveTo* moveto=CCMoveTo::create(0.3f, CCPoint(-this->getViewSize().width*(_isNext+1), 0));
    CCCallFunc* callFunc=CCCallFunc::create(this, callfunc_selector(CCScrollViewX::actionsFinish));
    CCSequence* sequence=CCSequence::create(moveto, callFunc, NULL);
    m_pContainer->runAction(sequence);
}

void CCScrollViewX::actionsFinish()
{
    
    for (unsigned int i=0; i<3; i++)
    {
        CC_BREAK_IF(m_array.empty());
        unsigned int index = _number + i - 1;
        index = index + m_array.size();
        index = index % m_array.size();
        
        m_nImage[i]->removeChildByTag(0xcccc);
        
        LUrlSprite* sprite = LUrlSprite::create(m_array.at(index).c_str());
        sprite->setPosition(CCPointZero);
        m_nImage[i]->addChild(sprite, 0, 0xcccc);
    }
    
    m_nInstruct->setPosition(m_nInstructs.at(_number)->getPosition());
    
    m_pContainer->setPosition(CCPoint(-this->getViewSize().width, 0));

    isActions=false;
}

void CCScrollViewX::touchUpInSide()
{
    //////

}

void CCScrollViewX::scrollViewDidScroll(CCScrollView* view)
{
    

}

void CCScrollViewX::scrollViewDidZoom(CCScrollView* view)
{
    
}
