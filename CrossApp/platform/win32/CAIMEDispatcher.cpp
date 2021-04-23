

#include "CAIMEDispatcher.h"
#include "platform/CACommon.h"
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
            CC_BREAK_IF(! pDelegate->canAttachWithIME());

            // detach first
            CAIMEDelegate * pOldDelegate = m_pImpl->m_DelegateWithIme;
            m_pImpl->m_DelegateWithIme->getKeyBoardHeight(0);
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

        m_pImpl->m_DelegateWithIme->getKeyBoardHeight(0);
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
void CAIMEDispatcher::dispatchAndroidWillInsertText(int start, const std::string &str, int before, int count)
{
    do
    {
        CC_BREAK_IF(! m_pImpl);
        CC_BREAK_IF(! m_pImpl->m_DelegateWithIme);
        m_pImpl->m_DelegateWithIme->AndroidWillInsertText(start, str.c_str(), before, count);
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
        m_pImpl->m_DelegateWithIme->didAttachWithIME();
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

void CAIMEDispatcher::dispatchCursorMoveBackward()
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

		m_pImpl->m_DelegateWithIme->cursorMoveBackward();
    } while (0);
}

void CAIMEDispatcher::dispatchCursorMoveForward()
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

		m_pImpl->m_DelegateWithIme->cursorMoveForward();
    } while (0);
}

void CAIMEDispatcher::dispatchCursorMoveUp()
{
    do
    {
        CC_BREAK_IF(!m_pImpl);
        
        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);
        
        m_pImpl->m_DelegateWithIme->cursorMoveUp();
    } while (0);
}

void CAIMEDispatcher::dispatchCursorMoveDown()
{
    do
    {
        CC_BREAK_IF(!m_pImpl);
        
        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);
        
        m_pImpl->m_DelegateWithIme->cursorMoveDown();
    } while (0);
}

void CAIMEDispatcher::dispatchMoveSelectChars(bool isLeftBtn, const DPoint& pt)
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

		return m_pImpl->m_DelegateWithIme->moveSelectChars(isLeftBtn, pt);
    } while (0);
}

void CAIMEDispatcher::dispatchMoveArrowBtn(const DPoint& pt)
{
	do
	{
		CC_BREAK_IF(!m_pImpl);

		// there is no delegate attached to IME
		CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

		m_pImpl->m_DelegateWithIme->moveArrowBtn(pt);
	} while (0);
}

void CAIMEDispatcher::dispatchCopyToClipboard()
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->copyToClipboard();
    }
    while (0);
}

void CAIMEDispatcher::dispatchCutToClipboard()
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->cutToClipboard();
    }
    while (0);
}

void CAIMEDispatcher::dispatchPasteFromClipboard()
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->pasteFromClipboard();
    }
    while (0);
}

void CAIMEDispatcher::dispatchSelectAll()
{
    do
    {
        CC_BREAK_IF(!m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(!m_pImpl->m_DelegateWithIme);

        m_pImpl->m_DelegateWithIme->selectAll();
    }
    while (0);
}

int CAIMEDispatcher::getCursorPos()
{
    if (m_pImpl && m_pImpl->m_DelegateWithIme)
    {
        return m_pImpl->m_DelegateWithIme->getCursorPos();
    }
    return 0;
}

const char * CAIMEDispatcher::getContentText()
{
    const char * pszContentText = 0;
    if (m_pImpl && m_pImpl->m_DelegateWithIme)
    {
        pszContentText = m_pImpl->m_DelegateWithIme->getContentText();
    }
    return (pszContentText) ? pszContentText : "";
}

//////////////////////////////////////////////////////////////////////////
// dispatch keyboard message
//////////////////////////////////////////////////////////////////////////

void CAIMEDispatcher::dispatchKeyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    if (m_pImpl)
    {
        if(m_pImpl->m_DelegateWithIme){
            m_pImpl->m_DelegateWithIme->keyboardWillShow(info);
        }
        
        return;
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
        if(m_pImpl->m_DelegateWithIme){
            m_pImpl->m_DelegateWithIme->keyboardDidShow(info);
        }
        
        return;
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
        if(m_pImpl->m_DelegateWithIme)
        {
            m_pImpl->m_DelegateWithIme->keyboardWillHide(info);
#if CC_TARGET_PLATFORM==CC_PLATFORM_IOS
			CC_RETURN_IF(! m_pImpl->m_DelegateWithIme->canDetachWithIME());
            m_pImpl->m_DelegateWithIme->getKeyBoardHeight(0);
            m_pImpl->m_DelegateWithIme->didDetachWithIME();
#endif
        }
    }
}

void CAIMEDispatcher::dispatchKeyboardDidHide(CCIMEKeyboardNotificationInfo& info)
{
    if (m_pImpl)
    {
        if(m_pImpl->m_DelegateWithIme){
            m_pImpl->m_DelegateWithIme->keyboardDidHide(info);
#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
			CC_RETURN_IF(!m_pImpl->m_DelegateWithIme->canDetachWithIME());
			m_pImpl->m_DelegateWithIme->getKeyBoardHeight(0);
			m_pImpl->m_DelegateWithIme->didDetachWithIME();
#endif
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
