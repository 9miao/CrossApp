#include "NavigationBarTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

NavigationBarTest::NavigationBarTest()
{

}

NavigationBarTest::~NavigationBarTest()
{

}

void NavigationBarTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	p_NavigationBar = CANavigationBar::create();
	p_NavigationBar->setFrame(CCRect(0,0,size.width,size.height*0.15));
	p_NavigationBar->setBackGroundImage(CAImage::create("source_material/btn_rounded3D_selected.png"));
	p_NavigationBar->pushItem(CANavigationBarItem::create("Number 1"));
	this->getView()->addSubview(p_NavigationBar);

	CAButton *next = CAButton::createWithCenter(CCRect(p_NavigationBar->getBounds().size.width*0.9, p_NavigationBar->getBounds().size.height*0.5, 100, 80), CAButtonTypeCustom);
	next->setImageForState(CAControlStateNormal, CAImage::create("source_material/btn_right_white.png"));
	next->setImageColorForState(CAControlStateHighlighted, ccc4(0, 255, 200, 255));
	next->addTarget(this, CAControl_selector(NavigationBarTest::nextLayer), CAControlEventTouchUpInSide);
	p_NavigationBar->addSubview(next);

	CAButton *menu = CAButton::createWithCenter(CCRect(size.width*0.5, size.height*0.5, 200, 80), CAButtonTypeRoundedRect);
	menu->setTitleForState(CAControlStateAll, "MainMenu");
	menu->addTarget(this, CAControl_selector(NavigationBarTest::returnMainMenu), CAControlEventTouchUpInSide);
	this->getView()->addSubview(menu);
}

void NavigationBarTest::nextLayer(CAControl* btn, CCPoint point)
{
	char num[20] = "";
	std::vector<CANavigationBarItem*> items;
	items = p_NavigationBar->getItems();
	sprintf(num, "Number %d", items.size() + 1);
	p_NavigationBar->pushItem(CANavigationBarItem::create(num));
}

void NavigationBarTest::returnMainMenu(CAControl* btn, CCPoint point)
{
	p_NavigationBar->removeFromSuperview();
	this->getNavigationController()->popViewControllerAnimated(true);
	this->getNavigationController()->setNavigationBarHidden(false, true);
}

void NavigationBarTest::viewDidUnload()
{
	
}