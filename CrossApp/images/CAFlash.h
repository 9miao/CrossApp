//
//  CAFlash.h
//  CrossApp
//
//  Created by qiaoxin on 15-06-03.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CAFlash_H__
#define __CAFlash_H__

#include <iostream>
#include "CrossApp.h"
#include "support/gameswf/gameswf/gameswf.h"
#include "support/gameswf/gameswf/gameswf_player.h"
#include "support/gameswf/gameswf/gameswf_root.h"
#include "support/gameswf/gameswf/gameswf_types.h"
#include "support/gameswf/gameswf/gameswf_impl.h"

NS_CC_BEGIN
class CAImage;

class CC_DLL CAFlash : public CAObject
{
public:
    CAFlash();
    virtual ~CAFlash();
    static CAFlash* createWithFilePath(const std::string& filePath);
    bool initWithFilePath(const std::string& filePath);
    void  setIndex(int index);
    int   getIndex();
    int   getCounts();
    void  playRun();
    void  advance(float dt);
    void  display(kmMat4* matrix);
    float getWidth();
    float getHeight();
    float getLocalScaleX();
    float getLocalScaleY();
    float getScaleX();
    float getScaleY();
    float getFrameCount();
    void  setRunning(bool running);
    bool  isRunning();
    void  setRepeatForever(bool repeatForever);
    bool  isRepeatForever();
protected:
    GLfloat m_movieWidth;
    GLfloat m_movieHeight;
    GLfloat m_localScaleX;
    GLfloat m_localScaleY;
    GLfloat m_fScaleX;
    GLfloat m_fScaleY;
    int     m_nFrameCount;
    bool    m_bIsRunning;
    bool    m_bIsRepeatForever;
    gameswf::gc_ptr<gameswf::player>    m_pPlayer;
    gameswf::gc_ptr<gameswf::root>      m_pMovie;
};

NS_CC_END

#endif //__CAFlash_H__

