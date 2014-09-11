#include "ListViewTest.h"

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
	listView->setListViewOrientation(CAListViewOrientationHorizontal);
	listView->setAllowsSelection(true);
	listView->setAllowsMultipleSelection(true);
	listView->setListHeaderHeight(size.height*0.15);
	listView->setListHeaderView(CAView::createWithColor(CAColor_green));
	listView->setListFooterHeight(size.height*0.15);
	listView->setListFooterView(CAView::createWithColor(CAColor_green));
	this->getView()->addSubview(listView);
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
	return size.height*0.3;
}

CAListViewCell* ListViewTest::listViewCellAtIndex(CAListView *listView, const CCSize& cellSize, unsigned int index)
{
	CADipSize _size = cellSize;
	CAListViewCell* cell = listView->dequeueReusableCellWithIdentifier("ListViewCell");
	if (!cell)
	{
		cell = CAListViewCell::create("ListViewCell");
		CALabel* test = CALabel::createWithCenter(CADipRect(_size.width*0.5,
			_size.height*0.5,
			_size.width*0.8,
			_size.height));
		test->setColor(CAColor_blueStyle);
		test->setTextAlignment(CATextAlignmentCenter);
		test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		test->setFontSize(_px(40));
		test->setTag(100);
		cell->addSubview(test);
	}
	char idx[10] = "";
	sprintf(idx,"list%d",index);
	CALabel* test = (CALabel*)cell->getSubviewByTag(100);
	test->setText(idx);

	return cell;
}
