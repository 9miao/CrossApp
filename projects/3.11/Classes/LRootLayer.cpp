//
//  LRootLayer.cpp
//  project
//
//  Created by 栗元峰 on 14-3-18.
//
//

#include "LRootLayer.h"
#include "LTableBarController.h"
bool LRootLayer::init()
{
    bool relust = CCLayerColor::init();
    CCLayerColor::setOpacity(0);
    if (relust)
    {
        this->setContentSize(CCSize(WINSIZE.width, WINSIZE.height - 160));
    }

    return relust;
}

void LRootLayer::setOpacity(GLubyte opacity){
    
    CCLayerColor::setOpacity(opacity);
    
    this->setOpacityOfChildren(opacity, this);
}

void LRootLayer::setOpacityOfChildren(GLubyte opacity, CCNode* node)
{
    for (int i=0; i<node->getChildrenCount(); i++)
    {
        CCNode* child = dynamic_cast<CCNode*>(node->getChildren()->objectAtIndex(i));

        if (CCNodeRGBA* _children = dynamic_cast<CCNodeRGBA*>(child))
        {
            _children->setOpacity(opacity);
        }
        
        this->setOpacityOfChildren(opacity, child);
    }
}