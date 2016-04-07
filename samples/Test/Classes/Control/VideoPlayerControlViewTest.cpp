
#include "VideoPlayerControlViewTest.h"

VideoPlayerControlViewTest::VideoPlayerControlViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

VideoPlayerControlViewTest::~VideoPlayerControlViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void VideoPlayerControlViewTest::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    CAVideoPlayerControlView* VideoPlayerControlView = CAVideoPlayerControlView::createWithLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B));
	//VideoPlayerControlView->setUrl("http://42.62.6.159/big_buck_bunny.mp4");
    VideoPlayerControlView->setFullPath(CCFileUtils::sharedFileUtils()->fullPathForFilename("image/abc.mp4"));
	//VideoPlayerControlView->setFullPath(CCFileUtils::sharedFileUtils()->fullPathForFilename("c:/video2.mp4"));
    VideoPlayerControlView->setTitle("视屏");
    //VideoPlayerControlView->setShowBackButton(false);
    VideoPlayerControlView->setPlayerControlViewDelegate(this);
    this->getView()->addSubview(VideoPlayerControlView);
}

void VideoPlayerControlViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void VideoPlayerControlViewTest::onBackButtonClicked(CAVideoPlayerControlView *playerControlView)
{
    CCLog("点击后退");
}