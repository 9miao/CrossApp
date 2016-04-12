
#include "ProgressTest.h"

ProgressTest::ProgressTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

ProgressTest::~ProgressTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void ProgressTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    progress_value = CALabel::createWithLayout(DLayout(100,100,300,50,DLayout::L_R_T_H));
    progress_value->setTag(100);
    progress_value->setText("");
    progress_value->setFontSize(30);
    progress_value->setColor(ccc4(51, 204, 255, 255));
    progress_value->setTextAlignment(CATextAlignmentCenter);
    progress_value->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    this->getView()->addSubview(progress_value);
    
    progress = CAProgress::create();
    progress->setLayout(DLayout(100,100,550,0,DLayout::L_R_T_H));
    CAScheduler::schedule(schedule_selector(ProgressTest::progressValueChange), this, 0.01, false);
    this->getView()->addSubview(progress);
}

void ProgressTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void ProgressTest::progressValueChange(float _t)
{
    char valueText[20] = "";
    float currentValue = progress->getProgress() + 0.001;
    progress->setProgress(currentValue);
    sprintf(valueText,"%.02f%%",currentValue*100-0.1);
    progress_value->setText(valueText);
    
}

