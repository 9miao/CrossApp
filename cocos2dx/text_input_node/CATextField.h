//
//  CATextField.h
//  project
//
//  Created by lh on 14-5-15.
//
//

#ifndef __project__CATextField__
#define __project__CATextField__

#include <iostream>
#include "CCIMEDispatcher.h"
#include "label_nodes/CALabel.h"
#include "ui_controller/CAControl.h"
#include <vector>
NS_CC_BEGIN
using namespace std;


enum eKeyBoardType{
    KEY_BOARD_TYPE_NORMAL = 0,
    KEY_BOARD_TYPE_NUMBER,
    KEY_BOARD_TYPE_ALPHABET,
};
enum eKeyBoardInputType{
    KEY_BOARD_INPUT_NORMAL=1,
    KEY_BOARD_INPUT_PASSWORD,
};
class CATextField;
class CC_DLL CATextFieldDelegate
{
public:
    virtual bool onTextFieldAttachWithIME(CATextField * sender)
    {
        CC_UNUSED_PARAM(sender);
        return false;
    }
    
    /**
     @brief    If the sender doesn't want to detach from the IME, return true;
     */
    virtual bool onTextFieldDetachWithIME(CATextField * sender)
    {
        CC_UNUSED_PARAM(sender);
        return false;
    }
    
    /**
     @brief    If the sender doesn't want to insert the text, return true;
     */
    virtual bool onTextFieldInsertText(CATextField * sender, const char * text, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(text);
        CC_UNUSED_PARAM(nLen);
        return false;
    }
    
    /**
     @brief    If the sender doesn't want to delete the delText, return true;
     */
    virtual bool onTextFieldDeleteBackward(CATextField * sender, const char * delText, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(delText);
        CC_UNUSED_PARAM(nLen);
        return false;
    }
    
    /**
     @brief    If the sender doesn't want to draw, return true.
     */
    virtual bool getKeyBoardHeight(int height)
    {
        return false;
    }
};

typedef struct _TextAttribute
{
    unsigned int charsize;
    
    float charlength;
 
}TextAttribute;

class CC_DLL CATextField
: public CAControl
, CCIMEDelegate
{
public:
    CATextField();
    
    virtual ~CATextField();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    virtual bool resignFirstResponder();
    
    virtual bool becomeFirstResponder();
    
public:
    
    static CATextField* createWithFrame(const CCRect& frame);
    
    static CATextField* createWithCenter(const CCRect& rect);
    
    bool initWithFrame(const CCRect& frame);
    
    bool initWithCenter(const CCRect& rect);
    
    CC_PROPERTY(std::string, m_sPlaceHolder, PlaceHolder);
    
    CC_PROPERTY(std::string, m_sText, Text);
    
    CC_SYNTHESIZE_READONLY(int, m_nCharCount, CharCount);
    
    CC_PROPERTY(CAColor4B, m_cSpaceHolderColor, SpaceHolderColor);
    
    CC_PROPERTY(CAColor4B, m_cTextColor, TextColor);
    
    CC_PROPERTY(CATextAlignment, m_aTextAlignment, TextAlignment);
    
    CC_SYNTHESIZE(CATextFieldDelegate*, m_pDelegate, Delegate);
    
    CC_PROPERTY(float, m_fFontSize, FontSize);
    
    CC_PROPERTY(eKeyBoardInputType, m_nInputType, InputType);
    
    inline void setKeyboardType (eKeyBoardType type) {m_keyboardType = type; }
    
    inline int getKeyboardType () {return m_keyboardType; }
  
private:
    
    std::vector<float> lengthArr;
    std::vector<int> byteArr;
    std::string frontStr;
    std::string temporaryString;
    std::string m_pPlaceHolderString;
    float beforeWidth;
    float afterWidth;
    float frontWidth;
    bool spaceHolderIsOn;
    bool isEditing;
    int byteCount;
    int bytePos;
    CAView *willBg;
    CALabel *m_pText;
    CCAction *m_pCursorAction;
    CAImageView *m_pMark;
    eKeyBoardType m_keyboardType;
    
protected:
    
    virtual bool attachWithIME();
    virtual bool detachWithIME();
    void initMarkSprite();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual bool canAttachWithIME();
    virtual bool canDetachWithIME();
    virtual void insertText(const char * text, int len);
    virtual void willInsertText(const char* text,int len);
    virtual void getKeyBoardHeight(int height);
    virtual void deleteBackward();
    virtual const char * getContentText();
};

NS_CC_END

#endif /* defined(__project__CATextField__) */
