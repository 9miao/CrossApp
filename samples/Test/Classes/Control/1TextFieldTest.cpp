
#include "TextFieldTest.h"

TextFieldTest::TextFieldTest():pageViewIndex(0)
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
    winSize = this->getView()->getBounds().size;
    winRect = this->getView()->getBounds();
    this->getView()->setColor(CAColor_gray);
    
    showIndex = 0;
    VIEWLIST.clear();
    
    CATextField* textField = CATextField::createWithLayout(DRectLayout(100,100,300,100,DRectLayout::L_R_T_H));
    textField->setTag(100);
    textField->setPlaceHolderText("Input");
    textField->setKeyboardType(CATextField::Default);
    this->getView()->addSubview(textField);
    //textField->becomeFirstResponder();
    showNum = 1;
}

void TextFieldTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
