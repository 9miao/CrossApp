
#include "CAWebViewImpl.h"
#include "view/CAWebView.h"
#include "basics/CAApplication.h"
#include "platform/CAFileUtils.h"
#include "platform/CADensityDpi.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

#define CLASS_NAME "org/CrossApp/lib/CrossAppWebViewHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"",__VA_ARGS__)


NS_CC_BEGIN
static std::map<int, std::function<void(CAImage*)> > s_gImageCallback;
static std::string s_cszWebViewHtmSource;
static std::map<int, CAWebViewImpl*> s_WebViewImpls;

extern "C" {

    JNIEXPORT jboolean JNICALL Java_org_CrossApp_lib_CrossAppWebViewHelper_shouldStartLoading(JNIEnv *env, jclass, jint index, jstring jurl) {
        const char* charUrl = env->GetStringUTFChars(jurl, NULL);
        std::string url = charUrl;
        env->ReleaseStringUTFChars(jurl, charUrl);
        return CAWebViewImpl::shouldStartLoading(index, url);
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppWebViewHelper_didFinishLoading(JNIEnv *env, jclass, jint index, jstring jurl) {
		const char* charUrl = env->GetStringUTFChars(jurl, NULL);
        std::string url = charUrl;
        env->ReleaseStringUTFChars(jurl, charUrl);
		CAWebViewImpl::didFinishLoading(index, url);
    }

	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppWebViewHelper_didLoadHtmlSource(JNIEnv *env, jclass, jstring jurl) {
		const char* charHtml = env->GetStringUTFChars(jurl, NULL);
		s_cszWebViewHtmSource = charHtml;
		env->ReleaseStringUTFChars(jurl, charHtml);
	}

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppWebViewHelper_didFailLoading(JNIEnv *env, jclass, jint index, jstring jurl) {
		const char* charUrl = env->GetStringUTFChars(jurl, NULL);
        std::string url = charUrl;
        env->ReleaseStringUTFChars(jurl, charUrl);
		CAWebViewImpl::didFailLoading(index, url);
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppWebViewHelper_onJsCallback(JNIEnv *env, jclass, jint index, jstring jmessage) {
		const char* charMessage = env->GetStringUTFChars(jmessage, NULL);
        std::string message = charMessage;
        env->ReleaseStringUTFChars(jmessage, charMessage);
		CAWebViewImpl::onJsCallback(index, message);
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppWebView_onSetByteArrayBuffer(JNIEnv *env, jclass, jint index, jbyteArray buf, jint width, jint height) {
			
        ssize_t length = (ssize_t)(width * height * 4);
        unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * (length + 1));
        data[length] = '\0';
        env->GetByteArrayRegion(buf, 0, length, (jbyte *)data);
        
        CrossApp::CAData* ca_data = CrossApp::CAData::create();
        ca_data->fastSet(data, length);

        if (CAImage* image = CAImage::createWithRawDataNoCache(ca_data, CAImage::PixelFormat::RGBA8888, width, height))
        {
            if (s_gImageCallback.count((int)index) > 0)
            {
                s_gImageCallback[(int)index](image);
                s_gImageCallback.erase((int)index);
            }
        }
        
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppWebViewHelper_pause() {
        CrossApp::CAApplication::getApplication()->pause();
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppWebViewHelper_resume() {
        CrossApp::CAApplication::getApplication()->resume();
    }
}


int createWebViewJNI() {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createWebView", "()I")) {
        jint viewTag = t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return viewTag;
    }
    return -1;
}

void removeWebViewJNI(const int index) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "removeWebView", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

void getWebViewImageJNI(const int index) {
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getWebViewImage", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
		t.env->DeleteLocalRef(t.classID);
	}
}

void setWebViewRectJNI(const int index, const int left, const int top, const int width, const int height) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setWebViewRect", "(IIIII)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, left, top, width, height);
        t.env->DeleteLocalRef(t.classID);
    }
}

void setJavascriptInterfaceSchemeJNI(const int index, const std::string &scheme) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setJavascriptInterfaceScheme", "(ILjava/lang/String;)V")) {
        jstring jScheme = t.env->NewStringUTF(scheme.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jScheme);

        t.env->DeleteLocalRef(jScheme);
        t.env->DeleteLocalRef(t.classID);
    }
}

void loadDataJNI(const int index, const std::string &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadData", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        jstring jData = t.env->NewStringUTF(data.c_str());
        jstring jMIMEType = t.env->NewStringUTF(MIMEType.c_str());
        jstring jEncoding = t.env->NewStringUTF(encoding.c_str());
        jstring jBaseURL = t.env->NewStringUTF(baseURL.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jData, jMIMEType, jEncoding, jBaseURL);

        t.env->DeleteLocalRef(jData);
        t.env->DeleteLocalRef(jMIMEType);
        t.env->DeleteLocalRef(jEncoding);
        t.env->DeleteLocalRef(jBaseURL);
        t.env->DeleteLocalRef(t.classID);
    }
}

void loadHTMLStringJNI(const int index, const std::string &string, const std::string &baseURL) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadHTMLString", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        jstring jString = t.env->NewStringUTF(string.c_str());
        jstring jBaseURL = t.env->NewStringUTF(baseURL.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jString, jBaseURL);

        t.env->DeleteLocalRef(jString);
        t.env->DeleteLocalRef(jBaseURL);
        t.env->DeleteLocalRef(t.classID);
    }
}

void loadUrlJNI(const int index, const std::string &url) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadUrl", "(ILjava/lang/String;)V")) {
        jstring jUrl = t.env->NewStringUTF(url.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jUrl);

        t.env->DeleteLocalRef(jUrl);
        t.env->DeleteLocalRef(t.classID);
    }
}

void loadFileJNI(const int index, const std::string &filePath) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadFile", "(ILjava/lang/String;)V")) {
        jstring jFilePath = t.env->NewStringUTF(filePath.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jFilePath);

        t.env->DeleteLocalRef(jFilePath);
        t.env->DeleteLocalRef(t.classID);
    }
}

void stopLoadingJNI(const int index) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "stopLoading", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

void reloadJNI(const int index) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "reload", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

bool canGoBackJNI(const int index) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "canGoBack", "(I)Z")) {
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
        return ret;
    }
    return false;
}

bool canGoForwardJNI(const int index) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "canGoForward", "(I)Z")) {
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
        return ret;
    }
    return false;
}

void goBackJNI(const int index) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "goBack", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

void goForwardJNI(const int index) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "goForward", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

std::string evaluateJSJNI(const int index, const std::string &js) {
	JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "evaluateJS", "(ILjava/lang/String;)V")) {

		s_cszWebViewHtmSource.clear();
        jstring jjs = t.env->NewStringUTF(js.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jjs);

        t.env->DeleteLocalRef(jjs);
        t.env->DeleteLocalRef(t.classID);
    }
	return s_cszWebViewHtmSource;
}

void setScalesPageToFitJNI(const int index, const bool scalesPageToFit) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setScalesPageToFit", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, scalesPageToFit);
        t.env->DeleteLocalRef(t.classID);
  }
}

void setWebViewVisibleJNI(const int index, const bool visible) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setVisible", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, visible);
        t.env->DeleteLocalRef(t.classID);
    }
}

std::string getUrlStringByFileName(const std::string &fileName) {
    const std::string basePath("file:///android_asset/");
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName);
    const std::string assetsPath("assets/");

    std::string urlString;
    if (fullPath.find(assetsPath) != std::string::npos) {
        urlString = fullPath.replace(fullPath.find_first_of(assetsPath), assetsPath.length(), basePath);
    } else {
        urlString = fullPath;
    }

    return urlString;
}



CAWebViewImpl::CAWebViewImpl(CAWebView *webView) : _viewTag(-1), _webView(webView) 
{
	_viewTag = createWebViewJNI();
	s_WebViewImpls[_viewTag] = this;
}

CAWebViewImpl::~CAWebViewImpl() {
	removeWebViewJNI(_viewTag);
	s_WebViewImpls.erase(_viewTag);
}

void CAWebViewImpl::loadHTMLString(const std::string &string, const std::string &baseURL) {
	loadHTMLStringJNI(_viewTag, string, baseURL);
}

void CAWebViewImpl::loadURL(const std::string &url) {
	loadUrlJNI(_viewTag, url);
}

void CAWebViewImpl::loadFile(const std::string &fileName) {
	std::string fullPath = getUrlStringByFileName(fileName);
	loadFileJNI(_viewTag, fullPath);
}

void CAWebViewImpl::stopLoading() {
	stopLoadingJNI(_viewTag);
}

void CAWebViewImpl::reload() {
	reloadJNI(_viewTag);
}

bool CAWebViewImpl::canGoBack() {
	return canGoBackJNI(_viewTag);
}

bool CAWebViewImpl::canGoForward() {
	return canGoForwardJNI(_viewTag);
}

void CAWebViewImpl::goBack() {
	goBackJNI(_viewTag);
}

void CAWebViewImpl::goForward() {
	goForwardJNI(_viewTag);
}

void CAWebViewImpl::setJavascriptInterfaceScheme(const std::string &scheme) {
	setJavascriptInterfaceSchemeJNI(_viewTag, scheme);
}

std::string CAWebViewImpl::evaluateJS(const std::string &js) {
	return evaluateJSJNI(_viewTag, js);
}

void CAWebViewImpl::setScalesPageToFit(const bool scalesPageToFit) {
	setScalesPageToFitJNI(_viewTag, scalesPageToFit);
}

bool CAWebViewImpl::shouldStartLoading(const int viewTag, const std::string &url) {

	std::map<int, CAWebViewImpl*>::iterator it = s_WebViewImpls.find(viewTag);
	if (it != s_WebViewImpls.end()) {
		CAWebView* webView = it->second->_webView;
		if (webView && webView->m_obStartLoading)
        {
			if (!webView->m_obStartLoading(url))
				return false;
		}
	}
	return true;
}

void CAWebViewImpl::didFinishLoading(const int viewTag, const std::string &url){
	std::map<int, CAWebViewImpl*>::iterator it = s_WebViewImpls.find(viewTag);
	if (it != s_WebViewImpls.end()) {
		CAWebView* webView = it->second->_webView;
		if (webView && webView->m_obFinishLoading)
        {
			webView->m_obFinishLoading(url);
		}
	}
}


void CAWebViewImpl::didFailLoading(const int viewTag, const std::string &url){
	std::map<int, CAWebViewImpl*>::iterator it = s_WebViewImpls.find(viewTag);
	if (it != s_WebViewImpls.end()) {
		CAWebView* webView = it->second->_webView;
		if (webView && webView->m_obFailLoading)
        {
			webView->m_obFailLoading(url);
		}
	}
}

void CAWebViewImpl::onJsCallback(const int viewTag, const std::string &message){
	std::map<int, CAWebViewImpl*>::iterator it = s_WebViewImpls.find(viewTag);
	if (it != s_WebViewImpls.end()) {
		CAWebView* webView = it->second->_webView;
        if (webView && webView->m_obJSCallback)
        {
            webView->m_obJSCallback(message);
        }
	}
}

void CAWebViewImpl::setAllWebviewRectEmpty()
{

}

void CAWebViewImpl::update(float dt)
{
	DRect cRect = _webView->convertRectToWorldSpace(_webView->getBounds());
    cRect.origin.x = s_dip_to_px(cRect.origin.x);
    cRect.origin.y = s_dip_to_px(cRect.origin.y);
    cRect.size.width = s_dip_to_px(cRect.size.width);
    cRect.size.height = s_dip_to_px(cRect.size.height);
	setWebViewRectJNI(_viewTag, cRect.origin.x, cRect.origin.y, cRect.size.width, cRect.size.height);
}

void CAWebViewImpl::setVisible(bool visible) {
	setWebViewVisibleJNI(_viewTag, visible);
}


void CAWebViewImpl::getWebViewImage(const std::function<void(CAImage*)>& callback)
{
    s_gImageCallback[_viewTag] = callback;
	getWebViewImageJNI(_viewTag);
}

NS_CC_END
