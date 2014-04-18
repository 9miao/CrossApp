//
//  LHyperLink.cpp
//  project
//
//  Created by 栗元峰 on 14-3-24.
//
//

#include "LUrlHyperLink.h"
#include "OpenBrowser.h"
LUrlHyperLink* LUrlHyperLink::create(const char* url, float width)
{
    CCLabelTTF* ttf = CCLabelTTF::create(url, "fonts/arial.ttf", 20,
                                         CCSize(width, 0), kCCTextAlignmentLeft);
    ttf->setColor(ccBLUE);
    
    CCLabelTTF* ttfHigh = CCLabelTTF::create(url, "fonts/arial.ttf", 20,
                                         CCSize(width, 0), kCCTextAlignmentLeft);
    ttfHigh->setColor(ccBLACK);
    
    LUrlHyperLink* btn=new LUrlHyperLink();
    
    if (btn && btn->initWithBackGround(ttf))
    {
        btn->setBackGround(LMenuStateHighlighted, ttfHigh);
        btn->m_sUrl = url;
        btn->addTarget(btn, LMenu_selector(LUrlHyperLink::OpenTheWebpage), TouchUpInSide);
        btn->setTouchDispatcher(false);
        btn->setPreferredSize(ccpAdd(btn->getContentSize(), CCSize(0, 10)));
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

void LUrlHyperLink::OpenTheWebpage(LMenu* menu, CCPoint point)
{
    OpenBrowser::openUrl(CCString::create(m_sUrl.c_str()));
}

bool LUrlHyperLink::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nStart = pTouch->getLocation();
    
    return LMenu::ccTouchBegan(pTouch, pEvent);
}

void LUrlHyperLink::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    float lenght=ccpDistance(pTouch->getLocation(), m_nStart);
    
    CCPoint uipoint=pTouch->getLocationInView();
    CCPoint point=CCDirector::sharedDirector()->convertToGL(uipoint);
    point=this->convertToNodeSpace(point);
    
    if (!this->getTouchClick()) return;
    
    if (lenght<6)
    {
        this->setTouchState(LMenuStateHighlighted);
        this->setTouchMoved(point);
    }
    else
    {
        this->setTouchState(LMenuStateNormal);
        this->setTouchMovedOutSide(point);
        this->setTouchClick(false);
    }
    
}