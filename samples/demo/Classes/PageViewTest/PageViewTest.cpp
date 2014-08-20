#include "PageViewTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

PageViewTest::PageViewTest()
:fullScreen(false)
{

}

PageViewTest::~PageViewTest()
{

}

void PageViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	segmented = CASegmentedControl::createWithCenter(CCRect(size.width*0.5, size.height*0.05+2, size.width*0.3, size.height*0.1), 2);
	segmented->setBackgroundImageAtIndex(CAImage::create("source_material/seg_highlighted1.png"), 0, CAControlStateNormal);
	segmented->setBackgroundImageAtIndex(CAImage::create("source_material/seg_normal1.png"), 0, CAControlStateHighlighted);
	segmented->setBackgroundImageAtIndex(CAImage::create("source_material/seg_selected1.png"), 0, CAControlStateSelected);
	segmented->setBackgroundImageAtIndex(CAImage::create("source_material/seg_highlighted3.png"), 1, CAControlStateNormal);
	segmented->setBackgroundImageAtIndex(CAImage::create("source_material/seg_normal3.png"), 1, CAControlStateHighlighted);
	segmented->setBackgroundImageAtIndex(CAImage::create("source_material/seg_selected3.png"), 1, CAControlStateSelected);
	segmented->setTitleAtIndex("0", 0, CAControlStateAll);
	segmented->setTitleAtIndex("1", 1, CAControlStateAll);
	segmented->setTitleColorAtIndex(CAColor_white, 0, CAControlStateAll);
	segmented->setTitleColorAtIndex(CAColor_white, 1, CAControlStateAll);
	segmented->setSelectedAtIndex(0);
	segmented->addTarget(this,CAControl_selector(PageViewTest::segBtnCallBack));
	this->getView()->insertSubview(segmented,3);


	CAImageView* view1 = CAImageView::createWithImage(CAImage::create("bg.jpg"));
	CAImageView* view2 = CAImageView::createWithImage(CAImage::create("2.png"));
	CAView* view3 = CAView::createWithColor(CAColor_green);
	_view.pushBack(view1);
	_view.pushBack(view2);
	_view.pushBack(view3);

	pageViewTest = CAPageView::createWithCenter(CCRect(size.width*0.5, size.height*0.5, size.width, size.height), CAPageView::CAPageViewDirectionHorizontal);
	pageViewTest->setPageViewDelegate(this);
	pageViewTest->setViews(_view);
	this->getView()->addSubview(pageViewTest);
}

void PageViewTest::segBtnCallBack(CAControl* btn, CCPoint point)
{
	pageViewTest->setCurrPage(segmented->getselectedIndex(), true);
}

void PageViewTest::pageViewDidBeginTurning(CAPageView* pageView)
{

}

void PageViewTest::pageViewDidEndTurning(CAPageView* pageView)
{
	segmented->setSelectedAtIndex(pageView->getCurrPage());
}

void PageViewTest::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index)
{
	if (!fullScreen)
	{
		segmented->setVisible(false);
		this->getNavigationController()->setNavigationBarHidden(true, true);
		this->getNavigationController()->getTabBarController()->setTabBarHidden(true, true);
		fullScreen = true;
	}
	else
	{
		segmented->setVisible(true);
		this->getNavigationController()->setNavigationBarHidden(false, true);
		this->getNavigationController()->getTabBarController()->setTabBarHidden(false, true);
		fullScreen = false;
	}
	reshapeViewRectDidFinish();
}

void PageViewTest::viewDidUnload()
{


}

void PageViewTest::reshapeViewRectDidFinish()
{
	CCSize currentSize = this->getView()->getBounds().size;
	float scaleY = fullScreen ? (currentSize.height / size.height) : (size.height / currentSize.height);
	pageViewTest->setScaleY(scaleY);
	pageViewTest->setCenterOrigin(CCPoint(currentSize.width*0.5, currentSize.height*0.5));
}