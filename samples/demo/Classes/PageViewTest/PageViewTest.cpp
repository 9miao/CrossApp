#include "PageViewTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

PageViewTest::PageViewTest()
:fullScreen(false)
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

PageViewTest::~PageViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

void PageViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	CAImageView* view1 = CAImageView::createWithImage(CAImage::create("background/5.jpg"));
	CAImageView* view2 = CAImageView::createWithImage(CAImage::create("background/6.jpg"));
	CAImageView* view3 = CAImageView::createWithImage(CAImage::create("background/7.jpg"));
	_view.pushBack(view1);
	_view.pushBack(view2);
	_view.pushBack(view3); 
	
	pageViewTest = CAPageView::createWithCenter(CADipRect(size.width*0.5, size.height*0.5, size.width, size.height), CAPageViewDirectionHorizontal);
	pageViewTest->setPageViewDelegate(this);
	pageViewTest->setViews(_view);
	this->getView()->addSubview(pageViewTest);

	pageControl = CAPageControl::createWithCenter(CADipRect(size.width*0.5, 50, 100, 50));
	pageControl->setNumberOfPages(_view.size());
	pageControl->setPageIndicatorTintColor(CAColor_gray);
	pageControl->setCurrentPageIndicatorTintColor(CAColor_green);
	pageControl->setPageIndicatorImage(CAImage::create("source_material/ex1.png"));
	pageControl->setCurrIndicatorImage(CAImage::create("source_material/ex2.png"));
	this->getView()->addSubview(pageControl);
}


void PageViewTest::pageViewDidBeginTurning(CAPageView* pageView)
{
	
}

void PageViewTest::pageViewDidEndTurning(CAPageView* pageView)
{
	pageControl->setCurrentPage(pageView->getCurrPage());
    pageControl->updateCurrentPageDisplay();
}

void PageViewTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const CCPoint& point)
{

}

void PageViewTest::viewDidUnload()
{

}

void PageViewTest::reshapeViewRectDidFinish()
{
	CADipSize currentSize = this->getView()->getBounds().size;
	CCLog("%f",currentSize.height);
	float scaleY = fullScreen ? (currentSize.height / size.height) : (size.height / currentSize.height);
	pageViewTest->setScaleY(scaleY);
	pageViewTest->setCenterOrigin(CADipPoint(currentSize.width*0.5, currentSize.height*0.5));
}