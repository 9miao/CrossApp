//
//  ViewController.cpp
//  project
//
//  Created by  on 14-4-16.
//
//

#include "ThirdViewController.h"



void ThirdViewController::viewDidLoad()
{
	CCRect rect = this->getView()->getBounds();

	CCRect tableRect = rect;

	m_pCollectionView = new CACollectionView();
	m_pCollectionView->initWithFrame(tableRect);
	m_pCollectionView->setCollectionViewDataSource(this);
	m_pCollectionView->setCollectionViewDelegate(this);
	this->getView()->addSubview(m_pCollectionView);
	m_pCollectionView->release();
	m_pCollectionView->setBackGroundColor(CAColor_gray);

    CAView* headView = CAView::createWithColor(CAColor_red);
    
    CCSize sz = CCSize(rect.size.width, 100);
	CALabel* pLabel = new CALabel();
	pLabel->initWithCenter(CCRectMake(sz.width / 2, sz.height / 2, sz.width, sz.height));
	pLabel->setTextAlignment(CATextAlignmentCenter);
	pLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	pLabel->setText("CollectionView");
	pLabel->setFontName("fonts/arial.ttf");
	pLabel->setFontSize(36);
	pLabel->setColor(CAColor_white);
	headView->addSubview(pLabel);
	pLabel->release();
    
	m_pCollectionView->setCollectionHeaderView(headView);
    m_pCollectionView->setCollectionHeaderHeight(100);
	m_pCollectionView->setCollectionFooterView(CAView::createWithColor(CAColor_blue));
    m_pCollectionView->setCollectionFooterHeight(100);

	CAView* s = CAView::createWithFrame(CCRectZero, CAColor_clear);
	m_pCollectionView->setCollectionPullUpView(s);
    CAView* s2 = CAView::createWithFrame(CCRectZero, CAColor_clear);
	m_pCollectionView->setCollectionPullDownView(s2);
	m_pCollectionView->setCollectionPullViewHeight(200);

    
	m_pCollectionView->setAllowsSelection(true);

}

void ThirdViewController::setAllowsSelection(CAButton* btn, CCPoint point)
{
    CCLog("%f   %f", point.x, point.y);
}

void ThirdViewController::viewDidUnload()
{

}

CACollectionViewCell* ThirdViewController::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int row, unsigned int item)
{
	CACollectionViewCell* pCell = new CACollectionViewCell();
	char cIdentifier[1024] = { 0 };
	sprintf(cIdentifier, "{%d, %d}", row, item);
	pCell->initWithReuseIdentifier(cIdentifier);

	CALabel* label = CALabel::createWithCenter(CCRectMake(cellSize.width / 2, cellSize.height - 30, cellSize.width, cellSize.height));
	label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	label->setTextAlignment(CATextAlignmentCenter);
	label->setFontSize(28);
	label->setText(pCell->getReuseIdentifier());
	pCell->addSubview(label);

	CAImageView* pCellImage = CAImageView::createWithCenter(CCRectMake(cellSize.width / 2, cellSize.height / 2 - 20, cellSize.width*0.7f, cellSize.height*0.7f));
    pCell->addSubview(pCellImage);
    pCellImage->setImageAsyncWithFile("1.jpg");
	return pCell;
}



unsigned int ThirdViewController::numberOfRowsInCollectionView(CACollectionView *collectionView)
{
	return 6;
}

unsigned int ThirdViewController::numberOfItemsInRows(CACollectionView *collectionView, unsigned int row)
{
	return 2 + row % 2;
}

unsigned int ThirdViewController::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int row)
{
    int height = this->getView()->getBounds().size.width * 0.75;
	return height - (row % 2) * height / 3;
}

void ThirdViewController::touchUpInSide()
{

}

void ThirdViewController::touchUpInSide2()
{
    if (this->getNavigationController() && this->getNavigationController()->popViewControllerAnimated(true))
    {
        return;
    }
    
    if (this->getTabBarController() && this->getTabBarController()->getNavigationController()->popViewControllerAnimated(true))
    {
        return;
    }
}