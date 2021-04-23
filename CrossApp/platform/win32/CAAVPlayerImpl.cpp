#include "../CAAVPlayerImpl.h"
#include "view/CAAVPlayerView.h"
#include "images/CAImage.h"
#include "basics/CAApplication.h"
#include "view/CAAlertView.h"
#include <map>


NS_CC_BEGIN


CAAVPlayerImpl::CAAVPlayerImpl(CAAVPlayer* Player)
: m_pPlayer(Player)
{
}

CAAVPlayerImpl::~CAAVPlayerImpl()
{
}

void CAAVPlayerImpl::setUrl(const std::string& url)
{
}

void CAAVPlayerImpl::setFilePath(const std::string& filePath)
{
}

void CAAVPlayerImpl::play()
{
	this->playWithRate(1.0f);
}

void CAAVPlayerImpl::playWithRate(float rate)
{
	MessageBoxA(nullptr, "windows平台暂无此功能，请在其他平台运行！", "温馨提示", MB_OK);
}

void CAAVPlayerImpl::pause()
{
}

float CAAVPlayerImpl::getDuration()
{
	return 0.0f;
}

float CAAVPlayerImpl::getCurrentTime()
{
	return 0.0f;
}

void CAAVPlayerImpl::stop()
{
}

void CAAVPlayerImpl::setCurrentTime(float current)
{
}

const DSize& CAAVPlayerImpl::getPresentationSize()
{
	return DSizeZero;
}

void CAAVPlayerImpl::onImage(const std::function<void(CAImage*)>& function)
{
}

CAImage* CAAVPlayerImpl::getFirstFrameImageWithFilePath(const std::string& filePath)
{
	return nullptr;
}

//void CAAVPlayerControllerImpl::showAVPlayerController(CAAVPlayer* player)
//{
//}



NS_CC_END
