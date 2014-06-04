

#ifndef __CCSCENE_H__
#define __CCSCENE_H__

#include "sprite_nodes/CAView.h"
#include "ui_controller/CAViewController.h"
NS_CC_BEGIN

class CC_DLL CAWindow : public CAView
{
    
    CC_PROPERTY(CAViewController*, m_pRootViewController, RootViewController);
    
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
};

NS_CC_END

#endif // __CCSCENE_H__
