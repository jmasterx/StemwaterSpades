//
//  KeyboardHandler.m
//  AllegroIOSTest
//
//  Created by Josh on 2/5/15.
//  Copyright (c) 2015 Allegro. All rights reserved.
//

#ifdef __OBJC__
#import <UIKit/UIKit.h>
#endif
#import "KeyboardHandler.h"
#include "KeyboardMediator.h"
#include <Agui/Widgets/TextField/TextField.hpp>
#include <Agui/Widgets/TextBox/TextBox.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_iphone_objc.h>
#include "Game/UI/TextField.hpp"
@interface KeyboardHandler()
{
    KeyboardMediator* m_kb;
    agui::Widget* m_widget;
    agui::Widget* m_sender;
}
@end
@implementation KeyboardHandler
-(id)init {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardDidHide:) name:UIKeyboardDidHideNotification object:nil];
    m_kb = NULL;
    m_sender = NULL;
    m_widget = NULL;
    self = [self initWithFrame:CGRectMake(-100, -100, 0, 0)];
    UIView* v = al_iphone_get_view(al_get_current_display());
    [v addSubview:self];
    return self;
}

- (UIKeyboardType) keyboardType
{
    return UIKeyboardTypeASCIICapable;
}

- (UIReturnKeyType) returnKeyType
{
    bool isSend = false;
    if(m_sender && m_sender->isTextField())
    {
        cge::TextField* t = (cge::TextField*)m_sender;
        
        if(t->wantsKeyboardNotification())
            isSend = true;
    }
    if(isSend)
        return UIReturnKeySend;
        else
    return UIReturnKeyDone;
}

- (UITextAutocorrectionType) autocorrectionType
{
    return UITextAutocorrectionTypeNo;
}

-(void)processWidget:(NSString*)data {
    std::string txt([data UTF8String]);
    if(m_widget)
    {
        m_widget->setText("foo");
        m_widget = NULL;
    }
}

- (void)insertText:(NSString *)text
{
    if([text isEqualToString:@"\n"])
    {
        [self hide];
        m_kb->setDonePressed();
    }
    else {
        std::string str = [text UTF8String];
        if(m_widget && str.length() == 1)
        {
            m_kb->queueString(str);
        }
    }
}

-(BOOL)canBecomeFirstResponder {
    return YES;
}

- (void)deleteBackward {
    if(m_widget)
    {
        std::string back = " ";
        back[0] = 127;
        m_kb->queueString(back);
    }
}

- (BOOL)hasText {
    return YES;
}


-(void)show {
 [self performSelectorOnMainThread:@selector(becomeFirstResponder) withObject:nil waitUntilDone:YES];
}
-(void)hide {
    [self performSelectorOnMainThread:@selector(resignFirstResponder) withObject:nil waitUntilDone:YES];
}
- (void)keyboardDidHide:(NSNotification *)notification {
    m_kb->keyboardDismissed();
    m_widget = NULL;
    m_sender = NULL;
}
-(void)setKeyboardMediator:(KeyboardMediator*)keyboard
{
    m_kb = keyboard;
}
-(void)fillText:(agui::Widget*)widget sender:(agui::Widget*)sender;
{
    
    if(widget->isTextComponent() && widget->canAcceptKeyInput())
    {
        int len = -1;
        if(widget->isTextField())
        {
            agui::TextField* t = (agui::TextField*)widget;
            len = t->getMaxLength();
        }
        else
        {
            agui::TextBox* t = (agui::TextBox*)widget;
            len = t->getMaxLength();
        }
        
        m_widget = widget;
        m_sender = sender;

        [self show];
    }
}
-(void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardDidHideNotification object:nil];
}
@end