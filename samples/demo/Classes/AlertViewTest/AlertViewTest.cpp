#include "AlertViewTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

AlertViewTest::AlertViewTest()
{

}

AlertViewTest::~AlertViewTest()
{

}

void AlertViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	imageBtn = CAButton::createWithCenter(CCRect(size.width*0.5, size.height*0.5,size.width*0.2,50),CAButtonTypeSquareRect);
	imageBtn->setTitleForState(CAControlStateAll,"Click");
	imageBtn->addTarget(this,CAControl_selector(AlertViewTest::respondTouch),CAControlEventTouchUpInSide);
	this->getView()->addSubview(imageBtn);
}

void AlertViewTest::respondTouch(CAControl* btn, const CCPoint& point)
{
	CCSize size = this->getView()->getBounds().size;
	std::vector<std::string> btnTitle;
	CAAlertView* alertView = CAAlertView::create();
	btnTitle.push_back("green");
	btnTitle.push_back("yeelow");
	btnTitle.push_back("orange");
	alertView->showMessage("ButtonImage", "What kind of background of button do you like?", btnTitle);
	alertView->setTitle("ButtonImage",CAColor_blueStyle);
	alertView->setAlertMessage("What kind of background of button do you like?", CAColor_blueStyle);
	alertView->setTarget(this, CAAlertView_selector(AlertViewTest::alertViewCallback));
}

void AlertViewTest::alertViewCallback(int btnIndex)
{
	if (btnIndex == 0)
	{
		imageBtn->setBackGroundViewForState(CAControlStateNormal,CAImageView::createWithImage(CAImage::create("source_material/ex1.png")));
	}
	else if (btnIndex==1)
	{
		imageBtn->setBackGroundViewForState(CAControlStateNormal, CAImageView::createWithImage(CAImage::create("source_material/ex2.png")));
	}
	else
	{
		imageBtn->setBackGroundViewForState(CAControlStateNormal, CAImageView::createWithImage(CAImage::create("source_material/ex3.png")));
	}
}

void AlertViewTest::viewDidUnload()
{

}