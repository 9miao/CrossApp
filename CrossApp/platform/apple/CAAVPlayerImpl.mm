

#include "platform/CAAVPlayerImpl.h"
#include "view/CAAVPlayerView.h"
#include "platform/CAFileUtils.h"
#include "images/CAImage.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#import "EAGLView.h"
#import <QuartzCore/QuartzCore.h>
#import <AVFoundation/AVFoundation.h>
#import <AVKit/AVKit.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#import <Cocoa/Cocoa.h>
#else
#import <UIKit/UIKit.h>
#endif



#define NATIVE_IMPL (NativeAVPlayer*)m_pNativeImpl

const char* ANIMATION_ID = "__AVPlayerLayer_play__";

static AVPlayerLayer* s_pAVPlayerLayer = nil;

static void playerLayer_play(AVPlayer* player, float rate, const std::function<void()>& callback)
{
    if (s_pAVPlayerLayer && s_pAVPlayerLayer.player)
    {
        [s_pAVPlayerLayer.player pause];
        CrossApp::CAScheduler::getScheduler()->unschedule(ANIMATION_ID, CrossApp::CAApplication::getApplication());
    }
    
    //const CGSize& size = s_pAVPlayerLayer.player.currentItem.presentationSize;
    
    if (s_pAVPlayerLayer == nil)
    {
        s_pAVPlayerLayer = [AVPlayerLayer playerLayerWithPlayer:player];
        [s_pAVPlayerLayer setVideoGravity:AVLayerVideoGravityResizeAspect];
        [s_pAVPlayerLayer setHidden:YES];
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        [s_pAVPlayerLayer setBackgroundColor:[NSColor blackColor].CGColor];
        [s_pAVPlayerLayer retain];
#else
        [s_pAVPlayerLayer setBackgroundColor:[UIColor blackColor].CGColor];
        UIWindow* window = [[UIApplication sharedApplication] keyWindow];
        [window.layer addSublayer:s_pAVPlayerLayer];
#endif
        
        
    }
    else
    {
        [s_pAVPlayerLayer setPlayer:player];
    }
    
    [s_pAVPlayerLayer setPosition:CGPointMake(2, 2)];
    [s_pAVPlayerLayer setBounds:CGRectMake(0, 0, 2, 2)];
    
    [player play];
    if (rate != 1.0f)
    {
        for (AVPlayerItemTrack *track in player.currentItem.tracks)
        {
            if ([track.assetTrack.mediaType isEqual:AVMediaTypeAudio])
            {
                track.enabled = YES;
            }
        }
        [player setRate:rate];
    }

    
    CrossApp::CAScheduler::getScheduler()->schedule([=](float dt)
    {
        callback();
    }, ANIMATION_ID, CrossApp::CAApplication::getApplication(), 1/40.f);
}

static void playerLayer_pause(AVPlayer* player)
{
    if (s_pAVPlayerLayer && player && s_pAVPlayerLayer.player == player)
    {
        [s_pAVPlayerLayer.player pause];
        [s_pAVPlayerLayer setPlayer:nil];
        CrossApp::CAScheduler::getScheduler()->unschedule(ANIMATION_ID, CrossApp::CAApplication::getApplication());
    }
}

static CrossApp::CAImage* get_first_frame_image_with_filePath(NSURL* url)
{
    CrossApp::CAImage* image = nullptr;
    
    do
    {
        AVURLAsset *asset = [[[AVURLAsset alloc] initWithURL:url options:nil] autorelease];
        AVAssetImageGenerator *assetGen = [[[AVAssetImageGenerator alloc] initWithAsset:asset] autorelease];
        
        assetGen.appliesPreferredTrackTransform = YES;
        CMTime time = CMTimeMakeWithSeconds(0.0, 600);
        NSError *error = nil;
        CMTime actualTime;
        CGImageRef videoImage = [assetGen copyCGImageAtTime:time actualTime:&actualTime error:&error];
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        NSSize size = NSMakeSize(CGImageGetWidth(videoImage), CGImageGetHeight(videoImage));
        NSImage* nsImage = [[NSImage alloc] initWithCGImage:videoImage size:size];
        NSData* nsData = [nsImage TIFFRepresentationUsingCompression:NSTIFFCompressionNone factor:1];
#else
        UIImage* uiImage = [UIImage imageWithCGImage:videoImage];
        NSData *nsData = UIImagePNGRepresentation(uiImage);
#endif
        
        ssize_t length = [nsData length];
        
        unsigned char* data = (unsigned char*)malloc(length);
        [nsData getBytes:data length:length];
        
        CrossApp::CAData* cross_data = new CrossApp::CAData();
        cross_data->fastSet(data, length);
        
        image = CrossApp::CAImage::createWithImageDataNoCache(cross_data);
        
        cross_data->release();
        CGImageRelease(videoImage);
        
    } while (0);
    
    return image;
}

@interface NativeAVPlayer : NSObject <AVPlayerItemOutputPullDelegate>
{
    AVPlayerItemVideoOutput*    _videoOutPut;
    bool                        _pasueTimer;
    NSTimer*                    _timer;
    CrossApp::DSize             _presentationSize;
    CrossApp::CAImage*          _firstFrameImage;
    std::string                 _PlayBufferLoadingStateTag;
}
@property (nonatomic, assign, setter=onPeriodicTime:) std::function<void(float, float)> periodicTime;
@property (nonatomic, assign, setter=onLoadedTime:) std::function<void(float, float)> loadedTime;
@property (nonatomic, assign, setter=onDidPlayToEndTime:) std::function<void()> didPlayToEndTime;
@property (nonatomic, assign, setter=onTimeJumped:) std::function<void()> timeJumped;
@property (nonatomic, assign, setter=onPlayBufferLoadingState:) std::function<void(const std::string&)> playBufferLoadingState;
@property (nonatomic, assign, setter=onPlayState:) std::function<void(const std::string&)> playState;
@property (nonatomic, assign, setter=onImage:) std::function<void(CrossApp::CAImage*)> onImage;
@property (nonatomic, assign, readonly) AVPlayer* player;

- (id)init;
- (void)setUrl:(std::string)url;
- (void)setFilePath:(std::string)filePath;
- (void)play;
- (void)playWithRate:(float)rate;
- (void)pause;
- (void)stop;
- (float)getDuration;
- (float)getCurrentTime;
- (void)setCurrentTime:(float)current;
- (const CrossApp::DSize&)getPresentationSize;
- (void)remove;
- (void)timerCurrentTime;
- (void)outputMediaData;
- (void)dealloc;
@end

@implementation NativeAVPlayer

- (id)init
{
    if (![super init])
    {
        return nil;
    }
    _pasueTimer = NO;
    _presentationSize = CrossApp::DSizeZero;
    _firstFrameImage = nullptr;
    _onImage = nullptr;
    //初始化输出流
    _videoOutPut = [[AVPlayerItemVideoOutput alloc] init];
    return self;
}

- (void)setUrl:(std::string)url
{

    NSURL* videoUrl = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
    [self setup:videoUrl load:NO];
}

- (void)setFilePath:(std::string)filePath
{
//    std::string resource;
//    std::string type;
//    size_t pos = filePath.find_last_of(".");
//    resource = filePath.substr(0, pos);
//    type = filePath.substr(pos + 1, filePath.length() - pos - 1);
//
//    NSString *path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:resource.c_str()]
//                                                   ofType:[NSString stringWithUTF8String:type.c_str()]];
//    ;
    std::string path = CrossApp::FileUtils::getInstance()->fullPathForFilename(filePath);
    NSURL* videoUrl = [NSURL fileURLWithPath:[NSString stringWithUTF8String:path.c_str()]];
    [self setup:videoUrl load:YES];
}

- (void)setup:(NSURL*)videoUrl load:(BOOL)load
{
    if (_player)
    {
        [self pause];
        [_player.currentItem removeOutput:_videoOutPut];
        [_player.currentItem removeObserver:self forKeyPath:@"status"];
        [_player.currentItem removeObserver:self forKeyPath:@"loadedTimeRanges"];
        [_player.currentItem removeObserver:self forKeyPath:@"playbackBufferEmpty"];
        [_player.currentItem removeObserver:self forKeyPath:@"playbackLikelyToKeepUp"];
        [[NSNotificationCenter defaultCenter] removeObserver:_player.currentItem];
        [_player removeObserver:self forKeyPath:@"rate"];
    }
    
    //初始化播放地址
    AVAsset* asset = [AVAsset assetWithURL:videoUrl];
    AVPlayerItem* item = [AVPlayerItem playerItemWithAsset:asset];

    if (_player == nil)
    {
        _player = [[AVPlayer playerWithPlayerItem:item] retain];
    }
    else
    {
        [_player replaceCurrentItemWithPlayerItem:item];
    }
    
    //添加输出流
    [_player.currentItem addOutput:_videoOutPut];
    
    //监听状态属性
    [_player.currentItem addObserver:self forKeyPath:@"status" options:NSKeyValueObservingOptionNew context:nil];
    //监听网络加载情况属性
    [_player.currentItem addObserver:self forKeyPath:@"loadedTimeRanges" options:NSKeyValueObservingOptionNew context:nil];
    //监听播放的区域缓存是否为空
    [_player.currentItem addObserver:self forKeyPath:@"playbackBufferEmpty" options:NSKeyValueObservingOptionNew context:nil];
    //缓存可以播放的时候调用
    [_player.currentItem addObserver:self forKeyPath:@"playbackLikelyToKeepUp" options:NSKeyValueObservingOptionNew context:nil];
    //监听暂停或者播放中
    [_player addObserver:self forKeyPath:@"rate" options:NSKeyValueObservingOptionNew context:nil];
    
    //监听当视频播放结束时
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(playToEndTimeNotification:) name:AVPlayerItemDidPlayToEndTimeNotification object:_player.currentItem];
    //监听当视频开始或快进或者慢进或者跳过某段播放
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(timeJumpedNotification:) name:AVPlayerItemTimeJumpedNotification object:_player.currentItem];
    

    
    /**************获取第一帧图片******************/
    if (load && _onImage)
    {
        CrossApp::CAImage* image = get_first_frame_image_with_filePath(videoUrl);
        _onImage(image);
        CC_SAFE_RETAIN(image);
        CC_SAFE_RELEASE(_firstFrameImage);
        _firstFrameImage = image;
        
    }
    
    _presentationSize = CrossApp::DSize(_player.currentItem.presentationSize.width, _player.currentItem.presentationSize.height);
}


- (void)play
{
    if (_timer)
    {
        [_timer invalidate];
        _timer = nil;
    }
    _timer = [NSTimer timerWithTimeInterval:0.05f target:self selector:@selector(timerCurrentTime) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:_timer forMode:NSRunLoopCommonModes];
    playerLayer_play(_player, 1.0f, [=]
    {
        [self outputMediaData];
    });
}

- (void)playWithRate:(float)rate
{
    if (_timer)
    {
        [_timer invalidate];
        _timer = nil;
    }
    _timer = [NSTimer timerWithTimeInterval:0.05f target:self selector:@selector(timerCurrentTime) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:_timer forMode:NSRunLoopCommonModes];
    playerLayer_play(_player, rate, [=]
    {
        [self outputMediaData];
    });
}

- (void)pause
{
    if (_timer)
    {
        [_timer invalidate];
        _timer = nil;
    }
    playerLayer_pause(_player);
}

- (void)stop
{
    if (_timer)
    {
        [_timer invalidate];
        _timer = nil;
    }
    [_player.currentItem seekToTime:kCMTimeZero completionHandler:^(BOOL finished) {
        [self timerCurrentTime];
    }];
    playerLayer_pause(_player);
    if (_onImage)
    {
        _onImage(_firstFrameImage);
    }
}

- (void)playToEndTimeNotification:(NSNotification*)notification
{
    [self pause];
    if (self.didPlayToEndTime)
    {
        self.didPlayToEndTime();
    }
}

- (void)timeJumpedNotification:(NSNotification*)notification
{
    if (self.timeJumped)
    {
        self.timeJumped();
    }
}

- (float)getDuration
{
    return CMTimeGetSeconds(_player.currentItem.duration);
}

- (float)getCurrentTime
{
    return CMTimeGetSeconds(_player.currentItem.currentTime);
}

- (void)setCurrentTime:(float)current
{
    _pasueTimer = YES;
    //seekToTime 跳转到指定的时间
    CMTimeScale timescale = _player.currentItem.currentTime.timescale;
    CMTime pointTime = CMTimeMake(current * timescale, timescale);
    
    [_player.currentItem seekToTime:pointTime toleranceBefore:kCMTimeZero toleranceAfter:kCMTimeZero completionHandler:^(BOOL finished) {
        _pasueTimer = NO;
    }];
}

- (const CrossApp::DSize&)getPresentationSize
{
    return _presentationSize;
}

- (void)timerCurrentTime
{
    if (_pasueTimer == NO && self.periodicTime)
    {
        self.periodicTime([self getCurrentTime], [self getDuration]);
    }
}

- (void)outputMediaData;
{
    do
    {
        CC_BREAK_IF(_onImage == nullptr);
        CC_BREAK_IF(_pasueTimer);
        
        CMTime itemTime = _player.currentItem.currentTime;
        CVPixelBufferRef pixelBuffer = [_videoOutPut copyPixelBufferForItemTime:itemTime itemTimeForDisplay:nil];
        
        CC_BREAK_IF(pixelBuffer == nil);
        CIImage *ciImage = [CIImage imageWithCVPixelBuffer:pixelBuffer];
        CVPixelBufferRelease(pixelBuffer);
        
        // imageWidth = width + ExtendedPixelsLeft + ExtendedPixelsRight;
        // imageHeight = height + ExtendedPixelsTop + ExtendedPixelsBottom;

//        CGFloat width = CVPixelBufferGetWidth(pixelBuffer);
//        CGFloat height = CVPixelBufferGetHeight(pixelBuffer);
                
        CIContext *ciContext = [CIContext contextWithOptions:nil];
        CGImageRef videoImage = [ciContext createCGImage:ciImage fromRect:ciImage.extent];
        
        size_t bitsPerComponent = CGImageGetBitsPerComponent(videoImage);
        size_t bitsPerPixel = CGImageGetBitsPerPixel(videoImage);
        size_t bytesPerRow = CGImageGetBytesPerRow(videoImage);
        
        CGDataProviderRef provider = CGImageGetDataProvider(videoImage);
        CFDataRef ref = CGDataProviderCopyData(provider);
        CGImageRelease(videoImage);
        
        unsigned char* data = (unsigned char*)CFDataGetBytePtr(ref);
        ssize_t length = (ssize_t)CFDataGetLength(ref);
        CGFloat pixelsWide = bytesPerRow / (bitsPerPixel / bitsPerComponent);
        CGFloat pixelsHigh = length / (bitsPerPixel / bitsPerComponent) / pixelsWide ;
        
        CrossApp::CAData* cross_data = new CrossApp::CAData();
        cross_data->copy(data, length);
        CFRelease(ref);
                
        CrossApp::CAImage* image = CrossApp::CAImage::createWithRawDataNoCache(cross_data, CrossApp::CAImage::PixelFormat::RGBA8888, pixelsWide, pixelsHigh);
        
        _onImage(image);
        cross_data->release();
    } while (0);
    
}

-(void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSString*,id> *)change context:(void *)context
{
    if ([keyPath isEqualToString:@"status"])
    {
        AVPlayerItemStatus itemStatus = (AVPlayerItemStatus)[[change objectForKey:NSKeyValueChangeNewKey] integerValue];
        
        switch (itemStatus) {
            case AVPlayerItemStatusUnknown:
            {
                NSLog(@"AVPlayerItemStatusUnknown");
            }
                break;
            case AVPlayerItemStatusReadyToPlay:
            {
                NSLog(@"AVPlayerItemStatusReadyToPlay");
            }
                break;
            case AVPlayerItemStatusFailed:
            {
                NSLog(@"AVPlayerItemStatusFailed");
            }
                break;
            default:
                break;
        }
    }
    else if ([keyPath isEqualToString:@"loadedTimeRanges"])
    {  //监听播放器的下载进度
        NSArray *loadedTimeRanges = [_player.currentItem loadedTimeRanges];
        CMTimeRange timeRange = [loadedTimeRanges.firstObject CMTimeRangeValue];// 获取缓冲区域
        float startSeconds = CMTimeGetSeconds(timeRange.start);
        float durationSeconds = CMTimeGetSeconds(timeRange.duration);
        float timeInterval = startSeconds + durationSeconds;// 计算缓冲总进度
        float totalDuration = CMTimeGetSeconds(_player.currentItem.duration);
        
        if (self.loadedTime)
        {
            self.loadedTime(timeInterval, totalDuration);
        }
    }
    else if ([keyPath isEqualToString:@"playbackBufferEmpty"])
    { //监听播放器在缓冲数据的状态
        
        if (_PlayBufferLoadingStateTag.compare("playbackBufferEmpty"))
        {
            NSLog(@"正在缓冲");
            _PlayBufferLoadingStateTag = "playbackBufferEmpty";
            if (self.playBufferLoadingState)
            {
                self.playBufferLoadingState(CrossApp::CAAVPlayer::PlaybackBufferEmpty);
            }
        }
    }
    else if ([keyPath isEqualToString:@"playbackLikelyToKeepUp"])
    {
        if (_PlayBufferLoadingStateTag.compare("playbackLikelyToKeepUp"))
        {
            NSLog(@"缓冲达到可播放");
            _PlayBufferLoadingStateTag = "playbackLikelyToKeepUp";
            if (self.playBufferLoadingState)
            {
                self.playBufferLoadingState(CrossApp::CAAVPlayer::PlaybackLikelyToKeepUp);
            }
        }
    }
    else if ([keyPath isEqualToString:@"rate"])
    {//当rate==0时为暂停,rate==1时为播放,当rate等于负数时为回放
        
        NSInteger rate = [[change objectForKey:NSKeyValueChangeNewKey] integerValue];
        
        if (rate == 0)
        {
            NSLog(@"暂停");
        }
        else if (rate == 1)
        {
            NSLog(@"播放");
        }
        else if (rate < 0)
        {
            NSLog(@"回放");
        }
        
        if (self.playState)
        {
            if (rate == 0)
            {
                self.playState(CrossApp::CAAVPlayer::PlayStatePause);
            }
            else if (rate == 1)
            {
                self.playState(CrossApp::CAAVPlayer::PlayStatePlaying);
            }
            else if (rate < 0)
            {
                self.playState(CrossApp::CAAVPlayer::PlayStatePlayback);
            }
        }
    }
    
}

- (void)dealloc
{
    if (_player)
    {
        [_player.currentItem removeOutput:_videoOutPut];
        [_player.currentItem removeObserver:self forKeyPath:@"status"];
        [_player.currentItem removeObserver:self forKeyPath:@"loadedTimeRanges"];
        [_player.currentItem removeObserver:self forKeyPath:@"playbackBufferEmpty"];
        [_player.currentItem removeObserver:self forKeyPath:@"playbackLikelyToKeepUp"];
        [[NSNotificationCenter defaultCenter] removeObserver:_player.currentItem];
        [_player removeObserver:self forKeyPath:@"rate"];
        [_player release];
    }
    [_videoOutPut release];
    CC_SAFE_RELEASE(_firstFrameImage);
    [super dealloc];
}

- (void)remove
{
    [self pause];
    self.periodicTime = nullptr;
    self.loadedTime = nullptr;
    self.didPlayToEndTime = nullptr;
    self.timeJumped = nullptr;
    self.playBufferLoadingState = nullptr;
    self.playState = nullptr;
    self.onImage = nullptr;
    [self autorelease];
}

@end

/*
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
@interface NativeAVPlayerView: AVPlayerView
{}
- (void)close;
@end

@implementation NativeAVPlayerView
- (void)close
{
    [self.player pause];
    [self removeFromSuperview];
}
@end
#endif
*/
NS_CC_BEGIN

//static std::map<CAAVPlayer*, NativeAVPlayer*> s_map;

CAImage* CAAVPlayerImpl::getFirstFrameImageWithFilePath(const std::string& filePath)
{
    if (filePath.length() == 0)
    {
        return nullptr;
    }
    
    std::string resource;
    std::string type;
    size_t pos = filePath.find_last_of(".");
    resource = filePath.substr(0, pos);
    type = filePath.substr(pos + 1, filePath.length() - pos - 1);
    
    NSString *path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:resource.c_str()]
                                                     ofType:[NSString stringWithUTF8String:type.c_str()]];
    
    NSURL* url = [NSURL fileURLWithPath:path];
    
    return get_first_frame_image_with_filePath(url);
}

CAAVPlayerImpl::CAAVPlayerImpl(CAAVPlayer* player)
: m_pPlayer(player)
{
    m_pNativeImpl = [[NativeAVPlayer alloc] init];
    
    //s_map[m_pPlayer] = NATIVE_IMPL;
    
    [NATIVE_IMPL onPeriodicTime:[&](float currTime, float duration)
    {
        if (m_pPlayer->m_obPeriodicTime)
        {
            m_pPlayer->m_obPeriodicTime(currTime, duration);
        }
    }];
    
    [NATIVE_IMPL onLoadedTime:[&](float currTime, float duration)
     {
         if (m_pPlayer->m_obLoadedTime)
         {
             m_pPlayer->m_obLoadedTime(currTime, duration);
         }
     }];
    
    [NATIVE_IMPL onDidPlayToEndTime:[&]
    {
        if (m_pPlayer->m_obDidPlayToEndTime)
        {
            m_pPlayer->m_obDidPlayToEndTime();
        }
    }];
    
    [NATIVE_IMPL onTimeJumped:[&]
     {
         if (m_pPlayer->m_obTimeJumped)
         {
             m_pPlayer->m_obTimeJumped();
         }
     }];
    
    [NATIVE_IMPL onPlayBufferLoadingState:[&](const std::string& var)
     {
         if (m_pPlayer->m_obPlayBufferLoadingState)
         {
             m_pPlayer->m_obPlayBufferLoadingState(var);
         }
     }];
    
    [NATIVE_IMPL onPlayState:[&](const std::string& var)
     {
         if (m_pPlayer->m_obPlayState)
         {
             m_pPlayer->m_obPlayState(var);
         }
     }];
}

CAAVPlayerImpl::~CAAVPlayerImpl()
{
    //s_map.erase(m_pPlayer);
    [NATIVE_IMPL remove];
}

void CAAVPlayerImpl::setUrl(const std::string& url)
{
    [NATIVE_IMPL setUrl:url];
}

void CAAVPlayerImpl::setFilePath(const std::string& filePath)
{
    [NATIVE_IMPL setFilePath:filePath];
}

void CAAVPlayerImpl::play()
{
    [NATIVE_IMPL play];
}

void CAAVPlayerImpl::playWithRate(float rate)
{
    [NATIVE_IMPL playWithRate:rate];
}

void CAAVPlayerImpl::pause()
{
    [NATIVE_IMPL pause];
}

void CAAVPlayerImpl::stop()
{
    [NATIVE_IMPL stop];
}

float CAAVPlayerImpl::getDuration()
{
    return [NATIVE_IMPL getDuration];
}

float CAAVPlayerImpl::getCurrentTime()
{
    return [NATIVE_IMPL getCurrentTime];
}

void CAAVPlayerImpl::setCurrentTime(float current)
{
    [NATIVE_IMPL setCurrentTime:current];
}

const DSize& CAAVPlayerImpl::getPresentationSize()
{
    return [NATIVE_IMPL getPresentationSize];
}

void CAAVPlayerImpl::onImage(const std::function<void(CAImage*)>& function)
{
    [NATIVE_IMPL onImage:function];
}
/*
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

void CAAVPlayerControllerImpl::showAVPlayerController(CAAVPlayer* player)
{
    NativeAVPlayerView* playerView = [[[NativeAVPlayerView alloc] init] autorelease];
    [playerView setPlayer:s_map[player].player];
    NSView* contentView = [[NSApplication sharedApplication] mainWindow].contentView;
    [playerView setFrame:contentView.bounds];
    [contentView addSubview:playerView];
    
    NSButton* closeBtn = [[[NSButton alloc] initWithFrame:NSMakeRect(0, [[EAGLView sharedEGLView] getHeight] - 40, 80, 40)] autorelease];
    [closeBtn setTitle:@"Done"];
    [playerView addSubview:closeBtn];
    [closeBtn setTarget:playerView];
    [closeBtn setAction:@selector(close)];
}

#else

void CAAVPlayerControllerImpl::showAVPlayerController(CAAVPlayer* player)
{
    AVPlayerViewController* playerViewController = [[[AVPlayerViewController alloc] init] autorelease];
    [playerViewController setPlayer:s_map[player].player];
    UIWindow* window = [[UIApplication sharedApplication] keyWindow];
    [window.rootViewController presentViewController:playerViewController animated:YES completion:nil];
}

#endif
*/


NS_CC_END
