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
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
#import "EAGLView.h"
#import <UIKit/UIKit.h>

#define textField_iOS ((IOSTextField*)m_pTextField)

static std::map<CrossApp::CATextField*, std::function<bool()> > s_ShouldBeginEditing_map;
static std::map<CrossApp::CATextField*, std::function<bool()> > s_ShouldEndEditing_map;
static std::map<CrossApp::CATextField*, std::function<void()> > s_ShouldReturn_map;
static std::map<CrossApp::CATextField*, std::function<void(int height)> > s_KeyBoardHeight_map;
static std::map<CrossApp::CATextField*, std::function<bool(ssize_t, ssize_t, const std::string&)> > s_ShouldChangeCharacters_map;
static std::map<CrossApp::CATextField*, std::function<void()> > s_DidChangeText_map;

@interface IOSTextFieldDelegate: NSObject<UITextFieldDelegate>
{
    
}

@property(nonatomic,assign) id textField;

@end

@interface IOSTextField: UITextField<UITextFieldDelegate>
{

}

@property(nonatomic,assign) CrossApp::CATextField* textField;
@property(nonatomic,retain) IOSTextFieldDelegate* textFieldDelegate;


-(void)regiestKeyBoardMessage;
-(void)removeTextField;
-(void)hide;
@end



@implementation IOSTextFieldDelegate
{
    
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    CrossApp::CATextField* crossTextField = ((IOSTextField*)self.textField).textField;
    
    if (crossTextField->isAllowkeyBoardHide())
    {
        crossTextField->resignFirstResponder();
    }
    
    if (s_ShouldReturn_map.count(crossTextField) > 0 && s_ShouldReturn_map[crossTextField])
    {
        s_ShouldReturn_map[crossTextField]();
    }
    else if (crossTextField->getDelegate())
    {
        crossTextField->getDelegate()->textFieldShouldReturn(crossTextField);
    }
    return NO;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    CrossApp::CATextField* crossTextField = ((IOSTextField*)self.textField).textField;
    
    if (crossTextField->getMaxLength() > 0)
    {
        NSUInteger oldLength = [[textField text] length];
        NSUInteger addLength = [string length];
        NSUInteger delLength = range.length;
        
        NSUInteger newLength = oldLength + addLength - delLength;
        
        if (newLength > crossTextField->getMaxLength())
        {
            return NO;
        }
    }
    
    if (s_ShouldChangeCharacters_map.count(crossTextField) > 0 && s_ShouldChangeCharacters_map[crossTextField])
    {
        return s_ShouldChangeCharacters_map[crossTextField](range.location, range.length, [string UTF8String]);
    }
    else if (crossTextField->getDelegate())
    {
        return crossTextField->getDelegate()->textFieldShouldChangeCharacters(crossTextField,
                                                                          (unsigned int)range.location,
                                                                          (unsigned int)range.length,
                                                                          [string UTF8String]);
    }
    
    
    return YES;
}

- (BOOL)textFieldShouldClear:(UITextField *)textField
{
    CrossApp::CATextField* crossTextField = ((IOSTextField*)self.textField).textField;
    
    if (s_ShouldChangeCharacters_map.count(crossTextField) > 0 && s_ShouldChangeCharacters_map[crossTextField])
    {
        return s_ShouldChangeCharacters_map[crossTextField](0, [textField.text length],"");
    }
    else if (crossTextField->getDelegate())
    {
        return crossTextField->getDelegate()->textFieldShouldChangeCharacters(crossTextField,
                                                                              0,
                                                                              (unsigned int)[textField.text length],
                                                                              "");
    }
    return YES;
}

@end


@implementation IOSTextField
{

}

-(id)initWithFrame:(CGRect)frame
{
    if ([super initWithFrame:frame])
    {
        _textFieldDelegate = [[IOSTextFieldDelegate alloc]init];
        [_textFieldDelegate setTextField:self];
        [self setDelegate: _textFieldDelegate];
        [self regiestKeyBoardMessage];
        [self setKeyboardType: UIKeyboardTypeDefault];
        [self setReturnKeyType: UIReturnKeyDone];
        [self setBorderStyle: UITextBorderStyleNone];
        [self setPlaceholder: @""];
        return self;
    }
    return nil;
}

-(void)dealloc
{
    [self setDelegate: nil];
    [_textFieldDelegate release];
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
    
    [self addTarget:self action:@selector(textFieldDidChange:) forControlEvents:UIControlEventEditingChanged];
}

-(void)removeTextField
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [self removeFromSuperview];
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
        
        if ([self isFirstResponder])
        {
            if (s_KeyBoardHeight_map.count(_textField) > 0 && s_KeyBoardHeight_map[_textField])
            {
                s_KeyBoardHeight_map[_textField](height);
            }
            else if (_textField->getDelegate())
            {
                _textField->getDelegate()->keyBoardHeight(_textField, height);
            }
        }
    }
}

- (void) keyboardWasHidden:(NSNotification *) notif
{
    if ([self isFirstResponder])
    {
        if (s_KeyBoardHeight_map.count(_textField) > 0 && s_KeyBoardHeight_map[_textField])
        {
            s_KeyBoardHeight_map[_textField](0);
        }
        else if (_textField->getDelegate())
        {
            _textField->getDelegate()->keyBoardHeight(_textField, 0);
        }
    }
}

- (void) textFieldDidChange:(NSNotification *) notif
{
    if ([self isFirstResponder])
    {
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
    CGPoint point = CGPointMake(-5000, -5000);
    m_pTextField = [[IOSTextField alloc]initWithFrame:CGRectMake(point.x, point.y, 100, 40)];
    EAGLView * eaglview = [EAGLView sharedEGLView];
    [eaglview addSubview:textField_iOS];
    [textField_iOS release];
    textField_iOS.textField = this;
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    textField_iOS.font = [UIFont systemFontOfSize:s_dip_to_px(m_iFontSize) / scale];
    
    setMarginLeft(m_iMarginLeft);
    setMarginRight(m_iMarginRight);
}

CATextField::~CATextField()
{
    s_ShouldBeginEditing_map.erase(this);
    s_ShouldEndEditing_map.erase(this);
    s_ShouldReturn_map.erase(this);
    s_KeyBoardHeight_map.erase(this);
    s_ShouldChangeCharacters_map.erase(this);
    s_DidChangeText_map.erase(this);
    
    [textField_iOS removeTextField];
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
    if (s_ShouldBeginEditing_map.count(this) > 0 && s_ShouldBeginEditing_map[this] && !s_ShouldBeginEditing_map[this]())
    {
        return false;
    }
    else if (m_pDelegate &&( !m_pDelegate->textFieldShouldBeginEditing(this)))
    {
        return false;
    }
    
    bool result = CAControl::becomeFirstResponder();
    
    this->hideTextField();
    
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
    CAScheduler::getScheduler()->unschedule(schedule_selector(CATextField::update), this);
    
    [textField_iOS performSelector:@selector(hide) withObject:nil afterDelay:1/60.0f];
}

void CATextField::showNativeTextField()
{
    this->update(0);
    CAScheduler::getScheduler()->schedule(schedule_selector(CATextField::update), this, 1/60.0f);
}

void CATextField::delayShowImage()
{
    this->cancelPreviousPerformRequests(callfunc_selector(CATextField::showImage));
    this->performSelector(callfunc_selector(CATextField::showImage), 0);
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
    unsigned char* data = (unsigned char*)malloc([iOSData length]);
    [iOSData getBytes:data];
    
    CAData* ca_data = CAData::create();
    ca_data->fastSet(data, [iOSData length]);
    
    CAImage *image = CAImage::createWithImageDataNoCache(ca_data);

    m_pImgeView->setImage(image);
}

void CATextField::updateAttributedPlaceholder()
{
    NSString* placeHolderText = [NSString stringWithUTF8String:m_sPlaceHolderText.c_str()];
    
    UIColor* color = [UIColor colorWithRed:m_cPlaceHdolderColor.r/255.f
                                     green:m_cPlaceHdolderColor.g/255.f
                                      blue:m_cPlaceHdolderColor.b/255.f
                                     alpha:m_cPlaceHdolderColor.a];
    
    NSDictionary<NSAttributedStringKey, id>* dict = @{NSForegroundColorAttributeName:color,NSFontAttributeName:textField_iOS.font};
    
    NSAttributedString* attributedPlaceholder = [[NSAttributedString alloc]initWithString:placeHolderText attributes:dict];

    [textField_iOS setAttributedPlaceholder:attributedPlaceholder];
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
    const CAThemeManager::stringMap& map = CAApplication::getApplication()->getThemeManager()->getThemeMap("CATextField");
    CAImage* image = CAImage::create(map.at("backgroundView_normal"));
    DRect capInsets = DRect(image->getPixelsWide()/2 ,image->getPixelsHigh()/2 , 1, 1);
    m_pBackgroundView = CAScale9ImageView::createWithImage(image);
    m_pBackgroundView->setLayout(DLayoutFill);
    m_pBackgroundView->setCapInsets(capInsets);
    this->insertSubview(m_pBackgroundView, -1);
    
    m_pImgeView = CAImageView::createWithLayout(DLayoutFill);
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
    CAControl::setContentSize(contentSize);
    
    DSize worldContentSize = this->convertToWorldSize(m_obContentSize);
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect rect = textField_iOS.frame;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    textField_iOS.frame = rect;
    
    if (m_bRunning)
    {
        this->showImage();
    }
}

bool CATextField::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    m_bMoved = false;
    return true;
}

void CATextField::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    m_bMoved = true;
}

void CATextField::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    DPoint point = this->convertTouchToNodeSpace(pTouch);
    
    if (this->getBounds().containsPoint(point))
    {
        becomeFirstResponder();
    }
    else if (!m_bMoved)
    {
        resignFirstResponder();
    }
    
}

void CATextField::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}

void CATextField::setKeyboardType(KeyboardType type)
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

CATextField::KeyboardType CATextField::getKeyboardType()
{
    return m_eKeyBoardType;
}

void CATextField::setReturnType(CATextField::ReturnType var)
{
    m_eReturnType = var;
    
    UIReturnKeyType keyBoardReturnType = UIReturnKeyDone;
    switch (var) {
        case CATextField::ReturnType::Go:
            keyBoardReturnType = UIReturnKeyGo;
            break;
        case CATextField::ReturnType::Next:
            keyBoardReturnType = UIReturnKeyNext;
            break;
        case CATextField::ReturnType::Search:
            keyBoardReturnType = UIReturnKeySearch;
            break;
        case CATextField::ReturnType::Send:
            keyBoardReturnType = UIReturnKeySend;
            break;
        default:
            keyBoardReturnType = UIReturnKeyDone;
            break;
    }
    
    textField_iOS.returnKeyType = keyBoardReturnType;
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
    
    this->updateAttributedPlaceholder();
    
    this->delayShowImage();
}

const std::string& CATextField::getPlaceHolderText()
{
    return m_sPlaceHolderText;
}

void CATextField::setPlaceHolderColor(const CAColor4B &var)
{
    m_cPlaceHdolderColor = var;
    
    this->updateAttributedPlaceholder();

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
    
    this->updateAttributedPlaceholder();

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
    
    this->delayShowImage();
}

const std::string& CATextField::getText()
{
    m_sText = [textField_iOS.text UTF8String];
    return m_sText;
}

void CATextField::setTextColor(const CAColor4B &var)
{
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
    if (m_eClearBtn == CATextField::ClearButtonMode::None)
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
    
    UIView *paddingView = [[UIView alloc]initWithFrame:rect];
    UIImage* image = [UIImage imageNamed:[NSString stringWithUTF8String:filePath.c_str()]];
    UIImageView *accountLView = [[UIImageView alloc]initWithImage:image];
    accountLView.frame = rect;
    [paddingView addSubview:accountLView];
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
    
    UIView *paddingView = [[UIView alloc]initWithFrame:rect];
    UIImage* image = [UIImage imageNamed:[NSString stringWithUTF8String:filePath.c_str()]];
    UIImageView *accountLView = [[UIImageView alloc]initWithImage:image];
    accountLView.frame = rect;
    [paddingView addSubview:accountLView];
    textField_iOS.rightView = paddingView;
    textField_iOS.rightViewMode = UITextFieldViewModeAlways;
    
    this->delayShowImage();
}

void CATextField::setClearButtonMode(CATextField::ClearButtonMode var)
{
    m_eClearBtn = var;
    
    UITextFieldViewMode mode= UITextFieldViewModeNever;
    switch (var)
    {
        case CATextField::ClearButtonMode::WhileEditing:
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

CATextField::ClearButtonMode CATextField::getClearButtonMode()
{
    return m_eClearBtn;
}

void CATextField::setAlign(CATextField::Align var)
{
    m_eAlign = var;
    
    switch (var)
    {
        case CATextField::Align::Center:
            textField_iOS.textAlignment = NSTextAlignmentCenter;
            break;
        case CATextField::Align::Left:
            textField_iOS.textAlignment = NSTextAlignmentLeft;
            break;
        case CATextField::Align::Right:
            textField_iOS.textAlignment = NSTextAlignmentRight;
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
    [textField_iOS setSecureTextEntry:m_bSecureTextEntry];
    this->delayShowImage();
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



