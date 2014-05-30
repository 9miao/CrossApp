//
//  CALocation.m
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#import "CALocation.h"
#include "CALocationManager.h"
@implementation CALocation

-(id)init
{
    if (![super init])
    {
        return nil;
    }
    
    _locationManager = [[CLLocationManager alloc] init];
    
    _locationManager.delegate = self;
    
    _locationManager.desiredAccuracy = kCLLocationAccuracyBest;
    
    _locationManager.distanceFilter = 1000.0f;
    
    return self;
}
-(void)startUpdatingLocation
{
    [_locationManager startUpdatingLocation];
}
-(void)stopUpdatingLocation
{
    [_locationManager stopUpdatingLocation];
}
-(CGPoint)getLocation
{
    
    
}
#pragma mark Core Location


- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations

{
    
    CLLocation * currLocation = [locations lastObject];
    
    NSString *str1 = [NSString stringWithFormat:@"%3.5f",
                      
                      currLocation.coordinate.latitude];
    
    NSString *str2 = [NSString stringWithFormat:@"%3.5f",
                      
                      currLocation.coordinate.longitude];
    
    NSString *str3 = [NSString stringWithFormat:@"%3.5f",
                      
                      currLocation.altitude];
    

    CALocationManager *_locations = (CALocationManager *)self.location;
    
    if(_locations == NULL)
    {
        return ;
    }
    
    CCDictionary *dic = CCDictionary::create();
    
    CCString *_str = CCString::create([str1 cStringUsingEncoding:NSUTF8StringEncoding]);
    
    CCString *_str1 = CCString::create([str2 cStringUsingEncoding:NSUTF8StringEncoding]);
    
    CCString *_str2 = CCString::create([str3 cStringUsingEncoding:NSUTF8StringEncoding]);
    
    dic->setObject(_str, "latitude");
    dic->setObject(_str1, "longitude");
    dic->setObject(_str2, "altitude");

    _locations->getLocationDelegate()->getLocations(dic);
    [self stopUpdatingLocation];

}



- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error

{
    
    NSLog(@"error: %@",error);
    
}

@end
