//
//  KeyboardMediator.cpp
//  AllegroIOSTest
//
//  Created by Josh on 2/11/15.
//  Copyright (c) 2015 Allegro. All rights reserved.
//

#include "KeyboardMediator.h"
#import "KeyboardHandler.h"
#include "Game/Engine/Scene.hpp"
#include <Agui/Widgets/TextField/TextField.hpp>
#include "Game/UI/TextField.hpp"
static KeyboardHandler* m_handler;
KeyboardMediator::KeyboardMediator()
: m_scene(NULL),m_tapped(NULL),m_txtOverlayBox(NULL),
m_txtOverlayField(NULL),m_finalStringChanged(false),
m_doneKeyPressed(false)
{
    m_handler = [KeyboardHandler new];
    [m_handler setKeyboardMediator:this];
}

void KeyboardMediator::widgetTapped(agui::Widget* widget, agui::MouseEvent& evt)
{
    if(widget->isTextComponent() && widget->canAcceptKeyInput())
    {
        m_doneKeyPressed  = false;
        m_txtOverlayField->setFocusable(false);
        m_tapped = widget;
        bool textField = widget->isTextField();
        std::string fieldName;
        m_txtOverlayField->setIsPassword(false);
        if(textField)
        {
            cge::TextField* tf = (cge::TextField*)m_tapped;
            if(tf->isPassword())
            {
                m_txtOverlayField->setIsPassword(true);
                m_txtOverlayField->setText(tf->getPassword());
            }
            else
            {
                m_txtOverlayField->setText(tf->getText());
            }
            m_txtOverlayField->setMaxLength(tf->getMaxLength());
            fieldName = tf->getFieldName();
            
            if(tf->needsEmoticons())
            {
                m_scene->toggleOverlayEmoticonsContainer(true);
            }
            else
            {
                m_scene->toggleOverlayEmoticonsContainer(false);
            }
        }
        else
        {
            cge::TextBox* tb = (cge::TextBox*)m_tapped;
            m_txtOverlayBox->setText(tb->getText());
            m_txtOverlayBox->setMaxLength(tb->getMaxLength());
            fieldName = tb->getFieldName();
            m_scene->toggleOverlayEmoticonsContainer(false);
        }
        
        m_scene->setMobileKeyboardMode(true, textField, false,fieldName);
        if(textField)
            [m_handler fillText:m_txtOverlayField sender:widget];
        else
            [m_handler fillText:m_txtOverlayBox sender:widget];
    }

}
void KeyboardMediator::queueString(const std::string& str) {
    m_events.push(str);
}

void KeyboardMediator::keyboardDismissed() {
    if(!m_scene)
        return;
    m_scene->setMobileKeyboardMode(false, false, false,"");
    if(m_tapped)
    {
        if(m_tapped->isTextField())
        {
            agui::TextField* tf = (agui::TextField*)m_tapped;
            
            if(tf->isPassword())
                m_finalString = m_txtOverlayField->getPassword();
            else
                m_finalString = m_txtOverlayField->getText();
        }
        else
        {
            m_finalString = m_txtOverlayBox->getText();
        }
        
        m_finalStringChanged = true;
    }
}

void KeyboardMediator::dequeue() {
    while(!m_events.empty())
    {
        if(m_tapped->isTextField())
        {
            agui::TextField* tf = (agui::TextField*)m_txtOverlayField;
            
            if(m_events.front().length() > 0 && m_events.front()[0] == 127)
                tf->removeLastCharacter();
            else {
                if(!tf->isPassword())
                tf->appendText(m_events.front());
                else
                tf->setText(tf->getPassword() + m_events.front());
            }
        }
        else
        {
            agui::TextBox* tf = (agui::TextBox*)m_txtOverlayBox;
            
            if(m_events.front().length() > 0 && m_events.front()[0] == 127) {
                tf->removeLastCharacter();
            }
            else
                tf->appendText(m_events.front());
        }
        m_events.pop();
    }
    
    if(m_finalStringChanged)
    {
        m_finalStringChanged = false;
        m_tapped->setText(m_finalString);
        m_finalString = "";
        
        if(m_tapped->isTextField() && m_doneKeyPressed)
        {
            cge::TextField* tf = (cge::TextField*)m_tapped;
            
            if(tf->wantsKeyboardNotification()) {
                tf->keyboardNotification();
            }
        }
        m_doneKeyPressed = false;
    }

}

void KeyboardMediator::setDonePressed() {
    m_doneKeyPressed = true;
}

void KeyboardMediator::hideKeyboard() {
    if(m_handler)
    {
        [m_handler hide];
    }
}

void KeyboardMediator::setScene(cge::Scene* scene) {
    m_scene = scene;
    if(m_scene)
    {
        m_txtOverlayField = scene->getOverlayTextField();
        m_txtOverlayBox = scene->getOverlayTextBox();
    }
    else
    {
        m_txtOverlayBox = NULL;
        m_txtOverlayField = NULL;
        m_tapped = NULL;
    }
}

void KeyboardMediator::showKeyboard()
{
    [m_handler show];
}
