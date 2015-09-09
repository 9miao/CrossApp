#include <vector>
#include <map>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include "CAVideoPlayerDecoder.h"
#include "SDL.h"

NS_CC_BEGIN

extern "C" 
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/pixdesc.h"
}



#pragma mark - static

static void FFLog(void* context, int level, const char* pszFormat, va_list args) 
{
	char szBuf[kMaxLogLen+1] = { 0 };
    vsnprintf(szBuf, kMaxLogLen, pszFormat, args);
	CCLog(szBuf);
}
    
static void avStreamFPSTimeBase(AVStream *st, float defaultTimeBase, float *pFPS, float *pTimeBase)
{
    float fps, timebase;
    
    if (st->time_base.den && st->time_base.num)
        timebase = av_q2d(st->time_base);
    else if(st->codec->time_base.den && st->codec->time_base.num)
        timebase = av_q2d(st->codec->time_base);
    else
        timebase = defaultTimeBase;
    
    if (st->codec->ticks_per_frame != 1) {
        printf("WARNING: st.codec.ticks_per_frame=%d", st->codec->ticks_per_frame);
        //timebase *= st->codec->ticks_per_frame;
    }
    
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

static std::vector<int> collectStreams(AVFormatContext *formatCtx, enum AVMediaType codecType)
{
    std::vector<int> ret;
    for (int i=0; i<formatCtx->nb_streams; ++i) {
        if (codecType == formatCtx->streams[i]->codec->codec_type) {
            ret.push_back(i);
        }
    }
    return ret;
}

static void copyFrameData(unsigned char *src, unsigned char **dst, int linesize, int width, int height, int *dstLength)
{
    if (dst) {
        width = MIN(linesize, width);
        *dst = (unsigned char*)malloc(width * height);
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

static bool isNetworkPath (std::string path)
{
    std::size_t found = path.find(":");
    if (found == std::string::npos) {
        return false;
    }
    std::string scheme = path.substr(0, found);
    if (path == "file") {
        return false;
    }
    return true;
}

#pragma mark - VPFrame

static SDL_AudioSpec s_audioSpec;

VPFrame::VPFrame()
: m_duration(0)
, m_position(0)
, m_type(kFrameTypeVideo)
{

}

VPFrame::~VPFrame()
{
    
}


#pragma mark - VPAudioFrame

VPAudioFrame::VPAudioFrame()
: m_data(NULL)
, m_dataLength(0)
{
    m_type = kFrameTypeAudio;
}

VPAudioFrame::~VPAudioFrame()
{
    if (m_data) {
        delete [] m_data;
    }
}

#pragma mark - VPVideoFrame

VPVideoFrame::VPVideoFrame()
: m_height(0)
, m_width(0)
{
    m_type = kFrameTypeVideo;
}

VPVideoFrame::~VPVideoFrame()
{
    
}

#pragma mark - VPVideoFrameRGB

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

#pragma mark - VPVideoFrameYUV

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
    if (m_luma) {
        delete [] m_luma;
    }
    
    if (m_chromaB) {
        delete [] m_chromaB;
    }
    
    if (m_chromaR) {
        delete [] m_chromaR;
    }
}


#pragma mark - VPDecoder

VPDecoder::VPDecoder()
: _formatCtx(NULL)
, _videoCodecCtx(NULL)
, _audioCodecCtx(NULL)
, _videoFrame(NULL)
, _audioFrame(NULL)
, _videoStream(0)
, _audioStream(0)
, _pictureValid(false)
, _swsContext(NULL)
, _videoTimeBase(0)
, _audioTimeBase(0)
, _position(0)
, _swrContext(NULL)
, _swrBuffer(NULL)
, _swrBufferSize(0)
, _videoFrameFormat(kVideoFrameFormatRGB)
, _isNetwork(false)
, _disableDeinterlacing(false)
, _isEOF(false)
, _path(std::string(""))
, _fps(0)
, m_audioCallback(NULL)
, m_pAudioCallbackTarget(NULL)
, _picture(NULL)
{
    av_log_set_callback(FFLog);
    av_register_all();
    avformat_network_init();
    
    _picture = (AVPicture*)malloc(sizeof(AVPicture));
}

VPDecoder::~VPDecoder()
{
    closeFile();
    SDL_CloseAudio();
    
    free(_picture);
}

float VPDecoder::getDuration()
{
    if (!_formatCtx) {
        return 0;
    }
    if (_formatCtx->duration == AV_NOPTS_VALUE) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return FLT_MAX;
#else
		return MAXFLOAT;
#endif
    }
    return (float)_formatCtx->duration / AV_TIME_BASE;
}

float VPDecoder::getPosition()
{
    return _position;
}

void VPDecoder::setPosition(float seconds)
{
    _position = seconds;
    _isEOF = false;
    
    if (_videoStream != -1) {
        long long ts = (long long)(seconds / _videoTimeBase);
        avformat_seek_file(_formatCtx, _videoStream, ts, ts, ts, AVSEEK_FLAG_FRAME);
        avcodec_flush_buffers(_videoCodecCtx);
    }
    
    if (_audioStream != -1) {
        long long ts = (long long)(seconds / _audioTimeBase);
        avformat_seek_file(_formatCtx, _audioStream, ts, ts, ts, AVSEEK_FLAG_FRAME);
        avcodec_flush_buffers(_audioCodecCtx);
    }
}

unsigned int VPDecoder::getFrameWidth()
{
    return _videoCodecCtx ? _videoCodecCtx->width : 0;
}

unsigned int VPDecoder::getFrameHeight()
{
    return _videoCodecCtx ? _videoCodecCtx->height : 0;
}

float VPDecoder::getSampleRate()
{
    return _audioCodecCtx ? _audioCodecCtx->sample_rate : 0;
}

unsigned int VPDecoder::getAudioStreamsCount()
{
    return (unsigned int)_audioStreams.size();
}

int VPDecoder::getSelectedAudioStream()
{
    if (_audioStream == -1) {
        return -1;
    }
    std::vector<int>::iterator iter = _audioStreams.begin();
    int index = 0;
    while (iter != _audioStreams.end()) {
        if (*iter == _audioStream) {
            return index;
        }
        iter++;
        index++;
    }
    return -1;
}

void VPDecoder::setSelectedAudioStream(int selected)
{
    this->closeAudioStream();
    if (selected == -1) {
        _audioStream = -1;
    } else {
        int audioStream = _audioStreams[selected];
        VPError errCode = this->openAudioStream(audioStream);
        if (kErrorNone != errCode) {
			CCLog("%s, %d", __FUNCTION__, errCode);
        }
    }
}

bool VPDecoder::isValidAudio()
{
    return _audioStream != -1;
}

bool VPDecoder::isValidVideo()
{
    return _videoStream != -1;
}

float VPDecoder::getStartTime()
{
    if (_videoStream != -1) {
        AVStream *st = _formatCtx->streams[_videoStream];
        if (AV_NOPTS_VALUE != st->start_time) {
            return st->start_time * _videoTimeBase;
        }
        return 0;
    }
    
    if (_audioStream != -1) {
        AVStream *st = _formatCtx->streams[_audioStream];
        if (AV_NOPTS_VALUE != st->start_time) {
            return st->start_time * _audioTimeBase;
        }
        return 0;
    }
    
    return 0;
}

bool VPDecoder::isEOF()
{
    return _isEOF;
}

bool VPDecoder::getIsNetwork()
{
    return _isNetwork;
}

float VPDecoder::getFPS()
{
    return _fps;
}

VPDecoder* VPDecoder::createWithContentPath(const std::string& path, bool isNetPath)
{
    VPDecoder* vpdec = new VPDecoder();
    if (vpdec) {
        if (!vpdec->openFile(path, isNetPath)) {
            delete vpdec;
            return NULL;
        };
    }
    return vpdec;
}

bool VPDecoder::openFile(const std::string& path, bool isNetPath)
{
    _isNetwork = isNetPath;
    
    static bool needNetworkInit = true;
    if (needNetworkInit && _isNetwork) {
        needNetworkInit = false;
        avformat_network_init();
    }
    
    _path = path;
    
	VPError errCode = kErrorNone;
	
	try
	{
		errCode = openInput(path);
	}
	catch (...)
	{
	}

    if (errCode == kErrorNone) {
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

#pragma mark - VPDecoder - Private

VPError VPDecoder::openInput(std::string path)
{
	AVFormatContext *formatCtx = avformat_alloc_context();
    
	if (!formatCtx) {
		return kErrorOpenFile;
	}

    CCLog("avformat_open_input");
    int ret = avformat_open_input(&formatCtx, path.c_str(), NULL, NULL);
    CCLog("avformat_open_input, %d", ret);
    if (ret < 0) {
        if (formatCtx) {
            avformat_free_context(formatCtx);
        }
        return kErrorOpenFile;
    }

    CCLog("avformat_find_stream_info");
    ret = avformat_find_stream_info(formatCtx, NULL);
    CCLog("avformat_find_stream_info, %d", ret);
    if (ret < 0) {
        avformat_close_input(&formatCtx);
        return kErrorStreamInfoNotFound;
    }
    
    CCLog("av_dump_format");
    
    av_dump_format(formatCtx, 0, path.c_str(), false);
    
    _formatCtx = formatCtx;

    CCLog("openInput return");

    
    return kErrorNone;
}

VPError VPDecoder::openVideoStream()
{
    VPError errCode = kErrorStreamNotFound;

	_videoStream = -1;
	_videoStreams = collectStreams(_formatCtx, AVMEDIA_TYPE_VIDEO);
    std::vector<int>::iterator iter = _videoStreams.begin();
    while (iter != _videoStreams.end()) {
        if (0 == (_formatCtx->streams[*iter]->disposition & AV_DISPOSITION_ATTACHED_PIC)) {
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
    AVCodecContext *codecCtx = _formatCtx->streams[videoStream]->codec;
    
    // find the decoder for the video stream
    AVCodec *codec = avcodec_find_decoder(codecCtx->codec_id);
    if (!codec)
        return kErrorCodecNotFound;
    
    // inform the codec that we can handle truncated bitstreams -- i.e.,
    // bitstreams where frame boundaries can fall in the middle of packets
    //if(codec->capabilities & CODEC_CAP_TRUNCATED)
    //    _codecCtx->flags |= CODEC_FLAG_TRUNCATED;
    
    // open codec
    if (avcodec_open2(codecCtx, codec, NULL) < 0)
        return kErrorOpenCodec;
    
    _videoFrame = av_frame_alloc();
    
    if (!_videoFrame) {
        avcodec_close(codecCtx);
        return kErrorAllocateFrame;
    }
    
    _videoStream = videoStream;
    _videoCodecCtx = codecCtx;
    
    // determine fps
    
    AVStream *st = _formatCtx->streams[_videoStream];
    avStreamFPSTimeBase(st, 0.04, &_fps, &_videoTimeBase);
    
    CCLog("video codec size: %d:%d fps: %.3f tb: %f", getFrameWidth(), getFrameHeight(), _fps, _videoTimeBase);
    CCLog("video start time %f", st->start_time * _videoTimeBase);
    CCLog("video disposition %d", st->disposition);
    
    return kErrorNone;
}

static void audio_callback(void *userdata, Uint8 *stream, int len) 
{
    VPDecoder* decoder = (VPDecoder*)userdata;
    
    if (decoder) {
        decoder->onAudioCallback(stream, len);
    }
}

VPError VPDecoder::openAudioStream()
{
    VPError errCode = kErrorStreamNotFound;
    _audioStream = -1;
    _audioStreams = collectStreams(_formatCtx, AVMEDIA_TYPE_AUDIO);
    std::vector<int>::iterator iter = _audioStreams.begin();
    while (iter != _audioStreams.end()) {
        errCode = openAudioStream(*iter);
        if (errCode == kErrorNone) {
            break;
        }
        iter++;
    }
    return errCode;
}

VPError VPDecoder::openAudioStream(int audioStream)
{
    if (audioStream < 0 || audioStream >= _formatCtx->nb_streams) {
        return kErrorStreamNotFound;
    }
    
    AVCodecContext *codecCtx = _formatCtx->streams[audioStream]->codec;    
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
    wanted_spec.format = AUDIO_S16SYS; // 具体含义请查看“SDL宏定义”部分
    wanted_spec.silence = 0;            // 0指示静音
    wanted_spec.samples = 1024;  // 自定义SDL缓冲区大小
    wanted_spec.callback = audio_callback;        // 音频解码的关键回调函数
    wanted_spec.userdata = this;                    // 传给上面回调函数的外带数据
    
    /*  打开音频设备，这里使用一个while来循环尝试打开不同的声道数(由上面 */
    while (SDL_OpenAudio(&wanted_spec, &spec) < 0) {
        CCLog("SDL_OpenAudio (%d channels): %s\n", wanted_spec.channels, SDL_GetError());
		/*  SDL支持的声道数为 1, 2, 4, 6 */
		/*  后面我们会使用这个数组来纠正不支持的声道数目 */
		/*  next_nb_channels数组指定）直到成功打开，或者全部失败 */
		const int next_nb_channels[] = { 0, 0, 1, 6, 2, 6, 4, 6 };

        wanted_spec.channels = next_nb_channels[MIN(7, wanted_spec.channels)];
        if (!wanted_spec.channels) {
            CCLog("No more channel combinations to tyu, audio open failed\n");
            return kErrorUnknown;
        }
        wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
    }
    
    /* 检查实际使用的配置（保存在spec,由SDL_OpenAudio()填充） */
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
        
    _audioFrame = av_frame_alloc();
    
    if (!_audioFrame) {
        if (swrContext)
            swr_free(&swrContext);
        avcodec_close(codecCtx);
        return kErrorAllocateFrame;
    }
    
    _audioStream = audioStream;
    _audioCodecCtx = codecCtx;
    _swrContext = swrContext;
    
    AVStream *st = _formatCtx->streams[_audioStream];
    avStreamFPSTimeBase(st, 0.025, 0, &_audioTimeBase);
    
    CCLog("audio codec smr: %.d fmt: %d chn: %d tb: %f %s",
          _audioCodecCtx->sample_rate,
          _audioCodecCtx->sample_fmt,
          _audioCodecCtx->channels,
          _audioTimeBase,
          _swrContext ? "resample" : "");
    
    SDL_PauseAudio(0);
    
    _formatCtx->streams[audioStream]->discard = AVDISCARD_DEFAULT;
    
    return kErrorNone;
}


void VPDecoder::closeFile()
{
    this->closeAudioStream();
    this->closeVideoStream();

    
    std::vector<int>().swap(_videoStreams);
    std::vector<int>().swap(_audioStreams);

    if (_formatCtx) {
        
        _formatCtx->interrupt_callback.opaque = NULL;
        _formatCtx->interrupt_callback.callback = NULL;
        
        avformat_close_input(&_formatCtx);
        _formatCtx = NULL;
    }
}

void VPDecoder::closeVideoStream()
{
    _videoStream = -1;
    
    this->closeScaler();
    
    if (_videoFrame) {
        
        av_free(_videoFrame);
        _videoFrame = NULL;
    }
    
    if (_videoCodecCtx) {
        
        avcodec_close(_videoCodecCtx);
        _videoCodecCtx = NULL;
    }
}

void VPDecoder::closeAudioStream()
{
    _audioStream = -1;
    
    if (_swrBuffer) {
        
        free(_swrBuffer);
        _swrBuffer = NULL;
        _swrBufferSize = 0;
    }
    
    if (_swrContext) {
        
        swr_free(&_swrContext);
        _swrContext = NULL;
    }
    
    if (_audioFrame) {
        
        av_free(_audioFrame);
        _audioFrame = NULL;
    }
    
    if (_audioCodecCtx) {
        
        avcodec_close(_audioCodecCtx);
        _audioCodecCtx = NULL;
    }
}

void VPDecoder::closeScaler()
{
    if (_swsContext) {
        sws_freeContext(_swsContext);
        _swsContext = NULL;
    }
    
    if (_pictureValid) {
        avpicture_free(_picture);
        _pictureValid = false;
    }
}

bool VPDecoder::setupScaler()
{
    this->closeScaler();
    
    _pictureValid = avpicture_alloc(_picture,
                                    PIX_FMT_RGB24,
                                    _videoCodecCtx->width,
                                    _videoCodecCtx->height) == 0;
    
    if (!_pictureValid)
        return false;
    
    _swsContext = sws_getCachedContext(_swsContext,
                                       _videoCodecCtx->width,
                                       _videoCodecCtx->height,
                                       _videoCodecCtx->pix_fmt,
                                       _videoCodecCtx->width,
                                       _videoCodecCtx->height,
                                       PIX_FMT_RGB24,
                                       SWS_FAST_BILINEAR,
                                       NULL, NULL, NULL);
    
    return _swsContext != NULL;
}

VPVideoFrame* VPDecoder::handleVideoFrame()
{
    if (!_videoFrame->data[0])
        return NULL;
    
    VPVideoFrame *frame;
    
    if (_videoFrameFormat == kVideoFrameFormatYUV) {
        
        VPVideoFrameYUV * yuvFrame = new VPVideoFrameYUV();
        
        unsigned char* data = NULL;
        int dataLength = 0;
        
        copyFrameData(_videoFrame->data[0], &data,
                      _videoFrame->linesize[0],
                      _videoCodecCtx->width,
                      _videoCodecCtx->height,
                      &dataLength);
        yuvFrame->setLuma((char*)data);
        yuvFrame->setLumaLength(dataLength);

        copyFrameData(_videoFrame->data[1], &data,
                      _videoFrame->linesize[1],
                      _videoCodecCtx->width / 2,
                      _videoCodecCtx->height / 2,
                      &dataLength);
        yuvFrame->setChromaB((char*)data);
        yuvFrame->setChromaBLength(dataLength);

        copyFrameData(_videoFrame->data[2], &data,
                      _videoFrame->linesize[2],
                      _videoCodecCtx->width / 2,
                      _videoCodecCtx->height / 2,
                      &dataLength);
        yuvFrame->setChromaR((char*)data);
        yuvFrame->setChromaRLength(dataLength);

        frame = yuvFrame;
        
    }
	else 
	{
        
        if (!_swsContext && !setupScaler()) {
            
            CCLog("fail setup video scaler");
            return NULL;
        }
        
		sws_scale(_swsContext,
			(const uint8_t **)_videoFrame->data,
			_videoFrame->linesize,
			0,
			_videoCodecCtx->height,
			_picture->data,
			_picture->linesize);
        
        VPVideoFrameRGB *rgbFrame = new VPVideoFrameRGB();
        rgbFrame->setLineSize(_picture->linesize[0]);
        rgbFrame->setData((char*)_picture->data[0]);
        rgbFrame->setDataLength(rgbFrame->getLineSize() * _videoCodecCtx->height);
        
        frame = rgbFrame;
        rgbFrame->retain();
    }    
    
    frame->setWidth(_videoCodecCtx->width);
    frame->setHeight(_videoCodecCtx->height);
    frame->setPosition(av_frame_get_best_effort_timestamp(_videoFrame) * _videoTimeBase);
    
    const long long frameDuration = av_frame_get_pkt_duration(_videoFrame);
    if (frameDuration) {
        
        frame->setDuration(frameDuration * _videoTimeBase);
        frame->setDuration(frame->getDuration() + _videoFrame->repeat_pict * _videoTimeBase * 0.5);
        
        
    } 
	else 
	{
        // sometimes, ffmpeg unable to determine a frame duration
        // as example yuvj420p stream from web camera
        frame->setDuration( 1.0 / _fps );
    }    
    
#if 0
    CCLog("VFD: %.4f %.4f | %lld ", 
          frame->getPosition(), 
          frame->getDuration(), 
          av_frame_get_pkt_pos(_videoFrame));
#endif
    
    return frame;
}


VPAudioFrame* VPDecoder::handleAudioFrame()
{
    if (!_audioFrame->data[0])
        return NULL;
    
    const unsigned int numChannels = s_audioSpec.channels;
    int numFrames;
    
    void * audioData;
    
    if (_swrContext) {
        
        const unsigned int ratio = MAX(1, s_audioSpec.freq / _audioCodecCtx->sample_rate) * 
        MAX(1, s_audioSpec.channels / _audioCodecCtx->channels) * 2;
        
        const int bufSize = av_samples_get_buffer_size(NULL,
                                                       s_audioSpec.channels,
                                                       _audioFrame->nb_samples * ratio,
                                                       AV_SAMPLE_FMT_S16,
                                                       1);
        
        if (!_swrBuffer || _swrBufferSize < bufSize) {
            _swrBufferSize = bufSize;
            _swrBuffer = realloc(_swrBuffer, _swrBufferSize);
        }
        
        unsigned char *outbuf[2] = { (unsigned char*)_swrBuffer, 0};
        
//        if (s_audioSpec.samples != _audioFrame->nb_samples) {
//            if (swr_set_compensation(_swrContext,
//                                     (s_audioSpec.samples - _audioFrame->nb_samples)
//                                     * s_audioSpec.freq
//                                     / _audioFrame->sample_rate,
//                                     s_audioSpec.samples * s_audioSpec.freq
//                                     / _audioFrame->sample_rate) < 0) {
//                CCLog("swr_set_compensation() failed\n");
//                return NULL;
//            }
//        }
        
        numFrames = swr_convert(_swrContext,
                                outbuf,
//                                _swrBufferSize,
                                _audioFrame->nb_samples * ratio,
                                (const uint8_t **)_audioFrame->data,
                                _audioFrame->nb_samples);
        
        if (numFrames < 0) {
            CCLog("fail resample audio");
            return NULL;
        }
        
        int64_t delay = swr_get_delay(_swrContext, s_audioSpec.freq);
        if (delay > 0)
            CCLog("resample delay %lld", delay);
        
        audioData = _swrBuffer;
        
    } else {
        
        if (_audioCodecCtx->sample_fmt != AV_SAMPLE_FMT_S16) {
            CCAssert(false, "bucheck, audio format is invalid");
            return NULL;
        }
        
        audioData = _audioFrame->data[0];
        numFrames = _audioFrame->nb_samples;
    }
    
    const unsigned int numElements = numFrames * numChannels;
    int dataLength = numElements * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
    char* data = (char*)malloc(dataLength);
//    float scale = 1.0 / (float)INT16_MAX ;
//    vDSP_vflt16((short *)audioData, 1, (float*)data, 1, numElements);
//    vDSP_vsmul((float*)data, 1, &scale, (float*)data, 1, numElements);
    memcpy(data, audioData, dataLength);
    
    VPAudioFrame *frame = new VPAudioFrame();
    frame->setPosition( av_frame_get_best_effort_timestamp(_audioFrame) * _audioTimeBase );
    frame->setDuration( av_frame_get_pkt_duration(_audioFrame) * _audioTimeBase );
    frame->setData(data);
    frame->setDataLength(dataLength);
    
    if (frame->getDuration() == 0) {
        // sometimes ffmpeg can't determine the duration of audio frame
        // especially of wma/wmv format
        // so in this case must compute duration
        frame->setDuration( dataLength / (sizeof(float) * numChannels * s_audioSpec.freq) );
    }
    
#if 0
    CCLog("AFD: %.4f %.4f | %.4f ",
          frame->getPosition(), 
          frame->getDuration(),
          frame->getDataLength() / (8.0 * 44100.0));
#endif
    
    return frame;
}

void VPDecoder::onAudioCallback(unsigned char *stream, int len)
{
    if (m_pAudioCallbackTarget && m_audioCallback) {
        ((CAObject*)m_pAudioCallbackTarget->*m_audioCallback)(stream, len, s_audioSpec.channels);
    }
}

#pragma mark - VPDecoder Public

bool VPDecoder::setupVideoFrameFormat(VPVideoFrameFormat format)
{
    if (format == kVideoFrameFormatYUV &&
        _videoCodecCtx &&
        (_videoCodecCtx->pix_fmt == AV_PIX_FMT_YUV420P || _videoCodecCtx->pix_fmt == AV_PIX_FMT_YUVJ420P)) {
        
        _videoFrameFormat = kVideoFrameFormatYUV;
        return true;
    }
    
    _videoFrameFormat = kVideoFrameFormatRGB;
    return false;
}

std::vector<VPFrame*> VPDecoder::decodeFrames(float minDuration)
{
    std::vector<VPFrame*> result;

    if (_videoStream == -1 &&
        _audioStream == -1)
        return result;
    
    if (!_formatCtx) {
        return result;
    }
    
    AVPacket packet;
    
    float decodedDuration = 0;
    
    bool finished = false;
    
    while (!finished) {
        
        if (av_read_frame(_formatCtx, &packet) < 0) {
            _isEOF = true;
            break;
        }
        
        if (packet.stream_index == _videoStream) {
            
            int pktSize = packet.size;
            
            while (pktSize > 0) {
                
                int gotframe = 0;
                int len = avcodec_decode_video2(_videoCodecCtx,
                                                _videoFrame,
                                                &gotframe,
                                                &packet);
                
                if (len < 0) {
                    CCLog("decode video error, skip packet");
                    break;
                }
                
                if (gotframe) {
                    
                    if (!_disableDeinterlacing &&
                        _videoFrame->interlaced_frame) {
                        
                        avpicture_deinterlace((AVPicture*)_videoFrame,
                                              (AVPicture*)_videoFrame,
                                              _videoCodecCtx->pix_fmt,
                                              _videoCodecCtx->width,
                                              _videoCodecCtx->height);
                    }
                    
                    VPVideoFrame *frame = this->handleVideoFrame();
                    if (frame) {
                        
                        result.push_back((VPFrame*)frame);
                        
                        _position = frame->getPosition();
                        decodedDuration += frame->getDuration();
                        if (decodedDuration > minDuration)
                            finished = true;
                    }
                }
                
                if (0 == len)
                    break;
                
                pktSize -= len;
            }
            
        } else if (packet.stream_index == _audioStream) {
            
            int pktSize = packet.size;
            
            while (pktSize > 0) {
                
                int gotframe = 0;
                int len = avcodec_decode_audio4(_audioCodecCtx,
                                                _audioFrame,                                                
                                                &gotframe,
                                                &packet);
                
                if (len < 0) {
                    CCLog("decode audio error, skip packet");
                    break;
                }
                
                if (gotframe) {
                    
                    VPAudioFrame *frame = this->handleAudioFrame();
                    if (frame) {
                        
                        result.push_back((VPFrame*)frame);
                        
                        if (_videoStream == -1) {
                            
                            _position = frame->getPosition();
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


NS_CC_END