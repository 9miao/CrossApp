//
//  CAVideoPlayerAudioManager.h
//  CrossApp
//
//  Created by dai xinping on 14-10-31.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#ifndef __CrossApp__CAVideoPlayerAudioManager__
#define __CrossApp__CAVideoPlayerAudioManager__

#include <string>
#include "CAVideoPlayerMacros.h"
#include "basics/CAObject.h"

using namespace CrossApp;

CA_VP_BEGIN

typedef bool (CAObject::*SEL_AudioManagerOutputBlock)(float *data, unsigned int numFrames, unsigned int numChannels);
#define audio_manager_selector(_SELECTOR) (SEL_DecoderInterruptCallback)(&_SELECTOR)
    
class VPAudioManager {    
        
public:
    VPAudioManager();
    ~VPAudioManager();
    
    static VPAudioManager* getInstance();
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(unsigned int, m_numOutputChannels, NumOutputChannels)
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(float, m_samplingRate, SamplingRate)
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(unsigned int, m_numBytesPerSample, NumBytesPerSample)
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(float, m_outputVolume, OutputVolume)
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(bool, m_playing, isPlaying);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_audioRoute, AudioRoute)
    
    CC_SYNTHESIZE(SEL_AudioManagerOutputBlock, m_outputBlock, OutputBlock)
    
public:
    bool activateAudioSession();
    void deactivateAudioSession();
    bool play();
    void pause();
};

CA_VP_END


#endif /* defined(__CrossApp__CAVideoPlayerAudioManager__) */
