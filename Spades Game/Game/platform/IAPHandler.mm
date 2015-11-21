//
//  IAPHandler.cpp
//  AllegroIOSTest
//
//  Created by Josh on 2/27/15.
//  Copyright (c) 2015 Allegro. All rights reserved.
//

#include "Game/Platform/IAPHandler.h"
#import "RageIAPHelper.h"
#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>
#include "Game/Net/LobbyNetHandler.hpp"
#include "Game/Engine/SceneManager.hpp"
namespace cge {
    void IAPHandler::init() {
        m_bgTaskID = UIBackgroundTaskInvalid;
        [[RageIAPHelper sharedInstance] setHandler:this];
        [[RageIAPHelper sharedInstance] fetchProducts];
    }
    
    void IAPHandler::clear() {
        m_products.clear();
    }
    void IAPHandler::add(const IAPProduct& product) {
        m_products.push_back(product);
    }
    const std::vector<IAPProduct>& IAPHandler::getProducts() {
        return m_products;
    }
    
    void IAPHandler::buy(const std::string& pid) {
        [[RageIAPHelper sharedInstance] buy:[NSString stringWithCString:pid.c_str() encoding:[NSString defaultCStringEncoding]]];
    }
    
    void IAPHandler::setUsername(const std::string& name) {
        m_username = name;
    }
    const std::string& IAPHandler::getUsername() const {
        return m_username;
    }
    void IAPHandler::setLobbyNetHandler(LobbyNetHandler* handler) {
        m_net = handler;
    }
    void IAPHandler::signalCoinChange() {
        if(m_net)
        {
            m_net->requestCoinAmount(true);
        }
    }
    
    void IAPHandler::signalLogin() {
        [[RageIAPHelper sharedInstance] fetchProducts];
    }
    
    void IAPHandler::playTick() {
        AudioServicesPlaySystemSound (1104);
    }
    
    void IAPHandler::launchURL(const std::string& url) {
        NSString* str = [NSString stringWithCString:url.c_str() encoding:[NSString defaultCStringEncoding]];
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
    }
    void IAPHandler::startBackgroundExecution(cge::SceneManager* scene)
    {
        scene->backgroundExecutionStarted();
        m_bgTaskID = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:^{
            scene->backgroundExecutionEnded(true);
            [[UIApplication sharedApplication] endBackgroundTask: m_bgTaskID];
            m_bgTaskID = UIBackgroundTaskInvalid;
            NSLog(@"BG Exec forced to die");
        }];
        
        UIApplication *app = [UIApplication sharedApplication];
        double secondsToStayOpen = app.backgroundTimeRemaining;
        NSLog(@"secondsToStayOpen %f ",secondsToStayOpen);
    }
    
    void IAPHandler::endBackgroundExecution(cge::SceneManager* scene)
    {
        if(m_bgTaskID == UIBackgroundTaskInvalid)
            return;
        
        scene->backgroundExecutionEnded(false);
        [[UIApplication sharedApplication] endBackgroundTask: m_bgTaskID];
        m_bgTaskID = UIBackgroundTaskInvalid;
        
        NSLog(@"Background exec ended normally");
    }
}