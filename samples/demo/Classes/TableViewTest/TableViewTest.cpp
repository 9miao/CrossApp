#include "TableViewTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

TableViewTest::TableViewTest()
{

}

TableViewTest::~TableViewTest()
{

}

void TableViewTest::viewDidLoad()
{
	CCSize size = this->getView()->getBounds().size;

	p_TableView = CATableView::createWithCenter(CCRect(size.width*0.5,size.height*0.5,size.width,size.height));
	p_TableView->setTableViewDataSource(this);
	p_TableView->setTableViewDelegate(this);
	p_TableView->setAllowsSelection(true);
	p_TableView->setAllowsMultipleSelection(true);
	p_TableView->setBackGroundImage(CAImage::create("bg.jpg"));
	this->getView()->addSubview(p_TableView);
}

void TableViewTest::viewDidUnload()
{

}

void TableViewTest::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	char s[32];
	sprintf(s, "The Page No.%ld", this->getNavigationController()->getViewControllerCount());

	CANavigationBarItem* item = CANavigationBarItem::create(s);
	//item->setShowGoBackButton(false);
	TableViewTest* viewController = new TableViewTest();
	viewController->init();
	viewController->setNavigationBarItem(item);

	this->getNavigationController()->replaceViewController(viewController, true);
	viewController->autorelease();
}

void TableViewTest::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	
}

void TableViewTest::tableViewDidShowPullDownView(CATableView* table)
{
	CCLog("Pulldown");
}

void TableViewTest::tableViewDidShowPullUpView(CATableView* table)
{
	
}

CATableViewCell* TableViewTest::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
	CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CrossApp");
	if (cell == NULL)
	{
		cell = CATableViewCell::create("CrossApp");
		//cell->setAlpha(0);
		//cell->setBackgroundView(CAView::createWithFrame(CCRect(0,0,cellSize.width,cellSize.height),CAColor_green));
		CALabel* cellText = CALabel::createWithCenter(CCRect(cellSize.width*0.1, cellSize.height*0.5, cellSize.width*0.3, cellSize.height*0.8));
		cellText->setTag(100);
		cellText->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
		cellText->setColor(CAColor_blueStyle);
		cellText->setTextAlignment(CATextAlignmentCenter);
		cellText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		cell->addSubview(cellText);

		CAButton* cellBtn = CAButton::createWithCenter(CCRect(cellSize.width*0.8,cellSize.height*0.5,cellSize.width*0.2,cellSize.height*0.5),CAButtonTypeRoundedRect);
		cellBtn->setTag(102);
		cellBtn->setTitleForState(CAControlStateAll,"Touch");
		cellBtn->addTarget(this, CAControl_selector(TableViewTest::cellBtnCallback), CAControlEventTouchUpInSide);
		cell->addSubview(cellBtn);
	}
	char order[20] = "";
	sprintf(order, "cell-%d", row);
	CALabel* cellText = (CALabel*)cell->getSubviewByTag(100);
	cellText->setText(order);

	return cell;

}

void TableViewTest::cellBtnCallback(CAControl* btn, CCPoint point)
{
	
}

CAView* TableViewTest::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
	std::string head = "";
	head = (section == 0) ? "selection-0" : "selection-1";
	CAView* view = CAView::createWithColor(CAColor_gray);

	CALabel* header = CALabel::createWithCenter(CCRect(viewSize.width*0.5, viewSize.height*0.5, viewSize.width*0.8, viewSize.height));
	header->setText(head);
	header->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	header->setColor(CAColor_white);
	header->setTextAlignment(CATextAlignmentCenter);
	header->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	view->addSubview(header);


	return view;
}

CAView* TableViewTest::tableViewSectionViewForFooterInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
	CAView* view = CAView::createWithColor(CAColor_gray);


	return view;
}

unsigned int TableViewTest::numberOfRowsInSection(CATableView *table, unsigned int section)
{
	return 7;
}

unsigned int TableViewTest::numberOfSections(CATableView *table)
{
	return 2;
}

unsigned int TableViewTest::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return this->getView()->getBounds().size.height*0.2;
}

unsigned int TableViewTest::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
	return 50 * CROSSAPP_ADPTATION_RATIO;
}

unsigned int TableViewTest::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
	return 1;
}


