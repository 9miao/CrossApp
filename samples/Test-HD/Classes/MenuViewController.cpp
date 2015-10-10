//
//  MenuViewController.cpp
//  Test
//
//  Created by renhongguang on 15/4/3.
//
//

#include "MenuViewController.h"
#include "CDWebViewController.h"
#include "CDUIShowCollectionView.h"
#include "CDTempViewController.h"
#include "CDNewsMenuView.h"
#include "CDNewsViewController.h"


MenuViewController::MenuViewController()
:isinitComplete(false)
{
}

MenuViewController::~MenuViewController()
{
    
}

void MenuViewController::viewDidLoad()
{
    //this->getView()->removeAllSubviews();
    this->getView()->setColor(CAColor_clear);
    size = this->getView()->getBounds().size;
    CCLog("size.width===%f,%f",size.width,size.height);
    
    CAImageView* icon = CAImageView::createWithCenter(DRect(size.width/2,200,size.width,size.width));
    icon->setImage(CAImage::create("image/logo.png"));
    icon->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    this->getView()->addSubview(icon);
    
    for(int i=0 ;i<4; i++)
    {
        char imagename[50];
        sprintf(imagename, "image/navbtn_up_%d.png",i+1);
        char imagename1[50];
        sprintf(imagename1, "image/navbtn_down_%d.png",i+1);
        CAButton* btn = CAButton::create(CAButtonTypeSquareRect);
        btn->setTag(i);
        btn->setCenter(DRect(size.width/2, size.width*i+500, size.width, size.width));
        btn->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create(imagename)));
        btn->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create(imagename1)));
        btn->setBackGroundViewForState(CAControlStateDisabled, CAScale9ImageView::createWithImage(CAImage::create(imagename1)));
        btn->addTarget(this, CAControl_selector(MenuViewController::buttonCallBack), CAControlEventTouchUpInSide);
        this->getView()->addSubview(btn);
        if (i==0)
        {
            btn->setControlState(CAControlStateDisabled);
        }
    }
}

void MenuViewController::viewDidUnload()
{

}

void MenuViewController::buttonCallBack(CAControl* btn,CCPoint point)
{
    CATouchDispatcher* touchDispatcher = CAApplication::getApplication()->getTouchDispatcher();
    CC_RETURN_IF(!touchDispatcher->isDispatchEvents());
    
    touchDispatcher->setDispatchEventsFalse();
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0.5f);
    CAViewAnimation::setAnimationDidStopSelector(touchDispatcher, CAViewAnimation0_selector(CATouchDispatcher::setDispatchEventsTrue));
    CAViewAnimation::commitAnimations();
    
    CAButton* _btn = (CAButton*)btn;
    
    switch (_btn->getTag()) {
        case 0:
            break;
        case 1:
        {
            CAApplication::getApplication()->setStatusBarStyle(CAStatusBarStyleDefault);
            //RootWindow::getInstance()->getSplitNavigationController()->setNavigationBarHidden(true, false);
            CDNewsMenuView* _menuview = new CDNewsMenuView();
            _menuview->init();
            _menuview->autorelease();
            
            CANavigationBarItem* temp_nav0 = CANavigationBarItem::create(" ");
            temp_nav0->setNagigationBarHidden(true);
            _menuview->setNavigationBarItem(temp_nav0);
            
            CDNewsViewController* news_controller1 = new CDNewsViewController(0,"getdemocon");
            news_controller1->autorelease();
            CANavigationBarItem* temp_nav = CANavigationBarItem::create(" ");
            temp_nav->setNagigationBarHidden(true);
            news_controller1->setNavigationBarItem(temp_nav);
            
            CCSize winSize = CAApplication::getApplication()->getWinSize();
            CAVector<CrossApp::CAViewController *> _vec;
            _vec.pushBack(_menuview);
            _vec.pushBack(news_controller1);
            vector<unsigned int> widths;
            widths.push_back(_px(400));
            widths.push_back(winSize.width-_px(400));
            
            CASplitViewController* _splitViewController = new CASplitViewController();
            _splitViewController->initWithController(_vec, widths);
            _splitViewController->setBackgroundView(CAImageView::createWithImage(CAImage::create("image/bg1.jpg")));
            
            CANavigationBarItem* temp_nav1 = CANavigationBarItem::create(" ");
            temp_nav1->setNagigationBarHidden(true);
            _splitViewController->setNavigationBarItem(temp_nav1);
            RootWindow::getInstance()->getSplitNavigationController()->pushViewController(_splitViewController, false);
        }
            break;
        case 2:
        {
            RootWindow::getInstance()->getSplitNavigationController()->setNavigationBarHidden(false, false);
            CDWebViewController* _webController = new CDWebViewController();
            _webController->init();
            _webController->setTitle(" ");
            
            _webController->autorelease();
            RootWindow::getInstance()->getSplitNavigationController()->pushViewController(_webController, false);
            _webController->initWebView("http://www.crossapp.com.cn");
        }
            break;
        case 3:
        {
            RootWindow::getInstance()->getSplitNavigationController()->setNavigationBarHidden(false, false);
            CDWebViewController* _webController = new CDWebViewController();
            _webController->init();
            _webController->setTitle(" ");
            
            _webController->autorelease();
            RootWindow::getInstance()->getSplitNavigationController()->pushViewController(_webController, false);
            _webController->initWebView("http://www.9miao.com/");
        }
            break;
    }
    for(int i=0 ;i<4; i++)
    {
        CAButton* btn1 = (CAButton*)this->getView()->getSubviewByTag(i);
        if (i==0) {
            btn1->setControlState(CAControlStateDisabled);
        }else{
            btn1->setControlState(CAControlStateNormal);
        }
    }
    //_btn->setControlState(CAControlStateDisabled);
}
