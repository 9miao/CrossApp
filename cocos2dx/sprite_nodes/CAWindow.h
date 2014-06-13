

#ifndef __CCSCENE_H__
#define __CCSCENE_H__

#include "sprite_nodes/CAView.h"
#include "ui_controller/CAViewController.h"
NS_CC_BEGIN

class CC_DLL CAWindow : public CAView
{

public:
    /**
     *  @js ctor
     */
    CAWindow();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CAWindow();
    bool init();

    static CAWindow *create(void);
    
    void presentModalViewController(CAViewController* controller, bool animated);
    
    void dismissModalViewController(bool animated);
    
protected:
    
    void presentStart();
    
    void presentEnd();
    
    void dismissStart();
    
    void dismissEnd();
    
protected:
    
    CC_PROPERTY(CAViewController*, m_pRootViewController, RootViewController);
    
    CC_SYNTHESIZE_READONLY(CAViewController*, m_pModalViewController, ModalViewController);
};

NS_CC_END

#endif // __CCSCENE_H__
