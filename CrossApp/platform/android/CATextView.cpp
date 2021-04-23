//
//  CAtextView.cpp
//  tesss
//
//  Created by 栗元峰 on 15/11/20.
//
//

#include "platform/CATextView.h"
#include "platform/CADensityDpi.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#include "jni/JniHelper.h"
#include <jni.h>
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
#define CLASS_TEXTVIEW "org/CrossApp/lib/CrossAppTextView"
#define GET_CLASS "(I)Lorg/CrossApp/lib/CrossAppTextView;"
#define CAColorToJavaColor(color) (color.b + color.g * 0x100 + color.r * 0x10000 + color.a * 0x1000000)


NS_CC_BEGIN

static std::map<CrossApp::CATextView*, std::function<bool()> > s_ShouldBeginEditing_map;
static std::map<CrossApp::CATextView*, std::function<bool()> > s_ShouldEndEditing_map;
static std::map<CrossApp::CATextView*, std::function<void()> > s_ShouldReturn_map;
static std::map<CrossApp::CATextView*, std::function<void(int height)> > s_KeyBoardHeight_map;
static std::map<CrossApp::CATextView*, std::function<bool(ssize_t, ssize_t, const std::string&)> > s_ShouldChangeCharacters_map;
static std::map<CrossApp::CATextView*, std::function<void()> > s_DidChangeText_map;

static std::map<int, CATextView*> s_map;
static bool s_lock = false;
void textViewOnCreateView(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "createTextView", "(I)V"))
    {
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, key);
        jni.env->DeleteLocalRef(jni.classID);
    }
}

void textViewOnRemoveView(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "removeTextView", "(I)V"))
    {
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, key);
        jni.env->DeleteLocalRef(jni.classID);
    }
}

void textViewSetTextViewPointJNI(int key, const int x, const int y)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "getTextView", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_TEXTVIEW, "setTextViewPoint", "(II)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, x, y);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void textViewSetTextViewSizeJNI(int key, const int width, const int height)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "getTextView", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_TEXTVIEW, "setTextViewSize", "(II)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, width, height);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}
    
void textViewGetTextViewImageJNI(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "getTextView", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_TEXTVIEW, "getImage", "()V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}
    
void textViewBecomeFirstResponderID(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "getTextView", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_TEXTVIEW, "becomeFirstResponder", "()V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void textViewResignFirstResponderID(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "getTextView", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_TEXTVIEW, "resignFirstResponder", "()V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}




void textViewSetFontSizeJNI(int key, int size)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "getTextView", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_TEXTVIEW, "setFontSize", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, size);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void textViewSetTextJNI(int key, const std::string& var)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "getTextView", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_TEXTVIEW, "setTextViewText", "(Ljava/lang/String;)V"))
        {
            jstring jFilePath = jni.env->NewStringUTF(var.c_str());
            jni.env->CallVoidMethod(obj, jni.methodID, jFilePath);
            jni.env->DeleteLocalRef(jFilePath);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void textViewSetTextColorJNI(int key, int color)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "getTextView", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_TEXTVIEW, "setTextViewTextColor", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, color);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void textViewSetReturnTypeJNI(int key, int type)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "getTextView", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_TEXTVIEW, "setKeyBoardReturnType", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, type);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

void textViewsetAlignJNI(int key, int type)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, CLASS_TEXTVIEW, "getTextView", GET_CLASS))
    {
        jobject obj = jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, key);
        
        if (JniHelper::getMethodInfo(jni, CLASS_TEXTVIEW, "setTextViewAlgin", "(I)V"))
        {
            jni.env->CallVoidMethod(obj, jni.methodID, type);
            jni.env->DeleteLocalRef(jni.classID);
        }
    }
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_showImageView(JNIEnv *env, jclass cls, jint key)
    {
        if (CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTag(0xbcda)))
        {
            imageView->setVisible(true);
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_hideImageView(JNIEnv *env, jclass cls, jint key)
    {
        if (CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTag(0xbcda)))
        {
            imageView->setVisible(false);
        }
    }
    
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_onByte(JNIEnv *env, jclass cls, jint key, jbyteArray buf, jint width, jint height)
    {
        ssize_t length = width * height * 4;
        unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * length);
        env->GetByteArrayRegion(buf, 0, width * height * 4, (jbyte *)data);
        
        CrossApp::CAData* ca_data = CrossApp::CAData::create();
        ca_data->fastSet(data, length);
        
        CAImage* image = CAImage::createWithRawDataNoCache(ca_data, CAImage::PixelFormat::RGBA8888, width, height);
        
        CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTag(0xbcda));
        imageView->setImage(image);
    }
    
    //textView delegate
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_keyBoardHeightReturn(JNIEnv *env, jclass cls, jint key, jint height)
    {
		CATextView* textView = s_map[(int)key];
        if (s_KeyBoardHeight_map.count(textView) > 0 && s_KeyBoardHeight_map[textView])
        {
            s_KeyBoardHeight_map[textView]((int)s_px_to_dip(height));
        }
        else if (textView->getDelegate())
        {
            textView->getDelegate()->keyBoardHeight(textView, (int)s_px_to_dip(height));
        }
    }
    //return call back
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_keyBoardReturnCallBack(JNIEnv *env, jclass cls, jint key, jint height)
    {
        CATextView* textView = s_map[(int)key];
        
        textView->resignFirstResponder();
        if (s_ShouldReturn_map.count(textView) > 0 && s_ShouldReturn_map[textView])
        {
            s_ShouldReturn_map[textView]();
        }
        else if (textView->getDelegate())
        {
            textView->getDelegate()->textViewShouldReturn(textView);
        }
    }
    
	JNIEXPORT bool JNICALL Java_org_CrossApp_lib_CrossAppTextView_textChange(JNIEnv *env, jclass cls, jint key, jstring before, jstring change, int arg0, int arg1)
    {
        const char* charBefore = env->GetStringUTFChars(before, NULL);
        std::string strBefore = charBefore;
        env->ReleaseStringUTFChars(before, charBefore);
        const char* charChange = env->GetStringUTFChars(change, NULL);
        std::string strChange = charChange;
        env->ReleaseStringUTFChars(change, charChange);
        
		CATextView* textView = s_map[(int)key];
        if (s_ShouldChangeCharacters_map.count(textView) > 0 && s_ShouldChangeCharacters_map[textView])
        {
            return s_ShouldChangeCharacters_map[textView](arg0, arg1, strChange.c_str());
        }
        else if (textView->getDelegate())
        {
            return textView->getDelegate()->textViewShouldChangeCharacters(textView, arg0, arg1, strChange.c_str());
        }
		return true;
    }
    
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_text(JNIEnv *env, jclass cls, jint key, jbyteArray textBuffer, int length)
    {
        std::string text;
        text.resize(length);
        env->GetByteArrayRegion(textBuffer, 0, length, (jbyte *)&text[0]);
        
        s_lock = true;
        CATextView* textView = s_map[(int)key];
        textView->setText(text);
        s_lock = false;
    }
    
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_resignFirstResponder(JNIEnv *env, jclass cls, jint key)
    {
		CATextView* textView = s_map[(int)key];
        if (textView->isFirstResponder())
        {
            textView->resignFirstResponder();
        }
    }
}





CATextView::CATextView()
: m_pBackgroundView(nullptr)
, m_pShowImageView(nullptr)
, m_pTextView(nullptr)
, m_pDelegate(nullptr)
, m_iFontSize(40)
, m_eAlign(CATextView::Align::Left)
, m_eReturnType(CATextView::ReturnType::Default)
, m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    this->setHaveNextResponder(false);
    s_map[m_u__ID] = this;
	textViewOnCreateView(m_u__ID);
}

CATextView::~CATextView()
{
    s_ShouldBeginEditing_map.erase(this);
    s_ShouldEndEditing_map.erase(this);
    s_ShouldReturn_map.erase(this);
    s_KeyBoardHeight_map.erase(this);
    s_ShouldChangeCharacters_map.erase(this);
    s_DidChangeText_map.erase(this);
    
    s_map.erase(m_u__ID);
    textViewOnRemoveView(m_u__ID);
    m_pDelegate = nullptr;
}

void CATextView::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextView::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
    if (this->isFirstResponder())
    {
        this->resignFirstResponder();
    }
}

bool CATextView::resignFirstResponder()
{
    if (s_ShouldEndEditing_map.count(this) > 0 && s_ShouldEndEditing_map[this] && !s_ShouldEndEditing_map[this]())
    {
        return false;
    }
    else if (m_pDelegate && (!m_pDelegate->textViewShouldEndEditing(this)))
    {
        return false;
    }

    bool result = CAControl::resignFirstResponder();

    textViewResignFirstResponderID(m_u__ID);
    
    this->hideNativeTextView();

    return result;
}

bool CATextView::becomeFirstResponder()
{
    if (s_ShouldBeginEditing_map.count(this) > 0 && s_ShouldBeginEditing_map[this] && !s_ShouldBeginEditing_map[this]())
    {
        return false;
    }
    else if (m_pDelegate && (!m_pDelegate->textViewShouldBeginEditing(this)))
    {
        return false;
    }

	bool result = CAControl::becomeFirstResponder();

	textViewBecomeFirstResponderID(m_u__ID);

	this->showNativeTextView();

    if (CAViewAnimation::areBeginAnimationsWithID(m_s__StrID + "showImage"))
    {
        CAViewAnimation::removeAnimations(m_s__StrID + "showImage");
    }
    
    return result;
}

void CATextView::hideTextView()
{
//    m_pImgeView->setVisible(false);
}

void CATextView::showTextView()
{
//    m_pImgeView->setVisible(true);
}

void CATextView::hideNativeTextView()
{
    CAScheduler::getScheduler()->unschedule(schedule_selector(CATextView::update), this);
}

void CATextView::showNativeTextView()
{
    CAScheduler::getScheduler()->schedule(schedule_selector(CATextView::update), this, 1/60.0f);
}

void CATextView::delayShowImage()
{
    this->cancelPreviousPerformRequests(callfunc_selector(CATextView::showImage));
    this->performSelector(callfunc_selector(CATextView::showImage), 0.1);
}

void CATextView::showImage()
{
    textViewGetTextViewImageJNI(m_u__ID);
}

CATextView* CATextView::createWithFrame(const DRect& frame)
{
	CATextView *textView = new CATextView();
	if (textView && textView->initWithFrame(frame))
    {
		textView->autorelease();
		return textView;
    }
	CC_SAFE_DELETE(textView);
    return NULL;
}

CATextView* CATextView::createWithCenter(const DRect& rect)
{
	CATextView* textView = new CATextView();
    
	if (textView && textView->initWithCenter(rect))
    {
		textView->autorelease();
		return textView;
    }
    
	CC_SAFE_DELETE(textView);
    return NULL;
}

CATextView* CATextView::createWithLayout(const DLayout& layout)
{
    CATextView* textView = new CATextView();
    if (textView&&textView->initWithLayout(layout))
    {
        textView->autorelease();
        return textView;
    }
    
    CC_SAFE_RELEASE_NULL(textView);
    return NULL;
}

bool CATextView::init()
{
    const CAThemeManager::stringMap& map = CAApplication::getApplication()->getThemeManager()->getThemeMap("CATextField");
    CAImage* image = CAImage::create(map.at("backgroundView_normal"));
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);

	m_pBackgroundView = CAScale9ImageView::createWithImage(image);
    m_pBackgroundView->setLayout(DLayoutFill);
	m_pBackgroundView->setCapInsets(capInsets);
	this->insertSubview(m_pBackgroundView, -1);
    
	m_pShowImageView = CAImageView::createWithLayout(DLayoutFill);
	m_pShowImageView->setTag(0xbcda);
	this->addSubview(m_pShowImageView);
	
    return true;
}

void CATextView::update(float dt)
{
    do
    {
        //CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);
        
        point.x = s_dip_to_px(point.x);
        point.y = s_dip_to_px(point.y);
        textViewSetTextViewPointJNI(m_u__ID, point.x, point.y);
    }
    while (0);
}

void CATextView::setContentSize(const DSize& contentSize)
{
    CAControl::setContentSize(contentSize);
    
    DSize worldContentSize = this->convertToWorldSize(m_obContentSize);
    
    DSize size;
    size.width = s_dip_to_px(worldContentSize.width);
    size.height =  s_dip_to_px(worldContentSize.height);
    textViewSetTextViewSizeJNI(m_u__ID, size.width, size.height);
}

bool CATextView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    
    return true;
}

void CATextView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CATextView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
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

void CATextView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}


void CATextView::setFontSize(const int& var)
{
	m_iFontSize = var;

	textViewSetFontSizeJNI(m_u__ID, m_iFontSize / 2);

    this->delayShowImage();
}

const int& CATextView::getFontSize()
{
	return m_iFontSize;
}

void CATextView::setText(const std::string& var)
{
	m_sText = var;
    if (s_lock == false)
    {
        textViewSetTextJNI(m_u__ID, var);
        this->delayShowImage();
    }
}

const std::string& CATextView::getText()
{
	return m_sText;
}

void CATextView::setTextColor(const CAColor4B& var)
{
	m_sTextColor = var;
	textViewSetTextColorJNI(m_u__ID, CAColorToJavaColor(var));

    this->delayShowImage();
}

const CAColor4B& CATextView::getTextColor()
{
	return m_sTextColor; 
}

void CATextView::setReturnType(CATextView::ReturnType var)
{
    m_eReturnType = var;
    textViewSetReturnTypeJNI(m_u__ID, (int)var);
}

CATextView::ReturnType CATextView::getReturnType()
{
    return m_eReturnType;
}

void CATextView::setBackgroundImage(CAImage* image)
{
    if (image)
    {
        DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
        m_pBackgroundView->setCapInsets(capInsets);
    }
    m_pBackgroundView->setImage(image);
}

void CATextView::setAlign(CATextView::Align var)
{
    m_eAlign = var;
    
    textViewsetAlignJNI(m_u__ID, (int)var);
    
    this->delayShowImage();
}

CATextView::Align CATextView::getAlign()
{
    return m_eAlign;
}

void CATextView::onShouldBeginEditing(const std::function<bool ()> &var)
{
    m_obShouldBeginEditing = var;
    s_ShouldBeginEditing_map[this] = var;
}

void CATextView::onShouldEndEditing(const std::function<bool ()> &var)
{
    m_obShouldEndEditing = var;
    s_ShouldEndEditing_map[this] = var;
}

void CATextView::onShouldReturn(const std::function<void ()> &var)
{
    m_obShouldReturn = var;
    s_ShouldReturn_map[this] = var;
}

void CATextView::onKeyBoardHeight(const std::function<void (int)> &var)
{
    m_obKeyBoardHeight = var;
    s_KeyBoardHeight_map[this] = var;
}

void CATextView::onShouldChangeCharacters(const std::function<bool (ssize_t, ssize_t, const std::string &)> &var)
{
    m_obShouldChangeCharacters = var;
    s_ShouldChangeCharacters_map[this] = var;
}

void CATextView::onDidChangeText(const std::function<void ()> &var)
{
    m_obDidChangeText = var;
    s_DidChangeText_map[this] = var;
}

NS_CC_END

