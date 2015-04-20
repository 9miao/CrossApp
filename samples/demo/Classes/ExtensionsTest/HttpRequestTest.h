#ifndef _Http_Request_Test_h_
#define _Http_Request_Test_h_

#include <iostream>
#include "CrossApp.h"
#include "CrossAppExt.h"

USING_NS_CC_EXT;

class HttpRequestTest :public CAViewController
{
public:
	HttpRequestTest();
	virtual ~HttpRequestTest();

private:
	void requestHttpInfo(CAControl* btn,CCPoint point);
	void requestResult(CAHttpClient* client, CAHttpResponse* response);
	void previousViewController(CAControl* btn, CCPoint point);
	void nextViewController(CAControl* btn, CCPoint point);

protected:
	void viewDidLoad();
	void viewDidUnload();

private:
	void loadingAnim(float interval);

private:
	CADipSize size;
	int cout;
	CALabel* errorInfo;
	CAView* loading;
	CAImageView* loadImage;
	CATextField* inputWebsite;
	CAButton* requestWebsite;
	CAButton *next;

};

#endif