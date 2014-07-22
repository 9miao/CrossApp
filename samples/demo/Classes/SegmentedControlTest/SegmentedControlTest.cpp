#include "SegmentedControlTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

#pragma execution_character_set("utf-8")

SegmentedControlTest::SegmentedControlTest():
firstPage(NULL), 
secondPage(NULL), 
thirdPage(NULL)
{

}

SegmentedControlTest::~SegmentedControlTest()
{

}

void SegmentedControlTest::viewDidLoad()
{
	char pages[10] = "";
	size = this->getView()->getBounds().size;
	
	CASegmentedControl* defaultSegment = CASegmentedControl::createWithCenter(CCRect(size.width*0.5, size.height*0.2, size.width*0.8, size.height*0.05), 3);
	this->getView()->addSubview(defaultSegment);

	segment = CASegmentedControl::createWithCenter(CCRect(size.width*0.5, size.height*0.4, size.width*0.8, size.height*0.05), 3);
	for (int i = 0; i < 3; i++)
	{
		sprintf(pages, "Number%d", i + 1);
		segment->setTitleAtIndex(pages, i, CAControlStateAll);
	}
	segment->setTitleColorAtIndex(CAColor_green, 0, CAControlStateAll);
	segment->setTitleColorAtIndex(CAColor_magenta, 1, CAControlStateAll);
	segment->setTitleColorAtIndex(CAColor_red, 2, CAControlStateAll);
	segment->addTarget(this, CAControl_selector(SegmentedControlTest::segmentCallback));
	this->getView()->addSubview(segment);
}

void SegmentedControlTest::segmentCallback(CAControl* btn, CCPoint point)
{
	int index = segment->getselectedIndex();
	switch (index)
	{
		case 0:
		{
			  if (firstPage)
			  {
				  firstPage->removeFromSuperview();
				  firstPage = NULL;
			  }
			  firstPage = CAView::createWithColor(CAColor_gray);
			  firstPage->setCenter(CCRect(size.width*0.5, size.height*0.65, size.width*0.8, size.height*0.4));
			  this->getView()->addSubview(firstPage);

			  label = CALabel::createWithCenter(CCRect(firstPage->getFrame().size.width*0.5, firstPage->getFrame().size.height*0.5, size.width*0.4, 50));
			  label->setText("The first page");
			  label->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
			  label->setColor(CAColor_blueStyle);
			  label->setTextAlignment(CATextAlignmentCenter);
			  firstPage->addSubview(label);

			  break;
		}
		case 1:
		{
			  if (secondPage)
			  {
				  secondPage->removeFromSuperview();
				  secondPage = NULL;
			  }
			  secondPage = CAView::createWithColor(CAColor_gray);
			  secondPage->setCenter(CCRect(size.width*0.5, size.height*0.65, size.width*0.8, size.height*0.4));
			  this->getView()->addSubview(secondPage);

			  label = CALabel::createWithCenter(CCRect(secondPage->getFrame().size.width*0.5, secondPage->getFrame().size.height*0.5, size.width*0.4, 50));
			  label->setText("The second page");
			  label->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
			  label->setColor(CAColor_blueStyle);
			  label->setTextAlignment(CATextAlignmentCenter);
			  secondPage->addSubview(label);

			  break;
		}
		case 2:
		{
			  if (thirdPage)
			  {
				  thirdPage->removeFromSuperview();
				  thirdPage = NULL;
			  }
			  thirdPage = CAView::createWithColor(CAColor_gray);
			  thirdPage->setCenter(CCRect(size.width*0.5, size.height*0.65 , size.width*0.8, size.height*0.4));
			  this->getView()->addSubview(thirdPage);

			  label = CALabel::createWithCenter(CCRect(thirdPage->getFrame().size.width*0.5, thirdPage->getFrame().size.height*0.5, size.width*0.4, 50));
			  label->setText("The third page");
			  label->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
			  label->setColor(CAColor_blueStyle);
			  label->setTextAlignment(CATextAlignmentCenter);
			  thirdPage->addSubview(label);

			  break;
		}
		default:
			break;
	}
}

void SegmentedControlTest::viewDidUnload()
{

}