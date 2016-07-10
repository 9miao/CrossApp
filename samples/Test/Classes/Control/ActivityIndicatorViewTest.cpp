
#include "ActivityIndicatorViewTest.h"
#include "CDUIShowAutoCollectionView.h"

int ActivityIndicatorNum = 0;

ActivityIndicatorViewTest::ActivityIndicatorViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);   
}

ActivityIndicatorViewTest::~ActivityIndicatorViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
    ActivityIndicatorNum = 0;
}

void CDUIShowAutoCollectionView::ActivityIndicatorRightBtnRightcallback(CAControl* control, DPoint point)
{
    if (showActivityIndicatorNavigationBar >= 4)
    {
        showActivityIndicatorNavigationBar = 0;
        ActivityIndicatorNum = showActivityIndicatorNavigationBar;
    }
    else
    {
        ActivityIndicatorNum = ++showActivityIndicatorNavigationBar;
    }
    
    ActivityIndicatorViewTest* ViewContrllerIndicatorViewTest = new ActivityIndicatorViewTest();
    ViewContrllerIndicatorViewTest->init();
    ViewContrllerIndicatorViewTest->setNavigationBarItem(ActivityIndicatorNavigationBar);
    ViewContrllerIndicatorViewTest->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->replaceViewController(ViewContrllerIndicatorViewTest, false);
}

void ActivityIndicatorViewTest::viewDidLoad()
{
    CCLog("ActivityIndicatorNum == %d", ActivityIndicatorNum);
    if (ActivityIndicatorNum == 0)
    {
        CAActivityIndicatorView* idc1 = CAActivityIndicatorView::createWithLayout(DLayoutFill);
        idc1->setStyle(CAActivityIndicatorViewStyleWhiteLarge);
        idc1->startAnimating();
        
        CAView* view1 = CAView::createWithLayout(DLayoutFill);
        view1->addSubview(idc1);
        view1->setColor(CAColor_gray);
        this->getView()->addSubview(view1);
    }
    else if (ActivityIndicatorNum == 1)
    {
        CAActivityIndicatorView* idc2 = CAActivityIndicatorView::createWithLayout(DLayoutFill);
        idc2->setStyle(CAActivityIndicatorViewStyleGrayLarge);
        idc2->startAnimating();
    
        CAView* view2 = CAView::createWithLayout(DLayoutFill);
        view2->addSubview(idc2);
        view2->setColor(CAColor_gray);
        this->getView()->addSubview(view2);
    }
    else if (ActivityIndicatorNum == 2)
    {
        CAActivityIndicatorView* idc3 = CAActivityIndicatorView::createWithLayout(DLayoutFill);
        idc3->setStyle(CAActivityIndicatorViewStyleWhite);
        idc3->setCycleTime(1.0f);
        idc3->setTimesOneCycle(12);
        idc3->startAnimating();
    
        CAView* view3 = CAView::createWithLayout(DLayoutFill);
        view3->addSubview(idc3);
        view3->setColor(CAColor_gray);
        this->getView()->addSubview(view3);
    }
    else if (ActivityIndicatorNum == 3)
    {
        CAActivityIndicatorView* idc4 = CAActivityIndicatorView::createWithLayout(DLayoutFill);
        idc4->setStyle(CAActivityIndicatorViewStyleGray);
        idc4->setCycleTime(1.0f);
        idc4->setTimesOneCycle(12);
        idc4->startAnimating();
    
        CAView* view4 = CAView::createWithLayout(DLayoutFill);
        view4->addSubview(idc4);
        view4->setColor(CAColor_gray);
        this->getView()->addSubview(view4);
    }
    else
    {
        CAImage* image = CAImage::create("image/indicator2.png");
        CAImageView* imageView = CAImageView::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_B(100,100)));
        imageView->setFrame(DRect(0, 0, 100, 100));
        imageView->setImage(image);

        CAActivityIndicatorView* idc5 = CAActivityIndicatorView::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_T_B(100,100)));
        idc5->setStyle(CAActivityIndicatorViewStyleImage);
        idc5->setActivityIndicatorView(imageView);
        idc5->startAnimating();
    
        CAView* view5 = CAView::createWithLayout(DLayoutFill);
        view5->addSubview(idc5);
        view5->setColor(CAColor_gray);
        this->getView()->addSubview(view5);
    }

}

void ActivityIndicatorViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

