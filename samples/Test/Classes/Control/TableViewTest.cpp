
#include "TableViewTest.h"

USING_NS_CC;

TableViewTest::TableViewTest()
{
    this->setTitle("CATableView");
}

TableViewTest::~TableViewTest()
{
}

void TableViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor4B::GRAY);
    
    m_pSectionTitle =
    {
        "A","B","C","D","E","F","G",
        "H","I","J","K","L","M","N",
        "O","P","Q",
        "R","S","T",
        "U","V","W",
        "X","Y","Z",
    };

    m_pTableView = CATableView::createWithLayout(DLayoutFill);
    m_pTableView->setAllowsSelection(true);
    m_pTableView->setAllowsMultipleSelection(false);
    m_pTableView->setShowsScrollIndicators(false);
    m_pTableView->setSelectRowAtIndexPath(2, 1);
    this->getView()->addSubview(m_pTableView);
    
    m_pTableView->onCellAtIndexPath(CALLBACK_BIND_3(TableViewTest::tableCellAtIndex, this));
    m_pTableView->onCellHeightAtIndexPath(CALLBACK_BIND_2(TableViewTest::tableViewHeightForRowAtIndexPath, this));
    m_pTableView->onNumberOfRowsAtIndexPath(CALLBACK_BIND_1(TableViewTest::numberOfRowsInSection, this));
    m_pTableView->onNumberOfSections(CALLBACK_BIND_0(TableViewTest::numberOfSections, this));
    m_pTableView->onSectionViewForHeaderInSection(CALLBACK_BIND_2(TableViewTest::tableViewSectionViewForHeaderInSection, this));
    m_pTableView->onHeightForHeaderInSection(CALLBACK_BIND_1(TableViewTest::tableViewHeightForHeaderInSection, this));
    m_pTableView->onSectionViewForFooterInSection(CALLBACK_BIND_2(TableViewTest::tableViewSectionViewForFooterInSection, this));
    m_pTableView->onHeightForFooterInSection(CALLBACK_BIND_1(TableViewTest::tableViewHeightForFooterInSection, this));
    m_pTableView->onDidSelectCellAtIndexPath(CALLBACK_BIND_2(TableViewTest::tableViewDidSelectRowAtIndexPath, this));
    m_pTableView->onDidDeselectCellAtIndexPath(CALLBACK_BIND_2(TableViewTest::tableViewDidDeselectRowAtIndexPath, this));
    
}

void TableViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void TableViewTest::tableViewDidSelectRowAtIndexPath(unsigned int section, unsigned int row) //1
{

}


void TableViewTest::tableViewDidDeselectRowAtIndexPath(unsigned int section, unsigned int row)
{
    
}

CATableViewCell* TableViewTest::tableCellAtIndex(DSize cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    CATableViewCell* cell = dynamic_cast<CATableViewCell*>(m_pTableView->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CATableViewCell::create("CrossApp");
        CAImageView* image = CAImageView::createWithLayout(DLayoutFill);
        image->setImage(CAImage::create("source_material/second_2.png"));
        cell->getContentView()->addSubview(image);
        
        CALabel* cellText = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(150, 10), DVerticalLayout_T_B(10, 10)));
        cellText->setTag(100);
        cellText->setFontSize(30);
        cellText->setTextAlignment(CATextAlignment::Left);
        cellText->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        cell->getContentView()->addSubview(cellText);
        
        CAButton* deleteBtn = CAButton::createWithLayout(DLayout(DHorizontalLayout_R_W(0, 180), DVerticalLayoutFill), CAButton::Type::Custom);
        deleteBtn->setBackgroundImageForState(CAControl::State::Normal, CAImage::createWithColor4B(CAColor4B::RED), false);
        deleteBtn->setBackgroundImageForState(CAControl::State::Highlighted, CAImage::createWithColor4B(CAColor4B::ORANGE), false);
        deleteBtn->setTitleForState(CAControl::State::Normal, "Delete");
        deleteBtn->setTitleColorForState(CAControl::State::Normal, CAColor4B::WHITE);
        deleteBtn->setTitleColorForState(CAControl::State::Highlighted, CAColor4B::WHITE);
        deleteBtn->setTitleFontSize(32);
        deleteBtn->setTag(101);
        cell->insertSubview(deleteBtn, -1);
    }
    
    cell->setDraggingLength(180);
    
    char order[20] = "";
    sprintf(order, "%s-%d", m_pSectionTitle.at(section).c_str(),row);
    CALabel* cellText = (CALabel*)cell->getContentView()->getSubviewByTag(100);
    cellText->setText(order);
    
    CAButton* deleteBtn = (CAButton*)cell->getSubviewByTag(101);
    deleteBtn->addTarget([=]()
    {
        CCLog("button at (section: %d, row: %d)", section, row);
        
    }, CAButton::Event::TouchUpInSide);
    
    return cell;
    
}

unsigned int TableViewTest::tableViewHeightForRowAtIndexPath(unsigned int section, unsigned int row)
{
    return 130;
}


unsigned int TableViewTest::numberOfRowsInSection(unsigned int section)
{
    return 2;
}

unsigned int TableViewTest::numberOfSections()
{
    return (unsigned int)m_pSectionTitle.size();
}

CAView* TableViewTest::tableViewSectionViewForHeaderInSection(DSize viewSize, unsigned int section)
{
    std::string head = m_pSectionTitle.at(section);
    CAView* view = CAView::createWithColor(CAColor4B::GRAY);
    
    DSize _size = viewSize;
    CALabel* header = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(50, 0), DVerticalLayoutFill));
    header->setText(head);
    header->setFontSize(30);
    header->setColor(CAColor4B::WHITE);
    header->setTextAlignment(CATextAlignment::Left);
    header->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
    view->addSubview(header);
    
    return view;
}

unsigned int TableViewTest::tableViewHeightForHeaderInSection(unsigned int section)
{
    return 50;
}

CAView* TableViewTest::tableViewSectionViewForFooterInSection(DSize viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(CAColor4B::GRAY);
    return view;
}

unsigned int TableViewTest::tableViewHeightForFooterInSection(unsigned int section)
{
    return 1;
}

