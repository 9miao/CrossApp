#include "CAVideoPlayerControlView.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"
#include "view/CAWindow.h"

NS_CC_BEGIN


CAVideoPlayerControlView::CAVideoPlayerControlView()
: m_glView(NULL)
, m_playButton(NULL)
, m_playSlider(NULL)
, m_playTimeLabel(NULL)
, m_szTitle(UTF8("\u672a\u547d\u540d"))
{
	CAScheduler::schedule(schedule_selector(CAVideoPlayerControlView::updatePlayUI), this, 0);
}

CAVideoPlayerControlView::~CAVideoPlayerControlView()
{
	CAScheduler::unschedule(schedule_selector(CAVideoPlayerControlView::updatePlayUI), this);
}

CAVideoPlayerControlView* CAVideoPlayerControlView::createWithFrame(const DRect& rect)
{
	CAVideoPlayerControlView* pCtrlView = new CAVideoPlayerControlView();
	if (pCtrlView && pCtrlView->initWithFrame(rect))
	{
		pCtrlView->buildCtrlViews();
		pCtrlView->autorelease();
		return pCtrlView;
    }
	CC_SAFE_DELETE(pCtrlView);
	return pCtrlView;
}

CAVideoPlayerControlView* CAVideoPlayerControlView::createWithCenter(const DRect& rect)
{
	CAVideoPlayerControlView* pCtrlView = new CAVideoPlayerControlView();
	if (pCtrlView && pCtrlView->initWithCenter(rect))
	{
		pCtrlView->buildCtrlViews();
		pCtrlView->autorelease();
		return pCtrlView;
	}
	CC_SAFE_DELETE(pCtrlView);
	return pCtrlView;
}


bool CAVideoPlayerControlView::init()
{
	if (!CAView::init())
	{
		return false;
	}

	this->setColor(ccc4(0, 0, 0, 255));
	return true;
}

void CAVideoPlayerControlView::initWithPath(const std::string& szPath)
{
	return m_glView->initWithPath(szPath);
}

void CAVideoPlayerControlView::initWithUrl(const std::string& szUrl)
{
	return m_glView->initWithUrl(szUrl);
}

bool CAVideoPlayerControlView::getShowBackButton()
{
    return m_bShowBackButton;
}

void CAVideoPlayerControlView::setShowBackButton(bool var)
{
    m_bShowBackButton = var;
}

void CAVideoPlayerControlView::buildCtrlViews()
{
	m_glView = CAVideoPlayerView::createWithFrame(getFrame());
	m_glView->setFrameOrigin(DPointZero);
	m_glView->setColor(ccc4(0, 0, 0, 0));
	this->insertSubview(m_glView, 1);

	// Bottom Panel Back
	CAImageView* bottomPanel = NULL;
	do {
		DRect frame = m_glView->getFrame();
		CAImage* image = CAImage::create("source_material/vdo_panel_bottom_bg.png");
		float width = m_glView->getFrame().size.width;
		float height = _px(image->getContentSize().height);
		bottomPanel = CAImageView::createWithFrame(DRect(0, frame.size.height - height, width, height));
		bottomPanel->setImage(image);
		m_glView->addSubview(bottomPanel);
	} while (0);

	// Slider 
	do {
		DRect frame = bottomPanel->getFrame();
		CAImage* backImage = CAImage::create("source_material/vdo_progress_back.png");
		CAImage* barImage = CAImage::create("source_material/vdo_progress_bar.png");
		m_playSlider = CASlider::createWithCenter(DRect(frame.size.width / 2, frame.size.height*0.3, frame.size.width * 0.9, _px(barImage->getContentSize().height)));
		m_playSlider->setMaxTrackTintImage(backImage);
		m_playSlider->setThumbTintImage(barImage);
		m_playSlider->setTrackHeight(_px(backImage->getContentSize().height));
		m_playSlider->addTargetForTouchUpSide(this, CAControl_selector(CAVideoPlayerControlView::onSlideChanged));
		m_playSlider->addTarget(this, CAControl_selector(CAVideoPlayerControlView::onSlideTouched));
		bottomPanel->addSubview(m_playSlider);
	} while (0);

	// Play Pause Button
	do {
		DRect frame = bottomPanel->getFrame();
		CAImage* backImage = CAImage::create("source_material/vdo_pause.png");
		CAImage* backImage_h = CAImage::create("source_material/vdo_pause_down.png");
		frame.origin.y = frame.size.height * 2 / 3;
		frame.origin.x = backImage->getContentSize().width;
		frame.size.height = _px(backImage->getContentSize().height);
		frame.size.width = _px(backImage->getContentSize().width);
		m_playButton = CAButton::createWithCenter(frame, CAButtonTypeCustom);
		m_playButton->setImageForState(CAControlStateAll, backImage);
		m_playButton->setImageForState(CAControlStateHighlighted, backImage_h);
		m_playButton->addTarget(this, CAControl_selector(CAVideoPlayerControlView::onButtonPause), CAControlEventTouchUpInSide);
		bottomPanel->addSubview(m_playButton);
	} while (0);

	// play time
	do {
		DRect frame = m_playButton->getFrame();
		DRect newFrame = DRectZero;
		m_playTimeLabel = CALabel::createWithFrame(DRectZero);
		m_playTimeLabel->setFontSize(32);
		m_playTimeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		m_playTimeLabel->setColor(ccc4(255, 255, 255, 255));
		newFrame.origin.x = frame.origin.x * 2 + frame.size.width;
		newFrame.origin.y = frame.origin.y;
		newFrame.size.width = m_playTimeLabel->getFontSize() * 20;
		newFrame.size.height = frame.size.height;
		m_playTimeLabel->setFrame(newFrame);
		m_playTimeLabel->setText("00:00 / 00:00");
		bottomPanel->addSubview(m_playTimeLabel);
	} while (0);

	// Top Panel Back
	CAImageView* topPanel = NULL;
	do {
		CAImage* image = CAImage::create("source_material/vdo_panel_top_bg.png");
		topPanel = CAImageView::createWithFrame(DRect(0, 0, m_glView->getFrame().size.width, _px(image->getContentSize().height)));
		topPanel->setImage(image);
		m_glView->addSubview(topPanel);
	} while (0);

	// Back Button
	CAButton* buttonBack = NULL;
	do {
		DRect frame = topPanel->getFrame();
		//        CAImage* backImage = CAImage::create("source_material/vdo_btn_back.png");
		//        CAImage* backImage_h = CAImage::create("source_material/vdo_btn_back_h.png");
		CAImage* backImage = CAImage::create("source_material/btn_left_blue.png");
		CAImage* backImage_h = CAImage::create("source_material/btn_left_white.png");
		frame.origin.y = frame.size.height / 3;
		frame.origin.x = frame.origin.y;
		frame.size.height = _px(backImage->getContentSize().height);
		frame.size.width = _px(backImage->getContentSize().width);
		buttonBack = CAButton::createWithCenter(frame, CAButtonTypeCustom);
		buttonBack->setImageForState(CAControlStateAll, backImage);
		buttonBack->setImageForState(CAControlStateHighlighted, backImage_h);
		buttonBack->addTarget(this, CAControl_selector(CAVideoPlayerControlView::onButtonBack), CAControlEventTouchUpInSide);
		topPanel->addSubview(buttonBack);
	} while (0);

	// Title
	do {
		DRect frame = buttonBack->getFrame();
		DRect r = DRectZero;
		r.origin.x = buttonBack->getFrame().origin.x * 2 + buttonBack->getFrame().size.width;
		r.origin.y = buttonBack->getFrame().origin.y;
		r.size.width = m_glView->getFrame().size.width - r.origin.x;
		r.size.height = buttonBack->getFrame().size.height;
		CALabel* title = CALabel::createWithFrame(r);
		title->setText(m_szTitle);
		title->setFontSize(_px(42));
		title->setColor(ccc4(255, 255, 255, 255));
		title->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		topPanel->addSubview(title);
	} while (0);

	updatePlayButton();
}

void CAVideoPlayerControlView::updatePlayButton()
{
	if (m_glView && m_glView->isPlaying()) 
	{
		CAImage* backImage = CAImage::create("source_material/vdo_pause.png");
		CAImage* backImage_h = CAImage::create("source_material/vdo_pause_down.png");
		m_playButton->setImageForState(CAControlStateAll, backImage);
		m_playButton->setImageForState(CAControlStateHighlighted, backImage_h);
	} 
	else 
	{
		CAImage* backImage = CAImage::create("source_material/vdo_play.png");
		CAImage* backImage_h = CAImage::create("source_material/vdo_play_down.png");
		m_playButton->setImageForState(CAControlStateAll, backImage);
		m_playButton->setImageForState(CAControlStateHighlighted, backImage_h);
	}
}

std::string CAVideoPlayerControlView::formatTimeInterval(float seconds, bool isLeft)
{
	seconds = MAX(0, seconds);

	int s = seconds;
	int m = s / 60;
	int h = m / 60;

	s = s % 60;
	m = m % 60;

	std::string format = (isLeft && seconds >= 0.5) ? "-" : "";
	char output[256] = { 0 };
	if (h != 0) {
		format.append("%d:%02d:%02d");
		sprintf(output, format.c_str(), h, m, s);
	}
	else {
		format.append("%02d:%02d");
		sprintf(output, format.c_str(), m, s);
	}

	return std::string(output);
}

void CAVideoPlayerControlView::onSlideTouched(CAControl* control, DPoint point)
{

}

void CAVideoPlayerControlView::onSlideChanged(CAControl* control, DPoint point)
{
	if (m_glView == NULL || m_playSlider == NULL)
		return;

	float moviePosition = m_playSlider->getValue() * m_glView->getDuration();
	m_glView->setPosition(moviePosition);
	m_glView->play();
}

void CAVideoPlayerControlView::onButtonPause(CAControl* control, DPoint point)
{
	if (m_glView == NULL)
		return;

	if (m_glView->isPlaying())
	{
		m_glView->pause();
	}
	else
	{
		m_glView->play();
	}
	updatePlayButton();
}

void CAVideoPlayerControlView::onButtonBack(CAControl* control, DPoint point)
{

}

void CAVideoPlayerControlView::updatePlayUI(float t)
{
	if (m_glView == NULL || m_playSlider == NULL || m_playTimeLabel == NULL)
		return;

	const float duration = m_glView->getDuration();
	const float position = m_glView->getPosition();

	m_playSlider->setValue(position / duration);
	m_playTimeLabel->setText(formatTimeInterval(position, false).append(" / ").append(formatTimeInterval(duration - 1, false)));

	updatePlayButton();
}

NS_CC_END
