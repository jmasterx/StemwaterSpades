#include "Game/UI/ChatComponent.hpp"
#include <sstream>
#include "Game/Engine/ListPairLoader.hpp"
#include "Game/Utility/StringUtil.hpp"
namespace cge
{
	ChatComponent::~ChatComponent(void)
	{
		m_chat->removeActionListener(this);
	}

	ChatComponent::ChatComponent( 
		GuiFactory* factory, LanguageManager* language, ChatAreaEnum area )
		: m_area(area),m_appendTable(false),m_table(-1),m_notifyFlag(false)
	{
		m_mainLayout = factory->createBorderLayout();
		m_entryLayout = factory->createBorderLayout();
		m_sendButton = factory->createButton();
		m_textEntry = factory->createTextField();
		m_chat = factory->createTextBox();

		m_mainLayout->setResizable(false);
		m_entryLayout->setResizable(false);
		m_entryLayout->setMargins(1,1,0,1);

		int height = 0;
        if(Platf::isRetina())
        {
            m_sendButton->setFont(factory->getFontManager()->getFont(2.0f));
            m_textEntry->setFont(factory->getFontManager()->getFont(2.0f));
        }
        m_textEntry->setNeedsEmoticons(true);
		m_sendButton->setText(language->getElement("chat.send"));
		m_sendButton->resizeToContents();
		height = m_sendButton->getHeight();
        if(Platf::isRetina())
        {
            m_sendButton->setFont(factory->getFontManager()->getFont(2.0f * 0.8f));
        }
        else
        {
            m_sendButton->setFont(factory->getFontManager()->getFont(0.8f));
        }
		
		m_sendButton->resizeToContents();
		m_sendButton->setSize(m_sendButton->getWidth() + 12,
			height + 7);
		m_entryLayout->setSize(0,m_sendButton->getHeight());

		m_sendButton->addActionListener(this);
		m_textEntry->addActionListener(this);

		m_entryLayout->add(m_textEntry,agui::BorderLayout::CENTER);
#ifndef CGE_MOBILE
		m_entryLayout->add(m_sendButton,agui::BorderLayout::EAST);
#else
        m_chat->setVScrollPolicy(agui::SHOW_NEVER);
#endif

		m_mainLayout->setSize(500,500); //make it happy
		m_mainLayout->add(m_chat,agui::BorderLayout::CENTER);
		m_mainLayout->add(m_entryLayout,agui::BorderLayout::SOUTH);

		m_textEntry->setMaxLength(200);

		m_chat->setReadOnly(true);
		m_chat->setWordWrap(true);
		m_chat->addActionListener(this);

		m_chat->setKeepingNames(true);
		m_chat->setKeepingTableNumbers(true);

		for(size_t i = 0; i < ListPairLoader::getStrings().size(); ++i)
		{
			ListPairItem item = ListPairLoader::getStrings()[i];
			m_chat->registerEmoticon(item.val,item.image,item.descr);
		}

        m_textEntry->setWantKeyboardNotification(true);
	}

	void ChatComponent::actionPerformed( const agui::ActionEvent &evt )
	{
		if((evt.getSource() == m_sendButton || evt.getSource() == m_textEntry) &&
			m_textEntry->getTextLength() > 0)
		{
			DISPATCH_LOBBY_EVENT
			{
				(*it)->chatSendMessage(appendEmoticonsTo(m_textEntry->getText()),m_area);
			}

			m_textEntry->setText("");
			m_textEntry->focus();
		}

		if(evt.getSource() == m_chat)
		{
			m_notifyFlag = true;
			std::stringstream ss;
			int msg = -1;
			ss << evt.getId();
			ss >> msg;
			TextCommandEnum tc = (TextCommandEnum)msg;
			
			switch(tc)
			{
			case TC_GO_TO_TABLE:
				DISPATCH_LOBBY_EVENT
					(*it)->focusOnLobbyTable(m_chat->getTable(),true);
				break;
			case TC_SHOW_USER_CONTEXT:
				DISPATCH_LOBBY_EVENT
					(*it)->showPlayerContext(m_chat->getName(),m_chat->getLastMouseX(),
					m_chat->getLastMouseY(),CHAT_RCLICK,m_chat);
				break;
			case TC_VIEW_USER_PROFILE:
				DISPATCH_LOBBY_EVENT
					(*it)->requestShowProfile(m_chat->getName());
				break;
			}
			
		}
		
	}

	void ChatComponent::chatReceiveMessage( 
		const std::string& message, const std::string& playerName,
		ChatAreaEnum chatArea, const agui::Color& nameColor )
	{
		bool reposCaret = true;

#ifdef CGE_MOBILE

		reposCaret = false;
#endif

		if(chatArea == m_area)
		{
			m_chat->setCurrentColor(nameColor);
			if(m_chat->getTextLength() != 0)
			{
				m_chat->appendText("\n",false,reposCaret);
			}
			if(isAppendingTable() && m_table != -1)
			{
				std::stringstream ss;
				ss << "(";
				ss << m_table;
				ss << ") ";
				m_chat->appendText(ss.str(),false,reposCaret);
			}

			m_chat->appendText(playerName + ": ",false,reposCaret);
			m_chat->setCurrentColor(m_chat->getFontColor());
			m_chat->appendText(message,false,reposCaret);
		}
	}

	BorderLayout* ChatComponent::getWidget()
	{
		return m_mainLayout;
	}

	TextField* ChatComponent::getEntryField()
	{
		return m_textEntry;
	}

	TextBox* ChatComponent::getChatBox()
	{
		return m_chat;
	}

	BorderLayout* ChatComponent::getEntryLayout()
	{
		return m_entryLayout;
	}

	void ChatComponent::setTable( int table )
	{
		m_table = table;
	}

	void ChatComponent::setAppendTable( bool append )
	{
		m_appendTable = append;
	}

	bool ChatComponent::isAppendingTable() const
	{
		return m_appendTable;
	}

	void ChatComponent::gotTextEvent()
	{
		m_notifyFlag = false;
	}

	bool ChatComponent::notifyFlagSet() const
	{
		return m_notifyFlag;
	}

	std::string ChatComponent::appendEmoticonsTo( const std::string& msg )
	{
		std::string ret = msg;
		for(int i = 0; i < ListPairLoader::getStringsSorted().size(); ++i)
		{
			ListPairItem item = ListPairLoader::getStringsSorted()[i];
			StringUtil::replace(ret,item.descr,item.val);
			StringUtil::toLower(item.descr);
			StringUtil::replace(ret,item.descr,item.val);
		}

		return ret;
	}

	cge::RequestMessage ChatComponent::receiveRequest( const std::string& player, const std::string& requestText, const std::string& tinfoText, const agui::Color& tableColor, const agui::Color& playerColor )
	{
		bool reposCaret = true;

#ifdef CGE_MOBILE

		reposCaret = false;
#endif

		m_chat->setCurrentColor(playerColor);
		if(m_chat->getTextLength() != 0)
		{
			m_chat->appendText("\n",false,reposCaret);
		}

		RequestMessage msg;
		msg.startPos = m_chat->getTextLength();
		if(isAppendingTable() && m_table != -1)
		{
			std::stringstream ss;
			ss << "(";
			ss << m_table;
			ss << ") ";
			m_chat->appendText(ss.str(),false,reposCaret);
		}
		m_chat->appendText(player + ": ",false,reposCaret);
		m_chat->setCurrentColor(m_chat->getFontColor());
		m_chat->appendText(requestText + "\n",false,reposCaret);
		m_chat->setCurrentColor(tableColor);
		m_chat->appendText(tinfoText,false,reposCaret);
		msg.endPos = m_chat->getTextLength();
		return msg;
	}

	void ChatComponent::grayOutRequest( const RequestMessage& r )
	{
		int selStart = m_chat->getSelectionStart();
		int selEnd = m_chat->getSelectionEnd();
		m_chat->setSelection(r.startPos,r.endPos);
		m_chat->setSelectionColor(agui::Color(0.5f,0.5f,0.5f));
		m_chat->setSelection(selStart,selEnd);
	}

	int ChatComponent::getEntryHeight() const
	{
		return m_sendButton->getHeight();
	}

}
