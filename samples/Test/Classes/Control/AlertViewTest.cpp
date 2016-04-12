
#include "AlertViewTest.h"

AlertViewTest::AlertViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);

}

AlertViewTest::~AlertViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);

}

void AlertViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    //初始化viewList
    CAButton* btn1 = CAButton::create(CAButtonTypeRoundedRect);
    btn1->setLayout(DLayout(240,240,200,60,DLayout::L_R_T_H));
    btn1->setTag(100);
    btn1->setTitleForState(CAControlStateAll, "Click-1");
    btn1->addTarget(this, CAControl_selector(AlertViewTest::alertButtonCallBack), CAControlEventTouchUpInSide);
    this->getView()->addSubview(btn1);
    
    CAButton* btn2 = CAButton::create(CAButtonTypeRoundedRect);
    btn2->setLayout(DLayout(240,240,400,60,DLayout::L_R_T_H));
    btn2->setTag(200);
    btn2->setTitleForState(CAControlStateAll, "Click-2");
    btn2->addTarget(this, CAControl_selector(AlertViewTest::alertButtonCallBack), CAControlEventTouchUpInSide);
    this->getView()->addSubview(btn2);
    
    CAButton* btn3 = CAButton::create(CAButtonTypeRoundedRect);
    btn3->setLayout(DLayout(240,240,600,60,DLayout::L_R_T_H));
    btn3->setTag(300);
    btn3->setTitleForState(CAControlStateAll, "Click-3");
    btn3->addTarget(this, CAControl_selector(AlertViewTest::alertButtonCallBack), CAControlEventTouchUpInSide);
    this->getView()->addSubview(btn3);
    
}

void AlertViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void AlertViewTest::alertButtonCallBack(CAControl* btn,DPoint point)
{
    int tag = btn->getTag();
    switch (tag) {
        case 100:
        {
            CAAlertView* alertView = CAAlertView::createWithText("Alert", "this is a alert!", "close",NULL);
            alertView->show();
            alertView->setTarget(this, CAAlertView_selector(AlertViewTest::alertCallBack));
            break;
        }
        case 200:
        {
            CAAlertView* alertView = CAAlertView::createWithText("Alert", "this is a alert!", "ok","close",NULL);
            alertView->show();
            alertView->setTarget(this, CAAlertView_selector(AlertViewTest::alertCallBack));
            break;
        }
        case 300:
        {
            CAAlertView* alertView = CAAlertView::createWithText("Alert", "message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,message is so long,!", "ok","close","button3","button4","button5","button6","button7",NULL);
            alertView->show();
            alertView->setTarget(this, CAAlertView_selector(AlertViewTest::alertCallBack));
            break;
        }
            
        default:
            break;
    }
}

void AlertViewTest::alertCallBack(int btnIndex)
{
    CCLog("btnIndex==%d",btnIndex);
}



