
#include "LabelTest.h"
#include "CDUIShowAutoCollectionView.h"

int LabelNum = 0;

LabelTest::LabelTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

LabelTest::~LabelTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
    LabelNum = 0;
}

void CDUIShowAutoCollectionView::LabelRightBtnRightcallback(CAControl* control, DPoint point)
{
    if (showLabelNavigationBar >= 2)
    {
        showLabelNavigationBar = 0;
        
        LabelNum = showLabelNavigationBar;
    }
    else
    {
        LabelNum = ++showLabelNavigationBar;
    }
    
    LabelTest* ViewContrllerLabelTest = new LabelTest();
    ViewContrllerLabelTest->init();
    ViewContrllerLabelTest->setNavigationBarItem(LabelNavigationBar);
    ViewContrllerLabelTest->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->replaceViewController(ViewContrllerLabelTest, false);
}

void LabelTest::viewDidLoad()
{
	std::string s = "Hello World";

	std::u32string c;
	StringUtils::UTF8ToUTF32(s, c);
	c += 0x1F604;

	std::string cc;
	StringUtils::UTF32ToUTF8(c, cc);

	if (LabelNum == 0)
	{
		CAFont Font;
		Font.fontSize = 36;
		Font.color = CAColor_red;
		Font.fontName = "c:/x.ttf";

		CALabel* label = CALabel::create();
		label->setText(cc);
		label->setFont(Font);
		label->setTextAlignment(CATextAlignmentCenter);
		label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		label->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(40, 0.16)));

		CAFont Font1;
		Font1.fontSize = 36;
		Font1.color = CAColor_red;
		Font1.fontName = "c:/x.ttf";
		Font1.underLine = true;

		CALabel* label1 = CALabel::create();
		label1->setText(cc);
		label1->setFont(Font1);
		label1->setTextAlignment(CATextAlignmentCenter);
		label1->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		label1->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(40, 0.32)));

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
		label2->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(40, 0.48)));

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
		label3->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(40, 0.64)));

		CALabel* label4 = CALabel::create();
		CAFont Font4;
		Font4.fontSize = 36;
		Font4.color = CAColor_red;
		label4->setText("Enable Copy");
		label4->setFont(Font4);
		label4->setTextAlignment(CATextAlignmentCenter);
		label4->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		label4->setLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(40, 0.8)));

		CAView* view = CAView::createWithLayout(DLayoutFill);
		view->addSubview(label);
		view->addSubview(label1);
		view->addSubview(label2);
		view->addSubview(label3);
		view->addSubview(label4);
		view->setColor(CAColor_gray);

		this->getView()->addSubview(view);
	}
	else if (LabelNum == 1)
	{
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
		label5->setLayout(DLayout(DHorizontalLayout_L_R(50, 50), DVerticalLayout_T_H(300, 300)));

		CAView* view1 = CAView::createWithLayout(DLayoutFill);
		view1->addSubview(label5);
		view1->setColor(CAColor_gray);

		this->getView()->addSubview(view1);
	}
	else
	{

		CARichLabel* RichLabel = CARichLabel::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_B(200, 200)));
		CAFont RichLabelFont;
		RichLabelFont.bold = true;
		RichLabelFont.fontSize = 42;
		RichLabelFont.color = CAColor_red;
		RichLabel->appendText("Hello World Hello World Hello World Hello World Hello World Hello World ", RichLabelFont);

		CAFont RichLabelFont1;
		RichLabelFont1.italics = true;
		RichLabelFont1.fontSize = 42;
		RichLabelFont1.color = CAColor_red;
		RichLabel->appendText("Hello World Hello World Hello World Hello World Hello World Hello World ", RichLabelFont1);

		CAFont RichLabelFont2;
		RichLabelFont2.underLine = true;
		RichLabelFont2.fontSize = 42;
		RichLabelFont2.color = CAColor_red;
		RichLabel->appendText("Hello World Hello World Hello World Hello World Hello World Hello World ", RichLabelFont2);

		CAView* view2 = CAView::createWithLayout(DLayoutFill);
		view2->addSubview(RichLabel);
		view2->setColor(CAColor_gray);

		this->getView()->addSubview(view2);
	}
}

void LabelTest::viewDidUnload()
{
}
