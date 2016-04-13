
#include "SegmentedControlTest.h"

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
    this->getView()->setColor(CAColor_gray);
    
    CASegmentedControl* segment1 = CASegmentedControl::createWithLayout(DLayout(DHorizontalLayout_W_C(300, 0.5), DVerticalLayout_H_C(54, 0.25)), 3);
    
    char temstr[20];
    for (int i = 0; i < 3; i++)
    {
        sprintf(temstr, "Page%d", i + 1);
        segment1->setTitleForSegmentAtIndex(temstr, i);
    }
    segment1->setTitleColor(CAColor_black);
    segment1->setTitleSelectedColor(CAColor_white);
    segment1->addTarget(this, CASegmentedControl_selector(SegmentedControlTest::segmentCallback));
    segment1->setSelectedAtIndex(0);
    this->getView()->addSubview(segment1);
    
    CASegmentedControl* segment2 = CASegmentedControl::createWithLayout(DLayout(DHorizontalLayout_W_C(300, 0.5), DVerticalLayout_H_C(54, 0.55)), 3);
    char temstr2[20];
    for (int i = 0; i < 3; i++)
    {
        sprintf(temstr2, "Page%d", i + 1);
        segment2->setTitleForSegmentAtIndex(temstr2, i);
    }
    segment2->setTitleColor(CAColor_black);
    segment2->setTitleSelectedColor(CAColor_white);
    
    segment2->addTarget(this, CASegmentedControl_selector(SegmentedControlTest::segmentCallback));
    segment2->setBackgroundImage(CAImage::create("image/seg1.png"));
    segment2->setSegmentItemBackgroundImage(CAImage::create("image/seg2.png"));
    segment2->setTintColor(ccc4(44, 178, 93, 255));
    segment2->setSelectedAtIndex(0);
    this->getView()->addSubview(segment2);
}

void SegmentedControlTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SegmentedControlTest::segmentCallback(CASegmentedControl* btn, int index)
{
    CCLog("segmentCallback");
}
