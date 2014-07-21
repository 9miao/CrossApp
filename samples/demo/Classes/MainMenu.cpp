#include "MainMenu.h"
#include "FirstViewController.h"
#include "SecondViewController.h"
#include "ThirdViewController.h"
#include "FourthViewController.h"
#include "FifthViewController.h"

MainMenu::MainMenu() :m_TabBarHeight(0.00), m_NavigationBarHeight(0.00)
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
	viewController1->setNavigationBarItem(CANavigationBarItem::create("TestMenu"));

	CANavigationController* navigationController = new CANavigationController();
	navigationController->initWithRootViewController(viewController1);
	navigationController->setTabBarItem(CATabBarItem::create("First", CAImage::create("first_1.png"), CAImage::create("first_2.png")));
	viewController1->release();

	SecondViewController* viewController2 = new SecondViewController();
	viewController2->init();
	viewController2->setTabBarItem(CATabBarItem::create("Second", CAImage::create("second_1.png"), CAImage::create("second_2.png")));

	ThirdViewController* viewController3 = new ThirdViewController();
	viewController3->init();
	viewController3->setTabBarItem(CATabBarItem::create("Third", CAImage::create("third_1.png"), CAImage::create("third_2.png")));

	FourthViewController* viewController4 = new FourthViewController();
	viewController4->init();
	viewController4->setTabBarItem(CATabBarItem::create("Fourth", CAImage::create("fourth_1.png"), CAImage::create("fourth_2.png")));

	std::vector<CAViewController*> controllerItem;
	controllerItem.push_back(navigationController);
	controllerItem.push_back(viewController2);
	controllerItem.push_back(viewController3);
	controllerItem.push_back(viewController4);

	MainMenu* p_Funtion = new MainMenu();
	p_Funtion->initWithViewControllers(controllerItem);
	p_Funtion->getTabBar()->showSelectedIndicator();
	p_Window->setRootViewController(p_Funtion);

	navigationController->release();
	viewController2->release();
	viewController3->release();
	viewController4->release();
	p_Funtion->release();

	return p_Window;
}

void MainMenu::viewDidLoad()
{
	CATabBarController::viewDidLoad();
}

void MainMenu::viewDidUnload()
{
	CATabBarController::viewDidUnload();
}

float MainMenu::getTabBarHeight()
{
	return 0;
}

float MainMenu::getNavigationBarHeight()
{
	return 0;
}
