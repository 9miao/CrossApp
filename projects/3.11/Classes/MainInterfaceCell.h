//
//  MainInterfaceCell.h
//  project
//
//  Created by 栗元峰 on 14-3-11.
//
//

#ifndef __project__MainInterfaceCell__
#define __project__MainInterfaceCell__

#include <iostream>
#include "LyfTableViewCell.h"
#include "MainInterfaceData.h"

#include "cocos-ext.h"

USING_NS_CC_EXT;
class MainInterfaceCell: public LyfTableViewCell
{
    MainInterfaceData* m_pData;
    
    CCScale9Sprite* m_pBg;
    
public:

    MainInterfaceCell(MainInterfaceData* data);
    
    static MainInterfaceCell* create(MainInterfaceData* data);
    
    bool init();
    
protected:
    
    virtual void setTouchedEffect();
    
    virtual void setNoTouchedEffect();
};


#endif /* defined(__project__MainInterfaceCell__) */
