//
//  RageIAPHelper.m
//  In App Rage
//
//  Created by Ray Wenderlich on 9/5/12.
//  Copyright (c) 2012 Razeware LLC. All rights reserved.
//

#import "RageIAPHelper.h"
#include "Game/Platform/IAPHandler.h"
@interface RageIAPHelper()
{
    cge::IAPHandler* m_handler;
}
@property NSArray* products;
@end
@implementation RageIAPHelper

+ (RageIAPHelper *)sharedInstance {
    static dispatch_once_t once;
    static RageIAPHelper * sharedInstance;
    dispatch_once(&once, ^{
        NSSet * productIdentifiers = [NSSet setWithObjects:
                                      @"com.stemwatergames.99cent",
                                      @"com.stemwatergames.499cent",
                                      @"com.stemwatergames.999cent",
                                      @"com.stemwatergames.1999cent",
                                      @"com.stemwatergames.4999cent",
                                      @"com.stemwatergames.7999cent",
                                      nil];
        sharedInstance = [[self alloc] initWithProductIdentifiers:productIdentifiers];
    });
    return sharedInstance;
}

-(void)setHandler:(cge::IAPHandler*)handler {
    m_handler = handler;
    [self setBaseHandler:handler];
}

-(void)fetchProducts {
    if(self.products != nil && [self.products count] > 0)
        return;
        
    [self requestProductsWithCompletionHandler:^(BOOL success, NSArray *products) {
        self.products = nil;
        if (success) {
            NSArray* pproducts = [products sortedArrayUsingComparator:^(id a, id b) {
                NSDecimalNumber *first = [(SKProduct*)a price];
                NSDecimalNumber *second = [(SKProduct*)b price];
                return [first compare:second];
            }];
            self.products = pproducts;
            m_handler->clear();
            for(SKProduct* product : pproducts)
            {
                IAPProduct p;
                if(product != nil && product.localizedTitle != nil && product.localizedDescription != nil)
                {
                    p.name = std::string([product.localizedTitle UTF8String]);
                    p.description = std::string([product.localizedDescription UTF8String]);
                    p.price = std::string([[product.price stringValue] UTF8String]);
                    p.priceLocale = std::string([[product.priceLocale objectForKey:NSLocaleCurrencyCode]  UTF8String]);
                    p.productID = std::string([product.productIdentifier UTF8String]);
                    p.priceSymbol = std::string([[product.priceLocale objectForKey:NSLocaleCurrencySymbol]  UTF8String]);
                    if(p.productID == "com.stemwatergames.999cent")
                        p.bonusPercent = "18";
                    else if(p.productID == "com.stemwatergames.1999cent")
                        p.bonusPercent = "48";
                    else if(p.productID == "com.stemwatergames.4999cent")
                        p.bonusPercent = "98";
                    else if(p.productID == "com.stemwatergames.7999cent")
                        p.bonusPercent = "147";
                    m_handler->add(p);
                }
            }
        }
    }];
}

-(void)buy:(NSString*)product {
    SKProduct* prod = nil;
    
    NSArray* pr = self.products;
    
    for(SKProduct* p : pr)
    {
        if([p.productIdentifier isEqualToString:product]) {
            prod = p;
            break;
        }
    }
   
    if(prod != nil) {
    [self buyProduct:prod];
    }
}

@end
