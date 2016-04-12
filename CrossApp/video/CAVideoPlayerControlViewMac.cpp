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
, m_bShowBackButton(false)
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

CAVideoPlayerControlView* CAVideoPlayerControlView::createWithLayout(const DLayout& layout)
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

void CAVideoPlayerControlView::setShowBackButton(bool val)
{
    m_bShowBackButton = val;
    if (m_backButton)
    {
        m_backButton->setVisible(val);
    }
}

bool CAVideoPlayerControlView::getShowBackButton()
{
    return m_bShowBackButton;
}

void CAVideoPlayerControlView::setFullPath(const std::string& szPath)
{
	return m_glView->setFullPath(szPath);
}

void CAVideoPlayerControlView::setUrl(const std::string& szUrl)
{
	return m_glView->setUrl(szUrl);
}

void CAVideoPlayerControlView::buildCtrlViews()
{
    m_glView = CAVideoPlayerView::createWithLayout(DLayoutFill);
	this->addSubview(m_glView);

	// Bottom Panel Back
	CAImageView* bottomPanel = CAImageView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_B_H(0, 188)));
    bottomPanel->setImage(CAImage::create("source_material/vdo_panel_bottom_bg.png"));
    m_glView->addSubview(bottomPanel);

    // Slider
    CAImage* barImage = CAImage::create("source_material/vdo_progress_bar.png");
    m_playSlider = CASlider::createWithLayout(DLayout(DHorizontalLayout_L_R(32, 32), DVerticalLayout_T_H(25, 56)));
    m_playSlider->setThumbTintImage(barImage);
    m_playSlider->addTargetForTouchUpSide(this, CAControl_selector(CAVideoPlayerControlView::onSlideChanged));
    bottomPanel->addSubview(m_playSlider);

    // Play Pause Button
    CAImage* backImage = CAImage::create("source_material/vdo_pause.png");
    CAImage* backImage_h = CAImage::create("source_material/vdo_pause_down.png");
    m_playButton = CAButton::createWithLayout(DLayout(DHorizontalLayout_L_W(32, 56), DVerticalLayout_T_H(96, 56)), CAButtonTypeCustom);
    m_playButton->setImageForState(CAControlStateAll, backImage);
    m_playButton->setImageForState(CAControlStateHighlighted, backImage_h);
    m_playButton->addTarget(this, CAControl_selector(CAVideoPlayerControlView::onButtonPause), CAControlEventTouchUpInSide);
    bottomPanel->addSubview(m_playButton);
    
    // play time
    m_playTimeLabel = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_W(120, 200), DVerticalLayout_T_H(96, 56)));
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
