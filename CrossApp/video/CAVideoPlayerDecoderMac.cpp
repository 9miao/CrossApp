#include <vector>
#include <map>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include "CAVideoPlayerDecoder.h"

NS_CC_BEGIN

static void FFLog(void* context, int level, const char* pszFormat, va_list args)
{
	char szBuf[kMaxLogLen+1] = { 0 };
    vsnprintf(szBuf, kMaxLogLen, pszFormat, args);
	CCLog("%s", szBuf);
}


VPFrame::VPFrame()
: m_duration(0)
, m_position(0)
, m_type(kFrameTypeVideo)
{
}



VPAudioFrame::VPAudioFrame()
: m_data(NULL)
, m_dataLength(0)
{
    m_type = kFrameTypeAudio;
}

VPAudioFrame::~VPAudioFrame()
{
	CC_SAFE_DELETE_ARRAY(m_data);
}


VPVideoFrame::VPVideoFrame()
: m_height(0)
, m_width(0)
{
    m_type = kFrameTypeVideo;
}


VPVideoFrameRGB::VPVideoFrameRGB()
: m_data(NULL)
, m_dataLength(0)
, m_lineSize(0)
{
    m_format = kVideoFrameFormatRGB;
}

VPVideoFrameRGB::~VPVideoFrameRGB()
{
	CC_SAFE_DELETE_ARRAY(m_data);
}

VPVideoFrameYUV::VPVideoFrameYUV()
: m_chromaB(NULL)
, m_chromaBLength(0)
, m_chromaR(NULL)
, m_chromaRLength(0)
, m_luma(NULL)
, m_lumaLength(0)
{
    m_format = kVideoFrameFormatYUV;
}

VPVideoFrameYUV::~VPVideoFrameYUV()
{
	CC_SAFE_DELETE_ARRAY(m_luma);
	CC_SAFE_DELETE_ARRAY(m_chromaB);
	CC_SAFE_DELETE_ARRAY(m_chromaR);
}


VPDecoder::VPDecoder()
: m_pFormatCtx(NULL)
, m_pVideoCodecCtx(NULL)
, m_pAudioCodecCtx(NULL)
, m_pVideoFrame(NULL)
, m_pAudioFrame(NULL)
, m_iVideoStream(-1)
, m_iAudioStream(-1)
, m_pSwsContext(NULL)
, m_fVideoTimeBase(0)
, m_fAudioTimeBase(0)
, m_fPosition(0)
, m_pSwrContext(NULL)
, m_pswrBuffer(NULL)
, m_uswrBufferSize(0)
, m_videoFrameFormat(kVideoFrameFormatRGB)
, _isEOF(false)
, _fps(0)
, m_audioCallback(NULL)
, m_pAudioCallbackTarget(NULL)
, m_pAVPicture(NULL)
{

}

VPDecoder::~VPDecoder()
{
    closeFile();
}


std::vector<int> VPDecoder::collectStreams(int codecType)
{
	std::vector<int> ret;
	return ret;
}


float VPDecoder::getDuration()
{
	return 0;
}

float VPDecoder::getPosition()
{
    return m_fPosition;
}

void VPDecoder::setPosition(float seconds)
{
	m_fPosition = seconds;
}

unsigned int VPDecoder::getFrameWidth()
{
	return 0;
}

unsigned int VPDecoder::getFrameHeight()
{
	return 0;
}

float VPDecoder::getSampleRate()
{
	return 0;
}


bool VPDecoder::isValidAudio()
{
	return m_iAudioStream != -1;
}

bool VPDecoder::isValidVideo()
{
	return m_iVideoStream != -1;
}

float VPDecoder::getStartTime()
{
    return 0;
}

bool VPDecoder::isEOF()
{
    return _isEOF;
}

bool VPDecoder::openFile(const std::string& path)
{
    return true;
}

VPError VPDecoder::openInput(std::string path)
{
    return kErrorNone;
}

VPError VPDecoder::openVideoStream()
{
    return kErrorStreamNotFound;
}

VPError VPDecoder::openVideoStream(int videoStream)
{    
    return kErrorNone;
}

VPError VPDecoder::openAudioStream()
{
    return kErrorStreamNotFound;
}

VPError VPDecoder::openAudioStream(int audioStream)
{
    return kErrorNone;
}


void VPDecoder::closeFile()
{

}

void VPDecoder::closeVideoStream()
{

}

void VPDecoder::closeAudioStream()
{
}

void VPDecoder::closeScaler()
{
}

bool VPDecoder::setupScaler()
{
    return m_pSwsContext != NULL;
}

VPVideoFrame* VPDecoder::handleVideoFrame()
{
    return 0;
}


VPAudioFrame* VPDecoder::handleAudioFrame()
{
    return 0;
}

void VPDecoder::onAudioCallback(unsigned char *stream, int len)
{

}

bool VPDecoder::setupVideoFrameFormat(VPVideoFrameFormat format)
{

    return false;
}

std::vector<VPFrame*> VPDecoder::decodeFrames(float minDuration)
{
    std::vector<VPFrame*> result;
    return result;
}


NS_CC_END