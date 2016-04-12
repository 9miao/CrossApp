
#include "TextViewTest.h"

TextViewTest::TextViewTest():pageViewIndex(0)
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

TextViewTest::~TextViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void TextViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    CATextView* textView = CATextView::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_B(100, 100)));
    this->getView()->addSubview(textView);
}

void TextViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
