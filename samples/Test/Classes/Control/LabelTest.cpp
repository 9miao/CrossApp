
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
    
    showNum = 2;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
    CALabel* label1 = CALabel::create();
    label1->setTag(1);
    label1->setLayout(DRectLayout(100, 100, 150, 40, DRectLayout::L_R_T_H));
    
    CALabel* label2 = CALabel::create();
    label2->setTag(2);
    label2->setLayout(DRectLayout(100, 100, 300, 40, DRectLayout::L_R_T_H));
    
    CALabel* label3 = CALabel::create();
    label3->setTag(3);
    label3->setLayout(DRectLayout(100, 100, 450, 40, DRectLayout::L_R_T_H));
    
    CALabel* label4 = CALabel::create();
    label4->setTag(4);
    label4->setLayout(DRectLayout(100, 100, 600, 40, DRectLayout::L_R_T_H));
    
    int tag = label1->getTag();
    for (int i=1; i<5; i++)
    {
        std::string s = "Hello World";
        
        std::u32string c;
        StringUtils::UTF8ToUTF32(s, c);
        c += 0x1F604;
        
        std::string cc;
        StringUtils::UTF32ToUTF8(c, cc);
        
        if (tag==1) {
            label1->setText(cc);
            label1->setColor(CAColor_red);
            label1->setFontName("c:/x.ttf");
            label1->setFontSize(36);
            label1->setTextAlignment(CATextAlignmentCenter);
            label1->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label1->setUnderLine(true);
        }else if(tag==2){
            label2->setText(cc);
            label2->setColor(CAColor_red);
            label2->setFontName("c:/x.ttf");
            label2->setFontSize(36);
            label2->setTextAlignment(CATextAlignmentCenter);
            label2->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label2->setBold(true);
        }else if(tag==3){
            label3->setText(cc);
            label3->setColor(CAColor_red);
            label3->setFontName("c:/x.ttf");
            label3->setFontSize(36);
            label3->setTextAlignment(CATextAlignmentCenter);
            label3->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label3->setItalics(true);
        }else if(tag==4){
            label4->setText(cc);
            label4->setColor(CAColor_red);
            label4->setFontName("c:/x.ttf");
            label4->setFontSize(36);
            label4->setTextAlignment(CATextAlignmentCenter);
            label4->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label4->setText("Enable Copy");
            label4->setEnableCopy(true);
        }
        tag++;
    }
    
    CAView* view = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view->addSubview(label1);
    view->addSubview(label2);
    view->addSubview(label3);
    view->addSubview(label4);
    view->setColor(CAColor_gray);
    VIEWLIST.pushBack(view);
    
    CALabel* label6 = CALabel::create();
    label6->setLayout(DRectLayout(50, 50, 300, 300, DRectLayout::L_R_T_H));
    label6->setColor(CAColor_red);
    label6->setFontName("c:/x.ttf");
    label6->setFontSize(36);
    label6->setTextAlignment(CATextAlignmentCenter);
    label6->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label6->setTag(6);
    label6->setUnderLine(true);
    label6->setBold(true);
    label6->setItalics(true);
    label6->setLineSpacing(20);
    label6->setText("Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World");

    CAView* view6 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view6->addSubview(label6);
    view6->setColor(CAColor_gray);
    VIEWLIST.pushBack(view6);
    
    showUI();
    p_PageViewVec->setViews(VIEWLIST);
}

void LabelTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;  miui 7.X的
}



