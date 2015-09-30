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

    for (int i=0; i<m_image.m_imageUrl.size(); i++) {
        //初始化viewList
        CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(CADipRect(0,0,winSize.width,winSize.height));
        temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        temImage->setImage(CAImage::create("HelloWorld.png"));
        temImage->setUrl(m_image.m_imageUrl[i]);
        
        CAScrollView* p_ScrollView = CAScrollView::createWithFrame(CADipRect(0,0,winSize.width,winSize.height));
        p_ScrollView->setMinimumZoomScale(1.0f);
        p_ScrollView->setMaximumZoomScale(2.5f);
        p_ScrollView->setBackGroundColor(CAColor_clear);
        p_ScrollView->setShowsScrollIndicators(false);
        p_ScrollView->setBounceVertical(false);
        p_ScrollView->setScrollViewDelegate(this);
        p_ScrollView->addSubview(temImage);
        viewList.pushBack(p_ScrollView);
    }
    p_PageView = CAPageView::createWithFrame(CADipRect(0, 30, winSize.width, winSize.height), CAPageViewDirectionHorizontal);
    p_PageView->setViews(viewList);
    p_PageView->setPageViewDelegate(this);
    p_PageView->setBackGroundColor(CAColor_black);
    p_PageView->setPageViewDelegate(this);
    this->getView()->addSubview(p_PageView);
    
    p_bg = CAView::createWithColor(ccc4(0,0,0,200));
    p_bg->setFrame(CADipRect(0,winSize.height/3*2,winSize.width,winSize.height/2));
    this->getView()->addSubview(p_bg);

    p_title = CALabel::createWithFrame(CADipRect(10,10,winSize.width-80,40));
    p_title->setText(m_image.m_title);
	p_title->setFontColor(CAColor_white);
    p_title->setFontSize(_px(34));
    p_title->setBold(true);
    p_bg->addSubview(p_title);
    
    char temp[10];
    sprintf(temp, "1/%lu",m_image.m_imageUrl.size());
    p_index = CALabel::createWithFrame(CADipRect(winSize.width-60,10,80,30));
    p_index->setText(temp);
	p_index->setFontColor(CAColor_white);
    p_index->setFontSize(_px(30));
    p_index->setBold(true);
    p_bg->addSubview(p_index);
    
    p_des = CALabel::createWithFrame(CADipRect(10,0,winSize.width-20,200));
    p_des->setText(m_image.m_imageDesc[4]);
	p_des->setFontColor(ccc4(200, 200, 200, 255));
    p_des->setFontSize(_px(26));
    
    CAScrollView* ps = CAScrollView::createWithFrame(CADipRect(0,70,winSize.width,200));
    ps->setBackGroundColor(CAColor_clear);
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
        tem->setContentOffset(CCPointZero, false);
    }
    
}

void CDShowNewsImage::scrollViewTouchUpWithoutMoved(CAScrollView* view, const CCPoint& point)
{
    if (m_isShow)
    {
        m_isShow=false;
        //开始执行动画
        CAViewAnimation::beginAnimations("", NULL);
        //动画时长
        CAViewAnimation::setAnimationDuration(0.2f);
        
        p_bg->setFrameOrigin(CADipPoint(0,winSize.height));
        
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
        
        p_bg->setFrameOrigin(CADipPoint(0,winSize.height/3*2));
        
        //执行动画
        CAViewAnimation::commitAnimations();
    }
}