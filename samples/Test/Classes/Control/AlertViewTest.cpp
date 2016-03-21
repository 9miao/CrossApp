
#include "AlertViewTest.h"

AlertViewTest::AlertViewTest():pageViewIndex(0)
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);

}

AlertViewTest::~AlertViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);

}

void AlertViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    showNum = 3;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
    for (int i=0; i<showNum; i++) {
        //初始化viewList
        char tem[10];
        sprintf(tem, "Click-%d",i+1);
        CAButton* btn = CAButton::create(CAButtonTypeRoundedRect);
        btn->setLayout(DRectLayout(280,280,550,60,DRectLayout::L_R_T_H));
        btn->setTitleForState(CAControlStateAll, tem);
        btn->addTarget(this, CAControl_selector(AlertViewTest::alertButtonCallBack), CAControlEventTouchUpInSide);
        
        //添加到屏幕
        CAView* view = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
        view->addSubview(btn);
        view->setColor(CAColor_gray);
        VIEWLIST.pushBack(view);
    }
    
    showUI();
    p_PageViewVec->setViews(VIEWLIST);
}

void AlertViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void AlertViewTest::alertButtonCallBack(CAControl* btn,DPoint point)
{
    if (showIndex==0)
    {
        CAAlertView* alertView = CAAlertView::createWithText("Alert", "this is a alert!", "close",NULL);
        alertView->show();
        alertView->setTarget(this, CAAlertView_selector(AlertViewTest::alertCallBack));
    }
    else if(showIndex==1)
    {
        CAAlertView* alertView = CAAlertView::createWithText("Alert", "this is a alert!", "ok","close",NULL);
        alertView->show();
        alertView->setTarget(this, CAAlertView_selector(AlertViewTest::alertCallBack));
    }
    else if(showIndex==2)
    {
        CAAlertView* alertView = CAAlertView::createWithText("Alert", "message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,!", "ok","close","button3","button4","button5","button6","button7",NULL);
        alertView->show();
        alertView->setTarget(this, CAAlertView_selector(AlertViewTest::alertCallBack));
    }
}

void AlertViewTest::alertCallBack(int btnIndex)
{
    CCLog("btnIndex==%d",btnIndex);
}

void AlertViewTest::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(AlertViewTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(AlertViewTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);
}

void AlertViewTest::buttonControlCallBack(CAControl* btn, DPoint point)
{
    point = btn->convertToWorldSpace(point);
    CAButton* button = (CAButton*)btn;
    //CCLog("btn_tag===%d",button->getTag());
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
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("AlertView"));
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("AlertView1"));
    }else if(showIndex==2){
        this->setNavigationBarItem(CANavigationBarItem::create("AlertView2"));
    }
}

void AlertViewTest::pageViewDidBeginTurning(CAPageView* pageView)
{

}

void AlertViewTest::pageViewDidEndTurning(CAPageView* pageView)
{

}

void AlertViewTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{

}


