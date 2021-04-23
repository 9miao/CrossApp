
#include "DrawViewTest.h"


DrawViewTest::DrawViewTest()
{
    this->setTitle("CADrawView");
}

DrawViewTest::~DrawViewTest()
{

}

void DrawViewTest::viewDidLoad()
{
    CADrawView* drawView = CADrawView::create() ;
    drawView->setLayout(DLayoutFill);
    this->getView()->addSubview(drawView) ;
    
    float width = this->getView()->getBounds().size.width;
    
    /** 线 */
    drawView->drawLine(DPoint(width * 0.2, 50), DPoint(width * 0.8, 50), CAColor4B::BLUE) ;
    
    //point (left-bottom , right-bottom , right-top , left-top)
    
    /** 矩形 */
    drawView->drawRect(DPoint(width * 0.25, 100), DPoint(width * 0.75, 100), DPoint(width * 0.75, 220), DPoint(width * 0.25, 220), CAColor4B::RED) ;
    
    /** 实心多边形 */
    drawView->drawSolidCircle(DPoint(width * 0.5, 400), 100, 270, 5, 1, 1, CAColor4B::YELLOW) ;
    
    /** 斜线 */
    drawView->drawSegment(DPoint(width * 0.3, 600), DPoint(width * 0.7 , 600), 5.0, CAColor4B::BLUE) ;
    
    /** 带中线的圆 */
    drawView->drawCircle(DPoint(width * 0.3, 800), 30, 270, 360, true, 3, 3, CAColor4B::MAGENTA) ;
    
    /** 不带中线的圆 */
    drawView->drawCircle(DPoint(width * 0.7, 800), 30, 270, 360, false, 3, 3, CAColor4B::MAGENTA) ;
    
    /* 菱形 */
    drawView->drawCircle(DPoint(width * 0.5, 990), 30, 270, 4, false, 3, 3, CAColor4B::ORANGE) ;
}

void DrawViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
