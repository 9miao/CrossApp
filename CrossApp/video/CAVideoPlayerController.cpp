#include "CAVideoPlayerController.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"
#include "view/CAWindow.h"


NS_CC_BEGIN

CAVideoPlayerController::CAVideoPlayerController()
{
    
}

CAVideoPlayerController::~CAVideoPlayerController()
{
}

void CAVideoPlayerController::viewDidLoad()
{
	/*CCRect frame = getView()->getFrame();

	frame.origin.x = frame.size.width/2;
	frame.origin.y = frame.size.height/2;
	_activityView = CAActivityIndicatorView::create();
	_activityView->setStyle(CAActivityIndicatorViewStyleWhite);
	_activityView->setCenterOrigin(frame.origin);
	_activityView->startAnimating();
	_activityView->retain();
	getView()->insertSubview(_activityView, 1);

	displayHUD(true);*/
}

void CAVideoPlayerController::viewDidUnload()
{
    
}

bool CAVideoPlayerController::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	/*if (_HUDView->isVisible()) {
		dispearHUDView();
		}
		else {
		showHUDView();
		}*/
    return false;
}

NS_CC_END
