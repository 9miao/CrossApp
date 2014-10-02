#include "MyTableViewCell.h"

MyTableViewCell::MyTableViewCell()
{

}

MyTableViewCell::~MyTableViewCell()
{

}

MyTableViewCell* MyTableViewCell::create(const std::string& identifier, const CADipRect& _rect)
{
	MyTableViewCell* tableViewCell = new MyTableViewCell();
	if(tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
	{
		tableViewCell->setFrame(_rect);
		tableViewCell->autorelease();
		return tableViewCell;
	}
	CC_SAFE_DELETE(tableViewCell);
	return NULL;
}

void MyTableViewCell::initWithCell()
{
	CADipSize m_size = this->getFrame().size;
	CALabel* cellText = CALabel::createWithCenter(CADipRect(m_size.width*0.1, m_size.height*0.5, m_size.width*0.3, m_size.height*0.8));
	cellText->setTag(100);
	cellText->setFontSize(_px(30));
	cellText->setTextAlignment(CATextAlignmentCenter);
	cellText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->addSubview(cellText);

	CAButton* btnOnCell = CAButton::createWithCenter(CADipRect(m_size.width*0.85, m_size.height*0.5, m_size.width*0.2, m_size.height*0.7), CAButtonTypeRoundedRect);
	btnOnCell->setTag(102);
	btnOnCell->setTitleForState(CAControlStateAll, "Touch");
	btnOnCell->addTarget(this, CAControl_selector(MyTableViewCell::cellBtnCallback), CAControlEventTouchUpInSide);
	this->addSubview(btnOnCell);
}

void MyTableViewCell::cellBtnCallback(CAControl* btn, CCPoint point)
{
	
}

void MyTableViewCell::normalTableViewCell()
{
	this->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/cell_bkg4.png")));
}

void MyTableViewCell::highlightedTableViewCell()
{
	this->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_normal.png")));
}

void MyTableViewCell::selectedTableViewCell()
{
	this->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/cell_bkg3.png")));
}

void MyTableViewCell::disabledTableViewCell()
{
	this->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_disabled.png")));
}