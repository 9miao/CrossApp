//
//  CAFlash.h
//  CrossApp
//
//  Created by qiaoxin on 15-06-03.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//


#include "CrossApp.h"
#include "CAFlash.h"

NS_CC_EXT_BEGIN


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

CAFlash::CAFlash()
:m_fScaleX(1)
,m_fScaleY(1)
,m_nFrameCount(0)
,m_pMovie(NULL)
,m_pPlayer(NULL)
{
    
}

CAFlash::~CAFlash()
{
    this->m_pMovie = NULL;
    this->m_pPlayer= NULL;
}

CAFlash* CAFlash::createWithFilePath(const std::string& filePath)
{
    CAFlash* flash = new CAFlash();
    if ( flash && flash->initWithFilePath(filePath)) {
        flash->autorelease();
        return flash;
    }
    return NULL;
}

bool CAFlash::initWithFilePath(const std::string& filePath)
{
    if (filePath.empty())
    {
        return false;
    }
    
    std::string pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath.c_str());
    gameswf::register_file_opener_callback(&CCGameSWF_file_opener);
    gameswf::register_fscommand_callback(&CCGameSWF_fscommand_handler);
    gameswf::register_log_callback(&CCGameSWF_log_handler);
    gameswf::set_sound_handler(0);
    gameswf::render_handler* render = gameswf::create_render_handler_ogles();
    gameswf::set_render_handler(render);
    gameswf::set_glyph_provider(gameswf::create_glyph_provider_tu());
    m_pPlayer = new gameswf::player();
    m_pMovie = m_pPlayer->load_file(pathKey.c_str());
    
    if (!m_pPlayer || !m_pMovie)
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

void CAFlash::setIndex(int index)
{
    m_pMovie->goto_frame(index);
}

int CAFlash::getIndex()
{
    return m_pMovie->get_current_frame();
}

int CAFlash::getCounts()
{
    return m_pMovie->get_frame_count();
}

void CAFlash::playRun()
{
    m_pMovie->set_play_run();
}

void CAFlash::advance(float dt)
{
    m_pMovie->advance(dt);
}

void  CAFlash::display(kmMat4* matrix)
{
    m_pMovie->display(&matrix->mat[0]);
}

float CAFlash::getWidth()
{
    return m_movieWidth;
}

float CAFlash::getHeight()
{
    return m_movieHeight;
}

float CAFlash::getLocalScaleX()
{
    return m_localScaleX;
}

float CAFlash::getLocalScaleY()
{
    return m_localScaleY;
}

float CAFlash::getScaleX()
{
    return m_fScaleX;
}

float CAFlash::getScaleY()
{
    return m_fScaleY;
}

float CAFlash::getFrameCount()
{
    return m_nFrameCount;
}


NS_CC_EXT_END
