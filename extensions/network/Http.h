
#ifndef __Http_H__
#define __Http_H__

#import <Foundation/Foundation.h>
#import <Security/Security.h>

@interface Http : NSObject <NSURLConnectionDelegate, NSURLConnectionDataDelegate>
{
}
@property (strong) NSString *srcURL;
@property (strong) NSString *sslFile;
@property (copy) NSDictionary *responseHeader;
@property (strong) NSMutableData *responseData;
@property (readonly) NSInteger getDataTime;
@property (readonly) NSInteger responseCode;
@property (strong) NSString *statusString;
@property (strong) NSError *responseError;
@property (strong) NSError *connError;
@property (strong) NSURLConnection *conn;
@property bool finish;
@property (strong) NSRunLoop *runLoop;

-(void) startRequest:(NSURLRequest*)request;

@end

#endif
