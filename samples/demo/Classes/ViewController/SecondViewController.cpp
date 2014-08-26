#include "SecondViewController.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

#define CELL_COUNT personList.size()

SecondViewController::SecondViewController()
{
	for (int i = 0; i < NUM; i++)
	{
		sect[i] = CELL_COUNT;
	}
}

SecondViewController::~SecondViewController()
{
	personList.clear();
}

void SecondViewController::viewDidLoad()
{	
	loadJsonData();
    CCSize size = this->getView()->getBounds().size;
	p_TableView = CATableView::createWithCenter(CCRect(size.width*0.5, size.height*0.5, size.width, size.height));
	p_TableView->setTableViewDataSource(this);
	p_TableView->setTableViewDelegate(this);
	p_TableView->setAllowsSelection(true);
	this->getView()->addSubview(p_TableView);
}

void SecondViewController::loadJsonData()
{
	Reader reader;
	Value value;
	string jsonFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("information.json");
	CCString *jsonData = CCString::createWithContentsOfFile(jsonFile.c_str());
	if (reader.parse(jsonData->getCString(), value))
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
			personList.pushBack(personInfo);
		}
	}
}

void SecondViewController::viewDidUnload()
{
	
}

void SecondViewController::reshapeViewRectDidFinish()
{

}

void SecondViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	
}

void SecondViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}

void SecondViewController::tableViewDidShowPullDownView(CATableView* table)
{

}

void SecondViewController::tableViewDidShowPullUpView(CATableView* table)
{

}

CATableViewCell* SecondViewController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
	Info* p_List = (Info*)personList.at(row%8);
	CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CrossApp");
	if (cell == NULL)
	{
		CCLog("Cell-%d",row);
		cell = CATableViewCell::create("CrossApp");
		CALabel* p_Name = CALabel::createWithCenter(CCRect(cellSize.width*0.1, cellSize.height*0.5, cellSize.width*0.2, cellSize.height));
		p_Name->setTag(9);
		p_Name->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
		p_Name->setColor(CAColor_blueStyle);
		p_Name->setTextAlignment(CATextAlignmentCenter);
		p_Name->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		cell->addSubview(p_Name);

		CALabel* p_Num = CALabel::createWithCenter(CCRect(cellSize.width*0.3, cellSize.height*0.5, cellSize.width*0.2, cellSize.height));
		p_Num->setTag(10);
		p_Num->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
		p_Num->setColor(CAColor_blueStyle);
		p_Num->setTextAlignment(CATextAlignmentCenter);
		p_Num->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		cell->addSubview(p_Num);

		CALabel* p_Gender = CALabel::createWithCenter(CCRect(cellSize.width*0.5, cellSize.height*0.5, cellSize.width*0.2, cellSize.height));
		p_Gender->setTag(11);
		p_Gender->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
		p_Gender->setColor(CAColor_blueStyle);
		p_Gender->setTextAlignment(CATextAlignmentCenter);
		p_Gender->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		cell->addSubview(p_Gender);

		CALabel* p_Occupation = CALabel::createWithCenter(CCRect(cellSize.width*0.8, cellSize.height*0.5, cellSize.width*0.3, cellSize.height));
		p_Occupation->setTag(12);
		p_Occupation->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
		p_Occupation->setColor(CAColor_blueStyle);
		p_Occupation->setTextAlignment(CATextAlignmentCenter);
		p_Occupation->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		cell->addSubview(p_Occupation);
	}
	CALabel* p_Name = (CALabel*)cell->getSubviewByTag(9);
	p_Name->setText(p_List->name.c_str());
	CALabel* p_Num = (CALabel*)cell->getSubviewByTag(10);
	p_Num->setText(p_List->num.c_str());
	CALabel* p_Gender = (CALabel*)cell->getSubviewByTag(11);
	p_Gender->setText(p_List->gender.c_str());
	CALabel* p_Occupation = (CALabel*)cell->getSubviewByTag(12);
	p_Occupation->setText(p_List->occupation.c_str());


	return cell;

}

CAView* SecondViewController::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
	char head[10] = "";
	CAView* view = CAView::createWithColor(CAColor_gray);
	CAButton* headControl1 = CAButton::createWithCenter(CCRect(viewSize.width*0.1, viewSize.height*0.5, viewSize.height*0.8, viewSize.height*0.8),
		CAButtonTypeRoundedRect);
	headControl1->setTag(100 + (int)section);
	if (sect[section] == CELL_COUNT)
	{
		headControl1->setBackGroundViewForState(CAControlStateNormal, CAImageView::createWithImage(CAImage::create("source_material/close1.png")));
	}
	else
	{
		headControl1->setBackGroundViewForState(CAControlStateNormal, CAImageView::createWithImage(CAImage::create("source_material/open1.png")));
	}
	headControl1->addTarget(this, CAControl_selector(SecondViewController::switchCellListInSection), CAControlEventTouchUpInSide);
	view->addSubview(headControl1);

	CALabel* header = CALabel::createWithCenter(CCRect(viewSize.width*0.5, viewSize.height*0.5, viewSize.width*0.3, viewSize.height));
	sprintf(head, "Section-%d", section);
	header->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	header->setText(head);
	header->setColor(CAColor_blueStyle);
	header->setTextAlignment(CATextAlignmentCenter);
	header->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	view->addSubview(header);

	return view;
}

CAView* SecondViewController::tableViewSectionViewForFooterInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
	CAView* view = CAView::createWithColor(CAColor_yellow);

    char head[10] = "";
    CALabel* header = CALabel::createWithCenter(CCRect(viewSize.width*0.5, viewSize.height*0.5, viewSize.width*0.3, viewSize.height));
	sprintf(head, "Section-%d", section);
	header->setFontSize(_px(30));
	header->setText(head);
	header->setColor(CAColor_blueStyle);
	header->setTextAlignment(CATextAlignmentCenter);
	header->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	view->addSubview(header);
    
	return view;
}

unsigned int SecondViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
	return sect[section];
}

unsigned int SecondViewController::numberOfSections(CATableView *table)
{
	return NUM;
}

unsigned int SecondViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return this->getView()->getBounds().size.height*0.1;
}

unsigned int SecondViewController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
	return this->getView()->getBounds().size.height*0.1;
}

unsigned int SecondViewController::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
	return this->getView()->getBounds().size.height*0.1;
}

void SecondViewController::switchCellListInSection(CAControl* btn, CCPoint point)
{
	CAButton* headerBtn = (CAButton*)btn;
	int section = btn->getTag() - 100;
	CC_RETURN_IF(section >= NUM);
	sect[section] = sect[section] ? 0 : CELL_COUNT;
	btn->retain()->autorelease();
	p_TableView->reloadData();
}

void SecondViewController::closeCellListInSection(CAControl* btn, CCPoint point)
{

}
