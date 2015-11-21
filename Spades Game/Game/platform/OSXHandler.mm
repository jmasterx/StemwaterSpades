//
//  OSXHandler.m
//  A51Test
//
//  Created by Josh on 5/4/15.
//  Copyright (c) 2015 josh. All rights reserved.
//

#include "Game/platform/OSXHandler.h"
#include <CoreFoundation/CFBundle.h>
#include <ApplicationServices/ApplicationServices.h>
#import <AppKit/AppKit.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_osx.h>
#import <dispatch/dispatch.h>
namespace cge {
    void OSXHandler::launchURL(const std::string& url_str) {
        CFURLRef url = CFURLCreateWithBytes (
                                             NULL,                        // allocator
                                             (UInt8*)url_str.c_str(),     // URLBytes
                                             url_str.length(),            // length
                                             kCFStringEncodingASCII,      // encoding
                                             NULL                         // baseURL
                                             );
        LSOpenCFURLRef(url,0);
        CFRelease(url);
    }
    void OSXHandler::checkForDuplicates() {
        if ([[NSRunningApplication runningApplicationsWithBundleIdentifier:[[NSBundle mainBundle] bundleIdentifier]] count] > 1) {
            [[NSAlert alertWithMessageText:[NSString stringWithFormat:@"Another copy of %@ is already running.", [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleNameKey]]
                             defaultButton:nil alternateButton:nil otherButton:nil informativeTextWithFormat:@"This copy will now quit."] runModal];
            
            exit(0);
        }
    }
    
    void OSXHandler::setWindowTitle(ALLEGRO_DISPLAY* d, const std::string& caption)
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            NSWindow* w = al_osx_get_window(d);
            [w setTitle:@" Stemwater Spades "];
            //this runs on the main thread.  Use theData
        });

    }
}
