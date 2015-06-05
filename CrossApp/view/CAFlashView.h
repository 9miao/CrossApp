//
//  CAFlashView.h
//  CrossApp
//
//  Created by qiaoxin on 15-05-12.
//


#ifndef __CrossApp__CAFlashView__
#define __CrossApp__CAFlashView__

#include <iostream>
#include "CrossApp.h"

NS_CC_BEGIN

class CAView;

class CC_DLL CAFlashView : public CAView
{
public:
    CAFlashView();
    virtual ~CAFlashView();
    static CAFlashView* createWithFlash(CAFlash* flash);
    bool initWithFlash(CAFlash* flash);
    void setFlash(CAFlash* flash);
    void stopAction();
    void runAction();
    void setRepeatForever(bool flag);
    bool getRepeatForever();
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float dt);
    virtual void draw();
protected:
    CAFlash*    m_pFlash;
};

NS_CC_END

#endif /* defined(__CrossApp__CAFlashView__) */
