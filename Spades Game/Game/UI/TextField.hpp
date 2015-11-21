#ifndef CGE_TEXTFIELD_HPP
#define CGE_TEXTFIELD_HPP
#include "Agui/Widgets/TextField/TextField.hpp"
#include "Game/UI/PopUpMenu.hpp"
#include "Game/Engine/LanguageManager.hpp"
#include "Agui/ActionListener.hpp"
namespace cge
{
	class TextField : public agui::TextField, agui::ActionListener
	{
		agui::Image* m_bgImage;
		agui::Image* m_focusImage;
		PopUpMenu* m_menu;
		agui::PopUpMenuItem m_cutItem;
		agui::PopUpMenuItem m_copyItem;
		agui::PopUpMenuItem m_pasteItem;
		agui::PopUpMenuItem m_deleteItem;
		agui::PopUpMenuItem m_selectAllItem;
		agui::PopUpMenuItem m_separatorItem;
        std::string m_fieldName;
        bool m_needsEmoticons;
        bool m_wantKeyboardNotification;
	public:
		TextField(	agui::Image* bgImage,
		agui::Image* focusImage,
		PopUpMenu* menu, LanguageManager* lang);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void mouseDown(agui::MouseEvent &mouseEvent);
		virtual ~TextField(void);
        void setFieldName(const std::string& name);
        const std::string& getFieldName() const;
        void setNeedsEmoticons(bool needs);
        bool needsEmoticons() const;
        void setWantKeyboardNotification(bool needs);
        bool wantsKeyboardNotification() const;
        void keyboardNotification();
    };
}

#endif
