//
//  AutoView.hpp
//  CAUIEDITOR
//
//  Created by renhongguang on 15/8/12.
//
//

#ifndef AutoView_cpp
#define AutoView_cpp

#include "CrossApp.h"

typedef enum
{
    ViewTypeCAView = 0,
    ViewTypeCAImageView,
    ViewTypeCAScale9ImageView,
    ViewTypeCALabel,
    ViewTypeCAButton,
    ViewTypeCASegmentControl
}ViewType;

enum AutoPoint
{
    Left_Up = 0,
    Left_Center,
    Left_Down,
    Center_Down,
    Right_Down,
    Right_Center,
    Right_Up,
    Center_Up,
    None
};

class AutoViewDelegate
{
public:
    virtual ~AutoViewDelegate(){};
    
    virtual void AutoViewChangeFrameStart(){};
    virtual void AutoViewChangeFrameEnd(){};
    virtual void AutoViewMoveToPosition(CCPoint& p){};
};

class AutoView : public CAButton
{
public:
    AutoView(const ViewType& type, bool bOriginal);
    virtual ~AutoView();
    
    virtual void onExitTransitionDidStart();
    virtual void onEnterTransitionDidFinish();
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    

    virtual bool resignFirstResponder();
    
    virtual bool becomeFirstResponder();
    
    CC_SYNTHESIZE(bool, m_bEditing, Editing);
    CC_SYNTHESIZE(AutoViewDelegate*, m_pAutoViewDelegate, AutoViewDelegate);
    
    CAView* getView(){ return m_pView; }
    ViewType getViewType(){return m_viewtype;}
    std::map<AutoPoint, CCPoint> getPoints();
    AutoPoint getAutoPoint(const CCPoint& point);
protected:
    
    void draw();
    
protected:
    
    CAView* m_pView;
    AutoPoint m_eAutoPoint;
    CCPoint touchBegin_point;
    ViewType m_viewtype;
};

#endif /* AutoView_cpp */
