//
//  CABlueTooth.h
//  DeviceTest
//
//  Created by lihui on 15/1/19.
//
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
@interface CABlueTooth : NSObject<CBCentralManagerDelegate>

-(int)getBlueToothState;
-(void)beginScan;
@property(nonatomic,strong)CBCentralManager *manager;
@end
