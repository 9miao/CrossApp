//
//  LyfIndicator.h
//  LTabelView
//
//  Created by LI YUANFENG on 13-3-13.
//
//

#ifndef __LTabelView__LyfIndicator__
#define __LTabelView__LyfIndicator__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

#define BACKGROUNDIMAGE NULL
#define INDICATORIMAGE "indicator.png"
#define OFF_HEIGHT dvalue(2)

USING_NS_CC;
USING_NS_CC_EXT;
class LyfIndicator:public CCSprite{
    
public:
    
    LyfIndicator(CCLayerColor* layerColor);
    
    virtual ~LyfIndicator();
    
    static LyfIndicator* createWithAddChild(CCLayerColor* layerColor, float scale);
    
    void refresh();
    
    void setIndicator(float windowSize, float contentSize, float lenght);
    
    void setScale(float scale);
    
    void setOpacity(GLubyte opacity);
    
    CC_SYNTHESIZE(CCLayerColor*, parentNode, ParentNode);
    
    void setPointOff(CCPoint point);
    
private:
    
    CCPoint m_offPoint;
    
    float m_scale;
    
    CCScale9Sprite* m_bg;
    
    CCScale9Sprite* m_indicator;
    
    float m_indWidth;
    
    float m_indHeight;
    
    bool init();
    
    void initWithIndicator(float scale);
    
    void initWithBg();
    
    void onEnter();
};




#endif /* defined(__LTabelView__LyfIndicator__) */
