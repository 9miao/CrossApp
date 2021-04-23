
#include "StepperTest.h"

StepperTest::StepperTest()
{
    this->setTitle("CAStepper");
}

StepperTest::~StepperTest()
{
}

void StepperTest::viewDidLoad()
{
    CALabel* stepValue = CALabel::createWithLayout(DLayout(DHorizontalLayout_W_C(300, 0.5), DVerticalLayout_H_C(100, 0.15)));
    stepValue->setText("step_value:0");
    stepValue->setColor(CAColor4B::BLACK);
    stepValue->setFontSize(28);
    stepValue->setTextAlignment(CATextAlignment::Center);
    stepValue->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
    this->getView()->addSubview(stepValue);
    
    CAStepper* stepper = CAStepper::createWithLayout(DLayout(DHorizontalLayout_W_C(360, 0.5), DVerticalLayout_H_C(60, 0.3)), CAStepper::Orientation::Horizontal);
    //stepper->setWraps(true);//是否循环,默认循环
    stepper->setMinValue(0);
    stepper->setMaxValue(50);
    stepper->setStepValue(1);
    //stepper->setAutoRepeat(false);
    this->getView()->addSubview(stepper);
    stepper->setTarget([=](float value){
        
        char tem[30];
        sprintf(tem, "step_value:%.0f",value);
        stepValue->setText(tem);
        CCLog("step-tag === %f",value);
    });
    
    CALabel* stepValue1 = CALabel::createWithLayout(DLayout(DHorizontalLayout_W_C(200, 0.5), DVerticalLayout_H_C(100, 0.6)));
    stepValue1->setText("step_value:0");
    stepValue1->setColor(CAColor4B::BLACK);
    stepValue1->setFontSize(28);
    stepValue1->setTextAlignment(CATextAlignment::Center);
    stepValue1->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
    this->getView()->addSubview(stepValue1);
    
    CAStepper* stepper1 = CAStepper::createWithLayout(DLayout(DHorizontalLayout_W_C(60, 0.5), DVerticalLayout_H_C(200, 0.8)), CAStepper::Orientation::Vertical);
    //stepper1->setWraps(true);//是否循环,默认循环
    stepper1->setMinValue(0);
    stepper1->setMaxValue(50);
    stepper1->setStepValue(1);
    //stepper1->setAutoRepeat(false);
    this->getView()->addSubview(stepper1);
    stepper1->setTarget([=](float value){
        
        char tem[30];
        sprintf(tem, "step_value:%.0f",value);
        stepValue1->setText(tem);
        CCLog("step-tag === %f",value);
    });
}

void StepperTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}



