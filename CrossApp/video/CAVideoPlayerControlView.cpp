#include "CAVideoPlayerControlView.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"
#include "view/CAWindow.h"

NS_CC_BEGIN

static string formatTimeInterval(float seconds, bool isLeft)
{
    seconds = MAX(0, seconds);
    
    int s = seconds;
    int m = s / 60;
    int h = m / 60;
    
    s = s % 60;
    m = m % 60;
    
    string format = (isLeft && seconds >= 0.5) ? "-" : "";
    char output[256] = {0};
    if (h != 0) {
        format.append("%d:%02d:%02d");
        sprintf(output, format.c_str(), h, m, s);
    } else {
        format.append("%02d:%02d");
        sprintf(output, format.c_str(), m, s);
    }
    
    return string(output);
}


CAVideoPlayerController::CAVideoPlayerController()
: _disableUpdateHUD(false)
, _glView(NULL)
, _slider(NULL)
, _HUDView(NULL)
, m_bNeedExit(false)
, m_inited(false)
, _activityView(NULL)
, _playButton(NULL)
, _playSlider(NULL)
, _playTime(NULL)
{
    
}

CAVideoPlayerController::~CAVideoPlayerController()
{
    CC_SAFE_RELEASE_NULL(_glView);
	//CC_SAFE_RELEASE_NULL(_currentAudioFrame);
     
    CC_SAFE_RELEASE_NULL(_slider);
    CC_SAFE_RELEASE_NULL(_HUDView);
    
    CC_SAFE_RELEASE_NULL(_activityView);
    CC_SAFE_RELEASE_NULL(_playButton);
    CC_SAFE_RELEASE_NULL(_playTime);
    CC_SAFE_RELEASE_NULL(_playSlider);
}

CAVideoPlayerController* CAVideoPlayerController::createWithPath(const string &path, const string& title)
{
    CAVideoPlayerController* p = new CAVideoPlayerController();
    if (p && p->init(path, title,false)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return p;
}

CAVideoPlayerController* CAVideoPlayerController::createWithUrl(const string &path, const string& title)
{
    CAVideoPlayerController* p = new CAVideoPlayerController();
    if (p && p->init(path, title,true)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return p;
}

bool CAVideoPlayerController::init(const string& path, const string& title,bool _pathType)
{
    CCAssert(path.size() > 0, "empty path");
    
    if (!CAViewController::init()) {
        return false;
    }
    
    getView()->setColor(ccc4(0, 0, 0, 255));

//    _moviePosition = 0;
 //   _isPathByUrl = _pathType;
    _path = path;
    _title = title;

    return true;
}

void CAVideoPlayerController::viewDidLoad()
{
    CCRect frame = getView()->getFrame();
    
    frame.origin.x = frame.size.width/2;
    frame.origin.y = frame.size.height/2;
    _activityView = CAActivityIndicatorView::create();
    _activityView->setStyle(CAActivityIndicatorViewStyleWhite);
    _activityView->setCenterOrigin(frame.origin);
    _activityView->startAnimating();
    _activityView->retain();
    getView()->insertSubview(_activityView, 1);
    
    displayHUD(true);
}

void CAVideoPlayerController::viewDidUnload()
{
    
}

bool CAVideoPlayerController::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    if (_HUDView->isVisible()) {
        dispearHUDView();
    }
    else {
        showHUDView();
    }
    return false;
}


void CAVideoPlayerController::play()
{
//    if (this->isPlaying()) {
//        return;
 //   }
    
    _disableUpdateHUD = false;
        
    
    updatePlayButton();
    
    CAScheduler::schedule(schedule_selector(CAVideoPlayerController::updateHUD), this, 0);
//    if (_decoder->isValidAudio()) {
        //enableAudio(true);
        //freeBufferedFrames();
 //   }
        
    //CCLog("play movie");
}

void CAVideoPlayerController::pause()
{
//    if (!this->isPlaying()) {
//        return;
//    }
 //   _playing = false;
 //   this->enableAudio(false);
    updatePlayButton();
}

void CAVideoPlayerController::updatePlayButton()
{
	/*
    if (isPlaying()) {
        CAImage* backImage = CAImage::create("source_material/vdo_pause.png");
        CAImage* backImage_h = CAImage::create("source_material/vdo_pause_down.png");
        _playButton->setImageForState(CAControlStateAll, backImage);
        _playButton->setImageForState(CAControlStateHighlighted, backImage_h);
    } else {
        CAImage* backImage = CAImage::create("source_material/vdo_play.png");
        CAImage* backImage_h = CAImage::create("source_material/vdo_play_down.png");
        _playButton->setImageForState(CAControlStateAll, backImage);
        _playButton->setImageForState(CAControlStateHighlighted, backImage_h);        
    }
	*/
}

void CAVideoPlayerController::onButtonBack(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    pause();
    CCLog("%s, SDL_Quilt", __FUNCTION__);
    SDL_Quit();
    CCLog("%s, freeBufferedFrames", __FUNCTION__);
//    freeBufferedFrames();
    
    CCLog("%s, signal", __FUNCTION__);
    
    m_bNeedExit = true;
}

void CAVideoPlayerController::onButtonPause(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
//    if (isPlaying()) {
//        pause();
//    } else {
//        play();
//    }
}

void CAVideoPlayerController::updateHUD(float)
{
    if (_disableUpdateHUD)
        return;
    //float ttt = _decoder->getStartTime();
//    const float duration = _decoder->getDuration();
//    const float position = _moviePosition - _decoder->getStartTime();

 //   _playSlider->setValue(position / duration);
//    _playTime->setText(formatTimeInterval(position, false).append(" / ").append(formatTimeInterval(duration-1, false)));
}

void CAVideoPlayerController::buildHUD()
{
    if (_HUDView) {
        getView()->removeSubview(_HUDView);
        CC_SAFE_RELEASE_NULL(_HUDView);
    }
    _HUDView = CAView::createWithFrame(getView()->getFrame());
    _HUDView->setFrameOrigin(CCPointZero);
    _HUDView->setColor(ccc4(0, 0, 0, 0));
    _HUDView->retain();
    getView()->insertSubview(_HUDView, 1);
    
    // Bottom Panel Back
    CAImageView* bottomPanel = NULL;
    do {
        CCRect frame = _HUDView->getFrame();
        CAImage* image = CAImage::create("source_material/vdo_panel_bottom_bg.png");
        float width = _HUDView->getFrame().size.width;
        float height = _px(image->getContentSize().height);
        bottomPanel = CAImageView::createWithFrame(CCRect(0, frame.size.height - height,
                                                          width, height));
        bottomPanel->setImage(image);
        _HUDView->addSubview(bottomPanel);
    } while (0);
    
    // Slider 
    do {
        CCRect frame = bottomPanel->getFrame();
        CAImage* backImage = CAImage::create("source_material/vdo_progress_back.png");
        CAImage* barImage = CAImage::create("source_material/vdo_progress_bar.png");
        _slider = CASlider::createWithCenter(CCRect(frame.size.width/2,
                                                    frame.size.height*0.3,
                                                    frame.size.width * 0.9,
                                                    _px(barImage->getContentSize().height)));
        _slider->setMaxTrackTintImage(backImage);
        _slider->setThumbTintImage(barImage);
        _slider->retain();
        _slider->setTrackHeight(_px(backImage->getContentSize().height));
        _slider->addTargetForTouchUpSide(this, CAControl_selector(CAVideoPlayerController::onSlideChanged));
        _slider->addTarget(this, CAControl_selector(CAVideoPlayerController::onSlideTouched));
        _playSlider = _slider; _playSlider->retain();
        bottomPanel->addSubview(_slider);
    } while (0);
    
    // Play Pause Button
    do {
        CCRect frame = bottomPanel->getFrame();
        CAImage* backImage = CAImage::create("source_material/vdo_pause.png");
        CAImage* backImage_h = CAImage::create("source_material/vdo_pause_down.png");
        frame.origin.y = frame.size.height*2/3;
        frame.origin.x = backImage->getContentSize().width;
        frame.size.height = _px(backImage->getContentSize().height);
        frame.size.width = _px(backImage->getContentSize().width);
        _playButton = CAButton::createWithCenter(frame, CAButtonTypeCustom);
        _playButton->setImageForState(CAControlStateAll, backImage);
        _playButton->setImageForState(CAControlStateHighlighted, backImage_h);
        _playButton->addTarget(this, CAControl_selector(CAVideoPlayerController::onButtonPause), CAControlEventTouchUpInSide);
        _playButton->retain();
        bottomPanel->addSubview(_playButton);
    } while (0);
    
    // play time
    do {
        CCRect frame = _playButton->getFrame();
        CCRect newFrame = CCRectZero;
        _playTime = CALabel::createWithFrame(CCRectZero);
        _playTime->setFontSize(32);
        _playTime->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        _playTime->setColor(ccc4(255, 255, 255, 255));
        newFrame.origin.x = frame.origin.x * 2 + frame.size.width;
        newFrame.origin.y = frame.origin.y;
        newFrame.size.width = _playTime->getFontSize() * 20;
        newFrame.size.height = frame.size.height;
        _playTime->setFrame(newFrame);
        _playTime->retain();
        _playTime->setText("00:00 / 00:00");
        bottomPanel->addSubview(_playTime);
    } while (0);
    
    // Top Panel Back
    CAImageView* topPanel = NULL;
    do {
        CAImage* image = CAImage::create("source_material/vdo_panel_top_bg.png");
        topPanel = CAImageView::createWithFrame(CCRect(0, 0, 
                                                       _HUDView->getFrame().size.width,
                                                       _px(image->getContentSize().height)));
        topPanel->setImage(image);
        _HUDView->addSubview(topPanel);
    } while (0);
    
    // Back Button
    CAButton* buttonBack = NULL;
    do {
        CCRect frame = topPanel->getFrame();
//        CAImage* backImage = CAImage::create("source_material/vdo_btn_back.png");
//        CAImage* backImage_h = CAImage::create("source_material/vdo_btn_back_h.png");
        CAImage* backImage = CAImage::create("source_material/btn_left_blue.png");
        CAImage* backImage_h = CAImage::create("source_material/btn_left_white.png");
        frame.origin.y = frame.size.height/3;
        frame.origin.x = frame.origin.y;
        frame.size.height = _px(backImage->getContentSize().height);
        frame.size.width = _px(backImage->getContentSize().width);
        buttonBack = CAButton::createWithCenter(frame, CAButtonTypeCustom);
        buttonBack->setImageForState(CAControlStateAll, backImage);
        buttonBack->setImageForState(CAControlStateHighlighted, backImage_h);
        buttonBack->addTarget(this, CAControl_selector(CAVideoPlayerController::onButtonBack), CAControlEventTouchUpInSide);
        topPanel->addSubview(buttonBack);
    } while (0);
    
    // Title
    do {
        CCRect frame = buttonBack->getFrame();
        CCRect r = CCRectZero;
        r.origin.x = buttonBack->getFrame().origin.x*2 + buttonBack->getFrame().size.width;
        r.origin.y = buttonBack->getFrame().origin.y;
        r.size.width = _HUDView->getFrame().size.width - r.origin.x;
        r.size.height = buttonBack->getFrame().size.height;
        CALabel* title = CALabel::createWithFrame(r);
        title->setText(_title);
        title->setFontSize(_px(42));
        title->setColor(ccc4(255, 255, 255, 255));
        title->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        topPanel->addSubview(title);
    } while (0);
    
    
}

void CAVideoPlayerController::displayHUD(bool bDisp)
{
    if (bDisp) {
        buildHUD();
    }
    
    if (bDisp && _HUDView && !_HUDView->getSuperview()) {
        getView()->addSubview(_HUDView);
    } else if (!bDisp) {
        getView()->removeSubview(_HUDView);
    }
}

void CAVideoPlayerController::onSlideTouched(CAControl* control, CCPoint point)
{
    _disableUpdateHUD = true;
//    if (isPlaying()) {
//        pause();
//    }
}

void CAVideoPlayerController::onSlideChanged(CAControl *control, CCPoint point)
{
    CASlider* slider = dynamic_cast<CASlider*>(control);
    
//    if (!_decoder) {
//        slider->setValue(0);
 //       return;
 //   }
//    char value[20] = "";
//    CASlider* p_Slider = (CASlider*)control;
//    sprintf(value, "%.02f%%", slider->getValue() * 100);
//    CCLog("onSlideChanged == %s",value);
//    _moviePosition = slider->getValue() * _decoder->getDuration();
//    _decoder->setPosition(_moviePosition);
    
  //  if (slider) {
 //       CCLog("onSlideChanged == %f",slider->getValue() * _decoder->getDuration());
  //      if (isPlaying()) {
 //           setMoviePosition(slider->getValue() * _decoder->getDuration());
 //       }else{
 //           play();
 //           setMoviePosition(slider->getValue() * _decoder->getDuration());
 //       }
 //   }
}
/*
void CAVideoPlayerController::setMoviePositionFromDecoder()
{
    _moviePosition = _decoder->getPosition();
}

void CAVideoPlayerController::setDecoderPosition(float position)
{
    _decoder->setPosition(position);
}

void CAVideoPlayerController::gotoWantedMoviePosition(float)
{
    if (_decoder && !_decoding) {
        CAScheduler::unschedule(schedule_selector(CAVideoPlayerController::gotoWantedMoviePosition), this);
        setMoviePosition(_wantMoviePosition);
        _wantMoviePosition = 0;
    }
}

void CAVideoPlayerController::setMoviePosition(float position)
{
//    if (!_decoder || _decoding) {
//        _wantMoviePosition = position;
//        CAScheduler::unschedule(schedule_selector(CAVideoPlayerController::gotoWantedMoviePosition), this);
//        CAScheduler::schedule(schedule_selector(CAVideoPlayerController::gotoWantedMoviePosition), this, 0.1f);
//        _activityView->startAnimating();
//        return;
//    }
    
    bool playing = _playing;
    
    _playing = false;
    enableAudio(false);
    updatePosition(position, playing);
}

void CAVideoPlayerController::updatePosition(float position, bool playing)
{
//    freeBufferedFrames();
    //position = MIN(_decoder->getDuration() - 1, MAX(0, position));



    if (playing) {
        CCLog("position====%f",position);
        setDecoderPosition(position);
 //       setMoviePositionFromDecoder();
 //       play();
    } else {
        CCLog("position11111====%f",position);
 //       setDecoderPosition(position);
 //       vector<VPFrame*> frames;
 //       if (_decoder->isValidAudio() || _decoder->isValidVideo()) {
 //            frames = _decoder->decodeFrames(0);
 //       }
 //       if (frames.size() > 0) {
 //           addFrames(frames);
 //       }
        _disableUpdateHUD = false;
//        setMoviePositionFromDecoder();
//        presentFrame();
//        updateHUD(0);
    }

}
*/
void CAVideoPlayerController::dispearHUDView()
{
	//CAViewAnimation::beginAnimations("", NULL);
	//CAViewAnimation::setAnimationDuration(0.3f);
	//CAViewAnimation::setAnimationRepeatAutoreverses(true);
	//_HUDView->setVisible(false);
	//CAViewAnimation::commitAnimations();
}

void CAVideoPlayerController::showHUDView()
{
	//CAViewAnimation::beginAnimations("", NULL);
	//CAViewAnimation::setAnimationDuration(0.3f);
	//CAViewAnimation::setAnimationRepeatAutoreverses(true);
	//_HUDView->setVisible(true);
	//CAViewAnimation::commitAnimations();
}

//float CAVideoPlayerController::getDuration()
//{
//    return _decoder->getDuration();
//}
//
//float CAVideoPlayerController::getPosition()
//{
//    return _moviePosition - _decoder->getStartTime();
//}
//
//void CAVideoPlayerController::setPosition(float pos)
//{
//    setMoviePosition(pos);
//}
NS_CC_END
