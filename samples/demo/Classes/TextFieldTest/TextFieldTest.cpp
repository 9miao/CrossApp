#include "TextFieldTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

TextFieldTest::TextFieldTest()
{

}

TextFieldTest::~TextFieldTest()
{

}

void TextFieldTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	CATextField* textField = CATextField::createWithCenter(CCRect(size.width*0.5, size.height*0.2, size.width*0.6, size.height*0.05));
	textField->setBackGroundImage(CAImage::create("btn_rounded_highlighted.png"));
	textField->setPlaceHolder("Input");
	this->getView()->addSubview(textField);

	CATextField* textField1 = CATextField::createWithCenter(CCRect(size.width*0.5, size.height*0.4, size.width*0.6, size.height*0.05));
	textField1->setBackGroundImage(CAImage::create("ex1.png"));
	textField1->setPlaceHolder("Please enter some infomation");
	textField1->setFontSize(24*CROSSAPP_ADPTATION_RATIO);
	textField1->setSpaceHolderColor(CAColor_yellow);
	this->getView()->addSubview(textField1);

	CATextField* textField2 = CATextField::createWithCenter(CCRect(size.width*0.5, size.height*0.6, size.width*0.6, size.height*0.05));
	textField2->setBackGroundImage(CAImage::create("btn_rounded3D_selected.png"));
	textField2->setPlaceHolder("Please enter some infomation");
	textField2->setFontSize(24 * CROSSAPP_ADPTATION_RATIO);
	textField2->setTextColor(CAColor_red);
	textField2->setSpaceHolderColor(CAColor_blue);
	this->getView()->addSubview(textField2);

	CATextField* textField3 = CATextField::createWithCenter(CCRect(size.width*0.5, size.height*0.8, size.width*0.6, size.height*0.05));
	textField3->setBackGroundImage(CAImage::create("ex3.png"));
	textField3->setPlaceHolder("Please enter some infomation");
	textField3->setFontSize(24 * CROSSAPP_ADPTATION_RATIO);
	textField3->setSpaceHolderColor(CAColor_green);
	textField3->setInputType(KEY_BOARD_INPUT_PASSWORD);
	this->getView()->addSubview(textField3);
}

void TextFieldTest::viewDidUnload() 
{

}