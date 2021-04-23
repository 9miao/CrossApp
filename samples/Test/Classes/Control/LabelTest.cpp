
#include "LabelTest.h"

LabelTest::LabelTest()
:m_iIndex(0)
{
    this->setTitle("CALabel");
}

LabelTest::~LabelTest()
{

}

void LabelTest::viewDidLoad()
{
    std::string s = "Hello World";
    
    std::u32string c;
    StringUtils::UTF8ToUTF32(s, c);
    c += 0x1F604;
    
    std::string cc;
    StringUtils::UTF32ToUTF8(c, cc);
    
    if (m_iIndex == 0)
    {
        CAFont Font;
        Font.fontSize = 50;
        Font.color = CAColor4B::BLUE;
        
        CALabel* label = CALabel::create();
        label->setText(cc);
        label->setFont(Font);
        label->setTextAlignment(CATextAlignment::Center);
        label->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(60, 1/8.f)));
        this->getView()->addSubview(label);
        
        
        CAFont Font1;
        Font1.fontSize = 50;
        Font1.color = CAColor4B::BLUE;
        Font1.bold = true;
        
        CALabel* label1 = CALabel::create();
        label1->setText(cc);
        label1->setFont(Font1);
        label1->setTextAlignment(CATextAlignment::Center);
        label1->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(60, 2/8.f)));
        this->getView()->addSubview(label1);
        
        
        CALabel* label2 = CALabel::create();
        CAFont Font2;
        Font2.fontSize = 50;
        Font2.color = CAColor4B::BLUE;
        Font2.underLine = true;
        
        label2->setText(cc);
        label2->setFont(Font2);
        label2->setTextAlignment(CATextAlignment::Center);
        label2->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(60, 3/8.f)));
        this->getView()->addSubview(label2);
        
        
        CALabel* label3 = CALabel::create();
        CAFont Font3;
        Font3.fontSize = 50;
        Font3.color = CAColor4B::BLUE;
        Font3.deleteLine = true;
        
        label3->setText(cc);
        label3->setFont(Font3);
        label3->setTextAlignment(CATextAlignment::Center);
        label3->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(60, 4/8.f)));
        this->getView()->addSubview(label3);
        
        
        CALabel* label4 = CALabel::create();
        CAFont Font4;
        Font4.fontSize = 50;
        Font4.color = CAColor4B::BLUE;
        Font4.italics = true;
        
        label4->setText(cc);
        label4->setFont(Font4);
        label4->setTextAlignment(CATextAlignment::Center);
        label4->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(60, 5/8.f)));
        this->getView()->addSubview(label4);
        
        CALabel* label5 = CALabel::create();
        CAFont Font5;
        Font5.fontSize = 50;
        Font5.color = CAColor4B::BLUE;
        Font5.shadow.shadowEnabled = true;
        Font5.shadow.shadowOffset = DSize(2, -2);
        Font5.shadow.shadowBlur = 2.0f;
        Font5.shadow.shadowColor = CAColor4B(0, 0, 0, 88);
        
        label5->setText(cc);
        label5->setFont(Font5);
        label5->setTextAlignment(CATextAlignment::Center);
        label5->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(60, 6/8.f)));
        label5->setNumberOfLine(3) ;
        this->getView()->addSubview(label5);
        
        CALabel* label6 = CALabel::create();
        CAFont Font6;
        Font6.fontSize = 50;
        Font6.color = CAColor4B::CLEAR;
        Font6.stroke.strokeEnabled = true;
        Font6.stroke.strokeSize = 3;
        Font6.stroke.strokeColor = CAColor4B::BLUE;
        
        label6->setText(cc);
        label6->setFont(Font6);
        label6->setTextAlignment(CATextAlignment::Center);
        label6->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(60, 7/8.f)));
        this->getView()->addSubview(label6);

    }
    else if (m_iIndex == 1)
    {
        std::string text = "        When a lion was asleep, a little mouse began running up and down beside him. This soon wakened the lion. He was very angry, and caught the mouse in his paws.\
        \n\
        \"Forgive me, please.\" cried the little mouse.\"I may be able to help you someday.\" The lion was tickled at these words.\
        \n\
        He thought, \"How could this little mouse help me?\" However he lifted up his paws and let him go.\
        \n\
        A few days later, the lion was caught in a trap.\
        　\n\
        The hunters wanted to take him alive to the king, so they tied him to a tree, and went away to look for a wagon.\
        \n\
        Just then the little mouse passed by, and saw the sad lion.\
        　\n\
        He went up to him, and soon gnawed away the ropes. \"Was I not right?\" asked the little mouse.";
        
        
        CAFont Font5;
        Font5.fontSize = 36;
        Font5.color = CAColor4B::BLUE;
        Font5.lineSpacing = 10;
        //Font5.italics = true;
        //Font5.underLine = true;
        
        CALabel* label5 = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 50), DVerticalLayout_T_B(50, 50)));
        label5->setText(text);
        label5->setFont(Font5);
        label5->setNumberOfLine(14);
        this->getView()->addSubview(label5);
    }
    else if (m_iIndex == 2)
    {
        CARichLabel* RichLabel = CARichLabel::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 50), DVerticalLayout_T_B(200, 200)));
        CAFont RichLabelFont;
        RichLabelFont.bold = true;
        RichLabelFont.fontSize = 36;
        RichLabelFont.color = CAColor4B::RED;
        RichLabelFont.lineSpacing = 30;
        RichLabel->appendText("        When a lion was asleep, a little mouse began running up and down beside him. This soon wakened the lion. He was very angry, and caught the mouse in his paws.", RichLabelFont);
        
        CAFont RichLabelFont1;
        RichLabelFont1.italics = true;
        RichLabelFont1.underLine = true;
        RichLabelFont1.fontSize = 28;
        RichLabelFont1.color = CAColor4B::YELLOW;
        RichLabel->appendText("https://www.baidu.com", RichLabelFont1);
        
        CAFont RichLabelFont2;
        RichLabelFont2.underLine = true;
        RichLabelFont2.fontSize = 28;
        RichLabelFont2.color = CAColor4B::RED;
        RichLabel->appendText("libpng warning: iCCP: known incorrect sRGB profile", RichLabelFont2);
        
        CAFont RichLabelFont3;
        RichLabelFont3.underLine = true;
        RichLabelFont3.fontSize = 32;
        RichLabelFont3.color = CAColor4B::RED;
        RichLabel->appendText("        When a lion was asleep, a little mouse began running up and down beside him. This soon wakened the lion. He was very angry, and caught the mouse in his paws.", RichLabelFont3);
        
        this->getView()->addSubview(RichLabel);


    }
    
    
    auto btn = CAButton::createWithLayout(DLayout(DHorizontalLayout_NW_C(0.3,0.5), DVerticalLayout_B_H(20 ,50)), CAButton::Type::RoundedRect);
    btn->setTitleForState(CAControl::State::Normal, "Switch Next");
    btn->addTarget([&] {
        
        ssize_t index = m_iIndex + 1;
        if (index > 2)
        {
            index = 0;
        }
        LabelTest* viewController = LabelTest::create();
        viewController->setIndex(index);
        RootWindow::getInstance()->getRootNavigationController()->replaceViewController(viewController, false);
        
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(btn);
}

void LabelTest::viewDidUnload()
{
}

