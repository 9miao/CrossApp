
#include "TextFieldTest.h"

TextFieldTest::TextFieldTest()
{
    this->setTitle("CATextField");
}

TextFieldTest::~TextFieldTest()
{
}

void TextFieldTest::viewDidLoad()
{
    this->getView()->setColor(CAColor4B::GRAY);
    
    CATextField* textField1 = CATextField::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_H(200, 100)));
    textField1->setTag(100);
    //PlaceHolder文本内容
    textField1->setPlaceHolderText("Input");
    //键盘类型
    textField1->setKeyboardType(CATextField::KeyboardType::Default);
    //TextField的对齐方式
    textField1->setAlign(CATextField::Align::Left);
    this->getView()->addSubview(textField1);
    
    CATextField* textField2 = CATextField::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_H(450, 100)));
    textField2->setTag(101);
    textField2->setPlaceHolderText("Input");
    textField2->setKeyboardType(CATextField::KeyboardType::Default);
    textField2->setMarginImageLeft(DSize(60,60),"source_material/search.png");
    textField2->setClearButtonMode(CATextField::ClearButtonMode::WhileEditing);
    textField2->setAlign(CATextField::Align::Left);
    textField2->setSecureTextEntry(true);
    this->getView()->addSubview(textField2);
    
}

void TextFieldTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}



