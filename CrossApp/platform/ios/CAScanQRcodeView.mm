//
//  CAScanQRcode.cpp
//  CrossApp
//
//  Created by mac on 2021/3/12.
//  Copyright © 2021 CrossApp. All rights reserved.
//

#import "CAScanQRcodeView.h"
#import <Foundation/Foundation.h>
#include "control/CABar.h"
#include "basics/CAApplication.h"
#include "dispatcher/CATouchDispatcher.h"
#include "platform/CADensityDpi.h"

#include "view/CAWindow.h"
#include "EAGLView.h"

#define SCANVIEW_EdgeTop 200.0
#define SCANVIEW_EdgeLeft 50.0

#define TINTCOLOR_ALPHA 0.2  //浅色透明度
#define DARKCOLOR_ALPHA 0.5  //深色透明度

static CAScanQRcodeView *_scanQRcodeView = nil;

@implementation CAScanQRcodeView

+ (id)show:(const std::function<void (const std::string &)> &)block
{
    if (_scanQRcodeView == nil)
    {
        CrossApp::CAApplication::getApplication()->pause();
        CrossApp::CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
        
        EAGLView* eaglview = [EAGLView sharedEGLView];
        _scanQRcodeView = [[[CAScanQRcodeView alloc] initWithFrame:eaglview.bounds callback:block] autorelease];
        [eaglview addSubview:_scanQRcodeView];
    }
    
    return _scanQRcodeView;
}

- (id)initWithFrame:(CGRect) rect callback:(const std::function<void(const std::string&)>&) block
{
    self = [super initWithFrame:rect];
    if (self) {
        
        _block = block;

        _scanViewRect = self.bounds;
        //初始化扫描界面
        [self setScanView];
        
        _readerView = [[[ZBarReaderView alloc] init] autorelease];
        _readerView.frame = self.bounds;
        _readerView.tracksSymbols = NO;
        _readerView.readerDelegate = self;
        [_readerView addSubview:_scanView];
        //关闭闪光灯
        _readerView.torchMode = 0;
        
        [self addSubview:_readerView];
        
        //扫描区域
        [_readerView start];
        
        [self createTimer];
        
        int top = CrossApp::CABar::get_top_clearance(CrossApp::CAApplication::getApplication()->getRootWindow());
        float y = top / 2 + 20;
        
        UIButton *btn_closed = nil;
        btn_closed = [UIButton buttonWithType:UIButtonTypeCustom];
        [btn_closed setBackgroundImage:[UIImage imageNamed:@"source_material/btn_left_white.png"] forState:UIControlStateNormal];
        [btn_closed setFrame:CGRectMake(30, y, 24, 24)];
        [self addSubview: btn_closed];
        [btn_closed addTarget:self action:@selector(closed) forControlEvents:UIControlEventTouchUpInside];
                
        
        [self setCenter:CGPointMake(self.bounds.size.width * 0.5, self.bounds.size.height * 1.5)];
        [UIView animateWithDuration:0.3 delay:0 options:UIViewAnimationOptionCurveEaseOut animations:^{
            [self setCenter:CGPointMake(self.bounds.size.width * 0.5, self.bounds.size.height * 0.5)];
        } completion:^(BOOL finished){}];
    }
    return self;
}

- (void) closed
{
    [self setCenter:CGPointMake(self.bounds.size.width * 0.5, self.bounds.size.height * 0.5)];
    [UIView animateWithDuration:0.3 delay:0 options:UIViewAnimationOptionCurveEaseOut animations:^{
        [self setCenter:CGPointMake(self.bounds.size.width * 0.5, self.bounds.size.height * 1.5)];
    } completion:^(BOOL finished){
        if (finished == YES)
        {
            if (_readerView.torchMode == 1) {
                _readerView.torchMode = 0;
            }
            [self  stopTimer];
            [_readerView stop];
            [self removeFromSuperview];
            _scanQRcodeView = nil;
            CrossApp::CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
            CrossApp::CAApplication::getApplication()->resume();
        }
    }];
    
}

#pragma mark -- ZBarReaderViewDelegate
-(void)readerView:(ZBarReaderView *)readerView didReadSymbols:(ZBarSymbolSet *)symbols fromImage:(UIImage *)image
{
    const zbar_symbol_t *symbol = zbar_symbol_set_first_symbol(symbols.zbarSymbolSet);
    NSString *symbolStr = [NSString stringWithUTF8String: zbar_symbol_get_data(symbol)];
    
    if (_block) _block([symbolStr UTF8String]);
    
    [self closed];
}

//二维码的扫描区域
- (void)setScanView
{
    _scanView = [[UIView alloc] initWithFrame:_scanViewRect];
    _scanView.backgroundColor = [UIColor clearColor];
    
    CGFloat left = SCANVIEW_EdgeLeft;
    CGFloat top = SCANVIEW_EdgeTop;
    CGFloat side = _scanViewRect.size.width - 2 * SCANVIEW_EdgeLeft;
    
    //最上部view
    UIView* upView = [[UIView alloc] initWithFrame:CGRectMake(0,0, _scanViewRect.size.width, top)];
    upView.alpha = TINTCOLOR_ALPHA;
    upView.backgroundColor = [UIColor blackColor];
    [_scanView addSubview:upView];
    
    //底部view
    UIView *downView = [[UIView alloc] initWithFrame:CGRectMake(0, top + side, _scanViewRect.size.width, _scanViewRect.size.height - top - side)];
    //downView.alpha = TINTCOLOR_ALPHA;
    downView.backgroundColor = [[UIColor blackColor] colorWithAlphaComponent:TINTCOLOR_ALPHA];
    [_scanView addSubview:downView];
    
    //左侧的view
    UIView *leftView = [[UIView alloc] initWithFrame:CGRectMake(0, top, left, side)];
    leftView.alpha = TINTCOLOR_ALPHA;
    leftView.backgroundColor = [UIColor blackColor];
    [_scanView addSubview:leftView];
    
    //右侧的view
    UIView *rightView = [[UIView alloc] initWithFrame:CGRectMake(left + side, top, left, side)];
    rightView.alpha =TINTCOLOR_ALPHA;
    rightView.backgroundColor = [UIColor blackColor];
    [_scanView addSubview:rightView];
    
    //用于说明的label
    UILabel *labIntroudction = [[UILabel alloc] init];
    labIntroudction.backgroundColor = [UIColor clearColor];
    labIntroudction.frame = CGRectMake(0,5, _scanViewRect.size.width, 20);
    labIntroudction.numberOfLines = 1;
    labIntroudction.font = [UIFont systemFontOfSize:15.0];
    labIntroudction.textAlignment = NSTextAlignmentCenter;
    labIntroudction.textColor = [UIColor whiteColor];
    labIntroudction.text = @"将二维码对准方框，即可自动扫描";
    [downView addSubview:labIntroudction];
    
    /******************中间扫描区域****************************/
    UIView *scanCropView = [[UIView alloc] initWithFrame:CGRectMake(left, top, side, side)];
    
    scanCropView.layer.borderColor = [UIColor greenColor].CGColor;
    scanCropView.layer.borderWidth = 2.0;
    
    scanCropView.backgroundColor  =[UIColor clearColor];
    [_scanView addSubview:scanCropView];
    
    //画中间的基准线
    _QrCodeline = [[UIView alloc] initWithFrame:CGRectMake(left, top, side, 2)];
    _QrCodeline.backgroundColor = [UIColor greenColor];
    [_scanView addSubview:_QrCodeline];
}

//二维码的横线移动
- (void)moveUpAndDownLine
{
    CGFloat left = SCANVIEW_EdgeLeft;
    CGFloat top = SCANVIEW_EdgeTop;
    CGFloat side = _scanViewRect.size.width - 2 * SCANVIEW_EdgeLeft;
    
    CGFloat Y = _QrCodeline.frame.origin.y;
    
    if (side + top == Y)
    {
        
        [UIView beginAnimations:@"moveUpAndDownLine" context:nil];
        [UIView setAnimationDuration:1];
        _QrCodeline.frame = CGRectMake(left, top, side, 1);
        [UIView commitAnimations];
    }
    else if(top == Y)
    {
        [UIView beginAnimations:@"moveUpAndDownLine" context:nil];
        [UIView setAnimationDuration:1];
        _QrCodeline.frame = CGRectMake(left, top + side, side, 1);
        [UIView commitAnimations];
    }
    
}

- (void)createTimer
{
    //创建一个时间计数
    _timer = [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(moveUpAndDownLine) userInfo:nil repeats:YES];
}

- (void)stopTimer
{
    if ([_timer isValid] == YES) {
        [_timer invalidate];
        _timer =nil;
    }
}

- (void)dealloc
{
    [super dealloc];
}

@end


