
#include "SwitchTest.h"

SwitchTest::SwitchTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

SwitchTest::~SwitchTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void SwitchTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
//    CASwitch* customSwitch1 = CASwitch::createWithCenter(DRect(winSize.width/2, winSize.height/2, 100, 20));
    CASwitch* customSwitch1 = CASwitch::createWithLayout(DRectLayout(150,100,350,20,DRectLayout::L_W_T_H));
    customSwitch1->setTag(100);
    customSwitch1->setIsOn(true, false);
    customSwitch1->addTarget(this, CAControl_selector(SwitchTest::switchStateChange));
    this->getView()->addSubview(customSwitch1);
    
    CASwitch* customSwitch2 = CASwitch::createWithLayout(DRectLayout(150,100,350,20,DRectLayout::R_W_T_H));
    customSwitch2->setTag(101);
    customSwitch2->setIsOn(true, false);
    customSwitch2->setOnImage(CAImage::create("image/Switch_on.png"));
    customSwitch2->setOffImage(CAImage::create("image/Switch_off.png"));
    customSwitch2->addTarget(this, CAControl_selector(SwitchTest::switchStateChange));
    this->getView()->addSubview(customSwitch2);
}

void SwitchTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SwitchTest::switchStateChange(CAControl* btn, DPoint point)
{
    CASwitch* state = (CASwitch*)btn;
    if (!state->isOn())
    {
        CCLog("switchStateChange false");
    }
    else
    {
        CCLog("switchStateChange true");
    }
}


