
#include "PickerViewTest.h"

#define _T(x) L##x


PickerViewTest::PickerViewTest()
:m_iIndex(0)
{
    this->setTitle("CAPickerView");
    
    m_vCitys =
    {
        _T("北京市"),
        _T("天津市"),
        _T("上海市"),
        _T("重庆市"),
        _T("河北省"),
        _T("山西省"),
        _T("辽宁省"),
        _T("吉林省"),
        _T("黑龙江省"),
        _T("江苏省"),
        _T("浙江省"),
        _T("安徽省"),
        _T("福建省"),
        _T("江西省"),
        _T("山东省"),
        _T("河南省"),
        _T("湖北省"),
        _T("湖南省"),
        _T("广东省"),
        _T("海南省"),
        _T("四川省"),
        _T("贵州省"),
        _T("云南省"),
        _T("陕西省"),
        _T("甘肃省"),
        _T("青海省"),
        _T("台湾省"),
        _T("广西壮族自治区"),
        _T("内蒙古自治区"),
        _T("西藏自治区"),
        _T("宁夏回族自治区"),
        _T("新疆维吾尔自治区"),
        _T("香港特别行政区"),
        _T("澳门特别行政区"),
    };
}

PickerViewTest::~PickerViewTest()
{
}



void PickerViewTest::viewDidLoad()
{
    if (m_iIndex == 0)
    {
        auto label = CALabel::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_H(100, 40)));
        label->setText(UTF8("北京市"));
        label->setColor(CAColor4B::BLACK);
        label->setFontSize(28);
        label->setTextAlignment(CATextAlignment::Center);
        label->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        this->getView()->addSubview(label);
        
        auto pickerView = CAPickerView::createWithLayout(DLayout(DHorizontalLayout_NW_C(0.8, 0.5), DVerticalLayout_T_H(200, 400)));
        pickerView->setFontSizeNormal(40);
        pickerView->setFontSizeSelected(40);
        this->getView()->addSubview(pickerView);
        pickerView->setBackgroundImage(CAImage::createWithColor4B(CAColor4B::YELLOW));
        
        pickerView->onTitleForRow([=](unsigned int row, unsigned int component){

            return unicode_to_utf8(m_vCitys.at(row));
        });
        
        pickerView->onHeightForComponent([=](unsigned int component){
            
            return 80;
        });
        
        pickerView->onWidthForComponent([=](unsigned int component){
            
            return this->getView()->getBounds().size.width * 0.8f;
        });
        
        pickerView->onNumberOfRowsInComponent([=](unsigned int component)
        {
            return m_vCitys.size();
        });
    
        pickerView->onViewForSelected([=](unsigned int component, DSize size){
            
            return CAView::createWithColor(CAColor4B::MAGENTA);
        });
        
        pickerView->onDidSelectRow([=](unsigned int row, unsigned int component){
            
            label->setText(unicode_to_utf8(m_vCitys[row]));
        });
       
    }
    
    else if(m_iIndex == 1)
    {
        CADatePickerView* dpv = CADatePickerView::createWithLayout(DLayout(DHorizontalLayout_NW_C(0.8, 0.5), DVerticalLayout_T_H(200, 400)),CADatePickerView::Mode::Date);
        dpv->setBackgroundImage(CAImage::createWithColor4B(CAColor4B::YELLOW));
        dpv->setBackgroundImageForSelected(CAImage::createWithColor4B(CAColor4B::GREEN));
        this->getView()->addSubview(dpv);
        
        //dpv->setDate(2004, 2, 16, false);
        dpv->onSelectRow(CALLBACK_BIND_1(PickerViewTest::dataPickerViewdidSelectRow, this));
        
    }
    else if(m_iIndex == 2)
    {
        CADatePickerView* dpv3 = CADatePickerView::createWithLayout(DLayout(DHorizontalLayout_NW_C(0.8, 0.5), DVerticalLayout_T_H(200, 400)),CADatePickerView::Mode::DateAndTime);
        dpv3->setBackgroundImage(CAImage::createWithColor4B(CAColor4B::YELLOW));
        dpv3->setBackgroundImageForSelected(CAImage::createWithColor4B(CAColor4B::GREEN));
        this->getView()->addSubview(dpv3);
        
        dpv3->onSelectRow(CALLBACK_BIND_1(PickerViewTest::dataPickerViewdidSelectRow, this));
    
    }
    else if(m_iIndex == 3)
    {
        CADatePickerView* dpv4 = CADatePickerView::createWithLayout(DLayout(DHorizontalLayout_NW_C(0.8, 0.5), DVerticalLayout_T_H(200, 400)),CADatePickerView::Mode::Time);
        dpv4->setBackgroundImage(CAImage::createWithColor4B(CAColor4B::YELLOW));
        dpv4->setBackgroundImageForSelected(CAImage::createWithColor4B(CAColor4B::GREEN));
        this->getView()->addSubview(dpv4);
        
        dpv4->onSelectRow(CALLBACK_BIND_1(PickerViewTest::dataPickerViewdidSelectRow, this));
    }
    else
    {
        CADatePickerView* dpv5 = CADatePickerView::createWithLayout(DLayout(DHorizontalLayout_NW_C(0.8, 0.5), DVerticalLayout_T_H(200, 400)),CADatePickerView::Mode::CountDownTimer);
        dpv5->setBackgroundImage(CAImage::createWithColor4B(CAColor4B::YELLOW));
        dpv5->setBackgroundImageForSelected(CAImage::createWithColor4B(CAColor4B::GREEN));
        this->getView()->addSubview(dpv5);
        
        dpv5->onSelectRow(CALLBACK_BIND_1(PickerViewTest::dataPickerViewdidSelectRow, this));
    
    }
    
    auto btn = CAButton::createWithLayout(DLayout(DHorizontalLayout_NW_C(0.3,0.5), DVerticalLayout_B_H(20 ,50)), CAButton::Type::RoundedRect);
    btn->setTitleForState(CAControl::State::Normal, "Switch Next");
    btn->addTarget([&] {
        
        ssize_t index = m_iIndex + 1;
        if (index > 4)
        {
            index = 0;
        }
        PickerViewTest* viewController = PickerViewTest::create();
        viewController->setIndex(index);
        RootWindow::getInstance()->getRootNavigationController()->replaceViewController(viewController, false);
        
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(btn);
}

void PickerViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void PickerViewTest::dataPickerViewdidSelectRow(const tm& tm)
{
    CCLog("didSelectRow tm: %d-%d-%d",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday);
}






