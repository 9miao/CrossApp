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

#define CLASS_NAME "org/CrossApp/lib/CrossAppTextField"
#define GET_CLASS "(I)Lorg/CrossApp/lib/CrossAppTextField;"

NS_CC_BEGIN

static std::map<int , CAImageView*> s_map;

extern "C"
{
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_onByte(JNIEnv *env, jclass cls, jint key, jbyteArray buf, jint width, jint height)
    {
        unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
        env->GetByteArrayRegion(buf, 0, width * height * 4, (jbyte *)data);
        CAImage* image = CAImage::createWithRawDataNoCache(data, CAImage::PixelFormat_RGBA8888, width, height);
        s_map[(int)key]->setImage(image);
    }
}

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

CATextFieldX::CATextFieldX()
:m_pImgeView(NULL)
,m_pTextField(NULL)
{
    this->setHaveNextResponder(false);
}

CATextFieldX::~CATextFieldX()
{
    s_map.erase(m_u__ID);
}

void CATextFieldX::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0);
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextFieldX::showImageView));
    CAViewAnimation::commitAnimations();
    
}

void CATextFieldX::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

bool CATextFieldX::resignFirstResponder()
{
    bool result = CAView::resignFirstResponder();

    resignFirstResponderID(m_u__ID);
    this->showImageView();
    
    m_pImgeView->setVisible(true);
    
    this->hideNativeTextField();
    
    return result;
}

bool CATextFieldX::becomeFirstResponder()
{
    bool result = CAView::becomeFirstResponder();
    
    m_pImgeView->setVisible(false);
    
    becomeFirstResponderID(m_u__ID);
    
    this->showNativeTextField();
    return result;
}

void CATextFieldX::hideNativeTextField()
{
    CAScheduler::unschedule(schedule_selector(CATextFieldX::update), this);
    
    this->hide();
}

void CATextFieldX::hide()
{
    setTextFieldPointJNI(m_u__ID, -1000, -1000);
}

void CATextFieldX::showNativeTextField()
{
    this->update(0);
    CAScheduler::schedule(schedule_selector(CATextFieldX::update), this, 1/60.0f);
}

void CATextFieldX::showImageView()
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
    
    m_pImgeView = CAImageView::createWithFrame(DRect(0, 0, 1, 1));
    this->addSubview(m_pImgeView);
    
    s_map[m_u__ID] = m_pImgeView;
    return true;
}

void CATextFieldX::update(float dt)
{
    do
    {
        //CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);
        
        //CC_BREAK_IF(m_obLastPoint.equals(point) && m_obLastContentSize.equals(contentSize));

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
    
}

NS_CC_END

