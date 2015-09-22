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
#include "platform/CCCommon.h"


NS_CC_BEGIN

extern "C" 
{
    struct AVFormatContext;
    struct AVCodecContext;
    struct AVFrame;
    struct AVPicture;
    struct SwrContext;
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
    
} VPFrameType;

typedef enum 
{
    kVideoFrameFormatRGB,
    kVideoFrameFormatYUV,
    
} VPVideoFrameFormat;

class VPFrame
{
public:
    VPFrame();
    
	CC_SYNTHESIZE_PASS_BY_REF(VPFrameType, m_type, Type);
	CC_SYNTHESIZE_PASS_BY_REF(float, m_position, Position);
	CC_SYNTHESIZE_PASS_BY_REF(float, m_duration, Duration);
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


typedef void (CAObject::*SEL_DecoderAudioCallback)(unsigned char *stream, int len, int channels);
#define decoder_audio_selector(_SELECTOR) (SEL_DecoderAudioCallback)(&_SELECTOR)


class VPDecoder 
{
public:
    VPDecoder();
	virtual ~VPDecoder();
    
public:
    
    bool openFile(const std::string& path);
    
    void closeFile();

    bool setupVideoFrameFormat(VPVideoFrameFormat format);
    
    std::vector<VPFrame*> decodeFrames(float minDuration);
    
    void onAudioCallback(unsigned char *stream, int len);

    void setAudioCallback(CAObject* target, SEL_DecoderAudioCallback callBack)
    {
        m_pAudioCallbackTarget = target;
        m_audioCallback = callBack;
    }
    
    float getDuration();
    float getPosition();
    void setPosition(float seconds);
    float getSampleRate();
    unsigned int getFrameWidth();
    unsigned int getFrameHeight();

    bool isValidAudio();
    bool isValidVideo();

    float getStartTime();
    bool isEOF();

protected:
	std::vector<int> collectStreams(int codecType);
	VPError openInput(std::string path);
	VPError openVideoStream();
	VPError openVideoStream(int videoStream);
	VPError openAudioStream();
	VPError openAudioStream(int audioStream);

	void closeVideoStream();
	void closeAudioStream();

	void closeScaler();
	bool setupScaler();

	VPVideoFrame* handleVideoFrame();
	VPAudioFrame* handleAudioFrame();
    
private:
	CAObject* m_pAudioCallbackTarget;
	SEL_DecoderAudioCallback m_audioCallback;

    AVFormatContext *m_pFormatCtx;

    AVCodecContext *m_pVideoCodecCtx;
    AVCodecContext *m_pAudioCodecCtx;

    AVFrame *m_pVideoFrame;
    AVFrame *m_pAudioFrame;

    int m_iVideoStream;
    int m_iAudioStream;

    AVPicture *m_pAVPicture;

	SwsContext *m_pSwsContext;
	SwrContext *m_pSwrContext;

    float m_fVideoTimeBase;
    float m_fAudioTimeBase;
    float m_fPosition;
 
    void *m_pswrBuffer;
    unsigned int m_uswrBufferSize;

    VPVideoFrameFormat m_videoFrameFormat;

    bool _isEOF;
    float _fps;
};

NS_CC_END

#endif /* defined(__CrossApp__CAVideoPlayerDecoder__) */
