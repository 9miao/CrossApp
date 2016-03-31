#include <vector>
#include <map>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include "CAVideoPlayerDecoder.h"

NS_CC_BEGIN

extern "C"
{
#include "SDL.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/pixdesc.h"
#include "libavutil/avutil.h"
}


//    extern void SDL_CloseAudio(void);
//    
//    extern void SDL_SetMainReady();
//    
//    extern int  SDL_Init(Uint32 flags);
//
//    extern const char * SDL_GetError();
//    
//    extern int SDL_OpenAudio(SDL_AudioSpec * desired, SDL_AudioSpec * obtained);
//    
//    extern void SDL_PauseAudio(int pause_on);
//    
//    extern void SDL_Quit(void);


    
static void avStreamFPSTimeBase(AVStream *st, float defaultTimeBase, float *pFPS, float *pTimeBase)
{
    float fps, timebase;
    
    if (st->time_base.den && st->time_base.num)
        timebase = av_q2d(st->time_base);
    else if(st->codec->time_base.den && st->codec->time_base.num)
        timebase = av_q2d(st->codec->time_base);
    else
        timebase = defaultTimeBase;
    
    if (st->avg_frame_rate.den && st->avg_frame_rate.num)
        fps = av_q2d(st->avg_frame_rate);
    else if (st->r_frame_rate.den && st->r_frame_rate.num)
        fps = av_q2d(st->r_frame_rate);
    else
        fps = 1.0 / timebase;
    
    if (pFPS)
        *pFPS = fps;
    if (pTimeBase)
        *pTimeBase = timebase;
}

static void copyFrameData(unsigned char *src, unsigned char **dst, int linesize, int width, int height, int *dstLength)
{
	if (dst) 
	{
        width = MIN(linesize, width);
        *dst = (unsigned char*)new char[width * height];
        if (dstLength) {
            *dstLength = width * height;
        }
        unsigned char* tmp = *dst;
        for (int i=0; i<height; ++i) {
            memcpy(tmp, src, width);
            tmp += width;
            src += linesize;
        }
    }
}

static SDL_AudioSpec s_audioSpec;

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
	av_register_all();
	avcodec_register_all();
    avformat_network_init();
}

VPDecoder::~VPDecoder()
{
    closeFile();
	avformat_network_deinit();
}


std::vector<int> VPDecoder::collectStreams(int codecType)
{
	std::vector<int> ret;

	AVMediaType atype = (AVMediaType)codecType;
	if (m_pFormatCtx)
	{
		for (int i = 0; i < m_pFormatCtx->nb_streams; ++i) {
			if (atype == m_pFormatCtx->streams[i]->codec->codec_type)
			{
				ret.push_back(i);
			}
		}
	}
	return ret;
}


float VPDecoder::getDuration()
{
	if (!m_pFormatCtx) {
        return 0;
    }
	if (m_pFormatCtx->duration == AV_NOPTS_VALUE) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return FLT_MAX;
#else
		return MAXFLOAT;
#endif
    }
	return (float)m_pFormatCtx->duration / AV_TIME_BASE;
}

float VPDecoder::getPosition()
{
    return m_fPosition;
}

void VPDecoder::setPosition(float seconds)
{
	m_fPosition = seconds;
    _isEOF = false;
    
	if (m_iVideoStream != -1) {
		long long ts = (long long)(seconds / m_fVideoTimeBase);
		avformat_seek_file(m_pFormatCtx, m_iVideoStream, 0, ts, ts, AVSEEK_FLAG_FRAME);
		avcodec_flush_buffers(m_pVideoCodecCtx);
    }
    
	if (m_iAudioStream != -1) {
		long long ts = (long long)(seconds / m_fAudioTimeBase);
		avformat_seek_file(m_pFormatCtx, m_fAudioTimeBase, 0, ts, ts, AVSEEK_FLAG_FRAME);
		avcodec_flush_buffers(m_pAudioCodecCtx);
    }
}

unsigned int VPDecoder::getFrameWidth()
{
	return m_pVideoCodecCtx ? m_pVideoCodecCtx->width : 0;
}

unsigned int VPDecoder::getFrameHeight()
{
	return m_pVideoCodecCtx ? m_pVideoCodecCtx->height : 0;
}

float VPDecoder::getSampleRate()
{
	return m_pAudioCodecCtx ? m_pAudioCodecCtx->sample_rate : 0;
}


bool VPDecoder::isValidAudio()
{
	return m_iAudioStream != -1;
}

bool VPDecoder::isValidVideo()
{
	return m_iVideoStream != -1;
}

void VPDecoder::enableAudio(bool on)
{
	SDL_PauseAudio(on?0:1);
}

float VPDecoder::getStartTime()
{
	if (m_iVideoStream != -1) {
		AVStream *st = m_pFormatCtx->streams[m_iVideoStream];
        if (AV_NOPTS_VALUE != st->start_time) {
			return st->start_time * m_fVideoTimeBase;
        }
        return 0;
    }
    
	if (m_iAudioStream != -1) {
		AVStream *st = m_pFormatCtx->streams[m_iAudioStream];
        if (AV_NOPTS_VALUE != st->start_time) {
			return st->start_time * m_fAudioTimeBase;
        }
        return 0;
    }
    
    return 0;
}

bool VPDecoder::isEOF()
{
    return _isEOF;
}

bool VPDecoder::openFile(const std::string& path)
{
	VPError errCode = openInput(path);
	
    if (errCode == kErrorNone) 
	{
        VPError videoErr = openVideoStream();
        VPError audioErr = openAudioStream();
        
		if (videoErr != kErrorNone && audioErr != kErrorNone) {
            errCode = videoErr;
        }
    }
    
    if (errCode != kErrorNone) {
        closeFile();
		CCLog("%s, %d", __FUNCTION__, errCode);
        return false;
    }
    
    return true;
}

int VPDecoder::interrupt_cb(void *ctx)
{
	VPDecoder* pDecoder = (VPDecoder*)ctx;

	if ( pDecoder->m_iTimeoutCnt > 150 )
	{
		pDecoder->m_iTimeoutCnt = 0;
		return 1;
	}
	pDecoder->m_iTimeoutCnt++;
	return 0;
}

VPError VPDecoder::openInput(std::string path)
{
	m_pFormatCtx = avformat_alloc_context();
    
	if (!m_pFormatCtx) {
		return kErrorOpenFile;
	}

	m_iTimeoutCnt = 0;
	m_pFormatCtx->interrupt_callback.callback = &VPDecoder::interrupt_cb;
	m_pFormatCtx->interrupt_callback.opaque = this;

	int ret = avformat_open_input(&m_pFormatCtx, path.c_str(), NULL, NULL);
    CCLog("avformat_open_input, %d", ret);
    if (ret < 0) 
	{
		avformat_free_context(m_pFormatCtx);
        return kErrorOpenFile;
    }

	ret = avformat_find_stream_info(m_pFormatCtx, NULL);
    CCLog("avformat_find_stream_info, %d", ret);
    if (ret < 0) 
	{
		avformat_close_input(&m_pFormatCtx);
        return kErrorStreamInfoNotFound;
    }
    
	av_dump_format(m_pFormatCtx, 0, path.c_str(), false);
    return kErrorNone;
}

VPError VPDecoder::openVideoStream()
{
    VPError errCode = kErrorStreamNotFound;

	m_iVideoStream = -1;
	std::vector<int> videoStreams = collectStreams(AVMEDIA_TYPE_VIDEO);
	std::vector<int>::iterator iter = videoStreams.begin();
	while (iter != videoStreams.end()) {
		if (0 == (m_pFormatCtx->streams[*iter]->disposition & AV_DISPOSITION_ATTACHED_PIC)) {
            errCode = openVideoStream(*iter);
            if (errCode == kErrorNone) {
                break;
            }
        } 
    }

    return errCode;
}

VPError VPDecoder::openVideoStream(int videoStream)
{    
    // get a pointer to the codec context for the video stream
    AVCodecContext *codecCtx = m_pFormatCtx->streams[videoStream]->codec;
    
    // find the decoder for the video stream
    AVCodec *codec = avcodec_find_decoder(codecCtx->codec_id);
    if (!codec)
        return kErrorCodecNotFound;
    
    // open codec
    if (avcodec_open2(codecCtx, codec, NULL) < 0)
        return kErrorOpenCodec;
    
    m_pVideoFrame = av_frame_alloc();
    
    if (!m_pVideoFrame) {
        avcodec_close(codecCtx);
        return kErrorAllocateFrame;
    }
    
    m_iVideoStream = videoStream;
    m_pVideoCodecCtx = codecCtx;
    
    // determine fps
    AVStream *st = m_pFormatCtx->streams[m_iVideoStream];
    avStreamFPSTimeBase(st, 0.04, &_fps, &m_fVideoTimeBase);
  
    return kErrorNone;
}

static void audio_callback(void *userdata, Uint8 *stream, int len) 
{
	((VPDecoder*)userdata)->onAudioCallback(stream, len);;
}

VPError VPDecoder::openAudioStream()
{
    VPError errCode = kErrorStreamNotFound;
    m_iAudioStream = -1;
	std::vector<int> audioStreams = collectStreams(AVMEDIA_TYPE_AUDIO);
	std::vector<int>::iterator iter = audioStreams.begin();
	while (iter != audioStreams.end())
    {
        errCode = openAudioStream(*iter);
        if (errCode == kErrorNone)
        {
            break;
        }
        iter++;
    }
    return errCode;
}

VPError VPDecoder::openAudioStream(int audioStream)
{
    if (audioStream < 0 || audioStream >= m_pFormatCtx->nb_streams) {
        return kErrorStreamNotFound;
    }
    
    AVCodecContext *codecCtx = m_pFormatCtx->streams[audioStream]->codec;    
    AVCodec *codec = avcodec_find_decoder(codecCtx->codec_id);
    SwrContext* swrContext;
    if(!codec)
        return kErrorCodecNotFound;
    
    if (avcodec_open2(codecCtx, codec, NULL) < 0)
        return kErrorOpenCodec;
    
    
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        CCLog("Could not initialize SDL - %s\n", SDL_GetError());
        exit(0);
    }
    
    SDL_AudioSpec wanted_spec, spec;
    long long wanted_channel_layout = 0;
    int wanted_nb_channels = codecCtx->channels;
        
    if (!wanted_channel_layout || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout)) {
        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
        wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
    }
    
    wanted_spec.channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
    wanted_spec.freq = codecCtx->sample_rate;
    if (wanted_spec.freq <= 0 || wanted_spec.channels <= 0) {
        CCLog("Invalid sample rate or channel count!\n");
        return kErrorUnknown;
    }
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.silence = 0;
    wanted_spec.samples = 1024;
    wanted_spec.callback = audio_callback;
    wanted_spec.userdata = this;

    while (SDL_OpenAudio(&wanted_spec, &spec) < 0) {
        CCLog("SDL_OpenAudio (%d channels): %s\n", wanted_spec.channels, SDL_GetError());

		const int next_nb_channels[] = { 0, 0, 1, 6, 2, 6, 4, 6 };

        wanted_spec.channels = next_nb_channels[MIN(7, wanted_spec.channels)];
        if (!wanted_spec.channels) {
            CCLog("No more channel combinations to tyu, audio open failed\n");
            return kErrorUnknown;
        }
        wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
    }
    
    if (spec.format != AUDIO_S16SYS) {
        CCLog("SDL advised audio format %d is not supported!\n", spec.format);
        return kErrorUnknown;
    }
    
    if (spec.channels != wanted_spec.channels) {
        wanted_channel_layout = av_get_default_channel_layout(spec.channels);
        if (!wanted_channel_layout) {
            CCLog("SDL advised channel count %d is not supported!\n", spec.channels);
            return kErrorUnknown;
        }
    }
    
    s_audioSpec = spec;

    if (1) {
        swrContext = swr_alloc_set_opts(NULL,
                                        av_get_default_channel_layout(spec.channels),
                                        AV_SAMPLE_FMT_S16,
                                        spec.freq,
                                        av_get_default_channel_layout(codecCtx->channels),
                                        codecCtx->sample_fmt,
                                        codecCtx->sample_rate,
                                        0,
                                        NULL);
        
        if (!swrContext || swr_init(swrContext)) {
            if (swrContext)
                swr_free(&swrContext);
            avcodec_close(codecCtx);
            return kErrorReSampler;
        }
    }
    
    m_pAudioFrame = av_frame_alloc();
    
    if (!m_pAudioFrame) {
        if (swrContext)
            swr_free(&swrContext);
        avcodec_close(codecCtx);
        return kErrorAllocateFrame;
    }
    
    m_iAudioStream = audioStream;
    m_pAudioCodecCtx = codecCtx;
    m_pSwrContext = swrContext;
    
    AVStream *st = m_pFormatCtx->streams[m_iAudioStream];
	avStreamFPSTimeBase(st, 0.025, 0, &m_fAudioTimeBase);
    
    CCLog("audio codec smr: %.d fmt: %d chn: %d tb: %f %s",
          m_pAudioCodecCtx->sample_rate,
          m_pAudioCodecCtx->sample_fmt,
          m_pAudioCodecCtx->channels,
		  m_fAudioTimeBase,
          m_pSwrContext ? "resample" : "");
    
    SDL_PauseAudio(0);
    
    m_pFormatCtx->streams[audioStream]->discard = AVDISCARD_DEFAULT;
    
    return kErrorNone;
}


void VPDecoder::closeFile()
{
    this->closeAudioStream();
    this->closeVideoStream();

    if (m_pFormatCtx) {
        
        m_pFormatCtx->interrupt_callback.opaque = NULL;
        m_pFormatCtx->interrupt_callback.callback = NULL;
        
		avformat_close_input(&m_pFormatCtx);
		avformat_free_context(m_pFormatCtx);
        
        m_pFormatCtx = NULL;
    }
}

void VPDecoder::closeVideoStream()
{
    m_iVideoStream = -1;
    
    this->closeScaler();
    
    if (m_pVideoFrame) {

		av_frame_free(&m_pVideoFrame);
        m_pVideoFrame = NULL;
    }
    
    if (m_pVideoCodecCtx) {
        
        avcodec_close(m_pVideoCodecCtx);
        m_pVideoCodecCtx = NULL;
    }
}

void VPDecoder::closeAudioStream()
{
    m_iAudioStream = -1;
    
	if (m_pswrBuffer) {
        
		free(m_pswrBuffer);
		m_pswrBuffer = NULL;
		m_uswrBufferSize = 0;
    }
    
    if (m_pSwrContext) {
        
        swr_free(&m_pSwrContext);
        m_pSwrContext = NULL;
    }
    
    if (m_pAudioFrame) {
        
		av_frame_free(&m_pAudioFrame);
        m_pAudioFrame = NULL;
    }
    
    if (m_pAudioCodecCtx) {
        
        avcodec_close(m_pAudioCodecCtx);
        m_pAudioCodecCtx = NULL;
    }

    SDL_CloseAudio();
    SDL_Quit();
}

void VPDecoder::closeScaler()
{
    if (m_pSwsContext) {
        sws_freeContext(m_pSwsContext);
        m_pSwsContext = NULL;
    }
    
	if (m_pAVPicture) {
		avpicture_free(m_pAVPicture);
		m_pAVPicture = NULL;
    }
}

bool VPDecoder::setupScaler()
{
    this->closeScaler();
    
	if (avpicture_alloc(m_pAVPicture,
		AV_PIX_FMT_RGB24,
		m_pVideoCodecCtx->width,
		m_pVideoCodecCtx->height))
		return false;

	m_pSwsContext = sws_getContext(m_pVideoCodecCtx->width,
		m_pVideoCodecCtx->height,
		m_pVideoCodecCtx->pix_fmt,
		m_pVideoCodecCtx->width,
		m_pVideoCodecCtx->height,
		AV_PIX_FMT_RGB24,
		SWS_FAST_BILINEAR, NULL, NULL, NULL);

    return m_pSwsContext != NULL;
}

VPVideoFrame* VPDecoder::handleVideoFrame()
{
	if (m_pVideoFrame==NULL || !m_pVideoFrame->data[0])
        return NULL;
    
    VPVideoFrame *frame = NULL;
    
	if (m_videoFrameFormat == kVideoFrameFormatYUV) {
        
        VPVideoFrameYUV * yuvFrame = new VPVideoFrameYUV();
        
        unsigned char* data = NULL;
        int dataLength = 0;
        
		copyFrameData(m_pVideoFrame->data[0], &data,
						m_pVideoFrame->linesize[0],
						m_pVideoCodecCtx->width,
						m_pVideoCodecCtx->height,
                      &dataLength);
        yuvFrame->setLuma((char*)data);
        yuvFrame->setLumaLength(dataLength);

		copyFrameData(m_pVideoFrame->data[1], &data,
						m_pVideoFrame->linesize[1],
						m_pVideoCodecCtx->width / 2,
						m_pVideoCodecCtx->height / 2,
                      &dataLength);
        yuvFrame->setChromaB((char*)data);
        yuvFrame->setChromaBLength(dataLength);

		copyFrameData(m_pVideoFrame->data[2], &data,
						m_pVideoFrame->linesize[2],
						m_pVideoCodecCtx->width / 2,
						m_pVideoCodecCtx->height / 2,
                      &dataLength);
        yuvFrame->setChromaR((char*)data);
        yuvFrame->setChromaRLength(dataLength);

        frame = yuvFrame;
    }
	else 
	{
        
		if (!m_pSwsContext && !setupScaler()) {
            
            CCLog("fail setup video scaler");
            return NULL;
        }
        
		sws_scale(m_pSwsContext,
			(const uint8_t **)m_pVideoFrame->data,
			m_pVideoFrame->linesize,
			0,
			m_pVideoCodecCtx->height,
			m_pAVPicture->data,
			m_pAVPicture->linesize);
        
        VPVideoFrameRGB *rgbFrame = new VPVideoFrameRGB();
		rgbFrame->setLineSize(m_pAVPicture->linesize[0]);
		rgbFrame->setData((char*)m_pAVPicture->data[0]);
        rgbFrame->setDataLength(rgbFrame->getLineSize() * m_pVideoCodecCtx->height);
        
        frame = rgbFrame;
    }    
    
	frame->setWidth(m_pVideoCodecCtx->width);
	frame->setHeight(m_pVideoCodecCtx->height);
	frame->setPosition(av_frame_get_best_effort_timestamp(m_pVideoFrame) * m_fVideoTimeBase);
    
	const long long frameDuration = av_frame_get_pkt_duration(m_pVideoFrame);
    if (frameDuration) {
        
        frame->setDuration(frameDuration * m_fVideoTimeBase);
		frame->setDuration(frame->getDuration() + m_pVideoFrame->repeat_pict * m_fVideoTimeBase * 0.5);
    } 
	else 
	{
        // sometimes, ffmpeg unable to determine a frame duration
        // as example yuvj420p stream from web camera
        frame->setDuration( 1.0 / _fps );
    }    

    return frame;
}


VPAudioFrame* VPDecoder::handleAudioFrame()
{
	if (!m_pAudioFrame->data[0])
        return NULL;
    
    const unsigned int numChannels = s_audioSpec.channels;
    int numFrames;
    
    void * audioData = NULL;
    
	if (m_pSwrContext) 
	{
        const unsigned int ratio = MAX(1, s_audioSpec.freq / m_pAudioCodecCtx->sample_rate) * MAX(1, s_audioSpec.channels / m_pAudioCodecCtx->channels) * 2;
        
        const int bufSize = av_samples_get_buffer_size(NULL,
                                                       s_audioSpec.channels,
													   m_pAudioFrame->nb_samples * ratio,
                                                       AV_SAMPLE_FMT_S16,
                                                       1);
        
		if (m_pswrBuffer==NULL || m_uswrBufferSize < bufSize) {
			m_uswrBufferSize = bufSize;
			m_pswrBuffer = realloc(m_pswrBuffer, m_uswrBufferSize);
        }
        
		unsigned char *outbuf[2] = { (unsigned char*)m_pswrBuffer, 0 };
        
		if (s_audioSpec.samples != m_pAudioFrame->nb_samples) 
		{
			if (swr_set_compensation(m_pSwrContext,
				(s_audioSpec.samples - m_pAudioFrame->nb_samples) * s_audioSpec.freq / m_pAudioFrame->sample_rate,
				s_audioSpec.samples * s_audioSpec.freq / m_pAudioFrame->sample_rate) < 0) {
				return NULL;
			}
		}
        
		numFrames = swr_convert(m_pSwrContext,
                                outbuf,
								m_pAudioFrame->nb_samples * ratio,
								(const uint8_t **)m_pAudioFrame->data,
								m_pAudioFrame->nb_samples);
        
        if (numFrames < 0) {
            CCLog("fail resample audio");
            return NULL;
        }
        
		swr_get_delay(m_pSwrContext, s_audioSpec.freq);
        
		audioData = m_pswrBuffer;
        
    } else {
        
		if (m_pAudioCodecCtx->sample_fmt != AV_SAMPLE_FMT_S16) {
			CCLog("bucheck, audio format is invalid");
            return NULL;
        }
        
		audioData = m_pAudioFrame->data[0];
		numFrames = m_pAudioFrame->nb_samples;
    }
    
    const unsigned int numElements = numFrames * numChannels;
    int dataLength = numElements * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
	char* data = new char[dataLength];
    memcpy(data, audioData, dataLength);
    
    VPAudioFrame *frame = new VPAudioFrame();
	frame->setPosition(av_frame_get_best_effort_timestamp(m_pAudioFrame) * m_fAudioTimeBase);
	frame->setDuration(av_frame_get_pkt_duration(m_pAudioFrame) * m_fAudioTimeBase);
    frame->setData(data);
    frame->setDataLength(dataLength);
    
    if (frame->getDuration() == 0) {
        // sometimes ffmpeg can't determine the duration of audio frame
        // especially of wma/wmv format
        // so in this case must compute duration
        frame->setDuration( dataLength / (sizeof(float) * numChannels * s_audioSpec.freq) );
    }
    return frame;
}

void VPDecoder::onAudioCallback(unsigned char *stream, int len)
{
    if (m_pAudioCallbackTarget && m_audioCallback) {
        ((CAObject*)m_pAudioCallbackTarget->*m_audioCallback)(stream, len, s_audioSpec.channels);
    }
}

bool VPDecoder::setupVideoFrameFormat(VPVideoFrameFormat format)
{
    if (format == kVideoFrameFormatYUV && m_pVideoCodecCtx && (m_pVideoCodecCtx->pix_fmt == AV_PIX_FMT_YUV420P || m_pVideoCodecCtx->pix_fmt == AV_PIX_FMT_YUVJ420P)) 
	{
		m_videoFrameFormat = kVideoFrameFormatYUV;
        return true;
    }
    
	m_videoFrameFormat = kVideoFrameFormatRGB;
    return false;
}

std::vector<VPFrame*> VPDecoder::decodeFrames(float minDuration)
{
    std::vector<VPFrame*> result;

    if (m_iVideoStream == -1 && m_iAudioStream == -1)
        return result;
    
	if (!m_pFormatCtx) {
        return result;
    }
    
    AVPacket packet;
    
    float decodedDuration = 0;
    
    bool finished = false;
    
    while (!finished) {
        
		if (av_read_frame(m_pFormatCtx, &packet) < 0) 
		{
			CCLog("decode av_read_frame < 0");
			_isEOF = true;
			break;
        }
		m_iTimeoutCnt = 0;
        
		if (packet.stream_index == m_iVideoStream) {
			
            int pktSize = packet.size;
            
				while (pktSize > 0) {
                
                int gotframe = 0;

				int len = avcodec_decode_video2(m_pVideoCodecCtx, m_pVideoFrame, &gotframe, &packet);
				if (len < 0) {
					CCLog("decode video error, skip packet");
                    break;
                }
                
                if (gotframe) 
				{
                    VPVideoFrame *frame = this->handleVideoFrame();
                    if (frame)
					{
                        result.push_back((VPFrame*)frame);
                        
						if (isValidVideo())
						{
							m_fPosition = frame->getPosition();
							decodedDuration += frame->getDuration();
							if (decodedDuration > minDuration)
								finished = true;
						}
                    }
                }
                
                if (0 == len)
                    break;
                
                pktSize -= len;
            }
		}
		else if (packet.stream_index == m_iAudioStream) {

            int pktSize = packet.size;
            
            while (pktSize > 0) {
                
                int gotframe = 0;
				int len = avcodec_decode_audio4(m_pAudioCodecCtx, m_pAudioFrame, &gotframe, &packet);
                
                if (len < 0) {
					CCLog("decode audio error, skip packet");
                    break;
                }
                
                if (gotframe) {
                    
                    VPAudioFrame *frame = this->handleAudioFrame();
                    if (frame) {
                        
                        result.push_back((VPFrame*)frame);
                        
						if (!isValidVideo() && isValidAudio())
						{
							m_fPosition = frame->getPosition();
							decodedDuration += frame->getDuration();
							if (decodedDuration > minDuration)
								finished = true;
						}
					}
                }
                
                if (0 == len)
                    break;
                
                pktSize -= len;
            }
        } 
        
        av_free_packet(&packet);
    }
    
    return result;
}


VPVideoFrame* VPDecoder::getFirstVideoFrame()
{
	if (m_iVideoStream == -1)
		return NULL;

	if (!m_pFormatCtx) {
		return NULL;
	}

	AVPacket packet;

	bool finished = false;

	while (!finished) {

		if (av_read_frame(m_pFormatCtx, &packet) < 0)
		{
			break;
		}

		if (packet.stream_index == m_iVideoStream) {

			int pktSize = packet.size;

			while (pktSize > 0) {

				int gotframe = 0;

				int len = avcodec_decode_video2(m_pVideoCodecCtx, m_pVideoFrame, &gotframe, &packet);
				if (len < 0) {
					break;
				}

				if (gotframe)
				{
					av_free_packet(&packet);
					return this->handleVideoFrame();
				}

				if (0 == len)
					break;

				pktSize -= len;
			}
		}
		av_free_packet(&packet);
	}

	return NULL;
}


NS_CC_END