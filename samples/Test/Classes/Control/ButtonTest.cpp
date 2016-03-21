
#include "ButtonTest.h"

ButtonTest::ButtonTest():pageViewIndex(0)
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

ButtonTest::~ButtonTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void ButtonTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);

    showNum = 5;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
        
    CAButton* btn1 = CAButton::create(CAButtonTypeCustom);
    //设置tag
    btn1->setTag(1);
    //设置Layout
//    btn1->setCenter(DRect(winSize.width/2, winSize.height/2, 100, 50));
    btn1->setLayout(DRectLayout(260,260,600,60,DRectLayout::L_R_T_H));
    //设置Button标题的显示文本
    btn1->setTitleForState(CAControlStateNormal, "Noborder");
    //设置Button的标题颜色和状态
    btn1->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    //添加回调事件
    btn1->addTarget(this, CAControl_selector(ButtonTest::Buttoncallback), CAControlEventTouchUpInSide);
    
    CAView* view1 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view1->addSubview(btn1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CAButton* btn2 = CAButton::create(CAButtonTypeSquareRect);
    //设置tag
    btn2->setTag(2);
    //设置Layout
    btn2->setLayout(DRectLayout(260,260,600,60,DRectLayout::L_R_T_H));
    //设置Button标题的显示文本
    btn2->setTitleForState(CAControlStateAll,"SquareRect");
    //添加回调事件
    btn2->addTarget(this, CAControl_selector(ButtonTest::Buttoncallback), CAControlEventTouchUpInSide);
    
    CAView* view2 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view2->addSubview(btn2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    CAButton* btn3 = CAButton::create(CAButtonTypeRoundedRect);
    //设置tag
    btn3->setTag(3);
    //设置Layout
    btn3->setLayout(DRectLayout(260,260,600,60,DRectLayout::L_R_T_H));
    //设置Button标题的显示文本
    btn3->setTitleForState(CAControlStateAll, "RoundedRect");
    //添加回调事件
    btn3->addTarget(this, CAControl_selector(ButtonTest::Buttoncallback), CAControlEventTouchUpInSide);
    
    CAView* view3 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view3->addSubview(btn3);
    view3->setColor(CAColor_gray);
    VIEWLIST.pushBack(view3);
    
    CAButton* btn4 = CAButton::create(CAButtonTypeSquareRect);
    //设置tag
    btn4->setTag(4);
    //设置Layout
    btn4->setLayout(DRectLayout(260,260,600,60,DRectLayout::L_R_T_H));
    //设置Button的标题颜色和状态
    btn4->setTitleColorForState(CAControlStateNormal,CAColor_white);
    //设置Button的背景View
    btn4->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/button_normal.png")));
    //设置Button的背景View
    btn4->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/button_down.png")));
    //添加回调事件
    btn4->addTarget(this, CAControl_selector(ButtonTest::Buttoncallback), CAControlEventTouchDown);
    
    CAView* view4 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view4->addSubview(btn4);
    view4->setColor(CAColor_gray);
    VIEWLIST.pushBack(view4);
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(5);
    btn5->setAllowsSelected(true);
    btn5->setLayout(DRectLayout(260,260,600,60,DRectLayout::L_R_T_H));
    btn5->setTitleForState(CAControlStateNormal, "Normal");
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setTitleForState(CAControlStateSelected, "Selected");
    btn5->setTitleForState(CAControlStateHighlighted, "Highlighted");
    btn5->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_normal.png")));
    btn5->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_highlighted.png")));
    btn5->setBackgroundViewForState(CAControlStateSelected, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_selected.png")));
    btn5->addTarget(this, CAControl_selector(ButtonTest::Buttoncallback), CAControlEventTouchDown);
    
    CAView* view5 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view5->addSubview(btn5);
    view5->setColor(CAColor_gray);
    VIEWLIST.pushBack(view5);

    showUI();
    p_PageViewVec->setViews(VIEWLIST);
}

void ButtonTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void ButtonTest::Buttoncallback(CAControl* btn, DPoint point)
{
    int tag = btn->getTag();
    switch (tag) {
        case 1:
            CCLog("Button Noborder");
            break;
        case 2:
            CCLog("Button SquareRect");
            break;
        case 3:
            CCLog("Button RoundedRect");
            break;
        case 4:
            CCLog("Button Custom");
            break;
        case 5:
            CCLog("Button AllowsSelected");
            break;
        default:
            break;
    }
}

void ButtonTest::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(ButtonTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(ButtonTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);
}

void ButtonTest::buttonControlCallBack(CAControl* btn, DPoint point)
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
    
    if (showIndex==0) {
        this->setNavigationBarItem(CANavigationBarItem::create("Button Noborder"));
    }else if(showIndex==1){
        this->setNavigationBarItem(CANavigationBarItem::create("Button SquareRect"));
    }else if(showIndex==2){
        this->setNavigationBarItem(CANavigationBarItem::create("Button RoundedRect"));
    }else if(showIndex==3){
        this->setNavigationBarItem(CANavigationBarItem::create("Button Custom"));
    }else if(showIndex==4){
        this->setNavigationBarItem(CANavigationBarItem::create("Button AllowsSelected"));
    }
}

void ButtonTest::pageViewDidBeginTurning(CAPageView* pageView)
{

}

void ButtonTest::pageViewDidEndTurning(CAPageView* pageView)
{

}

void ButtonTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{

}

