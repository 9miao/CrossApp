

#include "CAIMEDispatcher.h"

#include <list>

NS_CC_BEGIN

//////////////////////////////////////////////////////////////////////////
// add/remove delegate in CAIMEDelegate Cons/Destructor
//////////////////////////////////////////////////////////////////////////

CAIMEDelegate::CAIMEDelegate()
{
    CAIMEDispatcher::sharedDispatcher()->addDelegate(this);
}

CAIMEDelegate::~CAIMEDelegate()
{
    CAIMEDispatcher::sharedDispatcher()->removeDelegate(this);
}

bool CAIMEDelegate::attachWithIME()
{
    return CAIMEDispatcher::sharedDispatcher()->attachDelegateWithIME(this);
}

bool CAIMEDelegate::detachWithIME()
{
    return CAIMEDispatcher::sharedDispatcher()->detachDelegateWithIME(this);
}

//////////////////////////////////////////////////////////////////////////

typedef std::list< CAIMEDelegate * > DelegateList;
typedef std::list< CAIMEDelegate * >::iterator  DelegateIter;

//////////////////////////////////////////////////////////////////////////
// Delegate List manage class
//////////////////////////////////////////////////////////////////////////

class CAIMEDispatcher::Impl
{
public:
    Impl()
    {
    }

    ~Impl()
    {

    }

    void init()
    {
        m_DelegateWithIme = 0;
    }

    DelegateIter findDelegate(CAIMEDelegate* pDelegate)
    {
        DelegateIter end = m_DelegateList.end();
        for (DelegateIter iter = m_DelegateList.begin(); iter != end; ++iter)
        {
            if (pDelegate == *iter)
            {
                return iter;
            }
        }
        return end;
    }

    DelegateList    m_DelegateList;
    CAIMEDelegate*  m_DelegateWithIme;
};

//////////////////////////////////////////////////////////////////////////
// Cons/Destructor
//////////////////////////////////////////////////////////////////////////

CAIMEDispatcher::CAIMEDispatcher()
: m_pImpl(new CAIMEDispatcher::Impl)
{
    m_pImpl->init();
}

CAIMEDispatcher::~CAIMEDispatcher()
{
    CC_SAFE_DELETE(m_pImpl);
}

//////////////////////////////////////////////////////////////////////////
// Add/Attach/Remove CAIMEDelegate
//////////////////////////////////////////////////////////////////////////

void CAIMEDispatcher::addDelegate(CAIMEDelegate* pDelegate)
{
    if (! pDelegate || ! m_pImpl)
    {
        return;
    }
    if (m_pImpl->m_DelegateList.end() != m_pImpl->findDelegate(pDelegate))
    {
        // pDelegate already in list
        return;
    }
    m_pImpl->m_DelegateList.push_front(pDelegate);
}

bool CAIMEDispatcher::attachDelegateWithIME(CAIMEDelegate * pDelegate)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! m_pImpl || ! pDelegate);

        DelegateIter end  = m_pImpl->m_DelegateList.end();
        DelegateIter iter = m_pImpl->findDelegate(pDelegate);

        // if pDelegate is not in delegate list, return
        CC_BREAK_IF(end == iter);

        if (m_pImpl->m_DelegateWithIme)
        {
            // if old delegate canDetachWithIME return false 
            // or pDelegate canAttachWithIME return false,
            // do nothing.
            CC_BREAK_IF(! m_pImpl->m_DelegateWithIme->canDetachWithIME()
                || ! pDelegate->canAttachWithIME());

            // detach first
            CAIMEDelegate * pOldDelegate = m_pImpl->m_DelegateWithIme;
            m_pImpl->m_DelegateWithIme = 0;
            pOldDelegate->didDetachWithIME();

            m_pImpl->m_DelegateWithIme = *iter;
            pDelegate->didAttachWithIME();
            bRet = true;
            break;
        }

        // delegate hasn't attached to IME yet
        CC_BREAK_IF(! pDelegate->canAttachWithIME());

        m_pImpl->m_DelegateWithIme = *iter;
        pDelegate->didAttachWithIME();
        bRet = true;
    } while (0);
    return bRet;
}

bool CAIMEDispatcher::detachDelegateWithIME(CAIMEDelegate * pDelegate)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! m_pImpl || ! pDelegate);

        // if pDelegate is not the current delegate attached to IME, return
        CC_BREAK_IF(m_pImpl->m_DelegateWithIme != pDelegate);

        CC_BREAK_IF(! pDelegate->canDetachWithIME());

        m_pImpl->m_DelegateWithIme = 0;
        pDelegate->didDetachWithIME();
        bRet = true;
    } while (0);
    return bRet;
}

void CAIMEDispatcher::removeDelegate(CAIMEDelegate* pDelegate)
{
    do 
    {
        CC_BREAK_IF(! pDelegate || ! m_pImpl);

        DelegateIter iter = m_pImpl->findDelegate(pDelegate);
        DelegateIter end  = m_pImpl->m_DelegateList.end();
        CC_BREAK_IF(end == iter);

        if (m_pImpl->m_DelegateWithIme)

        if (*iter == m_pImpl->m_DelegateWithIme)
        {
            m_pImpl->m_DelegateWithIme = 0;
        }
        m_pImpl->m_DelegateList.erase(iter);
    } while (0);
}

//////////////////////////////////////////////////////////////////////////
// dispatch text message
//////////////////////////////////////////////////////////////////////////
void CAIMEDispatcher::dispatchWillInsertText(const char * pText, int nLen)
{
    
    do
    {
        CC_BREAK_IF(! m_pImpl || ! pText);
        // there is no delegate attached to IME
        CC_BREAK_IF(! m_pImpl->m_DelegateWithIme);
        
        m_pImpl->m_DelegateWithIme->willInsertText(pText, nLen);
    } while (0);
}
void CAIMEDispatcher::dispatchAndroidWillInsertText(int start,const std::string &str,int before,int count)
{
    do
    {
        CC_BREAK_IF(! m_pImpl);
        CC_BREAK_IF(! m_pImpl->m_DelegateWithIme);
        m_pImpl->m_DelegateWithIme->AndroidWillInsertText(start,str.c_str(),before,count);
    } while (0);
}
void CAIMEDispatcher::dispatchInsertText(const char * pText, int nLen)
{
    do 
    {
        CC_BREAK_IF(! m_pImpl || ! pText || nLen <= 0);

        // there is no delegate attached to IME
        CC_BREAK_IF(! m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->insertText(pText, nLen);
    } while (0);
}

void CAIMEDispatcher::dispatchGetKeyBoardHeight(int height)
{
    do
    {
        CC_BREAK_IF(! m_pImpl);
        
        CC_BREAK_IF(! m_pImpl->m_DelegateWithIme);
        
        m_pImpl->m_DelegateWithIme->getKeyBoardHeight(height);
    }while (0);
        
    
}
void CAIMEDispatcher::dispatchGetKeyBoradReturnCallBack()
{
    do
    {
        CC_BREAK_IF(! m_pImpl);
        
        CC_BREAK_IF(! m_pImpl->m_DelegateWithIme);
        
        m_pImpl->m_DelegateWithIme->getKeyBoradReturnCallBack();
    }while (0);
}
void CAIMEDispatcher::dispatchDeleteBackward()
{
    do 
    {
        CC_BREAK_IF(! m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(! m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->deleteBackward();
    } while (0);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void CAIMEDispatcher::dispatchDeleteForward()
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->deleteForward();
    } while (0);
}

void CAIMEDispatcher::dispatchCursorMoveBackward(bool selected)
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->cursorMoveBackward(selected);
    } while (0);
}

void CAIMEDispatcher::dispatchCursorMoveForward(bool selected)
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->cursorMoveForward(selected);
    } while (0);
}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
void CAIMEDispatcher::dispatchCopyToClipboard(std::string *content)
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->copyToClipboard(content);
    } while (0);
}

void CAIMEDispatcher::dispatchCutToClipboard(std::string *content)
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->cutToClipboard(content);
    } while (0);
}

void CAIMEDispatcher::dispatchPasteFromClipboard(const char *content)
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->pasteFromClipboard(content);
    } while (0);
}

void CAIMEDispatcher::dispatchSelectAll()
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->selectAll();
    } while (0);
}

#endif

const char * CAIMEDispatcher::getContentText()
{
    const char * pszContentText = 0;
    if (m_pImpl && m_pImpl->m_DelegateWithIme)
    {
        pszContentText = m_pImpl->m_DelegateWithIme->getContentText();
    }
    return (pszContentText) ? pszContentText : "";
}

int CAIMEDispatcher::getCursorPos()
{
    if (m_pImpl && m_pImpl->m_DelegateWithIme)
    {
        return m_pImpl->m_DelegateWithIme->getCursorPos();
    }
    return 0;
}

std::pair<int, int> CAIMEDispatcher::getCharRange()
{
    if (m_pImpl && m_pImpl->m_DelegateWithIme)
    {
        return m_pImpl->m_DelegateWithIme->getCharRange();
    }
    return std::pair<int, int>(0, 0);
}

//////////////////////////////////////////////////////////////////////////
// dispatch keyboard message
//////////////////////////////////////////////////////////////////////////

void CAIMEDispatcher::dispatchKeyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    if (m_pImpl)
    {
        CAIMEDelegate * pDelegate = 0;
        DelegateIter last = m_pImpl->m_DelegateList.end();
        for (DelegateIter first = m_pImpl->m_DelegateList.begin(); first != last; ++first)
        {
            pDelegate = *(first);
            if (pDelegate)
            {
                pDelegate->keyboardWillShow(info);
            }
        }
    }
}

void CAIMEDispatcher::dispatchKeyboardDidShow(CCIMEKeyboardNotificationInfo& info)
{
    if (m_pImpl)
    {
        CAIMEDelegate * pDelegate = 0;
        DelegateIter last = m_pImpl->m_DelegateList.end();
        for (DelegateIter first = m_pImpl->m_DelegateList.begin(); first != last; ++first)
        {
            pDelegate = *(first);
            if (pDelegate)
            {
                pDelegate->keyboardDidShow(info);
            }
        }
    }
}

void CAIMEDispatcher::dispatchKeyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
    if (m_pImpl)
    {
        CAIMEDelegate * pDelegate = 0;
        DelegateIter last = m_pImpl->m_DelegateList.end();
        for (DelegateIter first = m_pImpl->m_DelegateList.begin(); first != last; ++first)
        {
            pDelegate = *(first);
            if (pDelegate)
            {
                pDelegate->keyboardWillHide(info);
            }
        }
    }
}

void CAIMEDispatcher::dispatchKeyboardDidHide(CCIMEKeyboardNotificationInfo& info)
{
    if (m_pImpl)
    {
        CAIMEDelegate * pDelegate = 0;
        DelegateIter last = m_pImpl->m_DelegateList.end();
        for (DelegateIter first = m_pImpl->m_DelegateList.begin(); first != last; ++first)
        {
            pDelegate = *(first);
            if (pDelegate)
            {
                pDelegate->keyboardDidHide(info);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// protected member function
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////

CAIMEDispatcher* CAIMEDispatcher::sharedDispatcher()
{
    static CAIMEDispatcher s_instance;
    return &s_instance;
}

NS_CC_END
