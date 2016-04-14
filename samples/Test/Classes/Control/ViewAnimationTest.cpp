
#include "ViewAnimationTest.h"
#include "CDUIShowAutoCollectionView.h"

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

int AnimationNum = 0;

void CDUIShowAutoCollectionView::ViewAnimationRightBtnRightcallback(CAControl* control, DPoint point)
{
    if (showViewAnimationNavigationBar >= 2)
    {
        showViewAnimationNavigationBar = 0;
        AnimationNum = showViewAnimationNavigationBar;
    }
    else
    {
        AnimationNum = ++showViewAnimationNavigationBar;
    }
    ViewAnimationTest* ViewContrllerViewAnimationTest = new ViewAnimationTest();
    ViewContrllerViewAnimationTest->init();
    ViewContrllerViewAnimationTest->setNavigationBarItem(ViewAnimationNavigationBar);
    ViewContrllerViewAnimationTest->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->replaceViewController(ViewContrllerViewAnimationTest, false);
}

void ViewAnimationTest::viewDidLoad()
{
    winSize = this->getView()->getBounds().size;
    winRect = this->getView()->getBounds();
    
    if (AnimationNum == 0)
    {
        //Animation 1
        heart_index = 0;
        CAButton* btn1 = CAButton::create(CAButtonTypeRoundedRect);
        btn1->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(56, 0.25)));
        btn1->setTitleForState(CAControlStateNormal, "Play Animation");
        btn1->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
        btn1->setTag(100);
        btn1->addTarget(this, CAControl_selector(ViewAnimationTest::doAction), CAControlEventTouchUpInSide);
        animation_1_view = CAImageView::createWithLayout(DLayout(DHorizontalLayout_W_C(28, 0.5), DVerticalLayout_H_C(24, 0.5)));
        animation_1_view->setImage(CAImage::create("image/heart1.png"));
    
        CAView* view1 = CAView::createWithLayout(DLayoutFill);
        view1->addSubview(btn1);
        view1->addSubview(animation_1_view);
        view1->setColor(CAColor_gray);
        this->getView()->addSubview(view1);
    }
    else if (AnimationNum == 1)
    {
        //Animation 2
        CAImageView* bg = CAImageView::createWithLayout(DLayout(DHorizontalLayout_L_C(0, 0.5), DVerticalLayout_H_C(80, 0.5)));
        bg->setImage(CAImage::create("image/navbg.jpg"));
    
//        animation_2_textfield = CATextField::createWithFrame(DRect(90,winSize.height/2-25,100,50));
        animation_2_textfield = CATextField::createWithLayout(DLayout(DHorizontalLayout_L_W(90, 100), DVerticalLayout_H_C(50, 0.5)));
        animation_2_textfield->setFontSize(40);
        animation_2_textfield->setKeyboardType(CATextField::Default);
        animation_2_textfield->setVisible(false);
    
//        animation_2_btn_search = CAButton::createWithCenter(DRect(70,winSize.height/2,56,48), CAButtonTypeCustom);
        animation_2_btn_search = CAButton::createWithLayout(DLayout(DHorizontalLayout_L_W(25, 56), DVerticalLayout_H_C(48, 0.5)), CAButtonTypeCustom);
        animation_2_btn_search->setImageForState(CAControlStateNormal, CAImage::create("image/search_btn.png"));
        animation_2_btn_search->addTarget(this, CAControl_selector(ViewAnimationTest::doAction), CAControlEventTouchUpInSide);
        animation_2_btn_search->setTag(201);
    
//        animation_2_btn_cancel = CAButton::createWithCenter(DRect(winSize.width-60,winSize.height/2,100,50), CAButtonTypeCustom);
        animation_2_btn_cancel = CAButton::createWithLayout(DLayout(DHorizontalLayout_R_W(20, 100), DVerticalLayout_H_C(50, 0.5)), CAButtonTypeCustom);
        animation_2_btn_cancel->setTitleForState(CAControlStateNormal, "Cancel");
        animation_2_btn_cancel->setTag(202);
        animation_2_btn_cancel->setTitleColorForState(CAControlStateNormal, CAColor_white);
        animation_2_btn_cancel->addTarget(this, CAControl_selector(ViewAnimationTest::doAction), CAControlEventTouchUpInSide);
        animation_2_btn_cancel->setVisible(false);
    
        CAView* view2 = CAView::createWithLayout(DLayoutFill);
        view2->addSubview(bg);
        view2->addSubview(animation_2_textfield);
        view2->addSubview(animation_2_btn_search);
        view2->addSubview(animation_2_btn_cancel);
        view2->setColor(CAColor_gray);
        this->getView()->addSubview(view2);
    }
    else
    {
        //Animation 3
        animation_3_imageview = CAImageView::createWithLayout(DLayoutFill);
        animation_3_imageview->setImage(CAImage::create("image/2.jpg"));
    
        CAButton* btn3 = CAButton::create(CAButtonTypeRoundedRect);
        btn3->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(56, 0.25)));
        btn3->setTitleForState(CAControlStateNormal, "Play Animation");
        btn3->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
        btn3->setTag(300);
        btn3->addTarget(this, CAControl_selector(ViewAnimationTest::doAction), CAControlEventTouchUpInSide);
    
        CAView* view3 = CAView::createWithLayout(DLayoutFill);
        view3->addSubview(animation_3_imageview);
        view3->addSubview(btn3);
        view3->setColor(CAColor_gray);
        this->getView()->addSubview(view3);
    }

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
//        animation_2_textfield->setLayout(DRectLayout(90,500,560,60,DRectLayout::L_R_T_H));
//        animation_2_textfield->setLayout(DLayout(DHorizontalLayout_L_W(90, 100), DVerticalLayout_H_C(50, 0.5)));
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.3f);
        animation_2_textfield->setFrame(DRect(90,winSize.height/2-25,winSize.width-200,50));
//        animation_2_textfield->setLayout(DRectLayout(90,100,560,60,DRectLayout::L_R_T_H));
//        animation_2_textfield->setLayout(DLayout(DHorizontalLayout_L_W(90, 400), DVerticalLayout_H_C(50, 0.5)));
        animation_2_textfield->setAlpha(1);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(ViewAnimationTest::endAction));
        CAViewAnimation::commitAnimations();
    }else if(tag==202){
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.3f);
        animation_2_textfield->setFrame(DRect(90,winSize.height/2-25,0,50));
//        animation_2_textfield->setLayout(DLayout(DHorizontalLayout_L_W(90, 0), DVerticalLayout_H_C(50, 0.5)));
        animation_2_textfield->setAlpha(0);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(ViewAnimationTest::endAction));
        CAViewAnimation::commitAnimations();
    }else if(tag==300){
        animation_3_imageview->setImageRect(DRect(0,0,100,winSize.height));
        animation_3_imageview->setFrame(DRect(0,0,100,winSize.height));
//        animation_3_imageview->setLayout(DLayout(DHorizontalLayout_L_W(0, 100), DVerticalLayout_T_C(0, 0.5)));
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.8f);
        CAViewAnimation::setAnimationRepeatAutoreverses(true);
        animation_3_imageview->setImageRect(DRect(winSize.width-100,0,100,winSize.height));
        animation_3_imageview->setFrame(DRect(winSize.width-100,0,100,winSize.height));
//        animation_3_imageview->setLayout(DLayout(DHorizontalLayout_R_W(0, 100), DVerticalLayout_T_C(0, 0.5)));
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
