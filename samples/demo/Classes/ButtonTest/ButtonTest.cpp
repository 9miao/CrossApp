#include "ButtonTest.h"

ButtonTest::ButtonTest()
{

}

ButtonTest::~ButtonTest()
{

}

void ButtonTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	threeButtonType();
	buttonBackground();
	buttonTouchEvent();
}

void ButtonTest::threeButtonType()
{
	CALabel* buttonType = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.05, size.width*0.4, 50));
	buttonType->setText("ButtonType");
	buttonType->setFontSize(30*CROSSAPP_ADPTATION_RATIO);
	buttonType->setTextAlignment(CATextAlignmentCenter);
	buttonType->setColor(ccc4(51, 204, 255, 255));
	this->getView()->addSubview(buttonType);

	CAButton* defaultBtn = CAButton::create(CAButtonTypeCustom);
	defaultBtn->setCenter(CCRect(size.width*0.25-50, size.height*0.15, size.width*0.2, size.height*0.05));
	defaultBtn->setTitleForState(CAControlStateNormal, "Noborder");
	defaultBtn->setTitleColorForState(CAControlStateNormal, ccc4(51, 204, 255, 255));
	this->getView()->addSubview(defaultBtn);

	CALabel* custom = CALabel::createWithCenter(CCRect(size.width*0.25-50, size.height*0.15+80, size.width*0.2, 50));
	custom->setText("(CAButtonTypeCustom)");
	custom->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	custom->setTextAlignment(CATextAlignmentCenter);
	custom->setColor(ccc4(51, 204, 255, 255));
	this->getView()->addSubview(custom);

	CAButton* squareRectBtn = CAButton::create(CAButtonTypeSquareRect);
	squareRectBtn->setCenter(CCRect(size.width*0.5, size.height*0.15, size.width*0.2, size.height*0.05));
	this->getView()->addSubview(squareRectBtn);

	CALabel* square = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.15 + 80, size.width*0.2, 50));
	square->setText("(CAButtonTypeSquareRect)");
	square->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	square->setTextAlignment(CATextAlignmentCenter);
	square->setColor(ccc4(51, 204, 255, 255));
	this->getView()->addSubview(square);

	CAButton* roundedRectBtn = CAButton::create(CAButtonTypeRoundedRect);
	roundedRectBtn->setCenter(CCRect(size.width*0.75 + 50, size.height*0.15, size.width*0.2, size.height*0.05));
	this->getView()->addSubview(roundedRectBtn);

	CALabel* rounded = CALabel::createWithCenter(CCRect(size.width*0.75+50, size.height*0.15 + 80, size.width*0.2, 50));
	rounded->setText("(CAButtonTypeRoundedRect)");
	rounded->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	rounded->setTextAlignment(CATextAlignmentCenter);
	rounded->setColor(ccc4(51, 204, 255, 255));
	this->getView()->addSubview(rounded);
}

void ButtonTest::buttonBackground()
{
	CALabel* buttonImage = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.3, size.width*0.4, 50));
	buttonImage->setText("ButtonImage");
	buttonImage->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	buttonImage->setTextAlignment(CATextAlignmentCenter);
	buttonImage->setColor(ccc4(51, 204, 255, 255));
	this->getView()->addSubview(buttonImage);

	CAButton* defaultBtn = CAButton::create(CAButtonTypeCustom);
	defaultBtn->setCenter(CCRect(size.width*0.25 - 50, size.height*0.4, size.width*0.2, size.height*0.05));
	defaultBtn->setTitleForState(CAControlStateNormal, "Normal");
	defaultBtn->setTitleForState(CAControlStateSelected,"Selected");
	defaultBtn->setTitleForState(CAControlStateHighlighted, "Highlighted");
	defaultBtn->setBackGroundViewForState(CAControlStateNormal,CAView::createWithColor(CAColor_green));
	defaultBtn->setBackGroundViewForState(CAControlStateHighlighted, CAView::createWithColor(CAColor_yellow));
	defaultBtn->setTitleColorForState(CAControlStateAll, ccc4(51, 204, 255, 255));
	this->getView()->addSubview(defaultBtn);

	CALabel* custom = CALabel::createWithCenter(CCRect(size.width*0.25 - 50, size.height*0.4 + 80, size.width*0.2, 50));
	custom->setText("(BackgroundView)");
	custom->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	custom->setTextAlignment(CATextAlignmentCenter);
	custom->setColor(ccc4(51, 204, 255, 255));
	this->getView()->addSubview(custom);


	CAButton* squareRectBtn = CAButton::create(CAButtonTypeSquareRect);
	squareRectBtn->setCenter(CCRect(size.width*0.5, size.height*0.4, size.width*0.2, size.height*0.05));
	squareRectBtn->setImageForState(CAControlStateNormal,CAImage::create("square_nor.png"));
	squareRectBtn->setImageForState(CAControlStateHighlighted, CAImage::create("square_sel.png"));
	this->getView()->addSubview(squareRectBtn);

	CALabel* square = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.4 + 80, size.width*0.2, 50));
	square->setText("(StateImage)");
	square->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	square->setTextAlignment(CATextAlignmentCenter);
	square->setColor(ccc4(51, 204, 255, 255));
	this->getView()->addSubview(square);

	CAButton* roundedRectBtn = CAButton::create(CAButtonTypeRoundedRect);
	roundedRectBtn->setCenter(CCRect(size.width*0.75 + 50, size.height*0.4, size.width*0.2, size.height*0.05));
	roundedRectBtn->setControlState(CAControlStateDisabled);
	this->getView()->addSubview(roundedRectBtn);

	CALabel* rounded = CALabel::createWithCenter(CCRect(size.width*0.75 + 50, size.height*0.4 + 80, size.width*0.2, 50));
	rounded->setText("(Disabled)");
	rounded->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	rounded->setTextAlignment(CATextAlignmentCenter);
	rounded->setColor(ccc4(51, 204, 255, 255));
	this->getView()->addSubview(rounded);
}

void ButtonTest::buttonTouchEvent()
{
	CALabel* buttonTouch = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.55, size.width*0.4, 50));
	buttonTouch->setText("TouchEvent");
	buttonTouch->setFontSize(35 * CROSSAPP_ADPTATION_RATIO);
	buttonTouch->setTextAlignment(CATextAlignmentCenter);
	buttonTouch->setColor(ccc4(51, 204, 255, 255));
	this->getView()->addSubview(buttonTouch);

	CAButton* btnOne = CAButton::create(CAButtonTypeCustom);
	btnOne->setCenter(CCRect(size.width*0.25 - 50, size.height*0.65, size.width*0.2, size.height*0.05));
	btnOne->setTag(BUTTONONE);
	btnOne->setTitleForState(CAControlStateNormal, "Normal");
	btnOne->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
	btnOne->addTarget(this,CAControl_selector(ButtonTest::buttonCallback),CAControlEventTouchUpInSide);
	this->getView()->addSubview(btnOne);

	CAButton* btnTwo = CAButton::create(CAButtonTypeSquareRect);
	btnTwo->setCenter(CCRect(size.width*0.5, size.height*0.65, size.width*0.2, size.height*0.05));
	btnTwo->setTag(BUTTONTWO);
	btnTwo->addTarget(this, CAControl_selector(ButtonTest::buttonCallback), CAControlEventTouchUpInSide);
	this->getView()->addSubview(btnTwo);

	CAButton* btnThree = CAButton::create(CAButtonTypeRoundedRect);
	btnThree->setCenter(CCRect(size.width*0.75 + 50, size.height*0.65, size.width*0.2, size.height*0.05));
	btnThree->setTag(BUTTONTHREE);
	btnThree->addTarget(this, CAControl_selector(ButtonTest::buttonCallback), CAControlEventTouchUpInSide);
	this->getView()->addSubview(btnThree);

	descTest = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.8, size.width*0.9, 50));
	descTest->setText("Display coordinates");
	descTest->setFontSize(30*CROSSAPP_ADPTATION_RATIO);
	descTest->setColor(ccc4(51, 204, 255, 255));
	descTest->setTextAlignment(CATextAlignmentCenter);
	this->getView()->addSubview(descTest);
}

void ButtonTest::buttonCallback(CAControl* btn,CCPoint point)
{
	CAButton* button = (CAButton*)btn;
	char text[50] = "";
	switch (button->getTag())
	{
	case 0:
		sprintf(text,"The first button.Touch point(%.02f,%.02f)",point.x,point.y);
		descTest->setText(text);
		break;

	case 1:
		sprintf(text, "The second button.Touch point(%.02f,%.02f)", point.x, point.y);
		descTest->setText(text);
		break;

	case 2:
		sprintf(text, "The third button.Touch point(%.02f,%.02f)", point.x, point.y);
		descTest->setText(text);
		break;

	default:
		break;
	}
}

void ButtonTest::viewDidUnload()
{

}