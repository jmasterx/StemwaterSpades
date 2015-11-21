//
//  KeyboardHandler.h
//  AllegroIOSTest
//
//  Created by Josh on 2/5/15.
//  Copyright (c) 2015 Allegro. All rights reserved.
//

#import <UIKit/UIKit.h>
class KeyboardMediator;
namespace agui {
    class Widget;
}
@interface KeyboardHandler : UIView <UIKeyInput>
-(void)show;
-(void)hide;
-(void)setKeyboardMediator:(KeyboardMediator*)keyboard;
-(void)fillText:(agui::Widget*)widget sender:(agui::Widget*)sender;
@end

