//
//  CATextFieldX.cpp
//  tesss
//
//  Created by 栗元峰 on 15/11/20.
//
//

#include "platform/CATextFieldX.h"
#include "platform/CADensityDpi.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#import "EAGLView.h"
#import <UIKit/UIKit.h>

#define textField_iOS ((UITextField*)m_pTextField)

NS_CC_BEGIN

CATextFieldX::CATextFieldX()
:m_pImgeView(NULL)
,m_pTextField(NULL)
{
    this->setHaveNextResponder(false);
}

CATextFieldX::~CATextFieldX()
{
    
}

void CATextFieldX::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0);
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextFieldX::showImageView));
    CAViewAnimation::commitAnimations();
}

void CATextFieldX::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

bool CATextFieldX::resignFirstResponder()
{
    bool result = CAView::resignFirstResponder();

    if ([textField_iOS isFirstResponder])
    {
        [textField_iOS resignFirstResponder];
    }
    this->showImageView();
    
//    m_pImgeView->setVisible(true);
    
    this->hideNativeTextField();
    
    return result;
}

bool CATextFieldX::becomeFirstResponder()
{
    bool result = CAView::becomeFirstResponder();
    
//    m_pImgeView->setVisible(false);
    
    [textField_iOS becomeFirstResponder];
    
    this->showNativeTextField();
    return result;
}

void CATextFieldX::hideNativeTextField()
{
    CAScheduler::unschedule(schedule_selector(CATextFieldX::update), this);
    
    CAViewAnimation::beginAnimations(m_s__StrID, NULL);
    CAViewAnimation::setAnimationDuration(0);
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextFieldX::hide));
    CAViewAnimation::commitAnimations();
}

void CATextFieldX::hide()
{
    CGRect rect = textField_iOS.frame;
    rect.origin = CGPointMake(5000, 5000);
    textField_iOS.frame = rect;
}

void CATextFieldX::showNativeTextField()
{
    this->update(0);
    CAScheduler::schedule(schedule_selector(CATextFieldX::update), this, 1/60.0f);
}

void CATextFieldX::showImageView()
{
    static float scale = [UIScreen mainScreen].scale;
    UIGraphicsBeginImageContextWithOptions(textField_iOS.bounds.size, NO, scale);
    CGContextRef context = UIGraphicsGetCurrentContext();
    [textField_iOS.layer renderInContext:context];
    UIImage *image_iOS = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    NSData* iOSData = UIImagePNGRepresentation(image_iOS);
    [image_iOS release];
    
    unsigned char* data = (unsigned char*)malloc([iOSData length]);
    [iOSData getBytes:data];
    
    CAImage *image = CAImage::createWithImageDataNoCache(data, iOSData.length);
    free(data);

    m_pImgeView->setImage(image);
    m_pImgeView->setFrame(this->getBounds());
}

CATextFieldX* CATextFieldX::createWithFrame(const DRect& frame)
{
    CATextFieldX *textField = new CATextFieldX();
    if (textField && textField->initWithFrame(frame))
    {
        textField->autorelease();
        return textField;
    }
    CC_SAFE_DELETE(textField);
    return NULL;
}

CATextFieldX* CATextFieldX::createWithCenter(const DRect& rect)
{
    CATextFieldX* textField = new CATextFieldX();
    
    if (textField && textField->initWithCenter(rect))
    {
        textField->autorelease();
        return textField;
    }
    
    CC_SAFE_DELETE(textField);
    return NULL;
}

bool CATextFieldX::init()
{
    CGPoint point = CGPointMake(5000, 5000);
    m_pTextField = [[UITextField alloc]initWithFrame:CGRectMake(point.x, point.y, 100, 40)];
    EAGLView * eaglview = [EAGLView sharedEGLView];
    [eaglview addSubview:textField_iOS];
    [textField_iOS setBorderStyle:UITextBorderStyleRoundedRect];
    textField_iOS.placeholder = @"password";

    
    m_pImgeView = CAImageView::createWithFrame(DRect(0, 0, 1, 1));
    this->addSubview(m_pImgeView);
    return true;
}

void CATextFieldX::update(float dt)
{
    do
    {
        //CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);
        
        //CC_BREAK_IF(m_obLastPoint.equals(point) && m_obLastContentSize.equals(contentSize));

        CGFloat scale = [[UIScreen mainScreen] scale];
        CGRect rect = textField_iOS.frame;
        rect.origin.x = s_dip_to_px(point.x) / scale;
        rect.origin.y = s_dip_to_px(point.y) / scale;
        textField_iOS.frame = rect;
    }
    while (0);
}

void CATextFieldX::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
    
    DSize worldContentSize = DSizeApplyAffineTransform(m_obContentSize, worldToNodeTransform());
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect rect = textField_iOS.frame;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    textField_iOS.frame = rect;

    m_pImgeView->setFrame(this->getBounds());
}

bool CATextFieldX::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    
    return true;
}

void CATextFieldX::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CATextFieldX::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
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

void CATextFieldX::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    
}

NS_CC_END

