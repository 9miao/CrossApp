//
//  RootViewController.cpp
//  project
//
//  Created by  on 14-4-11.
//
//

#include "RootViewController.h"
#include "FirstViewController.h"

RootViewController::RootViewController()
{

}

RootViewController::~RootViewController()
{

}

void RootViewController::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    CCRect rect = this->getView()->getBounds();
    
    CCRect tableRect = rect;
    tableRect.size.height = rect.size.height - 80;

    tableView = new CATableView();
    tableView->initWithFrame(tableRect);
    tableView->setTableViewDataSource(this);
    tableView->setTableViewDelegate(this);
    this->getView()->addSubview(tableView);
    tableView->release();
    
    CAView* view = CAView::create();
    
    CALabel* label = CALabel::createWithFrame(CCRect(0, 0, rect.size.width, rect.size.width * 0.5));
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setTextAlignment(CATextAlignmentCenter);
    label->setText("CrossApp alpha v0.1.11");
    view->addSubview(label);
    
    
    tableView->setTableHeaderHeight(rect.size.width * 0.5);
    tableView->setTableHeaderView(view);
    
    CAView* footer = CAView::createWithColor(CAColor_white);
    tableView->setTableFooterHeight(120);
    tableView->setTableFooterView(footer);
    
    
    CAView* s = CAView::createWithFrame(CCRectZero, CAColor_clear);
    tableView->setTablePullUpView(s);
    tableView->setTablePullViewHeight(200);
    
    button_ = CAButton::createWithFrame(CCRect(150, tableRect.size.height+10, 200, 60), CAButtonTypeRoundedRect);
    button_->setTitleForState(CAControlStateNormal, "Hide-Bar");
    button_->setTitleForState(CAControlStateSelected, "Show-Bar");
    this->getView()->addSubview(button_);
    button_->addTarget(this, CAControl_selector(RootViewController::setAllowsMultipleSelection), CAControlEventTouchUpInSide);
    button_->setAllowsSelected(true);
    
//    if (this->getNavigationController()->isNavigationBarHidden())
//    {
//        button_->setControlState(CAControlStateSelected);
//    }
    
    switch_ = CASwitch::createWithFrame(CCRect(30, tableRect.size.height+20, 80, 48));
    switch_->addTarget(this, CAControl_selector(RootViewController::setAllowsSelection));
    this->getView()->addSubview(switch_);
    
    //CAScheduler::schedule(schedule_selector(RootViewController::updateProgress), this, 5, false);
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
    tableRect.size.height = rect.size.height - 80;
    
    tableView->setFrame(tableRect);
    
    button_->setFrame(CCRect(150, tableRect.size.height+10, 200, 60));
    
    switch_->setFrame(CCRect(10, tableRect.size.height+10, 120, 60));
    
}

void RootViewController::setAllowsSelection(CAControl* sender, CCPoint point)
{
    CASwitch* switch_ = (CASwitch*)sender;
    
    if (switch_->isOn() == false)
    {
        tableView->setAllowsSelection(false);
    }
    else
    {
        tableView->setAllowsSelection(true);
    }
    
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
//    
//    tableView->reloadData();
    
    
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
    char s[32];
    sprintf(s, "viewController = %ld",this->getNavigationController()->getViewControllerCount());
    
    CANavigationBarItem* item = CANavigationBarItem::create(s);
    item->setLeftButtonItem(CABarButtonItem::create("<Back", NULL, NULL));
    item->setRightButtonItem(CABarButtonItem::create("Menu", NULL, NULL));
    
    RootViewController* viewController = new RootViewController();
    viewController->init();
    //viewController->setNavigationBarItem(item);
    viewController->setTitle("view1");
    
    this->getNavigationController()->pushViewController(viewController, true);
    viewController->autorelease();
    
    CCLog("selected = %d %d",section, row);
}

void RootViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    CCLog("deselected = %d %d",section, row);
}

CATableViewCell* RootViewController::tableCellAtIndex(CATableView *table, unsigned int section, unsigned int row)
{
    CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("aaa");
    if (cell == NULL)
    {
        cell = CATableViewCell::create("aaa");
    }
    
    CCString* str = CCString::createWithFormat("CELL - %u", row);

    CCSize size = this->getView()->getBounds().size;
    
    CAButton* btn = CAButton::createWithCenter(CCRect(size.width - 30, 40, 100, 60), CAButtonTypeCustom);
    btn->setImageForState(CAControlStateAll, CAImage::create("source_material/cell_btn_right.png"));
    btn->setImageColorForState(CAControlStateHighlighted, ccc4(50, 193, 255, 255));
    cell->addSubview(btn);

//    btn->addTarget(this, CAControl_selector(RootViewController::setAllowsMultipleSelection), CAControlEventTouchUpInSide);
    
    CALabel* label = CALabel::createWithFrame(CCRect(40, 0, 200, 80));
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(28);
    label->setText(str->getCString());
    cell->addSubview(label);
    
    return cell;
    
}

CAView* RootViewController::tableViewSectionViewForHeaderInSection(CATableView* table, unsigned int section)
{
    CAView* view = CAView::createWithFrame(CCRect(0, 0, 0, 0), ccc4(224, 224, 224, 255));
    
    CCString* str = CCString::createWithFormat("Header - %u", section);
    CALabel* label = CALabel::createWithFrame(CCRect(20, 0, 200, 40));
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(20);
    label->setText(str->getCString());
    label->setColor(ccc4(127, 127, 127, 255));
    view->addSubview(label);
    
    return view;
}

CAView* RootViewController::tableViewSectionViewForFooterInSection(CATableView* table, unsigned int section)
{
    return CAView::createWithColor(CAColor_clear);
}

unsigned int RootViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return 5;
}

unsigned int RootViewController::numberOfSections(CATableView *table)
{
    return 3;
}

unsigned int RootViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return 80;
}

unsigned int RootViewController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    return 40;
}

unsigned int RootViewController::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
    return 120;
}

void RootViewController::tableViewDidShowPullDownView(CATableView* table)
{
    CCLog("----------down");
}

void RootViewController::tableViewDidShowPullUpView(CATableView* table)
{
    CCLog("----------up");
}

