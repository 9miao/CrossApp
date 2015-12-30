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
@end

@implementation IOSTextView
{
    
}
-(void)addIosTextView
{
    _iosTextView = [[[UITextView alloc]initWithFrame:[self bounds]] autorelease];
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
    rect.size.width = size.width;
    rect.size.height = size.height;
    self.frame = rect;
    
    _iosTextView.frame = self.bounds;
}

- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    
    NSLog(@"shouldChangeTextInRange --%lu--%lu--%@",(unsigned long)range.location,(unsigned long)range.length,text);

    
    if ([text isEqualToString:@"\n"]){
        //判断输入的字是否是回车，即按下return
        //在这里做你响应return键的代码
        if (_textView->getDelegate())
        {
            _textView->getDelegate()->textViewShouldReturn(_textView);
        }
        
        return NO; //这里返回NO，就代表return键值失效，即页面上按下return，不会出现换行，如果为yes，则输入页面会换行
    }
    
    
    std::string insert = [text cStringUsingEncoding:NSUTF8StringEncoding];
    std::string cur = [[_iosTextView text] cStringUsingEncoding:NSUTF8StringEncoding];
    
    int dele    = 0;
    int inse  = 0;
    
    if (range.length>0) {
        dele = (int)range.length;
        
    }else{
        inse = (int)text.length;
    }
    
    
    if (_textView->getDelegate())
    {
       _textView->getDelegate()->textViewAfterTextChanged(_textView, cur.c_str(), insert.c_str(), (int)range.location, dele, inse);
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
    

    if (_textView->getDelegate())
    {
        _textView->getDelegate()->keyBoardHeight(_textView, height);
    }
}

- (void) keyboardWasHidden:(NSNotification *) notif
{

    if (_textView->getDelegate())
    {
        _textView->getDelegate()->keyBoardHeight(_textView, 0);
    }
}

@end


NS_CC_BEGIN
//catextView
CATextView::CATextView()
:m_obLastPoint(DPoint(-0xffff, -0xffff))
,m_pDelegate(NULL)
{
    this->setHaveNextResponder(false);
}
CATextView::~CATextView()
{
    textView_iOS.removeTextView;
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
CATextView* CATextView::createWithCenter(const CrossApp::DRect &rect){
    return NULL;
}
bool CATextView::init()
{
    //curText
    m_pTextView = [[IOSTextView alloc]initWithFrame:CGRectMake(200,200,100,40)];
    [textView_iOS addIosTextView];
    EAGLView * eaglview = [EAGLView sharedEGLView];
    [eaglview addSubview:textView_iOS];
    textView_iOS.textView = this;
    textView_iOS.backgroundColor = 0;
    textView_iOS1.backgroundColor = 0;
    textView_iOS1.delegate = textView_iOS;

    //bg
    CAImage* image = CAImage::create("source_material/textView_bg.png");
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
    m_pBackgroundView = CAScale9ImageView::createWithFrame(DRectZero);
    m_pBackgroundView->setImage(image);
    m_pBackgroundView->setCapInsets(capInsets);
    this->insertSubview(m_pBackgroundView, -1);

    //show
    m_pShowImageView = CAImageView::createWithFrame(DRect(0,0,0,0));
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
}
bool CATextView::resignFirstResponder()
{
    if (m_pDelegate && (!m_pDelegate->textViewShouldEndEditing(this)))
    {
        return false;
    }
    
    bool result = CAView::resignFirstResponder();
    
    if ([textView_iOS1 isFirstResponder])
    {
        [textView_iOS1 resignFirstResponder];
    }
    this->showImage();
    
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
    
    CAViewAnimation::beginAnimations(m_s__StrID + "hideTextView", NULL);
    CAViewAnimation::setAnimationDuration(0);
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextView::hideTextView));
    CAViewAnimation::commitAnimations();
    
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
        
        CC_BREAK_IF(m_obLastPoint.equals(point));
        
        
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
    if (!CAViewAnimation::areBeginAnimationsWithID(m_s__StrID + "showImage"))
    {
        CAViewAnimation::beginAnimations(m_s__StrID + "showImage", NULL);
        CAViewAnimation::setAnimationDuration(0);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextView::showImage));
        CAViewAnimation::commitAnimations();
    }
    this->updateDraw();
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
    m_pShowImageView->setFrame(this->getBounds());
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
    CAScheduler::schedule(schedule_selector(CATextView::update), this, 1/60.0f);
}
void CATextView::hideNativeTextView()
{
    CAScheduler::unschedule(schedule_selector(CATextView::update), this);
    CGRect rect = textView_iOS.frame;
    rect.origin.x = -10000;
    rect.origin.y = -10000;
    textView_iOS.frame = rect;
    m_obLastPoint = DPoint(-10000, -10000);
}
void CATextView::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);

    DSize worldContentSize = DSizeApplyAffineTransform(m_obContentSize, worldToNodeTransform());
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect rect = textView_iOS.frame;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    [textView_iOS setContentSize:rect.size];
    
    m_pBackgroundView->setFrame(this->getBounds());
    m_pShowImageView->setFrame(this->getBounds());
}
bool CATextView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
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

void CATextView::setBackgroundImage(CAImage* image)
{
    if (image)
    {
        DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
        m_pBgImageView->setCapInsets(capInsets);
    }
    m_pBgImageView->setImage(image);
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