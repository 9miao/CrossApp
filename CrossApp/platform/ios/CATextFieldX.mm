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
@property(nonatomic,assign) CrossApp::CATextFieldX* textField;

-(void)regiestKeyBoardMessage;
@end
@implementation IOSTextField
{
    
}
-(void)regiestKeyBoardMessage{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillWasShown:) name:UIKeyboardWillShowNotification object:nil];

    [[NSNotificationCenter defaultCenter]  addObserver:self selector:@selector(keyboardWasHidden:) name:UIKeyboardWillHideNotification object:nil];
    
    [self addTarget:self action:@selector(textFieldEditChanged:) forControlEvents:UIControlEventEditingDidBegin];
}
- (void)textFieldEditChanged:(UITextField *)textField
{

}
- (void) keyboardWillWasShown:(NSNotification *) notif
{
    NSDictionary *info = [notif userInfo];
    NSValue *value = [info objectForKey:UIKeyboardFrameEndUserInfoKey];
    CGSize keyboardSize = [value CGRectValue].size;
    
    CGFloat scale  = [[UIScreen mainScreen] scale];
    int height = CrossApp::s_px_to_dip(keyboardSize.height * scale);
    
    
    if (_textField->getDelegate()) {
        _textField->getDelegate()->keyBoardHeight(_textField, height);
    }
}
- (void) keyboardWasHidden:(NSNotification *) notif
{
//    NSDictionary *info = [notif userInfo];
//    
//    NSValue *value = [info objectForKey:UIKeyboardFrameBeginUserInfoKey];
//    CGSize keyboardSize = [value CGRectValue].size;
//    NSLog(@"keyboardWasHidden keyBoard:%f", keyboardSize.height);

    if (_textField->getDelegate()) {
        _textField->getDelegate()->keyBoardHeight(_textField, 0);
    }
}
//- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField        // return NO to disallow editing.
//{
//    
//}
//- (void)textFieldDidBeginEditing:(UITextField *)textField           // became first responder
//{
//    
//}
//- (BOOL)textFieldShouldEndEditing:(UITextField *)textField
//{
//    
//}
//- (void)textFieldDidEndEditing:(UITextField *)textField{
//    
//}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string{
    
    
    NSLog(@"%s  --   %lu---%lu",string.UTF8String,(unsigned long)range.length,(unsigned long)range.location);
    NSLog(@"textField text : %@", [textField text]);
    return YES;
}

- (BOOL)textFieldShouldClear:(UITextField *)textField{
    return YES;
}
- (BOOL)textFieldShouldReturn:(UITextField *)textField{
    return YES;
}
@end





NS_CC_BEGIN
CATextFieldX::CATextFieldX()
:m_pImgeView(NULL)
,m_pTextField(NULL)
,m_pDlayeShow(false)
,m_marginLeft(16)
,m_marginRight(16)
{
    this->setHaveNextResponder(false);
}

CATextFieldX::~CATextFieldX()
{
    
}

void CATextFieldX::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    delayShowImageView();
}

void CATextFieldX::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

bool CATextFieldX::resignFirstResponder()
{
    bool result = CAView::resignFirstResponder();

    if ([textField_iOS isFirstResponder])
    {
        [textField_iOS resignFirstResponder];
    }
    this->showImageView();
    
    m_pImgeView->setVisible(true);
    
    this->hideNativeTextField();
    
    return result;
}

bool CATextFieldX::becomeFirstResponder()
{
    bool result = CAView::becomeFirstResponder();
    
    m_pImgeView->setVisible(false);
    
    [textField_iOS becomeFirstResponder];
    
    this->showNativeTextField();
    return result;
}

void CATextFieldX::hideNativeTextField()
{
    CAScheduler::unschedule(schedule_selector(CATextFieldX::update), this);
    
    CAViewAnimation::beginAnimations(m_s__StrID, NULL);
    CAViewAnimation::setAnimationDuration(0);
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextFieldX::hide));
    CAViewAnimation::commitAnimations();
}

void CATextFieldX::hide()
{
    CGRect rect = textField_iOS.frame;
    rect.origin = CGPointMake(5000, 5000);
    textField_iOS.frame = rect;
}

void CATextFieldX::showNativeTextField()
{
    this->update(0);
    CAScheduler::schedule(schedule_selector(CATextFieldX::update), this, 1/60.0f);
}
void CATextFieldX::delayShowImageView(){
    if (m_pDlayeShow) {
        return;
    }
    
    m_pDlayeShow = true;
    
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0);
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CATextFieldX::showImageView));
    CAViewAnimation::commitAnimations();
}
void CATextFieldX::showImageView()
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
    m_bgImgeView->setFrame(this->getBounds());
    
    m_pDlayeShow = false;
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
    m_bgImgeView = CAScale9ImageView::createWithFrame(DRect(0, 0, 1, 1));
    m_bgImgeView->setCapInsets(DRect(image->getPixelsWide()/2,image->getPixelsHigh()/2,1,1));
    m_bgImgeView->setImage(image);
    this->addSubview(m_bgImgeView);
    
    m_pImgeView = CAImageView::createWithFrame(DRect(0, 0, 1, 1));
    this->addSubview(m_pImgeView);
    return true;
}

void CATextFieldX::update(float dt)
{
    do
    {
        //CC_BREAK_IF(!CAApplication::getApplication()->isDrawing());
        DPoint point = this->convertToWorldSpace(DPointZero);
        
        //CC_BREAK_IF(m_obLastPoint.equals(point) && m_obLastContentSize.equals(contentSize));

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
 //   worldContentSize.width = s_px_to_dip(rect.size.width * scale);
    m_bgImgeView->setFrame(this->getBounds());
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
const CATextFieldX::KeyboardType& CATextFieldX::getKeyboardType(){
    return m_keyBoardType;
}
void CATextFieldX::setReturnType(const ReturnType &var){
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
const CATextFieldX::ReturnType& CATextFieldX::getReturnType(){
    return m_returnType;
}
void CATextFieldX::setBackGroundImage(CAImage* image){
    //设置背景
    if (!image)return;

    m_bgImgeView->setCapInsets(DRect(image->getPixelsWide()/2,image->getPixelsHigh()/2,1,1));
    m_bgImgeView->setImage(image);
}

void CATextFieldX::setPlaceHolderText(const std::string &var){
    m_placeHolderText = var;
    
    textField_iOS.placeholder = [NSString stringWithUTF8String:m_placeHolderText.c_str()];
    delayShowImageView();
}
const std::string& CATextFieldX::getPlaceHolderText(){
    return m_placeHolderText;
}
void CATextFieldX::setPlaceHolderColor(const CAColor4B &var) {
    if (CAColor4BEqual(m_placeHdolderColor, var)) {
        return;
    }
    
    m_placeHdolderColor = var;
    
    [textField_iOS setValue:[UIColor colorWithRed:var.r green:var.g blue:var.b alpha:var.a] forKeyPath:@"_placeholderLabel.textColor"];
    delayShowImageView();
}
const CAColor4B& CATextFieldX::getPlaceHolderColor(){
    return m_placeHdolderColor;
}
void CATextFieldX::setFontSize(const int &var){
    m_fontSize = var;
    
    [textField_iOS setValue:[UIFont boldSystemFontOfSize:var] forKeyPath:@"_placeholderLabel.font"];
    textField_iOS.font = [UIFont boldSystemFontOfSize:var];
    delayShowImageView();
}
const int& CATextFieldX::getFontSize(){
    return m_fontSize;
}
void CATextFieldX::setFieldText(const std::string &var){
    m_fieldText = var;
    
    textField_iOS.text = [NSString stringWithUTF8String:m_fieldText.c_str()];
    delayShowImageView();
}
const std::string& CATextFieldX::getFieldText(){
    return m_fieldText;
}
void CATextFieldX::setFieldTextColor(const CAColor4B &var){
    if (CAColor4BEqual(m_fieldTextColor, var)) {
        return;
    }
    
    m_fieldTextColor = var;
    
    textField_iOS.textColor = [UIColor colorWithRed:var.r green:var.g blue:var.b alpha:var.a];
    delayShowImageView();
}
const CAColor4B& CATextFieldX::getFieldTextColor(){
    return m_fieldTextColor;
}
void CATextFieldX::setMarginLeft(const int &var){
    m_marginLeft = var;
    
    DSize worldContentSize = DSizeApplyAffineTransform(DSize(var, 0), worldToNodeTransform());
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGFloat x = s_dip_to_px(worldContentSize.width) / scale;
    
    UIView *paddingView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, x, 0)];
    textField_iOS.leftView = paddingView;
    textField_iOS.leftViewMode = UITextFieldViewModeAlways;
    [paddingView release];
    
    delayShowImageView();
}
const int& CATextFieldX::getMarginLeft(){
    return m_marginLeft;
}
void CATextFieldX::setMarginRight(const int &var){
    m_marginRight = var;
    
    DSize worldContentSize = DSizeApplyAffineTransform(DSize(var, 0), worldToNodeTransform());
    
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGFloat x = s_dip_to_px(worldContentSize.width) / scale;
    
    UIView *paddingView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, x, 0)];
    textField_iOS.rightView = paddingView;
    textField_iOS.rightViewMode = UITextFieldViewModeAlways;
    [paddingView release];
    
    delayShowImageView();
}
const int& CATextFieldX::getMarginRight(){
    return m_marginRight;
}
void CATextFieldX::setMarginImageLeft(const DSize imgSize,const std::string& filePath){
    DSize worldContentSize = DSizeApplyAffineTransform(imgSize, worldToNodeTransform());
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect rect;
    rect.origin.x = 0;
    rect.origin.y = 0;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    
    UIImageView* paddingView=[[UIImageView alloc]initWithImage:[UIImage imageNamed:[NSString stringWithUTF8String:filePath.c_str()]]];
    [paddingView setFrame:rect];
    textField_iOS.leftView = paddingView;
    textField_iOS.leftViewMode = UITextFieldViewModeAlways;
    
    delayShowImageView();
}
void CATextFieldX::setMarginImageRight(const DSize imgSize,const std::string& filePath){
    DSize worldContentSize = DSizeApplyAffineTransform(imgSize, worldToNodeTransform());
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect rect;
    rect.origin.x = 0;
    rect.origin.y = 0;
    rect.size.width = s_dip_to_px(worldContentSize.width) / scale;
    rect.size.height =  s_dip_to_px(worldContentSize.height) / scale;
    
    UIImageView* paddingView=[[UIImageView alloc]initWithImage:[UIImage imageNamed:[NSString stringWithUTF8String:filePath.c_str()]]];
    [paddingView setFrame:rect];
    textField_iOS.rightView = paddingView;
    textField_iOS.rightViewMode = UITextFieldViewModeAlways;
    
    delayShowImageView();
}
void CATextFieldX::setClearButtonMode(const ClearButtonMode &var){
    m_clearBtn = var;
    
    UITextFieldViewMode mode= UITextFieldViewModeNever;
    switch (var) {
        case ClearButtonMode::ClearButtonWhileEditing:
            mode = UITextFieldViewModeWhileEditing;
            textField_iOS.rightViewMode = UITextFieldViewModeNever;
            break;
            
        default:
            mode = UITextFieldViewModeNever;
            break;
    }
    textField_iOS.clearButtonMode = mode;
    
    delayShowImageView();
}
const CATextFieldX::ClearButtonMode& CATextFieldX::getClearButtonMode(){
    return m_clearBtn;
}
NS_CC_END



