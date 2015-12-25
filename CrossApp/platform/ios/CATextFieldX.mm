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
#import <UIKit/UIKit.h>

#define textField_iOS ((IOSTextField*)m_pTextField)

@interface IOSTextField: UITextField<UITextFieldDelegate>
{
    BOOL _isShouldEdit;
}

@property(nonatomic,assign) CrossApp::CATextFieldX* textField;
@property(nonatomic,retain) NSString* beforeText;

-(void)regiestKeyBoardMessage;

@end

@implementation IOSTextField
{
    
}

-(void)hide
{
    CGRect rect = self.frame;
    rect.origin = CGPointMake(5000, 5000);
    self.frame = rect;
}

-(void)regiestKeyBoardMessage
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillWasShown:) name:UIKeyboardWillShowNotification object:nil];

    [[NSNotificationCenter defaultCenter]  addObserver:self selector:@selector(keyboardWasHidden:) name:UIKeyboardWillHideNotification object:nil];
    
    [self addTarget:self action:@selector(textFieldEditChanged:) forControlEvents:UIControlEventEditingChanged];
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
        _textField->getDelegate()->textFieldAfterTextChanged(_textField, str.c_str(), "haha", location, 1, 0);
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

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{

    std::string str = self.beforeText != nil ? [self.beforeText cStringUsingEncoding:NSUTF8StringEncoding] : "";
    std::string insert = [string cStringUsingEncoding:NSUTF8StringEncoding];
    self.beforeText = [textField text];
    
    if (_textField->getDelegate())
    {
        _textField->getDelegate()->textFieldAfterTextChanged(_textField, str.c_str(), insert.c_str(), (int)range.location, 0, (int)string.length);
    }
    
    _isShouldEdit = YES;
    return YES;
}

@end




NS_CC_BEGIN
CATextFieldX::CATextFieldX()
:m_pBackgroundView(NULL)
,m_pImgeView(NULL)
,m_pTextField(NULL)
,m_bUpdateImage(true)
,m_marginLeft(10)
,m_marginRight(10)
,m_pDelegate(NULL)
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

    if ([textField_iOS isFirstResponder])
    {
        [textField_iOS resignFirstResponder];
    }
    this->showImage();
    
    this->showTextField();
    
    this->hideNativeTextField();
    
    return result;
}

bool CATextFieldX::becomeFirstResponder()
{
    if (m_pDelegate &&( !m_pDelegate->textFieldShouldBeginEditing(this)))
    {
        return false;
    }
    
    bool result = CAView::becomeFirstResponder();
    
    CAViewAnimation::beginAnimations(m_s__StrID + "hideTextField", NULL);
    CAViewAnimation::setAnimationDuration(0);
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextFieldX::hideTextField));
    CAViewAnimation::commitAnimations();
    
    [textField_iOS becomeFirstResponder];
    
    this->showNativeTextField();
    
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
    
    [textField_iOS performSelector:@selector(hide) withObject:nil afterDelay:1/60.0f];
}

void CATextFieldX::showNativeTextField()
{
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
    CGPoint point = CGPointMake(5000, 5000);
    m_pTextField = [[IOSTextField alloc]initWithFrame:CGRectMake(point.x, point.y, 100, 40)];
    EAGLView * eaglview = [EAGLView sharedEGLView];
    [eaglview addSubview:textField_iOS];
    textField_iOS.textField = this;
    textField_iOS.delegate = textField_iOS;
    textField_iOS.regiestKeyBoardMessage;
    textField_iOS.keyboardType = UIKeyboardTypeDefault;
    textField_iOS.returnKeyType = UIReturnKeyDone;
    textField_iOS.placeholder = @"placeholder";
    textField_iOS.borderStyle = UITextBorderStyleNone;

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
        
        CC_BREAK_IF(m_obLastPoint.equals(point));

        CGFloat scale = [[UIScreen mainScreen] scale];
        CGRect rect = textField_iOS.frame;
        rect.origin.x = s_dip_to_px(point.x) / scale;
        rect.origin.y = s_dip_to_px(point.y) / scale;
        textField_iOS.frame = rect;
    }
    while (0);
}

void CATextFieldX::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
    
    DSize worldContentSize = DSizeApplyAffineTransform(m_obContentSize, worldToNodeTransform());
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect rect = textField_iOS.frame;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    textField_iOS.frame = rect;

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

void CATextFieldX::setKeyboardType(const KeyboardType& type){
    m_keyBoardType = type;
    
    UIKeyboardType keyBoardType = UIKeyboardTypeDefault;
    switch (type) {
        case KeyboardType::KeyboardTypeNumbersAndPunctuation:
            keyBoardType = UIKeyboardTypeNumbersAndPunctuation;
            break;
        case KeyboardType::KeyboardTypeURL:
            keyBoardType = UIKeyboardTypeURL;
            break;
        case KeyboardType::KeyboardTypeNumberPad:
            keyBoardType = UIKeyboardTypeNumberPad;
            break;
        case KeyboardType::KeyboardTypePhonePad:
            keyBoardType = UIKeyboardTypePhonePad;
            break;
        case KeyboardType::KeyboardTypeNamePhonePad:
            keyBoardType = UIKeyboardTypeNamePhonePad;
            break;
        case KeyboardType::KeyboardTypeEmailAddress:
            keyBoardType = UIKeyboardTypeEmailAddress;
            break;
        default:
            keyBoardType = UIKeyboardTypeDefault;
            break;
    }
    
    textField_iOS.keyboardType = keyBoardType;
}

const CATextFieldX::KeyboardType& CATextFieldX::getKeyboardType()
{
    return m_keyBoardType;
}

void CATextFieldX::setReturnType(const ReturnType &var)
{
    m_returnType = var;
    
    UIReturnKeyType keyBoardReturnType = UIReturnKeyDone;
    switch (var) {
        case ReturnType::ReturnTypeGo:
            keyBoardReturnType = UIReturnKeyGo;
            break;
        case ReturnType::ReturnTypeNext:
            keyBoardReturnType = UIReturnKeyNext;
            break;
        case ReturnType::ReturnTypeSearch:
            keyBoardReturnType = UIReturnKeySearch;
            break;
        case ReturnType::ReturnTypeSend:
            keyBoardReturnType = UIReturnKeySend;
            break;
        default:
            keyBoardReturnType = UIReturnKeyDone;
            break;
    }
    
    textField_iOS.returnKeyType = keyBoardReturnType;
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
    
    textField_iOS.placeholder = [NSString stringWithUTF8String:m_placeHolderText.c_str()];
    
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
    
    UIColor* color = [UIColor colorWithRed:var.r green:var.g blue:var.b alpha:var.a];
    [textField_iOS setValue:color forKeyPath:@"_placeholderLabel.textColor"];
    
    this->delayShowImage();
}

const CAColor4B& CATextFieldX::getPlaceHolderColor()
{
    return m_placeHdolderColor;
}

void CATextFieldX::setFontSize(const int &var)
{
    m_fontSize = var;
    
    textField_iOS.font = [UIFont systemFontOfSize:var];
    [textField_iOS setValue:textField_iOS.font forKeyPath:@"_placeholderLabel.font"];
    
    this->delayShowImage();
}

const int& CATextFieldX::getFontSize()
{
    return m_fontSize;
}

void CATextFieldX::setFieldText(const std::string &var)
{
    m_fieldText = var;
    
    textField_iOS.text = [NSString stringWithUTF8String:m_fieldText.c_str()];
    textField_iOS.beforeText = [textField_iOS text];
    
    this->delayShowImage();
}

const std::string& CATextFieldX::getFieldText()
{
    return m_fieldText;
}

void CATextFieldX::setFieldTextColor(const CAColor4B &var)
{
    if (CAColor4BEqual(m_fieldTextColor, var)) {
        return;
    }
    
    m_fieldTextColor = var;
    
    textField_iOS.textColor = [UIColor colorWithRed:var.r green:var.g blue:var.b alpha:var.a];
    
    this->delayShowImage();
}

const CAColor4B& CATextFieldX::getFieldTextColor()
{
    return m_fieldTextColor;
}

void CATextFieldX::setMarginLeft(const int &var)
{
    m_marginLeft = var;
    
    DSize worldContentSize = DSizeApplyAffineTransform(DSize(var, 0), worldToNodeTransform());
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGFloat x = s_dip_to_px(worldContentSize.width) / scale;
    
    UIView *paddingView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, x, 0)];
    textField_iOS.leftView = paddingView;
    textField_iOS.leftViewMode = UITextFieldViewModeAlways;
    [paddingView release];
    
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
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGFloat x = s_dip_to_px(worldContentSize.width) / scale;
    
    UIView *paddingView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, x, 0)];
    textField_iOS.rightView = paddingView;
    textField_iOS.rightViewMode = UITextFieldViewModeAlways;
    [paddingView release];
    
    this->delayShowImage();
}

const int& CATextFieldX::getMarginRight()
{
    return m_marginRight;
}

void CATextFieldX::setMarginImageLeft(const DSize imgSize,const std::string& filePath)
{
    DSize worldContentSize = DSizeApplyAffineTransform(imgSize, worldToNodeTransform());
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

void CATextFieldX::setMarginImageRight(const DSize imgSize,const std::string& filePath)
{
    DSize worldContentSize = DSizeApplyAffineTransform(imgSize, worldToNodeTransform());
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

void CATextFieldX::setClearButtonMode(const ClearButtonMode &var)
{
    m_clearBtn = var;
    
    UITextFieldViewMode mode= UITextFieldViewModeNever;
    switch (var)
    {
        case ClearButtonMode::ClearButtonWhileEditing:
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

const CATextFieldX::ClearButtonMode& CATextFieldX::getClearButtonMode()
{
    return m_clearBtn;
}

void CATextFieldX::setTextFieldAlign(const TextFieldAlign &var)
{
    m_align = var;
    
    switch (var)
    {
        case CATextFieldX::TextEditAlignCenter:
            textField_iOS.textAlignment = NSTextAlignmentCenter;
            break;
        case CATextFieldX::TextEditAlignLeft:
            textField_iOS.textAlignment = NSTextAlignmentLeft;
            break;
        case CATextFieldX::TextEditAlignRight:
            textField_iOS.textAlignment = NSTextAlignmentRight;
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
NS_CC_END



