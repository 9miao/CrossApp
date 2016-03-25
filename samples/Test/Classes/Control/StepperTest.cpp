
#include "StepperTest.h"

StepperTest::StepperTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

StepperTest::~StepperTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void StepperTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    step_value = CALabel::createWithLayout(DRectLayout(100,100,200,100,DRectLayout::L_R_T_H));
    step_value->setText("step_value:0");
    step_value->setColor(CAColor_black);
    step_value->setFontSize(28);
    step_value->setTextAlignment(CATextAlignmentCenter);
    step_value->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    this->getView()->addSubview(step_value);
    
    step = CAStepper::createWithLayout(DRectLayout(200,200,300,56,DRectLayout::L_R_B_H),CAStepperOrientationHorizontal);
    //step->setWraps(true);//是否循环,默认循环
    step->setMinValue(0);
    step->setMaxValue(50);
    step->setStepValue(1);
    //step->setAutoRepeat(false);
    this->getView()->addSubview(step);
    step->addTarget(this, CAControl_selector(StepperTest::stepperCallBack));
}

void StepperTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void StepperTest::stepperCallBack(CAControl *btn, DPoint point)
{
    char tem[30];
    sprintf(tem, "step_value:%.0f",step->getValue());
    step_value->setText(tem);
    CCLog("step-tag === %f",step->getValue());
}



