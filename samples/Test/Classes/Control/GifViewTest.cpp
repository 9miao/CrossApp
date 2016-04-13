
#include "GifViewTest.h"

GifViewTest::GifViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

GifViewTest::~GifViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void GifViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    CAGifView* GifView = CAGifView::createWithGif(CAGif::createWithFilePath("image/gifview1.gif"));
    GifView->setLayout(DLayout(DHorizontalLayout_W_C(400, 0.5), DVerticalLayout_H_C(400, 0.5)));
    //设置Gif永远重复
    GifView->setRepeatForever(true);
    //查看Gif是否永远重复
    CCLog("isRepeatForever = %d", GifView->isRepeatForever());
    
    //设置Frame
    //GifView->setFrame(DRect(this->getView()->getBounds()));
    //设置Center
    //GifView->setCenter(DRect(this->getView()->getBounds()));
    //设置Times
    //GifView->setTimes(10.00);
    //初始化
    //GifView->init();
    //初始化指定Gif
    //GifView->initWithGif(CAGif::createWithFilePath("image/gifview1.gif"));
    //设置Gif
    //GifView->setGif(CAGif::createWithFilePath("image/gifview1.gif"));
    
    this->getView()->addSubview(GifView);
}

void GifViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}