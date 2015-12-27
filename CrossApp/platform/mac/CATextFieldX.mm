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
}

@property(nonatomic,assign) CrossApp::CATextFieldX* textField;
@property(nonatomic,retain) NSString* beforeText;

-(void)regiestKeyBoardMessage;

@end

@implementation MACTextField
{
    
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
        return self;
    }
    return nil;
}

-(void)hide
{
    [self setHidden:YES];
    [self setWantsLayer:NO];
}

-(void)show
{
    [self setHidden:NO];
    [self setWantsLayer:YES];
}

-(void)regiestKeyBoardMessage
{
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillWasShown:) name:NSKeyboardWillShowNotification object:nil];
//
//    [[NSNotificationCenter defaultCenter]  addObserver:self selector:@selector(keyboardWasHidden:) name:UIKeyboardWillHideNotification object:nil];
//    
//    [self addTarget:self action:@selector(textFieldEditChanged:) forControlEvents:UIControlEventEditingChanged];
}
- (BOOL)textFieldShouldBeginEditing:(NSTextField *)sender        // return NO to disallow editing.
{
    CrossApp::CATextFieldDelegateX* delegate = _textField->getDelegate();
    if (delegate != NULL)
    {
        
    }

    return YES;
}

- (BOOL)textFieldShouldEndEditing:(NSTextField *)sender
{
    CrossApp::CATextFieldDelegateX* delegate = _textField->getDelegate();
    if (delegate != NULL)
    {
        
    }
    return YES;
}

/**
 * Delegate method called before the text has been changed.
 * @param textField The text field containing the text.
 * @param range The range of characters to be replaced.
 * @param string The replacement string.
 * @return YES if the specified text range should be replaced; otherwise, NO to keep the old text.
 */
- (BOOL)textField:(NSTextField *) textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    if (_textField->getMaxLenght() > 0)
    {
        NSUInteger oldLength = [[textField stringValue] length];
        NSUInteger replacementLength = [string length];
        NSUInteger rangeLength = range.length;
        
        NSUInteger newLength = oldLength - rangeLength + replacementLength;
        
        return newLength <= _textField->getMaxLenght();
    }
    
    return YES;
}

/**
 * Called each time when the text field's text has changed.
 */
- (void)controlTextDidChange:(NSNotification *)notification
{

}

@end




NS_CC_BEGIN
CATextFieldX::CATextFieldX()
:m_pBackgroundView(NULL)
,m_pImgeView(NULL)
,m_pTextField(NULL)
,m_pDelegate(NULL)
,m_bUpdateImage(true)
,m_marginLeft(10)
,m_marginRight(10)
,m_fontSize(24)
,m_iMaxLenght(0)
,m_clearBtn(ClearButtonMode::ClearButtonNone)
,m_obLastPoint(DPoint(-0xffff, -0xffff))
{
    this->setHaveNextResponder(false);
}

CATextFieldX::~CATextFieldX()
{
}

void CATextFieldX::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextFieldX::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

bool CATextFieldX::resignFirstResponder()
{
    if (m_pDelegate && (!m_pDelegate->textFieldShouldEndEditing(this)))
    {
        return false;
    }
    
    bool result = CAView::resignFirstResponder();
    
    if (result)
    {
        [textField_MAC resignFirstResponder];
        
        this->showImage();
        
        this->showTextField();
        
        this->hideNativeTextField();
    }
    return result;
}

bool CATextFieldX::becomeFirstResponder()
{
    if (m_pDelegate &&( !m_pDelegate->textFieldShouldBeginEditing(this)))
    {
        return false;
    }
    
    bool result = CAView::becomeFirstResponder();
    if (result)
    {
        [textField_MAC becomeFirstResponder];
        
        CAViewAnimation::beginAnimations(m_s__StrID + "hideTextField", NULL);
        CAViewAnimation::setAnimationDuration(0);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextFieldX::hideTextField));
        CAViewAnimation::commitAnimations();
        
        this->showNativeTextField();
    }
    
    
    
    return result;
}

void CATextFieldX::hideTextField()
{
    m_pImgeView->setVisible(false);
}

void CATextFieldX::showTextField()
{
    m_pImgeView->setVisible(true);
}

void CATextFieldX::hideNativeTextField()
{
    CAScheduler::unschedule(schedule_selector(CATextFieldX::update), this);
    
    [textField_MAC performSelector:@selector(hide) withObject:nil afterDelay:1/60.0f];
}

void CATextFieldX::showNativeTextField()
{
    [textField_MAC show];
    CAScheduler::schedule(schedule_selector(CATextFieldX::update), this, 1/60.0f);
}

void CATextFieldX::delayShowImage()
{
    if (!CAViewAnimation::areBeginAnimationsWithID(m_s__StrID + "showImage"))
    {
        CAViewAnimation::beginAnimations(m_s__StrID + "showImage", NULL);
        CAViewAnimation::setAnimationDuration(0);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextFieldX::showImage));
        CAViewAnimation::commitAnimations();
    }
}

void CATextFieldX::showImage()
{
    NSImage* image_MAC = [[[NSImage alloc]initWithData:[textField_MAC dataWithPDFInsideRect:[textField_MAC bounds]]]autorelease];
    
    NSData* data_MAC = [image_MAC TIFFRepresentation];
    
    unsigned char* data = (unsigned char*)malloc([data_MAC length]);
    [data_MAC getBytes:data];

    CAImage *image = CAImage::createWithImageDataNoCache(data, data_MAC.length);
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
    CGPoint point = CGPointMake(-5000, -5000);
    m_pTextField = [[MACTextField alloc]initWithFrame:CGRectMake(point.x, point.y, 100, 40)];
    EAGLView * eaglview = [EAGLView sharedEGLView];
    [eaglview addSubview:textField_MAC];
    textField_MAC.textField = this;
    textField_MAC.delegate = textField_MAC;
    
    textField_MAC.placeholderString = @"placeholder";
    textField_MAC.font = [NSFont systemFontOfSize:m_fontSize];
    [textField_MAC regiestKeyBoardMessage];
    
    setMarginLeft(m_marginLeft);
    setMarginRight(m_marginRight);
    
    CAImage* image = CAImage::create("source_material/textField_bg.png");
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
    m_pBackgroundView = CAScale9ImageView::createWithImage(image);
    m_pBackgroundView->setCapInsets(capInsets);
    this->insertSubview(m_pBackgroundView, -1);
    
    m_pImgeView = CAImageView::createWithFrame(DRect(0, 0, 1, 1));
    this->addSubview(m_pImgeView);
    m_pImgeView->setTextTag("textField");

    return true;
}

void CATextFieldX::update(float dt)
{
    do
    {
        CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);
        point.y = CAApplication::getApplication()->getWinSize().height - point.y;
        point.y = point.y - m_obContentSize.height;
        CC_BREAK_IF(m_obLastPoint.equals(point));

        CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
        CGRect rect = textField_MAC.frame;
        rect.origin.x = s_dip_to_px(point.x) / scale;
        rect.origin.y = s_dip_to_px(point.y) / scale;
        textField_MAC.frame = rect;
    }
    while (0);
}

void CATextFieldX::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
    
    DSize worldContentSize = DSizeApplyAffineTransform(m_obContentSize, worldToNodeTransform());
    
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    CGRect rect = textField_MAC.frame;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    textField_MAC.frame = rect;

    m_pBackgroundView->setFrame(this->getBounds());
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
    this->ccTouchEnded(pTouch, pEvent);
}

void CATextFieldX::setKeyboardType(const KeyboardType& type)
{
    m_keyBoardType = type;
}

const CATextFieldX::KeyboardType& CATextFieldX::getKeyboardType()
{
    return m_keyBoardType;
}

void CATextFieldX::setReturnType(const ReturnType &var)
{
    m_returnType = var;
}

const CATextFieldX::ReturnType& CATextFieldX::getReturnType()
{
    return m_returnType;
}

void CATextFieldX::setBackGroundImage(CAImage* image)
{
    if (image)
    {
        DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
        m_pBackgroundView->setCapInsets(capInsets);
    }
    m_pBackgroundView->setImage(image);
}

void CATextFieldX::setPlaceHolderText(const std::string &var)
{
    m_placeHolderText = var;
    
    textField_MAC.placeholderString = [NSString stringWithUTF8String:m_placeHolderText.c_str()];
    
    this->delayShowImage();
}

const std::string& CATextFieldX::getPlaceHolderText()
{
    return m_placeHolderText;
}

void CATextFieldX::setPlaceHolderColor(const CAColor4B &var)
{
    CC_RETURN_IF(CAColor4BEqual(m_placeHdolderColor, var));
    
    m_placeHdolderColor = var;
    
    NSColor* color = [NSColor colorWithRed:var.r green:var.g blue:var.b alpha:var.a];
    [textField_MAC setValue:color forKeyPath:@"_placeholderLabel.textColor"];
    
    this->delayShowImage();
}

const CAColor4B& CATextFieldX::getPlaceHolderColor()
{
    return m_placeHdolderColor;
}

void CATextFieldX::setFontSize(int var)
{
    m_fontSize = var;

    textField_MAC.font = [NSFont systemFontOfSize:var];
    [textField_MAC setValue:textField_MAC.font forKeyPath:@"_placeholderLabel.font"];
    
    this->delayShowImage();
}

int CATextFieldX::getFontSize()
{
    return m_fontSize;
}

void CATextFieldX::setText(const std::string &var)
{
    m_sText = var;
    textField_MAC.stringValue = [NSString stringWithUTF8String:m_sText.c_str()];
    textField_MAC.beforeText = [textField_MAC stringValue];
    
    this->delayShowImage();
}

const std::string& CATextFieldX::getText()
{
    return m_sText;
}

void CATextFieldX::setTextColor(const CAColor4B &var)
{
    if (CAColor4BEqual(m_sTextColor, var)) {
        return;
    }
    
    m_sTextColor = var;
    
    textField_MAC.textColor = [NSColor colorWithRed:var.r green:var.g blue:var.b alpha:var.a];
    
    this->delayShowImage();
}

const CAColor4B& CATextFieldX::getTextColor()
{
    return m_sTextColor;
}

void CATextFieldX::setMarginLeft(const int &var)
{
    m_marginLeft = var;
    
    DSize worldContentSize = DSizeApplyAffineTransform(DSize(var, 0), worldToNodeTransform());
    
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    CGFloat x = s_dip_to_px(worldContentSize.width) / scale;
    
//    NSView *paddingView = [[NSView alloc] initWithFrame:CGRectMake(0, 0, x, 0)];
//    textField_MAC.leftView = paddingView;
//    textField_MAC.leftViewMode = UITextFieldViewModeAlways;
//    [paddingView release];
    
    this->delayShowImage();
}

const int& CATextFieldX::getMarginLeft()
{
    return m_marginLeft;
}

void CATextFieldX::setMarginRight(const int &var)
{
    m_marginRight = var;
    
    DSize worldContentSize = DSizeApplyAffineTransform(DSize(var, 0), worldToNodeTransform());
    
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    CGFloat x = s_dip_to_px(worldContentSize.width) / scale;
    
//    UIView *paddingView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, x, 0)];
//    textField_MAC.rightView = paddingView;
//    textField_MAC.rightViewMode = UITextFieldViewModeAlways;
//    [paddingView release];
    
    this->delayShowImage();
}

const int& CATextFieldX::getMarginRight()
{
    return m_marginRight;
}

void CATextFieldX::setMarginImageLeft(const DSize imgSize,const std::string& filePath)
{
    DSize worldContentSize = DSizeApplyAffineTransform(imgSize, worldToNodeTransform());
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    CGRect rect;
    rect.origin.x = 0;
    rect.origin.y = 0;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    
//    UIImage* image = [UIImage imageNamed:[NSString stringWithUTF8String:filePath.c_str()]];
//    UIImageView* paddingView=[[UIImageView alloc]initWithImage:image];
//    [paddingView setFrame:rect];
//    textField_MAC.leftView = paddingView;
//    textField_MAC.leftViewMode = UITextFieldViewModeAlways;
    
    this->delayShowImage();
}

void CATextFieldX::setMarginImageRight(const DSize imgSize,const std::string& filePath)
{
    DSize worldContentSize = DSizeApplyAffineTransform(imgSize, worldToNodeTransform());
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    CGRect rect;
    rect.origin.x = 0;
    rect.origin.y = 0;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    
//    UIImage* image = [UIImage imageNamed:[NSString stringWithUTF8String:filePath.c_str()]];
//    UIImageView* paddingView=[[UIImageView alloc]initWithImage:image];
//    [paddingView setFrame:rect];
//    textField_MAC.rightView = paddingView;
//    textField_MAC.rightViewMode = UITextFieldViewModeAlways;
    
    this->delayShowImage();
}

void CATextFieldX::setClearButtonMode(const ClearButtonMode &var)
{
    m_clearBtn = var;
    
//    UITextFieldViewMode mode= UITextFieldViewModeNever;
//    switch (var)
//    {
//        case ClearButtonMode::ClearButtonWhileEditing:
//            mode = UITextFieldViewModeWhileEditing;
//            textField_MAC.rightViewMode = UITextFieldViewModeNever;
//            break;
//            
//        default:
//            mode = UITextFieldViewModeNever;
//            break;
//    }
//    textField_MAC.clearButtonMode = mode;
    
    this->delayShowImage();
}

const CATextFieldX::ClearButtonMode& CATextFieldX::getClearButtonMode()
{
    return m_clearBtn;
}

void CATextFieldX::setTextFieldAlign(const TextFieldAlign &var)
{
    m_align = var;
    
    switch (var)
    {
        case CATextFieldX::TextEditAlignLeft:
            [[textField_MAC cell] setAlignment:NSTextAlignmentLeft];
            break;
        case CATextFieldX::TextEditAlignCenter:
            [[textField_MAC cell] setAlignment:NSTextAlignmentCenter];
            break;
        case CATextFieldX::TextEditAlignRight:
            [[textField_MAC cell] setAlignment:NSTextAlignmentRight];
            break;
        default:
            break;
    }
    
    this->delayShowImage();
}

const CATextFieldX::TextFieldAlign& CATextFieldX::getTextFieldAlign()
{
    return m_align;
}

void CATextFieldX::setMaxLenght(int var)
{
    m_iMaxLenght = var;
}

int CATextFieldX::getMaxLenght()
{
    return m_iMaxLenght;
}

NS_CC_END



