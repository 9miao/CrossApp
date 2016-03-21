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
                                                 unsigned int lenght,
                                                 const std::string& changedText)
    {
        return true;
    }
};

class CC_DLL CATextField : public CAView
{
public:

    //keyBoard type
    typedef enum
    {
        Default = 0,
        NumbersAndPunctuation,
        URL,
        NumberPad,
        PhonePad,
        NamePhonePad,
        EmailAddress,
    }KeyboardType;
    
    //return type
    typedef enum
    {
		Done=0,
        Go,
        Next,
        Search,
        Send
    }ReturnType;
    
    //clear button Mode
    typedef enum
    {
        None = 0,
        WhileEditing
    }ClearButtonMode;

	typedef enum 
	{
        Left,
		Center,
		Right
	}TextFieldAlign;
    
public:
    
    CATextField();
    
    virtual ~CATextField();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    virtual bool resignFirstResponder();
    
    virtual bool becomeFirstResponder();
    
    static CATextField* createWithFrame(const DRect& frame);
    
    static CATextField* createWithCenter(const DRect& rect);
    
    static CATextField* createWithLayout(const DRectLayout& layout);
    
    bool init();
    
	//delegate
	CC_SYNTHESIZE(CATextFieldDelegate*, m_pDelegate, Delegate);

    //edit clearButton default:ClearButtonNone
    //MarginsRight set,clearbutton will be clear
    CC_PROPERTY_PASS_BY_REF(ClearButtonMode, m_eClearBtn, ClearButtonMode);
    
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
    CC_PROPERTY_PASS_BY_REF(KeyboardType, m_eKeyBoardType, KeyboardType);
    
    //returnType       default:ReturnTypeDone
    CC_PROPERTY_PASS_BY_REF(ReturnType, m_eReturnType, ReturnType);
    
	//textFieldAlign  default:left
	CC_PROPERTY_PASS_BY_REF(TextFieldAlign, m_eAlign, TextFieldAlign);
    
    //Password, the default is false
    CC_PROPERTY_IS(bool, m_bSecureTextEntry, SecureTextEntry);
    
    //Return key to recover the keyboard, the default is true
    CC_SYNTHESIZE_IS(bool, m_bAllowkeyBoardHide, AllowkeyBoardHide);
    
    CC_PROPERTY(int, m_iMaxLenght, MaxLenght);
    
    //BackgroundImage
    void setBackgroundImage(CAImage* image);
    
public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
protected:
    
	void clearBtnCallBack(CAControl* con,DPoint point);
    
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
};

NS_CC_END

#endif /* CATextField_hpp */
