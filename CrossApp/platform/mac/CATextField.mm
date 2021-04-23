//
//  CATextField.cpp
//  tesss
//
//  Created by 栗元峰 on 15/11/20.
//
//

#include "platform/CATextField.h"
#include "platform/CADensityDpi.h"
#include "control/CAButton.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#import "EAGLView.h"
#import <Cocoa/Cocoa.h>
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
#define textField_MAC ((MACTextField*)m_pTextField)

#ifdef NSTextAlignmentLeft
#else
#define NSTextAlignmentLeft NSLeftTextAlignment
#define NSTextAlignmentCenter NSCenterTextAlignment
#define NSTextAlignmentRight NSRightTextAlignment
#endif

#define MAC_SCALE 1//[[NSScreen mainScreen] backingScaleFactor]

static std::map<CrossApp::CATextField*, std::function<bool()> > s_ShouldBeginEditing_map;
static std::map<CrossApp::CATextField*, std::function<bool()> > s_ShouldEndEditing_map;
static std::map<CrossApp::CATextField*, std::function<void()> > s_ShouldReturn_map;
static std::map<CrossApp::CATextField*, std::function<void(int height)> > s_KeyBoardHeight_map;
static std::map<CrossApp::CATextField*, std::function<bool(ssize_t, ssize_t, const std::string&)> > s_ShouldChangeCharacters_map;
static std::map<CrossApp::CATextField*, std::function<void()> > s_DidChangeText_map;

@interface MACTextFieldCell: NSTextFieldCell
{
}

@end

@implementation MACTextFieldCell
{
    
}

- (NSRect)adjustedFrameToVerticallyCenterText:(NSRect)frame
{
    // super would normally draw text at the top of the cell
    NSInteger offset = floor((NSHeight(frame) -
                              ([[self font] ascender] - [[self font] descender])) / 2);
    return NSInsetRect(frame, 0.0, offset);
}

- (void)editWithFrame:(NSRect)aRect inView:(NSView *)controlView
               editor:(NSText *)editor delegate:(id)delegate event:(NSEvent *)event
{
    [super editWithFrame:[self adjustedFrameToVerticallyCenterText:aRect]
                  inView:controlView editor:editor delegate:delegate event:event];
}

- (void)selectWithFrame:(NSRect)aRect inView:(NSView *)controlView
                 editor:(NSText *)editor delegate:(id)delegate
                  start:(NSInteger)start length:(NSInteger)length
{
    [super selectWithFrame:[self adjustedFrameToVerticallyCenterText:aRect]
                    inView:controlView editor:editor delegate:delegate
                     start:start length:length];
}

- (void)drawInteriorWithFrame:(NSRect)frame inView:(NSView *)view
{
    [super drawInteriorWithFrame:
     [self adjustedFrameToVerticallyCenterText:frame] inView:view];
}

@end

@interface MACTextField: NSTextField
{
    BOOL _isShouldEdit;
    int _marginLeft;
    int _marginRight;
}

@property(nonatomic,assign) CrossApp::CATextField* textField;
@property(nonatomic,retain) NSString* beforeText;


-(void)setMarginLeft:(int)marginLeft;

-(int)getMarginLeft;

-(void)setMarginRight:(int)marginRight;

-(int)getMarginRight;

-(void)setFrameOrigin:(NSPoint)newOrigin;

-(void)setContentSize:(NSSize)newSize;

-(CrossApp::DRect)getDRect;


@end

@implementation MACTextField
{
    
}

- (void)fullScreenChanged
{

}

-(void)setText:(NSString* )value
{
    self.stringValue = value;
    self.beforeText = value;
}

-(void)setMarginLeft:(int)marginLeft
{
    int olbMarginLeft = _marginLeft;
    CGFloat scale = MAC_SCALE;
    _marginLeft = CrossApp::s_dip_to_px(marginLeft) / scale;
    
    NSRect rect = self.frame;
    rect.size.width -= _marginLeft - olbMarginLeft;
    [self setFrame:rect];
}

-(int)getMarginLeft
{
    CGFloat scale = MAC_SCALE;
    return CrossApp::s_px_to_dip(_marginLeft) * scale;
}

-(void)setMarginRight:(int)marginRight
{
    int olbMarginRight = _marginRight;
    CGFloat scale = MAC_SCALE;
    _marginRight = CrossApp::s_dip_to_px(marginRight) / scale;
    
    NSRect rect = self.frame;
    rect.size.width -= _marginRight - olbMarginRight;
    [self setFrame:rect];
}

-(int)getMarginRight
{
    CGFloat scale = MAC_SCALE;
    return CrossApp::s_px_to_dip(_marginRight) * scale;
}

-(void)setFrameOrigin:(NSPoint)newOrigin
{
    newOrigin.x += _marginLeft;
    [super setFrameOrigin:newOrigin];
}

-(void)setContentSize:(NSSize)newSize
{
    newSize.width -= (_marginLeft + _marginRight);
    NSRect rect = self.frame;
    rect.size = newSize;
    [self setFrame:rect];
}

-(CrossApp::DRect)getDRect
{
    CrossApp::DRect rect;
    CGFloat scale = MAC_SCALE;
    rect.origin.x = CrossApp::s_px_to_dip(_marginLeft) * scale;
    rect.origin.y = CrossApp::s_px_to_dip(0) * scale;
    rect.size.width = CrossApp::s_px_to_dip(self.frame.size.width) * scale;
    rect.size.height = CrossApp::s_px_to_dip(self.frame.size.height) * scale;
    return rect;
}

-(id)initWithFrame:(NSRect)frameRect
{
    if ([super initWithFrame:frameRect])
    {
        [self setBackgroundColor:[NSColor clearColor]];
        [self setBezeled:NO];
        
        {
            NSTextFieldCell* oldCell = [self cell];
            MACTextFieldCell* cell = [[MACTextFieldCell alloc]initTextCell:@""];
            
            [cell setState:[oldCell state]];
            [cell setTarget:[oldCell target]];
            [cell setAction:[oldCell action]];
            [cell setEnabled:[oldCell isEnabled]];
            [cell setContinuous:[oldCell isContinuous]];
            [cell setEditable:[oldCell isEditable]];
            [cell setSelectable:[oldCell isSelectable]];
            [cell setBordered:[oldCell isBordered]];
            [cell setBezeled:[oldCell isBezeled]];
            [cell setScrollable:[oldCell isScrollable]];
            [cell setHighlighted:[oldCell isHighlighted]];
            [cell setWraps:[oldCell wraps]];
            [cell setFont:[oldCell font]];
            [cell setFormatter:[oldCell formatter]];
            [cell setObjectValue:[cell objectValue]];

            [self setCell:cell];
            [cell release];
        }
        
        
        [[self cell] setAlignment:NSTextAlignmentLeft];
        [[self cell] setLineBreakMode:NSLineBreakByTruncatingTail];
        _marginLeft = 0;
        _marginRight = 0;
        
        [self setBeforeText:@"" ];
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(fullScreenChanged) name:@"FULL_SCREEN_CHANGED" object:nil];

        return self;
    }
    return nil;
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

- (ssize_t)getLocationWithBefore:(NSString*)before Current:(NSString*)current
{
    ssize_t location = 0;
    
    for(unsigned int i=0; i<before.length; i++)
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
        
        location = before.length;
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
                length = before.length - current.length;
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

        length = before.length - location;
    }
    
    return length;
}

- (void)textDidChange:(NSNotification *)notification
{
    NSString* before = [NSString stringWithString:[self beforeText]];
    NSString* current = [NSString stringWithString:[self stringValue]];;
    
    ssize_t location = [self getLocationWithBefore:before Current:current];
    ssize_t length = [self getLengthWithBefore:before Current:current Location:location];
    ssize_t addLength = MAX((ssize_t)(current.length - (before.length - (ssize_t)length)), 0);

    std::string changedText = "";
    
    if (addLength > 0)
    {
        changedText = [[current substringWithRange:NSMakeRange(location, addLength)] UTF8String];
    }
    else
    {
        changedText = "";
    }
    
    
    int maxLength = _textField->getMaxLength();
    if (maxLength > 0 && maxLength < current.length)
    {
        if (before.length < maxLength)
        {
            [self setStringValue:[current substringToIndex:maxLength]];
        }
        else
        {
            [self setStringValue:[self beforeText]];
        }
    }
    else
    {
        bool changedFlag = true;
        if (s_ShouldChangeCharacters_map.count(_textField) > 0 && s_ShouldChangeCharacters_map[_textField])
        {
            changedFlag = s_ShouldChangeCharacters_map[_textField](location, length, changedText);
        }
        else if (_textField->getDelegate())
        {
            changedFlag = _textField->getDelegate()->textFieldShouldChangeCharacters(_textField, (unsigned)location, (unsigned)length, changedText);
        }
        
        if (!changedFlag)
        {
            [self setStringValue:before];
        }
        else
        {
            [self setBeforeText:current];
            [self setStringValue:current];
            
            if (s_DidChangeText_map.count(_textField) > 0 && s_DidChangeText_map[_textField])
            {
                s_DidChangeText_map[_textField]();
            }
            else if (_textField->getDelegate())
            {
                _textField->getDelegate()->textFieldDidChangeText(_textField);
            }
        }
    }
}

- (void) keyUp:(NSEvent *)theEvent
{
    [super keyUp:theEvent];
    if ([theEvent keyCode] == 36)
    {
        if (_textField->isAllowkeyBoardHide())
        {
            _textField->resignFirstResponder();
        }
        
        if (s_ShouldReturn_map.count(_textField) > 0 && s_ShouldReturn_map[_textField])
        {
            s_ShouldReturn_map[_textField]();
        }
        else if (_textField->getDelegate())
        {
            _textField->getDelegate()->textFieldShouldReturn(_textField);
        }
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
CATextField::CATextField()
: m_pBackgroundView(nullptr)
, m_pImgeView(nullptr)
, m_pTextField(nullptr)
, m_pDelegate(nullptr)
, m_bUpdateImage(true)
, m_bSecureTextEntry(false)
, m_bAllowkeyBoardHide(true)
, m_cTextColor(CAColor4B::BLACK)
, m_cPlaceHdolderColor(CAColor4B::GRAY)
, m_iMarginLeft(10)
, m_iMarginRight(10)
, m_iFontSize(40)
, m_iMaxLength(0)
, m_eClearBtn(CATextField::ClearButtonMode::None)
, m_eAlign(CATextField::Align::Left)
, m_eReturnType(CATextField::ReturnType::Done)
, m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    this->setHaveNextResponder(false);
    this->setPoint(DPoint(-5000, -5000));
    NSPoint point = CGPointMake(-50000, -50000);
    m_pTextField = [[MACTextField alloc]initWithFrame:CGRectMake(point.x, point.y, 100, 40)];
    EAGLView * eaglview = [EAGLView sharedEGLView];
    [eaglview addSubview:textField_MAC];
    textField_MAC.textField = this;
    [textField_MAC release];
    
    textField_MAC.placeholderString = @"";
    CGFloat scale = MAC_SCALE;
    textField_MAC.font = [NSFont systemFontOfSize:s_dip_to_px(m_iFontSize) / scale];
}

CATextField::~CATextField()
{
    s_ShouldBeginEditing_map.erase(this);
    s_ShouldEndEditing_map.erase(this);
    s_ShouldReturn_map.erase(this);
    s_KeyBoardHeight_map.erase(this);
    s_ShouldChangeCharacters_map.erase(this);
    s_DidChangeText_map.erase(this);

    [[NSNotificationCenter defaultCenter] removeObserver:textField_MAC];
    [textField_MAC removeFromSuperview];
    m_pDelegate = NULL;
}

void CATextField::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextField::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
    if (this->isFirstResponder())
    {
        this->resignFirstResponder();
    }

}

bool CATextField::resignFirstResponder()
{
    if (s_ShouldEndEditing_map.count(this) > 0 && s_ShouldEndEditing_map[this] && !s_ShouldEndEditing_map[this]())
    {
        return false;
    }
    else if (m_pDelegate && (!m_pDelegate->textFieldShouldEndEditing(this)))
    {
        return false;
    }
    
    bool result = CAControl::resignFirstResponder();
    
    [textField_MAC resignFirstResponder];
    
    this->showTextField();

    this->hideNativeTextField();
    
    this->showImage();
    
    return result;
}

bool CATextField::becomeFirstResponder()
{
    if (s_ShouldBeginEditing_map.count(this) > 0 && s_ShouldBeginEditing_map[this] && !s_ShouldBeginEditing_map[this]())
    {
        return false;
    }
    else if (m_pDelegate &&( !m_pDelegate->textFieldShouldBeginEditing(this)))
    {
        return false;
    }
    
    bool result = CAControl::becomeFirstResponder();
    
    [textField_MAC becomeFirstResponder];
    
    this->showNativeTextField();
    
    this->hideTextField();
    
    return result;
}

void CATextField::hideTextField()
{
    m_pImgeView->setVisible(false);
}

void CATextField::showTextField()
{
    m_pImgeView->setVisible(true);
}

void CATextField::hideNativeTextField()
{
    CAScheduler::getScheduler()->unschedule(schedule_selector(CATextField::update), this);
    
    [textField_MAC hide];
}

void CATextField::showNativeTextField()
{
    [textField_MAC show];
    CAScheduler::getScheduler()->schedule(schedule_selector(CATextField::update), this, 1/60.0f);
}

void CATextField::delayShowImage()
{
    this->cancelPreviousPerformRequests(callfunc_selector(CATextField::showImage));
    this->performSelector(callfunc_selector(CATextField::showImage), 0);
}

void CATextField::showImage()
{
    [textField_MAC setFrameOrigin:CGPointMake(-50000, -50000)];

    NSImage* image_MAC = [[[NSImage alloc]initWithData:[textField_MAC dataWithPDFInsideRect:[textField_MAC bounds]]]autorelease];

    NSData* data_MAC = [image_MAC TIFFRepresentationUsingCompression:NSTIFFCompressionNone factor:MAC_SCALE];
    
    unsigned char* pData = (unsigned char*)malloc([data_MAC length]);
    [data_MAC getBytes:pData length:[data_MAC length]];
    
    CAData* data = CAData::create();
    data->fastSet(pData, [data_MAC length]);
    CAImage *image = CAImage::createWithImageDataNoCache(data);
    
    m_pImgeView->setLayout(DLayout(DHorizontalLayout_L_R(m_iMarginLeft, m_iMarginRight), DVerticalLayoutFill));
    m_pImgeView->setImage(image);
}

CATextField* CATextField::createWithFrame(const DRect& frame)
{
    CATextField *textField = new CATextField();
    if (textField && textField->initWithFrame(frame))
    {
        textField->autorelease();
        return textField;
    }
    CC_SAFE_DELETE(textField);
    return NULL;
}

CATextField* CATextField::createWithCenter(const DRect& rect)
{
    CATextField* textField = new CATextField();
    
    if (textField && textField->initWithCenter(rect))
    {
        textField->autorelease();
        return textField;
    }
    
    CC_SAFE_DELETE(textField);
    return NULL;
}

CATextField* CATextField::createWithLayout(const DLayout& layout)
{
    CATextField* textField = new CATextField();
    
    if (textField && textField->initWithLayout(layout))
    {
        textField->autorelease();
        return textField;
    }
    
    CC_SAFE_DELETE(textField);
    return NULL;
}

bool CATextField::init()
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CATextField");
    CAImage* image = CAImage::create(map.at("backgroundView_normal"));
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
    m_pBackgroundView = CAScale9ImageView::createWithImage(image);
    m_pBackgroundView->setLayout(DLayoutFill);
    m_pBackgroundView->setCapInsets(capInsets);
    this->insertSubview(m_pBackgroundView, -1);
    
    m_pImgeView = CAImageView::createWithLayout(DLayout(DHorizontalLayout_L_R(2, 2), DVerticalLayout_T_B(1, 0)));
    this->addSubview(m_pImgeView);
    m_pImgeView->setTextTag("textField");

    this->setMarginLeft(m_iMarginLeft);
    this->setMarginRight(m_iMarginRight);
    
    return true;
}

void CATextField::update(float dt)
{
    do
    {
        //CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);
        point.y = CAApplication::getApplication()->getWinSize().height - point.y;
        point.y = point.y - m_obContentSize.height;

        CGFloat scale = MAC_SCALE;
        NSPoint origin;
        origin.x = s_dip_to_px(point.x) / scale;
        origin.y = s_dip_to_px(point.y) / scale;
        [textField_MAC setFrameOrigin:origin];
    }
    while (0);
}

void CATextField::setContentSize(const DSize& contentSize)
{
    DSize size = contentSize;
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CATextField");
    if (m_bRecSpe)
    {
        int h = atoi(map.at("height").c_str());
        size.height = (h == 0) ? size.height : h;
    }
    CAControl::setContentSize(size);
    
    DSize worldContentSize = this->convertToWorldSize(m_obContentSize);
    
    CGFloat scale = MAC_SCALE;
    NSSize nssize;
    nssize.width = s_dip_to_px(worldContentSize.width) / scale;
    nssize.height =  s_dip_to_px(worldContentSize.height) / scale;
    [textField_MAC setContentSize:nssize];

    if (m_eClearBtn == CATextField::ClearButtonMode::WhileEditing)
    {
        this->setClearButtonMode(CATextField::ClearButtonMode::WhileEditing);
    }
    else
    {
        CAButton* rightMarginView = (CAButton*)this->getSubviewByTextTag("ImageRight");
        if (rightMarginView)
        {
            rightMarginView->removeFromSuperview();
        }
        this->setMarginRight(m_iMarginRight);
    }
        
    this->showImage();
}

bool CATextField::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
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

void CATextField::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CATextField::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
   
}

void CATextField::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}

void CATextField::setKeyboardType(CATextField::KeyboardType type)
{
    m_eKeyBoardType = type;
}

CATextField::KeyboardType CATextField::getKeyboardType()
{
    return m_eKeyBoardType;
}

void CATextField::setReturnType(CATextField::ReturnType var)
{
    m_eReturnType = var;
}

CATextField::ReturnType CATextField::getReturnType()
{
    return m_eReturnType;
}

void CATextField::setBackgroundImage(CAImage* image)
{
    if (image)
    {
        DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
        m_pBackgroundView->setCapInsets(capInsets);
    }
    m_pBackgroundView->setImage(image);
}

void CATextField::setPlaceHolderText(const std::string &var)
{
    m_sPlaceHolderText = var;
    
    textField_MAC.placeholderString = [NSString stringWithUTF8String:m_sPlaceHolderText.c_str()];
    
    this->delayShowImage();
}

const std::string& CATextField::getPlaceHolderText()
{
    return m_sPlaceHolderText;
}

void CATextField::setPlaceHolderColor(const CAColor4B &var)
{
    CC_RETURN_IF(CAColor4BEqual(m_cPlaceHdolderColor, var));
    
    m_cPlaceHdolderColor = var;
    
//    NSColor* color = [NSColor colorWithRed:var.r/255.f green:var.g/255.f blue:var.b/255.f alpha:var.a/255.f];
//    [textField_MAC setValue:color forKeyPath:@"_placeholderLabel.textColor"];
    CGFloat scale = MAC_SCALE;
    textField_MAC.font = [NSFont systemFontOfSize:s_dip_to_px(m_iFontSize) / scale];
    this->delayShowImage();
}

const CAColor4B& CATextField::getPlaceHolderColor()
{
    return m_cPlaceHdolderColor;
}

void CATextField::setFontSize(int var)
{
    m_iFontSize = var;
    
    CGFloat scale = MAC_SCALE;
    textField_MAC.font = [NSFont systemFontOfSize:s_dip_to_px(m_iFontSize) / scale];
//    [textField_MAC setValue:textField_MAC.font forKeyPath:@"_placeholderLabel.font"];
    
    this->delayShowImage();
}

int CATextField::getFontSize()
{
    return m_iFontSize;
}

void CATextField::setText(const std::string &var)
{
    m_sText = var;
    
    [textField_MAC setText:[NSString stringWithUTF8String:m_sText.c_str()]];
    
    this->delayShowImage();
}

const std::string& CATextField::getText()
{
    m_sText = [textField_MAC.beforeText UTF8String];
    return m_sText;
}

void CATextField::setTextColor(const CAColor4B &var)
{
    m_cTextColor = var;
    
    textField_MAC.textColor = [NSColor colorWithRed:var.r/255.f green:var.g/255.f blue:var.b/255.f alpha:var.a/255.f];
    
    this->delayShowImage();
}

const CAColor4B& CATextField::getTextColor()
{
    return m_cTextColor;
}

void CATextField::setMarginLeft(int var)
{
    m_iMarginLeft = var;
    
    DSize worldContentSize = this->convertToWorldSize(DSize(var, 0));
    
    [textField_MAC setMarginLeft:worldContentSize.width];
    
    this->delayShowImage();
}

int CATextField::getMarginLeft()
{
    return m_iMarginLeft;
}

void CATextField::setMarginRight(int var)
{
    if (m_eClearBtn == CATextField::ClearButtonMode::None)
    {
        m_iMarginRight = var;
        
        DSize worldContentSize = this->convertToWorldSize(DSize(var, 0));
        
        [textField_MAC setMarginRight:worldContentSize.width];
                
        this->delayShowImage();
    }
}

int CATextField::getMarginRight()
{
    return m_iMarginRight;
}

void CATextField::setMarginImageLeft(const DSize& imgSize,const std::string& filePath)
{
    //set margins
    this->setMarginLeft(imgSize.width);
    
    //setimage
    CAImageView* leftMarginView = (CAImageView*)this->getSubviewByTextTag("ImageLeft");
    if (!leftMarginView)
    {
        leftMarginView = CAImageView::create();
        leftMarginView->setTextTag("ImageLeft");
        this->addSubview(leftMarginView);
    }
    DLayout layout;
    layout.horizontal.left = 0;
    layout.horizontal.width = imgSize.width;
    layout.vertical.height = imgSize.height;
    layout.vertical.center = 0.5f;
    leftMarginView->setLayout(layout);
    leftMarginView->setImage(CAImage::create(filePath));
}

void CATextField::setMarginImageRight(const DSize& imgSize,const std::string& filePath)
{
    //set margins
    this->setMarginRight(imgSize.width);
    
    //setimage
    CAButton* rightMarginView = (CAButton*)this->getSubviewByTextTag("ImageRight");

    if (rightMarginView == NULL)
    {
        rightMarginView = CAButton::create(CAButton::Type::Custom);
        rightMarginView->setTextTag("ImageRight");
        this->addSubview(rightMarginView);
    }
    DLayout layout;
    layout.horizontal.right = 0;
    layout.horizontal.width = imgSize.width;
    layout.vertical.height = imgSize.height;
    layout.vertical.center = 0.5f;
    rightMarginView->setLayout(layout);
    rightMarginView->setImageSize(imgSize);
    rightMarginView->setImageForState(CAControl::State::Normal, CAImage::create(filePath));
}

void CATextField::setClearButtonMode(CATextField::ClearButtonMode var)
{
    if (var == CATextField::ClearButtonMode::WhileEditing)
    {
        float width = m_obContentSize.height/2;
        this->setMarginImageRight(DSize(width, width), "");
        const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CATextField");
        CAButton* rightMarginView = (CAButton*)this->getSubviewByTextTag("ImageRight");
        rightMarginView->setImageForState(CAControl::State::Normal, CAImage::create(map.at("clearImage")));
        rightMarginView->setImageColorForState(CAControl::State::Highlighted, ccc4Int(0xff666666));
        rightMarginView->addTarget([=]()
        {
        
            if (getText().length() > 0)
            {
                this->setText("");
                if (this->isFirstResponder() == false)
                {
                    this->delayShowImage();
                }
            }
            
        }, CAButton::Event::TouchUpInSide);

        DSize worldContentSize = this->convertToWorldSize(DSize(m_iMarginRight, 0));
        
        [textField_MAC setMarginRight:worldContentSize.width];
            }
    else
    {
        CAButton* rightMarginView = (CAButton*)this->getSubviewByTextTag("ImageRight");
        if (rightMarginView)
        {
            rightMarginView->removeFromSuperview();
        }
        this->setMarginRight(10);
    }
    
    m_eClearBtn = var;
}

CATextField::ClearButtonMode CATextField::getClearButtonMode()
{
    return m_eClearBtn;
}

void CATextField::setAlign(CATextField::Align var)
{
    m_eAlign = var;
    
    switch (var)
    {
        case CATextField::Align::Left:
            [[textField_MAC cell] setAlignment:NSTextAlignmentLeft];
            break;
        case CATextField::Align::Center:
            [[textField_MAC cell] setAlignment:NSTextAlignmentCenter];
            break;
        case CATextField::Align::Right:
            [[textField_MAC cell] setAlignment:NSTextAlignmentRight];
            break;
        default:
            break;
    }
    
    this->delayShowImage();
}

CATextField::Align CATextField::getAlign()
{
    return m_eAlign;
}

void CATextField::setSecureTextEntry(bool var)
{
    m_bSecureTextEntry = var;
}

bool CATextField::isSecureTextEntry()
{
    return m_bSecureTextEntry;
}

void CATextField::setMaxLength(int var)
{
    m_iMaxLength = var;
}

int CATextField::getMaxLength()
{
    return m_iMaxLength;
}

void CATextField::onShouldBeginEditing(const std::function<bool ()> &var)
{
    m_obShouldBeginEditing = var;
    s_ShouldBeginEditing_map[this] = var;
}

void CATextField::onShouldEndEditing(const std::function<bool ()> &var)
{
    m_obShouldEndEditing = var;
    s_ShouldEndEditing_map[this] = var;
}

void CATextField::onShouldReturn(const std::function<void ()> &var)
{
    m_obShouldReturn = var;
    s_ShouldReturn_map[this] = var;
}

void CATextField::onKeyBoardHeight(const std::function<void (int)> &var)
{
    m_obKeyBoardHeight = var;
    s_KeyBoardHeight_map[this] = var;
}

void CATextField::onShouldChangeCharacters(const std::function<bool (ssize_t, ssize_t, const std::string &)> &var)
{
    m_obShouldChangeCharacters = var;
    s_ShouldChangeCharacters_map[this] = var;
}

void CATextField::onDidChangeText(const std::function<void ()> &var)
{
    m_obDidChangeText = var;
    s_DidChangeText_map[this] = var;
}

NS_CC_END
