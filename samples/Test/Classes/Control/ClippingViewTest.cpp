
#include "ClippingViewTest.h"


ClippingViewTest::ClippingViewTest()
{
    this->setTitle("ClippingViewTest");
}

ClippingViewTest::~ClippingViewTest()
{

}

void ClippingViewTest::viewDidLoad()
{
    CAScrollView* scrllVeiw = CAScrollView::createWithLayout(DLayoutFill) ;
    scrllVeiw->setViewSize(DSize(1080, 1920)) ;
    CAImageView* imgView = CAImageView::createWithImage(CAImage::create("image/h1.png")) ;
    imgView->setLayout(DLayoutFill) ;
    scrllVeiw->addSubview(imgView) ;
    
    float width = this->getView()->getBounds().size.width;
    
    CADrawView* drawView = CADrawView::create() ;
    drawView->setLayout(DLayoutFill) ;
    drawView->drawSolidCircle(DPoint(width * 0.5 , 700) , width * 0.4 , 270 , 5  , 1 , 1 , CAColor4B::YELLOW) ;

    CAClippingView* clipView = CAClippingView::create(drawView);
    clipView->setLayout(DLayout(DHorizontalLayout_L_R(0,0), DVerticalLayout_T_B(0,0)));
    clipView->setClippingEnabled(true) ;
    //clipView.setInverted(true);
    this->getView()->addSubview(clipView) ;
    clipView->addSubview(scrllVeiw) ;
}

void ClippingViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
