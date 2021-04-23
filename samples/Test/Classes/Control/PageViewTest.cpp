
#include "PageViewTest.h"

PageViewTest::PageViewTest()
{
    this->setTitle("CAImageView");
}

PageViewTest::~PageViewTest()
{
}

void PageViewTest::viewDidLoad()
{
    
    std::vector<CAView*> vec =
    {
        CAImageView::createWithImage(CAImage::create("image/1.jpg")),
        CAImageView::createWithImage(CAImage::create("image/2.jpg")),
        CAImageView::createWithImage(CAImage::create("image/3.jpg"))
    };

    m_pPageView = CAPageView::createWithLayout(DLayoutFill, CAPageView::Orientation::Horizontal);
    m_pPageView->setViews(CAVector<CAView*>(vec));
    this->getView()->addSubview(m_pPageView);
    
    m_pPageView->onEndTurning(CALLBACK_BIND_0(PageViewTest::pageViewDidEndTurning, this));
    
    m_pPageControl = CAPageControl::createWithLayout(DLayout(DHorizontalLayout_W_C(100, 0.5), DVerticalLayout_T_H(30, 50)));
    m_pPageControl->setNumberOfPages((int)vec.size());
    m_pPageControl->setPageIndicatorTintColor(CAColor4B::GRAY);
    m_pPageControl->setCurrentPageIndicatorTintColor(CAColor4B::GREEN);
    m_pPageControl->setTarget([&](int index)
    {
        m_pPageView->setCurrPage(index, true);
    });
    this->getView()->addSubview(m_pPageControl);
}

void PageViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}



void PageViewTest::pageViewDidEndTurning()
{
    m_pPageControl->setCurrentPage(m_pPageView->getCurrPage());
    m_pPageControl->updateCurrentPageDisplay();
}

