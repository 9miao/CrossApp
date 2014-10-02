#include "HttpRequestTest.h"
#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
#include <tchar.h>
#endif

#define CAColor_blueStyle ccc4(51,204,255,255)

HttpRequestTest::HttpRequestTest() :cout(0)
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

HttpRequestTest::~HttpRequestTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

void HttpRequestTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	CABarButtonItem* rightButton = CABarButtonItem::create("", CAImage::create("source_material/btn_right_white.png"), CAImage::create(""));
	rightButton->setTarget(this, CAControl_selector(HttpRequestTest::nextViewController));
	this->getNavigationBarItem()->addRightButtonItem(rightButton);

	errorInfo = CALabel::createWithCenter(CADipRect(size.width*0.5, size.height*0.1, size.width, size.height*0.2));
	errorInfo->setText("");
	errorInfo->setColor(CAColor_blueStyle);
	errorInfo->setFontSize(_px(30));
	errorInfo->setTextAlignment(CATextAlignmentCenter);
	errorInfo->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->getView()->addSubview(errorInfo);

	inputWebsite = CATextField::createWithCenter(CADipRect(size.width*0.5, size.height*0.4, size.width*0.6, size.height*0.1));
	inputWebsite->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_highlighted.png")));
	inputWebsite->setPlaceHolder("Please enter a website");
	inputWebsite->setFontSize(_px(24));
	inputWebsite->setTextColor(CAColor_blueStyle);
	inputWebsite->setSpaceHolderColor(CAColor_gray);
	this->getView()->addSubview(inputWebsite);

	requestWebsite = CAButton::create(CAButtonTypeRoundedRect);
	requestWebsite->setCenter(CADipRect(size.width*0.5, size.height*0.8, size.width*0.2, size.height*0.06));
	requestWebsite->setTitleForState(CAControlStateAll,"Go");
	requestWebsite->addTarget(this,CAControl_selector(HttpRequestTest::requestHttpInfo),CAControlEventTouchUpInSide);
	this->getView()->addSubview(requestWebsite);
}

void HttpRequestTest::requestHttpInfo(CAControl* btn, CCPoint point)
{
	loading = CAView::createWithColor(ccc4(255, 255, 255, 0));
	loading->setTag(200);
	loading->setFrame(this->getView()->getBounds());
	this->getView()->addSubview(loading);

	loadImage = CAImageView::createWithImage(CAImage::create("loading.png"));
	loadImage->setCenterOrigin(CADipPoint(size.width*0.5, size.height*0.5));
	loadImage->setScale(0.5);
	loading->addSubview(loadImage);
	CAScheduler::schedule(schedule_selector(HttpRequestTest::loadingAnim), this, 0.01, false);

	CALabel* msg = CALabel::createWithCenter(CADipRect(size.width*0.5, size.height*0.5, loadImage->getFrame().size.width*0.9, 50));
	msg->setText("Loading");
	msg->setColor(CAColor_blueStyle);
	msg->setFontSize(_px(22));
	msg->setTextAlignment(CATextAlignmentCenter);
	msg->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	loading->addSubview(msg);

	string url = "";
	if (inputWebsite->getText().find("http://")==string::npos)
	{
		url = "http://" + inputWebsite->getText();
	}
	else
	{
		url = inputWebsite->getText();
	}
	CCHttpRequest* request = new CCHttpRequest();
	request->setTag("Getpage");
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setUrl(url.c_str());
	request->setResponseCallback(this,httpresponse_selector(HttpRequestTest::requestResult));
	CCHttpClient* httpClient = CCHttpClient::getInstance();
	httpClient->setTimeoutForConnect(30);
	httpClient->send(request); 
	request->release();
}

void HttpRequestTest::loadingAnim(float interval)
{
	cout++;
	loadImage->setRotation(cout * 3);
}

void HttpRequestTest::requestResult(CCHttpClient* client, CCHttpResponse* response)
{ 
	if (!response->isSucceed())
	{
		requestWebsite->setTitleForState(CAControlStateAll, "Failed");
		CCLog("Error reason: %s", response->getErrorBuffer()); 
		CAScheduler::unschedule(schedule_selector(HttpRequestTest::loadingAnim), this);
		loading->removeFromSuperview();
		errorInfo->setText(response->getErrorBuffer());

		return;
	}
	if (!strcmp("Getpage", response->getHttpRequest()->getTag()) && (response->getResponseCode() == 200))
	{
		std::string responseRes = "";
		std::vector<char> *buffer = response->getResponseData();
		for (unsigned int i = 0; i < buffer->size(); i++)
		{
			responseRes += (*buffer)[i];

		}
		errorInfo->setText("Request successful");
		requestWebsite->setTitleForState(CAControlStateAll, "Successful");
#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
		std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
		path += "DownLoad.html";/*samples\demo\proj.win32\Debug.win32*/
		char* buf = (char*)malloc(buffer->size());
		std::copy(buffer->begin(), buffer->end(), buf);
		FILE *fp = fopen(path.c_str(), "wb+");
		fwrite(buf, 1, buffer->size(), fp);
		fclose(fp);

		string website = response->getHttpRequest()->getUrl();
		int len = website.length();
		int unicodeLenght = ::MultiByteToWideChar(CP_UTF8,0,website.c_str(),-1,NULL,0);
		wchar_t *p_Unicode = new  wchar_t[unicodeLenght + 1];
		std::memset(p_Unicode, 0, (unicodeLenght + 1)*sizeof(wchar_t));
		::MultiByteToWideChar(CP_UTF8, 0, website.c_str(), -1, (LPWSTR)p_Unicode, unicodeLenght);
		wstring  webLink = (wchar_t*)p_Unicode;
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), (LPCWSTR)webLink.c_str(), NULL, SW_SHOW);
		delete  p_Unicode;
#endif
		CAScheduler::unschedule(schedule_selector(HttpRequestTest::loadingAnim), this);
		loading->removeFromSuperview();
	}

}

void HttpRequestTest::previousViewController(CAControl* btn, CCPoint point)
{
	this->getNavigationController()->popViewControllerAnimated(true);
}

void HttpRequestTest::nextViewController(CAControl* btn, CCPoint point)
{

}

void HttpRequestTest::viewDidUnload()
{

}