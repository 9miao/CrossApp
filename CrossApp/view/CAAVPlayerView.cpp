
#include "CAAVPlayerView.h"
#include "platform/CAAVPlayerImpl.h"
#include "images/CAImageCache.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCRenderState.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
NS_CC_BEGIN

const std::string CAAVPlayer::PlaybackBufferEmpty("PlaybackBufferEmpty");
const std::string CAAVPlayer::PlaybackLikelyToKeepUp("PlaybackLikelyToKeepUp");

const std::string CAAVPlayer::PlayStatePause("PlayStatePause");
const std::string CAAVPlayer::PlayStatePlaying("PlayStatePlaying");
const std::string CAAVPlayer::PlayStatePlayback("PlayStatePlayback");

CAAVPlayer::CAAVPlayer()
{
    m_pImpl = new CAAVPlayerImpl(this);
}

CAAVPlayer::~CAAVPlayer()
{
    CC_SAFE_DELETE(m_pImpl);
}

CAAVPlayer *CAAVPlayer::createWithUrl(const std::string &uri)
{
    CAAVPlayer *pRet = new CAAVPlayer();
    if (pRet && pRet->initWithUrl(uri))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

CAAVPlayer *CAAVPlayer::createWithFilePath(const std::string &uri)
{
    CAAVPlayer *pRet = new CAAVPlayer();
    if (pRet && pRet->initWithFilePath(uri))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool CAAVPlayer::initWithUrl(const std::string& uri)
{
    m_pImpl->setUrl(uri);
    return true;
}

bool CAAVPlayer::initWithFilePath(const std::string& uri)
{
    m_pImpl->setFilePath(uri);
    return true;
}

void CAAVPlayer::play()
{
    m_pImpl->play();
}

void CAAVPlayer::pause()
{
    m_pImpl->pause();
}

void CAAVPlayer::stop()
{
    m_pImpl->stop();
}

float CAAVPlayer::getDuration()
{
    return m_pImpl->getDuration();
}

float CAAVPlayer::getCurrentTime()
{
    return m_pImpl->getCurrentTime();
}

void CAAVPlayer::setCurrentTime(float current)
{
    m_pImpl->setCurrentTime(current);
}

const DSize& CAAVPlayer::getPresentationSize()
{
    return m_pImpl->getPresentationSize();
}

void CAAVPlayer::onImage(const std::function<void(CAImage*)>& function)
{
    m_pImpl->onImage(function);
}

CAAVPlayerView::CAAVPlayerView()
: m_fX(0)
, m_fY(0)
, m_fWidth(0)
, m_fHeight(0)
, m_pPlayer(nullptr)
{

}

CAAVPlayerView::~CAAVPlayerView()
{
    if (m_pPlayer)
    {
        m_pPlayer->onImage(nullptr);
    }
    CC_SAFE_RELEASE(m_pPlayer);
}

CAAVPlayerView* CAAVPlayerView::createWithFrame(const DRect& rect)
{
    CAAVPlayerView * pRet = new CAAVPlayerView();
    if (pRet && pRet->initWithFrame(rect))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CAAVPlayerView* CAAVPlayerView::createWithCenter(const DRect& rect)
{
    CAAVPlayerView * pRet = new CAAVPlayerView();
    if (pRet && pRet->initWithCenter(rect))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CAAVPlayerView* CAAVPlayerView::createWithLayout(const CrossApp::DLayout &layout)
{
    CAAVPlayerView * pRet = new CAAVPlayerView();
    if (pRet && pRet->initWithLayout(layout))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}


CAAVPlayerView *CAAVPlayerView::create()
{
    CAAVPlayerView *pRet = new CAAVPlayerView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool CAAVPlayerView::init()
{
    return true;
}

CAAVPlayer* CAAVPlayerView::getPlayer()
{
    return m_pPlayer;
}

void CAAVPlayerView::setPlayer(CrossApp::CAAVPlayer *player)
{
    if (m_pPlayer)
    {
        m_pPlayer->onImage(nullptr);
    }
    
    if (player)
    {
        player->onImage([&](CAImage* image)
        {
            this->setImage(image);
        });
    }
    
    CC_SAFE_RETAIN(player);
    CC_SAFE_RELEASE(m_pPlayer);
    m_pPlayer = player;
}

void CAAVPlayerView::setImage(CAImage* image)
{
    CAView::setImage(image);
    if (image)
    {
        DRect rect = DRectZero;
        rect.size = image->getContentSize();
        this->setImageRect(rect);
    }
}

void CAAVPlayerView::updateImageRect()
{
    GLfloat x1,x2,y1,y2;
    x1 = 0;
    y1 = 0;
    x2 = m_obContentSize.width;
    y2 = m_obContentSize.height;
    
    if (m_pobImage)
    {
        GLfloat scaleX = m_obContentSize.width / m_pobImage->getContentSize().width;
        GLfloat scaleY = m_obContentSize.height / m_pobImage->getContentSize().height;
        GLfloat scale = MIN(scaleX, scaleY);
        GLfloat width = m_pobImage->getContentSize().width * scale;
        GLfloat height = m_pobImage->getContentSize().height * scale;
        x1 = (m_obContentSize.width - width) / 2;
        y1 = (m_obContentSize.height - height) / 2;
        x2 = x1 + width;
        y2 = y1 + height;
    }
    
    m_sQuad.bl.vertices = DPoint3D(x1, y1, m_fPointZ);
    m_sQuad.br.vertices = DPoint3D(x2, y1, m_fPointZ);
    m_sQuad.tl.vertices = DPoint3D(x1, y2, m_fPointZ);
    m_sQuad.tr.vertices = DPoint3D(x2, y2, m_fPointZ);
}


NS_CC_END
