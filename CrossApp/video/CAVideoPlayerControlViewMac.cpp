#include "CAVideoPlayerControlView.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"
#include "view/CAWindow.h"
#include "support/ccUTF8.h"
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
		pCtrlView->autorelease();
		return pCtrlView;
	}
	CC_SAFE_DELETE(pCtrlView);
	return pCtrlView;
}

CAVideoPlayerControlView* CAVideoPlayerControlView::createWithLayout(const DRectLayout& layout)
{
    CAVideoPlayerControlView* pCtrlView = new CAVideoPlayerControlView();
    if (pCtrlView && pCtrlView->initWithLayout(layout))
    {
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
    
    this->buildCtrlViews();
    
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
    m_glView = CAVideoPlayerView::createWithLayout(DRectLayout(0, 0, 0, 0, DRectLayout::L_R_T_B));
	m_glView->setColor(CAColor_black);
	this->addSubview(m_glView);

	// Bottom Panel Back
	CAImageView* bottomPanel = CAImageView::createWithLayout(DRectLayout(0, 0, 0, 188, DRectLayout::L_R_B_H));
    bottomPanel->setImage(CAImage::create("source_material/vdo_panel_bottom_bg.png"));
    m_glView->addSubview(bottomPanel);

    // Slider
    CAImage* barImage = CAImage::create("source_material/vdo_progress_bar.png");
    m_playSlider = CASlider::createWithLayout(DRectLayout(32, 32, 25, 56, DRectLayout::L_R_T_H));
    m_playSlider->setThumbTintImage(barImage);
    m_playSlider->addTargetForTouchUpSide(this, CAControl_selector(CAVideoPlayerControlView::onSlideChanged));
    m_playSlider->addTarget(this, CAControl_selector(CAVideoPlayerControlView::onSlideTouched));
    bottomPanel->addSubview(m_playSlider);

    // Play Pause Button
    CAImage* backImage = CAImage::create("source_material/vdo_pause.png");
    CAImage* backImage_h = CAImage::create("source_material/vdo_pause_down.png");
    m_playButton = CAButton::createWithLayout(DRectLayout(32, 56, 96, 56, DRectLayout::L_W_T_H), CAButtonTypeCustom);
    m_playButton->setImageForState(CAControlStateAll, backImage);
    m_playButton->setImageForState(CAControlStateHighlighted, backImage_h);
    m_playButton->addTarget(this, CAControl_selector(CAVideoPlayerControlView::onButtonPause), CAControlEventTouchUpInSide);
    bottomPanel->addSubview(m_playButton);
    
    // play time
    m_playTimeLabel = CALabel::createWithLayout(DRectLayout(120, 200, 96, 56, DRectLayout::L_W_T_H));
    m_playTimeLabel->setFontSize(32);
    m_playTimeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    m_playTimeLabel->setColor(ccc4(255, 255, 255, 255));
    m_playTimeLabel->setText("00:00 / 00:00");
    bottomPanel->addSubview(m_playTimeLabel);
    
    /*


	// Top Panel Back
	CAImageView* topPanel = NULL;
	do {
		CAImage* image = CAImage::create("source_material/vdo_panel_top_bg.png");
		topPanel = CAImageView::createWithFrame(DRect(0, 0, m_glView->getFrame().size.width, image->getContentSize().height));
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
		frame.size.height = backImage->getContentSize().height;
		frame.size.width = backImage->getContentSize().width;
		buttonBack = CAButton::createWithCenter(frame, CAButtonTypeCustom);
		buttonBack->setImageForState(CAControlStateAll, backImage);
		buttonBack->setImageForState(CAControlStateHighlighted, backImage_h);
		buttonBack->addTarget(this, CAControl_selector(CAVideoPlayerControlView::onButtonBack), CAControlEventTouchUpInSide);
		topPanel->addSubview(buttonBack);
	} while (0);

	// Title
	do {
		DRect r = DRectZero;
		r.origin.x = buttonBack->getFrame().origin.x * 2 + buttonBack->getFrame().size.width;
		r.origin.y = buttonBack->getFrame().origin.y;
		r.size.width = m_glView->getFrame().size.width - r.origin.x;
		r.size.height = buttonBack->getFrame().size.height;
		CALabel* title = CALabel::createWithFrame(r);
		title->setText(m_szTitle);
		title->setFontSize(42);
		title->setColor(ccc4(255, 255, 255, 255));
		title->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
		topPanel->addSubview(title);
	} while (0);
*/
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
