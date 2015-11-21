#ifndef CGE_MESSAGE_BOX_HPP
#define CGE_MESSAGE_BOX_HPP
#include "Game/UI/Button.hpp"
#include "Game/UI/Label.hpp"
#include <Agui/EmptyWidget.hpp>
#include "Game/UI/FlowLayout.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Game/UI/Frame.hpp"
#include "Game/Engine/LanguageManager.hpp"
namespace cge
{
	class MessageBoxResultReceiver;
	class MessageBox : public agui::EmptyWidget, public agui::ActionListener
	{
	public:
		enum MessageType
		{
			MT_YES_NO,
			MT_YES_NO_CANCEL,
			MT_OK_CANCEL,
			MT_OK
		};
		enum MessageResult
		{
			MR_YES,
			MR_NO,
			MR_OK,
			MR_CANCEL
		};
	private:
		FlowLayout* m_buttonLayout;
		Button* m_btnYes;
		Button* m_btnNo;
		Button* m_btnCancel;
		Button* m_btnOk;
		Label* m_textLabel;
		Label* m_titleLabel;
		GuiFontManager* m_font;
		LanguageManager* m_language;
		float m_fontRatio;
		agui::Image* m_bolt;
		agui::Image* m_bg;
		Widget* m_container;
		int m_tag;
		MessageBoxResultReceiver* m_receiver;

		int m_lrpadding;
		int m_tbSpacing;
		int m_minMsgHeight;
		int m_minMsgBoxWidth;
		int m_extraTitlePad;
		MessageType m_type;
		FlowLayout* m_containLayout;
		int getHeightFromWidth(int width);
	public:
		virtual void actionPerformed(const agui::ActionEvent& evt);
		void setFontRatio(float ratio);
		void resize(Frame* frame);
		MessageBox(
		FlowLayout* buttonLayout,
		FlowLayout* containLayout,
		Button* btn1,
		Button* btn2,
		Button* btn3,
		Button* btn4,
		Label* textLabel,
		Label* titleLabel,
		GuiFontManager* font,
		LanguageManager* language,
		agui::Image* bolt, agui::Image* bg);
		void showMessage(const std::string& title, const std::string& message, Frame* frame, MessageType type, MessageBoxResultReceiver* rec, int tag);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		void setContainer(Widget* container);
		void hideMessage();
		int getTag() const;
		agui::FlowLayout* getModalContainer();
		virtual ~MessageBox(void);
	};
}

#endif
