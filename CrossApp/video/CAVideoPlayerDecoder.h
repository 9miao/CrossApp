//
//  CAVideoPlayerDecoder.h
//  CrossApp
//
//  Created by dai xinping on 14-11-6.
//  Modified by zhujian on 15-9-6
//
//  Copyright (c) 2015年 http://www.9miao.com. All rights reserved.
//

#ifndef __CrossApp__CAVideoPlayerDecoder__
#define __CrossApp__CAVideoPlayerDecoder__

#include <map>
#include <vector>

#include "basics/CAObject.h"
#include "images/CAImage.h"
#include "CrossApp.h"

NS_CC_BEGIN

extern "C" 
{
    struct AVFormatContext;
    struct AVCodecContext;
    struct AVFrame;
    struct AVPicture;
    struct SwrContext;
    struct AVSubtitle;
    struct SwsContext;
}

typedef enum {
    
    kErrorNone,
    kErrorOpenFile,
    kErrorStreamInfoNotFound,
    kErrorStreamNotFound,
    kErrorCodecNotFound,
    kErrorOpenCodec,
    kErrorAllocateFrame,
    kErrorSetupScaler,
    kErrorReSampler,
    kErrorUnsupported,
    kErrorUnknown
    
} VPError;

typedef enum 
{
    kFrameTypeAudio,
    kFrameTypeVideo,
    kFrameTypeArtwork,
    kFrameTypeSubtitle,
    
} VPFrameType;

typedef enum 
{
    kVideoFrameFormatRGB,
    kVideoFrameFormatYUV,
    
} VPVideoFrameFormat;

class VPFrame : public CAObject 
{
public:
    VPFrame();
    ~VPFrame();
    
    CC_SYNTHESIZE_PASS_BY_REF(VPFrameType, m_type, Type)
    CC_SYNTHESIZE_PASS_BY_REF(float, m_position, Position)
    CC_SYNTHESIZE_PASS_BY_REF(float, m_duration, Duration)
};

class VPAudioFrame : public VPFrame
{
public:
    VPAudioFrame();
    ~VPAudioFrame();
    
    CC_SYNTHESIZE_PASS_BY_REF(unsigned int, m_dataLength, DataLength)
    CC_SYNTHESIZE(char*, m_data, Data)
};

class VPVideoFrame : public VPFrame
{
public:
    VPVideoFrame();
    ~VPVideoFrame();
    
    CC_SYNTHESIZE_PASS_BY_REF(VPVideoFrameFormat, m_format, Format)
    CC_SYNTHESIZE_PASS_BY_REF(unsigned int, m_width, Width);
    CC_SYNTHESIZE_PASS_BY_REF(unsigned int, m_height, Height);

};

class VPVideoFrameRGB : public VPVideoFrame 
{
public:
    VPVideoFrameRGB();
    ~VPVideoFrameRGB();
    
    CC_SYNTHESIZE_PASS_BY_REF(unsigned int, m_lineSize, LineSize);
    CC_SYNTHESIZE_PASS_BY_REF(unsigned int, m_dataLength, DataLength);
    CC_SYNTHESIZE(char*, m_data, Data); // rgb data
    
    CAImage* asImage();
};

class VPVideoFrameYUV : public VPVideoFrame
{
public:
    VPVideoFrameYUV();
    ~VPVideoFrameYUV();
    
    CC_SYNTHESIZE(char*, m_luma, Luma)
    CC_SYNTHESIZE_PASS_BY_REF(unsigned int, m_lumaLength, LumaLength)
    CC_SYNTHESIZE(char*, m_chromaB, ChromaB)
    CC_SYNTHESIZE_PASS_BY_REF(unsigned int, m_chromaBLength, ChromaBLength)
    CC_SYNTHESIZE(char*, m_chromaR, ChromaR)
    CC_SYNTHESIZE_PASS_BY_REF(unsigned int, m_chromaRLength, ChromaRLength)
    
};

class VPArtworkFrame : public VPFrame 
{
public:
    VPArtworkFrame();
    ~VPArtworkFrame();
    
    CC_SYNTHESIZE(char*, m_data, Data); // picture
    CC_SYNTHESIZE_PASS_BY_REF(unsigned int, m_dataLength, DataLength);
    
    CAImage* asImage();
};

class VPSubtitleFrame : public VPFrame 
{
public:
    VPSubtitleFrame();
    ~VPSubtitleFrame();
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_text, Text);
};

typedef bool (CAObject::*SEL_DecoderInterruptCallback)();
#define decoder_selector(_SELECTOR) (SEL_DecoderInterruptCallback)(&_SELECTOR)

typedef void (CAObject::*SEL_DecoderAudioCallback)(unsigned char *stream, int len, int channels);
#define decoder_audio_selector(_SELECTOR) (SEL_DecoderAudioCallback)(&_SELECTOR)


class VPDecoder 
{
public:
    VPDecoder();
    ~VPDecoder();
    
public:
    
    static VPDecoder* createWithContentPath(const std::string& path, std::string& error,bool isNetworkPath);
    
    bool openFile(const std::string& path, std::string& error,bool isNetworkPath);
    
    void closeFile();
    
    bool setupVideoFrameFormat(VPVideoFrameFormat format);
    
    std::vector<VPFrame*> decodeFrames(float minDuration); // VPFrame
    
    bool interruputDecoder();
    
    void onAudioCallback(unsigned char *stream, int len);
    
private:
    CAObject* m_pInterruptTarget;
    SEL_DecoderInterruptCallback m_interruptCallback;
    
    CAObject* m_pAudioCallbackTarget;
    SEL_DecoderAudioCallback m_audioCallback;
    
    
public:
    void setInterruputCallback(CAObject* target, SEL_DecoderInterruptCallback callBack)
    {
        m_pInterruptTarget = target;
        m_interruptCallback = callBack;
    }
    
    void setAudioCallback(CAObject* target, SEL_DecoderAudioCallback callBack)
    {
        m_pAudioCallbackTarget = target;
        m_audioCallback = callBack;
    }
    
public:
    float getDuration();
    float getPosition();
    void setPosition(float seconds);
    float getSampleRate();
    unsigned int getFrameWidth();
    unsigned int getFrameHeight();
    unsigned int getAudioStreamsCount();
    int getSelectedAudioStream();
    void setSelectedAudioStream(int selected);
    unsigned int getSubtitleStreamsCount();
    int getSelectedSubtitleStream();
    void setSelectedSubtitleStream(int selected);
    bool isValidAudio();
    bool isValidVideo();
    bool isValidSubtitles();
    const std::map<std::string, std::string>& getInfo();
    std::string getVideoStreamFormatName();
    float getStartTime();
    bool getIsNetwork();
    bool isDisableDeinterlacing();
    void setDisableDeinterlacing(bool disable);
    bool isEOF();
    std::string getPath();
    float getFPS();
    
private:
    AVFormatContext     *_formatCtx;
    AVCodecContext      *_videoCodecCtx;
    AVCodecContext      *_audioCodecCtx;
    AVCodecContext      *_subtitleCodecCtx;
    AVFrame             *_videoFrame;
    AVFrame             *_audioFrame;
    int                 _videoStream;
    int                 _audioStream;
    int                 _subtitleStream;
    AVPicture           *_picture;
    bool                _pictureValid;
    SwsContext          *_swsContext;
    float               _videoTimeBase;
    float               _audioTimeBase;
    float               _position;
    std::vector<int>         _videoStreams;
    std::vector<int>         _audioStreams;
    std::vector<int>         _subtitleStreams;
    SwrContext          *_swrContext;
    void                *_swrBuffer;
    unsigned int        _swrBufferSize;
    std::map<std::string, std::string> _info;
    VPVideoFrameFormat  _videoFrameFormat;
    unsigned int        _artworkStream;
    int                 _subtitleASSEvents;
    bool                _isNetwork;
    bool                _disableDeinterlacing;
    bool                _isEOF;
    std::string         _path;
    float               _fps;
    
private:
    VPError openInput(std::string path);
    VPError openVideoStream();
    VPError openVideoStream(int videoStream);
    VPError openAudioStream();
    VPError openAudioStream(int audioStream);
    VPError openSubtitleStream(int subTitleStream);
    void closeVideoStream();
    void closeAudioStream();
    void closeSubtitleStream();
    void closeScaler();
    bool setupScaler();
    VPVideoFrame* handleVideoFrame();
    VPAudioFrame* handleAudioFrame();
    VPSubtitleFrame* handleSubtitle(AVSubtitle *pSubtitle);
};

NS_CC_END

#endif /* defined(__CrossApp__CAVideoPlayerDecoder__) */
