
#include "FirstViewController.h"

FirstViewController::FirstViewController()
{

}

FirstViewController::~FirstViewController()
{

}

void FirstViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
void FirstViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
	CCRect winRect = this->getView()->getBounds();
    CATableView *table = CATableView::createWithCenter(CCRectMake(winRect.size.width/2,
                                                                  winRect.size.height/2,
                                                                  winRect.size.width,
                                                                  winRect.size.height));
    table->setTableViewDataSource(this);
    this->getView()->addSubview(table);
}
CATableViewCell* FirstViewController::tableCellAtIndex(CATableView* table,
                                                       const CCSize& cellSize,
                                                       unsigned int section, unsigned int row)
{
    CATableViewCell *cell = table->dequeueReusableCellWithIdentifier("cell");
    if (!cell)
    {
        cell= CATableViewCell::create("cell");
        
    }
    return cell;
}
unsigned int FirstViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return  20;
}
unsigned int FirstViewController::tableViewHeightForRowAtIndexPath(CATableView* table,
                                                                   unsigned int section,
                                                                   unsigned int row)
{
    return 100;
}