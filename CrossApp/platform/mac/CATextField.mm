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

#define textField_MAC ((MACTextField*)m_pTextField)

#ifdef NSTextAlignmentLeft
#else
#define NSTextAlignmentLeft NSLeftTextAlignment
#define NSTextAlignmentCenter NSCenterTextAlignment
#define NSTextAlignmentRight NSRightTextAlignment
#endif

@interface MACTextField: NSTextField<NSTextFieldDelegate>
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

-(void)setMarginLeft:(int)marginLeft
{
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    _marginLeft = CrossApp::s_dip_to_px(marginLeft) / scale;
}

-(int)getMarginLeft
{
    int olbMarginLeft = _marginLeft;
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    return CrossApp::s_px_to_dip(_marginLeft) * scale;
    
    NSRect rect = self.frame;
    rect.size.width -= _marginLeft - olbMarginLeft;
    [self setFrame:rect];
}

-(void)setMarginRight:(int)marginRight
{
    int olbMarginRight = _marginRight;
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    _marginRight = CrossApp::s_dip_to_px(marginRight) / scale;
    
    NSRect rect = self.frame;
    rect.size.width -= _marginRight - olbMarginRight;
    [self setFrame:rect];
}

-(int)getMarginRight
{
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
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
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
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
        [self setDelegate:self];
        [self setBackgroundColor:[NSColor clearColor]];
        [self setBezeled:NO];
        [[self cell] setAlignment:NSTextAlignmentLeft];
        [[self cell] setLineBreakMode:NSLineBreakByTruncatingTail];
        _marginLeft = 0;
        _marginRight = 0;
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

- (void)textDidChange:(NSNotification *)notification
{
    [super textDidChange:notification];
}

- (void) keyDown:(NSEvent *)theEvent
{
    [super keyDown:theEvent];
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
        
        if (_textField->getDelegate())
        {
            _textField->getDelegate()->textFieldShouldReturn(_textField);
        }
    }
}

@end


NS_CC_BEGIN
CATextField::CATextField()
: m_pBackgroundView(NULL)
, m_pImgeView(NULL)
, m_pTextField(NULL)
, m_pDelegate(NULL)
, m_bUpdateImage(true)
, m_bSecureTextEntry(false)
, m_bAllowkeyBoardHide(true)
, m_iMarginLeft(10)
, m_iMarginRight(10)
, m_iFontSize(40)
, m_iMaxLenght(0)
, m_eClearBtn(None)
, m_eAlign(Left)
, m_eReturnType(Done)
, m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    this->setHaveNextResponder(false);
    
    CGPoint point = CGPointMake(-50000, -50000);
    m_pTextField = [[MACTextField alloc]initWithFrame:CGRectMake(point.x, point.y, 100, 40)];
    EAGLView * eaglview = [EAGLView sharedEGLView];
    [eaglview addSubview:textField_MAC];
    textField_MAC.textField = this;
    
    textField_MAC.placeholderString = @"placeholder";
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    textField_MAC.font = [NSFont systemFontOfSize:s_dip_to_px(m_iFontSize) / scale];
}

CATextField::~CATextField()
{
    [textField_MAC removeFromSuperview];
}

void CATextField::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextField::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
    
    if (this->isFirstResponder())
    {
        this->resignFirstResponder();
    }
}

bool CATextField::resignFirstResponder()
{
    if (m_pDelegate && (!m_pDelegate->textFieldShouldEndEditing(this)))
    {
        return false;
    }
    
    bool result = CAView::resignFirstResponder();
    
    [textField_MAC resignFirstResponder];
    
    this->showTextField();
    
    this->showImage();
    
    this->hideNativeTextField();
    return result;
}

bool CATextField::becomeFirstResponder()
{
    if (m_pDelegate &&( !m_pDelegate->textFieldShouldBeginEditing(this)))
    {
        return false;
    }
    
    bool result = CAView::becomeFirstResponder();
    
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
    CAScheduler::unschedule(schedule_selector(CATextField::update), this);
    
    [textField_MAC hide];
}

void CATextField::showNativeTextField()
{
    [textField_MAC show];
    CAScheduler::schedule(schedule_selector(CATextField::update), this, 1/60.0f);
}

void CATextField::delayShowImage()
{
    CC_RETURN_IF(CAScheduler::isScheduled(schedule_selector(CATextField::showImage), this));
    CAScheduler::schedule(schedule_selector(CATextField::showImage), this, 0, 0, 0);
}

void CATextField::showImage()
{
    NSImage* image_MAC = [[[NSImage alloc]initWithData:[textField_MAC dataWithPDFInsideRect:[textField_MAC bounds]]]autorelease];
    
    NSData* data_MAC = [image_MAC TIFFRepresentation];
    
    unsigned char* data = (unsigned char*)malloc([data_MAC length]);
    [data_MAC getBytes:data];

    CAImage *image = CAImage::createWithImageDataNoCache(data, data_MAC.length);
    free(data);
    m_pImgeView->setImage(image);
    
    this->updateDraw();
    CAScheduler::unschedule(schedule_selector(CATextField::showImage), this);
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

bool CATextField::init()
{
    CAImage* image = CAImage::create("source_material/textField_bg.png");
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
    m_pBackgroundView = CAScale9ImageView::createWithImage(image);
    m_pBackgroundView->setCapInsets(capInsets);
    this->insertSubview(m_pBackgroundView, -1);
    
    m_pImgeView = CAImageView::createWithFrame(DRect(0, 0, 1, 1));
    this->addSubview(m_pImgeView);
    m_pImgeView->setTextTag("textField");

    setMarginLeft(m_iMarginLeft);
    setMarginRight(m_iMarginRight);
    
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

        CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
        NSPoint origin;
        origin.x = s_dip_to_px(point.x) / scale;
        origin.y = s_dip_to_px(point.y) / scale;
        [textField_MAC setFrameOrigin:origin];
    }
    while (0);
}

void CATextField::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
    
    DSize worldContentSize = this->convertToWorldSize(m_obContentSize);
    
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    NSSize size;
    size.width = s_dip_to_px(worldContentSize.width) / scale;
    size.height =  s_dip_to_px(worldContentSize.height) / scale;
    [textField_MAC setContentSize:size];

    m_pBackgroundView->setFrame(this->getBounds());
    m_pImgeView->setFrame([textField_MAC getDRect]);
}

bool CATextField::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    
    return true;
}

void CATextField::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void CATextField::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
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

void CATextField::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}

void CATextField::setKeyboardType(const KeyboardType& type)
{
    m_eKeyBoardType = type;
}

const CATextField::KeyboardType& CATextField::getKeyboardType()
{
    return m_eKeyBoardType;
}

void CATextField::setReturnType(const ReturnType &var)
{
    m_eReturnType = var;
}

const CATextField::ReturnType& CATextField::getReturnType()
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
    
    NSColor* color = [NSColor colorWithRed:var.r/255.f green:var.g/255.f blue:var.b/255.f alpha:var.a];
//    [textField_MAC setValue:color forKeyPath:@"_placeholderLabel.textColor"];
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
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
    
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
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
    textField_MAC.stringValue = [NSString stringWithUTF8String:m_sText.c_str()];
    textField_MAC.beforeText = [textField_MAC stringValue];
    
    this->delayShowImage();
}

const std::string& CATextField::getText()
{
    m_sText = [textField_MAC.stringValue UTF8String];
    return m_sText;
}

void CATextField::setTextColor(const CAColor4B &var)
{
    if (CAColor4BEqual(m_cTextColor, var)) {
        return;
    }
    
    m_cTextColor = var;
    
    textField_MAC.textColor = [NSColor colorWithRed:var.r/255.f green:var.g/255.f blue:var.b/255.f alpha:var.a];
    
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
    
    m_pImgeView->setFrame([textField_MAC getDRect]);
    
    this->delayShowImage();
}

int CATextField::getMarginLeft()
{
    return m_iMarginLeft;
}

void CATextField::setMarginRight(int var)
{
    if (m_eClearBtn == None)
    {
        m_iMarginRight = var;
        
        DSize worldContentSize = this->convertToWorldSize(DSize(var, 0));
        
        [textField_MAC setMarginRight:worldContentSize.width];
        
        m_pImgeView->setFrame([textField_MAC getDRect]);
        
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
    setMarginLeft(imgSize.width);
    
    //setimage
    CAImageView* ima = (CAImageView*)this->getSubviewByTag(1010);
    if (!ima)
    {
        ima = CAImageView::create();
        ima->setTag(1010);
        this->addSubview(ima);
    }
    ima->setCenter(DRect(imgSize.width / 2, m_obContentSize.height / 2, imgSize.width, imgSize.height));
    ima->setImage(CAImage::create(filePath));
}

void CATextField::setMarginImageRight(const DSize& imgSize,const std::string& filePath)
{
    //set margins
    setMarginRight(imgSize.width);
    
    //setimage
    CAButton* rightMarginView = (CAButton*)this->getSubviewByTag(1011);

    if (rightMarginView == NULL)
    {
        rightMarginView = CAButton::create(CAButtonTypeCustom);
        rightMarginView->setTag(1011);
        this->addSubview(rightMarginView);
    }
    rightMarginView->setCenter(DRect(m_obContentSize.width - imgSize.width / 2, m_obContentSize.height / 2, imgSize.width, imgSize.height));
    rightMarginView->setImageSize(rightMarginView->getBounds().size);
    rightMarginView->setImageForState(CAControlStateAll, CAImage::create(filePath));
}

void CATextField::setClearButtonMode(const ClearButtonMode &var)
{
    if (var == WhileEditing)
    {
        setMarginImageRight(DSize(m_obContentSize.height, m_obContentSize.height), "");
        
        CAButton* rightMarginView = (CAButton*)this->getSubviewByTag(1011);
        rightMarginView->setImageForState(CAControlStateAll, CAImage::create("source_material/clear_button.png"));
        rightMarginView->setImageColorForState(CAControlStateHighlighted, CAColor_blue);
        rightMarginView->addTarget(this, CAControl_selector(CATextField::clearBtnCallBack), CAControlEventTouchUpInSide);

        DSize worldContentSize = this->convertToWorldSize(DSize(m_iMarginRight, 0));
        
        [textField_MAC setMarginRight:worldContentSize.width];
        
        m_pImgeView->setFrame([textField_MAC getDRect]);
    }
    else
    {
        CAButton* rightMarginView = (CAButton*)this->getSubviewByTag(1011);
        if (rightMarginView)
        {
            rightMarginView->removeFromSuperview();
        }
        setMarginRight(10);
    }
    
    m_eClearBtn = var;
}

const CATextField::ClearButtonMode& CATextField::getClearButtonMode()
{
    return m_eClearBtn;
}

void CATextField::setTextFieldAlign(const TextFieldAlign &var)
{
    m_eAlign = var;
    
    switch (var)
    {
        case Left:
            [[textField_MAC cell] setAlignment:NSTextAlignmentLeft];
            break;
        case Center:
            [[textField_MAC cell] setAlignment:NSTextAlignmentCenter];
            break;
        case Right:
            [[textField_MAC cell] setAlignment:NSTextAlignmentRight];
            break;
        default:
            break;
    }
    
    this->delayShowImage();
}

const CATextField::TextFieldAlign& CATextField::getTextFieldAlign()
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

void CATextField::setMaxLenght(int var)
{
    m_iMaxLenght = var;
}

int CATextField::getMaxLenght()
{
    return m_iMaxLenght;
}

void CATextField::clearBtnCallBack(CAControl* con, DPoint point)
{
    if (getText().length() > 0)
    {
        setText("");
        if (this->isFirstResponder() == false)
        {
            this->delayShowImage();
        }
    }
}

NS_CC_END



