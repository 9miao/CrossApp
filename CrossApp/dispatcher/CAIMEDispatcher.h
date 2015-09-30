

#ifndef __CC_IME_DISPATCHER_H__
#define __CC_IME_DISPATCHER_H__

#include <string>
#include <utility>
#include "CAIMEDelegate.h"
NS_CC_BEGIN

/**
 * @addtogroup input
 * @{
 */

/**
@brief    Input Method Edit Message Dispatcher.
@js NA
@lua NA
*/
class CC_DLL CAIMEDispatcher
{
public:
    ~CAIMEDispatcher();

    /**
    @brief Returns the shared CAIMEDispatcher object for the system.
    */
    static CAIMEDispatcher* sharedDispatcher();

//     /**
//     @brief Releases all CCIMEDelegates from the shared dispatcher.
//     */
//     static void purgeSharedDispatcher();

    /**
    @brief Dispatches the input text from IME.
    */
    void dispatchInsertText(const char * pText, int nLen);

    void dispatchWillInsertText(const char * pText, int nLen);
    
    void dispatchAndroidWillInsertText(int start, const std::string &str, int before, int count);
    /**
    @brief Dispatches the delete-backward operation.
    */
    void dispatchDeleteBackward();

    void dispatchGetKeyBoardHeight(int height);
    
    void dispatchGetKeyBoradReturnCallBack();

	void dispatchCursorMoveBackward();
	void dispatchCursorMoveForward();
    void dispatchCursorMoveUp();
    void dispatchCursorMoveDown();
	void dispatchMoveSelectChars(bool isLeftBtn, const DPoint& pt);
	void dispatchMoveSelectCharsCancel(const DPoint& pt);
	void dispatchMoveArrowBtn(const DPoint& pt);
	void dispatchCopyToClipboard();
	void dispatchCutToClipboard();
	void dispatchPasteFromClipboard();
    void dispatchSelectAll();
	int getCursorPos();

    /**
    @brief Get the content text from CAIMEDelegate, retrieved previously from IME.
    */
    const char * getContentText();

    //////////////////////////////////////////////////////////////////////////
    // dispatch keyboard notification
    //////////////////////////////////////////////////////////////////////////
    void dispatchKeyboardWillShow(CCIMEKeyboardNotificationInfo& info);
    void dispatchKeyboardDidShow(CCIMEKeyboardNotificationInfo& info);
    void dispatchKeyboardWillHide(CCIMEKeyboardNotificationInfo& info);
    void dispatchKeyboardDidHide(CCIMEKeyboardNotificationInfo& info);

protected:
    friend class CAIMEDelegate;

    /**
    @brief Add delegate to receive IME messages.
    */
    void addDelegate(CAIMEDelegate * pDelegate);

    /**
    @brief Attach the pDelegate to the IME.
    @return If the old delegate can detach from the IME, and the new delegate 
            can attach to the IME, return true, otherwise false.
    */
    bool attachDelegateWithIME(CAIMEDelegate * pDelegate);
    bool detachDelegateWithIME(CAIMEDelegate * pDelegate);

    /**
    @brief Remove the delegate from the delegates which receive IME messages.
    */
    void removeDelegate(CAIMEDelegate * pDelegate);

private:
    CAIMEDispatcher();
    
    class Impl;
    Impl * m_pImpl;
};

// end of input group
/// @}

NS_CC_END

#endif    // __CC_IME_DISPATCHER_H__
