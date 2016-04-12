
#include "LabelTest.h"

LabelTest::LabelTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

LabelTest::~LabelTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void LabelTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    showNum = 3;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
    std::string s = "Hello World";
    
    std::u32string c;
    StringUtils::UTF8ToUTF32(s, c);
    c += 0x1F604;
    
    std::string cc;
    StringUtils::UTF32ToUTF8(c, cc);
    
    CALabel* label = CALabel::create();
    CAFont Font;
    Font.fontSize = 36;
    Font.color = CAColor_red;
    Font.fontName = "c:/x.ttf";
    label->setText(cc);
    label->setFont(Font);
    label->setTextAlignment(CATextAlignmentCenter);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setLayout(DLayout(100, 100, 150, 40, DLayout::L_R_T_H));
    
    CALabel* label1 = CALabel::create();
    CAFont Font1;
    Font1.fontSize = 36;
    Font1.color = CAColor_red;
    Font1.fontName = "c:/x.ttf";
    Font1.underLine = true;
    label1->setText(cc);
    label1->setFont(Font1);
    label1->setTextAlignment(CATextAlignmentCenter);
    label1->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label1->setLayout(DLayout(100, 100, 300, 40, DLayout::L_R_T_H));
    
    CALabel* label2 = CALabel::create();
    CAFont Font2;
    Font2.fontSize = 36;
    Font2.color = CAColor_red;
    Font2.fontName = "c:/x.ttf";
    Font2.bold = true;
    label2->setText(cc);
    label2->setFont(Font2);
    label2->setTextAlignment(CATextAlignmentCenter);
    label2->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label2->setLayout(DLayout(100, 100, 450, 40, DLayout::L_R_T_H));
    
    CALabel* label3 = CALabel::create();
    CAFont Font3;
    Font3.fontSize = 36;
    Font3.color = CAColor_red;
    Font3.fontName = "c:/x.ttf";
    Font3.italics = true;
    label3->setText(cc);
    label3->setFont(Font3);
    label3->setTextAlignment(CATextAlignmentCenter);
    label3->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label3->setLayout(DLayout(100, 100, 600, 40, DLayout::L_R_T_H));
    
    CALabel* label4 = CALabel::create();
    CAFont Font4;
    Font4.fontSize = 36;
    Font4.color = CAColor_red;
    label4->setText("Enable Copy");
    label4->setFont(Font4);
    label4->setTextAlignment(CATextAlignmentCenter);
    label4->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label4->setLayout(DLayout(100, 100, 750, 40, DLayout::L_R_T_H));

    CAView* view = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view->addSubview(label);
    view->addSubview(label1);
    view->addSubview(label2);
    view->addSubview(label3);
    view->addSubview(label4);
    view->setColor(CAColor_gray);
    VIEWLIST.pushBack(view);
    
    CALabel* label5 = CALabel::create();
    CAFont Font5;
    Font5.fontSize = 36;
    Font5.color = CAColor_red;
    Font5.fontName = "c:/x.ttf";
    Font5.bold = true;
    Font5.italics = true;
    Font5.underLine = true;
    label5->setText("Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World");
    label5->setFont(Font5);
    label5->setLineSpacing(20);
    label5->setTextAlignment(CATextAlignmentCenter);
    label5->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label5->setLayout(DLayout(50, 50, 300, 300, DLayout::L_R_T_H));
    
	CAView* view1 = CAView::createWithLayout(DLayout(50, 50, 300, 300, DLayout::L_R_T_H));
    view1->addSubview(label5);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
	CARichLabel* RichLabel = CARichLabel::create();
	RichLabel->setLayout(DLayout(50, 50, 50, 500, DLayout::L_R_T_H));
	RichLabel->appendText("Hello World");

	CAFont ft;
	ft.fontSize = 48;
	ft.color = ccc4(255, 0, 255, 100);
	RichLabel->appendText(UTF8("阿斯蒂芬"), ft);

	RichLabel->appendText(UTF8("欢迎访问我的网站 http://www.baidu.com、以及我的微博：http://www.xxx.com"));
	ft.fontSize = 64;
	ft.color = ccc4(255, 0, 0, 100);
	RichLabel->appendText(UTF8("阿斯顿的双方各大发光火发过火各环节饭锅夫人"), ft);

	CAFont RichLabelFont2;
	RichLabelFont2.underLine = true;
	RichLabelFont2.fontSize = 42;
	RichLabelFont2.color = CAColor_yellow;
	RichLabel->appendText("Hello World Hello World Hello World Hello World Hello World Hello World ", RichLabelFont2);

    CAView* view2 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view2->addSubview(RichLabel);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    showUI();
    
    p_PageViewVec->setViews(VIEWLIST);
}

void LabelTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;  miui 7.X的
}



