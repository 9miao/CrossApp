//
//  CATextFieldX.hpp
//  tesss
//
//  Created by 栗元峰 on 15/11/20.
//
//

#ifndef CATextFieldX_h
#define CATextFieldX_h

#include "view/CAImageView.h"

NS_CC_BEGIN

class CATextFieldX: public CAView
{
public:
    CATextFieldX();
    
    virtual ~CATextFieldX();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    virtual bool resignFirstResponder();
    
    virtual bool becomeFirstResponder();
    
    static CATextFieldX* createWithFrame(const DRect& frame);
    
    static CATextFieldX* createWithCenter(const DRect& rect);
    
    bool init();

public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
protected:

    void showImageView();
    
    void hideNativeTextField();
    
    void showNativeTextField();
    
    void hide();
    
    void update(float dt);
    
    virtual void setContentSize(const DSize& contentSize);
    
    void* m_pTextField;
    
    CAImageView* m_pImgeView;
    DSize m_cImageSize;
};
NS_CC_END

#endif /* CATextFieldX_hpp */
