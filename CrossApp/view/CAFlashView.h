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
    bool init();
    void setFlash(CAFlash* flash);
    void stopAnimation();
    void runAnimation();
    void setRepeatForever(bool flag);
    bool isRepeatForever();
    void setRunning(bool running);
    bool isRunning();
    
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float dt);
    virtual void draw();
protected:
    CAFlash*    m_pFlash;
    bool        m_bIsRepeatForever;
    bool        m_bIsRunning;
};

NS_CC_END

#endif /* defined(__CrossApp__CAFlashView__) */
