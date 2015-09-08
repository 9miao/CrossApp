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
    ViewTypeCASegmentControl,
    ViewTypeCATabBar,
    viewTypeCAIndicator,
    viewTypeCAProgress,
    viewTypeCASlider,
    viewTypeCASwitch,
    viewTypeCAStepper,
    viewTypeCAWebView,
    viewTypeCAFlashView,
    viewTypeCAGifView,
    viewTypeCAPickerView,
    viewTypeCATextField,
    viewTypeCATextView,
    viewTypeCAPageView,
    viewTypeCAColectionView,
    viewTypeCATableView,
    viewTypeCAListView,
    viewTypeCAScrollView
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

enum ShowType{
    show_normal = 0,
    show_LR2
};

struct dataModel
{
    int tag;
    int parentid;
    int controlType;
    int fontsize;
    int type;
    
    float ix;
    float iy;
    float iw;
    float ih;
    float iz;
    
    string name;
    string imageNormal;
    string imageHighted;
    string imageDisabled;
    string imageSelected;
    string titleNormal;
    string titleHighted;
    string titleDisabled;
    string titleSelected;
    
    CAColor4B titleColorNormal;
    CAColor4B titleColorHighted;
    CAColor4B titleColorDisabled;
    CAColor4B titleColorSelected;
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
public:
    dataModel m_DM;
protected:
    CAView* m_pView;
    AutoPoint m_eAutoPoint;
    CCPoint touchBegin_point;
    ViewType m_viewtype;
    ShowType m_showType;
};

#endif /* AutoView_cpp */
