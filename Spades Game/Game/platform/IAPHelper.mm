//
//  IAPHelper.m
//  In App Rage
//
//  Created by Ray Wenderlich on 9/5/12.
//  Copyright (c) 2012 Razeware LLC. All rights reserved.
//

// 1
#import "IAPHelper.h"
#import <StoreKit/StoreKit.h>
#import <AFNetworking.h>
#include "Game/Platform/IAPHandler.h"
#include "Game/platform.hpp"
//#import "VerificationController.h"

NSString *const IAPHelperProductPurchasedNotification = @"IAPHelperProductPurchasedNotification";

// 2
@interface IAPHelper () <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    cge::IAPHandler* m_handler;
}
@end

// 3
@implementation IAPHelper {
    SKProductsRequest * _productsRequest;
    RequestProductsCompletionHandler _completionHandler;
    
    NSSet * _productIdentifiers;
    NSMutableSet * _purchasedProductIdentifiers;
}

- (id)initWithProductIdentifiers:(NSSet *)productIdentifiers {
    
    if ((self = [super init])) {
        
        // Store product identifiers
        _productIdentifiers = productIdentifiers;
        
        // Add self as transaction observer
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        [self processStoredTransactions];
    }
    return self;
    
}

- (void)requestProductsWithCompletionHandler:(RequestProductsCompletionHandler)completionHandler {
    
    
    // 1
    _completionHandler = [completionHandler copy];
    
    // 2
    _productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:_productIdentifiers];
    _productsRequest.delegate = self;
    [_productsRequest start];
    
}

- (BOOL)productPurchased:(NSString *)productIdentifier {
    return [_purchasedProductIdentifiers containsObject:productIdentifier];
}

- (void)buyProduct:(SKProduct *)product {
    
    NSLog(@"Buying %@...", product.productIdentifier);
    
    SKPayment * payment = [SKPayment paymentWithProduct:product];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
    
}

- (void)validateReceiptForTransaction:(SKPaymentTransaction *)transaction {
    /*
    VerificationController * verifier = [VerificationController sharedInstance];
    [verifier verifyPurchase:transaction completionHandler:^(BOOL success) {
        if (success) {
            NSLog(@"Successfully verified receipt!");
            [self provideContentForProductIdentifier:transaction.payment.productIdentifier];
        } else {
            NSLog(@"Failed to validate receipt.");
            [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
        }
    }];
     */
}

#pragma mark - SKProductsRequestDelegate

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    
    NSLog(@"Loaded list of products...");
    _productsRequest = nil;
    
    NSArray * skProducts = response.products;
    for (SKProduct * skProduct in skProducts) {
        NSLog(@"Found product: %@ %@ %0.2f",
              skProduct.productIdentifier,
              skProduct.localizedTitle,
              skProduct.price.floatValue);
    }
    
    _completionHandler(YES, skProducts);
    _completionHandler = nil;
    
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error {
    
    NSLog(@"Failed to load list of products.");
    _productsRequest = nil;
    
    _completionHandler(NO, nil);
    _completionHandler = nil;
}

#pragma mark SKPaymentTransactionOBserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction * transaction in transactions) {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
            default:
                break;
        }
    };
}

-(NSMutableDictionary*)userDictionary {
    return [self dict:@"userDict"];
}

-(NSMutableDictionary*)transDictionary {
    return [self dict:@"transDict"];
}

-(NSMutableDictionary*)dict:(NSString*)name {
    /// Read
    NSData* data = [[NSUserDefaults standardUserDefaults]objectForKey:name];
    if(data == nil)
        return [[NSMutableDictionary alloc]init];
    NSMutableDictionary* object = [NSKeyedUnarchiver unarchiveObjectWithData:data];
    
    if(object == nil)
        return [[NSMutableDictionary alloc] init];
    return object;
}

-(BOOL)transactionExists:(NSString*)transaction {
    NSMutableDictionary* d = [self userDictionary];
    return [d objectForKey:transaction] != nil;
}

-(void)saveDict:(NSString*)dictName withDict:(NSMutableDictionary*)object{
    [[NSUserDefaults standardUserDefaults] setObject:[NSKeyedArchiver archivedDataWithRootObject:object] forKey: dictName];
}

-(void)saveTransDict:(NSMutableDictionary*)dict {
    [self saveDict:@"transDict" withDict:dict];
}

-(void)saveUserDict:(NSMutableDictionary*)dict {
    [self saveDict:@"userDict" withDict:dict];
}

-(void)addDictEntry:(NSString*)username withTransaction:(NSString*)transaction transNo:(NSString*)transNo {
    NSMutableDictionary* userDict = [self userDictionary];
    NSMutableDictionary* transDict = [self transDictionary];
    [userDict setValue:username forKey:transNo];
    [transDict setValue:transaction forKey:transNo];
    [self saveUserDict:userDict];
    [self saveTransDict:transDict];
}

-(void)removeDictEntry:(NSString*)transNo {
    NSMutableDictionary* userDict = [self userDictionary];
    NSMutableDictionary* transDict = [self transDictionary];
    [userDict removeObjectForKey:transNo];
    [transDict removeObjectForKey:transNo];
    [self saveUserDict:userDict];
    [self saveTransDict:transDict];
}

-(void)setBaseHandler:(cge::IAPHandler*)handler {
    m_handler = handler;
}

-(void)processStoredTransactions {
    NSMutableDictionary* dict = [self userDictionary];
    NSMutableDictionary* tdict = [self transDictionary];
    
    for(id key in dict) {
        NSString* transNo = key;
        NSString* user = [dict objectForKey:key];
        NSString* trans = [tdict objectForKey:key];
        [self validateViaOwnServer:trans withUsername:user transNo:transNo];
    }
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"completeTransaction...");
    //[self validateReceiptForTransaction:transaction];
    //[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
    
    NSData *receipt = [NSData dataWithContentsOfURL:receiptURL];
    
    NSString *jsonObjectString = [receipt base64EncodedStringWithOptions:0];
    
    NSString* user = [NSString stringWithCString:m_handler->getUsername().c_str() encoding:[NSString defaultCStringEncoding]];
    //TODO: no user?
    [self addDictEntry:user withTransaction:jsonObjectString transNo:transaction.transactionIdentifier];
    [self validateViaOwnServer:jsonObjectString withUsername:user transNo:transaction.transactionIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

-(void)validateViaOwnServer  :(NSString*)receipt withUsername:(NSString*)user transNo:(NSString*)trans
{
    NSString* BASEURl = @CGE_APP_WEB_ADDRESS;
    NSString *TestReceipt= [BASEURl stringByAppendingString:@"/nonayobiz.html"];
    
    
    NSDictionary *params = @ {@"receipt" :receipt, @"username" :user};
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    manager.responseSerializer.acceptableContentTypes = [NSSet setWithObject:@"text/html"];
    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    
    [manager POST:TestReceipt
     parameters:params
     success:^(AFHTTPRequestOperation *operation, id responseData) {
         
         NSLog(@"Response");
         m_handler->signalCoinChange();
         [self removeDictEntry:trans];
         
     } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
         
         NSLog(@"No Response");
     }];
    
    NSLog(@"End Response");
}
- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"restoreTransaction...");
    
    [self validateReceiptForTransaction:transaction];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {
    
    NSLog(@"failedTransaction...");
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
        NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
    }
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)provideContentForProductIdentifier:(NSString *)productIdentifier {
    [[NSNotificationCenter defaultCenter] postNotificationName:IAPHelperProductPurchasedNotification object:productIdentifier userInfo:nil];
    
}

- (void)restoreCompletedTransactions {
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

@end