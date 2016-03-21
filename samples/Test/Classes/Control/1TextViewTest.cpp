
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
    winSize = this->getView()->getBounds().size;
    winRect = this->getView()->getBounds();
    this->getView()->setColor(CAColor_gray);
    
    CATextView* textView = CATextView::createWithLayout(DRectLayout(100,100,100,100, DRectLayout::L_R_T_B));
    this->getView()->addSubview(textView);
}

void TextViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
