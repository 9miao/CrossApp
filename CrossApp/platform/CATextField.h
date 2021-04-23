//
//  CATextField.hpp
//  tesss
//
//  Created by 栗元峰 on 15/11/20.
//
//

#ifndef CATextField_h
#define CATextField_h

#include "view/CAImageView.h"
#include "view/CAScale9ImageView.h"
#include "control/CAControl.h"
NS_CC_BEGIN

class CC_DLL CATextField;
class CC_DLL CATextFieldDelegate
{
public:
    
    virtual ~CATextFieldDelegate(){}
    
	virtual bool textFieldShouldBeginEditing(CATextField* sender)
	{
		return true;
	}

	//If the sender doesn't want to detach from the IME, return true;
	virtual bool textFieldShouldEndEditing(CATextField* sender)
	{
		return true;
	}

	//
	virtual void textFieldShouldReturn(CATextField* sender){}

	virtual void keyBoardHeight(CATextField* sender, int height){}

    //Warning!!! Warning!!! Warning!!!  This method is not on the OpenGL thread.
	virtual bool textFieldShouldChangeCharacters(CATextField* sender,
                                                 unsigned int location,
                                                 unsigned int length,
                                                 const std::string& changedText)
    {
        return true;
    }
    
    virtual void textFieldDidChangeText(CATextField* sender){}
};

class CC_DLL CATextField : public CAControl
{
public:

    //keyBoard type
    enum class KeyboardType : int
    {
        Default = 0,
        NumbersAndPunctuation,
        URL,
        NumberPad,
        PhonePad,
        NamePhonePad,
        EmailAddress,
    };
    
    //return type
    enum class ReturnType : int
    {
		Done = 0,
        Go,
        Next,
        Search,
        Send
    };
    
    //clear button Mode
    enum class ClearButtonMode : int
    {
        None = 0,
        WhileEditing
    };

	enum class Align : int
	{
        Left = 0,
		Center,
		Right
	};
    
public:
        
    CC_LISTENING_FUNCTION_D(bool(), ShouldBeginEditing);
        
    CC_LISTENING_FUNCTION_D(bool(), ShouldEndEditing);
        
    CC_LISTENING_FUNCTION_D(void(), ShouldReturn);
        
    CC_LISTENING_FUNCTION_D(void(int height), KeyBoardHeight);
        
    //Warning!!! Warning!!! Warning!!!  This method is not on the OpenGL thread.
    CC_LISTENING_FUNCTION_D(bool(ssize_t location, ssize_t length, const std::string& changedText), ShouldChangeCharacters);
        
    CC_LISTENING_FUNCTION_D(void(), DidChangeText);
public:
    
    CATextField();
    
    virtual ~CATextField();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    virtual bool resignFirstResponder();
    
    virtual bool becomeFirstResponder();
    
    static CATextField* createWithFrame(const DRect& frame);
    
    static CATextField* createWithCenter(const DRect& rect);
    
    static CATextField* createWithLayout(const DLayout& layout);
    
    bool init();
    
	//delegate
	CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CATextFieldDelegate*, m_pDelegate, Delegate);

    //edit clearButton default:ClearButtonNone
    //MarginsRight set,clearbutton will be clear
    CC_PROPERTY(CATextField::ClearButtonMode, m_eClearBtn, ClearButtonMode);
    
    //textfield Margins    default:left 16,right 16
    CC_PROPERTY(int, m_iMarginLeft, MarginLeft);
    CC_PROPERTY(int, m_iMarginRight, MarginRight);
    
    //textfield Margins image
    void setMarginImageLeft(const DSize& imgSize,const std::string& filePath);
    void setMarginImageRight(const DSize& imgSize,const std::string& filePath);
    
    //textfield font size
    CC_PROPERTY(int, m_iFontSize, FontSize);
    
    //splaceHolder  Text/Color
    CC_PROPERTY_PASS_BY_REF(std::string, m_sPlaceHolderText, PlaceHolderText);
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cPlaceHdolderColor, PlaceHolderColor);
    
    //Text     Text/Color
    CC_PROPERTY_PASS_BY_REF(std::string, m_sText, Text);
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_cTextColor, TextColor);
    
    //keyBoardType     default:KeyboardTypeDefault
    CC_PROPERTY(CATextField::KeyboardType, m_eKeyBoardType, KeyboardType);
    
    //returnType       default:ReturnTypeDone
    CC_PROPERTY(CATextField::ReturnType, m_eReturnType, ReturnType);
    
	//Align  default:left
    CC_PROPERTY(CATextField::Align, m_eAlign, Align);
    
    //Password, the default is false
    CC_PROPERTY_IS(bool, m_bSecureTextEntry, SecureTextEntry);
    
    //Return key to recover the keyboard, the default is true
    CC_SYNTHESIZE_IS(bool, m_bAllowkeyBoardHide, AllowkeyBoardHide);
    
    CC_PROPERTY(int, m_iMaxLength, MaxLength);
    
    //BackgroundImage
    void setBackgroundImage(CAImage* image);
    
public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
protected:
    
    void updateAttributedPlaceholder();
    
    void delayShowImage();

    void showImage();
    
    void hideTextField();
    
    void showTextField();
    
    void hideNativeTextField();
    
    void showNativeTextField();
    
    void update(float dt);
    
    virtual void setContentSize(const DSize& contentSize);
    
private:
    
    void*                   m_pTextField;
    
    CAImageView*            m_pImgeView;
    
    DSize                   m_cImageSize;
    
    CAScale9ImageView*      m_pBackgroundView;
    
    bool                    m_bUpdateImage;
    
    DPoint                  m_obLastPoint;
    
    bool                    m_bMoved;
};

NS_CC_END

#endif /* CATextField_hpp */
