#include "LabelTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

LabelTest::LabelTest()
{
	introduce = "CrossApp is a cross platform application development engine";
}

LabelTest::~LabelTest()
{

}

bool LabelTest::init()
{
    if (!CAViewController::init())
    {
        return false;
    }
    
    return true;
}

void LabelTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;
    
	textSetAlignment();
	textSetVerticalAlignment();
}

void LabelTest::textSetAlignment(void)
{
	CALabel* textAlignment = CALabel::createWithCenter(CADipRect(size.width*0.5, size.height*0.05, size.width*0.8, 50*CROSSAPP_ADPTATION_RATIO));
	textAlignment->setText("Horizontal alignment");
	textAlignment->setTextAlignment(CATextAlignmentCenter);
	textAlignment->setColor(CAColor_blueStyle);
	textAlignment->setFontSize(_px(40));
	this->getView()->addSubview(textAlignment);

	CALabel* left = CALabel::createWithFrame(CADipRect(size.width*0.05, size.height*0.1, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	left->setText("(AlignmentLeft)");
	left->setFontSize(_px(20));
	left->setTextAlignment(CATextAlignmentLeft);
	left->setColor(CAColor_blueStyle);
	this->getView()->addSubview(left);

	CALabel* leftAlignment = CALabel::createWithFrame(CADipRect(size.width*0.05, size.height*0.15, size.width*0.3, size.height*0.4));
	leftAlignment->setText(introduce.c_str());
	leftAlignment->setFontSize(_px(30));
	leftAlignment->setTextAlignment(CATextAlignmentLeft);
	leftAlignment->setColor(CAColor_gray);
	this->getView()->addSubview(leftAlignment);

	CALabel* center = CALabel::createWithFrame(CADipRect(size.width*0.35, size.height*0.1, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	center->setText("(AlignmentCenter)");
	center->setTextAlignment(CATextAlignmentCenter);
	center->setFontSize(_px(20));
	center->setColor(CAColor_blueStyle);
	this->getView()->addSubview(center);

	CALabel* centerAlignment = CALabel::createWithFrame(CADipRect(size.width*0.35, size.height*0.15, size.width*0.3, size.height*0.4));
	centerAlignment->setText(introduce.c_str());
	centerAlignment->setFontSize(_px(30));
	centerAlignment->setTextAlignment(CATextAlignmentCenter);
	centerAlignment->setColor(CAColor_yellow);
	this->getView()->addSubview(centerAlignment);

	CALabel* right = CALabel::createWithFrame(CADipRect(size.width*0.65, size.height*0.1, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	right->setText("(AlignmentRight)");
	right->setTextAlignment(CATextAlignmentRight);
	right->setFontSize(_px(20));
	right->setColor(CAColor_blueStyle);
	this->getView()->addSubview(right);

	CALabel* rightAlignment = CALabel::createWithFrame(CADipRect(size.width*0.65, size.height*0.15, size.width*0.3, size.height*0.4));
	rightAlignment->setText(introduce.c_str());
	rightAlignment->setFontSize(_px(30));
	rightAlignment->setTextAlignment(CATextAlignmentRight);
	rightAlignment->setColor(CAColor_green);
	this->getView()->addSubview(rightAlignment);
}

void LabelTest::textSetVerticalAlignment(void)
{
	CALabel* textAlignment = CALabel::createWithCenter(CADipRect(size.width*0.5, size.height*0.55, size.width*0.8, 50 * CROSSAPP_ADPTATION_RATIO));
	textAlignment->setText("Vertical alignment");
	textAlignment->setTextAlignment(CATextAlignmentCenter);
	textAlignment->setColor(CAColor_blueStyle);
	textAlignment->setFontSize(_px(40));
	this->getView()->addSubview(textAlignment);

	CALabel* left = CALabel::createWithFrame(CADipRect(size.width*0.05, size.height*0.6, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	left->setText("(AlignmentTop)");
	left->setFontSize(_px(20));
	left->setColor(CAColor_blueStyle);
	this->getView()->addSubview(left);

	CALabel* leftAlignment = CALabel::createWithFrame(CADipRect(size.width*0.05, size.height*0.65, size.width*0.3, size.height*0.4));
	leftAlignment->setText(introduce.c_str());
	leftAlignment->setFontSize(_px(30));
	leftAlignment->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	leftAlignment->setColor(CAColor_gray);
	this->getView()->addSubview(leftAlignment);

	CALabel* center = CALabel::createWithFrame(CADipRect(size.width*0.35, size.height*0.6, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	center->setText("(AlignmentCenter)");
	center->setTextAlignment(CATextAlignmentCenter);
	center->setFontSize(_px(20));
	center->setColor(CAColor_blueStyle);
	this->getView()->addSubview(center);

	CALabel* centerAlignment = CALabel::createWithFrame(CADipRect(size.width*0.35, size.height*0.65, size.width*0.3, size.height*0.4));
	centerAlignment->setText(introduce.c_str());
	centerAlignment->setFontSize(_px(30));
	centerAlignment->setTextAlignment(CATextAlignmentCenter);
	centerAlignment->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	centerAlignment->setColor(CAColor_yellow);
	this->getView()->addSubview(centerAlignment);

	CALabel* right = CALabel::createWithFrame(CADipRect(size.width*0.65, size.height*0.6, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	right->setText("(AlignmentBottom)");
	right->setTextAlignment(CATextAlignmentRight);
	right->setFontSize(_px(20));
	right->setColor(CAColor_blueStyle);
	this->getView()->addSubview(right);

	CALabel* rightAlignment = CALabel::createWithFrame(CADipRect(size.width*0.65, size.height*0.65, size.width*0.3, size.height*0.4));
	rightAlignment->setText(introduce.c_str());
	rightAlignment->setFontSize(_px(30));
	rightAlignment->setTextAlignment(CATextAlignmentRight);
	rightAlignment->setVerticalTextAlignmet(CAVerticalTextAlignmentBottom);
	rightAlignment->setColor(CAColor_green);
	this->getView()->addSubview(rightAlignment);
}

void LabelTest::viewDidUnload()
{

}