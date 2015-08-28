//
//  CanvasScenes.hpp
//  CAUIEDITOR
//
//  Created by renhongguang on 15/8/18.
//
//

#ifndef CanvasScenes_cpp
#define CanvasScenes_cpp

#include "CrossApp.h"

class CanvasScenes : public CAView
{
public:
    CanvasScenes();
    virtual ~CanvasScenes();
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void setDrawRect(bool);
protected:
    void draw2();
    void visit();
    CCPoint m_pointBegin;
    CCPoint m_pointEnd;
    bool isdraw;
    bool isTouch;

};

#endif /* CanvasScenes_cpp */
