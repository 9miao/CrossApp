#include "LabelTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

LabelTest::LabelTest()
{
	introduce = "CrossApp is a cross platform application development engine,and completely free open source";
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

void LabelTest::textSetAlignment()
{
	CALabel* textAlignment = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.05, size.width*0.8, 50*CROSSAPP_ADPTATION_RATIO));
	textAlignment->setText("Horizontal alignment");
	textAlignment->setTextAlignment(CATextAlignmentCenter);
	textAlignment->setColor(CAColor_blueStyle);
	textAlignment->setFontSize(40*CROSSAPP_ADPTATION_RATIO);
	this->getView()->addSubview(textAlignment);

	CALabel* left = CALabel::createWithFrame(CCRect(size.width*0.05, size.height*0.1, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	left->setText("(AlignmentLeft)");
	left->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	left->setTextAlignment(CATextAlignmentLeft);
	left->setColor(CAColor_blueStyle);
	this->getView()->addSubview(left);

	CALabel* leftAlignment = CALabel::createWithFrame(CCRect(size.width*0.05, size.height*0.15, size.width*0.3, size.height*0.4));
	leftAlignment->setText(introduce.c_str());
	leftAlignment->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	leftAlignment->setTextAlignment(CATextAlignmentLeft);
	leftAlignment->setColor(CAColor_gray);
	this->getView()->addSubview(leftAlignment);

	CALabel* center = CALabel::createWithFrame(CCRect(size.width*0.35, size.height*0.1, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	center->setText("(AlignmentCenter)");
	center->setTextAlignment(CATextAlignmentCenter);
	center->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	center->setColor(CAColor_blueStyle);
	this->getView()->addSubview(center);

	CALabel* centerAlignment = CALabel::createWithFrame(CCRect(size.width*0.35, size.height*0.15, size.width*0.3, size.height*0.4));
	centerAlignment->setText(introduce.c_str());
	centerAlignment->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	centerAlignment->setTextAlignment(CATextAlignmentCenter);
	centerAlignment->setColor(CAColor_yellow);
	this->getView()->addSubview(centerAlignment);

	CALabel* right = CALabel::createWithFrame(CCRect(size.width*0.65, size.height*0.1, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	right->setText("(AlignmentRight)");
	right->setTextAlignment(CATextAlignmentRight);
	right->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	right->setColor(CAColor_blueStyle);
	this->getView()->addSubview(right);

	CALabel* rightAlignment = CALabel::createWithFrame(CCRect(size.width*0.65, size.height*0.15, size.width*0.3, size.height*0.4));
	rightAlignment->setText(introduce.c_str());
	rightAlignment->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	rightAlignment->setTextAlignment(CATextAlignmentRight);
	rightAlignment->setColor(CAColor_green);
	this->getView()->addSubview(rightAlignment);
}

void LabelTest::textSetVerticalAlignment()
{
	CALabel* textAlignment = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.55, size.width*0.8, 50 * CROSSAPP_ADPTATION_RATIO));
	textAlignment->setText("Vertical alignment");
	textAlignment->setTextAlignment(CATextAlignmentCenter);
	textAlignment->setColor(CAColor_blueStyle);
	textAlignment->setFontSize(40 * CROSSAPP_ADPTATION_RATIO);
	this->getView()->addSubview(textAlignment);

	CALabel* left = CALabel::createWithFrame(CCRect(size.width*0.05, size.height*0.6, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	left->setText("(AlignmentTop)");
	left->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	left->setColor(CAColor_blueStyle);
	this->getView()->addSubview(left);

	CALabel* leftAlignment = CALabel::createWithFrame(CCRect(size.width*0.05, size.height*0.65, size.width*0.3, size.height*0.4));
	leftAlignment->setText(introduce.c_str());
	leftAlignment->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	leftAlignment->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	leftAlignment->setColor(CAColor_gray);
	this->getView()->addSubview(leftAlignment);

	CALabel* center = CALabel::createWithFrame(CCRect(size.width*0.35, size.height*0.6, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	center->setText("(AlignmentCenter)");
	center->setTextAlignment(CATextAlignmentCenter);
	center->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	center->setColor(CAColor_blueStyle);
	this->getView()->addSubview(center);

	CALabel* centerAlignment = CALabel::createWithFrame(CCRect(size.width*0.35, size.height*0.65, size.width*0.3, size.height*0.4));
	centerAlignment->setText(introduce.c_str());
	centerAlignment->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	centerAlignment->setTextAlignment(CATextAlignmentCenter);
	centerAlignment->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	centerAlignment->setColor(CAColor_yellow);
	this->getView()->addSubview(centerAlignment);

	CALabel* right = CALabel::createWithFrame(CCRect(size.width*0.65, size.height*0.6, size.width*0.3, 50 * CROSSAPP_ADPTATION_RATIO));
	right->setText("(AlignmentBottom)");
	right->setTextAlignment(CATextAlignmentRight);
	right->setFontSize(20 * CROSSAPP_ADPTATION_RATIO);
	right->setColor(CAColor_blueStyle);
	this->getView()->addSubview(right);

	CALabel* rightAlignment = CALabel::createWithFrame(CCRect(size.width*0.65, size.height*0.65, size.width*0.3, size.height*0.4));
	rightAlignment->setText(introduce.c_str());
	rightAlignment->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	rightAlignment->setTextAlignment(CATextAlignmentRight);
	rightAlignment->setVerticalTextAlignmet(CAVerticalTextAlignmentBottom);
	rightAlignment->setColor(CAColor_green);
	this->getView()->addSubview(rightAlignment);
}

void LabelTest::viewDidUnload()
{

}