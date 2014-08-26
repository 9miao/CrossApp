#include "FifthViewController.h"
#include "time.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

FifthViewController::FifthViewController() :isTouchEnd(false), touchTime(0.0)
{
	pointSet =new CCArray(2);
}

FifthViewController::~FifthViewController()
{

}

void FifthViewController::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	this->getView()->setColor(CAColor_gray);
	gestureView = CAImageView::createWithImage(CAImage::create("bg.jpg"));
	gestureView->setCenterOrigin(CCPoint(size.width*0.5,size.height*0.5));
	viewOriginalSize = gestureView->getFrame().size;
	this->getView()->addSubview(gestureView);
}

void FifthViewController::hiddenViewController(CAControl* btn,CCPoint point)
{
	this->dismissModalViewController(true);
}

void FifthViewController::viewDidUnload()
{

}
bool FifthViewController::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	if (isTouchEnd)
	{
		isTouchEnd = false;
	}
	CCLog("%.02f,%.02f", pTouch->getLocation().x, pTouch->getLocation().y);
	CCLog("%f,%f", gestureView->getFrame().size.width,gestureView->getFrame().size.height);
	CCLog("%.02f,%.02f", gestureView->getFrame().origin.x, gestureView->getFrame().origin.y);
	if (gestureView->getFrame().containsPoint(pTouch->getLocation()))
	{
		do
		{
			CC_BREAK_IF(pointSet->count()>2);
			if (!pointSet->containsObject(pTouch)&&pointSet->count()<2)
			{
				pointSet->addObject(pTouch);
			}
			if (pointSet->count() == 1)
			{
				double interval;
				if (point_Vector.size() == 1)
				{
					secondClick = clock();
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
					interval = (double)(secondClick - firstClick) / 1000000;
#else
					interval = (double)(secondClick - firstClick) / 1000;
#endif
					CCLog("%.03f", interval);
					if (interval<0.3)
					{
						if (viewOriginalSize.equals(gestureView->getFrame().size))
						{
							gestureView->runAction(CCScaleTo::create(0.2f, 0.5f));
						}
						else
						{
							gestureView->runAction(CCScaleTo::create(0.2f, 1.0f));
						}
					}
					else
					{
						point_Vector.clear();
					}
				}
				originalPoint = pTouch->getLocation();
				point_Vector.push_back(originalPoint);
				firstClick = clock();
				
				CAScheduler::schedule(schedule_selector(FifthViewController::isExecuteEndFunc),this,0.1f,false);
			}
			if (pointSet->count() == 2)
			{
				
			}
			return true;
		} while (0);
	}
	else
	{
		this->dismissModalViewController(true);
		return false;
	}

    return false;
}

void FifthViewController::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	if (pointSet->count() == 1)
	{
		if (CAScheduler::isScheduled(schedule_selector(FifthViewController::isExecuteEndFunc), this))
		{
			CAScheduler::unschedule(schedule_selector(FifthViewController::isExecuteEndFunc), this);
		}
		CCPoint currentPoint = pTouch->getLocation();
		gestureView->setCenterOrigin(CCPoint(gestureView->getCenterOrigin().x + (currentPoint.x - originalPoint.x),
											 gestureView->getCenterOrigin().y + (currentPoint.y - originalPoint.y)));
		CCLog("%f,%f",gestureView->getFrame().origin.x,gestureView->getFrame().origin.x);
		originalPoint = currentPoint;
	}
}

void FifthViewController::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
	isTouchEnd = true;
	if (CAScheduler::isScheduled(schedule_selector(FifthViewController::isExecuteEndFunc), this))
	{
		CAScheduler::unschedule(schedule_selector(FifthViewController::isExecuteEndFunc), this);
	}
	double interval;
	if (point_Vector.size() > 1)
	{
		point_Vector.clear();
	}
	invalidClick = clock();
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	interval = (double)(invalidClick - firstClick) / 1000000;
#else
	interval = (double)(invalidClick - firstClick) / 1000;
#endif
	if (interval > 0.3)
	{
		point_Vector.clear();
	}
	pointSet->removeObject(pTouch);
}

void FifthViewController::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{

}

void FifthViewController::isExecuteEndFunc(float intervalTime)
{
	touchTime = touchTime+intervalTime;
	CCLog("%f", touchTime);
	if (!isTouchEnd&&touchTime>0.6)
	{
		if (CAScheduler::isScheduled(schedule_selector(FifthViewController::isExecuteEndFunc), this))
		{
			CAScheduler::unschedule(schedule_selector(FifthViewController::isExecuteEndFunc), this);
		}
		if (viewOriginalSize.equals(gestureView->getFrame().size))
		{
			gestureView->runAction(CCScaleTo::create(0.8f, 0.5f));
		}
		else
		{
			gestureView->runAction(CCScaleTo::create(0.8f, 1.0f));
		}
		
		touchTime = 0;
	}
}