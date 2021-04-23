#include "platform/CATextView.h"
#include "platform/CADensityDpi.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#import "EAGLView.h"
#import <Cocoa/Cocoa.h>
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
#define textView_Mac ((MacTextView*)m_pTextView)

#ifdef NSTextAlignmentLeft
#else
#define NSTextAlignmentLeft NSLeftTextAlignment
#define NSTextAlignmentCenter NSCenterTextAlignment
#define NSTextAlignmentRight NSRightTextAlignment
#endif

#define MAC_SCALE 1//[[NSScreen mainScreen] backingScaleFactor]

static std::map<CrossApp::CATextView*, std::function<bool()> > s_ShouldBeginEditing_map;
static std::map<CrossApp::CATextView*, std::function<bool()> > s_ShouldEndEditing_map;
static std::map<CrossApp::CATextView*, std::function<void()> > s_ShouldReturn_map;
static std::map<CrossApp::CATextView*, std::function<void(int height)> > s_KeyBoardHeight_map;
static std::map<CrossApp::CATextView*, std::function<bool(ssize_t, ssize_t, const std::string&)> > s_ShouldChangeCharacters_map;
static std::map<CrossApp::CATextView*, std::function<void()> > s_DidChangeText_map;

@interface MacTextView: NSTextField
{
    
}

@property(nonatomic,assign) CrossApp::CATextView* textView;
@property(nonatomic,copy)   NSString* beforeText;
@end

@implementation MacTextView
{
    
}

-(id)initWithFrame:(NSRect)frameRect
{
    if ([super initWithFrame:frameRect])
    {
        [self setBackgroundColor:[NSColor clearColor]];
        [self setBezeled:NO];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(fullScreenChanged) name:@"FULL_SCREEN_CHANGED" object:nil];
        return self;
    }
    return nil;
}

- (void)fullScreenChanged
{

}

-(void)setText:(NSString* )value
{
    self.stringValue = value;
    self.beforeText = value;
}
-(void)hide
{
    [self setWantsLayer:NO];
    [self setFrameOrigin:NSMakePoint(-50000, -50000)];
}

-(void)show
{
    [self setWantsLayer:YES];
}

- (void) keyUp:(NSEvent *)theEvent
{
    [super keyUp:theEvent];
    
    
    if (_textView->getReturnType() != CrossApp::CATextView::ReturnType::Default)
    {
        if ([theEvent keyCode] == 36||
            [theEvent keyCode] == 76)
        {
            _textView->resignFirstResponder();
            if (s_ShouldReturn_map.count(_textView) > 0 && s_ShouldReturn_map[_textView])
            {
                s_ShouldReturn_map[_textView]();
            }
            else if (_textView->getDelegate())
            {
                _textView->getDelegate()->textViewShouldReturn(_textView);
            }
        }
    }
    
}

- (ssize_t)getLocationWithBefore:(NSString*)before Current:(NSString*)current
{
    ssize_t location = 0;
    
    for(ssize_t i=0; i<before.length; i++)
    {
        if (i == current.length)
        {
            location = i;
            break;
        }
        
        unichar before_char = [before characterAtIndex: i];
        unichar current_char = [current characterAtIndex: i];
        
        if(before_char != current_char)
        {
            location = i;
            break;
        }
        
        location = (ssize_t)before.length;
    }
    
    return location;
}

- (ssize_t)getLengthWithBefore:(NSString*)before Current:(NSString*)current Location:(ssize_t)location
{
    ssize_t length = 0;
    
    for(ssize_t i=location; i<before.length; i++)
    {
        if (i == current.length)
        {
            length = i + 1 - location;
            break;
        }
        
        unichar before_char = [before characterAtIndex: i];
        
        unichar current_char = [current characterAtIndex: i];
        
        if(before_char != current_char)
        {
            if (before.length > current.length)
            {
                length = (unsigned int)(before.length - current.length);
            }
            else if (before.length < current.length)
            {
                length = i - location;
            }
            else
            {
                length = 0;
            }
            
            break;
        }
        
        length = (ssize_t)(before.length - location);
    }
    
    return length;
}

- (void)textDidChange:(NSNotification *)notification
{
    NSString* before = [NSString stringWithString:[self beforeText]];
    NSString* current = [NSString stringWithString:[self stringValue]];;
    
    ssize_t location = [self getLocationWithBefore:before Current:current];
    ssize_t length = [self getLengthWithBefore:before Current:current Location:location];
    ssize_t addLength = MAX(current.length - (before.length - length), 0);
    
    std::string changedText = "";
    
    if (addLength > 0)
    {
        changedText = [[current substringWithRange:NSMakeRange(location, addLength)] UTF8String];
    }
    else
    {
        changedText = "";
    }
    bool changedFlag = true;
    if (s_ShouldChangeCharacters_map.count(_textView) > 0 && s_ShouldChangeCharacters_map[_textView])
    {
        changedFlag = s_ShouldChangeCharacters_map[_textView](location, length, changedText);
    }
    else if (_textView->getDelegate())
    {
        changedFlag = _textView->getDelegate()->textViewShouldChangeCharacters(_textView, (unsigned)location, (unsigned)length, changedText);
    }
    
    if (!changedFlag)
    {
        [self setStringValue:before];
    }
    else
    {
        
        [self setBeforeText:current];
        [self setStringValue:current];
        
//        if (s_DidChangeText_map.count(_textView) > 0 && s_DidChangeText_map[_textView])
//        {
//            s_DidChangeText_map[_textView]();
//        }
//        else if (_textView->getDelegate())
//        {
//            _textView->getDelegate()->textViewDidChangeText(_textView);
//        }
    }
}

- (BOOL)performKeyEquivalent:(NSEvent *)event
{
    if (([event modifierFlags] & NSDeviceIndependentModifierFlagsMask) == NSCommandKeyMask)
    {
        // The command key is the ONLY modifier key being pressed.
        if ([[event charactersIgnoringModifiers] isEqualToString:@"x"])
        {
            return [NSApp sendAction:@selector(cut:) to:[[self window] firstResponder] from:self];
        }
        else if ([[event charactersIgnoringModifiers] isEqualToString:@"c"])
        {
            return [NSApp sendAction:@selector(copy:) to:[[self window] firstResponder] from:self];
        }
        else if ([[event charactersIgnoringModifiers] isEqualToString:@"v"])
        {
            return [NSApp sendAction:@selector(paste:) to:[[self window] firstResponder] from:self];
        }
        else if ([[event charactersIgnoringModifiers] isEqualToString:@"a"])
        {
            return [NSApp sendAction:@selector(selectAll:) to:[[self window] firstResponder] from:self];
        }
    }
    return [super performKeyEquivalent:event];
}

@end


NS_CC_BEGIN
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
    CGPoint point = CGPointMake(-50000, -50000);
    m_pTextView = [[MacTextView alloc]initWithFrame:CGRectMake(point.x, point.y, 100, 40)];
    EAGLView * eaglview = [EAGLView sharedEGLView];
    [eaglview addSubview:textView_Mac];
    textView_Mac.textView = this;
    [textView_Mac setText:@""];
    [textView_Mac release];
    
    textView_Mac.font = [NSFont systemFontOfSize:s_dip_to_px(m_iFontSize) / MAC_SCALE];
}

CATextView::~CATextView()
{
    s_ShouldBeginEditing_map.erase(this);
    s_ShouldEndEditing_map.erase(this);
    s_ShouldReturn_map.erase(this);
    s_KeyBoardHeight_map.erase(this);
    s_ShouldChangeCharacters_map.erase(this);
    s_DidChangeText_map.erase(this);
    
    [[NSNotificationCenter defaultCenter] removeObserver:textView_Mac];
    [textView_Mac removeFromSuperview];
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
    if (textView && textView->initWithCenter(rect))
    {
        textView->autorelease();
        return textView;
    }
    
    CC_SAFE_DELETE(textView);
    return NULL;
}

CATextView* CATextView::createWithLayout(const DLayout& layout)
{
    CATextView* textView = new CATextView();
    if (textView && textView->initWithLayout(layout))
    {
        textView->autorelease();
        return textView;
    }
    
    CC_SAFE_DELETE(textView);
    return NULL;
}

bool CATextView::init()
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CATextField");
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
    CAControl::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextView::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
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
    
    bool result = CAControl::resignFirstResponder();
    
    [textView_Mac resignFirstResponder];

    this->showTextView();
    
    this->showImage();
    
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
    
    bool result = CAControl::becomeFirstResponder();
    
    [textView_Mac becomeFirstResponder];
    
    this->showNativeTextView();

    this->hideTextView();
    
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
        CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);
        point.y = CAApplication::getApplication()->getWinSize().height - point.y;
        point.y = point.y - m_obContentSize.height;
        //        CC_BREAK_IF(m_obLastPoint.equals(point));
        
        NSPoint origin;
        origin.x = s_dip_to_px(point.x) / MAC_SCALE;
        origin.y = s_dip_to_px(point.y) / MAC_SCALE;
        [textView_Mac setFrameOrigin:origin];
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
    NSImage* image_MAC = [[[NSImage alloc] initWithData:[textView_Mac dataWithPDFInsideRect:[textView_Mac bounds]]] autorelease];
    
    NSData* data_MAC = [image_MAC TIFFRepresentationUsingCompression:NSTIFFCompressionNone factor:MAC_SCALE];
    
    unsigned char* pData = (unsigned char*)malloc([data_MAC length]);
    [data_MAC getBytes:pData length:[data_MAC length]];
    
    CAData* data = CAData::create();
    data->fastSet(pData, [data_MAC length]);
    CAImage *image = CAImage::createWithImageDataNoCache(data);
    
    m_pShowImageView->setLayout(DLayoutFill);
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
    [textView_Mac show];
    CAScheduler::getScheduler()->schedule(schedule_selector(CATextView::update), this, 1/60.0f);
}

void CATextView::hideNativeTextView()
{
    CAScheduler::getScheduler()->unschedule(schedule_selector(CATextView::update), this);
    
    [textView_Mac hide];
}

void CATextView::setContentSize(const DSize& contentSize)
{
    CAControl::setContentSize(contentSize);
    
    DSize worldContentSize = this->convertToWorldSize(m_obContentSize);
    
    NSSize size;
    size.width = s_dip_to_px(worldContentSize.width) / MAC_SCALE;
    size.height =  s_dip_to_px(worldContentSize.height) / MAC_SCALE;
    
    NSRect rect = [textView_Mac frame];
    rect.size = size;
    textView_Mac.frame = rect;
        
    this->showImage();
}

bool CATextView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
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
    
}

void CATextView::setText(const std::string &var)
{
    m_sText = var;
    
    [textView_Mac setText:[NSString stringWithUTF8String:m_sText.c_str()]];
    //textView_Mac.stringValue = [NSString stringWithUTF8String:m_sText.c_str()];
    
    delayShowImage();
}

const std::string& CATextView::getText()
{
    m_sText = [textView_Mac.stringValue UTF8String];
    return m_sText;
}

void CATextView::setTextColor(const CAColor4B &var)
{
    m_sTextColor = var;
    
    textView_Mac.textColor = [NSColor colorWithRed:var.r/255.f green:var.g/255.f blue:var.b/255.f alpha:var.a/255.f];
    
    delayShowImage();
}

const CAColor4B& CATextView::getTextColor()
{
    return m_sTextColor;
}

void CATextView::setFontSize(const int &var)
{
    m_iFontSize = var;
    
    textView_Mac.font = [NSFont systemFontOfSize:s_dip_to_px(var) / MAC_SCALE];
    
    delayShowImage();
}

const int& CATextView::getFontSize()
{
    return m_iFontSize;
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
    
    switch (var)
    {
        case CATextView::Align::Left :
            [textView_Mac setAlignment:NSTextAlignmentLeft];
            break;
        case CATextView::Align::Center:
            [textView_Mac setAlignment:NSTextAlignmentCenter];
            break;
        case CATextView::Align::Right:
            [textView_Mac setAlignment:NSTextAlignmentRight];
            break;
        default:
            break;
    }
    
    delayShowImage();
}

CATextView::Align CATextView::getAlign()
{
    return m_eAlign;
}

void CATextView::setReturnType(CATextView::ReturnType var)
{
    m_eReturnType = var;
}

CATextView::ReturnType CATextView::getReturnType()
{
    return m_eReturnType;
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
