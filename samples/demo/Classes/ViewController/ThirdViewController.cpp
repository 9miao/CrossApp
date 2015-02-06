#include "ThirdViewController.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

ThirdViewController::ThirdViewController()
{
	
}

ThirdViewController::~ThirdViewController()
{

}

void ThirdViewController::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	scrollView = CAScrollView::createWithFrame(CADipRect(this->getView()->getBounds()));
	scrollView->setViewSize(scrollView->getBounds().size);
    scrollView->setBounceVertical(false);
    scrollView->setBounceHorizontal(false);
	scrollView->setMaximumZoomScale(3.0f);
	scrollView->setMinimumZoomScale(0.5f);
	this->getView()->addSubview(scrollView);

	CAImageView* s_Image= CAImageView::createWithImage(CAImage::create("bg.jpg"));
	s_Image->setFrame(CADipRect(0, 0, size.width, size.height));
	scrollView->addSubview(s_Image);

	scrollControl = CASegmentedControl::createWithCenter(CADipRect(size.width*0.5, size.height*0.05, size.width*0.8, size.height*0.05), 4);
	scrollControl->setTitleForSegmentAtIndex("Forbidden", 0);
	scrollControl->setTitleForSegmentAtIndex("Horizontal", 1);
	scrollControl->setTitleForSegmentAtIndex("Vertical", 2);
	scrollControl->setTitleForSegmentAtIndex("Unlimited", 3);
	scrollControl->setTitleColor(CAColor_blueStyle);
	scrollControl->addTarget(this, CASegmentedControl_selector(ThirdViewController::stateControl));
	scrollControl->setSelectedAtIndex(0);
	this->getView()->addSubview(scrollControl);
}

void ThirdViewController::viewDidAppear()
{
	CANavigationBarItem* item = CANavigationBarItem::create("ViewController3");
	this->getTabBarController()->setNavigationBarItem(item);
}

void ThirdViewController::stateControl(CASegmentedControl* btn, int index)
{
	switch (index)
	{
	case 0:
		scrollView->setBounceVertical(false);
		scrollView->setBounceHorizontal(false);
		break;
	case 1:
		scrollView->setBounceVertical(false);
		scrollView->setBounceHorizontal(true);
		break;
	case 2:
		scrollView->setBounceHorizontal(false);
		scrollView->setBounceVertical(true);
		break;
	case 3:
		scrollView->setBounceHorizontal(true);
		scrollView->setBounceVertical(true);
		break;
	default:
		break;
	}
}

void ThirdViewController::viewDidUnload()
{

}