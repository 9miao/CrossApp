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
//	size = this->getView()->getBounds().size;
//	CALabel* presentViewController = CALabel::createWithCenter(CADipRect(size.width*0.5,size.height*0.5,size.width*0.8,500));
//	presentViewController->setText("Drag your finger");
//	presentViewController->setColor(CAColor_blueStyle);
//	presentViewController->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
//	presentViewController->setTextAlignment(CATextAlignmentCenter);
//	presentViewController->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
//	this->getView()->insertSubview(presentViewController,1);

	imageView = CAImageView::createWithImage(CAImage::create("9m.jpg"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
	imageView->setFrame(CADipRect(100, 100, 100, 100));
	this->getView()->addSubview(imageView);
    
    imageView2 = CAImageView::createWithImage(CAImage::create("9m.jpg"));
    imageView2->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    imageView2->setFrame(CADipRect(440, 100, 100, 100));
    this->getView()->addSubview(imageView2);
}

void FourthViewController::viewDidAppear()
{
	CANavigationBarItem* item = CANavigationBarItem::create("ViewController4");
	this->getTabBarController()->setNavigationBarItem(item);
}

void FourthViewController::viewDidUnload()
{
    
}

void FourthViewController::start(const std::string& s, void* context)
{
    CCLog("1111");
}

void FourthViewController::stop(const std::string& s, void* context)
{
    CCLog("2222");
}

bool FourthViewController::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	originalPoint = pTouch->getLocation();
    
    
    
	return true;
}

void FourthViewController::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
//	CCPoint currentPoint = pTouch->getLocation();
//	if (originalPoint.y > currentPoint.y)
//	{
//		FifthViewController* fifthViewController = new FifthViewController();
//		fifthViewController->init();
//		fifthViewController->autorelease();
//		this->presentModalViewController(fifthViewController, true);
//	}

}

void FourthViewController::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    imageView->setFrame(CADipRect(100, 100, 100, 100));
    imageView->setSkewX(0);
    imageView->setSkewY(0);
    imageView->setRotation(0);
    imageView->setVertexZ(0);
    //imageView->setAlpha(0.0f);
    
    imageView2->setFrame(CADipRect(440, 100, 100, 100));
    imageView2->setSkewX(0);
    imageView2->setSkewY(0);
    imageView2->setRotation(0);
    imageView2->setVertexZ(0);
    
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(1.0f);
    //CAViewAnimation::setAnimationDelay(0.3f);
    CAViewAnimation::setAnimationWillStartSelector(this, CAViewAnimation_selector(FourthViewController::start));
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation_selector(FourthViewController::stop));
    imageView->setFrame(CADipRect(400, 400, 100, 100));
//    imageView->setScale(0.3f);
//    imageView->setColor(ccc4(128, 20, 70, 128));
//    imageView->setSkewX(180);
//    imageView->setSkewY(-180);
    imageView->setRotation(720);
//    imageView->setVertexZ(300);
//    imageView->setAlpha(1.0f);
    
    
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(1.0f);
//    CAViewAnimation::setAnimationDelay(1.0f);
    
    imageView2->setFrame(CADipRect(140, 400, 100, 100));
//    imageView2->setScale(0.3f);
//    imageView2->setColor(ccc4(128, 20, 70, 128));
//    imageView2->setSkewX(-180);
//    imageView2->setSkewY(180);
    //imageView2->setRotation(-3240);
    imageView2->setRotationY(360);

    
    CAViewAnimation::commitAnimations();
    
    CAViewAnimation::commitAnimations();
}

void FourthViewController::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{

}