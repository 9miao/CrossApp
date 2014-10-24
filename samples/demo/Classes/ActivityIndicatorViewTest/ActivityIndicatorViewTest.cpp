#include "ActivityIndicatorViewTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

ActivityIndicatorViewTest::ActivityIndicatorViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

ActivityIndicatorViewTest::~ActivityIndicatorViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

void ActivityIndicatorViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	this->getView()->setColor(ccc4(58,150,200,255));

	whiteLarge = CAActivityIndicatorView::createWithCenter(CADipRect(size.width*0.5, size.height*0.25,
		size.width*0.2, size.width*0.2));
	whiteLarge->setStyle(CAActivityIndicatorViewStyleWhiteLarge);
	whiteLarge->setTargetOnCancel(this, callfunc_selector(ActivityIndicatorViewTest::cancelCallback));
	this->getView()->addSubview(whiteLarge);
	whiteLarge->startAnimating();

	white = CAActivityIndicatorView::createWithCenter(CADipRect(size.width*0.5, size.height*0.5,
		size.width*0.2,size.width*0.2));
	white->setStyle(CAActivityIndicatorViewStyleWhite);
	this->getView()->addSubview(white);
	white->startAnimating();

	gray = CAActivityIndicatorView::createWithCenter(CADipRect(size.width*0.5, size.height*0.75,
		size.width*0.2, size.width*0.2));
	gray->setStyle(CAActivityIndicatorViewStyleGray);
	this->getView()->addSubview(gray);
	gray->startAnimating();

	CAButton* activityControl = CAButton::createWithCenter(CADipRect(size.width*0.5,size.height*0.85,150,100),CAButtonTypeRoundedRect);
	activityControl->addTarget(this, CAControl_selector(ActivityIndicatorViewTest::stopAllActivity), CAControlEventTouchUpInSide);
	//this->getView()->addSubview(activityControl);
}

void ActivityIndicatorViewTest::viewDidUnload()
{
	whiteLarge->stopAnimating();
	white->stopAnimating();
	gray->stopAnimating();
}

void ActivityIndicatorViewTest::cancelCallback()
{

}

void ActivityIndicatorViewTest::stopAllActivity(CAControl* btn, CCPoint point)
{

}
