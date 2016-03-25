
#include "SegmentedControlTest.h"

SegmentedControlTest::SegmentedControlTest():pageViewIndex(0)
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

SegmentedControlTest::~SegmentedControlTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void SegmentedControlTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    showNum = 2;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
//    CASegmentedControl* segment1 = CASegmentedControl::createWithCenter(DRect(winSize.width/2,winSize.height/2,winSize.width/2,50), 3);
    CASegmentedControl* segment1 = CASegmentedControl::createWithLayout(DRectLayout(150,150,500,80,DRectLayout::L_R_T_H), 3);
    
    char temstr[20];
    for (int i = 0; i < 3; i++)
    {
        sprintf(temstr, "Page%d", i + 1);
        segment1->setTitleForSegmentAtIndex(temstr, i);
    }
    segment1->setTitleColor(CAColor_black);
    segment1->setTitleSelectedColor(CAColor_white);
    segment1->addTarget(this, CASegmentedControl_selector(SegmentedControlTest::segmentCallback));
    segment1->setSelectedAtIndex(0);
    
    CAView* view1 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view1->addSubview(segment1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CASegmentedControl* segment2 = CASegmentedControl::createWithLayout(DRectLayout(150,150,500,80,DRectLayout::L_R_T_H), 3);
    
    char temstr2[20];
    for (int i = 0; i < 3; i++)
    {
        sprintf(temstr2, "Page%d", i + 1);
        segment2->setTitleForSegmentAtIndex(temstr2, i);
    }
    segment2->setTitleColor(CAColor_black);
    segment2->setTitleSelectedColor(CAColor_white);
    
    segment2->addTarget(this, CASegmentedControl_selector(SegmentedControlTest::segmentCallback));
    segment2->setBackgroundImage(CAImage::create("image/seg1.png"));
    segment2->setSegmentItemBackgroundImage(CAImage::create("image/seg2.png"));
    segment2->setTintColor(ccc4(44, 178, 93, 255));
    segment2->setSelectedAtIndex(0);
    
    CAView* view2 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view2->addSubview(segment2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    showUI();
    p_PageViewVec->setViews(VIEWLIST);
}

void SegmentedControlTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SegmentedControlTest::segmentCallback(CASegmentedControl* btn, int index)
{
    CCLog("segmentCallback");
}

void SegmentedControlTest::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(SegmentedControlTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(SegmentedControlTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);
}

void SegmentedControlTest::buttonControlCallBack(CAControl* btn, DPoint point)
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
        this->setNavigationBarItem(CANavigationBarItem::create("Segment"));
    }
    else if(showIndex==1)
    {
        this->setNavigationBarItem(CANavigationBarItem::create("Segment Custem"));
    }
}

void SegmentedControlTest::pageViewDidBeginTurning(CAPageView* pageView)
{

}

void SegmentedControlTest::pageViewDidEndTurning(CAPageView* pageView)
{

}

void SegmentedControlTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{

}
