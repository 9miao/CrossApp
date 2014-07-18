//
//  RootViewController.cpp
//  project
//
//  Created by  on 14-4-11.
//
//

#include "RootViewController.h"
#include "FirstViewController.h"
#include "ThirdViewController.h"

RootViewController::RootViewController()
{

}

RootViewController::~RootViewController()
{

}

void RootViewController::viewDidLoad()
{
    CCRect rect = this->getView()->getBounds();
    
    CCRect tableRect = rect;
    tableRect.size.height = rect.size.height - 80 * CROSSAPP_ADPTATION_RATIO;

    tableView = new CATableView();
    tableView->initWithFrame(tableRect);
    tableView->setTableViewDataSource(this);
    tableView->setTableViewDelegate(this);
    this->getView()->addSubview(tableView);
    tableView->release();
    tableView->setBackGroundColor(CAColor_gray);
    tableView->setAllowsSelection(true);
    
    CAScrollView* view = CAScrollView::createWithCenter(CCRectZero);
    view->setViewSize(CCSize(rect.size.width * 2, rect.size.width * 0.5));
    view->setTouchSidingDirection(CATouchSidingDirectionHorizontal);
    view->setBounceVertical(false);
    view->setShowsHorizontalScrollIndicator(false);
    view->setShowsVerticalScrollIndicator(false);
    
    CAImageView* imageView = CAImageView::createWithFrame(CCRect(0, 0, view->getViewSize().width, view->getViewSize().height));
    imageView->setImage(CAImage::create("bg0.jpg"));
    view->addSubview(imageView);
    
    CALabel* label = CALabel::createWithCenter(CCRect(view->getViewSize().width/2,
                                                      view->getViewSize().height/2,
                                                      view->getViewSize().width,
                                                      view->getViewSize().height));
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setTextAlignment(CATextAlignmentCenter);
    label->setText("CrossApp beta v0.2.0");
    label->setFontSize(36);
    view->addSubview(label);
    
    tableView->setTableHeaderHeight(rect.size.width * 0.5);
    tableView->setTableHeaderView(view);
    
    CAView* s = CAView::createWithFrame(CCRectZero, CAColor_white);
    tableView->setTablePullUpView(s);
    CAView* s2 = CAView::createWithFrame(CCRectZero, CAColor_white);
    tableView->setTablePullDownView(s2);
    tableView->setTablePullViewHeight(this->getView()->getBounds().size.height/6);
    
    button_ = CAButton::createWithFrame(CCRect(150 * CROSSAPP_ADPTATION_RATIO, tableRect.size.height+10, 200, 60), CAButtonTypeRoundedRect);
    button_->setTitleForState(CAControlStateNormal, "Hide-Bar");
    button_->setTitleForState(CAControlStateSelected, "Show-Bar");
    this->getView()->addSubview(button_);
    button_->addTarget(this, CAControl_selector(RootViewController::setAllowsMultipleSelection), CAControlEventTouchUpInSide);
    button_->setAllowsSelected(true);

//    if (this->getNavigationController()->isNavigationBarHidden())
//    {
//        button_->setControlState(CAControlStateSelected);
//    }
    
    switch_ = CASwitch::createWithFrame(CCRect(30 * CROSSAPP_ADPTATION_RATIO, tableRect.size.height+20, 80, 48));
    switch_->addTarget(this, CAControl_selector(RootViewController::setAllowsSelection));
    this->getView()->addSubview(switch_);
    
    //CAScheduler::schedule(schedule_selector(RootViewController::updateProgress), this, 5);
}

void RootViewController::viewDidUnload()
{
    button_->removeTarget(this, CAControl_selector(RootViewController::setAllowsMultipleSelection), CAControlEventTouchUpInSide);
    switch_->removeTarget(this, CAControl_selector(RootViewController::setAllowsSelection));
}

void RootViewController::reshapeViewRectDidFinish()
{
    CCRect rect = this->getView()->getBounds();
    CCLog("%f",rect.size.height);
    CCRect tableRect = rect;
    tableRect.size.height = rect.size.height - 80 * CROSSAPP_ADPTATION_RATIO;
    
    tableView->setFrame(tableRect);
    
    button_->setFrame(CCRect(150 * CROSSAPP_ADPTATION_RATIO,
                             tableRect.size.height+10,
                             200,
                             60 * CROSSAPP_ADPTATION_RATIO));
    
    switch_->setFrame(CCRect(30 * CROSSAPP_ADPTATION_RATIO,
                             tableRect.size.height+20,
                             80,
                             48 * CROSSAPP_ADPTATION_RATIO));
    
}

void RootViewController::setAllowsSelection(CAControl* sender, CCPoint point)
{
//    CASwitch* switch_ = (CASwitch*)sender;
//    
//    if (switch_->isOn() == false)
//    {
//        tableView->setAllowsSelection(false);
//    }
//    else
//    {
//        tableView->setAllowsSelection(true);
//    }

    tableView->reloadData();
}

void RootViewController::setAllowsMultipleSelection(CAControl* sender, CCPoint point)
{
	CAButton* btn = (CAButton*)sender;
//    
//	if (btn->isSelected())
//    {
//        tableView->setAllowsMultipleSelection(false);
//    }
//    else
//    {
//        tableView->setAllowsMultipleSelection(true);
//    }

    if (btn->isSelected())
    {
        this->getNavigationController()->getTabBarController()->setTabBarHidden(false, true);
        this->getNavigationController()->setNavigationBarHidden(false, true);
    }
    else
    {
        this->getNavigationController()->getTabBarController()->setTabBarHidden(true, true);
        this->getNavigationController()->setNavigationBarHidden(true, true);
    }
}

void RootViewController::scrollViewDidEndDragging(CAScrollView* view)
{
    CCPoint p = view->getContentOffset();
    int x = (p.x + view->getFrame().size.width/2) / view->getFrame().size.width;
    
    view->setContentOffset(CCPointMake(x * view->getFrame().size.width, 0), true);
}

void RootViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    CC_RETURN_IF(this->getNavigationController()->getViewControllerCount()>10);
    char s[32];
    sprintf(s, "The Page No.%ld",this->getNavigationController()->getViewControllerCount());
    
    CANavigationBarItem* item = CANavigationBarItem::create(s);
    //item->setShowGoBackButton(false);
    ThirdViewController* viewController = new ThirdViewController();
    viewController->init();
    viewController->setNavigationBarItem(item);
    viewController->setTitle("view1");
    
    this->getNavigationController()->replaceViewController(viewController, true);
    viewController->autorelease();
    
    CCLog("selected = %d %d",section, row);
}

void RootViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    CCLog("deselected = %d %d",section, row);
}

CATableViewCell* RootViewController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("aaa");
    if (cell == NULL)
    {
        cell = CATableViewCell::create("aaa");
    }
    
    CCString* str = CCString::createWithFormat("CELL - %u", row);

    CAButton* btn = CAButton::createWithCenter(CCRect(cellSize.width - 30,
                                                      cellSize.height/2,
                                                      100 * CROSSAPP_ADPTATION_RATIO,
                                                      60 * CROSSAPP_ADPTATION_RATIO),
                                               CAButtonTypeCustom);
    btn->setImageForState(CAControlStateAll,
                          CAImage::create("source_material/cell_btn_right.png"));
    btn->setImageColorForState(CAControlStateHighlighted, ccc4(50, 193, 255, 255));
    cell->addSubview(btn);

    CALabel* label = CALabel::createWithFrame(CCRect(40, 0, 200, cellSize.height));
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(32 * CROSSAPP_ADPTATION_RATIO);
    label->setText(str->getCString());
    cell->addSubview(label);
    
    return cell;
    
}

CAView* RootViewController::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
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

CAView* RootViewController::tableViewSectionViewForFooterInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
    return CAView::createWithColor(CAColor_clear);
}

unsigned int RootViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return 5;
}

unsigned int RootViewController::numberOfSections(CATableView *table)
{
    return 4;
}

unsigned int RootViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return this->getView()->getBounds().size.height/8;
}

unsigned int RootViewController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    return this->getView()->getBounds().size.height/24;
}

unsigned int RootViewController::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
    return this->getView()->getBounds().size.height/8;
}

void RootViewController::tableViewDidShowPullDownView(CATableView* table)
{
    CCLog("----------down");
}

void RootViewController::tableViewDidShowPullUpView(CATableView* table)
{
    CCLog("----------up");
}

