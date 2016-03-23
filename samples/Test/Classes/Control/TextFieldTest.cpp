
#include "TextFieldTest.h"

TextFieldTest::TextFieldTest():pageViewIndex(0)
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

TextFieldTest::~TextFieldTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void TextFieldTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    showNum = 2;
    pageViewIndex = 1;
    showIndex = 0;
    VIEWLIST.clear();
    
    CATextField* textField1 = CATextField::createWithLayout(DRectLayout(100,100,300,100,DRectLayout::L_R_T_H));
    textField1->setTag(100);
    //PlaceHolder文本内容
    textField1->setPlaceHolderText("Input");
    //键盘类型
    textField1->setKeyboardType(CATextField::Default);
    //TextField的对齐方式
    textField1->setTextFieldAlign(CATextField::Left);

    CAView* view1 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view1->addSubview(textField1);
    view1->setColor(CAColor_gray);
    VIEWLIST.pushBack(view1);
    
    CATextField* textField2 = CATextField::createWithLayout(DRectLayout(100,100,300,100,DRectLayout::L_R_T_H));
    textField2->setTag(101);
    textField2->setPlaceHolderText("Input");
    textField2->setKeyboardType(CATextField::Default);
    textField2->setMarginImageLeft(DSize(60,60),"source_material/search.png");
    textField2->setTextFieldAlign(CATextField::Left);
    textField2->setSecureTextEntry(true);
    
    CAView* view2 = CAView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B));
    view2->addSubview(textField2);
    view2->setColor(CAColor_gray);
    VIEWLIST.pushBack(view2);
    
    
    
    showUI();
    p_PageViewVec->setViews(VIEWLIST);
}

void TextFieldTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void TextFieldTest::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(TextFieldTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(TextFieldTest::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);
}

void TextFieldTest::buttonControlCallBack(CAControl* btn, DPoint point)
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
    
//    if (showIndex==0) {
//        this->setNavigationBarItem(CANavigationBarItem::create("Button Noborder"));
//    }else if(showIndex==1){
//        this->setNavigationBarItem(CANavigationBarItem::create("Button SquareRect"));
//    }else if(showIndex==2){
//        this->setNavigationBarItem(CANavigationBarItem::create("Button RoundedRect"));
//    }else if(showIndex==3){
//        this->setNavigationBarItem(CANavigationBarItem::create("Button Custom"));
//    }else if(showIndex==4){
//        this->setNavigationBarItem(CANavigationBarItem::create("Button AllowsSelected"));
//    }
}

void TextFieldTest::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void TextFieldTest::pageViewDidEndTurning(CAPageView* pageView)
{
    
}

void TextFieldTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    
}
