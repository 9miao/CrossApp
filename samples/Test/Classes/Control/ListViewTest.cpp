
#include "ListViewTest.h"

ListViewTest::ListViewTest()
{
    this->setTitle("CAListView");
}

ListViewTest::~ListViewTest()
{
}

void ListViewTest::viewDidLoad()
{
    m_pListViewHor = CAListView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_H(0, 88)));
    m_pListViewHor->setOrientation(CAListView::Orientation::Horizontal);
    m_pListViewHor->setAllowsSelection(true);
    m_pListViewHor->setReachBoundaryHandOverToSuperview(false);
    this->getView()->addSubview(m_pListViewHor);
    
    std::vector<std::string> vec1 =
    {
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
    };
    
    m_pListViewHor->onNumberOfRowsAtIndexPath([=]()
    {
        return vec1.size();
    });
    
    m_pListViewHor->onCellHeightAtIndexPath([=](unsigned int index)
    {
        return 200;
    });
    
    m_pListViewHor->onCellAtIndexPath([=](const DSize& cellSize, unsigned int index)
    {
        CAListViewCell* cell = (CAListViewCell*)m_pListViewHor->dequeueReusableCellWithIdentifier("ListViewCell");
        if (!cell)
        {
            cell = CAListViewCell::create("ListViewCell");
            CALabel* test = CALabel::createWithLayout(DLayoutFill);
            test->setColor(CAColor4B(51, 204, 255, 255));
            test->setTextAlignment(CATextAlignment::Center);
            test->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
            test->setFontSize(28);
            test->setTag(100);
            cell->addSubview(test);
        }
        
        CALabel* test = (CALabel*)cell->getSubviewByTag(100);
        test->setText(vec1.at(index));
        
        return cell;

    });
    
    m_pListViewHor->onDidSelectCellAtIndexPath([=](unsigned int index)
    {
        CCLog("DidSelectCell %s", vec1.at(index).c_str());
    });
    
    m_pListViewHor->onDidDeselectCellAtIndexPath([=](unsigned int index)
    {
        CCLog("DidDeselectCell %s", vec1.at(index).c_str());
    });

    this->getView()->addSubview(CAView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_H(88, 5)), CAColor4B::GREEN));

    m_pListViewVer = CAListView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_B(93, 0)));
    m_pListViewVer->setAllowsSelection(true);
    m_pListViewVer->setAllowsMultipleSelection(true);
    m_pListViewVer->setOrientation(CAListView::Orientation::Vertical);
    m_pListViewVer->setSeparatorColor(CAColor4B::GRAY);
    this->getView()->addSubview(m_pListViewVer);
    
    m_pListViewVer->onNumberOfRowsAtIndexPath(CALLBACK_BIND_0(ListViewTest::numberOfIndex, this));
    m_pListViewVer->onCellHeightAtIndexPath(CALLBACK_BIND_1(ListViewTest::heightForIndex, this));
    m_pListViewVer->onCellAtIndexPath(CALLBACK_BIND_2(ListViewTest::cellAtIndex, this));
    m_pListViewVer->onDidSelectCellAtIndexPath(CALLBACK_BIND_1(ListViewTest::didSelectCellAtIndex, this));
    m_pListViewVer->onDidDeselectCellAtIndexPath(CALLBACK_BIND_1(ListViewTest::didDeselectCellAtIndex, this));
    
}
void ListViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

unsigned int ListViewTest::numberOfIndex()
{
    return 30;
}

unsigned int ListViewTest::heightForIndex(unsigned int index)
{
    return 100;
}

CAListViewCell* ListViewTest::cellAtIndex(const DSize& cellSize, unsigned int index)
{
    CAListViewCell* cell = (CAListViewCell*)m_pListViewVer->dequeueReusableCellWithIdentifier("ListViewCell");
    if (cell==NULL)
    {
        cell = CAListViewCell::create("ListViewCell");
        
        CALabel* test = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_W(0, 200), DVerticalLayoutFill));
        test->setColor(ccc4(51, 204, 255, 255));
        test->setTextAlignment(CATextAlignment::Center);
        test->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        test->setFontSize(28);
        test->setTag(100);
        cell->addSubview(test);
        
        CAButton* btn = CAButton::createWithLayout(DLayout(DHorizontalLayout_W_C(100, 0.85), DVerticalLayout_H_C(50, 0.5)), CAButton::Type::SquareRect);
        btn->setTitleForState(CAControl::State::Normal, "btn");
        btn->setTag(200);
        cell->addSubview(btn);
        //btn->setTouchEventScrollHandOverToSuperview(false);
    }
    char temptext[10];
    sprintf(temptext, "cell-%d",index);
    CALabel* test = (CALabel*)cell->getSubviewByTag(100);
    test->setText(temptext);
    
    return cell;
}

void ListViewTest::didSelectCellAtIndex(unsigned int index)
{
    m_sDidSelectCellAtIndexs.insert(index);
    
    std::string text;
    for (auto& i : m_sDidSelectCellAtIndexs)
    {
        text += crossapp_format_string("%u >", i);
    }
    CCLog("%s", text.c_str());
}

void ListViewTest::didDeselectCellAtIndex(unsigned int index)
{
    m_sDidSelectCellAtIndexs.erase(index);
    std::string text;
    for (auto& i : m_sDidSelectCellAtIndexs)
    {
        text += crossapp_format_string("%u >", i);
    }
    CCLog("%s", text.c_str());
}
