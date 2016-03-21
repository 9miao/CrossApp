
#include "ViewAnimationTest.h"

ViewAnimationTest::ViewAnimationTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

ViewAnimationTest::~ViewAnimationTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void ViewAnimationTest::viewDidLoad()
{
    winSize = this->getView()->getBounds().size;
    winRect = this->getView()->getBounds();
    this->getView()->setColor(CAColor_gray);
    
    showNum = 3;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
    //Animation 1
    heart_index = 0;
    CAButton* btn1 = CAButton::create(CAButtonTypeRoundedRect);
    btn1->setLayout(DRectLayout(260,260,300,50,DRectLayout::L_R_T_H));
    btn1->setTitleForState(CAControlStateNormal, "Play Animation");
    btn1->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    btn1->setTag(100);
    btn1->addTarget(this, CAControl_selector(ViewAnimationTest::doAction), CAControlEventTouchUpInSide);
    animation_1_view = CAImageView::createWithLayout(DRectLayout(400,400,550,550,DRectLayout::L_R_T_B));
    animation_1_view->setImage(CAImage::create("image/heart1.png"));
    
    CAView* view1 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view1->addSubview(btn1);
    view1->addSubview(animation_1_view);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    //Animation 2
    CAImageView* bg = CAImageView::createWithLayout(DRectLayout(0, 0, 550, 80, DRectLayout::L_R_T_H));
    bg->setImage(CAImage::create("image/navbg.jpg"));
    
    animation_2_textfield = CATextField::createWithLayout(DRectLayout(100,100,100,100,DRectLayout::L_R_T_B));//123456
    animation_2_textfield->setFontSize(40);
    animation_2_textfield->setKeyboardType(CATextField::Default);
    animation_2_textfield->setVisible(false);
    
    animation_2_btn_search = CAButton::createWithCenter(DRect(70,winSize.height/2,56,48), CAButtonTypeCustom);
    animation_2_btn_search->setImageForState(CAControlStateNormal, CAImage::create("image/search_btn.png"));
    animation_2_btn_search->addTarget(this, CAControl_selector(ViewAnimationTest::doAction), CAControlEventTouchUpInSide);
    animation_2_btn_search->setTag(201);
    
    animation_2_btn_cancel = CAButton::createWithCenter(DRect(winSize.width-60,winSize.height/2,100,50), CAButtonTypeCustom);
    animation_2_btn_cancel->setTitleForState(CAControlStateNormal, "Cancel");
    animation_2_btn_cancel->setTag(202);
    animation_2_btn_cancel->setTitleColorForState(CAControlStateNormal, CAColor_white);
    animation_2_btn_cancel->addTarget(this, CAControl_selector(ViewAnimationTest::doAction), CAControlEventTouchUpInSide);
    animation_2_btn_cancel->setVisible(false);
    
    CAView* view2 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view2->addSubview(bg);
    view2->addSubview(animation_2_textfield);
    view2->addSubview(animation_2_btn_search);
    view2->addSubview(animation_2_btn_cancel);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    //Animation 3
    animation_3_imageview = CAImageView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
    animation_3_imageview->setImage(CAImage::create("image/2.jpg"));
    
    CAButton* btn3 = CAButton::create(CAButtonTypeRoundedRect);
    btn3->setCenter(DRect(winSize.width/2, winSize.height/4, 200, 50));
    btn3->setTitleForState(CAControlStateNormal, "Play Animation");
    btn3->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    btn3->setTag(300);
    btn3->addTarget(this, CAControl_selector(ViewAnimationTest::doAction), CAControlEventTouchUpInSide);
    
    CAView* view3 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view3->addSubview(animation_3_imageview);
    view3->addSubview(btn3);
    view3->setColor(CAColor_gray);
    VIEWLIST.pushBack(view3);
    
    
    showUI();
    p_PageViewVec->setViews(VIEWLIST);

}

void ViewAnimationTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void ViewAnimationTest::doAction(CAControl* btn,DPoint point)
{
    CAButton* button = (CAButton*)btn;
    int tag = button->getTag();
    
    if (tag==100) {
        animation_1_view->setScale(1);
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.3f);
        CAViewAnimation::setAnimationRepeatAutoreverses(true);
        if (heart_index==0) {
            animation_1_view->setImage(CAImage::create("image/heart2.png"));
            heart_index = 1;
        }else{
            animation_1_view->setImage(CAImage::create("image/heart1.png"));
            heart_index = 0;
        }
        animation_1_view->setScale(2);
        CAViewAnimation::commitAnimations();
    }else if (tag==201) {
        animation_2_textfield->setVisible(true);
        animation_2_btn_cancel->setVisible(false);
        animation_2_textfield->setAlpha(0);
        animation_2_textfield->setFrame(DRect(90,winSize.height/2-25,100,50));
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.3f);
        animation_2_textfield->setFrame(DRect(90,winSize.height/2-25,winSize.width-200,50));
        animation_2_textfield->setAlpha(1);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(ViewAnimationTest::endAction));
        CAViewAnimation::commitAnimations();
    }else if(tag==202){
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.3f);
        animation_2_textfield->setFrame(DRect(90,winSize.height/2-25,0,50));
        animation_2_textfield->setAlpha(0);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(ViewAnimationTest::endAction));
        CAViewAnimation::commitAnimations();
    }else if(tag==300){
        animation_3_imageview->setImageRect(DRect(0,0,100,winSize.height));
        animation_3_imageview->setFrame(DRect(0,0,100,winSize.height));
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.8f);
        CAViewAnimation::setAnimationRepeatAutoreverses(true);
        animation_3_imageview->setImageRect(DRect(winSize.width-100,0,100,winSize.height));
        animation_3_imageview->setFrame(DRect(winSize.width-100,0,100,winSize.height));
        CAViewAnimation::commitAnimations();
    }
}

void ViewAnimationTest::endAction()
{
    if (animation_2_btn_cancel->isVisible()) {
        animation_2_btn_cancel->setVisible(false);
    }else{
        animation_2_btn_cancel->setVisible(true);
    }
    
}

void ViewAnimationTest::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(ViewAnimationTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(ViewAnimationTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);
}

void ViewAnimationTest::buttonControlCallBack(CAControl* btn, DPoint point)
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

void ViewAnimationTest::pageViewDidBeginTurning(CAPageView* pageView)
{
    CCLog("Begin--->");
}

void ViewAnimationTest::pageViewDidEndTurning(CAPageView* pageView)
{
    CCLog("End--->");
}

void ViewAnimationTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    //CCLog("Index:%d",index);
}
