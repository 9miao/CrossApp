#include "SegmentedControlTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

SegmentedControlTest::SegmentedControlTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

SegmentedControlTest::~SegmentedControlTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

void SegmentedControlTest::viewDidLoad()
{
	char segNormal[50] = "";
	char segHighlighted[50] = "";
	char segSelected[50] = "";
	char pages[10] = "";
	size = this->getView()->getBounds().size;
	
	CALabel* defaultStyle = CALabel::createWithCenter(CADipRect(size.width*0.5,size.height*0.05,size.width*0.5,50));
	defaultStyle->setText("Default Style");
	defaultStyle->setFontSize(_px(30));
	defaultStyle->setColor(CAColor_blueStyle);
	defaultStyle->setTextAlignment(CATextAlignmentCenter);
	defaultStyle->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->getView()->addSubview(defaultStyle);

	CASegmentedControl* defaultSegment = CASegmentedControl::createWithCenter(CADipRect(size.width*0.5,
																					size.height*0.15, 
																					size.width*0.8, 
																					size.height*0.1), 3);
	defaultSegment->setSelectedAtIndex(1);
	this->getView()->addSubview(defaultSegment);
	CCLog("%f",size.height);
	segment = CASegmentedControl::createWithCenter(CADipRect(size.width*0.5,
														size.height*0.3, 
														size.width*0.8, 
														size.height*0.1), 3);
	for (int i = 0; i < 3; i++)
	{
		sprintf(segNormal,"source_material/seg_normal%d.png",i+1);
		sprintf(segHighlighted,"source_material/seg_highlighted%d.png",i+1);
		sprintf(segSelected,"source_material/seg_selected%d.png",i+1);
		sprintf(pages, "Number%d", i + 1);
		segment->setTitleAtIndex(pages, i, CAControlStateAll);
		segment->setBackgroundImageAtIndex(CAImage::create(segNormal), i, CAControlStateNormal);
		segment->setBackgroundImageAtIndex(CAImage::create(segHighlighted), i, CAControlStateHighlighted);
		segment->setBackgroundImageAtIndex(CAImage::create(segSelected), i, CAControlStateSelected);
	}
	segment->setTitleColorAtIndex(CAColor_white, 0, CAControlStateAll);
	segment->setTitleColorAtIndex(CAColor_gray, 1, CAControlStateAll);
	segment->setTitleColorAtIndex(CAColor_yellow, 2, CAControlStateAll);
	segment->setSelectedAtIndex(1);
	segment->addTarget(this, CAControl_selector(SegmentedControlTest::segmentCallback));
	this->getView()->addSubview(segment);
	
	defaultView = CAView::createWithColor(ccc4(150,150,150,150));
	defaultView->setCenter(CADipRect(size.width*0.5, size.height*0.6 + 1, size.width*0.8, size.height*0.5));
	this->getView()->addSubview(defaultView);

	CADipSize viewSize = defaultView->getFrame().size;
	defaultPage = CALabel::createWithCenter(CADipRect(viewSize.width*0.5, 
												   viewSize.height*0.5, 
												   viewSize.width*0.4, 50));
	defaultPage->setText("The second page");
	defaultPage->setFontSize(_px(30));
	defaultPage->setColor(CAColor_white);
	defaultPage->setTextAlignment(CATextAlignmentCenter);
	defaultView->addSubview(defaultPage);
}

void SegmentedControlTest::segmentCallback(CAControl* btn, CCPoint point)
{
	switch (segment->getSelectedIndex())
	{
		case 0:
		{
			  defaultPage->setText("The first page");
			  break;
		}
		case 1:
		{
			  defaultPage->setText("The second page");
			  break;
		}
		case 2:
		{
			  defaultPage->setText("The third page");
			  break;
		}
		default:
			break;
	}
}

void SegmentedControlTest::viewDidUnload()
{

}