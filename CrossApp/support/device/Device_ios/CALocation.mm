//
//  CALocation.m
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#import "CALocation.h"
#include "../CADevice.h"
@implementation CALocation

static CALocation* m_pLocation = NULL;

-(id)init
{
    if (![super init])
    {
        return nil;
    }

    _sender = NULL;
    
    _locationManager = [[CLLocationManager alloc] init];
    
    _locationManager.delegate = self;
    
    _locationManager.desiredAccuracy = kCLLocationAccuracyBest;
    
    _locationManager.distanceFilter = kCLDistanceFilterNone;
    
    [_locationManager requestAlwaysAuthorization];
    
    return self;
}

+ (CALocation *)sharedLocation
{
    if( m_pLocation == NULL )
        m_pLocation = [[CALocation alloc] init];
    
    return m_pLocation;
}

-(void)startUpdatingLocation
{
    [_locationManager startUpdatingLocation];
}

-(void)stopUpdatingLocation
{
    [_locationManager stopUpdatingLocation];
}

-(void)setDesiredAccuracy
{
    [_locationManager setDesiredAccuracy:kCLLocationAccuracyBest];
}
#pragma mark Core Location

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations

{
    CLLocation *curLocation = [locations lastObject];

    
    if (CrossApp::CALocationDelegate *locationDelegate = (CrossApp::CALocationDelegate*)(self.sender))
    {
        
        NSString *longitude = [[NSString alloc] initWithFormat:@"%g",curLocation.coordinate.longitude];
        
        NSString *latitude = [[NSString alloc] initWithFormat:@"%g",curLocation.coordinate.latitude];
        
        NSString *altitude = [[NSString alloc] initWithFormat:@"%g",curLocation.verticalAccuracy];
        
        NSString *speed = [[NSString alloc] initWithFormat:@"%g",curLocation.speed];
        
        NSString* bearing = [[NSString alloc] initWithFormat:@"%g",curLocation.course];
        
        
        CrossApp::CALocationInfo info;
        info.sLongitude = [longitude UTF8String];
        info.sLatitude = [latitude UTF8String];
        info.sAltitude = [altitude UTF8String];
        info.sSpeed = [speed UTF8String];
        info.sBearing = [bearing UTF8String];
        
        locationDelegate->getLocationChanged(info);
    }
}

-(void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error{
    if ([error code]==kCLErrorDenied) {
        NSLog(@"访问被拒绝");
    }
    if ([error code]==kCLErrorLocationUnknown) {
        NSLog(@"无法获取位置信息");
    }
}

@end
