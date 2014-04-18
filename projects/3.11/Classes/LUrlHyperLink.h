//
//  LHyperLink.h
//  project
//
//  Created by 栗元峰 on 14-3-24.
//
//

#ifndef __project__LHyperLink__
#define __project__LHyperLink__

#include <iostream>
#include "LMenu.h"

class LUrlHyperLink
: public LMenu
{
    std::string m_sUrl;
    
    CCPoint m_nStart;
    
public:
    
    static LUrlHyperLink* create(const char* url, float width);
    
    void OpenTheWebpage(LMenu* menu, CCPoint point);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
};


#endif /* defined(__project__LHyperLink__) */
