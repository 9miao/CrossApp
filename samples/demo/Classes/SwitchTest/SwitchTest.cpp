#include "SwitchTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

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
	size = this->getView()->getBounds().size;

	CASwitch* defaultSwitch = CASwitch::createWithCenter(CADipRect(size.width*0.5, size.height*0.2, size.width*0.3, 20));
	defaultSwitch->setTag(100);
	defaultSwitch->addTarget(this,CAControl_selector(SwitchTest::switchStateChange));
	this->getView()->addSubview(defaultSwitch);

	CASwitch* customSwitch = CASwitch::createWithCenter(CADipRect(size.width*0.5, size.height*0.4, size.width*0.3, 20));
	customSwitch->setTag(101);
	customSwitch->setOnImage(CAImage::create("source_material/btn_rounded_highlighted.png"));
	customSwitch->setOffImage(CAImage::create("source_material/btn_rounded_normal.png"));
	customSwitch->setThumbTintImage(CAImage::create("source_material/btn_rounded3D_selected.png"));
	customSwitch->addTarget(this, CAControl_selector(SwitchTest::switchStateChange));
	this->getView()->addSubview(customSwitch);

	switchState = CALabel::createWithCenter(CADipRect(size.width*0.5, size.height*0.6, size.width*0.5, 50));
	switchState->setColor(CAColor_blueStyle);
	switchState->setText("Switch State:Off");
	switchState->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	switchState->setTextAlignment(CATextAlignmentCenter);
	switchState->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->getView()->addSubview(switchState);
}

void SwitchTest::switchStateChange(CAControl* btn, CCPoint point)
{
	CASwitch* state = (CASwitch*)btn;
	if (!state->isOn())
	{
		switchState->setText("Switch State:Off");
	}
	else
	{
		switchState->setText("Switch State:On");
	}
}

void SwitchTest::viewDidUnload()
{

}