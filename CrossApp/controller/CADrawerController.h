//
//  CADrawerController.h
//
//  Created by 栗元峰 on 14-8-20.
//
//

#ifndef __CADrawerController__
#define __CADrawerController__

#include <iostream>
#include "CAViewController.h"

NS_CC_BEGIN

class CC_DLL CADrawerController: public CAViewController
{
    
public:

    CADrawerController();
    
    virtual ~CADrawerController();
    
    virtual bool initWithController(CAViewController* leftViewController,
                                    CAViewController* rightViewController);
    
    void replaceRightViewController(CAViewController* rightViewController);
    
    CAViewController* getLeftViewController();
    
    CAViewController* getRightViewController();
    
    void showLeftViewController(bool animated);
    
    void hideLeftViewController(bool animated);
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    CC_PROPERTY_IS(bool, m_bTouchMoved, TouchMoved);
    
    CC_SYNTHESIZE(bool, m_bEffect3D, Effect3D);
    
    bool isShowLeftViewController();
    
    virtual bool isReachBoundaryLeft();
    
    virtual bool isReachBoundaryRight();
    
    void setBackgroundImage(CAImage* var);
    
    CC_DEPRECATED_ATTRIBUTE void setBackgroundView(CAView* var);
    
    CC_DEPRECATED_ATTRIBUTE CAView* getBackgroundView();
    
    CC_DEPRECATED_ATTRIBUTE virtual bool initWithController(CAViewController* leftViewController,
                                    CAViewController* rightViewController,
                                    float division);
    
protected:
    
    void begin();
    
    void showEnded();
    
    void hideEnded();
    
    void updateViewFrame();
    
    void scheduleShowAction(float dt);
    
    void scheduleHideAction(float dt);
    
    void changeStatusBarOrientation(CAObject* obj);
    
    void achieveLayout();
    
protected:
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    void setLeftViewController(CAViewController* leftViewController);
    
    void setRightViewController(CAViewController* rightViewController);
    
    CAViewController* m_pLeftViewController;
    
    CAViewController* m_pRightViewController;
    
    CAView* m_pContainer[2];
    
    CAView* m_pBackgroundView;
    
    float m_fDivision;
    
    float m_fCurrDivision;
    
    bool m_bShow;
    
    int m_fOffX;
    
    bool m_bAnimation;
    
    DLayout m_rShowLayout[2];
    DLayout m_rHideLayout[2];
};

NS_CC_END

#endif /* defined(__CartoonHouse__CADrawerController__) */
