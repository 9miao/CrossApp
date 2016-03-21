
#include "Scale9ImageViewTest.h"

Scale9ImageViewTest::Scale9ImageViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

Scale9ImageViewTest::~Scale9ImageViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void Scale9ImageViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    CAScale9ImageView* s9image = CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded_normal.png"));
    s9image->setLayout(DRectLayout(200,200,500,200,DRectLayout::L_R_T_H));
    this->getView()->addSubview(s9image);
}

void Scale9ImageViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

