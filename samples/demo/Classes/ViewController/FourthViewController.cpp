#include "FourthViewController.h"
#include "FifthViewController.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

FourthViewController::FourthViewController()
{

}

FourthViewController::~FourthViewController()
{

}

void FourthViewController::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	CALabel* presentViewController = CALabel::createWithCenter(CADipRect(size.width*0.5,size.height*0.5,size.width*0.8,500));
	presentViewController->setText("Drag your finger");
	presentViewController->setColor(CAColor_blueStyle);
	presentViewController->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	presentViewController->setTextAlignment(CATextAlignmentCenter);
	presentViewController->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->getView()->insertSubview(presentViewController,1);

	imageView = CAImageView::createWithImage(CAImage::create("2.jpg"));
	imageView->setCenterOrigin(CADipPoint(size.width*0.5,size.height*0.5));
	this->getView()->addSubview(imageView);
}

void FourthViewController::viewDidAppear()
{
	CANavigationBarItem* item = CANavigationBarItem::create("ViewController4");
	this->getTabBarController()->setNavigationBarItem(item);
}

void FourthViewController::viewDidUnload()
{
	
}

bool FourthViewController::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	originalPoint = pTouch->getLocation();

	return true;
}

void FourthViewController::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	CCPoint currentPoint = pTouch->getLocation();
	if (originalPoint.y > currentPoint.y)
	{
		FifthViewController* fifthViewController = new FifthViewController();
		fifthViewController->init();
		fifthViewController->autorelease();
		this->presentModalViewController(fifthViewController, true);
	}
}

void FourthViewController::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{

}

void FourthViewController::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{

}