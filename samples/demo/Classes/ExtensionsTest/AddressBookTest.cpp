#include "AddressBookTest.h"
#include "HttpRequestTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)
#define MAXZORDER 100
#define LABEL_PROPERTY(label,num)									\
	label->setTag(num);												\
	label->setColor(CAColor_blueStyle);								\
	label->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);				\
	label->setTextAlignment(CATextAlignmentCenter);					\
	label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);

AddressBookTest::AddressBookTest():cout(0)
{

}

AddressBookTest::~AddressBookTest()
{

}

void AddressBookTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	this->getNavigationBarItem()->setShowGoBackButton(false);
#if(CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	loadingView();
	getAddressBookList();
#endif
	table = CATableView::createWithCenter(CCRect(size.width*0.5, size.height*0.5, size.width, size.height));
	table->setTableViewDataSource(this);
	table->setTableViewDelegate(this);
	table->setAllowsSelection(true);
	table->setSeparatorViewHeight(1);
	this->getView()->addSubview(table);

	CABarButtonItem* leftButton = CABarButtonItem::create("", CAImage::create("source_material/btn_left_white.png"),CAImage::create(""));
	leftButton->setTarget(this, CAControl_selector(AddressBookTest::previousViewController));
	this->getNavigationBarItem()->addLeftButtonItem(leftButton);

	CABarButtonItem* rightButton = CABarButtonItem::create("", CAImage::create("source_material/btn_right_white.png"), CAImage::create(""));
	rightButton->setTarget(this, CAControl_selector(AddressBookTest::nextViewController));
	this->getNavigationBarItem()->addRightButtonItem(rightButton);
}

void AddressBookTest::loadingView()
{
	CAView* loading = CAView::createWithColor(ccc4(255, 255, 255, 0));
	loading->setTag(MAXZORDER);
	loading->setFrame(this->getView()->getBounds());
	this->getView()->addSubview(loading);

	loadIamge = CAImageView::createWithImage(CAImage::create("loading.png"));
	loadIamge->setCenterOrigin(CCPoint(size.width*0.5, size.height*0.5));
	loadIamge->setScale(0.5);
	loading->addSubview(loadIamge);
	CAScheduler::schedule(schedule_selector(AddressBookTest::addressBookLoadProgress), this, 0.01, false);
}

void AddressBookTest::previousViewController(CAControl* btn, CCPoint point)
{
	this->getNavigationController()->getNavigationBar()->getSubviewByTag(1000)->setVisible(true);
	this->getNavigationController()->popViewControllerAnimated(true);
}

void AddressBookTest::nextViewController(CAControl* btn, CCPoint point)
{
	HttpRequestTest* httpRequestView = new HttpRequestTest();
	httpRequestView->init();
	httpRequestView->setNavigationBarItem(CANavigationBarItem::create("HttpRequestTest"));
	httpRequestView->autorelease();
	this->getNavigationController()->pushViewController(httpRequestView,true);
}

void AddressBookTest::getAddressBookList()
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	addressBookList=CAAddressManager::shareAddressManager()->getAddressBook();
	if(addressBookList.size()>0)
	{
		//CCLog("Size is %d",addressBookList.size());
		CAScheduler::unschedule(schedule_selector(AddressBookTest::addressBookLoadProgress), this);
		this->getView()->removeSubviewByTag(MAXZORDER);
	}
#endif
}

void AddressBookTest::addressBookLoadProgress(float interval)
{
	cout++;
	loadIamge->setRotation(cout*3);
}

void AddressBookTest::viewDidUnload()
{

}

void AddressBookTest::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}

void AddressBookTest::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}

CATableViewCell* AddressBookTest::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
	CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CrossApp");
	if (cell == NULL)
	{
		cell = CATableViewCell::create("CrossApp");
#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
		if (row == 3)
		{
			CALabel* notice = CALabel::createWithCenter(CCRect(cellSize.width*0.5,cellSize.height*0.5,cellSize.width,cellSize.height));
			notice->setText("You need a mobile device to access address book");
			LABEL_PROPERTY(notice,NOTICE);
			cell->addSubview(notice);
		}
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
		CALabel* fullName = CALabel::createWithFrame(CCRect(cellSize.width*0.01, 0, cellSize.width*0.3, cellSize.height));
		LABEL_PROPERTY(fullName, FULLNAME);
		cell->addSubview(fullName);

		CALabel* phoneNumber = CALabel::createWithFrame(CCRect(cellSize.width*0.3, 0, cellSize.width*0.3, cellSize.height));
		LABEL_PROPERTY(phoneNumber, NUMBER);
		cell->addSubview(phoneNumber);

		CALabel* province = CALabel::createWithCenter(CCRect(cellSize.width*0.7, cellSize.height*0.5, cellSize.width*0.2, cellSize.height));
		LABEL_PROPERTY(province, PROVINCE);
		cell->addSubview(province);

		CALabel* nickName = CALabel::createWithCenter(CCRect(cellSize.width*0.9, cellSize.height*0.5, cellSize.width*0.2, cellSize.height));
		LABEL_PROPERTY(nickName, NICKNAME);
		cell->addSubview(nickName);
#endif
	}
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	CALabel* fullName = (CALabel*)cell->getSubviewByTag(FULLNAME);
	fullName->setText(addressBookList[row]->fullname.c_str());

	CALabel* phoneNumber = (CALabel*)cell->getSubviewByTag(NUMBER);
	phoneNumber->setText(addressBookList[row]->phoneNumber.c_str());

	CALabel* province = (CALabel*)cell->getSubviewByTag(PROVINCE);
	province->setText(addressBookList[row]->province.c_str());

	CALabel* nickName = (CALabel*)cell->getSubviewByTag(NICKNAME);
	nickName->setText(addressBookList[row]->nickname.c_str());
#endif

	return cell;

}

CAView* AddressBookTest::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
	CAView* view = CAView::createWithColor(CAColor_gray);

	return view;
}

CAView* AddressBookTest::tableViewSectionViewForFooterInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
	CAView* view = CAView::createWithColor(CAColor_gray);

	return view;
}

unsigned int AddressBookTest::numberOfRowsInSection(CATableView *table, unsigned int section)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	return 8;
#endif
	return addressBookList.size();
}

unsigned int AddressBookTest::numberOfSections(CATableView *table)
{
	return 1;
}

unsigned int AddressBookTest::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	return this->getView()->getBounds().size.height / 8;
#endif
	return this->getView()->getBounds().size.height*0.1;
	
}

unsigned int AddressBookTest::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
	return 1;
}

unsigned int AddressBookTest::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
	return 1;
}