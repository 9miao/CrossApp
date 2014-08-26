#include "ProgressTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

ProgressTest::ProgressTest()
{

}

ProgressTest::~ProgressTest()
{

}

void ProgressTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	progressValue = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.3, size.width*0.2, 50));
	progressValue->setText("");
	progressValue->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	progressValue->setColor(CAColor_blueStyle);
	progressValue->setTextAlignment(CATextAlignmentCenter);
	progressValue->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->getView()->addSubview(progressValue);

	firstProgress = CAProgress::create();
	firstProgress->setCenter(CCRect(size.width*0.5, size.height*0.5, size.width*0.4, 50));
	this->getView()->addSubview(firstProgress);

	CAScheduler::schedule(schedule_selector(ProgressTest::firstProgressValueChange), this, 0.01f, false);

	secondProgress = CAProgress::create();
	secondProgress->setCenter(CCRect(size.width*0.5, size.height*0.7, size.width*0.4, 50));
	secondProgress->setProgressTrackImage(CAImage::create("source_material/btn_rounded_highlighted.png"));
	secondProgress->setProgressTintImage(CAImage::create("source_material/btn_rounded3D_selected.png"));
	this->getView()->addSubview(secondProgress);

	CAScheduler::schedule(schedule_selector(ProgressTest::secondProgressValueChange), this, 0.01f, false);
}

void ProgressTest::firstProgressValueChange(float intevalTime)
{
	char valueText[20] = "";
	float currentValue = firstProgress->getProgress() + 0.001;
	firstProgress->setProgress(currentValue);
	sprintf(valueText,"%.02f%%",currentValue*100-0.1);
	progressValue->setText(valueText);
}

void ProgressTest::secondProgressValueChange(float intevalTime)
{
	float currentVal = secondProgress->getProgress() + 0.002;
	secondProgress->setProgress(currentVal);
}

void ProgressTest::viewDidUnload()
{

}