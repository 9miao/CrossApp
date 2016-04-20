//
//  CDShowNewsImage.cpp
//  Test
//
//  Created by renhongguang on 15/5/6.
//
//

#include "CDShowNewsImage.h"

CDShowNewsImage::CDShowNewsImage()
:m_isShow(true)
{
    
}

CDShowNewsImage::~CDShowNewsImage()
{
    
}

void CDShowNewsImage::viewDidLoad()
{
    winSize = this->getView()->getBounds().size;
    this->getView()->setColor(CAColor_black);
}

void CDShowNewsImage::viewDidUnload()
{
    
}

void CDShowNewsImage::initNewsImageView(newsImage _image)
{
    m_image = _image;
    
    CAVector<CAView* > viewList;
    for (int i=0; i<m_image.m_imageUrl.size(); i++)
    {
        CommonUrlImageView* temImage = CommonUrlImageView::createWithLayout(DLayoutFill);
        temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        temImage->setImage(CAImage::create("HelloWorld.png"));
        temImage->setUrl(m_image.m_imageUrl[i]);
        
        CAScrollView* p_ScrollView = CAScrollView::createWithLayout(DLayoutFill);
        p_ScrollView->setMinimumZoomScale(1.0f);
        p_ScrollView->setMaximumZoomScale(2.5f);
        p_ScrollView->setMultitouchGesture(CAScrollView::MultitouchGesture::Zoom);
        p_ScrollView->setBackgroundColor(CAColor_clear);
        p_ScrollView->setShowsScrollIndicators(false);
        p_ScrollView->setBounceVertical(false);
        p_ScrollView->setScrollViewDelegate(this);
        p_ScrollView->addSubview(temImage);
        viewList.pushBack(p_ScrollView);
    }
    
    p_PageView = CAPageView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_B(30, 0)), CAPageView::Horizontal);
    p_PageView->setViews(viewList);
    p_PageView->setPageViewDelegate(this);
    p_PageView->setBackgroundColor(CAColor_black);
    p_PageView->setPageViewDelegate(this);
    p_PageView->setSpacing(40);
    this->getView()->addSubview(p_PageView);
    
    p_bg = CAView::createWithColor(ccc4(0,0,0,200));
    p_bg->setLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_B_C(0, 0.85)));
    this->getView()->addSubview(p_bg);

    p_title = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(10, 80), DVerticalLayout_T_H(10, 50)));
    p_title->setText(m_image.m_title);
	p_title->setColor(CAColor_white);
    p_title->setFontSize(34);
    p_title->setBold(true);
    p_bg->addSubview(p_title);
    
    char temp[10];
    sprintf(temp, "1/%lu",m_image.m_imageUrl.size());
    p_index = CALabel::createWithLayout(DLayout(DHorizontalLayout_R_W(0, 70), DVerticalLayout_T_H(10, 50)));
    p_index->setText(temp);
	p_index->setColor(CAColor_white);
    p_index->setFontSize(30);
    p_index->setBold(true);
    p_bg->addSubview(p_index);
    
    p_des = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(10, 20), DVerticalLayoutFill));
    p_des->setText(m_image.m_imageDesc[4]);
	p_des->setColor(ccc4(200, 200, 200, 255));
    p_des->setFontSize(26);
    
    CAScrollView* ps = CAScrollView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_H(70, 200)));
    ps->setBackgroundColor(CAColor_clear);
    ps->setShowsVerticalScrollIndicator(false);
    ps->setShowsHorizontalScrollIndicator(false);
    ps->setBounceHorizontal(false);
    ps->addSubview(p_des);
    p_bg->addSubview(ps);

}

void CDShowNewsImage::pageViewDidEndTurning(CAPageView* pageView)
{
    CCLog("----%d----%s",pageView->getCurrPage(),m_image.m_imageDesc[pageView->getCurrPage()].c_str());
    p_des->setText(m_image.m_imageDesc[pageView->getCurrPage()]);
    char temp[10];
    sprintf(temp, "%d/%lu",pageView->getCurrPage()+1,m_image.m_imageUrl.size());
    p_index->setText(temp);
    CAVector<CAView* > tem_view = p_PageView->getSubviews();
    for (int i=0; i<tem_view.size(); i++) {
        CAScrollView* tem = (CAScrollView*)p_PageView->getSubViewAtIndex(i);
        tem->setZoomScale(1.0f);
        tem->setContentOffset(DPointZero, false);
    }
    
}

void CDShowNewsImage::scrollViewTouchUpWithoutMoved(CAScrollView* view, const DPoint& point)
{
    if (m_isShow)
    {
        m_isShow=false;
        //开始执行动画
        CAViewAnimation::beginAnimations("", NULL);
        //动画时长
        CAViewAnimation::setAnimationDuration(0.2f);
        
        p_bg->setLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_B_H(0, 0)));
        
        //执行动画
        CAViewAnimation::commitAnimations();
    }
    else
    {
        m_isShow=true;
        //开始执行动画
        CAViewAnimation::beginAnimations("", NULL);
        //动画时长
        CAViewAnimation::setAnimationDuration(0.2f);
        
        p_bg->setLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_B_C(0, 0.85)));
        
        //执行动画
        CAViewAnimation::commitAnimations();
    }
}