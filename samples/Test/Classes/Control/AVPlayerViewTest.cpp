
#include "AVPlayerViewTest.h"

AVPlayerViewTest::AVPlayerViewTest():
m_pPreviewBtn(NULL)
,m_pAvplayer(NULL)
{
    this->setTitle("CAAVPlayer");
}

AVPlayerViewTest::~AVPlayerViewTest()
{

}

void AVPlayerViewTest::viewDidLoad()
{
    m_pPreviewBtn = CAButton::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_H_C(640, 0.3)), CAButton::Type::Custom);
    m_pPreviewBtn->setImageForState(CAControl::State::Normal, CAImage::create("image/start.png"));
    m_pPreviewBtn->setImageForState(CAControl::State::Highlighted, CAImage::create("image/start.png"));
    m_pPreviewBtn->setImageSize(DSize(46, 46));
    this->setTextTagAndBackgroundImage(m_pPreviewBtn, "", "image/preview.png");
    m_pPreviewBtn->setZOrder(200);
    m_pPreviewBtn->addTarget([=]
    {
        m_pPreviewBtn->setVisible(false);
        this->playVideo();
        
    }, CAButton::Event::TouchUpInSide);
    this->getView()->addSubview(m_pPreviewBtn);
}

void AVPlayerViewTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void AVPlayerViewTest::playVideo()
{
    if(m_pAvplayer)
    {
        m_pPreviewBtn->setVisible(false);
        m_pAvplayer->play();
        return;
    }
    m_pAvplayer = CAAVPlayer::createWithUrl("https://vd3.bdstatic.com/mda-kmrjft83jp9e60y3/v1-cae/sc/mda-kmrjft83jp9e60y3.mp4?v_from_s=nj_haokan_4469&auth_key=1615190283-0-0-f8f717c7a86cf702ddc7cbb516b75619&bcevod_channel=searchbox_feed&pd=1&pt=3&abtest=");
    
    m_pAvplayer->onDidPlayToEndTime([=]
    {
        m_pPreviewBtn->setVisible(true);
        m_pAvplayer->stop();
    });
    m_pAvplayer->onTimeJumped([=]
    {
        CCLog("进度调整");
    });
    
    CAAVPlayerView* playerView = CAAVPlayerView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_H_C(640, 0.3)));
    playerView->setPlayer(m_pAvplayer);
    this->getView()->addSubview(playerView);
    
    playerView->insertSubview(CAView::createWithLayout(DLayoutFill, CAColor4B::BLACK), -1);
    
    
    CASlider* slider = CASlider::createWithLayout(DLayout(DHorizontalLayout_L_R(100, 100), DVerticalLayout_H_C(100, 0.52)));
    slider->setValue(0);
    slider->setZOrder(100);
    this->getView()->addSubview(slider);
    slider->setMaxTrackTintImage(CAImage::CLEAR_IMAGE());
    slider->setTargetForTouchUpSide([=](float var)
    {
        float current = var * m_pAvplayer->getDuration();
        m_pAvplayer->setCurrentTime(current);
    });
    m_pAvplayer->onPeriodicTime([=](float current, float duration)
    {
        slider->setValue(current / duration);
    });
    
    CAProgress* progress = CAProgress::createWithLayout(DLayout(DHorizontalLayout_L_R(3, 3), DVerticalLayoutFill));
    progress->setZOrder(-1);
    slider->addSubview(progress);
    progress->setProgressTintImage(progress->getProgressTrackImage());
    progress->setProgressTintColor(CAColor4B::RED);
    progress->setProgressTrackColor(CAColor4B::YELLOW);
    progress->setProgress(0);
    m_pAvplayer->onLoadedTime([=](float current, float duration)
    {
       progress->setProgress(current / duration);
    });
    
    CAButton* btn0 = CAButton::create(CAButton::Type::Custom);
    btn0->setLayout(DLayout(DHorizontalLayout_L_W(30, 46), DVerticalLayout_H_C(46, 0.52)));
    this->setTextTagAndBackgroundImage(btn0, "暂停", "image/play.png");
    this->getView()->addSubview(btn0);
    btn0->setZOrder(100);
    btn0->addTarget([=]
    {
        const std::string& textTag = btn0->getTextTag();
        if (textTag.compare("播放") == 0)
        {
            this->setTextTagAndBackgroundImage(btn0, "暂停", "image/play.png");
            m_pAvplayer->play();
        }
        else if (textTag.compare("暂停") == 0)
        {
            this->setTextTagAndBackgroundImage(btn0, "播放", "image/start.png");
            m_pAvplayer->pause();
        }

    }, CAButton::Event::TouchUpInSide);
    
    CAButton* btn2 = CAButton::create(CAButton::Type::Custom);
    btn2->setLayout(DLayout(DHorizontalLayout_R_W(30, 46), DVerticalLayout_H_C(46, 0.52)));
//    btn2->setTitleForState(CAControl::State::Normal, "停止");
    btn2->setBackgroundImageForState(CAControl::State::Normal, CAImage::create("image/stop.png"), true);
    btn2->setBackgroundImageForState(CAControl::State::Highlighted, CAImage::create("image/stop.png"), true);
    this->getView()->addSubview(btn2);
    btn2->setZOrder(100);
    btn2->addTarget([=]
    {
        this->setTextTagAndBackgroundImage(btn0, "暂停", "image/play.png");
        m_pPreviewBtn->setVisible(true);
        m_pAvplayer->stop();
    }, CAButton::Event::TouchUpInSide);
    
    CAActivityIndicatorView* activity = CAActivityIndicatorView::createWithLayout(DLayoutFill);
    playerView->addSubview(activity);
    activity->stopAnimating();

    m_pAvplayer->onPlayState([=](const CAAVPlayer::PlayState& var)
    {
        CCLog("onPlayState %s", var.c_str());
        if (var == CAAVPlayer::PlayStatePlaying)
        {
        }
        else if (var == CAAVPlayer::PlayStatePause)
        {
        }
    });
    
    m_pAvplayer->onPlayBufferLoadingState([=](const CAAVPlayer::PlayBufferLoadingState& var)
    {
        CCLog("onPlayBufferLoadingState %s", var.c_str());
        if (var == CAAVPlayer::PlaybackBufferEmpty)
        {
           activity->startAnimating();
        }
        else if (var == CAAVPlayer::PlaybackLikelyToKeepUp)
        {
           activity->stopAnimating();
        }
    });
    
    m_pAvplayer->play();
}

void AVPlayerViewTest::setTextTagAndBackgroundImage(CAButton* btn, const std::string& textTag, const std::string& filePath)
{
    btn->setTextTag(textTag);
    btn->setBackgroundImageForState(CAControl::State::Normal, CAImage::create(filePath), false);
    btn->setBackgroundImageForState(CAControl::State::Highlighted, CAImage::create(filePath), false);
}

