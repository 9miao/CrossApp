#include "TableViewTest.h"
#include "MyTableViewCell.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

TableViewTest::TableViewTest()
{

}

TableViewTest::~TableViewTest()
{

}

void TableViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	p_TableView = CATableView::createWithCenter(CADipRect(size.width*0.5, size.height*0.5, size.width, size.height));
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

}

void TableViewTest::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	
}

CATableViewCell* TableViewTest::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
	CADipSize _size = cellSize;
	MyTableViewCell* cell = dynamic_cast<MyTableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
	if (cell == NULL)
	{
		cell = MyTableViewCell::create("CrossApp");
		cell->setFrame(CADipRect(0, 0, _size.width, _size.height));
		cell->initCell();
	}
	if (section == 1)
	{
		CAButton* cellBtn = (CAButton*)cell->getSubviewByTag(102);
		cellBtn->setVisible(false);
	}
	else
	{
		CAButton* cellBtn = (CAButton*)cell->getSubviewByTag(102);
		cellBtn->setVisible(true);
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
	head = (section == 0) ? "Selection-0" : "Selection-1";
	CAView* view = CAView::createWithColor(CAColor_gray);

	CADipSize _size = viewSize;
	CALabel* header = CALabel::createWithCenter(CADipRect(_size.width*0.5, _size.height*0.5, _size.width*0.8, _size.height));
	header->setText(head);
	header->setFontSize(_px(30));
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
	return 10;
}

unsigned int TableViewTest::numberOfSections(CATableView *table)
{
	return 2;
}

unsigned int TableViewTest::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return size.height*0.2;
}

unsigned int TableViewTest::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
	return _px(50);
}

unsigned int TableViewTest::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
	return 1;
}


