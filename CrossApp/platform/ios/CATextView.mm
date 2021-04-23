#include "platform/CATextView.h"
#include "platform/CADensityDpi.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
#import "EAGLView.h"
#import <UIKit/UIKit.h>


#define textView_iOS ((IOSTextView*)m_pTextView)
#define textView_iOS1 [textView_iOS iosTextView]

static std::map<CrossApp::CATextView*, std::function<bool()> > s_ShouldBeginEditing_map;
static std::map<CrossApp::CATextView*, std::function<bool()> > s_ShouldEndEditing_map;
static std::map<CrossApp::CATextView*, std::function<void()> > s_ShouldReturn_map;
static std::map<CrossApp::CATextView*, std::function<void(int height)> > s_KeyBoardHeight_map;
static std::map<CrossApp::CATextView*, std::function<bool(ssize_t, ssize_t, const std::string&)> > s_ShouldChangeCharacters_map;
static std::map<CrossApp::CATextView*, std::function<void()> > s_DidChangeText_map;

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
    if (_textView->getReturnType() != CrossApp::CATextView::ReturnType::Default)
    {
        if ([string isEqualToString:@"\n"])
        {
            //判断输入的字是否是回车，即按下return
            //在这里做你响应return键的代码
            if (s_ShouldReturn_map.count(_textView) > 0 && s_ShouldReturn_map[_textView])
            {
                s_ShouldReturn_map[_textView]();
            }
            else if (_textView->getDelegate())
            {
                _textView->getDelegate()->textViewShouldReturn(_textView);
            }
            
            return NO; //这里返回NO，就代表return键值失效，即页面上按下return，不会出现换行，如果为yes，则输入页面会换行
        }
    }
    
    if (s_ShouldChangeCharacters_map.count(_textView) > 0 && s_ShouldChangeCharacters_map[_textView])
    {
        return s_ShouldChangeCharacters_map[_textView](range.location, range.length, [string UTF8String]);
    }
    else if (_textView->getDelegate())
    {
        return _textView->getDelegate()->textViewShouldChangeCharacters(_textView, (unsigned)range.location, (unsigned)range.length, [string UTF8String]);
    }
    
    return YES;
}

- (void) keyboardWillWasShown:(NSNotification *) notif
{
    NSDictionary *info = [notif userInfo];
    
    CGFloat keyBoardHeight = [[info objectForKey:@"UIKeyboardBoundsUserInfoKey"] CGRectValue].size.height;
    CGRect begin = [[info objectForKey:@"UIKeyboardFrameBeginUserInfoKey"] CGRectValue];
    CGRect end = [[info objectForKey:@"UIKeyboardFrameEndUserInfoKey"] CGRectValue];

    if(begin.size.height > 0 && (begin.origin.y - end.origin.y > 0))
    {
        CGFloat scale  = [[UIScreen mainScreen] scale];
        int height = CrossApp::s_px_to_dip(keyBoardHeight * scale);
        
        if ([self.iosTextView isFirstResponder])
        {
            if (s_KeyBoardHeight_map.count(_textView) > 0 && s_KeyBoardHeight_map[_textView])
            {
                s_KeyBoardHeight_map[_textView](height);
            }
            else if (_textView->getDelegate())
            {
                _textView->getDelegate()->keyBoardHeight(_textView, height);
            }
        }
    }
}

- (void) keyboardWasHidden:(NSNotification *) notif
{
    if ([self.iosTextView isFirstResponder])
    {
        if (s_KeyBoardHeight_map.count(_textView) > 0 && s_KeyBoardHeight_map[_textView])
        {
            s_KeyBoardHeight_map[_textView](0);
        }
        else if (_textView->getDelegate())
        {
            _textView->getDelegate()->keyBoardHeight(_textView, 0);
        }
    }
}

@end


NS_CC_BEGIN
//catextView
CATextView::CATextView()
: m_pBackgroundView(nullptr)
, m_pShowImageView(nullptr)
, m_pTextView(nullptr)
, m_pDelegate(nullptr)
, m_iFontSize(40)
, m_eAlign(CATextView::Align::Left)
, m_eReturnType(CATextView::ReturnType::Default)
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
    s_ShouldBeginEditing_map.erase(this);
    s_ShouldEndEditing_map.erase(this);
    s_ShouldReturn_map.erase(this);
    s_KeyBoardHeight_map.erase(this);
    s_ShouldChangeCharacters_map.erase(this);
    s_DidChangeText_map.erase(this);
    
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
    const CAThemeManager::stringMap& map = CAApplication::getApplication()->getThemeManager()->getThemeMap("CATextField");
    CAImage* image = CAImage::create(map.at("backgroundView_normal"));
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
    if (this->isFirstResponder())
    {
        this->resignFirstResponder();
    }
}
bool CATextView::resignFirstResponder()
{
    if (s_ShouldEndEditing_map.count(this) > 0 && s_ShouldEndEditing_map[this] && !s_ShouldEndEditing_map[this]())
    {
        return false;
    }
    else if (m_pDelegate && (!m_pDelegate->textViewShouldEndEditing(this)))
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
    if (s_ShouldBeginEditing_map.count(this) > 0 && s_ShouldBeginEditing_map[this] && !s_ShouldBeginEditing_map[this]())
    {
        return false;
    }
    else if (m_pDelegate && (!m_pDelegate->textViewShouldBeginEditing(this)))
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
    this->cancelPreviousPerformRequests(callfunc_selector(CATextView::showImage));
    this->performSelector(callfunc_selector(CATextView::showImage), 0);
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
    
    CAData* ca_data = CAData::create();
    ca_data->fastSet(data, [iOSData length]);
    
    CAImage *image = CAImage::createWithImageDataNoCache(ca_data);
    
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
    CAScheduler::getScheduler()->schedule(schedule_selector(CATextView::update), this, 1/60.0f);
}
void CATextView::hideNativeTextView()
{
    CAScheduler::getScheduler()->unschedule(schedule_selector(CATextView::update), this);
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
void CATextView::setFontSize(const int &var)
{
    m_iFontSize = var;
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    
    textView_iOS1.font = [UIFont systemFontOfSize:s_dip_to_px(var) / scale];
    
    delayShowImage();
}

const int& CATextView::getFontSize()
{
    return m_iFontSize;
}

void CATextView::setReturnType(CATextView::ReturnType var)
{
    m_eReturnType = var;
}

CATextView::ReturnType CATextView::getReturnType()
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


void CATextView::setAlign(CATextView::Align var)
{
    m_eAlign = var;
    
    textView_iOS1.textAlignment = (NSTextAlignment)var;
    
    delayShowImage();
}

CATextView::Align CATextView::getAlign()
{
    return m_eAlign;
}

void CATextView::onShouldBeginEditing(const std::function<bool ()> &var)
{
    m_obShouldBeginEditing = var;
    s_ShouldBeginEditing_map[this] = var;
}

void CATextView::onShouldEndEditing(const std::function<bool ()> &var)
{
    m_obShouldEndEditing = var;
    s_ShouldEndEditing_map[this] = var;
}

void CATextView::onShouldReturn(const std::function<void ()> &var)
{
    m_obShouldReturn = var;
    s_ShouldReturn_map[this] = var;
}

void CATextView::onKeyBoardHeight(const std::function<void (int)> &var)
{
    m_obKeyBoardHeight = var;
    s_KeyBoardHeight_map[this] = var;
}

void CATextView::onShouldChangeCharacters(const std::function<bool (ssize_t, ssize_t, const std::string &)> &var)
{
    m_obShouldChangeCharacters = var;
    s_ShouldChangeCharacters_map[this] = var;
}

void CATextView::onDidChangeText(const std::function<void ()> &var)
{
    m_obDidChangeText = var;
    s_DidChangeText_map[this] = var;
}

NS_CC_END
