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

#define CLASS_TEXTVIEW "org/CrossApp/lib/CrossAppTextView"
#define GET_CLASS "(I)Lorg/CrossApp/lib/CrossAppTextView;"
#define CAColorToJavaColor(color) (color.b + color.g * 0x100 + color.r * 0x10000 + color.a * 0x1000000)


NS_CC_BEGIN

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

void textViewSetTextViewAlignJNI(int key, int type)
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
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_onByte(JNIEnv *env, jclass cls, jint key, jbyteArray buf, jint width, jint height)
    {
        unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
        env->GetByteArrayRegion(buf, 0, width * height * 4, (jbyte *)data);
        CAImage* image = CAImage::createWithRawDataNoCache(data, CAImage::PixelFormat_RGBA8888, width, height);
        CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTextTag("textView"));
        imageView->setImage(image);
        imageView->setVisible(true);
        s_map[(int)key]->reViewlayout();
        CAApplication::getApplication()->updateDraw();
    }
    
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_hideImageView(JNIEnv *env, jclass cls, jint key)
    {
        CAImageView* imageView = (CAImageView*)(s_map[(int)key]->getSubviewByTextTag("textView"));
        imageView->setVisible(false);
    }
    
    //textView delegate
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_keyBoardHeightReturn(JNIEnv *env, jclass cls, jint key, jint height)
    {
		CATextView* textView = s_map[(int)key];
        if (textView->getDelegate())
        {
            textView->getDelegate()->keyBoardHeight(textView, (int)s_px_to_dip(height));
        }
    }
    //return call back
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_keyBoardReturnCallBack(JNIEnv *env, jclass cls, jint key, jint height)
    {
        CATextView* textView = s_map[(int)key];
        
        textView->resignFirstResponder();
        
        if (textView->getDelegate())
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
		if (textView->getDelegate())
		{
			return textView->getDelegate()->textViewShouldChangeCharacters(textView, arg0, arg1, strChange.c_str());
		}

		return true;
    }
    
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextView_text(JNIEnv *env, jclass cls, jint key, jbyteArray textBuffer, int lenght)
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
: m_pBackgroundView(NULL)
, m_pShowImageView(NULL)
, m_pTextView(NULL)
, m_iFontSize(40)
, m_pDelegate(NULL)
, m_eAlign(Left)
, m_eReturnType(Default)
, m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    s_map[m_u__ID] = this;
    this->setHaveNextResponder(false);
	textViewOnCreateView(m_u__ID);
    textViewSetFontSizeJNI(m_u__ID, m_iFontSize / 2);
}

CATextView::~CATextView()
{
    s_map.erase(m_u__ID);
    textViewOnRemoveView(m_u__ID);
}

void CATextView::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextView::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
    
    if (this->isFirstResponder())
    {
        this->resignFirstResponder();
    }
}

bool CATextView::resignFirstResponder()
{
	if (m_pDelegate && (!m_pDelegate->textViewShouldEndEditing(this)))
	{
		return false;
	}

    bool result = CAView::resignFirstResponder();

    textViewResignFirstResponderID(m_u__ID);
    
    this->hideNativeTextView();

    return result;
}

bool CATextView::becomeFirstResponder()
{
	if (m_pDelegate &&( !m_pDelegate->textViewShouldBeginEditing(this)))
	{
		return false;
	}

	bool result = CAView::becomeFirstResponder();

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
	CAScheduler::unschedule(schedule_selector(CATextView::update), this);
}

void CATextView::showNativeTextView()
{
    this->update(0);
	CAScheduler::schedule(schedule_selector(CATextView::update), this, 1 / 60.0f);
}

void CATextView::delayShowImage()
{
    if (!CAViewAnimation::areBeginAnimationsWithID(m_s__StrID + "showImage"))
    {
        CAViewAnimation::beginAnimations(m_s__StrID + "showImage", NULL);
        CAViewAnimation::setAnimationDuration(0);
		CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextView::showImage));
        CAViewAnimation::commitAnimations();
    }
}

void CATextView::showImage()
{
    textViewGetTextViewImageJNI(m_u__ID);
    
    m_pShowImageView->setFrame(this->getBounds());
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

bool CATextView::init()
{
    CAImage* image = CAImage::create("source_material/textField_bg.png");
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);

	m_pBackgroundView = CAScale9ImageView::createWithImage(image);
	m_pBackgroundView->setCapInsets(capInsets);
	this->insertSubview(m_pBackgroundView, -1);
    
	m_pShowImageView = CAImageView::createWithFrame(DRect(0, 0, 1, 1));
	m_pShowImageView->setTextTag("textView");
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
    CAView::setContentSize(contentSize);
    
    DSize worldContentSize = this->convertToWorldSize(m_obContentSize);
    
    DSize size;
    size.width = s_dip_to_px(worldContentSize.width);
    size.height =  s_dip_to_px(worldContentSize.height);
    textViewSetTextViewSizeJNI(m_u__ID, size.width, size.height);

	m_pBackgroundView->setFrame(this->getBounds());
    m_pShowImageView->setFrame(this->getBounds());
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


void CATextView::setTextFontSize(const int& var)
{
	m_iFontSize = var;

	textViewSetFontSizeJNI(m_u__ID, m_iFontSize / 2);

    this->delayShowImage();
}

const int& CATextView::getTextFontSize()
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

void CATextView::setReturnType(const ReturnType& var)
{
    m_eReturnType = var;
    textViewSetReturnTypeJNI(m_u__ID, (int)var);
}

const CATextView::ReturnType& CATextView::getReturnType()
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

void CATextView::setTextViewAlign(const TextViewAlign& var)
{
    m_eAlign = var;
    
    textViewSetTextViewAlignJNI(m_u__ID, (int)var);
    
    this->delayShowImage();
}

const CATextView::TextViewAlign& CATextView::getTextViewAlign()
{
    return m_eAlign;
}



NS_CC_END

