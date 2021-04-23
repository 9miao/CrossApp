
#include "Scale9ImageViewTest.h"

Scale9ImageViewTest::Scale9ImageViewTest()
{
    this->setTitle("CAScale9ImageView");
}

Scale9ImageViewTest::~Scale9ImageViewTest()
{
}

void Scale9ImageViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor4B::GRAY);
    
    CAImageView* image = CAImageView::createWithLayout(DLayout(DHorizontalLayout_L_W(200, 30), DVerticalLayout_T_H(100, 30)));
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

