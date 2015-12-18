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

	//textfield delegate
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_keyBoardHeightReturn(JNIEnv *env, jclass cls, jint key, jint height)
	{
		CATextFieldX* textField = (CATextFieldX*)(s_map[(int)key]->getSuperview());
		if (textField->getDelegate())
		{
			textField->getDelegate()->keyBoardHeight(textField, (int)s_px_to_dip(height));
		}
	}
	//return call back
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_keyBoardReturnCallBack(JNIEnv *env, jclass cls, jint key, jint height)
	{
		CATextFieldX* textField = (CATextFieldX*)(s_map[(int)key]->getSuperview());
		if (textField->getDelegate())
		{
			textField->getDelegate()->textFieldShouldReturn(textField);
		}
	}
// 	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_keyBoardShow(JNIEnv *env, jclass cls, jint key)
// 	{
// 		CATextFieldX* textField = (CATextFieldX*)(s_map[(int)key]->getSuperview());
// 	}
// 	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppTextField_keyBoardHide(JNIEnv *env, jclass cls, jint key)
// 	{
// 		CATextFieldX* textField = (CATextFieldX*)(s_map[(int)key]->getSuperview());
// 	}
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
, m_bgImgeView(NULL)
, m_pDelegate(NULL)
, m_pDlayeShow(false)
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
    
	delayShowImageView();
//     CAViewAnimation::beginAnimations("", NULL);
//     CAViewAnimation::setAnimationDuration(0);
//     CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextFieldX::showImageView));
//     CAViewAnimation::commitAnimations();
    
}

void CATextFieldX::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

bool CATextFieldX::resignFirstResponder()
{
	if (m_pDelegate && (!m_pDelegate->textFieldShouldEndEditing(this)))
	{
		return false;
	}

    bool result = CAView::resignFirstResponder();

    resignFirstResponderID(m_u__ID);
    this->showImageView();
    
    m_pImgeView->setVisible(true);
    
    this->hideNativeTextField();
    
    return result;
}

bool CATextFieldX::becomeFirstResponder()
{
	if (m_pDelegate &&( !m_pDelegate->textFieldShouldBeginEditing(this)))
	{
		return false;
	}

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
	//m_bgImgeView->setFrame(this->getBounds());
	m_pDlayeShow = false;
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
	m_bgImgeView = CAScale9ImageView::createWithFrame(DRect(0, 0, 1, 1));
	m_bgImgeView->setCapInsets(DRect(image->getPixelsWide() / 2, image->getPixelsHigh() / 2, 1, 1));
	m_bgImgeView->setImage(image);
	this->addSubview(m_bgImgeView);

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
	m_bgImgeView->setFrame(this->getBounds());
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

//
void CATextFieldX::setClearButtonMode(const ClearButtonMode& var){
	m_clearBtn = var;

	
}
const CATextFieldX::ClearButtonMode& CATextFieldX::getClearButtonMode(){
	return m_clearBtn;
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

void CATextFieldX::setMarginLeft(const int& var){
	m_marginLeft = var;





	return;
	DSize worldContentSize = DSizeApplyAffineTransform(DSize(m_marginLeft,0), worldToNodeTransform());
	float x = s_dip_to_px(worldContentSize.width);

	worldContentSize = DSizeApplyAffineTransform(DSize(m_marginRight, 0), worldToNodeTransform());
	float y = s_dip_to_px(worldContentSize.width);

	setMarginJNI(m_u__ID, x, y);
}
const int& CATextFieldX::getMarginLeft(){
	return m_marginLeft;
}

void CATextFieldX::setMarginRight(const int& var){
	m_marginRight = var;



	return;
	setMarginJNI(m_u__ID,m_marginLeft, m_marginRight);
}
const int& CATextFieldX::getMarginRight(){
	return m_marginRight;
}
void CATextFieldX::setMarginImageLeft(const DSize imgSize, const std::string& filePath){

}
void CATextFieldX::setMarginImageRight(const DSize imgSize, const std::string& filePath){

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
void CATextFieldX::setFontSize(const int& var){
	m_fontSize = var;
	setFontSizeJNI(m_u__ID,var);

	delayShowImageView();
}
const int& CATextFieldX::getFontSize(){
	return m_fontSize;
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
void CATextFieldX::setPlaceHolderText(const std::string& var){
	m_placeHolderText = var;
	setPlaceHolderTextJNI(m_u__ID, var);

	delayShowImageView();
}
const std::string& CATextFieldX::getPlaceHolderText(){
	return m_placeHolderText;
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
void CATextFieldX::setPlaceHolderColor(const CAColor4B& var){
	m_placeHdolderColor = var;
	setPlaceHolderColorJNI(m_u__ID, getUIntFormColor4B(var));

	delayShowImageView();
}
const CAColor4B& CATextFieldX::getPlaceHolderColor(){
	return m_placeHdolderColor;
}
void setFieldTextJNI(int key, const std::string& var)
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
void CATextFieldX::setFieldText(const std::string& var){
	m_fieldText = var;
	setFieldTextJNI(m_u__ID, var);

	delayShowImageView();
}
const std::string& CATextFieldX::getFieldText(){
	return m_fieldText;
}
void setFieldTextColorJNI(int key, int color)
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
void CATextFieldX::setFieldTextColor(const CAColor4B& var){
	m_fieldTextColor = var;
	setFieldTextColorJNI(m_u__ID, getUIntFormColor4B(var));

	delayShowImageView();
}
const CAColor4B& CATextFieldX::getFieldTextColor(){
	return m_fieldTextColor; 
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
void CATextFieldX::setKeyboardType(const KeyboardType& var){
	m_keyBoardType = var;
	setKeyboardTypeJNI(m_u__ID,(int)var);
}
const CATextFieldX::KeyboardType& CATextFieldX::getKeyboardType(){
	return m_keyBoardType;
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
void CATextFieldX::setReturnType(const ReturnType& var){
	m_returnType = var;
	setReturnTypeJNI(m_u__ID, (int)var);
}
const CATextFieldX::ReturnType& CATextFieldX::getReturnType(){
	return m_returnType;
}
void CATextFieldX::setBackGroundImage(CAImage* image){
	//设置背景
	if (!image)return;

	m_bgImgeView->setCapInsets(DRect(image->getPixelsWide() / 2, image->getPixelsHigh() / 2, 1, 1));
	m_bgImgeView->setImage(image);
}

void CATextFieldX::delayShowImageView()
{
	if (m_pDlayeShow) {
		return;
	}

	m_pDlayeShow = true;

	CAViewAnimation::beginAnimations("", NULL);
	CAViewAnimation::setAnimationDuration(0);
	CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextFieldX::showImageView));
	CAViewAnimation::commitAnimations();
}

NS_CC_END

