//
//  CATextField.cpp
//  tesss
//
//  Created by 栗元峰 on 15/11/20.
//
//

#include "platform/CATextField.h"
#include "platform/CADensityDpi.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"
#import "EAGLView.h"
#import <UIKit/UIKit.h>

#define textField_iOS ((IOSTextField*)m_pTextField)

@interface IOSTextField: UITextField<UITextFieldDelegate>
{
    BOOL _isShouldEdit;
}

@property(nonatomic,assign) CrossApp::CATextField* textField;
@property(nonatomic,copy) NSString* beforeText;

-(void)regiestKeyBoardMessage;
-(void)removeTextView;
-(void)hide;
@end
 
@implementation IOSTextField
{

}

-(id)initWithFrame:(CGRect)frame
{
    if ([super initWithFrame:frame])
    {
        _beforeText = [[NSString alloc]initWithFormat:@""];
        return self;
    }
    return nil;
}

-(void)dealloc
{
    [super dealloc];
}

-(void)hide
{
    CGRect rect = self.frame;
    rect.origin = CGPointMake(-5000, -5000);
    self.frame = rect;
}

-(void)regiestKeyBoardMessage
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillWasShown:) name:UIKeyboardWillShowNotification object:nil];

    [[NSNotificationCenter defaultCenter]  addObserver:self selector:@selector(keyboardWasHidden:) name:UIKeyboardWillHideNotification object:nil];
    
    [self addTarget:self action:@selector(textFieldEditChanged:) forControlEvents:UIControlEventEditingChanged];
}

-(void)removeTextView
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [self removeFromSuperview];
}

- (void)textFieldEditChanged:(UITextField *)textField
{
    if (_isShouldEdit)
    {
        _isShouldEdit = NO;
        std::string str = [self.beforeText cStringUsingEncoding:NSUTF8StringEncoding];
        self.beforeText = [textField text];
        return;
    }
    
    NSString *stra = @"";
    NSString *strb = @"";
    int location = 0;
    for (int i =0,j =0; i<[self.beforeText length]; i++)
    {
        location = i;
        CC_BREAK_IF(i+1>[textField text].length);
        
        strb = [self.beforeText substringWithRange:NSMakeRange(i, 1)];
        stra = [[textField text] substringWithRange:NSMakeRange(j, 1)];
        
        CC_BREAK_IF(![strb isEqualToString:stra]);
        j++;
    }

    std::string str = [self.beforeText cStringUsingEncoding:NSUTF8StringEncoding];
    self.beforeText = [textField text];

    if (_textField->getDelegate())
    {
        _textField->getDelegate()->textFieldAfterTextChanged(_textField,
                                                             str.c_str(),
                                                             "",
                                                             location,
                                                             1,
                                                             0);
    }
    
    _isShouldEdit = NO;
}

- (void) keyboardWillWasShown:(NSNotification *) notif
{
    NSDictionary *info = [notif userInfo];
    NSValue *value = [info objectForKey:UIKeyboardFrameEndUserInfoKey];
    CGSize keyboardSize = [value CGRectValue].size;
    
    CGFloat scale  = [[UIScreen mainScreen] scale];
    int height = CrossApp::s_px_to_dip(keyboardSize.height * scale);
    
    if (_textField->getDelegate())
    {
        _textField->getDelegate()->keyBoardHeight(_textField, height);
    }
}

- (void) keyboardWasHidden:(NSNotification *) notif
{
    if (_textField->getDelegate())
    {
        _textField->getDelegate()->keyBoardHeight(_textField, 0);
    }
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    if (_textField->isAllowkeyBoardHide())
    {
        _textField->resignFirstResponder();
    }
    
    if (_textField->getDelegate())
    {
        _textField->getDelegate()->textFieldShouldReturn(_textField);
    }
    return NO;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    _isShouldEdit = YES;
    
    if (_textField->getMaxLenght() > 0)
    {
        NSUInteger oldLength = [[textField text] length];
        NSUInteger replacementLength = [string length];
        NSUInteger rangeLength = range.length;
        
        NSUInteger newLength = oldLength - rangeLength + replacementLength;
        
        if (newLength > _textField->getMaxLenght())
        {
            return NO;
        }
    }
    
    std::string str = [self.beforeText cStringUsingEncoding:NSUTF8StringEncoding];
    std::string insert = [string cStringUsingEncoding:NSUTF8StringEncoding];
    self.beforeText = [textField text];
    
    if (_textField->getDelegate())
    {
        _textField->getDelegate()->textFieldAfterTextChanged(_textField, str.c_str(), insert.c_str(), (int)range.location, 0, (int)string.length);
    }
    
    
    return YES;
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
    
    CGPoint point = CGPointMake(-5000, -5000);
    m_pTextField = [[IOSTextField alloc]initWithFrame:CGRectMake(point.x, point.y, 100, 40)];
    EAGLView * eaglview = [EAGLView sharedEGLView];
    [eaglview addSubview:textField_iOS];
    [textField_iOS release];
    textField_iOS.textField = this;
    textField_iOS.delegate = textField_iOS;
    textField_iOS.regiestKeyBoardMessage;
    textField_iOS.keyboardType = UIKeyboardTypeDefault;
    textField_iOS.returnKeyType = UIReturnKeyDone;
    textField_iOS.borderStyle = UITextBorderStyleNone;
    textField_iOS.placeholder = @"";
    CGFloat scale = [[UIScreen mainScreen] scale];
    textField_iOS.font = [UIFont systemFontOfSize:s_dip_to_px(m_iFontSize) / scale];
    
    setMarginLeft(m_iMarginLeft);
    setMarginRight(m_iMarginRight);
}

CATextField::~CATextField()
{
    [textField_iOS removeTextView];
}

void CATextField::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    this->delayShowImage();
}

void CATextField::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

bool CATextField::resignFirstResponder()
{
    if (m_pDelegate && (!m_pDelegate->textFieldShouldEndEditing(this)))
    {
        return false;
    }
    
    bool result = CAView::resignFirstResponder();

    if ([textField_iOS isFirstResponder])
    {
        [textField_iOS resignFirstResponder];
    }
    this->showImage();
    
    this->showTextField();
    
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
    
    CAViewAnimation::beginAnimations(m_s__StrID + "hideTextField", NULL);
    CAViewAnimation::setAnimationDuration(0);
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextField::hideTextField));
    CAViewAnimation::commitAnimations();
    
    [textField_iOS becomeFirstResponder];
    
    this->showNativeTextField();
    
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
    
    [textField_iOS performSelector:@selector(hide) withObject:nil afterDelay:1/60.0f];
}

void CATextField::showNativeTextField()
{
    this->update(0);
    CAScheduler::schedule(schedule_selector(CATextField::update), this, 1/60.0f);
}

void CATextField::delayShowImage()
{
    if (!CAViewAnimation::areBeginAnimationsWithID(m_s__StrID + "showImage"))
    {
        CAViewAnimation::beginAnimations(m_s__StrID + "showImage", NULL);
        CAViewAnimation::setAnimationDuration(0);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextField::showImage));
        CAViewAnimation::commitAnimations();
    }
}

void CATextField::showImage()
{
    static float scale = [UIScreen mainScreen].scale;
    UIGraphicsBeginImageContextWithOptions(textField_iOS.bounds.size, NO, scale);
    CGContextRef context = UIGraphicsGetCurrentContext();
    [textField_iOS.layer renderInContext:context];
    UIImage *image_iOS = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    NSData* iOSData = UIImagePNGRepresentation(image_iOS);
    //[image_iOS release];
    
    unsigned char* data = (unsigned char*)malloc([iOSData length]);
    [iOSData getBytes:data];
    
    CAImage *image = CAImage::createWithImageDataNoCache(data, iOSData.length);
    free(data);

    m_pImgeView->setImage(image);
    m_pImgeView->setFrame(this->getBounds());
    this->updateDraw();
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

    return true;
}

void CATextField::update(float dt)
{
    do
    {
        //CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);

        CGFloat scale = [[UIScreen mainScreen] scale];
        CGRect rect = textField_iOS.frame;
        rect.origin.x = s_dip_to_px(point.x) / scale;
        rect.origin.y = s_dip_to_px(point.y) / scale;
        textField_iOS.frame = rect;
    }
    while (0);
}

void CATextField::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
    
    DSize worldContentSize = this->convertToWorldSize(m_obContentSize);
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect rect = textField_iOS.frame;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    textField_iOS.frame = rect;

    m_pBackgroundView->setFrame(this->getBounds());
    m_pImgeView->setFrame(this->getBounds());
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
    
    UIKeyboardType keyBoardType = UIKeyboardTypeDefault;
    switch (type) {
        case KeyboardType::NumbersAndPunctuation:
            keyBoardType = UIKeyboardTypeNumbersAndPunctuation;
            break;
        case KeyboardType::URL:
            keyBoardType = UIKeyboardTypeURL;
            break;
        case KeyboardType::NumberPad:
            keyBoardType = UIKeyboardTypeNumberPad;
            break;
        case KeyboardType::PhonePad:
            keyBoardType = UIKeyboardTypePhonePad;
            break;
        case KeyboardType::NamePhonePad:
            keyBoardType = UIKeyboardTypeNamePhonePad;
            break;
        case KeyboardType::EmailAddress:
            keyBoardType = UIKeyboardTypeEmailAddress;
            break;
        default:
            keyBoardType = UIKeyboardTypeDefault;
            break;
    }
    
    textField_iOS.keyboardType = keyBoardType;
}

const CATextField::KeyboardType& CATextField::getKeyboardType()
{
    return m_eKeyBoardType;
}

void CATextField::setReturnType(const ReturnType &var)
{
    m_eReturnType = var;
    
    UIReturnKeyType keyBoardReturnType = UIReturnKeyDone;
    switch (var) {
        case Go:
            keyBoardReturnType = UIReturnKeyGo;
            break;
        case Next:
            keyBoardReturnType = UIReturnKeyNext;
            break;
        case Search:
            keyBoardReturnType = UIReturnKeySearch;
            break;
        case Send:
            keyBoardReturnType = UIReturnKeySend;
            break;
        default:
            keyBoardReturnType = UIReturnKeyDone;
            break;
    }
    
    textField_iOS.returnKeyType = keyBoardReturnType;
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
    
    textField_iOS.placeholder = [NSString stringWithUTF8String:m_sPlaceHolderText.c_str()];
    
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
    
    UIColor* color = [UIColor colorWithRed:var.r/255.f green:var.g/255.f blue:var.b/255.f alpha:var.a];
    [textField_iOS setValue:color forKeyPath:@"_placeholderLabel.textColor"];
    
    this->delayShowImage();
}

const CAColor4B& CATextField::getPlaceHolderColor()
{
    return m_cPlaceHdolderColor;
}

void CATextField::setFontSize(int var)
{
    m_iFontSize = var;
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    textField_iOS.font = [UIFont systemFontOfSize:s_dip_to_px(m_iFontSize) / scale];
    [textField_iOS setValue:textField_iOS.font forKeyPath:@"_placeholderLabel.font"];
    
    this->delayShowImage();
}

int CATextField::getFontSize()
{
    return m_iFontSize;
}

void CATextField::setText(const std::string &var)
{
    m_sText = var;
    
    textField_iOS.text = [NSString stringWithUTF8String:m_sText.c_str()];
    textField_iOS.beforeText = [textField_iOS text];
    
    this->delayShowImage();
}

const std::string& CATextField::getText()
{
    m_sText = [textField_iOS.text UTF8String];
    return m_sText;
}

void CATextField::setTextColor(const CAColor4B &var)
{
    if (CAColor4BEqual(m_cTextColor, var)) {
        return;
    }
    
    m_cTextColor = var;
    
    textField_iOS.textColor = [UIColor colorWithRed:var.r/255.f green:var.g/255.f blue:var.b/255.f alpha:var.a];
    
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
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGFloat x = s_dip_to_px(worldContentSize.width) / scale;
    
    UIView *paddingView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, x, 0)];
    textField_iOS.leftView = paddingView;
    textField_iOS.leftViewMode = UITextFieldViewModeAlways;
    [paddingView release];
    
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
        
        CGFloat scale = [[UIScreen mainScreen] scale];
        CGFloat x = s_dip_to_px(worldContentSize.width) / scale;
        
        UIView *paddingView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, x, 0)];
        textField_iOS.rightView = paddingView;
        textField_iOS.rightViewMode = UITextFieldViewModeAlways;
        [paddingView release];
        
        this->delayShowImage();
    }
}

int CATextField::getMarginRight()
{
    return m_iMarginRight;
}

void CATextField::setMarginImageLeft(const DSize& imgSize,const std::string& filePath)
{
    DSize worldContentSize = this->convertToWorldSize(imgSize);
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect rect;
    rect.origin.x = 0;
    rect.origin.y = 0;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    
    UIImage* image = [UIImage imageNamed:[NSString stringWithUTF8String:filePath.c_str()]];
    UIImageView* paddingView=[[UIImageView alloc]initWithImage:image];
    [paddingView setFrame:rect];
    textField_iOS.leftView = paddingView;
    textField_iOS.leftViewMode = UITextFieldViewModeAlways;
    
    this->delayShowImage();
}

void CATextField::setMarginImageRight(const DSize& imgSize,const std::string& filePath)
{
    DSize worldContentSize = this->convertToWorldSize(imgSize);
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect rect;
    rect.origin.x = 0;
    rect.origin.y = 0;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    
    UIImage* image = [UIImage imageNamed:[NSString stringWithUTF8String:filePath.c_str()]];
    UIImageView* paddingView=[[UIImageView alloc]initWithImage:image];
    [paddingView setFrame:rect];
    textField_iOS.rightView = paddingView;
    textField_iOS.rightViewMode = UITextFieldViewModeAlways;
    
    this->delayShowImage();
}

void CATextField::setClearButtonMode(const ClearButtonMode &var)
{
    m_eClearBtn = var;
    
    UITextFieldViewMode mode= UITextFieldViewModeNever;
    switch (var)
    {
        case WhileEditing:
            mode = UITextFieldViewModeWhileEditing;
            textField_iOS.rightViewMode = UITextFieldViewModeNever;
            break;
            
        default:
            mode = UITextFieldViewModeNever;
            break;
    }
    textField_iOS.clearButtonMode = mode;
    
    this->delayShowImage();
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
        case Center:
            textField_iOS.textAlignment = NSTextAlignmentCenter;
            break;
        case Left:
            textField_iOS.textAlignment = NSTextAlignmentLeft;
            break;
        case Right:
            textField_iOS.textAlignment = NSTextAlignmentRight;
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
    [textField_iOS setSecureTextEntry:m_bSecureTextEntry];
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

NS_CC_END



