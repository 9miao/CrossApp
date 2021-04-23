

#ifndef __CC_IME_DELEGATE_H__
#define __CC_IME_DELEGATE_H__

#include <string>
#include <utility>
#include "ccTypes.h"

NS_CC_BEGIN

/**
 * @addtogroup input
 * @{
 */

typedef struct
{
    DRect  begin;              // the soft keyboard rectangle when animation begins
    DRect  end;                // the soft keyboard rectangle when animation ends
    float     duration;           // the soft keyboard animation duration
} CCIMEKeyboardNotificationInfo;

/**
@brief    Input method editor delegate.
@js NA
@lua NA
*/
class CC_DLL CAIMEDelegate
{
public:
    virtual ~CAIMEDelegate();

    virtual bool attachWithIME();
    virtual bool detachWithIME();

protected:
    friend class CAIMEDispatcher;

    /**
    @brief    Decide if the delegate instance is ready to receive an IME message.

    Called by CAIMEDispatcher.
    */
    virtual bool canAttachWithIME() { return false; }
    /**
    @brief    When the delegate detaches from the IME, this method is called by CAIMEDispatcher.
    */
    virtual void didAttachWithIME() {}

    /**
    @brief    Decide if the delegate instance can stop receiving IME messages.
    */
    virtual bool canDetachWithIME() { return false; }

    /**
    @brief    When the delegate detaches from the IME, this method is called by CAIMEDispatcher.
    */
    virtual void didDetachWithIME() {}

    /**
    @brief    Called by CAIMEDispatcher when text input received from the IME.
    */
    virtual void insertText(const char * text, int len) {CC_UNUSED_PARAM(text);CC_UNUSED_PARAM(len);}

    
    virtual void willInsertText(const char * text, int len) {CC_UNUSED_PARAM(text);CC_UNUSED_PARAM(len);}
    
    virtual void AndroidWillInsertText(int start, const char* str, int before, int count){CC_UNUSED_PARAM(before);};
    
    /**
    @brief    Called by CAIMEDispatcher after the user clicks the backward key.
    */
    virtual void deleteBackward() {}

    virtual void selectAll() {}
	virtual void cursorMoveBackward() {}
    virtual void cursorMoveForward() {}
    virtual void cursorMoveUp() {}
    virtual void cursorMoveDown() {}
	virtual void moveSelectChars(bool isLeftBtn, const DPoint& pt) {}
	virtual void moveArrowBtn(const DPoint& pt) {}

	virtual void copyToClipboard() {}
	virtual void cutToClipboard() {}
	virtual void pasteFromClipboard() {}

    virtual void getKeyBoardHeight(int height) {}
    
    virtual void getKeyBoradReturnCallBack() {}
    /**
    @brief    Called by CAIMEDispatcher for text stored in delegate.
    */
    virtual const char * getContentText() { return 0; }

    virtual int getCursorPos() { return 0; }
    //////////////////////////////////////////////////////////////////////////
    // keyboard show/hide notification
    //////////////////////////////////////////////////////////////////////////
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info)   {CC_UNUSED_PARAM(info);}
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);}
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info)   {CC_UNUSED_PARAM(info);}
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);}
protected:
    CAIMEDelegate();
};

// end of input group
/// @}

NS_CC_END

#endif    // __CC_IME_DELEGATE_H__
