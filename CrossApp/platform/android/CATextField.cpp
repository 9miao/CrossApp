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
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
#define CLASS_NAME "org/CrossApp/lib/CrossAppTextField"
#define GET_CLASS "(I)Lorg/CrossApp/lib/CrossAppTextField;"
#define CAColorToJavaColor(color) (color.b + color.g * 0x100 + color.r * 0x10000 + color.a * 0x1000000)
NS_CC_BEGIN

static std::map<CrossApp::CATextField*, std::function<bool()> > s_ShouldBeginEditing_map;
static std::map<CrossApp::CATextField*, std::function<bool()> > s_ShouldEndEditing_map;
static std::map<CrossApp::CATextField*, std::function<void()> > s_ShouldReturn_map;
static std::map<CrossApp::CATextField*, std::function<void(int height)> > s_KeyBoardHeight_map;
static std::map<CrossApp::CATextField*, std::function<bool(ssize_t, ssize_t, const std::string&)> > s_ShouldChangeCharacters_map;
static std::map<CrossApp::CATextField*, std::function<void()> > s_DidChangeText_map;

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

void setAlignJNI(int key, int type)
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

void setMaxLengthJNI(int key, int type)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_NAME, "getTextField", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_NAME, "setMaxLength", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, type);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}


extern "C"
{
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_showImageView(JNIEnv *env, jclass cls, jint key)
    {
        if (CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTextTag("imageView")))
        {
            imageView->setVisible(true);
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_hideImageView(JNIEnv *env, jclass cls, jint key)
    {
        if (CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTextTag("imageView")))
        {
            imageView->setVisible(false);
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_onByte(JNIEnv *env, jclass cls, jint key, jbyteArray buf, jint width, jint height)
    {
        ssize_t length = width * height * 4;
        unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * length);
        env->GetByteArrayRegion(buf, 0, width * height * 4, (jbyte *)data);
        
        CrossApp::CAData* ca_data = CrossApp::CAData::create();
        ca_data->fastSet(data, length);
        
        CAImage* image = CAImage::createWithRawDataNoCache(ca_data, CAImage::PixelFormat::RGBA8888, width, height);
        
        CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTextTag("imageView"));
        imageView->setImage(image);
    }

    //textfield delegate
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_keyBoardHeightReturn(JNIEnv *env, jclass cls, jint key, jint height)
    {
        CATextField* textField = s_map[(int)key];
        
        if (s_KeyBoardHeight_map.count(textField) > 0 && s_KeyBoardHeight_map[textField])
        {
            s_KeyBoardHeight_map[textField]((int)s_px_to_dip(height));
        }
        else if (textField->getDelegate())
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
        
        if (s_ShouldReturn_map.count(textField) > 0 && s_ShouldReturn_map[textField])
        {
            s_ShouldReturn_map[textField]();
        }
        else if (textField->getDelegate())
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
        
        if (s_ShouldChangeCharacters_map.count(textField) > 0 && s_ShouldChangeCharacters_map[textField])
        {
            return s_ShouldChangeCharacters_map[textField](arg0, arg1, strChange.c_str());
        }
        else if (textField->getDelegate())
        {
            return textField->getDelegate()->textFieldShouldChangeCharacters(textField, arg0, arg1, strChange.c_str());
        }
        
		return true;
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_didTextChanged(JNIEnv *env, jclass cls, jint key)
    {
        CATextField* textField = s_map[(int)key];
        if (s_DidChangeText_map.count(textField) > 0 && s_DidChangeText_map[textField])
        {
            s_DidChangeText_map[textField]();
        }
        else if (textField->getDelegate())
        {
            textField->getDelegate()->textFieldDidChangeText(textField);
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_text(JNIEnv *env, jclass cls, jint key, jbyteArray textBuffer, int length)
    {
        std::string text;
        text.resize(length);
        env->GetByteArrayRegion(textBuffer, 0, length, (jbyte *)&text[0]);
        
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
: m_pBackgroundView(nullptr)
, m_pImgeView(nullptr)
, m_pTextField(nullptr)
, m_pDelegate(nullptr)
, m_bUpdateImage(true)
, m_bSecureTextEntry(false)
, m_bAllowkeyBoardHide(true)
, m_cTextColor(CAColor4B::BLACK)
, m_cPlaceHdolderColor(CAColor4B::GRAY)
, m_iMarginLeft(10)
, m_iMarginRight(10)
, m_iFontSize(40)
, m_iMaxLength(0)
, m_eClearBtn(CATextField::ClearButtonMode::None)
, m_eAlign(CATextField::Align::Left)
, m_eReturnType(CATextField::ReturnType::Done)
, m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    this->setHaveNextResponder(false);
    s_map[m_u__ID] = this;
    onCreateView(m_u__ID);
}

CATextField::~CATextField()
{
    s_ShouldBeginEditing_map.erase(this);
    s_ShouldEndEditing_map.erase(this);
    s_ShouldReturn_map.erase(this);
    s_KeyBoardHeight_map.erase(this);
    s_ShouldChangeCharacters_map.erase(this);
    s_DidChangeText_map.erase(this);
    
    s_map.erase(m_u__ID);
    onRemoveView(m_u__ID);
    m_pDelegate = nullptr;
}

void CATextField::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextField::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
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
    if (s_ShouldEndEditing_map.count(this) > 0 && s_ShouldEndEditing_map[this] && !s_ShouldEndEditing_map[this]())
    {
        return false;
    }
    else if (m_pDelegate && (!m_pDelegate->textFieldShouldEndEditing(this)))
    {
        return false;
    }

    bool result = CAControl::resignFirstResponder();

    resignFirstResponderID(m_u__ID);
    
    this->hideNativeTextField();
    
    if (m_eClearBtn == CATextField::ClearButtonMode::WhileEditing)
    {
        CAImageView* ima = (CAImageView*)this->getSubviewByTextTag("ImageRight");
        ima->setImage(nullptr);
    }

    return result;
}

bool CATextField::becomeFirstResponder()
{
    if (s_ShouldBeginEditing_map.count(this) > 0 && s_ShouldBeginEditing_map[this] && !s_ShouldBeginEditing_map[this]())
    {
        return false;
    }
    else if (m_pDelegate &&( !m_pDelegate->textFieldShouldBeginEditing(this)))
    {
        return false;
    }

	bool result = CAControl::becomeFirstResponder();

    becomeFirstResponderID(m_u__ID);
    
    this->showNativeTextField();
    
	if (m_eClearBtn == CATextField::ClearButtonMode::WhileEditing)
	{
        const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CATextField");
        CAImageView* ima = (CAImageView*)this->getSubviewByTextTag("ImageRight");
        ima->setImage(CAImage::create(map.at("clearImage")));
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
    CAScheduler::getScheduler()->unschedule(schedule_selector(CATextField::update), this);
}

void CATextField::showNativeTextField()
{
    CAScheduler::getScheduler()->schedule(schedule_selector(CATextField::update), this, 1/60.0f);
}

void CATextField::delayShowImage()
{
    this->cancelPreviousPerformRequests(callfunc_selector(CATextField::showImage));
    this->performSelector(callfunc_selector(CATextField::showImage), 0.1);
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

CATextField* CATextField::createWithLayout(const DLayout& layout)
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
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CATextField");
    CAImage* image = CAImage::create(map.at("backgroundView_normal"));
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
    m_pBackgroundView = CAScale9ImageView::createWithImage(image);
    m_pBackgroundView->setLayout(DLayoutFill);
    m_pBackgroundView->setCapInsets(capInsets);
    this->insertSubview(m_pBackgroundView, -1);
    
    m_pImgeView = CAImageView::createWithLayout(DLayoutFill);
	this->addSubview(m_pImgeView);
    m_pImgeView->setTextTag("imageView");
    
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
    DSize size = contentSize;
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CATextField");
    if (m_bRecSpe)
    {
        int h = atoi(map.at("height").c_str());
        size.height = (h == 0) ? size.height : h;
    }
    CAControl::setContentSize(size);
    
    if (m_eClearBtn == CATextField::ClearButtonMode::WhileEditing)
    {
        if (CAImageView* rightMarginView = dynamic_cast<CAImageView*>(this->getSubviewByTextTag("ImageRight")))
        {
            DLayout layout;
            layout.horizontal.right = 0;
            layout.horizontal.width = m_obContentSize.height;
            layout.vertical.height = m_obContentSize.height;
            layout.vertical.center = 0.5f;
            rightMarginView->setLayout(layout);
        }
    }
    
    DSize worldContentSize = this->convertToWorldSize(m_obContentSize);
    
    DSize nssize;
    nssize.width = s_dip_to_px(worldContentSize.width);
    nssize.height =  s_dip_to_px(worldContentSize.height);
    setTextFieldSizeJNI(m_u__ID, nssize.width, nssize.height);
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

void CATextField::setClearButtonMode(ClearButtonMode var)
{
    setMarginImageRight(DSize(m_obContentSize.height, m_obContentSize.height), "");
    showClearButtonJNI(m_u__ID);
    m_eClearBtn = var;
}

CATextField::ClearButtonMode CATextField::getClearButtonMode()
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
    if (m_eClearBtn == CATextField::ClearButtonMode::None)
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
	CAImageView* leftMarginView = (CAImageView*)this->getSubviewByTextTag("ImageLeft");
	if (!leftMarginView)
	{
		leftMarginView = CAImageView::create();
		leftMarginView->setTextTag("ImageLeft");
		this->addSubview(leftMarginView);
	}
    DLayout layout;
    layout.horizontal.left = 0;
    layout.horizontal.width = imgSize.width;
    layout.vertical.height = imgSize.height;
    layout.vertical.center = 0.5f;
    leftMarginView->setLayout(layout);
	leftMarginView->setImage(CAImage::create(filePath));
}

void CATextField::setMarginImageRight(const DSize& imgSize, const std::string& filePath)
{
	//set margins
	setMarginRight(imgSize.width);

    if (m_eClearBtn == CATextField::ClearButtonMode::None)
    {
        //setimage
        CAImageView* rightMarginView = (CAImageView*)this->getSubviewByTextTag("ImageRight");
        if (!rightMarginView)
        {
            rightMarginView = CAImageView::create();
            rightMarginView->setTextTag("ImageRight");
            this->addSubview(rightMarginView);
        }
        DLayout layout;
        layout.horizontal.right = 0;
        layout.horizontal.width = imgSize.width;
        layout.vertical.height = imgSize.height;
        layout.vertical.center = 0.5f;
        rightMarginView->setLayout(layout);
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

void CATextField::setKeyboardType(CATextField::KeyboardType var)
{
	m_eKeyBoardType = var;
	setKeyboardTypeJNI(m_u__ID,(int)var);
}

CATextField::KeyboardType CATextField::getKeyboardType()
{
	return m_eKeyBoardType;
}

void CATextField::setReturnType(CATextField::ReturnType var)
{
	m_eReturnType = var;
	setReturnTypeJNI(m_u__ID, (int)var);
}

CATextField::ReturnType CATextField::getReturnType()
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

void CATextField::setAlign(CATextField::Align var)
{
    m_eAlign = var;
    
    setAlignJNI(m_u__ID, (int)var);
    
    this->delayShowImage();
}

CATextField::Align CATextField::getAlign()
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
    this->delayShowImage();
}

bool CATextField::isSecureTextEntry()
{
    return m_bSecureTextEntry;
}


void CATextField::setMaxLength(int var)
{
    m_iMaxLength = var;
    setMaxLengthJNI(m_u__ID, var);
}

int CATextField::getMaxLength()
{
    return m_iMaxLength;
}

void CATextField::onShouldBeginEditing(const std::function<bool ()> &var)
{
    m_obShouldBeginEditing = var;
    s_ShouldBeginEditing_map[this] = var;
}

void CATextField::onShouldEndEditing(const std::function<bool ()> &var)
{
    m_obShouldEndEditing = var;
    s_ShouldEndEditing_map[this] = var;
}

void CATextField::onShouldReturn(const std::function<void ()> &var)
{
    m_obShouldReturn = var;
    s_ShouldReturn_map[this] = var;
}

void CATextField::onKeyBoardHeight(const std::function<void (int)> &var)
{
    m_obKeyBoardHeight = var;
    s_KeyBoardHeight_map[this] = var;
}

void CATextField::onShouldChangeCharacters(const std::function<bool (ssize_t, ssize_t, const std::string &)> &var)
{
    m_obShouldChangeCharacters = var;
    s_ShouldChangeCharacters_map[this] = var;
}

void CATextField::onDidChangeText(const std::function<void ()> &var)
{
    m_obDidChangeText = var;
    s_DidChangeText_map[this] = var;
}
NS_CC_END

