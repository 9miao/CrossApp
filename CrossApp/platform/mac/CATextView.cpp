//
//  CAtextView.cpp
//  tesss
//
//  Created by 栗元峰 on 15/11/20.
//
//

#include "platform/CATextView.h"
#include "platform/CADensityDpi.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"


NS_CC_BEGIN



CATextView::CATextView()
: m_pBackgroundView(NULL)
, m_pShowImageView(NULL)
, m_pTextView(NULL)
, m_iFontSize(40)
, m_pDelegate(NULL)
, m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    this->setHaveNextResponder(false);
}

CATextView::~CATextView()
{
    
}

void CATextView::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextView::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

bool CATextView::resignFirstResponder()
{
	if (m_pDelegate && (!m_pDelegate->textViewShouldEndEditing(this)))
	{
		return false;
	}

    bool result = CAView::resignFirstResponder();


    return result;
}

bool CATextView::becomeFirstResponder()
{
	if (m_pDelegate &&( !m_pDelegate->textViewShouldBeginEditing(this)))
	{
		return false;
	}

	bool result = CAView::becomeFirstResponder();


    return result;
}

void CATextView::hideTextView()
{
    m_pBackgroundView->setVisible(false);
}

void CATextView::showTextView()
{
    m_pBackgroundView->setVisible(true);
}

void CATextView::hideNativeTextView()
{
	CAScheduler::unschedule(schedule_selector(CATextView::update), this);
}

void CATextView::showNativeTextView()
{
    this->update(0);
	CAScheduler::schedule(schedule_selector(CATextView::update), this, 1 / 60.0f);
}

void CATextView::delayShowImage()
{

}

void CATextView::showImage()
{
    m_pShowImageView->setFrame(this->getBounds());
}

CATextView* CATextView::createWithFrame(const DRect& frame)
{
	CATextView *textView = new CATextView();
	if (textView && textView->initWithFrame(frame))
    {
		textView->autorelease();
		return textView;
    }
	CC_SAFE_DELETE(textView);
    return NULL;
}

CATextView* CATextView::createWithCenter(const DRect& rect)
{
	CATextView* textView = new CATextView();
    
	if (textView && textView->initWithCenter(rect))
    {
		textView->autorelease();
		return textView;
    }
    
	CC_SAFE_DELETE(textView);
    return NULL;
}

bool CATextView::init()
{
    CAImage* image = CAImage::create("source_material/textField_bg.png");
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);

	m_pBackgroundView = CAScale9ImageView::createWithImage(image);
	m_pBackgroundView->setCapInsets(capInsets);
	this->insertSubview(m_pBackgroundView, -1);
    
	m_pShowImageView = CAImageView::createWithFrame(DRect(0, 0, 1, 1));
	m_pShowImageView->setTextTag("textView");
	this->addSubview(m_pShowImageView);
	
    return true;
}

void CATextView::update(float dt)
{
    do
    {
        CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);
        
        CC_BREAK_IF(m_obLastPoint.equals(point));
        
        point.x = s_dip_to_px(point.x);
        point.y = s_dip_to_px(point.y);
    }
    while (0);
}

void CATextView::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
    
    DSize worldContentSize = DSizeApplyAffineTransform(m_obContentSize, worldToNodeTransform());
    
    DSize size;
    size.width = s_dip_to_px(worldContentSize.width);
    size.height =  s_dip_to_px(worldContentSize.height);

    
	m_pBackgroundView->setFrame(this->getBounds());
    m_pShowImageView->setFrame(this->getBounds());
}

bool CATextView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    
    return true;
}

void CATextView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CATextView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    DPoint point = this->convertTouchToNodeSpace(pTouch);
    
    if (this->getBounds().containsPoint(point))
    {
        becomeFirstResponder();
    }
    else
    {
        resignFirstResponder();
    }
}

void CATextView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}


void CATextView::setTextFontSize(const int& var)
{
	m_iFontSize = var;


    this->delayShowImage();
}

const int& CATextView::getTextFontSize()
{
	return m_iFontSize;
}



void CATextView::setText(const std::string& var)
{
	m_sText = var;

}

const std::string& CATextView::getText()
{
	return m_sText;
}

void CATextView::setTextColor(const CAColor4B& var)
{
	m_sTextColor = var;

    this->delayShowImage();
}

const CAColor4B& CATextView::getTextColor()
{
	return m_sTextColor; 
}

void CATextView::setBackgroundImage(CAImage* image)
{
    if (image)
    {
        DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
        m_pBackgroundView->setCapInsets(capInsets);
    }
    m_pBackgroundView->setImage(image);
}

void CATextView::setTextViewAlign(const TextViewAlign& var)
{
    m_eAlign = var;
    
    this->delayShowImage();
}

const CATextView::TextViewAlign& CATextView::getTextViewAlign()
{
    return m_eAlign;
}



NS_CC_END

