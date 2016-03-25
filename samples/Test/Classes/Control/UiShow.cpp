//
//  shouUiView.cpp
//  Test
//
//  Created by Jerson on 16/3/24.
//
//

#include "UiShow.h"

UiShow::UiShow():pageViewIndex(0)
{
//    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
//    drawer->setTouchMoved(false);
}

UiShow::~UiShow()
{
//    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
//    drawer->setTouchMoved(true);
}

void UiShow::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
}

void UiShow::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void UiShow::showUI()
{
    CAButton* btn_Left = CAButton::create(CAButtonTypeSquareRect);
    btn_Left->setTag(1);
    btn_Left->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::L_W_B_H));
    btn_Left->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_white.png")));
    btn_Left->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_left_blue.png")));
    btn_Left->addTarget(this, CAControl_selector(UiShow::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Left, 10);
    
    CAButton* btn_Right = CAButton::create(CAButtonTypeSquareRect);
    btn_Right->setTag(2);
    btn_Right->setLayout(DRectLayout(200, 64, 20, 64, DRectLayout::R_W_B_H));
    btn_Right->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_white.png")));
    btn_Right->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_right_blue.png")));
    btn_Right->addTarget(this, CAControl_selector(UiShow::buttonControlCallBack), CAControlEventTouchDown);
    this->getView()->insertSubview(btn_Right, 10);
    
    p_PageViewVec = CAPageView::createWithLayout(DRectLayout(0, 0, 0, 100, DRectLayout::L_R_T_B), CAPageViewDirectionHorizontal);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setBackgroundColor(CAColor_gray);
    p_PageViewVec->setPageViewDelegate(this);
    p_PageViewVec->setScrollEnabled(false);
    
    this->getView()->addSubview(p_PageViewVec);
}

void UiShow::buttonControlCallBack(CAControl* btn, DPoint point)
{
    point = btn->convertToWorldSpace(point);
    CAButton* button = (CAButton*)btn;
    CCLog("btn_tag===%d",button->getTag());
    int temIndex = button->getTag();
    if (temIndex==1)
    {
        if (showIndex>0)
        {
            showIndex--;
            p_PageViewVec->setCurrPage(showIndex, true); //123456
        }
        else if(showIndex==0)
        {
            showIndex=showNum-1;
            p_PageViewVec->setCurrPage(showIndex, false);
        }
    }
    else if(temIndex==2)
    {
        if (showIndex<showNum)
        {
            showIndex++;
            if (showIndex==showNum)
            {
                showIndex=0;
                p_PageViewVec->setCurrPage(showIndex, false);
            }
            else
            {
                p_PageViewVec->setCurrPage(showIndex, true);
            }
        }
    }
}

void UiShow::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void UiShow::pageViewDidEndTurning(CAPageView* pageView)
{
    
}

void UiShow::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    
}