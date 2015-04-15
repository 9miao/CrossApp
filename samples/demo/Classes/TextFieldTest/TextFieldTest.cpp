#include "TextFieldTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

TextFieldTest::TextFieldTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

TextFieldTest::~TextFieldTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

void TextFieldTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	CATextField* textField = CATextField::createWithCenter(CADipRect(size.width*0.5, size.height*0.2, size.width*0.6, size.height*0.05));
	textField->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded_highlighted.png")));
	textField->setPlaceHolder("Input");
	textField->setFontSize(78);
	this->getView()->addSubview(textField);

	CATextField* textField1 = CATextField::createWithCenter(CADipRect(size.width*0.5, size.height*0.4, size.width*0.6, size.height*0.05));
	textField1->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/ex1.png")));
	textField1->setPlaceHolder("Please enter some infomation");
	textField1->setFontSize(_px(24));
	textField1->setSpaceHolderColor(CAColor_yellow);
	this->getView()->addSubview(textField1);

	CATextField* textField2 = CATextField::createWithCenter(CADipRect(size.width*0.5, size.height*0.6, size.width*0.6, size.height*0.05));
	textField2->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_selected.png")));
	textField2->setPlaceHolder("Please enter some infomation");
	textField2->setFontSize(_px(24));
	textField2->setTextColor(CAColor_red);
	textField2->setSpaceHolderColor(CAColor_blue);
	this->getView()->addSubview(textField2);

	CATextField* textField3 = CATextField::createWithCenter(CADipRect(size.width*0.5, size.height*0.8, size.width*0.6, size.height*0.05));
	textField3->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/ex3.png")));
	textField3->setPlaceHolder("Please enter some infomation");
	textField3->setFontSize(_px(24));
	textField3->setSpaceHolderColor(CAColor_green);
	textField3->setInputType(KEY_BOARD_INPUT_PASSWORD);
	this->getView()->addSubview(textField3);
}

void TextFieldTest::viewDidUnload() 
{

}