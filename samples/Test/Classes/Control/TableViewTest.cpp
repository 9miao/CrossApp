
#include "TableViewTest.h"
#include "support/device/CADevice.h"
#include "CrossAppExt.h"

USING_NS_CC;
USING_NS_CC_EXT;

std::vector<std::string> sectionTitle;

ETableViewCell::ETableViewCell()
{
    this->setAllowsSelected(false);
}

ETableViewCell::~ETableViewCell()
{
    
}

ETableViewCell* ETableViewCell::create(const std::string& identifier)
{
    ETableViewCell* cell = new ETableViewCell();
    if (cell && cell->initWithReuseIdentifier(identifier))
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

void ETableViewCell::highlightedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(CAColor_clear));
}

void ETableViewCell::selectedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(CAColor_clear));
}

void ETableViewCell::initWithCell()
{
    CALabel* test = CALabel::createWithLayout(DLayoutFill);
    test->setColor(ccc4(34, 151, 254, 255));
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(28);
    test->setTag(100);
    this->addSubview(test);
}

//-------------------------------------------------------------------------
//ETableView
ETableView::ETableView(){
    
}

ETableView::~ETableView(){
}

bool ETableView::init(const CrossApp::DRect &rect)
{
    size = rect.size;
    p_TableView = CATableView::createWithLayout(DLayoutFill);
    p_TableView->setAllowsSelection(true);
    p_TableView->setTableViewDelegate(this);
    p_TableView->setTableViewDataSource(this);
    p_TableView->setShowsScrollIndicators(false);
    p_TableView->setSeparatorViewHeight(0);
    p_TableView->setScrollEnabled(false);
    p_TableView->setSeparatorColor(CAColor_clear);
    this->addSubview(p_TableView);
    return true;
}

ETableView* ETableView::createWithLayout(const CrossApp::DLayout &layout)
{
    ETableView* tableView = new ETableView();
    if (tableView && tableView->initWithLayout(layout))
    {
        tableView->autorelease();
        return tableView;
    }
    CC_SAFE_DELETE(tableView);
    return NULL;
}

void ETableView::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    m_pETableViewDelegate->etableViewDidSelectRowAtIndexPath(table, section, row);
}

void ETableView::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* ETableView::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    ETableViewCell* cell = dynamic_cast<ETableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = ETableViewCell::create("CrossApp");
        cell->setLayout(DLayoutFill);
        cell->initWithCell();
    }
    char order[20] = "";
    sprintf(order, "%s", sectionTitle.at(row).c_str());
    CALabel* cellText = (CALabel*)cell->getSubviewByTag(100);
    cellText->setText(order);
    
    return cell;
    
}

unsigned int ETableView::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return sectionTitle.size();
}

unsigned int ETableView::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int ETableView::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return size.height/sectionTitle.size();
}

unsigned int ETableView::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    return 0;
}

unsigned int ETableView::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
    return 0;
}



TableViewTest::TableViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

TableViewTest::~TableViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void TableViewTest::viewDidLoad()
{
    
    sectionTitle.clear();
    sectionTitle.push_back("A");
    sectionTitle.push_back("B");
    sectionTitle.push_back("C");
    sectionTitle.push_back("D");
    sectionTitle.push_back("E");
    sectionTitle.push_back("F");
    sectionTitle.push_back("G");
    sectionTitle.push_back("H");
    sectionTitle.push_back("I");
    sectionTitle.push_back("J");
    sectionTitle.push_back("K");
    sectionTitle.push_back("L");
    sectionTitle.push_back("M");
    sectionTitle.push_back("N");
    sectionTitle.push_back("O");
    sectionTitle.push_back("P");
    sectionTitle.push_back("Q");
    sectionTitle.push_back("R");
    sectionTitle.push_back("S");
    sectionTitle.push_back("T");
    sectionTitle.push_back("U");
    sectionTitle.push_back("V");
    sectionTitle.push_back("W");
    sectionTitle.push_back("X");
    sectionTitle.push_back("Y");
    sectionTitle.push_back("Z");
    
    showIndex = 0;
    VIEWLIST.clear();
    this->getView()->setColor(CAColor_gray);

//    p_TableView = CATableView::createWithLayout(DRectLayout(0,50,0,0,DRectLayout::L_R_T_B));
    p_TableView = CATableView::createWithLayout(DLayout(DHorizontalLayout_L_R(0, 50), DVerticalLayoutFill));
    p_TableView->setTableViewDataSource(this);
    p_TableView->setTableViewDelegate(this);
    p_TableView->setAllowsSelection(true);
    p_TableView->setAllowsMultipleSelection(false);
    p_TableView->setShowsScrollIndicators(false);
    p_TableView->setSelectRowAtIndexPath(2, 1);
    this->getView()->addSubview(p_TableView);
    
    ETableView* p_TableView1 = ETableView::createWithLayout(DLayout(DHorizontalLayout_R_W(0, 50), DVerticalLayoutFill));
    p_TableView1->init(this->getView()->getBounds());
    p_TableView1->setETableViewDelegate(this);
    this->getView()->addSubview(p_TableView1);
}

void TableViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void TableViewTest::etableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    //CCLog("ssss====%d",row);
    p_TableView->setContentOffset(DPoint(0,313*row), false);
}

void TableViewTest::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

void TableViewTest::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* TableViewTest::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    CATableViewCell* cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CATableViewCell::create("CrossApp");
        CAImageView* image = CAImageView::createWithLayout(DLayoutFill);
        image->setImage(CAImage::create("source_material/second_2.png"));
        cell->addSubview(image);
        
//        CALabel* cellText = CALabel::createWithLayout(DRectLayout(150,10,10,10,DRectLayout::L_R_T_B));
        CALabel* cellText = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(150, 10), DVerticalLayout_T_B(10, 10)));
        cellText->setTag(100);
        cellText->setFontSize(30);
        cellText->setTextAlignment(CATextAlignmentLeft);
        cellText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        cell->addSubview(cellText);
    }
    char order[20] = "";
    sprintf(order, "%s-%d", sectionTitle.at(section).c_str(),row);
    CALabel* cellText = (CALabel*)cell->getSubviewByTag(100);
    cellText->setText(order);
    return cell;
    
}

CAView* TableViewTest::tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section)
{
    std::string head = sectionTitle.at(section);
    CAView* view = CAView::createWithColor(CAColor_gray);
    
    DSize _size = viewSize;
//    CALabel* header = CALabel::createWithLayout(DRectLayout(50,0,0,0,DRectLayout::L_R_T_B));
    CALabel* header = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 0), DVerticalLayoutFill));
    header->setText(head);
    header->setFontSize(30);
    header->setColor(CAColor_white);
    header->setTextAlignment(CATextAlignmentLeft);
    header->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    view->addSubview(header);
    
    return view;
}

CAView* TableViewTest::tableViewSectionViewForFooterInSection(CATableView* table, const DSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(CAColor_gray);
    return view;
}

unsigned int TableViewTest::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return 2;
}

unsigned int TableViewTest::numberOfSections(CATableView *table)
{
    return sectionTitle.size();
}

unsigned int TableViewTest::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return 130;
}

unsigned int TableViewTest::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    return 50;
}

unsigned int TableViewTest::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
    return 1;
}

