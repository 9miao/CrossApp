//
//  CanvasView.cpp
//  CAUIEDITOR
//
//  Created by renhongguang on 15/8/14.
//
//

#include "CanvasView.h"

CanvasView::CanvasView()
:m_pointBegin(CCPointZero)
,m_pointEnd(CCPointZero)
,isdraw(true)
,m_TempAutoView(NULL)
,m_TempAutoPoint(AutoPoint::None)
,selectMoreView(false)
,isCanMove(false)
,hadMove(false)
{
    this->setHaveNextResponder(false);
}

CanvasView::~CanvasView()
{
    
}

void CanvasView::draw2()
{
    ccDrawColor4B(0, 0, 0, 127);
    glLineWidth(_px(2));
    CCPoint tem_point = ccp(m_pointBegin.x,getBounds().size.height-m_pointBegin.y);
    CCPoint tem_point2 = ccp(m_pointEnd.x,getBounds().size.height-m_pointEnd.y);
    if (isdraw) {
        ccDrawRect(tem_point,tem_point2);
    }
}

void CanvasView::visit()
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

bool CanvasView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    CAView::ccTouchBegan(pTouch, pEvent);
    m_pointBegin = pTouch->getLocation();
    m_pointBegin = this->convertToNodeSpace(m_pointBegin);
    m_pointEnd = m_pointBegin;
    CAVector<CAView *> m_autoViews = this->getSubviews();
    if (m_autoViews.size()>0 && !selectMoreView) {
        int num = 0;
        for (int i=0;i<m_autoViews.size();i++)
        {
            AutoView* av = (AutoView*)m_autoViews.at(i);
            if (av->getEditing()) {
                num++;
            }
        }
        
        for (int i=0;i<m_autoViews.size();i++)
        {
            AutoView* av = (AutoView*)m_autoViews.at(i);
            if (av->getFrame().containsPoint(m_pointBegin)) {
                if (num>0) {
                    for (int j=0;j<m_autoViews.size();j++)
                    {
                        AutoView* av1 = (AutoView*)m_autoViews.at(j);
                        if (i==j) {
                            av->setEditing(true);
                            m_TempAutoView = av;
                        }else{
                            av1->setEditing(false);
                        }
                    }
                }else{
                    if (!av->getEditing()) {
                        av->setEditing(true);
                        m_TempAutoView = av;
                    }
                }
                
            }
            
            
        }
        if (m_TempAutoView) {
            m_TempAutoPoint = m_TempAutoView->getAutoPoint(m_TempAutoView->convertToNodeSpace(pTouch->getLocation()));
            m_TempRect = m_TempAutoView->getFrame();
            isdraw = false;
            if (m_TempAutoPoint==AutoPoint::None) {
                if(m_TempRect.containsPoint(m_pointBegin))
                {
                }else{
                    m_TempAutoView->setEditing(false);
                    m_TempAutoView = NULL;
                    isdraw = true;
                }
            }
        }
    }else if(selectMoreView){
        for (int i=0;i<m_autoViews.size();i++)
        {
            AutoView* av = (AutoView*)m_autoViews.at(i);
            AutoPoint mp = av->getAutoPoint(av->convertToNodeSpace(pTouch->getLocation()));
            if (mp==AutoPoint::None) {
                if (av->getFrame().containsPoint(m_pointBegin)) {
                    isCanMove = true;
                    isdraw = false;
                }
            }else{
                for (int j=0;j<m_autoViews.size();j++)
                {
                    AutoView* av1 = (AutoView*)m_autoViews.at(j);
                    if (j!=i) {
                        av1->setEditing(false);
                    }
                }
                av->setEditing(true);
                m_TempAutoView = av;
                m_TempAutoPoint = mp;
                m_TempRect = m_TempAutoView->getFrame();
                selectMoreView = false;
                isCanMove = true;
                isdraw = false;
                break;
                
            }
            
        }
        if(!isCanMove){
            for (int i=0;i<m_autoViews.size();i++)
            {
                AutoView* av = (AutoView*)m_autoViews.at(i);
                av->setEditing(false);
                selectMoreView = false;
                isdraw = true;
                
            }
        }
    }
    return true;
}

void CanvasView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CAView::ccTouchMoved(pTouch, pEvent);
    m_pointEnd = pTouch->getLocation();
    m_pointEnd = this->convertToNodeSpace(m_pointEnd);
    
    float temp_x = m_pointEnd.x - m_pointBegin.x;
    float temp_y = m_pointEnd.y - m_pointBegin.y;
    
    CCPoint p_off = CCPointZero;
    
    p_off = ccpSub(this->convertToNodeSpace(pTouch->getLocation()),
                   this->convertToNodeSpace(pTouch->getPreviousLocation()));
    
    if (this->getSubviews().size()>0) {
        if (m_TempAutoView && m_TempAutoView->getEditing()) {
            CAView* tempview = m_TempAutoView->getBackGroundViewForState(CAControlStateNormal);
            switch (m_TempAutoPoint) {
                case AutoPoint::Left_Up:
                    tempview->setFrame(CCRect(m_TempRect.origin.x+temp_x,m_TempRect.origin.y+temp_y,m_TempRect.size.width-temp_x,m_TempRect.size.height-temp_y));
                    m_TempAutoView->setFrame(CCRect(m_TempRect.origin.x+temp_x,m_TempRect.origin.y+temp_y,tempview->getFrame().size.width,tempview->getFrame().size.height));
                    break;
                case AutoPoint::Left_Center:
                    tempview->setFrame(CCRect(m_TempRect.origin.x+temp_x,m_TempRect.origin.y,m_TempRect.size.width-temp_x,m_TempRect.size.height));
                    m_TempAutoView->setFrame(CCRect(m_TempRect.origin.x+temp_x,m_TempRect.origin.y,tempview->getFrame().size.width,tempview->getFrame().size.height));
                    break;
                case AutoPoint::Left_Down:
                    tempview->setFrame(CCRect(m_TempRect.origin.x+temp_x,m_TempRect.origin.y,m_TempRect.size.width-temp_x,m_TempRect.size.height+temp_y));
                    m_TempAutoView->setFrame(CCRect(m_TempRect.origin.x+temp_x,m_TempRect.origin.y,tempview->getFrame().size.width,tempview->getFrame().size.height));
                    break;
                case AutoPoint::Center_Down:
                    tempview->setFrame(CCRect(m_TempRect.origin.x,m_TempRect.origin.y,m_TempRect.size.width,m_TempRect.size.height+temp_y));
                    m_TempAutoView->setFrame(CCRect(m_TempRect.origin.x,m_TempRect.origin.y,tempview->getFrame().size.width,tempview->getFrame().size.height));
                    break;
                case AutoPoint::Right_Down:
                    tempview->setFrame(CCRect(m_TempRect.origin.x,m_TempRect.origin.y,m_TempRect.size.width+temp_x,m_TempRect.size.height+temp_y));
                    m_TempAutoView->setFrame(CCRect(m_TempRect.origin.x,m_TempRect.origin.y,tempview->getFrame().size.width,tempview->getFrame().size.height));
                    break;
                case AutoPoint::Right_Center:
                    tempview->setFrame(CCRect(m_TempRect.origin.x,m_TempRect.origin.y,m_TempRect.size.width+temp_x,m_TempRect.size.height));
                    m_TempAutoView->setFrame(CCRect(m_TempRect.origin.x,m_TempRect.origin.y,tempview->getFrame().size.width,tempview->getFrame().size.height));
                    break;
                case AutoPoint::Right_Up:
                    tempview->setFrame(CCRect(m_TempRect.origin.x,m_TempRect.origin.y+temp_y,m_TempRect.size.width+temp_x,m_TempRect.size.height-temp_y));
                    m_TempAutoView->setFrame(CCRect(m_TempRect.origin.x,m_TempRect.origin.y+temp_y,tempview->getFrame().size.width,tempview->getFrame().size.height));
                    break;
                case AutoPoint::Center_Up:
                    tempview->setFrame(CCRect(m_TempRect.origin.x,m_TempRect.origin.y+temp_y,m_TempRect.size.width,m_TempRect.size.height-temp_y));
                    m_TempAutoView->setFrame(CCRect(m_TempRect.origin.x,m_TempRect.origin.y+temp_y,tempview->getFrame().size.width,tempview->getFrame().size.height));
                    break;
                case AutoPoint::None:
                    m_TempAutoView->setFrameOrigin(CCPoint(m_TempRect.origin.x+temp_x,m_TempRect.origin.y+temp_y));
                    break;
            }
        }else if(selectMoreView && isCanMove){
            CAVector<CAView *> m_autoViews = this->getSubviews();
            for (int i=0;i<m_autoViews.size();i++)
            {
                AutoView* av = (AutoView*)m_autoViews.at(i);
                if (av->getEditing()) {
                    av->setFrameOrigin(CCPoint(av->getFrameOrigin().x+p_off.x,av->getFrameOrigin().y+p_off.y));
                    hadMove = true;
                }
            }
        }
        else{
            int selectNum = 0;
            CAVector<CAView *> m_autoViews = this->getSubviews();
            for (int i=0;i<m_autoViews.size();i++)
            {
                AutoView* av = (AutoView*)m_autoViews.at(i);
                float x = MIN(m_pointBegin.x, m_pointEnd.x);
                float y = MIN(m_pointBegin.y, m_pointEnd.y);
                int   w = abs((int)(m_pointEnd.x-m_pointBegin.x));
                int   h = abs((int)(m_pointEnd.y-m_pointBegin.y));
                CCRect tempRect = CCRect(x,y,w,h);
                if (av->getFrame().intersectsRect(tempRect)) {
                    av->setEditing(true);
                    selectNum++;
                }else{
                    av->setEditing(false);
                    selectNum--;
                }
                
            }
            if (selectNum>1) {
                selectMoreView = true;
            }else{
                selectMoreView = false;
            }
        }
        
    }
    updateDraw();
}

void CanvasView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint p = pTouch->getLocation();
    p = this->convertToNodeSpace(p);
    
    CAView::ccTouchEnded(pTouch, pEvent);
    m_pointBegin = CCPointZero;
    m_pointEnd = CCPointZero;
    updateDraw();
    if (!hadMove && selectMoreView && isCanMove) {
        CAVector<CAView *> m_autoViews = this->getSubviews();
        for (int i=0;i<m_autoViews.size();i++)
        {
            AutoView* av = (AutoView*)m_autoViews.at(i);
            if (av->getFrame().containsPoint(p)) {
                
            }else{
                av->setEditing(false);
            }
        }
        selectMoreView = false;
    }
    isdraw = true;
    isCanMove = false;
    hadMove = false;
}

void CanvasView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    CAView::ccTouchCancelled(pTouch, pEvent);
    m_pointBegin = CCPointZero;
    m_pointEnd = CCPointZero;
    updateDraw();
    isdraw = true;
    isCanMove = false;
    hadMove = false;
}