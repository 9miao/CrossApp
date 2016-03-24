
#include "FlashViewTest.h"
#include "gameswf/CAFlashView.h"
#include "gameswf/CAFlash.h"

FlashViewTest::FlashViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

FlashViewTest::~FlashViewTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void FlashViewTest::viewDidLoad()
{
    this->getView()->setColor(CAColor_gray);
    
    CrossApp::extension::CAFlashView* swfBg = CrossApp::extension::CAFlashView::createWithFlash(CrossApp::extension::CAFlash::createWithFilePath("image/swfWK.swf"));
    swfBg->setLayout(DRectLayout(150, 150, 200, 200, DRectLayout::L_R_T_B));
    swfBg->runAnimation();
    swfBg->setRepeatForever(true);
    this->getView()->addSubview(swfBg);
}

void FlashViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}