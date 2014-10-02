#include "ExtensionsTest.h"
#include "AddressBookTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

ExtensionsTest::ExtensionsTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

ExtensionsTest::~ExtensionsTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

void ExtensionsTest::viewDidLoad()
{
	this->getNavigationBarItem()->setTitle("Json analysis");
	size = this->getView()->getBounds().size;

	table = CATableView::createWithCenter(CADipRect(size.width*0.5, size.height*0.5, size.width, size.height));
	table->setTableViewDataSource(this);
	table->setTableViewDelegate(this);
	table->setAllowsSelection(true);
	table->setSeparatorViewHeight(1);
	this->getView()->addSubview(table);

	CADipSize nSize = this->getNavigationController()->getNavigationBar()->getBounds().size;
	next = CAButton::createWithCenter(CADipRect(nSize.width*0.905, nSize.height*0.5, nSize.height*0.9, nSize.height*0.8), CAButtonTypeCustom);
	next->setTag(1000);
	next->setImageForState(CAControlStateNormal, CAImage::create("source_material/btn_right_white.png"));
	next->setImageColorForState(CAControlStateHighlighted, ccc4(0, 255, 200, 255));
	next->addTarget(this, CAControl_selector(ExtensionsTest::nextViewController), CAControlEventTouchUpInSide);
	this->getNavigationController()->getNavigationBar()->addSubview(next);

	loadJsonData();
}

void ExtensionsTest::loadJsonData(void)
{
	Reader reader;
	Value value;
	string jsonFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("information.json");
	CCString *jsonData = CCString::createWithContentsOfFile(jsonFile.c_str());
	if (reader.parse(jsonData->getCString(),value))
	{
		int length = value["info"].size();
		for (int index = 0; index < length; index++)
		{
			Info* personInfo = new Info();
			personInfo->autorelease();
			personInfo->name = value["info"][index]["name"].asString();
			personInfo->num = value["info"][index]["num"].asString();
			personInfo->gender = value["gender"].asString();
			personInfo->occupation = value["occupation"].asString();
			personList.insert(index,personInfo);
		}
	}
}

void ExtensionsTest::nextViewController(CAControl* btn, CCPoint point)
{
	next->setVisible(false);
	AddressBookTest* addressBookView = new AddressBookTest();
	addressBookView->init();
	addressBookView->setNavigationBarItem(CANavigationBarItem::create("AddressBookTest"));
	addressBookView->autorelease();
	this->getNavigationController()->pushViewController(addressBookView,true);
}

void ExtensionsTest::viewDidUnload()
{
	personList.clear();
	next->removeFromSuperview();
}

void ExtensionsTest::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}

void ExtensionsTest::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}

CATableViewCell* ExtensionsTest::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
	CADipSize _size = cellSize;
	Info* p_List = (Info*)personList.getValue(row);
	CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CrossApp");
	if (cell == NULL)
	{
		cell = CATableViewCell::create("CrossApp");
		CALabel* p_Name = CALabel::createWithCenter(CADipRect(_size.width*0.1, _size.height*0.5, _size.width*0.2, _size.height));
		p_Name->setTag(NAME);
		p_Name->setText(p_List->name.c_str());
		p_Name->setFontSize(_px(30));
		p_Name->setColor(CAColor_blueStyle);
		p_Name->setTextAlignment(CATextAlignmentCenter);
		p_Name->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		cell->addSubview(p_Name);

		CALabel* p_Num = CALabel::createWithCenter(CADipRect(_size.width*0.3, _size.height*0.5, _size.width*0.2, _size.height));
		p_Num->setTag(NUM);
		p_Num->setText(p_List->num.c_str());
		p_Num->setFontSize(_px(30));
		p_Num->setColor(CAColor_blueStyle);
		p_Num->setTextAlignment(CATextAlignmentCenter);
		p_Num->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		cell->addSubview(p_Num);

		CALabel* p_Gender = CALabel::createWithCenter(CADipRect(_size.width*0.5, _size.height*0.5, _size.width*0.2, _size.height));
		p_Gender->setTag(GENDER);
		p_Gender->setText(p_List->gender.c_str());
		p_Gender->setFontSize(_px(30));
		p_Gender->setColor(CAColor_blueStyle);
		p_Gender->setTextAlignment(CATextAlignmentCenter);
		p_Gender->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		cell->addSubview(p_Gender);

		CALabel* p_Occupation = CALabel::createWithCenter(CADipRect(_size.width*0.8, _size.height*0.5, _size.width*0.3, _size.height));
		p_Occupation->setTag(OCCUPATION);
		p_Occupation->setText(p_List->occupation.c_str());
		p_Occupation->setFontSize(_px(30));
		p_Occupation->setColor(CAColor_blueStyle);
		p_Occupation->setTextAlignment(CATextAlignmentCenter);
		p_Occupation->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		cell->addSubview(p_Occupation);
	}
	CALabel* p_Name = (CALabel*)cell->getSubviewByTag(NAME);
	p_Name->setText(p_List->name.c_str());
	CALabel* p_Num = (CALabel*)cell->getSubviewByTag(NUM);
	p_Num->setText(p_List->num.c_str());
	CALabel* p_Gender = (CALabel*)cell->getSubviewByTag(GENDER);
	p_Gender->setText(p_List->gender.c_str());
	CALabel* p_Occupation = (CALabel*)cell->getSubviewByTag(OCCUPATION);
	p_Occupation->setText(p_List->occupation.c_str());

	return cell;

}

CAView* ExtensionsTest::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
	CAView* view = CAView::createWithColor(CAColor_gray);

	return view;
}

CAView* ExtensionsTest::tableViewSectionViewForFooterInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
	CAView* view = CAView::createWithColor(CAColor_gray);


	return view;
}

unsigned int ExtensionsTest::numberOfRowsInSection(CATableView *table, unsigned int section)
{
	return personList.size();
}

unsigned int ExtensionsTest::numberOfSections(CATableView *table)
{
	return 1;
}

unsigned int ExtensionsTest::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return size.height*0.1;
}

unsigned int ExtensionsTest::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
	return 0;
}

unsigned int ExtensionsTest::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
	return 0;
}