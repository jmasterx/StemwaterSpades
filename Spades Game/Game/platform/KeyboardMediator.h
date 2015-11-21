//
//  KeyboardMediator.h
//  AllegroIOSTest
//
//  Created by Josh on 2/11/15.
//  Copyright (c) 2015 Allegro. All rights reserved.
//

#ifndef __AllegroIOSTest__KeyboardMediator__
#define __AllegroIOSTest__KeyboardMediator__

#include <stdio.h>
#include <Agui/TapListener.hpp>
#include <Agui/Gui.hpp>
#include "Game/UI/TextBox.hpp"
#include "Game/UI/TextField.hpp"
#include <queue>

namespace cge {
    class Scene;
}
class KeyboardMediator : public agui::TapListener
{
    cge::Scene* m_scene;
    std::queue<std::string> m_events;
    agui::Widget* m_tapped;
    cge::TextBox* m_txtOverlayBox;
    cge::TextField* m_txtOverlayField;
    std::string m_finalString;
    bool m_finalStringChanged;
    bool m_doneKeyPressed;
public:
    KeyboardMediator();
    virtual void widgetTapped(agui::Widget* widget, agui::MouseEvent& evt);
    void queueString(const std::string& str);
    void dequeue();
    void setScene(cge::Scene* scene);
    void keyboardDismissed();
    void setDonePressed();
    void hideKeyboard();
    void showKeyboard();
};
#endif /* defined(__AllegroIOSTest__KeyboardMediator__) */
