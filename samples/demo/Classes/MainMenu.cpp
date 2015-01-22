#include "MainMenu.h"
#include "FirstViewController.h"
#include "SecondViewController.h"
#include "ThirdViewController.h"
#include "FourthViewController.h"
#include "FifthViewController.h"

MainMenu::MainMenu()
{

}

MainMenu::~MainMenu()
{

}

CAWindow* MainMenu::createWindow()
{
	CAWindow* p_Window = CAWindow::create();

	FirstViewController* viewController1 = new FirstViewController();
	viewController1->init();

	SecondViewController* viewController2 = new SecondViewController();
	viewController2->init();
	viewController2->setTabBarItem(CATabBarItem::create("Second", CAImage::create("tabbar_function/second_1.png"), CAImage::create("tabbar_function/second_2.png")));
	//viewController2->setNavigationBarItem(CANavigationBarItem::create("ViewController2"));
	//CANavigationController* navigationController2 = new CANavigationController();
	//navigationController2->initWithRootViewController(viewController2);

	ThirdViewController* viewController3 = new ThirdViewController();
	viewController3->init();
	viewController3->setTabBarItem(CATabBarItem::create("Third", CAImage::create("tabbar_function/third_1.png"), CAImage::create("tabbar_function/third_2.png")));
	//viewController3->setNavigationBarItem(CANavigationBarItem::create("ViewController3"));
	//CANavigationController* navigationController3 = new CANavigationController();
	//navigationController3->initWithRootViewController(viewController3);

	FourthViewController* viewController4 = new FourthViewController();
	viewController4->init();
	viewController4->setTabBarItem(CATabBarItem::create("Fourth", CAImage::create("tabbar_function/fourth_1.png"), CAImage::create("tabbar_function/fourth_2.png")));
	//viewController4->setNavigationBarItem(CANavigationBarItem::create("ViewController4"));
	//CANavigationController* navigationController4 = new CANavigationController();
	//navigationController4->initWithRootViewController(viewController4);

	std::vector<CAViewController*> controllerItem;
	controllerItem.push_back(viewController2);
	controllerItem.push_back(viewController3);
	controllerItem.push_back(viewController4);

	MainMenu* p_Funtion = new MainMenu();
	p_Funtion->initWithViewControllers(controllerItem);
	p_Funtion->showTabBarSelectedIndicator();
	p_Funtion->setScrollEnabled(true);

	CANavigationController* navigationController = new CANavigationController();
	navigationController->initWithRootViewController(p_Funtion);
	navigationController->getView()->setColor(CAColor_clear);

	float winWidth = p_Window->getBounds().size.width;
	CADrawerController* drawerController = new CADrawerController();
	drawerController->initWithController(viewController1, navigationController, winWidth*0.7);
	drawerController->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("background/1.jpg")));
	drawerController->autorelease();

	p_Window->setRootViewController(drawerController);
	viewController1->release();
	viewController2->release();
	viewController3->release();
	viewController4->release();
	//navigationController2->release();
	//navigationController3->release();
	//navigationController4->release();
	p_Funtion->release();

	return p_Window;
}

void MainMenu::viewDidLoad()
{
	CATabBarController::viewDidLoad();
    this->setScrollEnabled(false);
}

void MainMenu::viewDidUnload()
{
	CATabBarController::viewDidUnload();
}
