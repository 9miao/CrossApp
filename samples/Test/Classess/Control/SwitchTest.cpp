
#include "SwitchTest.h"

SwitchTest::SwitchTest():pageViewIndex(0)
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

SwitchTest::~SwitchTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void SwitchTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    showNum = 2;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
//    CASwitch* customSwitch1 = CASwitch::createWithCenter(DRect(winSize.width/2, winSize.height/2, 100, 20));
    CASwitch* customSwitch1 = CASwitch::createWithLayout(DRectLayout(350,350,550,0,DRectLayout::L_R_T_B));
    customSwitch1->setTag(100);
    customSwitch1->setIsOn(true, false);
    customSwitch1->addTarget(this, CAControl_selector(SwitchTest::switchStateChange));
    
    CAView* view1 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view1->addSubview(customSwitch1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CASwitch* customSwitch2 = CASwitch::createWithLayout(DRectLayout(350,350,550,0,DRectLayout::L_R_T_B));
    customSwitch2->setTag(101);
    customSwitch2->setIsOn(true, false);
    customSwitch2->setOnImage(CAImage::create("image/Switch_on.png"));
    customSwitch2->setOffImage(CAImage::create("image/Switch_off.png"));
    customSwitch2->addTarget(this, CAControl_selector(SwitchTest::switchStateChange));
    
    CAView* view2 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view2->addSubview(customSwitch2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    
    showUI();
    p_PageViewVec->setViews(VIEWLIST);
}

void SwitchTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SwitchTest::switchStateChange(CAControl* btn, DPoint point)
{
    CASwitch* state = (CASwitch*)btn;
    if (!state->isOn())
    {
        CCLog("switchStateChange false");
    }
    else
    {
        CCLog("switchStateChange true");
    }
}

void SwitchTest::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(SwitchTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(SwitchTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);
}

void SwitchTest::buttonControlCallBack(CAControl* btn, DPoint point)
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
    
    if (showIndex==0)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("Switch"));
    }
    else if(showIndex==1)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("Switch Custem"));
    }
}

void SwitchTest::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void SwitchTest::pageViewDidEndTurning(CAPageView* pageView)
{

}

void SwitchTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    
}
