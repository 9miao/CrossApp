
#include "ButtonTest.h"

ButtonTest::ButtonTest()
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
        
    CAButton* btn1 = CAButton::create(CAButtonTypeCustom);
    //设置tag
    btn1->setTag(1);
    //设置Layout
    btn1->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.16)));
    //设置Button标题的显示文本和字体大小
    btn1->setTitleForState(CAControlStateNormal, "Noborder");
    btn1->setTitleFontSize(40);
    //设置Button的标题颜色和状态
    btn1->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    //添加回调事件
    btn1->addTarget(this, CAControl_selector(ButtonTest::Buttoncallback), CAControlEventTouchUpInSide);
    this->getView()->addSubview(btn1);
    
    CAButton* btn2 = CAButton::create(CAButtonTypeSquareRect);
    //设置tag
    btn2->setTag(2);
    //设置Layout
    btn2->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.32)));
    //设置Button标题的显示文本
    btn2->setTitleForState(CAControlStateAll,"SquareRect");
    //添加回调事件
    btn2->addTarget(this, CAControl_selector(ButtonTest::Buttoncallback), CAControlEventTouchUpInSide);
    this->getView()->addSubview(btn2);
    
    CAButton* btn3 = CAButton::create(CAButtonTypeRoundedRect);
    //设置tag
    btn3->setTag(3);
    //设置Layout
    btn3->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.48)));
    //设置Button标题的显示文本
    btn3->setTitleForState(CAControlStateAll, "RoundedRect");
    //添加回调事件
    btn3->addTarget(this, CAControl_selector(ButtonTest::Buttoncallback), CAControlEventTouchUpInSide);
    this->getView()->addSubview(btn3);
    
    CAButton* btn4 = CAButton::create(CAButtonTypeSquareRect);
    //设置tag
    btn4->setTag(4);
    //设置Layout
    btn4->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.64)));
    //设置Button的标题颜色和状态
    btn4->setTitleColorForState(CAControlStateNormal,CAColor_white);
    //设置Button的背景View
    btn4->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/button_normal.png")));
    //设置Button的背景View
    btn4->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/button_down.png")));
    //添加回调事件
    btn4->addTarget(this, CAControl_selector(ButtonTest::Buttoncallback), CAControlEventTouchDown);
    this->getView()->addSubview(btn4);
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(5);
    btn5->setAllowsSelected(true);
    btn5->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.8)));
    btn5->setTitleForState(CAControlStateNormal, "Normal");
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setTitleForState(CAControlStateSelected, "Selected");
    btn5->setTitleForState(CAControlStateHighlighted, "Highlighted");
    btn5->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_normal.png")));
    btn5->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_highlighted.png")));
    btn5->setBackgroundViewForState(CAControlStateSelected, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_selected.png")));
    btn5->addTarget(this, CAControl_selector(ButtonTest::Buttoncallback), CAControlEventTouchDown);
    this->getView()->addSubview(btn5);

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

