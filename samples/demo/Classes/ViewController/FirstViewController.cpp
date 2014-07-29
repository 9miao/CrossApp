#include "FirstViewController.h"
#include "AlertViewTest.h"
#include "ButtonTest.h"
#include "CollectionViewTest.h"
#include "ImageViewTest.h"
#include "LabelTest.h"
#include "NavigationBarTest.h"
#include "ProgressTest.h"
#include "ScrollViewTest.h"
#include "SegmentedControlTest.h"
#include "SliderTest.h"
#include "SwitchTest.h"
#include "TabBarTest.h"
#include "TableViewTest.h"
#include "TextFieldTest.h"
#include "ExtensionsTest.h"

#define VIEWCONTROLLER_JUMP(p_Contrller,title)                                  \
	p_Contrller* p_ViewContrller = new p_Contrller();                           \
	p_ViewContrller->init();                                                    \
	p_ViewContrller->setNavigationBarItem(CANavigationBarItem::create(title));  \
	p_ViewContrller->autorelease();                                             \
	this->getNavigationController()->pushViewController(p_ViewContrller, true);

FirstViewController::FirstViewController()
{
	testList[0] =  "AlertViewTest" ;
	testList[1] =  "ButtonTest" ;
	testList[2] =  "CollectionViewTest" ;
	testList[3] =  "ImageViewTest" ;
	testList[4] =  "LabelTest" ;
	testList[5] =  "NavigationBarTest" ;
	testList[6] =  "ProgressTest" ;
	testList[7] =  "ScrollViewTest" ;
	testList[8] =  "SegmentedControlTest" ;
	testList[9] =  "SliderTest" ;
	testList[10] =  "SwitchTest" ;
	testList[11] =  "TabBarTest" ;
	testList[12] =  "TableViewTest" ;
	testList[13] =  "TextFieldTest" ;
	testList[14] =  "ExtensionsTest" ;
}

FirstViewController::~FirstViewController()
{

}

void FirstViewController::viewDidLoad()
{
	size = this->getView()->getBounds().size;
	tableView = CATableView::createWithCenter(CCRect(size.width*0.5, size.height*0.5, size.width, size.height));
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
			VIEWCONTROLLER_JUMP(AlertViewTest, "AlertViewTest");
			break;
		}
		case BUTTONTEST:
		{
			VIEWCONTROLLER_JUMP(ButtonTest, "ButtonTest");
			break;
		}
		case COLLECTIONVIEWTEST:
		{
			VIEWCONTROLLER_JUMP(CollectionViewTest, "CollectionTest");
			break;
		}
		case IMAGEVIEWTEST:
		{
			VIEWCONTROLLER_JUMP(ImageViewTest, "ImageViewTest");
			break;
		}
		case LABELTEST:
		{
			VIEWCONTROLLER_JUMP(LabelTest, "LabelTest");
			break;
		}
		case NAVIGATIONBARTEST:
		{
			VIEWCONTROLLER_JUMP(NavigationBarTest, "NavigationBarTest");
			this->getNavigationController()->setNavigationBarHidden(true,true);
			break;
		}
		case PROGRESSTEST:
		{
			VIEWCONTROLLER_JUMP(ProgressTest,"ProgressTest");
			break;
		}
		case SCROLLVIEWTEST:
		{
			VIEWCONTROLLER_JUMP(ScrollViewTest, "ScrollViewTest");
			break;
		}
		case SEGMENTEDCONTROLTEST:
		{
			VIEWCONTROLLER_JUMP(SegmentedControlTest, "SegmentedTest");
			break;
		}	
		case SLIDERTEST:
		{
			VIEWCONTROLLER_JUMP(SliderTest,"SliderTest");
			break;
		}
		case SWITCHTEST:
		{
			VIEWCONTROLLER_JUMP(SwitchTest, "SwitchTest");
			break;
		}
		case TABBARTEST:
		{
			VIEWCONTROLLER_JUMP(TabBarTest, "TabBarTest");
			this->getNavigationController()->getTabBarController()->setTabBarHidden(true,true);
			break;
		}
		case TABVIEWTEST:
		{
			VIEWCONTROLLER_JUMP(TableViewTest, "TableViewTest");
			break;
		}
		case TEXTFIELDTEST:
		{
			VIEWCONTROLLER_JUMP(TextFieldTest, "TextFieldTest");
			break;
		}
		case EXTENSIONSTEST:
		{
			VIEWCONTROLLER_JUMP(ExtensionsTest, "JSON analysis");
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
	CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CrossApp");
	if (cell == NULL)
	{
		cell = CATableViewCell::create("CrossApp");
	}

	test = CALabel::createWithCenter(CCRect(cellSize.width*0.5,
                                            cellSize.height*0.5,
                                            size.width*0.6,
                                            cellSize.height*0.5));
	test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	test->setFontSize(40*CROSSAPP_ADPTATION_RATIO);
	test->setColor(ccc4(51, 204, 255, 255));
    test->setText(testList[row]);
	cell->addSubview(test);
    
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
	return 15;
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


