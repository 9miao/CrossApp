
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
    
    CAImageView* image = CAImageView::createWithLayout(DLayout(DHorizontalLayout_L_R(200, 200), DVerticalLayout_T_H(100, 200)));
    image->setImage(CAImage::create("source_material/btn_rounded_normal.png"));
    this->getView()->addSubview(image);
    
    CAScale9ImageView* s9image = CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded_normal.png"));
    s9image->setLayout(DLayout(DHorizontalLayout_L_R(200, 200), DVerticalLayout_T_H(400, 200)));
    this->getView()->addSubview(s9image);
}

void Scale9ImageViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

