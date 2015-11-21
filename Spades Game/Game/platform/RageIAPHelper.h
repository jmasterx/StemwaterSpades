//
//  RageIAPHelper.h
//  In App Rage
//
//  Created by Ray Wenderlich on 9/5/12.
//  Copyright (c) 2012 Razeware LLC. All rights reserved.
//

#import "IAPHelper.h"
namespace cge {
    class IAPHandler;
}
@interface RageIAPHelper : IAPHelper

+ (RageIAPHelper *)sharedInstance;
-(void)setHandler:(cge::IAPHandler*)handler;
-(void)fetchProducts;
-(void)buy:(NSString*)product;
@end
