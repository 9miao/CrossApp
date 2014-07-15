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
    
    
    CAView* footerView = CAView::createWithColor(CAColor_blue);
    
    CALabel* pLabel2 = new CALabel();
	pLabel2->initWithCenter(CCRectMake(sz.width / 2, sz.height / 2, sz.width, sz.height));
	pLabel2->setTextAlignment(CATextAlignmentCenter);
	pLabel2->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	pLabel2->setText("Footer View");
	pLabel2->setFontName("fonts/arial.ttf");
	pLabel2->setFontSize(28 * CROSSAPP_ADPTATION_RATIO);
	pLabel2->setColor(CAColor_gray);
	footerView->addSubview(pLabel2);
	pLabel2->release();
    
	m_pCollectionView->setCollectionFooterView(footerView);
    m_pCollectionView->setCollectionFooterHeight(100);

	m_pCollectionView->setAllowsSelection(true);

}

void ThirdViewController::setAllowsSelection(CAButton* btn, CCPoint point)
{
    CCLog("%f   %f", point.x, point.y);
}

void ThirdViewController::viewDidUnload()
{

}


CACollectionViewCell* ThirdViewController::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
	CACollectionViewCell* pCell = new CACollectionViewCell();
	char cIdentifier[1024] = { 0 };
	sprintf(cIdentifier, "{%d, %d, %d}" ,section, row, item);
	pCell->initWithReuseIdentifier(cIdentifier);

//	CALabel* label = CALabel::createWithCenter(CCRectMake(cellSize.width / 2,
//                                                          cellSize.height - 30,
//                                                          cellSize.width,
//                                                          cellSize.height));
//	label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
//	label->setTextAlignment(CATextAlignmentCenter);
//	label->setFontSize(28 * CROSSAPP_ADPTATION_RATIO);
//	label->setText(pCell->getReuseIdentifier());
//	pCell->addSubview(label);

	CAImageView* pCellImage = CAImageView::createWithCenter(CCRectMake(cellSize.width / 2,
                                                                       cellSize.height / 2 - 20,
                                                                       cellSize.width*0.7f,
                                                                       cellSize.height*0.7f));
    pCell->addSubview(pCellImage);
    pCellImage->setImageAsyncWithFile("1.jpg");

	return pCell;
}

unsigned int ThirdViewController::numberOfSectionsInCollectioView()
{
	return 2;
}

unsigned int ThirdViewController::numberOfRowsInSectionCollectionView(CACollectionView *collectionView, unsigned int section)
{
	return 4;
}

unsigned int ThirdViewController::collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section)
{
	return 80 * CROSSAPP_ADPTATION_RATIO;
}

CAView* ThirdViewController::collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithFrame(CCRect(0, 0, 0, 0), ccc4(224, 224, 224, 255));
    
    CCString* str = CCString::createWithFormat("Section Header - %u", section);
    CALabel* label = CALabel::createWithFrame(CCRect(20, 0, 200, viewSize.height));
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(24);
    label->setText(str->getCString());
    label->setColor(ccc4(127, 127, 127, 255));
    view->addSubview(label);
    
	return view;
}

unsigned int ThirdViewController::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
	return 3;
}

unsigned int ThirdViewController::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
    int height = this->getView()->getBounds().size.width * 0.75;
	return height - 1 * height / 3;
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