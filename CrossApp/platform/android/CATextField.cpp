//
//  CATextField.cpp
//  tesss
//
//  Created by 栗元峰 on 15/11/20.
//
//

#include "platform/CATextField.h"
#include "platform/CADensityDpi.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#include "jni/JniHelper.h"
#include <jni.h>
#include "control/CAButton.h"

#define CLASS_NAME "org/CrossApp/lib/CrossAppTextField"
#define GET_CLASS "(I)Lorg/CrossApp/lib/CrossAppTextField;"
#define CAColorToJavaColor(color) (color.b + color.g * 0x100 + color.r * 0x10000 + color.a * 0x1000000)
NS_CC_BEGIN

static std::map<int , CATextField*> s_map;
static bool s_lock = false;
void onCreateView(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "createTextField", "(I)V"))
    {
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, key);
        jni.env->DeleteLocalRef(jni.classID);
    }
}

void onRemoveView(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "removeTextField", "(I)V"))
    {
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, key);
        jni.env->DeleteLocalRef(jni.classID);
    }
}

void setTextFieldPointJNI(int key, const int x, const int y)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setTextFieldPoint", "(II)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, x, y);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void setTextFieldSizeJNI(int key, const int width, const int height)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setTextFieldSize", "(II)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, width, height);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}
    
void getTextFieldImageJNI(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "getImage", "()V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}
    
void becomeFirstResponderID(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "becomeFirstResponder", "()V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void resignFirstResponderID(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "resignFirstResponder", "()V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}


void setMarginJNI(int key, int left, int right, int top = 0, int bottom = 0)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setMarginsDis", "(IIII)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, left, right, top, bottom);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void setFontSizeJNI(int key, int size)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setFontSize", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, size);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void setPlaceHolderTextJNI(int key, const std::string& var)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setTextFieldPlacHolder", "(Ljava/lang/String;)V"))
        {
            jstring jFilePath = jni.env->NewStringUTF(var.c_str());
            jni.env->CallVoidMethod(obj, jni.methodID, jFilePath);
            jni.env->DeleteLocalRef(jFilePath);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void setPlaceHolderColorJNI(int key, int color)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setTextFieldPlacHolderColor", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, color);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void setTextJNI(int key, const std::string& var)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setTextFieldText", "(Ljava/lang/String;)V"))
        {
            jstring jFilePath = jni.env->NewStringUTF(var.c_str());
            jni.env->CallVoidMethod(obj, jni.methodID, jFilePath);
            jni.env->DeleteLocalRef(jFilePath);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void setTextColorJNI(int key, int color)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setTextFieldTextColor", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, color);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void setKeyboardTypeJNI(int key, int type)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setKeyBoardType", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, type);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void setReturnTypeJNI(int key, int type)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setKeyBoardReturnType", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, type);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void setTextFieldAlignJNI(int key, int type)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setTextFieldAlgin", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, type);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void setSecureTextEntryJNI(int key, int var)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setSecureTextEntry", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, var);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void setMaxLenghtJNI(int key, int type)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setMaxLenght", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, type);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}


extern "C"
{
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_onByte(JNIEnv *env, jclass cls, jint key, jbyteArray buf, jint width, jint height)
    {
        unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
        env->GetByteArrayRegion(buf, 0, width * height * 4, (jbyte *)data);
        CAImage* image = CAImage::createWithRawDataNoCache(data, CAImage::PixelFormat_RGBA8888, width, height);
        CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTag(0xbcda));
        imageView->setImage(image);
        imageView->setVisible(true);
        free(data);
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_hideImageView(JNIEnv *env, jclass cls, jint key)
    {
        CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTag(0xbcda));
        if (imageView)
        {
            imageView->setVisible(false);
        }
    }
    
    //textfield delegate
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_keyBoardHeightReturn(JNIEnv *env, jclass cls, jint key, jint height)
    {
        CATextField* textField = s_map[(int)key];
        if (textField->getDelegate())
        {
            textField->getDelegate()->keyBoardHeight(textField, (int)s_px_to_dip(height));
        }
    }
    //return call back
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_keyBoardReturnCallBack(JNIEnv *env, jclass cls, jint key, jint height)
    {
        CATextField* textField = s_map[(int)key];
        
        if (textField->isAllowkeyBoardHide())
        {
            textField->resignFirstResponder();
        }
        
        if (textField->getDelegate())
        {
            textField->getDelegate()->textFieldShouldReturn(textField);
        }
    }
    
    JNIEXPORT bool JNICALL Java_org_CrossApp_lib_CrossAppTextField_textChange(JNIEnv *env, jclass cls, jint key, jstring before, jstring change, int arg0, int arg1)
    {
        const char* charBefore = env->GetStringUTFChars(before, NULL);
        std::string strBefore = charBefore;
        env->ReleaseStringUTFChars(before, charBefore);
        const char* charChange = env->GetStringUTFChars(change, NULL);
        std::string strChange = charChange;
        env->ReleaseStringUTFChars(change, charChange);
        
        CATextField* textField = s_map[(int)key];
        if (textField->getDelegate())
        {
			return textField->getDelegate()->textFieldShouldChangeCharacters(textField, arg0, arg1, strChange.c_str());
        }

		return true;
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_text(JNIEnv *env, jclass cls, jint key, jbyteArray textBuffer, int lenght)
    {
        char* buffer = (char*)malloc(sizeof(char) * lenght);
        env->GetByteArrayRegion(textBuffer, 0, lenght, (jbyte *)buffer);
        
        std::string text;
        text.resize(lenght);
        for (int i=0; i<lenght; i++)
        {
            text[i] = buffer[i];
        }
        
        s_lock = true;
        CATextField* textField = s_map[(int)key];
        textField->setText(text);
        s_lock = false;
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_resignFirstResponder(JNIEnv *env, jclass cls, jint key)
    {
        CATextField* textField = s_map[(int)key];
        if (textField->isFirstResponder())
        {
            textField->resignFirstResponder();
        }
    }
}


CATextField::CATextField()
: m_pBackgroundView(NULL)
, m_pImgeView(NULL)
, m_pTextField(NULL)
, m_pDelegate(NULL)
, m_bUpdateImage(true)
, m_bSecureTextEntry(false)
, m_bAllowkeyBoardHide(true)
, m_iMarginLeft(10)
, m_iMarginRight(10)
, m_iFontSize(40)
, m_iMaxLenght(0)
, m_eClearBtn(None)
, m_eAlign(Left)
, m_eReturnType(Done)
, m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    s_map[m_u__ID] = this;
    this->setHaveNextResponder(false);
    onCreateView(m_u__ID);
    this->setPlaceHolderText("");
    setFontSizeJNI(m_u__ID, m_iFontSize / 2);
}

CATextField::~CATextField()
{
    s_map.erase(m_u__ID);
    onRemoveView(m_u__ID);
}

void CATextField::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextField::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
    
    if (this->isFirstResponder())
    {
        this->resignFirstResponder();
    }
}

void showClearButtonJNI(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "showClearButton", "()V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

bool CATextField::resignFirstResponder()
{
	if (m_pDelegate && (!m_pDelegate->textFieldShouldEndEditing(this)))
	{
		return false;
	}

    bool result = CAView::resignFirstResponder();

    resignFirstResponderID(m_u__ID);
    
    this->hideNativeTextField();

    if (m_eClearBtn == WhileEditing)
    {
        CAImageView* ima = (CAImageView*)this->getSubviewByTag(1011);
        ima->setImage(NULL);
    }

    return result;
}

bool CATextField::becomeFirstResponder()
{
	if (m_pDelegate &&( !m_pDelegate->textFieldShouldBeginEditing(this)))
	{
		return false;
	}

	bool result = CAView::becomeFirstResponder();

	becomeFirstResponderID(m_u__ID);

	this->showNativeTextField();

	if (m_eClearBtn == WhileEditing)
	{
        CAImageView* ima = (CAImageView*)this->getSubviewByTag(1011);
        ima->setImage(CAImage::create("source_material/clear_button.png"));
	}
    
    if (CAViewAnimation::areBeginAnimationsWithID(m_s__StrID + "showImage"))
    {
        CAViewAnimation::removeAnimations(m_s__StrID + "showImage");
    }
    
    return result;
}

void CATextField::hideTextField()
{
//    m_pImgeView->setVisible(false);
}

void CATextField::showTextField()
{
//    m_pImgeView->setVisible(true);
}

void CATextField::hideNativeTextField()
{
    CAScheduler::unschedule(schedule_selector(CATextField::update), this);
}

void CATextField::showNativeTextField()
{
    this->update(0);
    CAScheduler::schedule(schedule_selector(CATextField::update), this, 1/60.0f);
}

void CATextField::delayShowImage()
{
    if (!CAViewAnimation::areBeginAnimationsWithID(m_s__StrID + "showImage"))
    {
        CAViewAnimation::beginAnimations(m_s__StrID + "showImage", NULL);
        CAViewAnimation::setAnimationDuration(0);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextField::showImage));
        CAViewAnimation::commitAnimations();
    }
}

void CATextField::showImage()
{
    getTextFieldImageJNI(m_u__ID);
}

CATextField* CATextField::createWithFrame(const DRect& frame)
{
    CATextField *textField = new CATextField();
    if (textField && textField->initWithFrame(frame))
    {
        textField->autorelease();
        return textField;
    }
    CC_SAFE_DELETE(textField);
    return NULL;
}

CATextField* CATextField::createWithCenter(const DRect& rect)
{
    CATextField* textField = new CATextField();
    
    if (textField && textField->initWithCenter(rect))
    {
        textField->autorelease();
        return textField;
    }
    
    CC_SAFE_DELETE(textField);
    return NULL;
}

CATextField* CATextField::createWithLayout(const DRectLayout& layout)
{
    CATextField* textField = new CATextField();
    if (textField && textField->initWithLayout(layout))
    {
        textField->autorelease();
        return textField;
    }
    CC_SAFE_DELETE(textField);
    return NULL;
}

bool CATextField::init()
{
    CAImage* image = CAImage::create("source_material/textField_bg.png");
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
    m_pBackgroundView = CAScale9ImageView::createWithImage(image);
    m_pBackgroundView->setLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B));
    m_pBackgroundView->setCapInsets(capInsets);
    this->insertSubview(m_pBackgroundView, -1);
    
    m_pImgeView = CAImageView::createWithLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B));
	this->addSubview(m_pImgeView);
    m_pImgeView->setTag(0xbcda);
    
    return true;
}

void CATextField::update(float dt)
{
    do
    {
        //CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);
        
        point.x = s_dip_to_px(point.x);
        point.y = s_dip_to_px(point.y);
        setTextFieldPointJNI(m_u__ID, point.x, point.y);
    }
    while (0);
}

void CATextField::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
    
    DSize worldContentSize = this->convertToWorldSize(m_obContentSize);
    
    DSize size;
    size.width = s_dip_to_px(worldContentSize.width);
    size.height =  s_dip_to_px(worldContentSize.height);
    setTextFieldSizeJNI(m_u__ID, size.width, size.height);
}

bool CATextField::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    
    return true;
}

void CATextField::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CATextField::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    DPoint point = this->convertTouchToNodeSpace(pTouch);
    
    if (this->getBounds().containsPoint(point))
    {
        becomeFirstResponder();
    }
    else
    {
        resignFirstResponder();
    }
}

void CATextField::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}

//
void CATextField::setClearButtonMode(const ClearButtonMode& var)
{
    setMarginImageRight(DSize(m_obContentSize.height, m_obContentSize.height), "");
    showClearButtonJNI(m_u__ID);
    m_eClearBtn = var;
}

const CATextField::ClearButtonMode& CATextField::getClearButtonMode()
{
	return m_eClearBtn;
}

void CATextField::setMarginLeft(int var)
{
	m_iMarginLeft = var;

	DSize worldContentSize = this->convertToWorldSize(DSize(m_iMarginLeft, 0));
	float x = s_dip_to_px(worldContentSize.width);

	worldContentSize = this->convertToWorldSize(DSize(m_iMarginRight, 0));
	float y = s_dip_to_px(worldContentSize.width);

	setMarginJNI(m_u__ID, x, y);

    this->delayShowImage();
}

int CATextField::getMarginLeft()
{
	return m_iMarginLeft;
}

void CATextField::setMarginRight(int var)
{
    if (m_eClearBtn == None)
    {
        m_iMarginRight = var;
        
        DSize worldContentSize = this->convertToWorldSize(DSize(m_iMarginLeft, 0));
        float x = s_dip_to_px(worldContentSize.width);
        
        worldContentSize = this->convertToWorldSize(DSize(m_iMarginRight, 0));
        float y = s_dip_to_px(worldContentSize.width);
        
        setMarginJNI(m_u__ID, x, y);
        
        this->delayShowImage();
    }
}

int CATextField::getMarginRight()
{
	return m_iMarginRight;
}

void CATextField::setMarginImageLeft(const DSize& imgSize, const std::string& filePath)
{
	//set margins
	setMarginLeft(imgSize.width);

	//setimage
	CAImageView* leftMarginView = (CAImageView*)this->getSubviewByTag(1010);
	if (!leftMarginView)
	{
		leftMarginView = CAImageView::create();
		leftMarginView->setTag(1010);
		this->addSubview(leftMarginView);
	}
	leftMarginView->setLayout(DRectLayout(0, imgSize.width, 0, 0, DRectLayout::L_W_T_B));
	leftMarginView->setImage(CAImage::create(filePath));
}

void CATextField::setMarginImageRight(const DSize& imgSize, const std::string& filePath)
{
	//set margins
	setMarginRight(imgSize.width);

    if (m_eClearBtn == None)
    {
        //setimage
        CAImageView* rightMarginView = (CAImageView*)this->getSubviewByTag(1011);
        if (!rightMarginView)
        {
            rightMarginView = CAImageView::create();
            rightMarginView->setTag(1011);
            this->addSubview(rightMarginView);
        }
        rightMarginView->setLayout(DRectLayout(0, imgSize.width, 0, 0, DRectLayout::R_W_T_B));
        rightMarginView->setImage(CAImage::create(filePath));
    }
}

void CATextField::setFontSize(int var)
{
	m_iFontSize = var;

	setFontSizeJNI(m_u__ID, m_iFontSize / 2);

    this->delayShowImage();
}

int CATextField::getFontSize()
{
	return m_iFontSize;
}

void CATextField::setPlaceHolderText(const std::string& var)
{
	m_sPlaceHolderText = var;
	setPlaceHolderTextJNI(m_u__ID, var);

    this->delayShowImage();
}

const std::string& CATextField::getPlaceHolderText()
{
	return m_sPlaceHolderText;
}

void CATextField::setPlaceHolderColor(const CAColor4B& var)
{
	m_cPlaceHdolderColor = var;
	setPlaceHolderColorJNI(m_u__ID, CAColorToJavaColor(var));

    this->delayShowImage();
}

const CAColor4B& CATextField::getPlaceHolderColor()
{
	return m_cPlaceHdolderColor;
}

void CATextField::setText(const std::string& var)
{
	m_sText = var;
    if (s_lock == false)
    {
        setTextJNI(m_u__ID, var);
        this->delayShowImage();
    }
}

const std::string& CATextField::getText()
{
	return m_sText;
}

void CATextField::setTextColor(const CAColor4B& var)
{
	m_cTextColor = var;
	setTextColorJNI(m_u__ID, CAColorToJavaColor(var));

    this->delayShowImage();
}

const CAColor4B& CATextField::getTextColor()
{
	return m_cTextColor; 
}

void CATextField::setKeyboardType(const KeyboardType& var)
{
	m_eKeyBoardType = var;
	setKeyboardTypeJNI(m_u__ID,(int)var);
}

const CATextField::KeyboardType& CATextField::getKeyboardType()
{
	return m_eKeyBoardType;
}

void CATextField::setReturnType(const ReturnType& var)
{
	m_eReturnType = var;
	setReturnTypeJNI(m_u__ID, (int)var);
}

const CATextField::ReturnType& CATextField::getReturnType()
{
	return m_eReturnType;
}

void CATextField::setBackgroundImage(CAImage* image)
{
    if (image)
    {
        DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
        m_pBackgroundView->setCapInsets(capInsets);
    }
    m_pBackgroundView->setImage(image);
}

void CATextField::setTextFieldAlign(const TextFieldAlign& var)
{
    m_eAlign = var;
    
    setTextFieldAlignJNI(m_u__ID, (int)var);
    
    this->delayShowImage();
}

const CATextField::TextFieldAlign& CATextField::getTextFieldAlign()
{
    return m_eAlign;
}

void CATextField::setSecureTextEntry(bool var)
{
    m_bSecureTextEntry = var;
    if (m_bSecureTextEntry)
    {
        setSecureTextEntryJNI(m_u__ID, 1);
    }
    else
    {
        setSecureTextEntryJNI(m_u__ID, 0);
    }
}

bool CATextField::isSecureTextEntry()
{
    return m_bSecureTextEntry;
}


void CATextField::setMaxLenght(int var)
{
    m_iMaxLenght = var;
    setMaxLenghtJNI(m_u__ID, var);
}

int CATextField::getMaxLenght()
{
    return m_iMaxLenght;
}

void CATextField::clearBtnCallBack(CAControl* con, DPoint point)
{
	setText("");
}


NS_CC_END

