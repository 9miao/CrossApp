#include "platform/CATextView.h"
#include "platform/CADensityDpi.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#import "EAGLView.h"
#import <UIKit/UIKit.h>


#define textView_iOS ((IOSTextView*)m_pTextView)
#define textView_iOS1 [textView_iOS iosTextView]

@interface IOSTextView : UIView <UITextViewDelegate>
{

}
@property(nonatomic,assign) UITextView*   iosTextView;
@property(nonatomic,assign) CrossApp::CATextView* textView;

-(void)addIosTextView;
-(void)removeTextView;
-(void)setContentSize:(CGSize) size;
-(void)hide;
@end

@implementation IOSTextView
{
    
}

-(void)hide
{
    CGRect rect = self.frame;
    rect.origin = CGPointMake(-5000, -5000);
    self.frame = rect;
}

-(void)addIosTextView
{
    _iosTextView = [[[UITextView alloc]initWithFrame:[self bounds]] autorelease];
    _iosTextView.backgroundColor = nil;
    _iosTextView.delegate = self;
    [self addSubview:_iosTextView];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillWasShown:) name:UIKeyboardWillShowNotification object:nil];
    
    [[NSNotificationCenter defaultCenter]  addObserver:self selector:@selector(keyboardWasHidden:) name:UIKeyboardWillHideNotification object:nil];
}

-(void)removeTextView
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [self removeFromSuperview];
}

-(void)setContentSize:(CGSize)size
{
    CGRect rect = self.frame;
    rect.size = size;
    self.frame = rect;
    
    _iosTextView.frame = CGRectMake(0, 0, size.width, size.height);
}

- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)string
{
    if (_textView->getReturnType() != CrossApp::CATextView::Default)
    {
        if ([string isEqualToString:@"\n"])
        {
            //判断输入的字是否是回车，即按下return
            //在这里做你响应return键的代码
            if (_textView->getDelegate())
            {
                _textView->getDelegate()->textViewShouldReturn(_textView);
            }
            
            return NO; //这里返回NO，就代表return键值失效，即页面上按下return，不会出现换行，如果为yes，则输入页面会换行
        }
    }
    
    if (_textView->getDelegate())
    {
        std::string text = [string UTF8String];
        return _textView->getDelegate()->textViewShouldChangeCharacters(_textView,
                                                                          (unsigned int)range.location,
                                                                          (unsigned int)range.length,
                                                                          text);
    }
    
    
    return YES;
}

- (void) keyboardWillWasShown:(NSNotification *) notif
{
    NSDictionary *info = [notif userInfo];
    NSValue *value = [info objectForKey:UIKeyboardFrameEndUserInfoKey];
    CGSize keyboardSize = [value CGRectValue].size;
    
    CGFloat scale  = [[UIScreen mainScreen] scale];
    int height = CrossApp::s_px_to_dip(keyboardSize.height * scale);
    
    if (_textView->getDelegate() && [self.iosTextView isFirstResponder])
    {
        _textView->getDelegate()->keyBoardHeight(_textView, height);
    }
}

- (void) keyboardWasHidden:(NSNotification *) notif
{
    if (_textView->getDelegate() && [self.iosTextView isFirstResponder])
    {
        _textView->getDelegate()->keyBoardHeight(_textView, 0);
    }
}

@end


NS_CC_BEGIN
//catextView
CATextView::CATextView()
: m_pBackgroundView(NULL)
, m_pShowImageView(NULL)
, m_pTextView(NULL)
, m_pDelegate(NULL)
, m_iFontSize(40)
, m_eAlign(Left)
, m_eReturnType(Default)
, m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    this->setHaveNextResponder(false);
    this->setPoint(DPoint(-5000, -5000));
    m_pTextView = [[IOSTextView alloc]initWithFrame:CGRectMake(-5000, -5000, 500, 200)];
    [textView_iOS addIosTextView];
    EAGLView * eaglview = [EAGLView sharedEGLView];
    [eaglview addSubview:textView_iOS];
    [textView_iOS release];
    textView_iOS.textView = this;
    textView_iOS1.backgroundColor = nil;
    CGFloat scale = [[UIScreen mainScreen] scale];
    textView_iOS1.font = [UIFont systemFontOfSize:s_dip_to_px(m_iFontSize) / scale];
}
CATextView::~CATextView()
{
    [textView_iOS removeTextView];
    m_pDelegate = NULL;
}

CATextView* CATextView::createWithFrame(const DRect& frame)
{
    CATextView* textView = new CATextView();
    if (textView&&textView->initWithFrame(frame)) {
        textView->autorelease();
        return textView;
    }
    
    CC_SAFE_RELEASE_NULL(textView);
    return NULL;
}

CATextView* CATextView::createWithCenter(const CrossApp::DRect &rect)
{
    CATextView* textView = new CATextView();
    if (textView&&textView->initWithCenter(rect))
    {
        textView->autorelease();
        return textView;
    }
    
    CC_SAFE_RELEASE_NULL(textView);
    return NULL;
}

CATextView* CATextView::createWithLayout(const DLayout& layout)
{
    CATextView* textView = new CATextView();
    if (textView&&textView->initWithLayout(layout))
    {
        textView->autorelease();
        return textView;
    }
    
    CC_SAFE_RELEASE_NULL(textView);
    return NULL;
}

bool CATextView::init()
{
    CAImage* image = CAImage::create("source_material/textField_bg.png");
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
    m_pBackgroundView = CAScale9ImageView::createWithImage(image);
    m_pBackgroundView->setLayout(DLayoutFill);
    m_pBackgroundView->setCapInsets(capInsets);
    this->insertSubview(m_pBackgroundView, -1);

    m_pShowImageView = CAImageView::createWithLayout(DLayoutFill);
    this->addSubview(m_pShowImageView);
    m_pShowImageView->setTextTag("textView");

    return true;
}
void CATextView::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}
void CATextView::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
    this->resignFirstResponder();
}
bool CATextView::resignFirstResponder()
{
    if (m_pDelegate && (!m_pDelegate->textViewShouldEndEditing(this)))
    {
        return false;
    }
    
    bool result = CAView::resignFirstResponder();
    
    this->showImage();
    if ([textView_iOS1 isFirstResponder])
    {
        [textView_iOS1 resignFirstResponder];
    }
    this->delayShowImage();
    
    this->showTextView();
    
    this->hideNativeTextView();
    
    return result;

}
bool CATextView::becomeFirstResponder()
{
    if (m_pDelegate &&( !m_pDelegate->textViewShouldBeginEditing(this)))
    {
        return false;
    }
    
    bool result = CAView::becomeFirstResponder();
    
    this->hideTextView();
    
    [textView_iOS1 becomeFirstResponder];
    
    this->showNativeTextView();
    
    return result;
}
const DRect CATextView::convertRect(const CrossApp::DRect &rect)
{
    DRect cnvRect;
 
    return cnvRect;
}
void CATextView::update(float t)
{
    do
    {
        //CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);

        CGFloat scale = [[UIScreen mainScreen] scale];
        CGRect rect = textView_iOS.frame;
        rect.origin.x = s_dip_to_px(point.x) / scale;
        rect.origin.y = s_dip_to_px(point.y) / scale;
        textView_iOS.frame = rect;
        m_obLastPoint = point;
    }
    while (0);
}
void CATextView::delayShowImage()
{
    CC_RETURN_IF(CAScheduler::isScheduled(schedule_selector(CATextView::showImage), this));
    CAScheduler::schedule(schedule_selector(CATextView::showImage), this, 0, 0, 0);
}
void CATextView::showImage()
{
    static float scale = [UIScreen mainScreen].scale;
    UIGraphicsBeginImageContextWithOptions(textView_iOS.bounds.size, NO, scale);
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    [textView_iOS.layer renderInContext:context];
    UIImage *image_iOS = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    NSData* iOSData = UIImagePNGRepresentation(image_iOS);
    //[image_iOS release];
    
    unsigned char* data = (unsigned char*)malloc([iOSData length]);
    [iOSData getBytes:data];
    
    CAImage *image = CAImage::createWithImageDataNoCache(data, iOSData.length);
    free(data);
    
    m_pShowImageView->setImage(image);
}
void CATextView::showTextView()
{
    m_pShowImageView->setVisible(true);
}
void CATextView::hideTextView()
{
    m_pShowImageView->setVisible(false);
}
void CATextView::showNativeTextView()
{
    this->update(0);
    CAScheduler::schedule(schedule_selector(CATextView::update), this, 1/60.0f);
}
void CATextView::hideNativeTextView()
{
    CAScheduler::unschedule(schedule_selector(CATextView::update), this);
    [textView_iOS performSelector:@selector(hide) withObject:nil afterDelay:1/60.0f];
}
void CATextView::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);

    DSize worldContentSize = this->convertToWorldSize(m_obContentSize);
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect rect = textView_iOS.frame;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    [textView_iOS setContentSize:rect.size];
    
    if (m_bRunning)
    {
        this->showImage();
    }
}
bool CATextView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    return true;
}

void CATextView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CATextView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
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

void CATextView::setText(const std::string &var)
{
    m_sText = var;
    
    textView_iOS1.text = [NSString stringWithUTF8String:m_sText.c_str()];
    
    delayShowImage();
}
const std::string& CATextView::getText()
{
    m_sText = [textView_iOS1.text UTF8String];
    return m_sText;
}
void CATextView::setTextColor(const CAColor4B &var)
{
    m_sTextColor = var;
    
    textView_iOS1.textColor = [UIColor colorWithRed:var.r/255.f green:var.g/255.f blue:var.b/255.f alpha:var.a];
    
    delayShowImage();
}
const CAColor4B& CATextView::getTextColor()
{
    return m_sTextColor;
}
void CATextView::setTextFontSize(const int &var)
{
    m_iFontSize = var;
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    
    textView_iOS1.font = [UIFont systemFontOfSize:s_dip_to_px(var) / scale];
    
    delayShowImage();
}

const int& CATextView::getTextFontSize()
{
    return m_iFontSize;
}

void CATextView::setReturnType(const ReturnType& var)
{
    m_eReturnType = var;
}

const CATextView::ReturnType& CATextView::getReturnType()
{
    return m_eReturnType;
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


void CATextView::setTextViewAlign(const TextViewAlign &var)
{
    m_eAlign = var;
    
    textView_iOS1.textAlignment = (NSTextAlignment)var;
    
    delayShowImage();
}
const CATextView::TextViewAlign& CATextView::getTextViewAlign()
{
    return m_eAlign;
}

NS_CC_END