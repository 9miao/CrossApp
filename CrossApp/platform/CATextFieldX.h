//
//  CATextFieldX.hpp
//  tesss
//
//  Created by 栗元峰 on 15/11/20.
//
//

#ifndef CATextFieldX_h
#define CATextFieldX_h

#include "view/CAImageView.h"
#include "view/CAScale9ImageView.h"
#include "control/CAControl.h"
NS_CC_BEGIN

class CATextFieldX;
class CATextFieldDelegateX
{
public:
	virtual bool textFieldShouldBeginEditing(CATextFieldX * sender)
	{
		return true;
	}

	//If the sender doesn't want to detach from the IME, return true;
	virtual bool textFieldShouldEndEditing(CATextFieldX * sender)
	{
		return true;
	}

	//
	virtual void textFieldShouldReturn(CATextFieldX *sender){}

	//
	virtual void keyBoardHeight(CATextFieldX *sender, int height){}

	//arg0  
	virtual void textFieldAfterTextChanged(CATextFieldX *sender,const char* beforeText,const char* changeText,int arg0,int arg1,int arg2){}
};

class CATextFieldX: public CAView
{
public:
    typedef enum {
        TextFieldTypeNone = 0,
        TextFieldTypeSquareRect,
        TextFieldTypeRoundedRect
    }TextFieldType;
    
    //keyBoard type
    typedef enum{
        KeyboardTypeDefault = 0,
        KeyboardTypeNumbersAndPunctuation,
        KeyboardTypeURL,
        KeyboardTypeNumberPad,
        KeyboardTypePhonePad,
        KeyboardTypeNamePhonePad,
        KeyboardTypeEmailAddress,
    }KeyboardType;
    
    //return type
    typedef enum{
		ReturnTypeDone=0,
        ReturnTypeGo,
        ReturnTypeNext,
        ReturnTypeSearch,
        ReturnTypeSend
    }ReturnType;
    
    //clear button Mode
    typedef enum{
        ClearButtonNone = 0,
        ClearButtonWhileEditing
    }ClearButtonMode;

	typedef enum 
	{
		TextEditAlignCenter = 0,
		TextEditAlignLeft,
		TextEditAlignRight
	}TextFieldAlign;
public:
    CATextFieldX();
    
    virtual ~CATextFieldX();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    virtual bool resignFirstResponder();
    
    virtual bool becomeFirstResponder();
    
    static CATextFieldX* createWithFrame(const DRect& frame);
    
    static CATextFieldX* createWithCenter(const DRect& rect);
    
    bool init();
    
	//delegate
	CC_SYNTHESIZE(CATextFieldDelegateX*, m_pDelegate, Delegate);

    //edit clearButton default:ClearButtonNone
    //MarginsRight set,clearbutton will be clear
    CC_PROPERTY_PASS_BY_REF(ClearButtonMode, m_clearBtn, ClearButtonMode);
    
    //textfield Margins    default:left 16,right 16
    CC_PROPERTY_PASS_BY_REF(int, m_marginLeft, MarginLeft);
    CC_PROPERTY_PASS_BY_REF(int, m_marginRight, MarginRight);
    
    //textfield Margins image
    void setMarginImageLeft(const DSize imgSize,const std::string& filePath);
    void setMarginImageRight(const DSize imgSize,const std::string& filePath);
    
    //textfield font size
    CC_PROPERTY_PASS_BY_REF(int, m_fontSize, FontSize);
    
    //splaceHolder  Text/Color
    CC_PROPERTY_PASS_BY_REF(std::string, m_placeHolderText, PlaceHolderText);
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_placeHdolderColor, PlaceHolderColor);
    
    //fieldText     Text/Color
    CC_PROPERTY_PASS_BY_REF(std::string, m_fieldText, FieldText);
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_fieldTextColor,FieldTextColor);
    
    //keyBoardType     default:KeyboardTypeDefault
    CC_PROPERTY_PASS_BY_REF(KeyboardType,m_keyBoardType,KeyboardType);
    
    //returnType       default:ReturnTypeDone
    CC_PROPERTY_PASS_BY_REF(ReturnType,m_returnType,ReturnType);
    
	//textFieldAlign  default:center
	CC_PROPERTY_PASS_BY_REF(TextFieldAlign,m_align,TextFieldAlign);

    //backGroundImage
    void setBackGroundImage(CAImage* image);
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

#endif /* CATextFieldX_hpp */
