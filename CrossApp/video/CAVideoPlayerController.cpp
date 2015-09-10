#include "CAVideoPlayerController.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"
#include "view/CAWindow.h"
#include <pthread.h>

#include "SDL.h"

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

static void* decodeProcess(void* param)
{
    CAVideoPlayerController* controller = (CAVideoPlayerController*)param;
    
    controller->retain();
    controller->lazyProcess();
    CCLog("%s, retain=%d", __FUNCTION__, controller->retainCount());
    controller->release();
    
    return NULL;
}

void CAVideoPlayerController::lazyProcess()
{
	while (true) {
		if (m_bNeedExit) {
			break;
		}
		_decodeProcess();
		if (m_bNeedExit) {
			break;
		}
		pthread_mutex_lock(&m_vp_cond_mutex);
		pthread_cond_wait(&m_vp_cond, &m_vp_cond_mutex);
		pthread_mutex_unlock(&m_vp_cond_mutex);
	}

	if (m_inited) {
		pthread_mutex_destroy(&m_vp_data_mutex);
		pthread_mutex_destroy(&m_vp_cond_mutex);
		pthread_cond_destroy(&m_vp_cond);
		m_inited = false;
		m_bNeedExit = false;
	}
}

void CAVideoPlayerController::lazyInit()
{
    if (!m_inited) {
        pthread_mutex_init(&m_vp_data_mutex, NULL);
        pthread_mutex_init(&m_vp_cond_mutex, NULL);
        pthread_cond_init(&m_vp_cond, NULL);
        pthread_create(&m_thread, NULL, decodeProcess, (void*)this);
        m_inited = true;
    }
}

#define LOCAL_MIN_BUFFERED_DURATION   0.2
#define LOCAL_MAX_BUFFERED_DURATION   0.4
#define NETWORK_MIN_BUFFERED_DURATION 2.0
#define NETWORK_MAX_BUFFERED_DURATION 20.0


CAVideoPlayerController::CAVideoPlayerController()
: _decoder(NULL)
, _audioFrames(NULL)
, _videoFrames(NULL)
, _moviePosition(0)
, _bufferedDuration(0)
, _minBufferedDuration(0)
, _maxBufferedDuration(0)
, _currentAudioFrame(NULL)
, _playing(false)
, _disableUpdateHUD(false)
, _decoding(false)
, _buffered(false)
, _glView(NULL)
, _currentAudioFramePos(0)
, _slider(NULL)
, _HUDView(NULL)
, m_bNeedExit(false)
, m_inited(false)
, _activityView(NULL)
, _playButton(NULL)
, _playSlider(NULL)
, _playTime(NULL)
, _isPathByUrl(false)
{
    SDL_Quit();
}

CAVideoPlayerController::~CAVideoPlayerController()
{
    if (_decoder) {
        delete _decoder;
    }
    
    CC_SAFE_RELEASE_NULL(_glView);
	CC_SAFE_RELEASE_NULL(_currentAudioFrame);
     
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

    _moviePosition = 0;
    _isPathByUrl = _pathType;
    _path = path;
    _title = title;

    return true;
}

void CAVideoPlayerController::viewDidLoad()
{
    
    lazyInit();
    
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

bool CAVideoPlayerController::setMovieDecoder()
{
    CCLog("setMovieDecoder");    
    
    if (_decoder) {
        CCLog("_decoder has existed!");
        return true;
    }
    
    string path = _path;
    
    VPDecoder* decoder = new VPDecoder();
    decoder->setAudioCallback(this, decoder_audio_selector(CAVideoPlayerController::audioCallback));
    
    if (decoder && decoder->openFile(path, _isPathByUrl)) {
    
        if (_isPathByUrl) {
            
            _minBufferedDuration = NETWORK_MIN_BUFFERED_DURATION;
            _maxBufferedDuration = NETWORK_MAX_BUFFERED_DURATION;
            
        } else {
            
            _minBufferedDuration = LOCAL_MIN_BUFFERED_DURATION;
            _maxBufferedDuration = LOCAL_MAX_BUFFERED_DURATION;
        }
        
        if (!decoder->isValidVideo())
            _minBufferedDuration *= 10.0; // increase for audio
        
        _decoder = decoder;
        
        CCLog("buffered limit: %.1f - %.1f", _minBufferedDuration, _maxBufferedDuration);        
        
        return true;
    }
    
    return false;
}

void CAVideoPlayerController::freeBufferedFrames()
{
    pthread_mutex_lock(&m_vp_data_mutex);
    
    CAObject* object = NULL;
        
    _videoFrames.clear();
    _audioFrames.clear();

    _buffered = true;
    _bufferedDuration = 0;
    
    pthread_mutex_unlock(&m_vp_data_mutex);
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


void CAVideoPlayerController::prepare(float)
{
    if (!_decoder) {
        return;
    }
    
    if (_decoder && !_glView) {
        if (_decoder->isValidVideo()) {
            CCRect frame = getView()->getBounds();
            _glView = CAVideoPlayerView::createWithFrame(frame, _decoder);
            CC_SAFE_RETAIN(_glView);
            getView()->addSubview(_glView);
        }
    }
    
    play();
    
    CAScheduler::unschedule(schedule_selector(CAVideoPlayerController::prepare), this);
    
    pause();
}

void CAVideoPlayerController::play()
{
    if (this->isPlaying()) {
        return;
    }
    
    if (!_decoder) {
        CAScheduler::schedule(schedule_selector(CAVideoPlayerController::prepare), this, 0);
        return;
    }
    
    if (!_decoder->isValidAudio() && !_decoder->isValidVideo()) {
        return;
    }

    
    //CCLog("_wantMoviePosition===%f",_decoder->getPosition());
    _playing = true;
    _disableUpdateHUD = false;
        
	asyncDecodeFrames();
    
    updatePlayButton();
    
    tick(0);
    CAScheduler::schedule(schedule_selector(CAVideoPlayerController::updateHUD), this, 0);
    if (_decoder->isValidAudio()) {
        enableAudio(true);
        //freeBufferedFrames();
    }
        
    //CCLog("play movie");
}

void CAVideoPlayerController::pause()
{
    if (!this->isPlaying()) {
        return;
    }
    _playing = false;
    this->enableAudio(false);
    updatePlayButton();
}

bool CAVideoPlayerController::isPlaying()
{
    return _playing;
}

void CAVideoPlayerController::enableAudio(bool on)
{
    SDL_PauseAudio(!on);
}

void CAVideoPlayerController::asyncDecodeFrames()
{
    if (_decoding) {
        return;
    }
    
    if (!_playing) {
        return;
    }

    pthread_mutex_lock(&m_vp_cond_mutex);
    pthread_cond_signal(&m_vp_cond);
    pthread_mutex_unlock(&m_vp_cond_mutex);

}

void CAVideoPlayerController::_decodeProcess()
{
    if (!_decoder && !setMovieDecoder()) {
        return;
    }
    
    if (_decoding) {
        return;
    }
    
    if (!_playing) {
        return;
    }
    
    float duration = _decoder->getIsNetwork() ? .0f : 0.1f;
    
    _decoding = true;
    
    bool good = true;
    
    while (good) {
        good = false;
        if (_decoder && (_decoder->isValidVideo() || _decoder->isValidAudio())) {
            vector<VPFrame*> frames = _decoder->decodeFrames(duration);
            if (frames.size()) {
                good = addFrames(frames);
            }
        }
    }
    
    _decoding = false;
}

void CAVideoPlayerController::updatePlayButton()
{
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
}

bool CAVideoPlayerController::addFrames(const vector<VPFrame*>& frames)
{
    pthread_mutex_lock(&m_vp_data_mutex);
    
    vector<VPFrame*>::iterator obj;
    
    if (_decoder->isValidVideo()) {
        for (int i=0; i<frames.size(); i++) {
            VPFrame* frame = frames.at(i);
            if (frame && frame->getType() == kFrameTypeVideo) {
                _videoFrames.pushBack(frame);
                _bufferedDuration += frame->getDuration();
            }
        }
    }
    
    if (_decoder->isValidAudio()) {

        for (int i=0; i<frames.size(); i++) {
            VPFrame* frame = frames.at(i);
            if (frame && frame->getType() == kFrameTypeAudio) {
                _audioFrames.pushBack(frame);
                if (!_decoder->isValidVideo()) {
                    _bufferedDuration += frame->getDuration();
                }
            }
        }
    }
    
        
    pthread_mutex_unlock(&m_vp_data_mutex);
    
    return _playing && _bufferedDuration < _maxBufferedDuration;
}

void CAVideoPlayerController::tick(float dt)
{
    if (_buffered && ((_bufferedDuration > _minBufferedDuration) || _decoder->isEOF())) {
        
        _buffered = false;
        
        _activityView->stopAnimating();
    }
    
    
    float interval = 0;
    if (!_buffered && _playing)
        interval = presentFrame();
    
    if (_playing) {
        
        const unsigned int leftFrames =
        (_decoder->isValidVideo() ? _videoFrames.size() : 0) +
        (_decoder->isValidAudio() ? _audioFrames.size() : 0);
        
        if (0 == leftFrames) {
            
            if (_decoder->isEOF()) {
                
                pause();
                updateHUD(0);
                return;
            }
            
            if (_minBufferedDuration > 0 && !_buffered) {
                
                _buffered = true;
                
                _activityView->startAnimating();
            }
        }

//        if (!leftFrames || !(_bufferedDuration > _minBufferedDuration)) {
		if (_bufferedDuration < _maxBufferedDuration) 
		{
			asyncDecodeFrames();
		}
        
		float correction = 0;// tickCorrection();
        float time = MAX(interval + correction, 0.01);
//        CCLog("%s, %f, %f, %f", __FUNCTION__, time, dt, correction);
        
        CAScheduler::schedule(schedule_selector(CAVideoPlayerController::tick), this, time);
    }

}

/*
float CAVideoPlayerController::tickCorrection()
{
    if (_buffered)
        return 0;
    
    struct timeval now;
    gettimeofday(&now,  0);
    
    if (!_tickCorrectionTime.tv_sec) {
        
        _tickCorrectionTime = now;
        _tickCorrectionPosition = _moviePosition;
        return 0;
    }
    
    float dPosition = _moviePosition - _tickCorrectionPosition;
    float dTime = (now.tv_sec - _tickCorrectionTime.tv_sec) + (now.tv_usec - _tickCorrectionTime.tv_usec)/1000000.0f;
    float correction = dPosition - dTime;
    
    if (correction > 1.f || correction < -1.f) {
        
        CCLog("tick correction reset %.2f", correction);
        correction = 0;
        _tickCorrectionTime.tv_sec = 0;
        _tickCorrectionTime.tv_usec = 0;
    }
    
    return correction;
}
*/


float CAVideoPlayerController::presentFrame()
{
    float interval = 0;
    
    if (_decoder->isValidVideo()) {
        
        VPVideoFrame *frame = NULL;
        
        pthread_mutex_lock(&m_vp_data_mutex);
                    
        if (_videoFrames.size() > 0) {
            
            frame = (VPVideoFrame*)_videoFrames.at(0);
            _videoFrames.eraseObject(frame);
            _bufferedDuration -= frame->getDuration();
        }
        
        pthread_mutex_unlock(&m_vp_data_mutex);
        
        if (frame) {
            interval = presentVideoFrame(frame);
            frame->release();
        }
        
    }
    return interval;
}

float CAVideoPlayerController::presentVideoFrame(VPVideoFrame *frame)
{
    if (_glView) {
        
        _glView->setCurrentFrame(frame);
    }
    _moviePosition = frame->getPosition();

	//float aaaaa = _decoder->getDuration();
	//aaaaa -= _moviePosition;
    if (_decoder->getDuration()-_moviePosition<1) {
        pause();
        _moviePosition = 0;
        _currentAudioFramePos = 0;
        _decoder->setPosition(0);
        setDecoderPosition(0);
        presentFrame();
        freeBufferedFrames();
    }
    return frame->getDuration();
}


void CAVideoPlayerController::audioCallback(unsigned char *stream, int len, int channels)
{
    if (_buffered) {
		memset(stream, 0, len * channels);
        return;
    }
    
    while (len > 0) {
        
        unsigned int count = _audioFrames.size();

        if (!_currentAudioFrame) {
            
            if (count > 0) {
                pthread_mutex_lock(&m_vp_data_mutex);
                VPAudioFrame* frame = (VPAudioFrame*)_audioFrames.at(0);
                pthread_mutex_unlock(&m_vp_data_mutex);

                
                //CCLog("Audio frame position: %f, %f", _moviePosition, frame->getPosition());
                
                if (_decoder->isValidVideo()) {
                    const float delta = _moviePosition - frame->getPosition();
                    
                    if (delta < -0.1) {
                        memset(stream, 0, len * channels);
                        break;
                    }
                    
                    pthread_mutex_lock(&m_vp_data_mutex);
                    _audioFrames.erase(0);
                    pthread_mutex_unlock(&m_vp_data_mutex);
                    
                    if (delta > 0.1 && count > 1)
                    {
                        continue;
                    }
                    
                } else {
                    _audioFrames.erase(0);
                    _moviePosition = frame->getPosition();
                    _bufferedDuration -= frame->getDuration();
                }
                
                _currentAudioFramePos = 0;
                CC_SAFE_RELEASE_NULL(_currentAudioFrame);
                _currentAudioFrame = frame;
//                CC_SAFE_RETAIN(_currentAudioFrame);
            }
        }
        
        if (_currentAudioFrame) {
            unsigned char* bytes = (unsigned char*)(_currentAudioFrame->getData() + _currentAudioFramePos);
            const unsigned int bytesLeft = _currentAudioFrame->getDataLength() - _currentAudioFramePos;
            const unsigned int bytesToCopy = MIN(len, bytesLeft);
            
//            CCLog("%s, copyLen = %d, leftLen = %d, count = %d", __FUNCTION__, bytesToCopy, len - bytesToCopy, count);
            memcpy(stream, bytes, bytesToCopy);
            stream += bytesToCopy;
            len -= bytesToCopy;
            
            if (bytesToCopy < bytesLeft) {
                _currentAudioFramePos += bytesToCopy;
            } else {
                CC_SAFE_RELEASE_NULL(_currentAudioFrame);
            }
        } else {
			memset(stream, 0, len * channels);
			return;
        }
    } 
}

void CAVideoPlayerController::onButtonBack(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    pause();
    CCLog("%s, SDL_Quilt", __FUNCTION__);
    SDL_Quit();
    CCLog("%s, freeBufferedFrames", __FUNCTION__);
    freeBufferedFrames();
    
    CCLog("%s, signal", __FUNCTION__);
    
    m_bNeedExit = true;
    pthread_mutex_lock(&m_vp_cond_mutex);
    pthread_cond_signal(&m_vp_cond);
    pthread_mutex_unlock(&m_vp_cond_mutex);
}

void CAVideoPlayerController::onButtonPause(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    if (isPlaying()) {
        pause();
    } else {
        play();
    }
}

void CAVideoPlayerController::updateHUD(float)
{
    if (_disableUpdateHUD)
        return;
    //float ttt = _decoder->getStartTime();
    const float duration = _decoder->getDuration();
    const float position = _moviePosition - _decoder->getStartTime();

    _playSlider->setValue(position / duration);
    _playTime->setText(formatTimeInterval(position, false).append(" / ").append(formatTimeInterval(duration-1, false)));
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
    if (isPlaying()) {
        pause();
    }
}

void CAVideoPlayerController::onSlideChanged(CAControl *control, CCPoint point)
{
    CASlider* slider = dynamic_cast<CASlider*>(control);
    
    if (!_decoder) {
        slider->setValue(0);
        return;
    }
//    char value[20] = "";
//    CASlider* p_Slider = (CASlider*)control;
//    sprintf(value, "%.02f%%", slider->getValue() * 100);
//    CCLog("onSlideChanged == %s",value);
//    _moviePosition = slider->getValue() * _decoder->getDuration();
//    _decoder->setPosition(_moviePosition);
    
    if (slider) {
        CCLog("onSlideChanged == %f",slider->getValue() * _decoder->getDuration());
        if (isPlaying()) {
            setMoviePosition(slider->getValue() * _decoder->getDuration());
        }else{
            play();
            setMoviePosition(slider->getValue() * _decoder->getDuration());
        }
    }
}

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
    freeBufferedFrames();
    //position = MIN(_decoder->getDuration() - 1, MAX(0, position));

    pthread_mutex_lock(&m_vp_data_mutex);

    if (playing) {
        CCLog("position====%f",position);
        setDecoderPosition(position);
        setMoviePositionFromDecoder();
        play();
    } else {
        CCLog("position11111====%f",position);
        setDecoderPosition(position);
        vector<VPFrame*> frames;
        if (_decoder->isValidAudio() || _decoder->isValidVideo()) {
             frames = _decoder->decodeFrames(0);
        }
        if (frames.size() > 0) {
            addFrames(frames);
        }
        _disableUpdateHUD = false;
        setMoviePositionFromDecoder();
        presentFrame();
        updateHUD(0);
    }
    pthread_mutex_unlock(&m_vp_data_mutex);
}

void CAVideoPlayerController::dispearHUDView()
{
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0.3f);
    CAViewAnimation::setAnimationRepeatAutoreverses(true);
    _HUDView->setVisible(false);
    CAViewAnimation::commitAnimations();
}

void CAVideoPlayerController::showHUDView()
{
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0.3f);
    CAViewAnimation::setAnimationRepeatAutoreverses(true);
    _HUDView->setVisible(true);
    CAViewAnimation::commitAnimations();
}

float CAVideoPlayerController::getDuration()
{
    return _decoder->getDuration();
}

float CAVideoPlayerController::getPosition()
{
    return _moviePosition - _decoder->getStartTime();
}

void CAVideoPlayerController::setPosition(float pos)
{
    setMoviePosition(pos);
}
NS_CC_END
