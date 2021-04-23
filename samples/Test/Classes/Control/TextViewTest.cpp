
#include "TextViewTest.h"

TextViewTest::TextViewTest()
{
    this->setTitle("CATextView");
}

TextViewTest::~TextViewTest()
{
}

void TextViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor4B::GRAY);
    
    CATextView* textView = CATextView::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_B(100, 100)));
    this->getView()->addSubview(textView);
}

void TextViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
