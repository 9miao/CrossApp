
#include "PickerViewTest.h"
//#include "support/device/CADevice.h"


PickerViewTest::PickerViewTest():pageViewIndex(0)
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

PickerViewTest::~PickerViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
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

void PickerViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
    p_pickerView = CAPickerView::createWithLayout(DLayout(10, 10, 100, 400, DLayout::L_R_T_H));
    p_pickerView->setPickerViewDelegate(this);
    p_pickerView->setPickerViewDataSource(this);
    p_pickerView->setFontSizeNormal(40);
    p_pickerView->setFontSizeSelected(40);
    
    city_value = CALabel::createWithLayout(DLayout(0, winSize.width, 100, 40, DLayout::L_W_T_H));
    city_value->setText(UTF8("天津市"));
    city_value->setColor(CAColor_black);
    city_value->setFontSize(28);
    city_value->setTextAlignment(CATextAlignmentCenter);
    city_value->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    
    CAView* view1 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view1->addSubview(p_pickerView);
    view1->addSubview(city_value);
    view1->setColor(CAColor_white);
    VIEWLIST.pushBack(view1);

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    
    CADatePickerView* dpv = CADatePickerView::createWithLayout(DLayout(10, 10, 100, 400, DLayout::L_R_T_H),CADatePickerModeDate);
    //dpv->setDate(2004, 2, 16, false);
    dpv->setDelegate(this);
    
    CAView* view2 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view2->addSubview(dpv);
    view2->setColor(CAColor_white);
    VIEWLIST.pushBack(view2);
    
    CADatePickerView* dpv3 = CADatePickerView::createWithLayout(DLayout(10, 10, 100, 400, DLayout::L_R_T_H),CADatePickerModeDateAndTime);
    dpv3->setDelegate(this);
    
    CAView* view3 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view3->addSubview(dpv3);
    view3->setColor(CAColor_white);
    VIEWLIST.pushBack(view3);
    
    CADatePickerView* dpv4 = CADatePickerView::createWithLayout(DLayout(10, 10, 100, 400, DLayout::L_R_T_H),CADatePickerModeTime);
    dpv4->setDelegate(this);
    
    CAView* view4 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view4->addSubview(dpv4);
    view4->setColor(CAColor_white);
    VIEWLIST.pushBack(view4);
    
    CADatePickerView* dpv5 = CADatePickerView::createWithLayout(DLayout(10, 10, 100, 400, DLayout::L_R_T_H),CADatePickerModeCountDownTimer);
    dpv5->setDelegate(this);
    
    CAView* view5 = CAView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B));
    view5->addSubview(dpv5);
    view5->setColor(CAColor_white);
    VIEWLIST.pushBack(view5);
#endif
    
    showNum = VIEWLIST.size();
    showUI();
    p_PageViewVec->setViews(VIEWLIST);
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
    CCLog("return = %f", p_pickerView->getLayout().width);
    return p_pickerView->getFrame().size.width/numberOfComponentsInPickerView(pickerView);
    //return p_pickerView->getLayout().width/numberOfComponentsInPickerView(pickerView);
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

void PickerViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void PickerViewTest::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DLayout(200, 64, 20, 64, DLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(PickerViewTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DLayout(200, 64, 20, 64, DLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(PickerViewTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DLayout(0, 0, 0, 100, DLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);
}

void PickerViewTest::buttonControlCallBack(CAControl* btn, DPoint point)
{
    point = btn->convertToWorldSpace(point);
    CAButton* button = (CAButton*)btn;
    CCLog("btn_tag===%d",button->getTag());
    int temIndex = button->getTag();
    if (temIndex==1) {
        if (showIndex>0) {
            showIndex--;
            p_PageViewVec->setCurrPage(showIndex, true); //123456
        }else if(showIndex==0){
            showIndex=showNum-1;
            p_PageViewVec->setCurrPage(showIndex, false);
        }
    }else if(temIndex==2){
        if (showIndex<showNum) {
            showIndex++;
            if (showIndex==showNum) {
                showIndex=0;
                p_PageViewVec->setCurrPage(showIndex, false);
            }else{
                p_PageViewVec->setCurrPage(showIndex, true);
            }
        }
    }
}

void PickerViewTest::pageViewDidBeginTurning(CAPageView* pageView)
{

}

void PickerViewTest::pageViewDidEndTurning(CAPageView* pageView)
{

}

void PickerViewTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{

}
