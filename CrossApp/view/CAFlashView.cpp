//
//  CAFlashView.cpp
//  GSGGameSWF
//
//  Created by qiaoxin on 15-05-12.
//

#include "CrossApp.h"
#include "CAFlashView.h"
#include "../support/gameswf/gameswf/gameswf.h"
#include "../support/gameswf/gameswf/gameswf_player.h"
#include "../support/gameswf/gameswf/gameswf_root.h"
#include "../support/gameswf/gameswf/gameswf_types.h"
#include "../support/gameswf/gameswf/gameswf_impl.h"
#include "../support/gameswf/base/tu_file.h"


NS_CC_BEGIN


void CCGameSWF_fscommand_handler (gameswf::character* movie, const char* command, const char* arg)
{
    
}

tu_file* CCGameSWF_file_opener (const char* url_or_path)
{
    assert(url_or_path);
    unsigned long pSize = 0;
    unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(url_or_path, "rb", &pSize);
    return new tu_file(tu_file::memory_buffer, (int)pSize, data);
}

static void CCGameSWF_log_handler (bool error, const char* message)
{
    if (error) {
        CCLog("qiaoxin::ERROR:: %s",message);
    }else{
        //CCLog("qiaoxin:::: %s",message);
    }
}

CAFlashView::CAFlashView()
:m_fScaleX(1)
,m_fScaleY(1)
,m_bIsRuning(false)
,m_bIsRepeat(false)
,m_nFrameCount(0)
{
    
}

CAFlashView::~CAFlashView()
{
    this->stopAction();
    this->m_pMovie = NULL;
    this->m_pPlayer= NULL;
}

std::string CAFlashView::movieName()
{
    return this->m_pMovie->m_movie->m_name.c_str();
}

void CAFlashView::setMovieName(char *movieName)
{
    m_pMovie->m_movie->m_name = movieName;
}

CAFlashView* CAFlashView::create(const std::string& file)
{
    CAFlashView* swf = new CAFlashView();
    if ( swf && swf->initWithSWFFile(file)) {
        swf->autorelease();
        return swf;
    }
    return NULL;
}

bool CAFlashView::initWithSWFFile(const std::string& file)
{
    if(!CAView::init())
    {
        return false;
    }
    CCRect rect = CCRectZero;
    this->setImage(NULL);
    this->setImageRect(rect, false, rect.size);
    std::string pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(file.c_str());
    if (pathKey.size() == 0)
    {
        return false;
    }
    gameswf::register_file_opener_callback(&CCGameSWF_file_opener);
    gameswf::register_fscommand_callback(&CCGameSWF_fscommand_handler);
    gameswf::register_log_callback(&CCGameSWF_log_handler);
    gameswf::set_sound_handler(0);
    gameswf::render_handler* render = gameswf::create_render_handler_ogles();
    gameswf::set_render_handler(render);
    gameswf::set_glyph_provider(gameswf::create_glyph_provider_tu());
    m_pPlayer = new gameswf::player();
    m_pMovie = m_pPlayer->load_file(pathKey.c_str());
    if (m_pPlayer == NULL)
    {
        return false;
    }
    if (m_pMovie == NULL)
    {
        return false;
    }
    m_movieWidth = m_pMovie->m_def->m_frame_size.m_x_max - m_pMovie->m_def->m_frame_size.m_x_min;
    m_movieHeight = m_pMovie->m_def->m_frame_size.m_y_max - m_pMovie->m_def->m_frame_size.m_y_min;
    m_localScaleX = (m_pMovie->get_movie_width() / m_movieWidth);
    m_localScaleY = -(m_pMovie->get_movie_height() / m_movieHeight);
    m_nFrameCount = m_pMovie->get_frame_count();
    return true;
}

void CAFlashView::setScale(float scale)
{
    m_fScaleX = m_fScaleY = scale;
    setScaleX(m_fScaleX);
    setScaleY(m_fScaleY);
}

void CAFlashView::setScaleX(float scaleX)
{
    CAView::setScaleX(m_fScaleX);
}

void CAFlashView::setScaleY(float scaleY)
{
    CAView::setScaleY(m_fScaleY);
}

void CAFlashView::onEnter()
{
    CAView::onEnter();
}

void CAFlashView::onExit()
{
    CAView::onExit();
    if(! m_bIsRuning)
        return;
    CAScheduler::unschedule(schedule_selector(CAFlashView::update), this);
    m_bIsRuning = false;
}

void CAFlashView::runAction()
{
    if(m_bIsRuning)
        return;
    m_bIsRuning = true;
    m_pMovie->goto_frame(0);
    m_pMovie->set_play_run();
    CAScheduler::schedule(schedule_selector(CAFlashView::update), this, 0.01f);
}

void CAFlashView::stopAction()
{
    if(! m_bIsRuning)
        return;
    CAScheduler::unschedule(schedule_selector(CAFlashView::update), this);
    this->m_bIsRuning = false;
}

void CAFlashView::setRepeat(bool flag)
{
    m_bIsRepeat = flag;
}

bool CAFlashView::getRepeat()
{
    return m_bIsRepeat;
}

void CAFlashView::update(float dt)
{
    CAView::update(dt);
    int iFrame = m_pMovie->get_current_frame();
    m_nFrameCount = m_pMovie->get_frame_count();

    bool tobestop = false;
    if(! m_bIsRepeat)
    {
        if (iFrame == m_nFrameCount - 1)
        {
            tobestop = true;
        }
    }
    m_pMovie->advance(dt);
    if(tobestop)
    {
        this->stopAction();
    }
    this->updateDraw();
}

void CAFlashView::draw()
{
    CAView::draw();
    CC_NODE_DRAW_SETUP();
    ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
    kmMat4 matrixP;
    kmMat4 matrixMV;
    kmMat4 matrixMVP;
    kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
    kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );
    matrixMV.mat[13] = this->getFrame().size.height + matrixMV.mat[13];
    kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
#if CC_ENABLE_GL_STATE_CACHE
    ccGLUseProgram(0);//valid program is NON_ZERO unsigned int
#endif
    CCSize size = this->getFrame().size;
    m_localScaleX = size.width/ m_movieWidth;
    m_localScaleY = -(size.height / m_movieHeight);
    kmMat4 matrixs;
    kmMat4Scaling(&matrixs, m_localScaleX , m_localScaleY , getZOrder());
    kmMat4Multiply(&matrixMVP, &matrixMVP, &matrixs);
    m_pMovie->display(&matrixMVP.mat[0]);
    CHECK_GL_ERROR_DEBUG();
}
NS_CC_END
