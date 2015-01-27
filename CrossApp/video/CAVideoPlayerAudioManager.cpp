//
//  CAVideoPlayerAudioManager.cpp
//  CrossApp
//
//  Created by dai xinping on 14-10-31.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#include "CAVideoPlayerAudioManager.h"

USING_CA_VP

VPAudioManager::VPAudioManager()
: m_audioRoute(std::string(""))
, m_outputBlock(NULL)
, m_numBytesPerSample(0)
, m_outputVolume(0)
, m_playing(false)
, m_numOutputChannels(0)
, m_samplingRate(0)
{
    
}

VPAudioManager::~VPAudioManager()
{
    
}

bool VPAudioManager::activateAudioSession()
{
    return false;
}

void VPAudioManager::deactivateAudioSession()
{
    
}

bool VPAudioManager::play()
{
    return false;
}

void VPAudioManager::pause()
{
    
}

VPAudioManager* VPAudioManager::getInstance()
{
    static VPAudioManager* s_instance = NULL;
    if (!s_instance) {
        s_instance = new VPAudioManager();
    }
    return s_instance;
}





