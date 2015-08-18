//
//  CAFlashView.h
//  CrossApp
//
//  Created by qiaoxin on 15-05-12.
//


#ifndef __CrossApp__CAFlashView__
#define __CrossApp__CAFlashView__

#include <iostream>
#include "CAFlash.h"

NS_CC_EXT_BEGIN

class  CAFlashView : public CAView
{
    
public:
    CAFlashView();
    virtual ~CAFlashView();
	static CAFlashView* createWithFlash(extension::CAFlash* flash);
    bool initWithFlash(CAFlash* flash);
    bool init();
	void setFlash(extension::CAFlash* flash);
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

NS_CC_EXT_END

#endif /* defined(__CrossApp__CAFlashView__) */
