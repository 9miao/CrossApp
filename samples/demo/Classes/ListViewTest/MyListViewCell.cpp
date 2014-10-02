#include "MyListViewCell.h"

MyListViewCell::MyListViewCell()
{

}

MyListViewCell::~MyListViewCell()
{

}

MyListViewCell* MyListViewCell::create(const std::string& identifier, const CADipRect& _rect)
{
	MyListViewCell* listViewCell = new MyListViewCell();
	if (listViewCell&&listViewCell->initWithReuseIdentifier(identifier))
	{
		listViewCell->setFrame(_rect);
		listViewCell->autorelease();
		return listViewCell;
	}
	CC_SAFE_DELETE(listViewCell);
	return NULL;
}

void MyListViewCell::initWithCell()
{
	CADipSize _size = this->getFrame().size;
	CALabel* test = CALabel::createWithCenter(CADipRect(_size.width*0.5,
		_size.height*0.5,
		_size.width*0.8,
		_size.height));
	test->setTextAlignment(CATextAlignmentCenter);
	test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	test->setFontSize(_px(40));
	test->setTag(100);
	this->addSubview(test);
}

void MyListViewCell::cellBtnCallback(CAControl* btn, CCPoint point)
{

}

void MyListViewCell::normalListViewCell()
{
	this->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/cell_bkg4.png")));
}

void MyListViewCell::highlightedListViewCell()
{
	this->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_normal.png")));
}

void MyListViewCell::selectedListViewCell()
{
	this->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/ex4.png")));
}

void MyListViewCell::disabledListViewCell()
{
	this->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/ex5.png")));
}