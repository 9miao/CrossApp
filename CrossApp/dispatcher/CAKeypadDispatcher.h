

#ifndef __CAKEYPAD_DISPATCHER_H__
#define __CAKEYPAD_DISPATCHER_H__

#include "CAKeypadDelegate.h"
#include "cocoa/CCArray.h"

NS_CC_BEGIN

typedef enum
{
    kTypeBackClicked = 1,
    kTypeMenuClicked,
} ccKeypadMSGType;

struct _ccCArray;

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

    CCArray* m_pDelegates;
    bool m_bLocked;
    bool m_bToAdd;
    bool m_bToRemove;

    struct _ccCArray *m_pHandlersToAdd;
    struct _ccCArray *m_pHandlersToRemove;
};

// end of input group
/// @} 

NS_CC_END

#endif //__CCKEYPAD_DISPATCHER_H__
