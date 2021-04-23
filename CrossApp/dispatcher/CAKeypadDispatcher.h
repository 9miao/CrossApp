

#ifndef __CAKEYPAD_DISPATCHER_H__
#define __CAKEYPAD_DISPATCHER_H__
#include "basics/CASTLContainer.h"
NS_CC_BEGIN


class CC_DLL CAKeypadDispatcher : public CAObject
{
public:
    
    enum class KeypadMSGType : int
    {
        BackClicked = 1,
        MenuClicked,
    };
    
public:
    
    CAKeypadDispatcher();
    
    ~CAKeypadDispatcher();

    CC_LISTENING_FUNCTION(void(), BackClicked);
    
    CC_LISTENING_FUNCTION(void(), MenuClicked);
    
    bool dispatchKeypadMSG(KeypadMSGType nMsgType);

protected:

    bool m_bLocked;
};

// end of input group
/// @} 

NS_CC_END

#endif //__CCKEYPAD_DISPATCHER_H__
