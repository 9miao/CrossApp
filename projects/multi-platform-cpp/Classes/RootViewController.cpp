//
//  RootViewController.cpp
//  project
//
//  Created by  on 14-4-11.
//
//

#include "RootViewController.h"

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
    
    CAImageView* v = CAImageView::createWithImage(CAImage::create("2.jpg"));
    tableView->setTableHeaderHeight(400);
    tableView->setTableHeaderView(v);
    
    CAView* v2 = CAView::createWithFrame(CCRectZero, ccc4(80, 80, 180, 255));
    tableView->setTableFooterHeight(200);
    tableView->setTableFooterView(v2);
    
    CAView* s = CAView::createWithFrame(CCRectZero, ccc4(255, 0, 0, 255));
    tableView->setTablePullDownView(s);
    tableView->setTablePullViewHeight(200);
    
    tableView->setBackGroundImage(CAImage::create("bg.jpg"));
    
    //tableView->setAllowsSelection(true);
    
    CAButton* btn1 = CAButton::createWithFrame(CCRect(10, tableRect.size.height+10, 240, 60));
    btn1->setSprite(CAControlStateNormal, CCLabelTTF::create("目前不可选", "Arial", 30));
    btn1->getSprite(CAControlStateNormal)->setColor(ccBLACK);
    this->getView()->addSubview(btn1);
    btn1->addTarget(this, CAButton_selector(RootViewController::setAllowsSelection), TouchUpInSide);
    
    CAButton* btn2 = CAButton::createWithFrame(CCRect(260, tableRect.size.height+10, 240, 60));
    btn2->setSprite(CAControlStateNormal, CCLabelTTF::create("目前单选", "Arial", 30));
    btn2->getSprite(CAControlStateNormal)->setColor(ccBLACK);
    this->getView()->addSubview(btn2);
    btn2->addTarget(this, CAButton_selector(RootViewController::setAllowsMultipleSelection), TouchUpInSide);
    
    progress = CAProgress::create();
    progress->setFrame(CCRect(520, tableRect.size.height+10, 200, 16));
    this->getView()->addSubview(progress);
    progress->setProgress(0.5f);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(RootViewController::updateProgress), this, 1, false);
    
}

void RootViewController::viewDidUnload()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(RootViewController::updateProgress), this);
}

void RootViewController::updateProgress(float dt)
{
    progress->setProgress(CCRANDOM_0_1(), true);
}

bool RootViewController::setAllowsSelection(CAButton* btn, CCPoint point)
{
    if (tableView->isAllowsSelection())
    {
        tableView->setAllowsSelection(false);
        btn->setSprite(CAControlStateNormal, CCLabelTTF::create("目前不可选", "Arial", 30));
    }
    else
    {
        tableView->setAllowsSelection(true);
        btn->setSprite(CAControlStateNormal, CCLabelTTF::create("目前可选", "Arial", 30));
    }
    btn->getSprite(CAControlStateNormal)->setColor(ccBLACK);
    tableView->reloadData();
    
    
    return false;
}

bool RootViewController::setAllowsMultipleSelection(CAButton* btn, CCPoint point)
{
    if (tableView->isAllowsMultipleSelection())
    {
        tableView->setAllowsMultipleSelection(false);
        btn->setSprite(CAControlStateNormal, CCLabelTTF::create("目前单选", "Arial", 30));
    }
    else
    {
        tableView->setAllowsMultipleSelection(true);
        btn->setSprite(CAControlStateNormal, CCLabelTTF::create("目前多选", "Arial", 30));
    }
    btn->getSprite(CAControlStateNormal)->setColor(ccBLACK);
    tableView->reloadData();
    
    return false;
}

void RootViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    char s[32];
    sprintf(s, "viewControll = %ld",this->getNavigationController()->getViewControllerCount());
    
    RootViewController* viewController = new RootViewController();
    viewController->init();
    viewController->setNavigationBarItem(CANavigationBarItem::create(s));
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
        //cell->setBackGroundView(CAView::createWithFrame(cell->getBounds(), ccc4(0, 0, 0, 0)));
    }
    
    CCString* str = CCString::createWithFormat("CELL - %u", row);
    CCLabelTTF* ttf = CCLabelTTF::create(str->getCString(), "Arial", 40);
    ttf->setColor(ccBLACK);
    ttf->setFrame(CCRect(10, 60, 0, 0));
    cell->addSubview(ttf);
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
    return 10;
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

