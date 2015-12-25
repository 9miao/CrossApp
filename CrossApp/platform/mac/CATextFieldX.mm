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
        self.backgroundColor = [NSColor clearColor];
        self.bezeled = NO;
        self.cell.alignment = NSTextAlignmentLeft;
        self.cell.lineBreakMode = NSLineBreakByTruncatingTail;
        self.cell.userInterfaceLayoutDirection = NSUserInterfaceLayoutDirectionRightToLeft;
        
        [[self cell] setTruncatesLastVisibleLine:NO];
        
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

//- (void)textFieldEditChanged:(UITextField *)textField
//{
//    if (_isShouldEdit)
//    {
//        _isShouldEdit = NO;
//        std::string str = [self.beforeText cStringUsingEncoding:NSUTF8StringEncoding];
//        self.beforeText = [textField text];
//        return;
//    }
//    
//    NSString *stra = @"";
//    NSString *strb = @"";
//    int location = 0;
//    for (int i =0,j =0; i<[self.beforeText length]; i++)
//    {
//        location = i;
//        CC_BREAK_IF(i+1>[textField text].length);
//        
//        strb = [self.beforeText substringWithRange:NSMakeRange(i, 1)];
//        stra = [[textField text] substringWithRange:NSMakeRange(j, 1)];
//        
//        CC_BREAK_IF(![strb isEqualToString:stra]);
//        j++;
//    }
//
//    std::string str = [self.beforeText cStringUsingEncoding:NSUTF8StringEncoding];
//    self.beforeText = [textField text];
//    
//    if (_textField->getDelegate())
//    {
//        _textField->getDelegate()->textFieldAfterTextChanged(_textField, str.c_str(), "haha", location, 1, 0);
//    }
//    
//    _isShouldEdit = NO;
//}
//
//- (void) keyboardWillWasShown:(NSNotification *) notif
//{
//    NSDictionary *info = [notif userInfo];
//    NSValue *value = [info objectForKey:UIKeyboardFrameEndUserInfoKey];
//    CGSize keyboardSize = [value CGRectValue].size;
//    
//    CGFloat scale  = [[NSScreen mainScreen] backingScaleFactor];
//    int height = CrossApp::s_px_to_dip(keyboardSize.height * scale);
//    
//    if (_textField->getDelegate())
//    {
//        _textField->getDelegate()->keyBoardHeight(_textField, height);
//    }
//}
//
//- (void) keyboardWasHidden:(NSNotification *) notif
//{
//    if (_textField->getDelegate())
//    {
//        _textField->getDelegate()->keyBoardHeight(_textField, 0);
//    }
//}
//
//- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
//{
//
//    std::string str = self.beforeText != nil ? [self.beforeText cStringUsingEncoding:NSUTF8StringEncoding] : "";
//    std::string insert = [string cStringUsingEncoding:NSUTF8StringEncoding];
//    self.beforeText = [textField text];
//    
//    if (_textField->getDelegate())
//    {
//        _textField->getDelegate()->textFieldAfterTextChanged(_textField, str.c_str(), insert.c_str(), (int)range.location, 0, (int)string.length);
//    }
//    
//    _isShouldEdit = YES;
//    return YES;
//}

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

void CATextFieldX::setFontSize(const int &var)
{
    m_fontSize = var;

    textField_MAC.font = [NSFont systemFontOfSize:var];
    [textField_MAC setValue:textField_MAC.font forKeyPath:@"_placeholderLabel.font"];
    
    this->delayShowImage();
}

const int& CATextFieldX::getFontSize()
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
    
//    switch (var)
//    {
//        case CATextFieldX::TextEditAlignCenter:
//            textField_MAC.textAlignment = NSTextAlignmentCenter;
//            break;
//        case CATextFieldX::TextEditAlignLeft:
//            textField_MAC.textAlignment = NSTextAlignmentLeft;
//            break;
//        case CATextFieldX::TextEditAlignRight:
//            textField_MAC.textAlignment = NSTextAlignmentRight;
//            break;
//        default:
//            break;
//    }
    
    this->delayShowImage();
}

const CATextFieldX::TextFieldAlign& CATextFieldX::getTextFieldAlign()
{
    return m_align;
}
NS_CC_END



