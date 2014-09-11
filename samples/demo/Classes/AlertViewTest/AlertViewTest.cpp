#include "AlertViewTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

AlertViewTest::AlertViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

AlertViewTest::~AlertViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

void AlertViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	imageBtn = CAButton::createWithCenter(CADipRect(size.width*0.5, size.height*0.5, size.width*0.2, 50), CAButtonTypeSquareRect);
	imageBtn->setTitleForState(CAControlStateAll,"Click");
	imageBtn->addTarget(this,CAControl_selector(AlertViewTest::respondTouch),CAControlEventTouchUpInSide);
	this->getView()->addSubview(imageBtn);
}

void AlertViewTest::respondTouch(CAControl* btn ,CCPoint point)
{
	CCSize size = this->getView()->getBounds().size;
	CAAlertView* alertView = CAAlertView::createWithText("ButtonImage", "What kind of background of button do you like?", "green", "yellow", NULL);
	float randNum = CCRANDOM_0_1();
	if (randNum > 0.333f)
	{
		alertView->addButton("orange");
	}
	if (randNum> 0.666f)
	{
		alertView->addButton("blue");
	}
	alertView->show();
	alertView->setTarget(this, CAAlertView_selector(AlertViewTest::alertViewCallback));
}

void AlertViewTest::alertViewCallback(int btnIndex)
{
	if (btnIndex == 0)
	{
		imageBtn->setBackGroundViewForState(CAControlStateNormal,CAImageView::createWithImage(CAImage::create("source_material/ex1.png")));
	}
	else if (btnIndex == 1)
	{
		imageBtn->setBackGroundViewForState(CAControlStateNormal, CAImageView::createWithImage(CAImage::create("source_material/ex2.png")));
	}
	else if (btnIndex == 2)
	{
		imageBtn->setBackGroundViewForState(CAControlStateNormal, CAImageView::createWithImage(CAImage::create("source_material/ex3.png")));
	}
	else
	{
		imageBtn->setBackGroundViewForState(CAControlStateNormal, CAImageView::createWithImage(CAImage::create("source_material/ex3.png")));
	}
}

void AlertViewTest::viewDidUnload()
{

}