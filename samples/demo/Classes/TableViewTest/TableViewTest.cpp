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
	this->getView()->addSubview(p_TableView);
}

void TableViewTest::viewDidUnload()
{

}

void TableViewTest::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	CCLog("%d,%d",section,row);
	//if (row < 6 && section==1)
	//{
	//	p_TableView->setSelectRowAtIndexPath(section,row+1);
	//}
}

void TableViewTest::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	
}

void TableViewTest::tableViewDidShowPullDownView(CATableView* table)
{
	
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
		CALabel* cellText = CALabel::createWithCenter(CCRect(cellSize.width*0.1, cellSize.height*0.5, cellSize.width*0.3, cellSize.height*0.8));
		cellText->setTag(100);
		cellText->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
		cellText->setColor(CAColor_blueStyle);
		cellText->setTextAlignment(CATextAlignmentCenter);
		cellText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		cell->addSubview(cellText);
		if (section == 1 && row == 0)
		{
			CALabel* cellDesc = CALabel::createWithCenter(CCRect(cellSize.width*0.5 + 50 * CROSSAPP_ADPTATION_RATIO, cellSize.height*0.5, cellSize.width*0.8, cellSize.height*0.8));
			cellDesc->setTag(101);
			cellDesc->setText("When you click on a cell, will select the next cell");
			cellDesc->setColor(CAColor_blueStyle);
			cellDesc->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
			cellDesc->setTextAlignment(CATextAlignmentCenter);
			cellDesc->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
			cell->addSubview(cellDesc);
		}
	}
	char order[20] = "";
	sprintf(order, "cell-%d", row);
	CALabel* cellText = (CALabel*)cell->getSubviewByTag(100);
	cellText->setText(order);

	//CALabel* cellDesc = (CALabel*)cell->getSubviewByTag(101);
	//if (section == 1 && row == 0)
	//{
	//	if (cellDesc == NULL)
	//	{
	//		CCLog("nil...");
	//	}
	//	cellDesc->setText("When you click on a cell, will select the next cell");
	//}

	return cell;

}

CAView* TableViewTest::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
	char head[20] = "";
	CAView* view = CAView::createWithColor(CAColor_gray);

	CALabel* header = CALabel::createWithCenter(CCRect(viewSize.width*0.5, viewSize.height*0.5, viewSize.width*0.3, viewSize.height));
	sprintf(head, "Section-%d", section);
	header->setText(head);
	header->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	header->setColor(CAColor_blueStyle);
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


