#include "MyTableViewCell.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

MyTableViewCell::MyTableViewCell()
{

}

MyTableViewCell::~MyTableViewCell()
{

}

MyTableViewCell* MyTableViewCell::create(const std::string& identifier)
{
	//CATableViewCell* tableViewCell = new CATableViewCell();
	MyTableViewCell* tableViewCell = new MyTableViewCell();
	if(tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
	{
		tableViewCell->autorelease();
		return tableViewCell;
	}
	CC_SAFE_DELETE(tableViewCell);
	return NULL;
}

void MyTableViewCell::initCell()
{
	CADipSize cellSize = this->getFrame().size;
	CALabel* cellText = CALabel::createWithCenter(CADipRect(cellSize.width*0.1, cellSize.height*0.5, cellSize.width*0.3, cellSize.height*0.8));
	cellText->setTag(100);
	cellText->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	cellText->setColor(CAColor_blueStyle);
	cellText->setTextAlignment(CATextAlignmentCenter);
	cellText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->addSubview(cellText);

	CAButton* cellBtn = CAButton::createWithCenter(CADipRect(cellSize.width*0.8, cellSize.height*0.5, cellSize.width*0.2, cellSize.height*0.5), CAButtonTypeRoundedRect);
	cellBtn->setTag(102);
	cellBtn->setTitleForState(CAControlStateAll, "Touch");
	cellBtn->addTarget(this, CAControl_selector(MyTableViewCell::cellBtnCallback), CAControlEventTouchUpInSide);
	this->addSubview(cellBtn);
}

void MyTableViewCell::cellBtnCallback(CAControl* btn, CCPoint point)
{

}