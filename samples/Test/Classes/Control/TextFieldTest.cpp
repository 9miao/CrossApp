
#include "TextFieldTest.h"

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
    this->getView()->setColor(CAColor_gray);
    
    CATextField* textField1 = CATextField::createWithLayout(DRectLayout(100,100,200,100,DRectLayout::L_R_T_H));
    textField1->setTag(100);
    //PlaceHolder文本内容
    textField1->setPlaceHolderText("Input");
    //键盘类型
    textField1->setKeyboardType(CATextField::Default);
    //TextField的对齐方式
    textField1->setTextFieldAlign(CATextField::Left);
    this->getView()->addSubview(textField1);
    
    CATextField* textField2 = CATextField::createWithLayout(DRectLayout(100,100,450,100,DRectLayout::L_R_T_H));
    textField2->setTag(101);
    textField2->setPlaceHolderText("Input");
    textField2->setKeyboardType(CATextField::Default);
    textField2->setMarginImageLeft(DSize(60,60),"source_material/search.png");
    textField2->setTextFieldAlign(CATextField::Left);
    textField2->setSecureTextEntry(true);
    this->getView()->addSubview(textField2);
 
}

void TextFieldTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


