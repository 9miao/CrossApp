//
//  CanvasScenes.cpp
//  CAUIEDITOR
//
//  Created by renhongguang on 15/8/18.
//
//

#include "CanvasScenes.h"

CanvasScenes::CanvasScenes()
:m_pointBegin(CCPointZero)
,m_pointEnd(CCPointZero)
,isdraw(false)
,isTouch(true)
{
    
}

CanvasScenes::~CanvasScenes()
{
    
}

void CanvasScenes::draw2()
{
    ccDrawColor4B(0, 0, 0, 127);
    glLineWidth(_px(2));
    CCPoint tem_point = ccp(m_pointBegin.x,getBounds().size.height-m_pointBegin.y);
    CCPoint tem_point2 = ccp(m_pointEnd.x,getBounds().size.height-m_pointEnd.y);
    if (isdraw) {
        ccDrawRect(tem_point,tem_point2);
    }
}

void CanvasScenes::visit()
{
    CC_RETURN_IF(!m_bVisible);
    
    kmGLPushMatrix();
    
    this->transform();
    
    bool isScissor = (bool)glIsEnabled(GL_SCISSOR_TEST);
    CCRect restoreScissorRect = CCRectZero;
    if (isScissor)
    {
        GLfloat params[4];
        glGetFloatv(GL_SCISSOR_BOX, params);
        restoreScissorRect = CCRect(params[0], params[1], params[2], params[3]);
    }
    
    
    if (!m_bDisplayRange)
    {
        kmMat4 modelview;
        kmGLGetMatrix(KM_GL_MODELVIEW, &modelview);
        kmMat4 tm;
        kmMat4Identity(&tm);
        tm.mat[12] = m_obContentSize.width;
        tm.mat[13] = m_obContentSize.height;
        kmMat4 tm2;
        kmMat4Multiply(&tm2, &modelview, &tm);
        
        CCSize winSize = CAApplication::getApplication()->getWinSize();
        CCPoint point = CCPoint(modelview.mat[12], modelview.mat[13]) + winSize/2;
        CCSize size = CCSize(tm2.mat[12] - modelview.mat[12], tm2.mat[13] - modelview.mat[13]);
        
        CCRect frame = CCRect(point.x + 0.5f, point.y, size.width + 1.0f, size.height + 1.0f);
        
        if (isScissor)
        {
            float x1 = MAX(frame.getMinX(), restoreScissorRect.getMinX());
            float y1 = MAX(frame.getMinY(), restoreScissorRect.getMinY());
            float x2 = MIN(frame.getMaxX(), restoreScissorRect.getMaxX());
            float y2 = MIN(frame.getMaxY(), restoreScissorRect.getMaxY());
            float width = MAX(x2-x1, 0);
            float height = MAX(y2-y1, 0);
            
            glScissor(x1, y1, width, height);
        }
        else
        {
            glEnable(GL_SCISSOR_TEST);
            glScissor(frame.origin.x,
                      frame.origin.y,
                      frame.size.width,
                      frame.size.height);
        }
    }
    
    this->sortAllSubviews();
    
    CAVector<CAView*>::iterator itr=m_obSubviews.begin();
    while (itr!=m_obSubviews.end())
    {
        CC_BREAK_IF((*itr)->m_nZOrder >= 0);
        (*itr)->visit();
        itr++;
    }
    
    this->draw();
    
    while (itr!=m_obSubviews.end())
    {
        (*itr)->visit();
        itr++;
    }
    
    this->draw2();
    //m_uOrderOfArrival = 0;
    
    if (!m_bDisplayRange)
    {
        if (isScissor)
        {
            glScissor(restoreScissorRect.origin.x,
                      restoreScissorRect.origin.y,
                      restoreScissorRect.size.width,
                      restoreScissorRect.size.height);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }
    }
    
    kmGLPopMatrix();
}


bool CanvasScenes::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    if (!isTouch) {
        return false;
    }
    m_pointBegin = pTouch->getLocation();
    m_pointBegin = this->convertToNodeSpace(m_pointBegin);
    return true;
}

void CanvasScenes::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    if (!isTouch) {
        return;
    }
    isdraw = true;
    m_pointEnd = pTouch->getLocation();
    m_pointEnd = this->convertToNodeSpace(m_pointEnd);
    updateDraw();
}

void CanvasScenes::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    m_pointBegin = CCPointZero;
    m_pointEnd = CCPointZero;
    updateDraw();
    isdraw = false;
}

void CanvasScenes::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    m_pointBegin = CCPointZero;
    m_pointEnd = CCPointZero;
    updateDraw();
    isdraw = false;
}

void CanvasScenes::setDrawRect(bool _b){
    isTouch = _b;
}