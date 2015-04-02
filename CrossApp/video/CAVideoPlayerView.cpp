//
//  CAVideoPlayerView.cpp
//  CrossApp
//
//  Created by dai xinping on 14-10-27.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#include "CAVideoPlayerView.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

CAVideoPlayerView::CAVideoPlayerView(VPDecoder* decoder)
: _renderer(NULL)
, _currFrame(NULL)
, _image(NULL)
{
    m_elapsed = 0;
    _decoder = decoder;
}

CAVideoPlayerView::~CAVideoPlayerView()
{
    if (_renderer) {
        delete _renderer;
    }
    CC_SAFE_RELEASE(_currFrame);
}

CAVideoPlayerView* CAVideoPlayerView::create(VPDecoder* decoder)
{
    CAVideoPlayerView* view = new CAVideoPlayerView(decoder);
    if (view && view->init()) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return NULL;
}

CAVideoPlayerView* CAVideoPlayerView::createWithCenter(const CCRect &rect, VPDecoder* decoder)
{
    CAVideoPlayerView* view = new CAVideoPlayerView(decoder);
    if (view && view->initWithCenter(rect)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    
    return NULL;
}

CAVideoPlayerView* CAVideoPlayerView::createWithFrame(const CCRect &rect, VPDecoder* decoder)
{
    CAVideoPlayerView* view = new CAVideoPlayerView(decoder);
    if (view && view->initWithFrame(rect)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    
    return NULL;
}

bool CAVideoPlayerView::init()
{
    if (!CAView::init()) {
        return false;
    }
    
    setColor(ccc4(0, 0, 0, 255));
    
//    _decoder->setupVideoFrameFormat(kVideoFrameFormatYUV);
    
    if (_decoder->setupVideoFrameFormat(kVideoFrameFormatYUV)) {
        _renderer = new VPFrameRenderYUV();
    } else {
        _renderer = new VPFrameRenderRGB();
    }
            
    if (!_renderer->loadShaders()) {
        delete _renderer;
        return false;
    }
    
    CCLog("OK setup GL");
    
    return true;
}

bool CAVideoPlayerView::initWithFrame(const CCRect &rect)
{
    if (!CAView::initWithFrame(rect)) {
        return false;
    }
        
    return true;
}

bool CAVideoPlayerView::initWithCenter(const CCRect &rect)
{
    if (!CAView::initWithFrame(rect)) {
        return false;
    }
        
    return true;
}

void CAVideoPlayerView::setContentSize(const CCSize& size)
{
    CAView::setContentSize(size);
    
    CCRect pictureRect = _renderer->updateVertices(_decoder->getFrameWidth(), _decoder->getFrameHeight(),
                                                   getFrame().size.width, getFrame().size.height);
    
    _pictRect = pictureRect;
    setImageRect(_pictRect); 
}

void CAVideoPlayerView::setImageCoords(CCRect rect)
{
    rect = CC_RECT_POINTS_TO_PIXELS(rect);
    
    float atlasWidth = _decoder->getFrameWidth();
    float atlasHeight = _decoder->getFrameHeight();
    
    float left, right, top, bottom;
    
    if (m_bRectRotated)
    {
//        left    = rect.origin.x/atlasWidth;
//        right    = (rect.origin.x+rect.size.height) / atlasWidth;
//        top        = rect.origin.y/atlasHeight;
//        bottom    = (rect.origin.y+rect.size.width) / atlasHeight;

        left    = 0;
        right   = 1;
        top     = 0;
        bottom  = 1;

        if (m_bFlipX)
        {
            CC_SWAP(top, bottom, float);
        }
        
        if (m_bFlipY)
        {
            CC_SWAP(left, right, float);
        }
        
        m_sQuad.bl.texCoords.u = left;
        m_sQuad.bl.texCoords.v = top;
        m_sQuad.br.texCoords.u = left;
        m_sQuad.br.texCoords.v = bottom;
        m_sQuad.tl.texCoords.u = right;
        m_sQuad.tl.texCoords.v = top;
        m_sQuad.tr.texCoords.u = right;
        m_sQuad.tr.texCoords.v = bottom;
    }
    else
    {
//        left    = rect.origin.x/atlasWidth;
//        right    = (rect.origin.x + rect.size.width) / atlasWidth;
//        top        = rect.origin.y/atlasHeight;
//        bottom    = (rect.origin.y + rect.size.height) / atlasHeight;

        left    = 0;
        right   = 1;
        top     = 0;
        bottom  = 1;        
        
        if(m_bFlipX)
        {
            CC_SWAP(left,right,float);
        }
        
        if(m_bFlipY)
        {
            CC_SWAP(top,bottom,float);
        }
        
        m_sQuad.bl.texCoords.u = left;
        m_sQuad.bl.texCoords.v = bottom;
        m_sQuad.br.texCoords.u = right;
        m_sQuad.br.texCoords.v = bottom;
        m_sQuad.tl.texCoords.u = left;
        m_sQuad.tl.texCoords.v = top;
        m_sQuad.tr.texCoords.u = right;
        m_sQuad.tr.texCoords.v = top;
    }
}

void CAVideoPlayerView::updateImageRect()
{
    // Don't update Z.
    float m_fLeft, m_fTop, m_fRight, m_fBottom;
    
    m_fLeft = _pictRect.origin.x;
    m_fRight = _pictRect.origin.x + _pictRect.size.width;
    m_fTop = _pictRect.origin.y;
    m_fBottom = _pictRect.origin.y + _pictRect.size.height;
    
    m_sQuad.bl.vertices = vertex3(m_fLeft, m_fTop, 0);
    m_sQuad.br.vertices = vertex3(m_fRight, m_fTop, 0);
    m_sQuad.tl.vertices = vertex3(m_fLeft, m_fBottom, 0);
    m_sQuad.tr.vertices = vertex3(m_fRight, m_fBottom, 0);
}

void CAVideoPlayerView::visit()
{
    CAView::visit();
    
    updateDraw();
}

#include "view/CADrawingPrimitives.h"
void CAVideoPlayerView::draw()
{
//    CC_RETURN_IF(m_pobImage == NULL);
//    
//    CC_NODE_DRAW_SETUP();
//    
//    ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
        
    long offset = (long)&m_sQuad;
    
    if (_currFrame && _renderer) {
        _renderer->draw(_currFrame, offset);
    }
}

void CAVideoPlayerView::setCurrentFrame(VPVideoFrame *frame)
{
    CC_SAFE_RELEASE_NULL(_currFrame);
    _currFrame = frame;
    CC_SAFE_RETAIN(_currFrame);
}

void CAVideoPlayerView::setDecoder(VPDecoder *decoder)
{
    _decoder = decoder;
}

bool CAVideoPlayerView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    return false;
}

void CAVideoPlayerView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CAVideoPlayerView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CAVideoPlayerView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    
}


NS_CC_END