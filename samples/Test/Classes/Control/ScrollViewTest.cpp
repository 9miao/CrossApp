
#include "ScrollViewTest.h"

ScrollViewTest::ScrollViewTest()
{
    this->setTitle("CAScrollView");
}

ScrollViewTest::~ScrollViewTest()
{
}

void ScrollViewTest::viewDidLoad()
{
    auto scrollView = CAScrollView::createWithLayout(DLayoutFill);
    scrollView->setMinimumZoomScale(0.2f);
    scrollView->setMaximumZoomScale(5.0f);
    scrollView->setMultitouchGesture(CAScrollView::MultitouchGesture::ZoomAndRotate);
    scrollView->setViewSize(DSize(2160, 3840));
    scrollView->setReachBoundaryHandOverToSuperview(false);
    this->getView()->addSubview(scrollView);

    auto imageView = CAImageView::createWithImage(CAImage::create("image/h1.png"));
    imageView->setLayout(DLayoutFill);
    scrollView->addSubview(imageView);

    scrollView->onDidMoved([=]
    {
        CCLog("onDidMoved");
    });
    scrollView->onStopMoved([=]
    {
        CCLog("onStopMoved");
    });
    scrollView->onWillBeginDragging([=]
    {
        CCLog("onWillBeginDragging");
    });
    scrollView->onDragging([=]
    {
        CCLog("onDragging");
    });
    scrollView->onDidEndDragging([=]
    {
        CCLog("onDidEndDragging");
    });
    scrollView->onDidZoom([=]
    {
        CCLog("onDidZoom");
    });
    scrollView->onTouchUpWithoutMoved([=](DPoint point)
    {
        CCLog("onTouchUpWithoutMoved");
    });
}

void ScrollViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
