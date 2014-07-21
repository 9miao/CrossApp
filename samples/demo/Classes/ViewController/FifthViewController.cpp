#include "FifthViewController.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

FifthViewController::FifthViewController()
{

}

FifthViewController::~FifthViewController()
{

}

void FifthViewController::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	this->getView()->setColor(ccc4(255,255,0,100));
	CAButton* hiddenView = CAButton::createWithCenter(CCRect(size.width*0.5, size.height*0.5, 200, 100), CAButtonTypeCustom);
	hiddenView->setTitleForState(CAControlStateAll, "return");
	hiddenView->setTitleColorForState(CAControlStateNormal, CAColor_blueStyle);
	hiddenView->addTarget(this,CAControl_selector(FifthViewController::hiddenViewController),CAControlEventTouchUpInSide);
	this->getView()->addSubview(hiddenView);
}

void FifthViewController::hiddenViewController(CAControl* btn,CCPoint point)
{
	this->dismissModalViewController(true);
}

void FifthViewController::viewDidUnload()
{

}