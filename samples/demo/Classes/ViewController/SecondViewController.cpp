#include "SecondViewController.h"

#define CAColor_blueStyle ccc4(51,204,255,255)


SecondViewController::SecondViewController()
{
	for (int i = 0; i < NUM; i++)
	{
		sect[i] = 8;
	}
}

SecondViewController::~SecondViewController()
{

}

void SecondViewController::viewDidLoad()
{
    CCSize size = this->getView()->getBounds().size;

	p_TableView = CATableView::createWithCenter(CCRect(size.width*0.5, size.height*0.5, size.width, size.height));
	p_TableView->setTableViewDataSource(this);
	p_TableView->setTableViewDelegate(this);
	p_TableView->setAllowsSelection(true);
	this->getView()->addSubview(p_TableView);
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
	CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("CrossApp");
	if (cell == NULL)
	{
		cell = CATableViewCell::create("CrossApp");
        CALabel* cellText = CALabel::createWithCenter(CCRect(cellSize.width*0.1, cellSize.height*0.5, cellSize.width*0.3, cellSize.height*0.5));
        
        cellText->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
        
        cellText->setColor(CAColor_blueStyle);
        cellText->setTextAlignment(CATextAlignmentCenter);
        cellText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        cell->addSubview(cellText);
        cellText->setTag(100);
	}
    
    CALabel* cellText = (CALabel*)cell->getSubviewByTag(100);
    char order[20] = "";
    sprintf(order, "cell-%d", row);
    cellText->setText(order);
    
	return cell;

}

CAView* SecondViewController::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
	char head[10] = "";
	CAView* view = CAView::createWithColor(CAColor_gray);
	CAButton* headControl1 = CAButton::createWithCenter(CCRect(viewSize.width*0.1, viewSize.height*0.5, 100, 80),
		CAButtonTypeCustom);
	headControl1->setTag(100 + (int)section);
	if (sect[section] == 8)
	{
		headControl1->setImageForState(CAControlStateNormal, CAImage::create("close1.png"));
	}
	else
	{
		headControl1->setImageForState(CAControlStateNormal, CAImage::create("open1.png"));
	}
	//headControl1->setImageColorForState(CAControlStateHighlighted, ccc4(0, 255, 200, 255));
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
	return 1;
}

void SecondViewController::switchCellListInSection(CAControl* btn, CCPoint point)
{
	CAButton* headerBtn = (CAButton*)btn;
	int section = btn->getTag() - 100;
	CC_RETURN_IF(section >= NUM);
	sect[section] = sect[section] ? 0 : 8;
	btn->retain();
	p_TableView->reloadData();
	btn->autorelease();
}

void SecondViewController::closeCellListInSection(CAControl* btn, CCPoint point)
{

}
