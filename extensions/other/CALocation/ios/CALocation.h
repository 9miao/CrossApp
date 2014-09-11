//
//  CALocation.h
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <CoreLocation/CLLocationManagerDelegate.h>




@interface CALocation : NSObject<CLLocationManagerDelegate>
{
    CLLocationManager *_locationManager;
    
}

@property(assign,nonatomic)void* location;
-(void)startUpdatingLocation;
-(void)stopUpdatingLocation;
@end
