

#ifndef __CC_IME_DELEGATE_H__
#define __CC_IME_DELEGATE_H__

#include <string>
#include <utility>
#include "basics/CAGeometry.h"

NS_CC_BEGIN

/**
 * @addtogroup input
 * @{
 */

typedef struct
{
    CCRect  begin;              // the soft keyboard rectangle when animation begins
    CCRect  end;                // the soft keyboard rectangle when animation ends
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
    
    virtual void AndroidWillInsertText(int start,const char* str,int before,int count){CC_UNUSED_PARAM(before);};
    /**
    @brief    Called by CAIMEDispatcher after the user clicks the backward key.
    */
    virtual void deleteBackward() {}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    virtual void deleteForward() {}
    virtual void cursorMoveBackward(bool selected) {CC_UNUSED_PARAM(selected);}
    virtual void cursorMoveForward(bool selected) {CC_UNUSED_PARAM(selected);}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    virtual void copyToClipboard(std::string *content) {CC_UNUSED_PARAM(content);}
    virtual void cutToClipboard(std::string *content) {CC_UNUSED_PARAM(content);}
    virtual void pasteFromClipboard(const char *content) {CC_UNUSED_PARAM(content);}
    virtual void selectAll() {}
#endif

    virtual void getKeyBoardHeight(int height) {}
    
    virtual void getKeyBoradReturnCallBack() {}
    /**
    @brief    Called by CAIMEDispatcher for text stored in delegate.
    */
    virtual const char * getContentText() { return 0; }
    virtual int getCursorPos() { return 0; }
    virtual std::pair<int, int> getCharRange() { return std::pair<int, int>(0, 0); }

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
