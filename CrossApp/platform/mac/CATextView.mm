#include "platform/CATextView.h"
#include "platform/CADensityDpi.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#import "EAGLView.h"
#import <Cocoa/Cocoa.h>

#define textView_Mac ((MacTextView*)m_pTextView)

#ifdef NSTextAlignmentLeft
#else
#define NSTextAlignmentLeft NSLeftTextAlignment
#define NSTextAlignmentCenter NSCenterTextAlignment
#define NSTextAlignmentRight NSRightTextAlignment
#endif

#define MAC_SCALE 1//[[NSScreen mainScreen] backingScaleFactor]

@interface MacTextView: NSTextField
{
    
}

@property(nonatomic,assign) CrossApp::CATextView* textView;
@property(nonatomic,copy)   NSString* beforeText;
@end

@implementation MacTextView
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
    
    
    if (_textView->getReturnType() != CrossApp::CATextView::Default)
    {
        if ([theEvent keyCode] == 36||
            [theEvent keyCode] == 76)
        {
            _textView->resignFirstResponder();
            if (_textView->getDelegate())
            {
                _textView->getDelegate()->textViewShouldReturn(_textView);
            }
        }
    }
    
}

- (unsigned int)getLocationWithBefore:(NSString*)before Current:(NSString*)current
{
    unsigned int location = 0;
    
    for(unsigned int i=0; i<before.length; i++)
    {
        if (i == current.length)
        {
            break;
        }
        
        unichar before_char = [before characterAtIndex: i];
        unichar current_char = [current characterAtIndex: i];
        
        if(before_char != current_char)
        {
            break;
        }
        
        location = before.length + 1;
    }
    
    return location;
}

- (unsigned int)getLenghtWithBefore:(NSString*)before Current:(NSString*)current Location:(unsigned int)location
{
    unsigned int lenght = 0;
    
    for(unsigned int i=location; i<before.length; i++)
    {
        if (i == current.length)
        {
            lenght = before.length - location;
            break;
        }
        
        unichar before_char = [before characterAtIndex: i];
        
        unichar current_char = [current characterAtIndex: i];
        
        if(before_char == current_char)
        {
            break;
        }
        
        lenght = i + 1 - location;
    }
    
    return lenght;
}

- (void)textDidChange:(NSNotification *)notification
{
    NSString* before = [self beforeText];
    NSString* current = [self stringValue];
    
    unsigned int location = [self getLocationWithBefore:before Current:current];
    unsigned int lenght = [self getLenghtWithBefore:before Current:current Location:location];
    
    unsigned int addLenght = current.length - (before.length - lenght);
    
    std::string changedText = "";
    
    if (addLenght > 0)
    {
        changedText = [[current substringWithRange:NSMakeRange(location, addLenght)] UTF8String];
    }
    else
    {
        changedText = "";
    }
    
    if (_textView->getDelegate() && !_textView->getDelegate()->textViewShouldChangeCharacters(_textView, location, lenght, changedText))
    {
        [self setStringValue:before];
    }
    else
    {
        [self setBeforeText:current];
        [self setStringValue:current];
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
:m_obLastPoint(DPoint(-0xffff, -0xffff))
,m_pDelegate(NULL)
,m_iFontSize(40)
{
    this->setHaveNextResponder(false);
    
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
    [textView_Mac removeFromSuperview];
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
    if (m_pDelegate && (!m_pDelegate->textViewShouldEndEditing(this)))
    {
        return false;
    }
    
    bool result = CAView::resignFirstResponder();
    
    [textView_Mac resignFirstResponder];

    this->showTextView();
    
    this->showImage();
    
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
    CC_RETURN_IF(CAScheduler::isScheduled(schedule_selector(CATextView::showImage), this));
    CAScheduler::schedule(schedule_selector(CATextView::showImage), this, 0, 0, 0);
}
void CATextView::showImage()
{
    NSImage* image_MAC = [[[NSImage alloc]initWithData:[textView_Mac dataWithPDFInsideRect:[textView_Mac bounds]]]autorelease];
    
    NSData* data_MAC = [image_MAC TIFFRepresentation];
    
    unsigned char* data = (unsigned char*)malloc([data_MAC length]);
    [data_MAC getBytes:data];
    
    CAImage *image = CAImage::createWithImageDataNoCache(data, data_MAC.length);
    free(data);
    m_pShowImageView->setImage(image);
    
    this->updateDraw();
    CAScheduler::unschedule(schedule_selector(CATextView::showImage), this);
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
    CAScheduler::schedule(schedule_selector(CATextView::update), this, 1/60.0f);
}
void CATextView::hideNativeTextView()
{
    CAScheduler::unschedule(schedule_selector(CATextView::update), this);
    
    [textView_Mac hide];
}
void CATextView::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
    
    DSize worldContentSize = this->convertToWorldSize(m_obContentSize);
    
    NSSize size;
    size.width = s_dip_to_px(worldContentSize.width) / MAC_SCALE;
    size.height =  s_dip_to_px(worldContentSize.height) / MAC_SCALE;
    
    NSRect rect = [textView_Mac frame];
    rect.size = size;
    textView_Mac.frame = rect;
    
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
    
    textView_Mac.textColor = [NSColor colorWithRed:var.r/255.f green:var.g/255.f blue:var.b/255.f alpha:var.a];
    
    delayShowImage();
}
const CAColor4B& CATextView::getTextColor()
{
    return m_sTextColor;
}
void CATextView::setTextFontSize(const int &var)
{
    m_iFontSize = var;
    
    textView_Mac.font = [NSFont systemFontOfSize:s_dip_to_px(var) / MAC_SCALE];
    
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
        m_pBackgroundView->setCapInsets(capInsets);
    }
    m_pBackgroundView->setImage(image);
}


void CATextView::setTextViewAlign(const TextViewAlign &var)
{
    m_eAlign = var;
    
    switch (var)
    {
        case CATextView::Left :
            [textView_Mac setAlignment:NSTextAlignmentLeft];
            break;
        case CATextView::Center:
            [textView_Mac setAlignment:NSTextAlignmentCenter];
            break;
        case CATextView::Right:
            [textView_Mac setAlignment:NSTextAlignmentRight];
            break;
        default:
            break;
    }
    
    delayShowImage();
}
const CATextView::TextViewAlign& CATextView::getTextViewAlign()
{
    return m_eAlign;
}
void CATextView::setReturnType(const ReturnType &var)
{
    m_eReturnType = var;
    
    
}
const CATextView::ReturnType& CATextView::getReturnType()
{
    return m_eReturnType;
}



NS_CC_END