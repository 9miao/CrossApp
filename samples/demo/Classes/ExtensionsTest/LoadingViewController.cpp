#include "LoadingViewController.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

LoadingViewController::LoadingViewController()
{
	
}

LoadingViewController::~LoadingViewController()
{

}

void LoadingViewController::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	//CAView* loading = CAView::createWithColor(ccc4(51, 204, 255, 50));
	//loading->setTag(MAXZORDER);
	//loading->setFrame(this->getView()->getBounds());
	this->getView()->setColor(ccc4(255, 255, 255, 0));
	loadProgress = CAProgress::create();
	loadProgress->setCenter(CCRect(size.width*0.5, size.height*0.5, size.width*0.6, 50));
	//loading->addSubview(loadProgress);
	//this->getView()->insertSubview(loading, MAXZORDER);
	this->getView()->addSubview(loadProgress);

	CALabel* msg = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.5 - 60, size.width*0.6, 50));
	msg->setText("Loading...please wait");
	msg->setColor(CAColor_blueStyle);
	msg->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	msg->setTextAlignment(CATextAlignmentCenter);
	msg->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	//loading->addSubview(msg);
	this->getView()->addSubview(msg);
	CAScheduler::schedule(schedule_selector(LoadingViewController::addressBookLoadProgress), this, 0.1, false);
}

void LoadingViewController::addressBookLoadProgress(float interval)
{
	CCLog("111111111111111111111111111");
	float currentValue = loadProgress->getProgress() + 0.01;
	loadProgress->setProgress(currentValue);
	//int listSize = 0;
	//if (addressBookList.size() > listSize)
	//{
	//	listSize = addressBookList.size();
	//	CCLog("--------%d--------",listSize);
	//}
	//else if ((addressBookList.size()==listSize)&&(addressBookList.size()!=0))
	//{
	//	CAScheduler::unschedule(schedule_selector(AddressBookTest::addressBookLoadProgress), this);
	//	this->getView()->removeSubviewByTag(MAXZORDER);

	//	table = CATableView::createWithCenter(CCRect(size.width*0.5, size.height*0.5, size.width, size.height));
	//	table->setTableViewDataSource(this);
	//	table->setTableViewDelegate(this);
	//	table->setAllowsSelection(true);
	//	table->setSeparatorViewHeight(1);
	//	this->getView()->addSubview(table);
	//}
}

void LoadingViewController::viewDidUnload()
{

}