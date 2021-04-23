
#include "ButtonTest.h"

ButtonTest::ButtonTest()
{
    this->setTitle("CAButton");
}

ButtonTest::~ButtonTest()
{

}

void ButtonTest::viewDidLoad()
{
    this->getView()->setColor(CAColor4B::GRAY);
        
    CAButton* btn1 = CAButton::create(CAButton::Type::Custom);
    //设置tag
    btn1->setTag(1);
    //设置Layout
    btn1->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.16)));
    //设置Button标题的显示文本和字体大小
    btn1->setTitleForState(CAControl::State::Normal, "Noborder");
    btn1->setTitleFontSize(40);
    //设置Button的标题颜色和状态
    btn1->setTitleColorForState(CAControl::State::Normal, ccc4Int(CrossApp::hex2Int("0xff2ec0ff")));
    //添加回调事件
    btn1->addTarget([=]()
    {
        CCLog("Button Noborder");
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(btn1);
    
    CAButton* btn2 = CAButton::create(CAButton::Type::SquareRect);
    //设置tag
    btn2->setTag(2);
    //设置Layout
    btn2->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.32)));
    //设置Button标题的显示文本
    btn2->setTitleForState(CAControl::State::Normal,"SquareRect");
    //添加回调事件
    btn2->addTarget([=]()
    {
        CCLog("Button SquareRect");
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(btn2);

    CAButton* btn3 = CAButton::create(CAButton::Type::RoundedRect);
    //设置tag
    btn3->setTag(3);
    //设置Layout
    btn3->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.48)));
    //设置Button标题的显示文本
    btn3->setTitleForState(CAControl::State::Normal, "RoundedRect");
    //添加回调事件
    btn3->addTarget([=]()
    {
        CCLog("Button RoundedRect");
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(btn3);
    
    CAButton* btn4 = CAButton::create(CAButton::Type::SquareRect);
    //设置tag
    btn4->setTag(4);
    //设置Layout
    btn4->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.64)));
    //设置Button的标题颜色和状态
    btn4->setTitleColorForState(CAControl::State::Normal,CAColor4B::WHITE);
    //设置Button的背景View
    btn4->setBackgroundImageForState(CAControl::State::Normal, CAImage::create("image/button_normal.png"), true);
    //设置Button的背景View
    btn4->setBackgroundImageForState(CAControl::State::Highlighted, CAImage::create("image/button_down.png"), true);
    //添加回调事件
    btn4->addTarget([=]()
    {
        CCLog("Button Custom");
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(btn4);
}

void ButtonTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


