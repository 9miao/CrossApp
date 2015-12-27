//
//  CATextFieldX.cpp
//  tesss
//
//  Created by 栗元峰 on 15/11/20.
//
//

#include "platform/CATextFieldX.h"
#include "platform/CADensityDpi.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#include "jni/JniHelper.h"
#include <jni.h>
#include "control/CAButton.h"

#define CLASS_NAME "org/CrossApp/lib/CrossAppTextField"
#define GET_CLASS "(I)Lorg/CrossApp/lib/CrossAppTextField;"

NS_CC_BEGIN

static std::map<int , CATextFieldX*> s_map;

void onCreateView(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "createTextField", "(I)V"))
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
            //jstring rtstr = jni.env->NewStringUTF(var);
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
        CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTextTag("textField"));
        imageView->setImage(image);
        imageView->setVisible(true);
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_hideImageView(JNIEnv *env, jclass cls, jint key)
    {
        CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTextTag("textField"));
        imageView->setVisible(false);
    }
    
    //textfield delegate
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_keyBoardHeightReturn(JNIEnv *env, jclass cls, jint key, jint height)
    {
        CATextFieldX* textField = s_map[(int)key];
        if (textField->getDelegate())
        {
            textField->getDelegate()->keyBoardHeight(textField, (int)s_px_to_dip(height));
        }
    }
    //return call back
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_keyBoardReturnCallBack(JNIEnv *env, jclass cls, jint key, jint height)
    {
        CATextFieldX* textField = s_map[(int)key];
        if (textField->getDelegate())
        {
            textField->getDelegate()->textFieldShouldReturn(textField);
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_textChange(JNIEnv *env, jclass cls, jint key, jstring before, jstring change, int arg0, int arg1, int arg2)
    {
        const char* charBefore = env->GetStringUTFChars(before, NULL);
        std::string strBefore = charBefore;
        env->ReleaseStringUTFChars(before, charBefore);
        const char* charChange = env->GetStringUTFChars(change, NULL);
        std::string strChange = charChange;
        env->ReleaseStringUTFChars(change, charChange);
        
        CATextFieldX* textField = s_map[(int)key];
        if (textField->getDelegate())
        {
            textField->getDelegate()->textFieldAfterTextChanged(textField,strBefore.c_str(), strChange.c_str(), arg0, arg1, arg2);
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_resignFirstResponder(JNIEnv *env, jclass cls, jint key)
    {
        CATextFieldX* textField = s_map[(int)key];
        textField->resignFirstResponder();
    }
}


CATextFieldX::CATextFieldX()
:m_pBackgroundView(NULL)
,m_pImgeView(NULL)
,m_pTextField(NULL)
,m_bUpdateImage(true)
,m_marginLeft(10)
,m_marginRight(10)
,m_fontSize(24)
,m_iMaxLenght(0)
,m_pDelegate(NULL)
,m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    s_map[m_u__ID] = this;
    this->setHaveNextResponder(false);
}

CATextFieldX::~CATextFieldX()
{
    s_map.erase(m_u__ID);
}

void CATextFieldX::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextFieldX::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
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

bool CATextFieldX::resignFirstResponder()
{
	if (m_pDelegate && (!m_pDelegate->textFieldShouldEndEditing(this)))
	{
		return false;
	}

    bool result = CAView::resignFirstResponder();

    resignFirstResponderID(m_u__ID);
    
    this->hideNativeTextField();

    if (m_clearBtn == ClearButtonMode::ClearButtonWhileEditing)
    {
        CAImageView* ima = (CAImageView*)this->getSubviewByTag(1011);
        ima->setImage(NULL);
    }

    return result;
}

bool CATextFieldX::becomeFirstResponder()
{
	if (m_pDelegate &&( !m_pDelegate->textFieldShouldBeginEditing(this)))
	{
		return false;
	}

	bool result = CAView::becomeFirstResponder();

	becomeFirstResponderID(m_u__ID);

	this->showNativeTextField();

	if (m_clearBtn == ClearButtonMode::ClearButtonWhileEditing)
	{
        CAImageView* ima = (CAImageView*)this->getSubviewByTag(1011);
        ima->setImage(CAImage::create("source_material/clear_button.png"));
	}
    return result;
}

void CATextFieldX::hideTextField()
{
//    m_pImgeView->setVisible(false);
}

void CATextFieldX::showTextField()
{
//    m_pImgeView->setVisible(true);
}

void CATextFieldX::hideNativeTextField()
{
    CAScheduler::unschedule(schedule_selector(CATextFieldX::update), this);
}

void CATextFieldX::showNativeTextField()
{
    this->update(0);
    CAScheduler::schedule(schedule_selector(CATextFieldX::update), this, 1/60.0f);
}

void CATextFieldX::delayShowImage()
{
    if (!CAViewAnimation::areBeginAnimationsWithID(m_s__StrID + "showImage"))
    {
        CAViewAnimation::beginAnimations(m_s__StrID + "showImage", NULL);
        CAViewAnimation::setAnimationDuration(0);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextFieldX::showImage));
        CAViewAnimation::commitAnimations();
    }
}

void CATextFieldX::showImage()
{
    getTextFieldImageJNI(m_u__ID);
    
    m_pImgeView->setFrame(this->getBounds());
}

CATextFieldX* CATextFieldX::createWithFrame(const DRect& frame)
{
    CATextFieldX *textField = new CATextFieldX();
    if (textField && textField->initWithFrame(frame))
    {
        textField->autorelease();
        return textField;
    }
    CC_SAFE_DELETE(textField);
    return NULL;
}

CATextFieldX* CATextFieldX::createWithCenter(const DRect& rect)
{
    CATextFieldX* textField = new CATextFieldX();
    
    if (textField && textField->initWithCenter(rect))
    {
        textField->autorelease();
        return textField;
    }
    
    CC_SAFE_DELETE(textField);
    return NULL;
}

bool CATextFieldX::init()
{
    onCreateView(m_u__ID);
    
    CAImage* image = CAImage::create("source_material/textField_bg.png");
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
    m_pBackgroundView = CAScale9ImageView::createWithImage(image);
    m_pBackgroundView->setCapInsets(capInsets);
    this->insertSubview(m_pBackgroundView, -1);

	m_pImgeView = CAImageView::createWithFrame(DRect(0, 0, 1, 1));
	this->addSubview(m_pImgeView);
    m_pImgeView->setTextTag("textField");
    
    return true;
}

void CATextFieldX::update(float dt)
{
    do
    {
        CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);
        
        CC_BREAK_IF(m_obLastPoint.equals(point));

        point.x = s_dip_to_px(point.x);
        point.y = s_dip_to_px(point.y);
        setTextFieldPointJNI(m_u__ID, point.x, point.y);
    }
    while (0);
}

void CATextFieldX::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
    
    DSize worldContentSize = DSizeApplyAffineTransform(m_obContentSize, worldToNodeTransform());
    
    DSize size;
    size.width = s_dip_to_px(worldContentSize.width);
    size.height =  s_dip_to_px(worldContentSize.height);
    setTextFieldSizeJNI(m_u__ID, size.width, size.height);

    m_pBackgroundView->setFrame(this->getBounds());
    m_pImgeView->setFrame(this->getBounds());
}

bool CATextFieldX::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    
    return true;
}

void CATextFieldX::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CATextFieldX::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
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

void CATextFieldX::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}

//
void CATextFieldX::setClearButtonMode(const ClearButtonMode& var)
{
	m_clearBtn = var;
    setMarginImageRight(DSize(m_obContentSize.height, m_obContentSize.height), "");
    showClearButtonJNI(m_u__ID);
}

const CATextFieldX::ClearButtonMode& CATextFieldX::getClearButtonMode()
{
	return m_clearBtn;
}

void CATextFieldX::setMarginLeft(const int& var)
{
	m_marginLeft = var;

	DSize worldContentSize = DSizeApplyAffineTransform(DSize(m_marginLeft,0), worldToNodeTransform());
	float x = s_dip_to_px(worldContentSize.width);

	worldContentSize = DSizeApplyAffineTransform(DSize(m_marginRight, 0), worldToNodeTransform());
	float y = s_dip_to_px(worldContentSize.width);

	setMarginJNI(m_u__ID, x, y);

    this->delayShowImage();
}

const int& CATextFieldX::getMarginLeft()
{
	return m_marginLeft;
}

void CATextFieldX::setMarginRight(const int& var)
{
	m_marginRight= var;

	DSize worldContentSize = DSizeApplyAffineTransform(DSize(m_marginLeft, 0), worldToNodeTransform());
	float x = s_dip_to_px(worldContentSize.width);

	worldContentSize = DSizeApplyAffineTransform(DSize(m_marginRight, 0), worldToNodeTransform());
	float y = s_dip_to_px(worldContentSize.width);

	setMarginJNI(m_u__ID, x, y);

    this->delayShowImage();
}

const int& CATextFieldX::getMarginRight()
{
	return m_marginRight;
}

void CATextFieldX::setMarginImageLeft(const DSize imgSize, const std::string& filePath)
{
	//set margins
	setMarginLeft(imgSize.width);

	//setimage
	CAImageView* ima = (CAImageView*)this->getSubviewByTag(1010);
	if (!ima)
	{
		ima = CAImageView::create();
		ima->setTag(1010);
		this->addSubview(ima);
	}
	ima->setCenter(DRect(imgSize.width / 2, getBounds().size.height / 2, imgSize.width, imgSize.height));
	ima->setImage(CAImage::create(filePath));
}

void CATextFieldX::setMarginImageRight(const DSize imgSize, const std::string& filePath)
{
	//set margins
	setMarginRight(imgSize.width);

	//setimage
	//setimage
	CAImageView* ima = (CAImageView*)this->getSubviewByTag(1011);
	if (!ima)
	{
		ima = CAImageView::create();
		ima->setTag(1011);
		this->addSubview(ima);
	}
	ima->setCenter(DRect(getBounds().size.width - imgSize.width / 2, getBounds().size.height / 2, imgSize.width, imgSize.height));
	ima->setImage(CAImage::create(filePath));
}

void CATextFieldX::setFontSize(int var)
{
	m_fontSize = var;
	setFontSizeJNI(m_u__ID,var);

    this->delayShowImage();
}

int CATextFieldX::getFontSize(){
	return m_fontSize;
}

void CATextFieldX::setPlaceHolderText(const std::string& var)
{
	m_placeHolderText = var;
	setPlaceHolderTextJNI(m_u__ID, var);

    this->delayShowImage();
}

const std::string& CATextFieldX::getPlaceHolderText()
{
	return m_placeHolderText;
}

void CATextFieldX::setPlaceHolderColor(const CAColor4B& var)
{
	m_placeHdolderColor = var;
	setPlaceHolderColorJNI(m_u__ID, getUIntFormColor4B(var));

    this->delayShowImage();
}

const CAColor4B& CATextFieldX::getPlaceHolderColor()
{
	return m_placeHdolderColor;
}

void CATextFieldX::setText(const std::string& var)
{
	m_sText = var;
	setTextJNI(m_u__ID, var);

    this->delayShowImage();
}

const std::string& CATextFieldX::getText()
{
	return m_sText;
}

void CATextFieldX::setTextColor(const CAColor4B& var)
{
	m_sTextColor = var;
	setTextColorJNI(m_u__ID, getUIntFormColor4B(var));

    this->delayShowImage();
}

const CAColor4B& CATextFieldX::getTextColor()
{
	return m_sTextColor; 
}

void CATextFieldX::setKeyboardType(const KeyboardType& var)
{
	m_keyBoardType = var;
	setKeyboardTypeJNI(m_u__ID,(int)var);
}

const CATextFieldX::KeyboardType& CATextFieldX::getKeyboardType()
{
	return m_keyBoardType;
}

void CATextFieldX::setReturnType(const ReturnType& var)
{
	m_returnType = var;
	setReturnTypeJNI(m_u__ID, (int)var);
}

const CATextFieldX::ReturnType& CATextFieldX::getReturnType()
{
	return m_returnType;
}

void CATextFieldX::setBackGroundImage(CAImage* image)
{
    if (image)
    {
        DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
        m_pBackgroundView->setCapInsets(capInsets);
    }
    m_pBackgroundView->setImage(image);
}

void CATextFieldX::setTextFieldAlign(const TextFieldAlign& var)
{
    m_align = var;
    
    setTextFieldAlignJNI(m_u__ID,(int)var);
    
    this->delayShowImage();
}

const CATextFieldX::TextFieldAlign& CATextFieldX::getTextFieldAlign()
{
    return m_align;
}

void CATextFieldX::setMaxLenght(int var)
{
    m_iMaxLenght = var;
    setMaxLenghtJNI(m_u__ID, var);
}

int CATextFieldX::getMaxLenght()
{
    return m_iMaxLenght;
}

void CATextFieldX::clearBtnCallBack(CAControl* con, DPoint point)
{
	setText("");
}


NS_CC_END

