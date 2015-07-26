

#ifndef __CAKEYPAD_DELEGATE_H__
#define __CAKEYPAD_DELEGATE_H__


#include "basics/CAObject.h"

NS_CC_BEGIN

class CC_DLL CAKeypadDelegate
{
public:

    virtual bool keyBackClicked() {return false;}

    virtual bool keyMenuClicked() {return false;};
};

class CC_DLL CAKeypadHandler : public CAObject
{
public:
    
    virtual ~CAKeypadHandler(void);

    CAKeypadDelegate* getDelegate();
    
    void setDelegate(CAKeypadDelegate *pDelegate);

    virtual bool initWithDelegate(CAKeypadDelegate *pDelegate);

public:

    static CAKeypadHandler* handlerWithDelegate(CAKeypadDelegate *pDelegate);

protected:
    
    CAKeypadDelegate* m_pDelegate;
};

// end of input group
/// @} 

NS_CC_END

#endif // __CCKEYPAD_DELEGATE_H__
