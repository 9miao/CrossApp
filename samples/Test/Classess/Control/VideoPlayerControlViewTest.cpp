
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
    DRectLayout winLayout = this->getView()->getLayout();
    CAVideoPlayerControlView* VideoPlayerControlView = CAVideoPlayerControlView::createWithLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B));
    VideoPlayerControlView->initWithUrl("http://42.62.6.159/video.mp4");
    //VideoPlayerControlView->initWithPath("/image/a.mp4");
    VideoPlayerControlView->setTitle("视频");
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