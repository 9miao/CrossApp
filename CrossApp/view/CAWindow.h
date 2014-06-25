

#ifndef __CCSCENE_H__
#define __CCSCENE_H__

#include "CAView.h"
#include "controller/CAViewController.h"
NS_CC_BEGIN

enum
{
    CAWindowZoderBottom = 0,
    CAWindowZoderCenter = 1,
    CAWindowZoderTop = 0xffff
};

class CC_DLL CAWindow : public CAView
{

public:

    static CAWindow *create(void);
    
    CAWindow();

    virtual ~CAWindow();
    
    bool init();
    
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
