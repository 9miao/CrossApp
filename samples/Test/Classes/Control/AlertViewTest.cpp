
#include "AlertViewTest.h"

AlertViewTest::AlertViewTest()
{
    this->setTitle("CAAlertView");
}

AlertViewTest::~AlertViewTest()
{
}


void AlertViewTest::viewDidLoad()
{
    CAButton* btn1 = CAButton::create(CAButton::Type::RoundedRect);
    btn1->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.25)));
    btn1->setTag(100);
    btn1->setTitleForState(CAControl::State::Normal, "Click-1");
    btn1->addTarget([=]()
    {
        
        CAAlertView* alert = CAAlertView::create("Alert", "this is a alert!");
        alert->show([=](int index)
        {
            this->alertCallBack(index);
        });
        

    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(btn1);
    
    CAButton* btn2 = CAButton::create(CAButton::Type::RoundedRect);
    btn2->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.5)));
    btn2->setTag(200);
    btn2->setTitleForState(CAControl::State::Normal, "Click-2");
    btn2->addTarget([=]()
    {
        
        CAAlertView::create("Alert", "this is a alert!", {"ok", "close"})->show([=](int index)
        {
            this->alertCallBack(index);
        });
        
        
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(btn2);
    
    CAButton* btn3 = CAButton::create(CAButton::Type::RoundedRect);
    btn3->setLayout(DLayout(DHorizontalLayout_W_C(240, 0.5), DVerticalLayout_H_C(54, 0.75)));
    btn3->setTag(300);
    btn3->setTitleForState(CAControl::State::Normal, "Click-3");
    btn3->addTarget([=]()
    {
        
        std::string message = std::string("        When a lion was asleep, a little mouse began running up and down beside him. This soon wakened the lion. He was very angry, and caught the mouse in his paws.")
        + "Forgive me, please.\" cried the little mouse.\"I may be able to help you someday.\" The lion was tickled at these words."
        + "He thought, \"How could this little mouse help me?\" However he lifted up his paws and let him go."
        + "A few days later, the lion was caught in a trap."
        + "The hunters wanted to take him alive to the king, so they tied him to a tree, and went away to look for a wagon."
        + "Just then the little mouse passed by, and saw the sad lion."
        + "He went up to him, and soon gnawed away the ropes. \"Was I not right?\" asked the little mouse.";
        
        CAAlertView* alert = CAAlertView::create("Alert", message.c_str());
        alert->setButtonTitles({"button1", "button2", "button3", "button4", "button5", "button6", "button7", "button8", "button9", "button10"});
        alert->show([=](int index)
        {
            this->alertCallBack(index);
        });
        
        
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(btn3);
    
}

void AlertViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void AlertViewTest::alertCallBack(int btnIndex)
{
    CCLog("btnIndex==%d",btnIndex);
}



