
#include "SliderTest.h"

SliderTest::SliderTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

SliderTest::~SliderTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void SliderTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    showNum = 2;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
//    sliderValue1 = CALabel::createWithCenter(DRect(winSize.width/2, winSize.height/4, 200, 50));
    sliderValue1 = CALabel::createWithLayout(DRectLayout(100,100,300,50,DRectLayout::L_R_T_H));
    sliderValue1->setColor(ccc4(51,204,255,255));
    sliderValue1->setText("0");
    sliderValue1->setFontSize(30);
    sliderValue1->setTextAlignment(CATextAlignmentCenter);
    sliderValue1->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    
//    CASlider* slider1 = CASlider::createWithCenter(DRect(winSize.width/2,winSize.height/2,winSize.width/2,56));
    CASlider* slider1 = CASlider::createWithLayout(DRectLayout(120,120,500,56,DRectLayout::L_R_T_H));
    slider1->addTarget(this, CAControl_selector(SliderTest::sliderValueChange));
    slider1->setTag(100);
    
    CAView* view1 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view1->addSubview(slider1);
    view1->addSubview(sliderValue1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    sliderValue2 = CALabel::createWithLayout(DRectLayout(100,100,300,50,DRectLayout::L_R_T_H));
    sliderValue2->setColor(ccc4(51, 204, 255, 255));
    sliderValue2->setText("0");
    sliderValue2->setFontSize(30);
    sliderValue2->setTextAlignment(CATextAlignmentCenter);
    sliderValue2->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    
    CASlider* slider2 = CASlider::createWithLayout(DRectLayout(120,120,500,56,DRectLayout::L_R_T_H));
    slider2->setTag(101);
    slider2->setMaxTrackTintImage(CAImage::create("source_material/ex1.png"));
    slider2->setMinTrackTintImage(CAImage::create("source_material/ex3.png"));
    slider2->setThumbTintImage(CAImage::create("source_material/btn_square_highlighted.png"));
    slider2->addTarget(this, CAControl_selector(SliderTest::sliderValueChange));
    
    CAView* view2 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    //view2->addSubview(slider2);
    view2->addSubview(sliderValue2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
  
    showUI();
    p_PageViewVec->setViews(VIEWLIST);

}

void SliderTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SliderTest::sliderValueChange(CAControl* btn, DPoint point)
{
    char value[20] = "";
    CASlider* p_Slider = (CASlider*)btn;
    sprintf(value, "%.02f%%", p_Slider->getValue() * 100);
    if (p_Slider->getTag()==100) {
        sliderValue1->setText(value);
    }else if(p_Slider->getTag()==101)
    {
        sliderValue2->setText(value);
    }
    
}


void SliderTest::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(SliderTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(SliderTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);
}

void SliderTest::buttonControlCallBack(CAControl* btn, DPoint point)
{
    point = btn->convertToWorldSpace(point);
    CAButton* button = (CAButton*)btn;
    CCLog("btn_tag===%d",button->getTag());
    int temIndex = button->getTag();
    if (temIndex==1) {
        if (showIndex>0) {
            showIndex--;
            p_PageViewVec->setCurrPage(showIndex, true); //123456
        }else if(showIndex==0){
            showIndex=showNum-1;
            p_PageViewVec->setCurrPage(showIndex, false);
        }
    }else if(temIndex==2){
        if (showIndex<showNum) {
            showIndex++;
            if (showIndex==showNum) {
                showIndex=0;
                p_PageViewVec->setCurrPage(showIndex, false);
            }else{
                p_PageViewVec->setCurrPage(showIndex, true);
            }
        }
    }
    
    this->getView()->removeSubview(slider);
    if (showIndex==0)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("Slider"));
    }
    else if(showIndex==1)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("Slider Custem"));
    }
}

void SliderTest::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void SliderTest::pageViewDidEndTurning(CAPageView* pageView)
{

}

void SliderTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    //CCLog("Index:%d",index);
}