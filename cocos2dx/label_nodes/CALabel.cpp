//
//  CALabel.cpp
//  project
//
//  Created by lh on 14-5-12.
//
//

#include "CALabel.h"
#include "include/ccMacros.h"
#include "CCDirector.h"
#include <locale>
#include <cstdlib>
NS_CC_BEGIN


static unsigned int getFontHeight(const char *fontName, int fontSize)
{
    unsigned int result=0;
    CAImage *image = new CAImage();
    image->initWithString("9m", fontName, fontSize, CCSizeZero, CATextAlignmentLeft, CAVerticalTextAlignmentTop);
    
    CAImage *image1 = new CAImage();
    image1->initWithString("9\nm", fontName, fontSize, CCSizeZero,  CATextAlignmentLeft, CAVerticalTextAlignmentTop);
    result = image1->getContentSize().height-image->getContentSize().height;
    image->release();
    image1->release();
    return result;
}

CALabel::CALabel()
:m_nNumberOfLine(0),
m_nTextAlignment(CATextAlignmentLeft),
m_nText(""),
m_nfontName("Arial"),
m_nVerticalTextAlignmet(CAVerticalTextAlignmentTop),
m_pTextImage(NULL),
m_nDimensions(CCSizeZero),
m_nfontSize(24),
m_cLabelSize(CCSizeZero),
isInput(false)
{
    this->setColor(CAColor_black);
}

CALabel::~CALabel()
{
    
}

CALabel *CALabel::createWithFrame(const CCRect& rect)
{
    CALabel *label = new CALabel();
    if (label && label->initWithFrame(rect))
    {
        label->autorelease();
        return label;
    }
    CC_SAFE_DELETE(label);
    return NULL;
}

CALabel* CALabel::createWithCenter(const cocos2d::CCRect &rect)
{
    CALabel *label = new CALabel();
    if (label && label->initWithCenter(rect))
    {
        label->autorelease();
        return label;
    }
    
    CC_SAFE_DELETE(label);
    return NULL;
}

void CALabel::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    this->updateImage();
}

bool CALabel::init()
{
    CAView::init();
    this->setColor(CAColor_black);
    return true;
}

bool CALabel::initWithFrame(const CCRect& rect)
{
   
    if (!this->init())
    {
        return false;
    }
    this->setFrame(rect);
    m_rRect = rect;
    
    
    return true;
}

bool CALabel::initWithCenter(const CCRect& rect)
{
    
    if (!this->init())
    {
        return false;
    }
    this->setCenter(rect);
    m_rRect =rect;

    return true;
}

caFontDefinition CALabel::setFontDefiniton(bool flag)
{
    caFontDefinition texDef;
    
    if (flag)
        texDef.m_fontSize       =  m_nfontSize * CC_CONTENT_SCALE_FACTOR();
    else
        texDef.m_fontSize       =  m_nfontSize;
    
    texDef.m_fontName       = m_nfontName;
    texDef.m_alignment      =  m_nTextAlignment;
    texDef.m_vertAlignment  =  m_nVerticalTextAlignmet;
    
    if (flag)
        texDef.m_dimensions     =  CC_SIZE_POINTS_TO_PIXELS(m_nDimensions);
    else
        texDef.m_dimensions     =  m_nDimensions;
    
    return texDef;
}

bool CALabel::updateImage()
{
    if (m_bRunning == false)
    {
        return false;
    }
    
    CAImage *tex = new CAImage();
    if (!tex)
    {
        return false;
    }
    caFontDefinition fontDef = setFontDefiniton(true);
   
    float fontHeight =getFontHeight(fontDef.m_fontName.c_str(), fontDef.m_fontSize);
    
    unsigned int linenumber = (int)m_rRect.size.height/fontHeight;
    
    if ((m_nNumberOfLine <= linenumber && m_nNumberOfLine != 0))
    {

        if (isInput)
        {
            tex->initWithString(m_nText.c_str(),
                                fontDef.m_fontName.c_str(),
                                fontDef.m_fontSize,
                                CCSizeMake(0, fontHeight*m_nNumberOfLine),
                                fontDef.m_alignment,
                                fontDef.m_vertAlignment);
        }
        else
        {
            tex->initWithString(m_nText.c_str(),
                                fontDef.m_fontName.c_str(),
                                fontDef.m_fontSize,
                                CCSizeMake(m_rRect.size.width, fontHeight*m_nNumberOfLine),
                                fontDef.m_alignment,
                                fontDef.m_vertAlignment);
        }
    }
    else
    {
        if ((m_rRect.size.width == 0 && m_rRect.size.height == 0))
        {
            
            tex->initWithString(m_nText.c_str(),
                                fontDef.m_fontName.c_str(),
                                fontDef.m_fontSize, CCSizeZero,
                                fontDef.m_alignment,
                                fontDef.m_vertAlignment);
            
        }else
        {
            tex->initWithString(m_nText.c_str(),
                                fontDef.m_fontName.c_str(),
                                fontDef.m_fontSize, m_rRect.size,
                                fontDef.m_alignment,
                                fontDef.m_vertAlignment);
        }
    }
    m_cLabelSize = tex->getContentSize();
    CCRect rect = CCRectZero;
    if(isInput)
    {
        rect.size.width = m_rRect.size.width;

        if (tex->getContentSize().width>m_rRect.size.width)
        {
            rect.size.width = m_rRect.size.width;
        }
        else
        {
            rect.size.width =tex->getContentSize().width;
            
        }
        rect.size.height = tex->getContentSize().height;
    }
    else if (m_nNumberOfLine!=0 && m_nNumberOfLine*fontHeight < m_rRect.size.height && !isInput)
    {
        rect.size.width = m_rRect.size.width;
        rect.size.height = m_nNumberOfLine*fontHeight;
    }
    else
    {
        rect.size.width = m_rRect.size.width;
        rect.size.height = tex->getContentSize().height;
    }
    float width = MIN(m_rRect.size.width,tex->getContentSize().width);
    rect.size.width = width;
    this->setImage(tex);
    this->setImageRect(rect);//rect,false,rect.size
    if (isInput)
    {
        float pTextHeight= 0;
        
        switch (m_nVerticalTextAlignmet)
        {
            case CAVerticalTextAlignmentTop:
                pTextHeight =0;
                break;
            case CAVerticalTextAlignmentCenter:
                pTextHeight =(m_rRect.size.height-rect.size.height)/2;
                break;
            case CAVerticalTextAlignmentBottom:
                pTextHeight =m_rRect.size.height - rect.size.height;
                break;
            default:
                break;
        }
        this->CAView::setFrameOrigin(CCPoint(0, pTextHeight));
    }
    
    tex->release();
    
    return true;
}
void CALabel::setImageRect(const CCRect& rect)
{
    m_bRectRotated = false;
    
    setVertexRect(rect);
    setImageCoords(rect);

    if (m_pobBatchView)
    {
        // update dirty_, don't update recursiveDirty_
        setDirty(true);
    }
    else
    {
        this->updateImageRect();
    }
}

void CALabel::updateImageRect()
{
    GLfloat x1,x2,y1,y2;
    x1 = 0;
    
    if (isInput)
    {
        x1 = m_rRect.origin.x;
    }
    
    y1=0;
    y1=m_obContentSize.height - m_obRect.size.height - y1;

    x2=x1+m_obRect.size.width;
    y2=y1+m_obRect.size.height;
    
    m_sQuad.bl.vertices = vertex3(x1, y1, 0);
    m_sQuad.br.vertices = vertex3(x2, y1, 0);
    m_sQuad.tl.vertices = vertex3(x1, y2, 0);
    m_sQuad.tr.vertices = vertex3(x2, y2, 0);
}


void CALabel::setDimensions(cocos2d::CCSize var)
{
    m_nDimensions = var;
    this->updateImage();
}

CCSize CALabel::getDimensions()
{
    return m_nDimensions;
}

void CALabel::setText(string var)
{
    m_nText = var;

    this->updateImage();
}

void CALabel::setTextAlignment(CATextAlignment var)
{
    m_nTextAlignment = var;
    
    if (m_nText.size()>0)
    {
        this->updateImage();
    }
    
}

CATextAlignment CALabel::getTextAlignment()
{
    return m_nTextAlignment;
}

string CALabel::getText()
{
    return m_nText;
}

unsigned int CALabel::getNumberOfLine()
{
    return m_nNumberOfLine;
}

void CALabel::setNumberOfLine(unsigned int var)
{
    m_nNumberOfLine = var;
    this->updateImage();
}

NS_CC_END