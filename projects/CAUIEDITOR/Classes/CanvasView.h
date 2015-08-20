//
//  CanvasView.hpp
//  CAUIEDITOR
//
//  Created by renhongguang on 15/8/14.
//
//

#ifndef CanvasView_cpp
#define CanvasView_cpp

#include "CrossApp.h"
#include "AutoView.h"

class CanvasView : public CAView
{
public:
    CanvasView();
    virtual ~CanvasView();
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
protected:
    void draw2();
    void visit();
    CCPoint m_pointBegin;
    CCPoint m_pointEnd;
    bool isdraw;
    AutoView*  m_TempAutoView;
    int m_TempAutoPoint;
    CCRect m_TempRect;
    bool selectMoreView;
    bool isCanMove;
    bool hadMove;
};
#endif /* CanvasView_cpp */
