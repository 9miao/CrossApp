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
	scrollControl->setTitleAtIndex("Forbidden", 0, CAControlStateAll);
	scrollControl->setTitleAtIndex("Horizontal", 1,CAControlStateAll);
	scrollControl->setTitleAtIndex("Vertical", 2, CAControlStateAll);
	scrollControl->setTitleAtIndex("Unlimited", 3, CAControlStateAll);
	scrollControl->setTitleColorAtIndex(CAColor_blueStyle,0,CAControlStateNormal);
	scrollControl->setTitleColorAtIndex(CAColor_blueStyle, 1, CAControlStateNormal);
	scrollControl->setTitleColorAtIndex(CAColor_blueStyle, 2, CAControlStateNormal);
	scrollControl->setTitleColorAtIndex(CAColor_blueStyle, 3, CAControlStateNormal);
	scrollControl->addTarget(this, CAControl_selector(ThirdViewController::stateControl));
	scrollControl->setSelectedAtIndex(0);
	this->getView()->addSubview(scrollControl);
}

void ThirdViewController::stateControl(CAControl* btn, CCPoint point)
{
	int index = scrollControl->getselectedIndex();
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