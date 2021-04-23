

#include "CAKeypadDispatcher.h"
#include "basics/CAApplication.h"
#include "CATouchDispatcher.h"
NS_CC_BEGIN

CAKeypadDispatcher::CAKeypadDispatcher()
: m_bLocked(false)
{

}

CAKeypadDispatcher::~CAKeypadDispatcher()
{

}

bool CAKeypadDispatcher::dispatchKeypadMSG(KeypadMSGType nMsgType)
{
    if (!CAApplication::getApplication()->getTouchDispatcher()->isDispatchEvents())
    {
        return true;
    }
    m_bLocked = true;
    switch (nMsgType) {
        case KeypadMSGType::BackClicked:
            if (m_obBackClicked)
            {
                m_obBackClicked();
            }
            break;
        case KeypadMSGType::MenuClicked:
            if (m_obMenuClicked)
            {
                m_obMenuClicked();
            }
            break;
        default:
            break;
    }
    m_bLocked = false;
    
    return true;
}

NS_CC_END
