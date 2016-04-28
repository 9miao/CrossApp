
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
    
    CATextField* textField1 = CATextField::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_H(200, 100)));
    textField1->setTag(100);
    //PlaceHolder文本内容
    textField1->setPlaceHolderText("Input");
    //键盘类型
    textField1->setKeyboardType(CATextField::Default);
    //TextField的对齐方式
    textField1->setTextFieldAlign(CATextField::Left);
    textField1->setDelegate(this);
    this->getView()->addSubview(textField1);
    
    CATextField* textField2 = CATextField::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_H(450, 100)));
    textField2->setTag(101);
    textField2->setPlaceHolderText("Input");
    textField2->setKeyboardType(CATextField::Default);
    textField2->setMarginImageLeft(DSize(60,60),"source_material/search.png");
    textField2->setClearButtonMode(CATextField::WhileEditing);
    textField2->setTextFieldAlign(CATextField::Left);
    textField2->setSecureTextEntry(true);
    textField2->setDelegate(this);
    this->getView()->addSubview(textField2);
 
}

void TextFieldTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

bool TextFieldTest::textFieldShouldBeginEditing(CATextField* sender)
{
    return true;
}

//If the sender doesn't want to detach from the IME, return true;
bool TextFieldTest::textFieldShouldEndEditing(CATextField* sender)
{

    return true;
}


//
void TextFieldTest::textFieldShouldReturn(CATextField* sender)
{
}


void TextFieldTest::keyBoardHeight(CATextField* sender, int height)
{
    switch (sender->getTag())
    {
        case 100:
        {
            CATextField* textField = dynamic_cast<CATextField*>(this->getView()->getSubviewByTag(101));
            if (textField)
            {
                textField->setText(crossapp_format_string("键盘高度%d", height));
            }
        }
            break;
        case 101:
        {
            CATextField* textField = dynamic_cast<CATextField*>(this->getView()->getSubviewByTag(100));
            if (textField)
            {
                textField->setText(crossapp_format_string("键盘高度%d", height));
            }
        }
            break;
        default:
            break;
    }
}


//Warning!!! Warning!!! Warning!!!  This method is not on the OpenGL thread.
bool TextFieldTest::textFieldShouldChangeCharacters(CATextField* sender,
                                             unsigned int location,
                                             unsigned int lenght,
                                                    const std::string& changedText)
{
    return true;
}


