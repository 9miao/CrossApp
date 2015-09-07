//
//  AutoView.cpp
//  CAUIEDITOR
//
//  Created by renhongguang on 15/8/12.
//
//

#include "AutoView.h"

AutoView::AutoView(const ViewType& type, bool bOriginal)
:CAButton(CAButtonTypeCustom)
,m_pView(NULL)
,m_bEditing(false)
,m_eAutoPoint(AutoPoint::None)
,m_viewtype(ViewTypeCAView)
{
    m_viewtype = type;
    this->setTouchEnabled(false);
}

AutoView::~AutoView(){
    
}

void AutoView::onExitTransitionDidStart()
{
    CAButton::onExitTransitionDidStart();
}

void AutoView::onEnterTransitionDidFinish()
{
    CAButton::onEnterTransitionDidFinish();
    
    CADipRect rect = this->getBounds();
    rect.size = ccpSub(rect.size, CADipSize(40, 40));
    
    switch (m_viewtype) {
        case ViewTypeCAView:
            m_pView = CAView::createWithFrame(rect);
            break;
        case ViewTypeCAImageView:
            break;
        case ViewTypeCAScale9ImageView:
            break;
        case ViewTypeCALabel:
        {
            CALabel* label = CALabel::createWithFrame(rect);
            label->setColor(CAColor_black);
            label->setText("Label");
            label->setFontSize(_px(36));
            
            m_pView = label;
        }
            break;
        case ViewTypeCAButton:
            break;
        case ViewTypeCASegmentControl:
            break;
    }
    if (m_pView) {
        m_pView->setTouchEnabled(false);
        m_pView->setScrollEnabled(false);
        this->setBackGroundViewForState(CAControlStateNormal, m_pView);
    }

}

void AutoView::draw()
{
    CAButton::draw();
    ccDrawColor4B(0, 0, 0, 127);
    glLineWidth(_px(2));
    CCPoint vertices[4] =
    {
        CCPoint( m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y ),
        CCPoint( m_sQuad.br.vertices.x, m_sQuad.br.vertices.y ),
        CCPoint( m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y ),
        CCPoint( m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y ),
    };
    ccDrawPoly(vertices, 4, true);
    
    if (m_bEditing)
    {
        float w = m_sQuad.br.vertices.x - m_sQuad.tl.vertices.x;
        float h = m_sQuad.bl.vertices.y - m_sQuad.tl.vertices.y;
        
        //左上
        ccDrawRect(CCPoint( m_sQuad.tl.vertices.x-5, m_sQuad.tl.vertices.y-5 ), CCPoint( m_sQuad.tl.vertices.x+5, m_sQuad.tl.vertices.y+5 ));
        //左中
        ccDrawRect(CCPoint( m_sQuad.bl.vertices.x-5, m_sQuad.tl.vertices.y+h/2-5 ), CCPoint( m_sQuad.tl.vertices.x+5, m_sQuad.tl.vertices.y+h/2+5 ));
        //左下
        ccDrawRect(CCPoint( m_sQuad.bl.vertices.x-5, m_sQuad.bl.vertices.y-5 ), CCPoint( m_sQuad.bl.vertices.x+5, m_sQuad.bl.vertices.y+5 ));
        //中上
        ccDrawRect(CCPoint( m_sQuad.tl.vertices.x+w/2-5, m_sQuad.tl.vertices.y-5 ), CCPoint( m_sQuad.tl.vertices.x+w/2+5, m_sQuad.tl.vertices.y+5 ));
        //中下
        ccDrawRect(CCPoint( m_sQuad.bl.vertices.x+w/2-5, m_sQuad.bl.vertices.y-5 ), CCPoint( m_sQuad.bl.vertices.x+w/2+5, m_sQuad.bl.vertices.y+5 ));
        //右上
        ccDrawRect(CCPoint( m_sQuad.tr.vertices.x-5, m_sQuad.tr.vertices.y-5 ), CCPoint( m_sQuad.tr.vertices.x+5, m_sQuad.tr.vertices.y+5 ));
        //右中
        ccDrawRect(CCPoint( m_sQuad.tr.vertices.x-5, m_sQuad.tr.vertices.y+h/2-5 ), CCPoint( m_sQuad.tr.vertices.x+5, m_sQuad.tr.vertices.y+h/2+5 ));
        //右下
        ccDrawRect(CCPoint( m_sQuad.br.vertices.x-5, m_sQuad.br.vertices.y-5 ), CCPoint( m_sQuad.br.vertices.x+5, m_sQuad.br.vertices.y+5 ));
    }
}

bool AutoView::resignFirstResponder()
{
    if (!CAButton::resignFirstResponder())
    {
        return false;
    }
    
    m_bEditing = false;
    this->updateDraw();
    return true;
}

bool AutoView::becomeFirstResponder()
{
    if (!CAButton::becomeFirstResponder())
    {
        return false;
    }
    m_bEditing = true;
    this->updateDraw();
    return true;
}


std::map<AutoPoint, CCPoint> AutoView::getPoints()
{
    std::map<AutoPoint, CCPoint> mm;
    CCSize size = this->getBounds().size;
    mm[Left_Up] = CCPointZero;
    mm[Left_Center] = CCPoint(0,size.height/2);
    mm[Left_Down] = CCPoint(0,size.height);
    mm[Center_Down] = CCPoint(size.width/2,size.height);
    mm[Right_Down] = CCPoint(size.width,size.height);
    mm[Right_Center] = CCPoint(size.width,size.height/2);
    mm[Right_Up] = CCPoint(size.width,0);
    mm[Center_Up] = CCPoint(size.width/2,0);

    return mm;
}

AutoPoint AutoView::getAutoPoint(const CCPoint& point)
{
    AutoPoint ap = None;
    
    std::map<AutoPoint, CCPoint> mm = this->getPoints();
    
    std::map<AutoPoint, CCPoint>::iterator itr;
    for (itr=mm.begin(); itr!=mm.end(); itr++)
    {
        std::pair<AutoPoint, CCPoint> pp = *itr;
        
        if (ccpDistance(pp.second, point) < 7)
        {
            ap = pp.first;
            break;
        }
    }
    return ap;
}

bool AutoView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    CAButton::ccTouchBegan(pTouch, pEvent);
    
    touchBegin_point = pTouch->getLocation();
    touchBegin_point = this->convertToNodeSpace(touchBegin_point);
    
    CCRect rect = this->getBounds();
    rect.InflateRect(7);
    
    if (rect.containsPoint(touchBegin_point))
    {
        CCLog("123456");
        setEditing(true);
    }
    m_eAutoPoint = this->getAutoPoint(touchBegin_point);
    return true;
}

void AutoView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CAButton::ccTouchMoved(pTouch, pEvent);
    
//    CAResponder* responder = CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder();
//    
//    if (responder == this)
//    {
//        CCPoint point = pTouch->getLocation();
//        point = this->convertToNodeSpace(point);
//        float temp_x = point.x - touchBegin_point.x;
//        float temp_y = point.y - touchBegin_point.y;
//        
//        if (!m_bEditing)
//        {
//            return;
//        }
//        switch (m_eAutoPoint) {
//            case Left_Up:
//                this->setFrame(CCRect(getFrameOrigin().x+temp_x,getFrameOrigin().y+temp_y,getFrame().size.width-temp_x,getFrame().size.height-temp_y));
//                break;
//            case Left_Center:
//                this->setFrame(CCRect(getFrameOrigin().x+temp_x,getFrameOrigin().y,getFrame().size.width-temp_x,getFrame().size.height));
//                break;
//            case Left_Down:
//                this->setFrame(CCRect(getFrameOrigin().x+temp_x,getFrameOrigin().y,getFrame().size.width-temp_x,getFrame().size.height+temp_y));
//                touchBegin_point.y = point.y;
//                break;
//            case Center_Down:
//                this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y,getFrame().size.width,getFrame().size.height+temp_y));
//                touchBegin_point.y = point.y;
//                break;
//            case Right_Down:
//                this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y,getFrame().size.width+temp_x,getFrame().size.height+temp_y));
//                touchBegin_point.x = point.x;
//                touchBegin_point.y = point.y;
//                break;
//            case Right_Center:
//                this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y,getFrame().size.width+temp_x,getFrame().size.height));
//                touchBegin_point.x = point.x;
//                break;
//            case Right_Up:
//                this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y+temp_y,getFrame().size.width+temp_x,getFrame().size.height-temp_y));
//                touchBegin_point.x = point.x;
//                break;
//            case Center_Up:
//                this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y+temp_y,getFrame().size.width,getFrame().size.height-temp_y));
//                break;
//            case None:
//            {
//                //this->setFrame(CCRect(getFrameOrigin().x+temp_x,getFrameOrigin().y+temp_y,getFrame().size.width,getFrame().size.height));
//                CCPoint p = CCPoint(temp_x, temp_y);
//                m_pAutoViewDelegate->AutoViewMoveToPosition(p);
//            }
//                break;
//        }
//        if (this->getFrame().size.height<30) {
//            this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y,getFrame().size.width,30));
//        }
//        if(this->getFrame().size.width<30){
//            this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y,30,getFrame().size.height));
//        }
//        m_pAutoViewDelegate->AutoViewChangeFrameStart();
//    }
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    float temp_x = point.x - touchBegin_point.x;
    float temp_y = point.y - touchBegin_point.y;
    
    if (!m_bEditing)
    {
        return;
    }
    switch (m_eAutoPoint) {
        case Left_Up:
            this->setFrame(CCRect(getFrameOrigin().x+temp_x,getFrameOrigin().y+temp_y,getFrame().size.width-temp_x,getFrame().size.height-temp_y));
            break;
        case Left_Center:
            this->setFrame(CCRect(getFrameOrigin().x+temp_x,getFrameOrigin().y,getFrame().size.width-temp_x,getFrame().size.height));
            break;
        case Left_Down:
            this->setFrame(CCRect(getFrameOrigin().x+temp_x,getFrameOrigin().y,getFrame().size.width-temp_x,getFrame().size.height+temp_y));
            touchBegin_point.y = point.y;
            break;
        case Center_Down:
            this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y,getFrame().size.width,getFrame().size.height+temp_y));
            touchBegin_point.y = point.y;
            break;
        case Right_Down:
            this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y,getFrame().size.width+temp_x,getFrame().size.height+temp_y));
            touchBegin_point.x = point.x;
            touchBegin_point.y = point.y;
            break;
        case Right_Center:
            this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y,getFrame().size.width+temp_x,getFrame().size.height));
            touchBegin_point.x = point.x;
            break;
        case Right_Up:
            this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y+temp_y,getFrame().size.width+temp_x,getFrame().size.height-temp_y));
            touchBegin_point.x = point.x;
            break;
        case Center_Up:
            this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y+temp_y,getFrame().size.width,getFrame().size.height-temp_y));
            break;
        case None:
        {
            //this->setFrame(CCRect(getFrameOrigin().x+temp_x,getFrameOrigin().y+temp_y,getFrame().size.width,getFrame().size.height));
            CCPoint p = CCPoint(temp_x, temp_y);
            m_pAutoViewDelegate->AutoViewMoveToPosition(p);
        }
            break;
    }
    if (this->getFrame().size.height<30) {
        this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y,getFrame().size.width,30));
    }
    if(this->getFrame().size.width<30){
        this->setFrame(CCRect(getFrameOrigin().x,getFrameOrigin().y,30,getFrame().size.height));
    }
    m_pAutoViewDelegate->AutoViewChangeFrameStart();
}

void AutoView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CAButton::ccTouchEnded(pTouch, pEvent);
    m_eAutoPoint = AutoPoint::None;
    
    touchBegin_point = CCPointZero;
    m_pAutoViewDelegate->AutoViewChangeFrameEnd();
}

void AutoView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    CAButton::ccTouchCancelled(pTouch, pEvent);
    m_eAutoPoint = AutoPoint::None;
    touchBegin_point = CCPointZero;
    m_pAutoViewDelegate->AutoViewChangeFrameEnd();
}