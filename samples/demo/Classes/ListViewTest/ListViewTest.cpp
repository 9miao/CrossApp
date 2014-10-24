#include "ListViewTest.h"
#include "MyListViewCell.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

ListViewTest::ListViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

ListViewTest::~ListViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

void ListViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	CAListView *listView = CAListView::createWithCenter(CADipRect(size.width*0.5,size.height*0.5,size.width,size.height));
	listView->setListViewDelegate(this);
	listView->setListViewDataSource(this);
	listView->setAllowsSelection(true);
	listView->setAllowsMultipleSelection(true);
	listView->setListViewOrientation(CAListViewOrientationHorizontal);
	listView->setSeparatorColor(CAColor_clear);
	listView->setListHeaderHeight(_px(100));
	listView->setListFooterHeight(_px(100));
	listView->setListHeaderView(CAView::createWithColor(CAColor_gray));
	listView->setListFooterView(CAView::createWithColor(CAColor_gray));
	this->getView()->addSubview(listView);

	CALabel* header = CALabel::createWithCenter(CADipRect(50, size.height*0.5, 100, size.height));
	header->setText("Header");
	header->setFontSize(35);
	header->setTextAlignment(CATextAlignmentCenter);
	header->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	listView->getListHeaderView()->addSubview(header);

	CALabel* footer = CALabel::createWithCenter(CADipRect(50, size.height*0.5, 100, size.height));
	footer->setText("Footer");
	footer->setFontSize(35);
	footer->setTextAlignment(CATextAlignmentCenter);
	footer->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	CAView* listFootView = CAView::createWithColor(CAColor_green);
	listView->getListFooterView()->addSubview(footer);
}

void ListViewTest::viewDidUnload()
{

}

void ListViewTest::listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index)
{
	
}

void ListViewTest::listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index)
{

}

unsigned int ListViewTest::numberOfIndex(CAListView *listView)
{
	return 10;
}

unsigned int ListViewTest::listViewHeightForIndex(CAListView *listView, unsigned int index)
{
	return _px(200);
}

CAListViewCell* ListViewTest::listViewCellAtIndex(CAListView *listView, const CCSize& cellSize, unsigned int index)
{
	CADipSize _size = cellSize;
	MyListViewCell* cell = (MyListViewCell*)listView->dequeueReusableCellWithIdentifier("ListViewCell");
	if (!cell)
	{
		cell = MyListViewCell::create("ListViewCell", CADipRect(0, 0, _size.width, _size.height));
		cell->initWithCell();
	}
	char idx[10] = "";
	sprintf(idx,"list%d",index);
	CALabel* test = (CALabel*)cell->getSubviewByTag(100);
	test->setText(idx);

	return cell;
}
