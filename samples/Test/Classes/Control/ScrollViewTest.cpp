
#include "ScrollViewTest.h"

ScrollViewTest::ScrollViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

ScrollViewTest::~ScrollViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void ScrollViewTest::viewDidLoad()
{
    p_ScrollView = CAScrollView::createWithLayout(DLayoutFill);
    p_ScrollView->setMinimumZoomScale(0.2f);
    p_ScrollView->setMaximumZoomScale(5.0f);
    p_ScrollView->setMultitouchGesture(CAScrollView::MultitouchGesture::ZoomAndRotate);
    this->getView()->addSubview(p_ScrollView);
    p_ScrollView->setViewSize(DSize(2160, 3840));
    
    DSize _size = p_ScrollView->getBounds().size;
    p_imageView = CAImageView::createWithImage(CAImage::create("image/h1.png"));
    p_imageView->setLayout(DLayoutFill);
    p_ScrollView->addSubview(p_imageView);

}

void ScrollViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void ScrollViewTest::refreshData(float interval)
{
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        colorArr.push_back(ccc4(r, g, b, 255));
    }
    p_Conllection->reloadData();
}

void ScrollViewTest::scrollViewDidMoved(CAScrollView* view)
{
    CCLog("DidMoved-->");
}

void ScrollViewTest::scrollViewStopMoved(CAScrollView* view)
{
    CCLog("StopMoved-->");
}

void ScrollViewTest::scrollViewDidScroll(CAScrollView* view)
{
    CCLog("DidMScroll-->");
}

void ScrollViewTest::scrollViewWillBeginDragging(CAScrollView* view)
{
    CCLog("BeginDragging-->");
}

void ScrollViewTest::scrollViewDidEndDragging(CAScrollView* view)
{
    CCLog("DidEndDragging-->");
}

void ScrollViewTest::scrollViewDidZoom(CAScrollView* view)
{
    CCLog("DidZoom-->");
}

void ScrollViewTest::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
    colorArr.clear();
    CAScheduler::schedule(schedule_selector(ScrollViewTest::refreshData), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2, false);
}

void ScrollViewTest::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    CAScheduler::schedule(schedule_selector(ScrollViewTest::refreshData), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2, false);
}
