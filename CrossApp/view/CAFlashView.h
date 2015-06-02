//
//  CAFlashView.h
//  CrossApp
//
//  Created by qiaoxin on 15-05-12.
//


#ifndef __CrossApp__CAFlashView__
#define __CrossApp__CAFlashView__

#include <iostream>
#include "CrossApp.h"
#include "support/gameswf/gameswf/gameswf.h"
#include "support/gameswf/gameswf/gameswf_player.h"
#include "support/gameswf/gameswf/gameswf_root.h"
#include "support/gameswf/gameswf/gameswf_types.h"
#include "support/gameswf/gameswf/gameswf_impl.h"

NS_CC_BEGIN

class CAView;

class CC_DLL CAFlashView : public CAView
{
public:
    CAFlashView();
    virtual ~CAFlashView();
    static CAFlashView* create(const std::string& file);
    bool initWithSWFFile(const std::string& file);
    virtual void onEnter();
    virtual void onExit();
    std::string movieName();
    void setMovieName(char *movieName);
    void stopAction();
    void runAction();
    bool Runing(){return m_bIsRuning;};
    void setRepeat(bool flag);
    bool getRepeat();
    void setScale(float scale);
protected:
    virtual void update(float dt);
    virtual void draw();
    void setScaleX(float scaleX);
    void setScaleY(float scaleY);
protected:
    gameswf::gc_ptr<gameswf::player>    m_pPlayer;
    gameswf::gc_ptr<gameswf::root>      m_pMovie;
    bool    m_bIsRuning;
    bool    m_bIsRepeat;
    GLfloat m_movieWidth;
    GLfloat m_movieHeight;
    GLfloat m_localScaleX;
    GLfloat m_localScaleY;
    GLfloat m_fScaleX;
    GLfloat m_fScaleY;
    int     m_nFrameCount;
};

NS_CC_END

#endif /* defined(__CrossApp__CAFlashView__) */
