#include "ButtonTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

ButtonTest::ButtonTest()
{

}

ButtonTest::~ButtonTest()
{

}

void ButtonTest::viewDidLoad()
{
	CCSize  size = this->getView()->getBounds().size;

	typeView = CAScale9ImageView::createWithImage(CAImage::create("source_material/round1.png"));
	typeView->setFrame(CCRect(1,1,size.width-1,size.height/3));
	this->getView()->addSubview(typeView);

	bkgView = CAScale9ImageView::createWithImage(CAImage::create("source_material/round2.png"));
	bkgView->setFrame(CCRect(1, size.height/3, size.width - 1, size.height/3));
	this->getView()->addSubview(bkgView);

	eventView = CAScale9ImageView::createWithImage(CAImage::create("source_material/round3.png"));
	eventView->setFrame(CCRect(1, size.height*2/3, size.width - 1, size.height/3));
	this->getView()->addSubview(eventView);

	threeButtonType();
	buttonBackground();
	buttonTouchEvent();
}

void ButtonTest::threeButtonType()
{
	CCSize size = typeView->getBounds().size;
	CALabel* buttonType = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.2, size.width*0.4, 50));
	buttonType->setText("DefaultType");
	buttonType->setFontSize(30*CROSSAPP_ADPTATION_RATIO);
	buttonType->setTextAlignment(CATextAlignmentCenter);
	buttonType->setColor(ccc4(51, 204, 255, 255));
	typeView->addSubview(buttonType);

	CAButton* defaultBtn = CAButton::create(CAButtonTypeCustom);
	defaultBtn->setCenter(CCRect(size.width*0.25-50, size.height*0.5, size.width*0.25, size.height*0.1));
	defaultBtn->setTitleForState(CAControlStateNormal, "Noborder");
	defaultBtn->setTitleColorForState(CAControlStateNormal, CAColor_blueStyle);
	typeView->addSubview(defaultBtn);

	CALabel* custom = CALabel::createWithCenter(CCRect(size.width*0.25-50, size.height*0.5+80, size.width*0.3, 50));
	custom->setText("(Custom)");
	custom->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	custom->setTextAlignment(CATextAlignmentCenter);
	custom->setColor(ccc4(51, 204, 255, 255));
	typeView->addSubview(custom);

	CAButton* squareRectBtn = CAButton::create(CAButtonTypeSquareRect);
	squareRectBtn->setCenter(CCRect(size.width*0.5, size.height*0.5, size.width*0.25, size.height*0.1));
	squareRectBtn->setTitleForState(CAControlStateAll,"SquareRect");
	typeView->addSubview(squareRectBtn);

	CALabel* square = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.5 + 80, size.width*0.3, 50));
	square->setText("(SquareRect)");
	square->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	square->setTextAlignment(CATextAlignmentCenter);
	square->setColor(CAColor_blueStyle);
	typeView->addSubview(square);

	CAButton* roundedRectBtn = CAButton::create(CAButtonTypeRoundedRect);
	roundedRectBtn->setCenter(CCRect(size.width*0.75 + 50, size.height*0.5, size.width*0.25, size.height*0.1));
	roundedRectBtn->setTitleForState(CAControlStateAll, "RoundedRect");
	typeView->addSubview(roundedRectBtn);

	CALabel* rounded = CALabel::createWithCenter(CCRect(size.width*0.75+50, size.height*0.5 + 80, size.width*0.3, 50));
	rounded->setText("(RoundedRect)");
	rounded->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	rounded->setTextAlignment(CATextAlignmentCenter);
	rounded->setColor(CAColor_blueStyle);
	typeView->addSubview(rounded);
}

void ButtonTest::buttonBackground()
{
	CCSize size = bkgView->getBounds().size;

	CALabel* buttonImage = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.2, size.width*0.4, 50));
	buttonImage->setText("ButtonImage");
	buttonImage->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	buttonImage->setTextAlignment(CATextAlignmentCenter);
	buttonImage->setColor(ccc4(51, 204, 255, 255));
	bkgView->addSubview(buttonImage);

	CAButton* defaultBtn = CAButton::create(CAButtonTypeCustom);
	defaultBtn->setCenter(CCRect(size.width*0.25 - 50, size.height*0.5, size.width*0.25, size.height*0.1));
	defaultBtn->setTitleForState(CAControlStateNormal, "Normal");
	defaultBtn->setTitleColorForState(CAControlStateNormal, CAColor_white);
	defaultBtn->setTitleForState(CAControlStateSelected,"Selected");
	defaultBtn->setTitleForState(CAControlStateHighlighted, "Highlighted");
	defaultBtn->setBackGroundViewForState(CAControlStateNormal,CAView::createWithColor(CAColor_green));
	defaultBtn->setBackGroundViewForState(CAControlStateHighlighted, CAView::createWithColor(CAColor_yellow));
	bkgView->addSubview(defaultBtn);

	CALabel* custom = CALabel::createWithCenter(CCRect(size.width*0.25 - 50, size.height*0.5 + 80, size.width*0.3, 50));
	custom->setText("(BackgroundView)");
	custom->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	custom->setTextAlignment(CATextAlignmentCenter);
	custom->setColor(CAColor_blueStyle);
	bkgView->addSubview(custom);


	CAButton* squareRectBtn = CAButton::create(CAButtonTypeSquareRect);
	squareRectBtn->setAllowsSelected(true);
	squareRectBtn->setCenter(CCRect(size.width*0.5, size.height*0.5, size.width*0.25, size.height*0.1));
	squareRectBtn->setTitleForState(CAControlStateNormal, "Normal");
	squareRectBtn->setTitleColorForState(CAControlStateNormal, CAColor_white);
	squareRectBtn->setTitleForState(CAControlStateSelected, "Selected");
	squareRectBtn->setTitleForState(CAControlStateHighlighted, "Highlighted");
	squareRectBtn->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_normal.png")));
	squareRectBtn->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/ex4.png")));
	squareRectBtn->setBackGroundViewForState(CAControlStateSelected, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_selected.png")));
	bkgView->addSubview(squareRectBtn);

	CALabel* square = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.5 + 80, size.width*0.3, 50));
	square->setText("(StateImage)");
	square->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	square->setTextAlignment(CATextAlignmentCenter);
	square->setColor(CAColor_blueStyle);
	bkgView->addSubview(square);

	CAButton* roundedRectBtn = CAButton::create(CAButtonTypeRoundedRect);
	roundedRectBtn->setCenter(CCRect(size.width*0.75 + 50, size.height*0.5, size.width*0.25, size.height*0.1));
	roundedRectBtn->setBackGroundViewForState(CAControlStateDisabled, CAScale9ImageView::createWithImage(CAImage::create("source_material/ex5.png")));
	roundedRectBtn->setControlState(CAControlStateDisabled);
	bkgView->addSubview(roundedRectBtn);

	CALabel* rounded = CALabel::createWithCenter(CCRect(size.width*0.75 + 50, size.height*0.5 + 80, size.width*0.3, 50));
	rounded->setText("(Disabled)");
	rounded->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	rounded->setTextAlignment(CATextAlignmentCenter);
	rounded->setColor(CAColor_blueStyle);
	bkgView->addSubview(rounded);
}

void ButtonTest::buttonTouchEvent()
{
	CCSize size = eventView->getBounds().size;

	CALabel* buttonTouch = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.2, size.width*0.4, 50));
	buttonTouch->setText("TouchEvent");
	buttonTouch->setFontSize(35 * CROSSAPP_ADPTATION_RATIO);
	buttonTouch->setTextAlignment(CATextAlignmentCenter);
	buttonTouch->setColor(CAColor_blueStyle);
	eventView->addSubview(buttonTouch);

	CAButton* btnOne = CAButton::create(CAButtonTypeCustom);
	btnOne->setCenter(CCRect(size.width*0.25 - 50, size.height*0.5, size.width*0.25, size.height*0.1));
	btnOne->setTag(BUTTONONE);
	btnOne->setTitleForState(CAControlStateAll, "TouchDown");
	btnOne->setTitleColorForState(CAControlStateNormal, CAColor_blueStyle);
	btnOne->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/round1.png")));
	btnOne->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/round2.png")));
	btnOne->addTarget(this, CAControl_selector(ButtonTest::buttonCallback), CAControlEventTouchDown);
	eventView->addSubview(btnOne);

	CAButton* btnTwo = CAButton::create(CAButtonTypeSquareRect);
	btnTwo->setCenter(CCRect(size.width*0.5, size.height*0.5, size.width*0.25, size.height*0.1));
	btnTwo->setTag(BUTTONTWO);
	btnTwo->setTitleForState(CAControlStateAll, "TouchMoved");
	btnTwo->setTitleColorForState(CAControlStateNormal,CAColor_white);
	btnTwo->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_square_highlighted.png")));
	btnTwo->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_square_selected.png")));
	btnTwo->addTarget(this, CAControl_selector(ButtonTest::buttonCallback), CAControlEventTouchMoved);
	eventView->addSubview(btnTwo);

	CAButton* btnThree = CAButton::create(CAButtonTypeRoundedRect);
	btnThree->setCenter(CCRect(size.width*0.75 + 50, size.height*0.5, size.width*0.25, size.height*0.1));
	btnThree->setTag(BUTTONTHREE);
	btnThree->setTitleForState(CAControlStateAll, "TouchUpInSide");
	btnThree->setTitleColorForState(CAControlStateNormal, CAColor_white);
	btnThree->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_highlighted.png")));
	btnThree->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_selected.png")));
	btnThree->addTarget(this, CAControl_selector(ButtonTest::buttonCallback), CAControlEventTouchUpInSide);
	eventView->addSubview(btnThree);

	descTest = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.8, size.width, 50));
	descTest->setText("Display coordinates");
	descTest->setFontSize(30*CROSSAPP_ADPTATION_RATIO);
	descTest->setColor(CAColor_blueStyle);
	descTest->setTextAlignment(CATextAlignmentCenter);
	eventView->addSubview(descTest);
}

void ButtonTest::buttonCallback(CAControl* btn, const CCPoint& point)
{
	CAButton* button = (CAButton*)btn;
	char text[50] = "";
	sprintf(text, "Button %d--Touch point(%.02f,%.02f)", button->getTag()+1,point.x, point.y);
	descTest->setText(text);
}

void ButtonTest::viewDidUnload()
{

}