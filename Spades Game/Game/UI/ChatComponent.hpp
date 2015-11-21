#ifndef CGE_CHAT_COMPONENT_HPP
#define CGE_CHAT_COMPONENT_HPP
#include "Game/UI/GuiFactory.hpp"
#include "Agui/ActionListener.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
namespace cge
{
	struct RequestMessage {
		int tableNo;
		int startPos;
		int endPos;

		RequestMessage() :
		tableNo(-1),startPos(0),
			endPos(0)
		{

		}
	};

	class ChatComponent : public agui::ActionListener, public LobbyEventProvider
	{
		BorderLayout* m_mainLayout;
		BorderLayout* m_entryLayout;
		Button* m_sendButton;
		TextField* m_textEntry;
		TextBox* m_chat;
		ChatAreaEnum m_area;
		int m_table;
		bool m_appendTable;
		bool m_notifyFlag;
	public:
		ChatComponent(GuiFactory* factory, LanguageManager* language, ChatAreaEnum area);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void chatReceiveMessage(const std::string& message, 
			const std::string& playerName, 
			ChatAreaEnum chatArea, const agui::Color& nameColor);
		BorderLayout* getWidget();
		TextField* getEntryField();
		BorderLayout* getEntryLayout();
		TextBox* getChatBox();
		void setTable(int table);
		void setAppendTable(bool append);
		bool isAppendingTable() const;
		void gotTextEvent();
		bool notifyFlagSet() const;
		int getEntryHeight() const;
		std::string appendEmoticonsTo(const std::string& msg);
		RequestMessage receiveRequest(const std::string& player, const std::string& requestText, const std::string& tinfoText, const agui::Color& tableColor, const agui::Color& playerColor);
		void grayOutRequest(const RequestMessage& r);
		virtual ~ChatComponent(void);
	};
}

#endif