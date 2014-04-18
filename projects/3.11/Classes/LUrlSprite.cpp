//
//  LUrlSprite.cpp
//  project
//
//  Created by 栗元峰 on 14-3-21.
//
//

#include "LUrlSprite.h"
#include "LRootLayer.h"

void LUrlSprite::onEnterTransitionDidFinish()
{
    CCSprite::onEnterTransitionDidFinish();
    this->downLoad();
}

void LUrlSprite::onExitTransitionDidStart()
{
    CCSprite::onExitTransitionDidStart();
    
    if (m_httpDownload)
    {
        m_httpDownload->setDelegate(NULL);
        m_httpDownload->renunciation();
        m_httpDownload=NULL;
    }
}

LUrlSprite* LUrlSprite::create(const char* url)
{
    LUrlSprite* sprite = new LUrlSprite();
    if (sprite && sprite->initWithFile("9m.jpg"))
    {
        sprite->setTextureForUrl(url);
        sprite->setScale((600)/sprite->getContentSize().width);
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void LUrlSprite::downLoad()
{
    if (m_url.compare("") != 0)
    {
        m_httpDownload = LHttpDownload::create(this, m_url.c_str(), "0");
    }
}

void LUrlSprite::setTextureForUrl(const char *url)
{
    m_url = url;
    if (this->getParent())
    {
        this->downLoad();
    }
}

void LUrlSprite::downLoadFinish(const char* fileName)
{
    float height = this->getContentSize().height * this->getScaleY();
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(fileName);
    if (texture)
    {
        CCRect rect = CCRectZero;
        rect.size = texture->getContentSize();
        this->setTexture(texture);
        this->setTextureRect(rect);
        this->setScale(height / this->getContentSize().height);
    }
}

void LUrlSprite::downLoadFaild()
{

}