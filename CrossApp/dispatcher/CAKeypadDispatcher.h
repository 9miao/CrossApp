

#ifndef __CAKEYPAD_DISPATCHER_H__
#define __CAKEYPAD_DISPATCHER_H__

#include "CAKeypadDelegate.h"
#include "basics/CASTLContainer.h"

NS_CC_BEGIN

typedef enum
{
    kTypeBackClicked = 1,
    kTypeMenuClicked,
} ccKeypadMSGType;

class CC_DLL CAKeypadDispatcher : public CAObject
{
public:
    
    CAKeypadDispatcher();
    
    ~CAKeypadDispatcher();

    void addDelegate(CAKeypadDelegate* pDelegate);

    void removeDelegate(CAKeypadDelegate* pDelegate);

    void forceAddDelegate(CAKeypadDelegate* pDelegate);

    void forceRemoveDelegate(CAKeypadDelegate* pDelegate);

    bool dispatchKeypadMSG(ccKeypadMSGType nMsgType);

protected:

    CAVector<CAObject*> m_vDelegates;
    bool m_bLocked;
    bool m_bToAdd;
    bool m_bToRemove;

    std::vector<CAKeypadDelegate*> m_vHandlersToAdd;
    std::vector<CAKeypadDelegate*> m_vHandlersToRemove;
};

// end of input group
/// @} 

NS_CC_END

#endif //__CCKEYPAD_DISPATCHER_H__
