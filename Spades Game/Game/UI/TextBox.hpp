#ifndef CGE_TEXTBOX_HPP
#define CGE_TEXTBOX_HPP
#include "Agui/Widgets/TextBox/ExtendedTextBox.hpp"
#include "Game/UI/PopUpMenu.hpp"
#include "Agui/ActionListener.hpp"
#include "Game/Engine/LanguageManager.hpp"
namespace cge
{
	enum NameParseResultEnum
	{
		NAME_PARSED_NOTHING,
		NAME_PARSED_NAME,
		NAME_PARSED_NAME_AND_TABLE

	};

	enum CursorOnSomethingEnum
	{
		CURSOR_ON_NOTHING,
		CURSOR_ON_NAME,
		CURSOR_ON_TABLE
	};

	enum TextCommandEnum
	{
		TC_GO_TO_TABLE,
		TC_VIEW_USER_PROFILE,
		TC_SHOW_USER_CONTEXT
	};
	class TextBox : public agui::ExtendedTextBox, agui::ActionListener
	{
		agui::VScrollBar* m_vscroll;
		agui::HScrollBar* m_hscroll;
		agui::Widget* m_scrollInset;
		agui::Image* m_bgImage;
		agui::Image* m_focusImage;
		PopUpMenu* m_menu;
		agui::PopUpMenuItem m_cutItem;
		agui::PopUpMenuItem m_copyItem;
		agui::PopUpMenuItem m_pasteItem;
		agui::PopUpMenuItem m_deleteItem;
		agui::PopUpMenuItem m_selectAllItem;
		agui::PopUpMenuItem m_separatorItem;
		bool m_asLabel;
		bool m_keepingTableNumbers;
		bool m_keepingNames;
		int m_lastTableNum;
		std::string m_lastUserName;
		CursorOnSomethingEnum m_cursorObject;
		int lastMouseX;
		int lastMouseY;

		agui::Font* m_isTypingFont;
		agui::Image* m_isTypingImg;
		agui::Color m_isTypingColor;
		bool m_isTyping;
		int m_isTypingTimeout;
		int m_isTypingCounter;
		std::string m_isTypingText;

		int m_pencilAnimCounter;
		int m_pencilX;
		int m_pencilY;
		int m_elipsisCounter;
		std::string m_fullPencil;
        
        std::string m_fieldName;
	public:
		TextBox(	
		agui::VScrollBar* vscroll,
		agui::HScrollBar* hscroll,
		agui::Widget* scrollInset,
		agui::Image* bgImage,
		agui::Image* focusImage,
		PopUpMenu* menu, LanguageManager* langMan);
		NameParseResultEnum findLine(int index, int y);
		bool isKeepingTableNumbers() const;
		void setKeepingTableNumbers(bool keeping);
		bool isKeepingNames() const;
		void setKeepingNames(bool keeping);
		void generateAction(TextCommandEnum c);
		virtual void mouseClick(agui::MouseEvent &mouseEvent);
		virtual void mouseMove(agui::MouseEvent &mouseEvent);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual bool intersectionWithPoint(const agui::Point &p) const;
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual int getLineHeight() const;
		void setAsLabel(bool asLabel);
		virtual void mouseLeave(agui::MouseEvent &mouseEvent);
		virtual void mouseDrag(agui::MouseEvent &mouseEvent);
		virtual void valueChanged(agui::VScrollBar* source, int val);
		virtual void setText(const std::string &text);
		int getLastMouseX() const;
		int getLastMouseY() const;
		int getTable() const;
		const std::string& getName() const;

		void setIsTypingFont(agui::Font* f);
		agui::Font* getIsTypingFont() const;
		void setIsTypingImage(agui::Image* img);
		agui::Image* getIsTypingImage() const;
		void setIsTypingColor(const agui::Color& color);
		const agui::Color& getIsTypingColor() const;
		void activateIsTyping();
		void setIsTypingTimeout(int timeout);
		int getIsTypingTimeout() const;
		void setIsTypingText(const std::string& text);
		const std::string& getIsTypingText() const;
		void setPaddingForIsTyping();
		void haltIsTyping();
		virtual void logic(double time);
        void setFieldName(const std::string& name);
        const std::string& getFieldName() const;
		virtual ~TextBox(void);
	};
}

#endif
