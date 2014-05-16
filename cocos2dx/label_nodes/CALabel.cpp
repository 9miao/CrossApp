//
//  CALabel.cpp
//  project
//
//  Created by lh on 14-5-12.
//
//

#include "CALabel.h"

CALabel::CALabel()
:m_nNumberOfLine(0),
m_nShadowcolor(ccc4(0, 0, 0,255)),
m_nShadowOffset(CCSizeZero),
m_nTextAlignment(kCCTextAlignmentCenter),
m_nText(""),
m_nfontName("Arial"),
m_nTextcolor(ccc4(0, 0, 0, 255)),
m_nvTextAlignmet(kCCVerticalTextAlignmentTop),
m_pTextTexture(NULL),
m_nDimensions(CCSizeZero),
m_nfontSize(18)
{
    
}
CALabel::~CALabel()
{
    
}
CALabel *CALabel::create(CCRect frame)
{
    CALabel *label = new CALabel();
    if (label && label->initWithFrame(frame))
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
    this->updataTexture();
}
bool CALabel::initWithFrame(CCRect frame)
{
   
    if (!CAView::initWithFrame(frame)) {
        return false;
    }
    
    this->setOpacity(0);
    m_pTextTexture = CCSprite::create();
    
    m_pTextTexture -> setFrame(this->getBounds());
    
    this->addSubview(m_pTextTexture);
    
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
    texDef.m_vertAlignment  =  m_nvTextAlignmet;
    
    
    if (flag)
        texDef.m_dimensions     =  CC_SIZE_POINTS_TO_PIXELS(m_nDimensions);
    else
        texDef.m_dimensions     =  m_nDimensions;
    
    // text tint
    texDef.m_fontFillColor = m_nTextcolor;
    
    return texDef;
}

bool CALabel::updataTexture()
{
    
    if (m_bRunning == false)
    {
        return false;
    }
    
    CCTexture2D *tex = new CCTexture2D();
    if (!tex)
    {
        return false;
    }
    
    caFontDefinition fontDef = setFontDefiniton(true);
    tex->initWithString("国", fontDef.m_fontName.c_str(), fontDef.m_fontSize, CCSizeMake(this->getBounds().size.width, 0), fontDef.m_alignment, fontDef.m_vertAlignment);
    float fontHeight = tex->getContentSize().height+1;

    unsigned int linenumber = (int)this->getBounds().size.height/fontHeight;
    if (m_nNumberOfLine <= linenumber && m_nNumberOfLine != 0)
    {

        tex->initWithString(m_nText.c_str(), fontDef.m_fontName.c_str(), fontDef.m_fontSize, CCSizeMake(this->getBounds().size.width, m_nNumberOfLine*fontHeight), fontDef.m_alignment, fontDef.m_vertAlignment);
    }
    else
    {
         tex->initWithString(m_nText.c_str(), fontDef.m_fontName.c_str(), fontDef.m_fontSize, this->getBounds().size, fontDef.m_alignment, fontDef.m_vertAlignment);
    }
    
    
    m_pTextTexture->setTexture(tex);
    

    
    m_pTextTexture->setColor(ccc3(m_nTextcolor.r, m_nTextcolor.g, m_nTextcolor.b));
    
    m_pTextTexture->setOpacity(m_nTextcolor.a);
    
    if (this->getBounds().size.height == 0)
    {
        this->setFrame(CCRectMake(this->getBounds().origin.x, this->getBounds().origin.y, tex->getContentSize().width, tex->getContentSize().height));
        m_pTextTexture->setTextureRect(CCRectMake(0, 0, tex->getContentSize().width, tex->getContentSize().height));
        m_pTextTexture -> setFrame(CCRectMake(0, 0, tex->getContentSize().width, tex->getContentSize().height));
        
    }
    else
    {
        m_pTextTexture->setTextureRect(this->getBounds());
        m_pTextTexture -> setFrame(CCRectMake(0, 0, this->getBounds().size.width, this->getBounds().size.height));
    }
    
    
    tex->release();
    
    return true;
}

void CALabel::setText(string var)
{
    m_nText = var;
    if(var.size()>0){
        this->updataTexture();
        
    }
   
}

void CALabel::setTextAlignment(CCTextAlignment var)
{
    m_nTextAlignment = var;
    if (m_nText.size()>0) {
        this->updataTexture();
    }
    
}

CCTextAlignment CALabel::getTextAlignment()
{
    return m_nTextAlignment;
}

ccColor4B CALabel::getTextcolor()
{
    return m_nTextcolor;
}

ccColor4B CALabel::getShadowcolor()
{
    return m_nShadowcolor;
}

CCSize CALabel::getShadowOffset()
{
    return m_nShadowOffset;
}

string CALabel::getText()
{
    return m_nText;
}

unsigned int CALabel::getNumberOfLine()
{
    return m_nNumberOfLine;
}

void CALabel::setTextcolor(ccColor4B var)
{
    m_nTextcolor = var;
  
    this->updataTexture();

    
}
void CALabel::setShadowcolor(ccColor4B var)
{
    m_nShadowcolor= var;
    if (m_nText.size()>0)
    {
        this->updataTexture();
    }
    
}

void CALabel::setShadowOffset(cocos2d::CCSize var)
{
    m_nShadowOffset = var;
    this->updataTexture();
}

void CALabel::setNumberOfLine(unsigned int var)
{
    m_nNumberOfLine = var;
    this->updataTexture();
}
