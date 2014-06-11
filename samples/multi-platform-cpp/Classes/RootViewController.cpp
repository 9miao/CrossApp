//
//  RootViewController.cpp
//  project
//
//  Created by  on 14-4-11.
//
//

#include "RootViewController.h"

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
    tableRect.size.height = rect.size.height - 80;

    tableView = new CATableView();
    tableView->initWithFrame(tableRect);
    tableView->setTableViewDataSource(this);
    tableView->setTableViewDelegate(this);
    this->getView()->addSubview(tableView);
    tableView->release();
    
    CAScrollView* scroll = new CAScrollView();
    scroll->initWithFrame(CCRect(0, 0, rect.size.width, rect.size.width * 0.6));
    scroll->setViewSize(CCSize(rect.size.width * 3, rect.size.width * 0.6));
    scroll->setBounceVertical(false);
    scroll->setShowsVerticalScrollIndicator(false);
    scroll->setScrollViewDelegate(this);
    tableView->setTableHeaderHeight(rect.size.width * 0.6);
    tableView->setTableHeaderView(scroll);
    
    for (int i=0; i<3; i++)
    {
        CAImageView* image = CAImageView::createWithImage(CAImage::create("2.jpg"));
        image->setFrame(CCRect(i * rect.size.width, 0, rect.size.width, rect.size.width * 0.6));
        scroll->addSubview(image);
    }
    
    CAView* v2 = CAView::createWithFrame(CCRectZero, ccc4(80, 80, 180, 255));
    tableView->setTableFooterHeight(200);
    tableView->setTableFooterView(v2);
    
    CAView* s = CAView::createWithFrame(CCRectZero, ccc4(127, 255, 127, 127));
    tableView->setTablePullUpView(s);
    tableView->setTablePullViewHeight(200);
    
    tableView->setBackGroundImage(CAImage::create("bg.jpg"));
    
    button_ = CAButton::createWithFrame(CCRect(200, tableRect.size.height+10, 320, 60), CAButtonTypeRoundedRect);
    button_->setTitleForState(CAControlStateNormal, "Hide-Bar");
    button_->setTitleForState(CAControlStateSelected, "Show-Bar");
    this->getView()->addSubview(button_);
    button_->addTarget(this, CAControl_selector(RootViewController::setAllowsMultipleSelection), CAControlEventTouchUpInSide);
    button_->setAllowsSelected(true);
    
    if (this->getNavigationController()->isNavigationBarHidden())
    {
        button_->setControlState(CAControlStateSelected);
    }
    
    switch_ = CASwitch::createWithFrame(CCRect(10, tableRect.size.height+10, 120, 60));
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
    
    CCRect tableRect = rect;
    tableRect.size.height = rect.size.height - 80;
    
    tableView->setFrame(tableRect);
    
    button_->setFrame(CCRect(200, tableRect.size.height+10, 320, 60));
    
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
    item->setRightButtonItem(CABarButtonItem::create("<返回", NULL, NULL));
    
    RootViewController* viewController = new RootViewController();
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

CATableViewCell* RootViewController::tableCellAtIndex(CATableView *table, unsigned int section, unsigned int row)
{
    
    
    CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("aaa");
    if (cell == NULL)
    {
        cell = CATableViewCell::create("aaa");
        //cell->setBackGroundView(NULL);
    }
    
    CCString* str = CCString::createWithFormat("CELL - %u", row);
    CCLabelTTF* ttf = CCLabelTTF::create(str->getCString(), "Arial", 40);
    ttf->setColor(ccBLACK);
    ttf->setFrame(CCRect(10, 60, 0, 0));
    cell->addSubview(ttf);
    
    CCSize size = this->getView()->getBounds().size;
    
    CAButton* btn = CAButton::createWithCenter(CCRect(size.width - 100, 60, 120, 100), CAButtonTypeRoundedRect);
    btn->setTitleForState(CAControlStateAll, "button");
    cell->addSubview(btn);
    btn->setAllowsSelected(true);
    btn->addTarget(this, CAControl_selector(RootViewController::setAllowsMultipleSelection), CAControlEventTouchUpInSide);
//    CALabel* label = CALabel::create(CCRect(0, 0, 600, 100));
//    label->setOpacity(128);
//    label->setVerticalTextAlignmet(kCCVerticalTextAlignmentCenter);
//    label->setCenterOrigin(CCPoint(300, 60));
//    label->setText(str->getCString());
//    cell->addSubview(label);
    
    return cell;
    
}

CAView* RootViewController::tableViewSectionViewForHeaderInSection(CATableView* table, unsigned int section)
{
    CAView* view = CAView::createWithFrame(CCRect(0, 0, 0, 0), ccc4(200, 200, 255, 255));
    
    CCString* str = CCString::createWithFormat("Header - %u", section);
    CCLabelTTF* ttf = CCLabelTTF::create(str->getCString(), "Arial", 20);
    ttf->setColor(ccc3(127, 127, 127));
    ttf->setFrame(CCRect(10, 10, 0, 0));
    view->addSubview(ttf);
    
    return view;
}

CAView* RootViewController::tableViewSectionViewForFooterInSection(CATableView* table, unsigned int section)
{
    CAView* view = CAView::createWithFrame(CCRect(0, 0, 0, 0), ccc4(200, 255, 200, 255));
    
    CCString* str = CCString::createWithFormat("Footer - %u", section);
    CCLabelTTF* ttf = CCLabelTTF::create(str->getCString(), "Arial", 20);
    ttf->setColor(ccc3(127, 127, 127));
    ttf->setFrame(CCRect(10, 10, 0, 0));
    view->addSubview(ttf);
    
    return view;
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
    return 120;
}

unsigned int RootViewController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    return 40;
}

unsigned int RootViewController::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
    return 40;
}

void RootViewController::tableViewDidShowPullDownView(CATableView* table)
{
    CCLog("----------down");
}

void RootViewController::tableViewDidShowPullUpView(CATableView* table)
{
    CCLog("----------up");
}

