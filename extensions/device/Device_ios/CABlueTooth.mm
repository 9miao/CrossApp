//
//  CABlueTooth.m
//  DeviceTest
//
//  Created by lihui on 15/1/19.
//
//

#import "CABlueTooth.h"

@implementation CABlueTooth
@synthesize manager;
-(id)init
{
    if (![super init])
    {
        return nil;
    }
    manager = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
   // manager.delegate = self;
    return self;
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI
{
    if([peripheral.name  isEqualToString:@""]){
        [self connect:peripheral];
    }
    
}

-(BOOL)connect:(CBPeripheral *)peripheral{
    self.manager.delegate = self;
    [self.manager connectPeripheral:peripheral
                            options:[NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES] forKey:CBConnectPeripheralOptionNotifyOnDisconnectionKey]];
    return YES;
}

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
 
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error
{

}

- (void)centralManager:(CBCentralManager *)central willRestoreState:(NSDictionary *)dict
{
    
}

- (void)centralManager:(CBCentralManager *)central didRetrievePeripherals:(NSArray *)peripherals
{
 
}

-(void)beginScan
{
    [manager scanForPeripheralsWithServices:nil options:@{CBCentralManagerScanOptionAllowDuplicatesKey: @YES}];
}

-(int)getBlueToothState
{
    return [manager state];
}

@end
