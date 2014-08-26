#ifndef _Http_Request_Test_h_
#define _Http_Request_Test_h_

#include "CrossApp.h"
#include "CrossAppExt.h"

USING_NS_CC_EXT;

class HttpRequestTest :public CAViewController
{
public:
	HttpRequestTest();
	virtual ~HttpRequestTest();

private:
    void requestHttpInfo(CAControl* btn, const CCPoint& point);
	void requestResult(CCHttpClient* client, CCHttpResponse* response);
    void previousViewController(CAControl* btn, const CCPoint& point);
    void nextViewController(CAControl* btn, const CCPoint& point);

protected:
	void viewDidLoad();
	void viewDidUnload();

private:
	void loadingAnim(float interval);

private:
	CCSize size;
	int cout;
	CALabel* errorInfo;
	CAView* loading;
	CAImageView* loadImage;
	CATextField* inputWebsite;
	CAButton* requestWebsite;
	CAButton *next;

};

#endif