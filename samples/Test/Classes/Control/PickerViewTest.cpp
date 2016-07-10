
#include "PickerViewTest.h"
//#include "support/device/CADevice.h"
#include "CDUIShowAutoCollectionView.h"

int PickerViewNum = 0;

PickerViewTest::PickerViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

PickerViewTest::~PickerViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
    PickerViewNum = 0;
}

static const CHAR* adressTag[34] =
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

void CDUIShowAutoCollectionView::PickerViewRightBtnRightcallback(CAControl* control, DPoint point)
{
    if (showPickerViewNavigationBar >= 4)
    {
        showPickerViewNavigationBar = 0;
        PickerViewNum = showPickerViewNavigationBar;
    }
    else
    {
        PickerViewNum = ++showPickerViewNavigationBar;
    }
    PickerViewTest* ViewContrllerPickerViewTest = new PickerViewTest();
    ViewContrllerPickerViewTest->init();
    ViewContrllerPickerViewTest->setNavigationBarItem(PickerViewNavigationBar);
    ViewContrllerPickerViewTest->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->replaceViewController(ViewContrllerPickerViewTest, false);
}


void PickerViewTest::viewDidLoad()
{
    if (PickerViewNum == 0)
    {
        p_pickerView = CAPickerView::createWithLayout(DLayout(DHorizontalLayout_L_R(10, 10), DVerticalLayout_T_H(200, 400)));
        p_pickerView->setPickerViewDelegate(this);
        p_pickerView->setPickerViewDataSource(this);
        p_pickerView->setFontSizeNormal(40);
        p_pickerView->setFontSizeSelected(40);
        p_pickerView->reloadAllComponents();
        
        city_value = CALabel::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_H(100, 40)));
        city_value->setText(UTF8("天津市"));
        city_value->setColor(CAColor_black);
        city_value->setFontSize(28);
        city_value->setTextAlignment(CATextAlignmentCenter);
        city_value->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    
        CAView* view1 = CAView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayoutFill));
        view1->addSubview(p_pickerView);
        view1->addSubview(city_value);
        view1->setColor(CAColor_white);
        this->getView()->addSubview(view1);
    }
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    else if(PickerViewNum == 1)
    {
        CADatePickerView* dpv = CADatePickerView::createWithLayout(DLayout(DHorizontalLayout_L_R(10, 10), DVerticalLayout_T_H(200, 400)),CADatePickerModeDate);
        //dpv->setDate(2004, 2, 16, false);
        dpv->setDelegate(this);
        
        CAView* view2 = CAView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayoutFill));
        view2->addSubview(dpv);
        view2->setColor(CAColor_white);
        this->getView()->addSubview(view2);
    }
    else if(PickerViewNum == 2)
    {
        CADatePickerView* dpv3 = CADatePickerView::createWithLayout(DLayout(DHorizontalLayout_L_R(10, 10), DVerticalLayout_T_H(200, 400)),CADatePickerModeDateAndTime);
        dpv3->setDelegate(this);
    
        CAView* view3 = CAView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayoutFill));
        view3->addSubview(dpv3);
        view3->setColor(CAColor_white);
        this->getView()->addSubview(view3);
    }
    else if(PickerViewNum == 3)
    {
        CADatePickerView* dpv4 = CADatePickerView::createWithLayout(DLayout(DHorizontalLayout_L_R(10, 10), DVerticalLayout_T_H(200, 400)),CADatePickerModeTime);
        dpv4->setDelegate(this);
    
        CAView* view4 = CAView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayoutFill));
        view4->addSubview(dpv4);
        view4->setColor(CAColor_white);
        this->getView()->addSubview(view4);
    }
    else
    {
        CADatePickerView* dpv5 = CADatePickerView::createWithLayout(DLayout(DHorizontalLayout_L_R(10, 10), DVerticalLayout_T_H(200, 400)),CADatePickerModeCountDownTimer);
        dpv5->setDelegate(this);
    
        CAView* view5 = CAView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayoutFill));
        view5->addSubview(dpv5);
        view5->setColor(CAColor_white);
        this->getView()->addSubview(view5);
    }
#endif
    
}

void PickerViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void PickerViewTest::didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
    char tem[100];
    sprintf(tem, "%s", unicode_to_utf8(adressTag[row]).c_str());
    city_value->setText(tem);
}

void PickerViewTest::didSelectRow(const struct tm& tm)
{
    char temp_time[20];
    strftime(temp_time, 100, "%F", &tm);
    CCLog("didSelectRow----tm======%d-%d-%d",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday);
    CCLog("didSelectRow----tm======%s",temp_time);
}

unsigned int PickerViewTest::numberOfComponentsInPickerView(CAPickerView* pickerView)
{
    return 1;
}

unsigned int PickerViewTest::numberOfRowsInComponent(CAPickerView* pickerView, unsigned int component)
{
    return 34;
}

float PickerViewTest::widthForComponent(CAPickerView* pickerView, unsigned int component)
{
    return p_pickerView->getFrame().size.width/numberOfComponentsInPickerView(pickerView);
}

float PickerViewTest::rowHeightForComponent(CAPickerView* pickerView, unsigned int component)
{
    static float rowHeight = 80;
    return rowHeight;
}

const char* PickerViewTest::titleForRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
    static std::string s;
    s = unicode_to_utf8(adressTag[row]);
    return s.c_str();
}



