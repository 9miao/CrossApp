#include "Demo.h"

#define VIEWCONTROLLER_SWITCH(p_Contrller,index)											\
	p_Contrller* p_ViewContrller = new p_Contrller();										\
	p_ViewContrller->init();																\
	p_ViewContrller->setNavigationBarItem(CANavigationBarItem::create(testList.at(index))); \
	p_ViewContrller->autorelease();\
	CAWindow* window = CAApplication::getApplication()->getRootWindow();\
	CADrawerController* drawer = (CADrawerController*)window->getRootViewController();\
	CANavigationController* nav = (CANavigationController*)drawer->getRightViewController();\
	nav->pushViewController(p_ViewContrller, false);\
	drawer->hideLeftViewController(true);
	
FirstViewController::FirstViewController()
{
	testList.push_back("AlertViewTest");
	testList.push_back("ButtonTest");
	testList.push_back("CollectionViewTest");
	testList.push_back("ImageViewTest");
	testList.push_back("IndicatorViewTest");
	testList.push_back("LabelTest");
	testList.push_back("PageViewTest");
	testList.push_back("ProgressTest");
	testList.push_back("ScrollViewTest");
	testList.push_back("SegmentedControlTest");
	testList.push_back("SliderTest");
	testList.push_back("SwitchTest");
	testList.push_back("TabBarTest");
	testList.push_back("TableViewTest");
	testList.push_back("TextFieldTest");
	testList.push_back("ExtensionsTest");
}

FirstViewController::~FirstViewController()
{

}

void FirstViewController::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	tableView = CATableView::createWithCenter(CADipRect(size.width*0.5, size.height*0.5, size.width, size.height));
	tableView->setAllowsSelection(true);
	tableView->setTableViewDelegate(this);
	tableView->setTableViewDataSource(this);
	this->getView()->addSubview(tableView);
}

void FirstViewController::viewDidUnload()
{

}

void FirstViewController::reshapeViewRectDidFinish()
{

}

void FirstViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	switch (row)
	{
		case AlERTVIEWTEST:
		{
			VIEWCONTROLLER_SWITCH(AlertViewTest, row);
			break;
		}
		case BUTTONTEST:
		{
			VIEWCONTROLLER_SWITCH(ButtonTest, row);
			break;
		}
		case COLLECTIONVIEWTEST:
		{
			VIEWCONTROLLER_SWITCH(CollectionViewTest, row);
			break;
		}
		case IMAGEVIEWTEST:
		{
			VIEWCONTROLLER_SWITCH(ImageViewTest, row);
			break;
		}
		case INDICATORVIEWTEST:
		{
			VIEWCONTROLLER_SWITCH(ActivityIndicatorViewTest, row);
			break;
		}
		case LABELTEST:
		{
			VIEWCONTROLLER_SWITCH(LabelTest, row);
			break;
		}
		case PAGEVIEWTEST:
		{
			VIEWCONTROLLER_SWITCH(PageViewTest,row);
			break;
		}
		case PROGRESSTEST:
		{
			VIEWCONTROLLER_SWITCH(ProgressTest, row);
			break;
		}
		case SCROLLVIEWTEST:
		{
			VIEWCONTROLLER_SWITCH(ScrollViewTest, row);
			break;
		}
		case SEGMENTEDCONTROLTEST:
		{
			VIEWCONTROLLER_SWITCH(SegmentedControlTest, row);
			break;
		}	
		case SLIDERTEST:
		{
			VIEWCONTROLLER_SWITCH(SliderTest, row);
			break;
		}
		case SWITCHTEST:
		{
			VIEWCONTROLLER_SWITCH(SwitchTest, row);
			break;
		}
		case TABBARTEST:
		{
			VIEWCONTROLLER_SWITCH(TabBarTest, row);
			break;
		}
		case TABVIEWTEST:
		{
			VIEWCONTROLLER_SWITCH(TableViewTest, row);
			break;
		}
		case TEXTFIELDTEST:
		{
			VIEWCONTROLLER_SWITCH(TextFieldTest, row);
			break;
		}
		case EXTENSIONSTEST:
		{
			VIEWCONTROLLER_SWITCH(ExtensionsTest, row);
			break;
		}
		default:
			break;
	}
}

void FirstViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}

void FirstViewController::tableViewDidShowPullDownView(CATableView* table)
{

}

void FirstViewController::tableViewDidShowPullUpView(CATableView* table)
{

}

CATableViewCell* FirstViewController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
	CADipSize _size = cellSize;
	CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CrossApp");
	if (cell == NULL)
	{
		cell = CATableViewCell::create("CrossApp");
		CALabel* test = CALabel::createWithCenter(CADipRect(_size.width*0.5,
												_size.height*0.5,
												_size.width*0.8,
												_size.height));
		test->setColor(ccc4(51, 204, 255, 255));
		test->setTextAlignment(CATextAlignmentCenter);
		test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		test->setFontSize(_px(40));
		test->setTag(100);
		cell->addSubview(test);
	}
	CALabel* test = (CALabel*)cell->getSubviewByTag(100);
	test->setText(testList.at(row));
	
	return cell;
}


CAView* FirstViewController::tableViewSectionViewForHeaderInSection(CATableView* table, unsigned int section)
{
	CAView* headerView = CAView::createWithColor(CAColor_gray);

	return headerView;
}

CAView* FirstViewController::tableViewSectionViewForFooterInSection(CATableView* table, unsigned int section)
{
	CAView* footerView = CAView::createWithColor(CAColor_gray);

	return footerView;
}

unsigned int FirstViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
	return testList.size();
}

unsigned int FirstViewController::numberOfSections(CATableView *table)
{
	return 1;
}

unsigned int FirstViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return size.width*0.2;
}

unsigned int FirstViewController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
	return 1;
}

unsigned int FirstViewController::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
	return 1;
}

