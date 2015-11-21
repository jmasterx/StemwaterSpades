#include "Game/Handler/LobbyContextHandler.hpp"
#include <iostream>
#include "Game/Utility/StringUtil.hpp"
namespace cge
{
	LobbyContextHandler::~LobbyContextHandler(void)
	{
		m_menu->removeActionListener(this);
		m_gui->removeMousePreviewListener(this);
	}

	LobbyContextHandler::LobbyContextHandler( GuiFactory* factory,LanguageManager* language, agui::Gui* gui,SettingsManager* settings )
		: m_factory(factory), m_languageManager(language),m_invokeX(0),m_invokeY(0),m_mouseX(0),m_mouseY(0),m_gui(gui),
		m_invoker(NULL),m_botPos(-1),m_settings(settings)
	{
		m_menu = factory->createPopUpMenu();
		m_menu->setTextGaps(2,0,2);
		m_menu->setShowIcon(false);
		m_menu->addActionListener(this);
		m_gui->addMousePreviewListener(this);
	}


	void LobbyContextHandler::showPlayerContext( 
		const std::string& playerName, int x, int y,
		PlayerMenuContext context, agui::Widget* invoker )
	{
		showContext(playerName,x,y,context,invoker);
	}


	void LobbyContextHandler::addAvatarItems( const std::string& playerName )
	{
		if(playerName == m_factory->getLanguageManager()->getElement("cyborg"))
		{
			m_bidTextItem.setText(m_languageManager->
				getElement("bid.text"));
			m_menu->addItem(&m_bidTextItem);

			m_sendGiftItem.setText(m_languageManager->
				getElement("send.gift"));
			m_menu->addItem(&m_sendGiftItem);
		}
		else
		{
			m_viewPlayerProfileItem.setText(
			m_languageManager->
			getElementWithArg("context.viewprofile",playerName));
		m_menu->addItem(&m_viewPlayerProfileItem);

		m_bidTextItem.setText(m_languageManager->
			getElement("bid.text"));
		m_menu->addItem(&m_bidTextItem);

		bool selfA = isSelfPlayer(playerName);
		bool selfB = isSelfPlayerG(playerName);

		if(selfA || selfB)
		m_sendGiftItem.setText(m_languageManager->
			getElement("send.gift.self"));
		else
			m_sendGiftItem.setText(m_languageManager->
			getElement("send.gift"));
		m_menu->addItem(&m_sendGiftItem);

		if(!selfA && !selfB)
		{


			if((isFriend(playerName) || isFriendG(playerName)) &&
				(!isSelfPlayer(playerName) || !isSelfPlayerG(playerName)))
			{
				m_removePlayerFromFriendsItem.setText(
					m_languageManager->
					getElementWithArg("context.removefriend",playerName));
				m_menu->addItem(&m_removePlayerFromFriendsItem);
			}
			/*
			else if((isEnemy(playerName) || isEnemyG(playerName)) && 
				(!isSelfPlayer(playerName) || !isSelfPlayerG(playerName)))
			{
				m_removePlayerFromEnemiesItem.setText(
					appendVar(m_languageManager->
					getElement("context.removeenemy"),playerName));
				m_menu->addItem(&m_removePlayerFromEnemiesItem);
			}
			*/
			else if(!isSelfPlayer(playerName) || !isSelfPlayerG(playerName))
			{
				m_addPlayerToFriendsItem.setText(
					m_languageManager->
					getElementWithArg("context.addfriend",playerName));
				m_menu->addItem(&m_addPlayerToFriendsItem);

				/*
				m_addPlayerToEnemiesItem.setText(
					appendVar(m_languageManager->
					getElement("context.addenemy"),playerName));
				m_menu->addItem(&m_addPlayerToEnemiesItem);
				*/
			}

			m_kickPlayerItem.setText(m_languageManager->
				getElement("kick.player"));
			m_menu->addItem(&m_kickPlayerItem);

			if(!isSelfPlayerG(playerName) || !isSelfPlayer(playerName))
			{
				if(isMuted(playerName) || isMutedG(playerName))
				{
					m_unMutePlayerItem.setText(
						m_languageManager->
						getElementWithArg("context.unmute",playerName));
					m_menu->addItem(&m_unMutePlayerItem);
				}
				else
				{
					m_mutePlayerItem.setText(
						m_languageManager->
						getElementWithArg("context.mute",playerName));
					m_menu->addItem(&m_mutePlayerItem);
				}
			}
		}
		else
		{
			
			m_sendGiftTableItem.setText(m_languageManager->
				getElement("send.gift.table"));
			m_menu->addItem(&m_sendGiftTableItem);
			

			m_removeGift.setText(m_languageManager->
				getElement("remove.gift"));
			m_menu->addItem(&m_removeGift);
/*
			if(m_settings->getBoolSetting("light.meter.on"))
			{
				m_bidSetting.setText(m_languageManager->getElement("show.numbers"));
			}
			else
			{
				m_bidSetting.setText(m_languageManager->getElement("show.lights"));
			}

			m_menu->addItem(&m_bidSetting);

			if(m_settings->getBoolSetting("player.names.on"))
			{
				m_showPlayerNamesItem.setText(m_languageManager->getElement("hide.player.names"));
			}
			else
			{
				m_showPlayerNamesItem.setText(m_languageManager->getElement("show.player.names"));
			}

			m_menu->addItem(&m_showPlayerNamesItem);
			*/
		}
	  }
	}

	void LobbyContextHandler::addFriendItems( const std::string& playerName )
	{
		m_viewPlayerProfileItem.setText(
			m_languageManager->
			getElementWithArg("context.viewprofile",playerName));
		m_menu->addItem(&m_viewPlayerProfileItem);
		
		if(!isSelfPlayer(playerName) && !isSelfPlayerG(playerName))
		{
			if(!isSelfInGame() && getPlayerTable(playerName) != -1)
			{
				int table = getPlayerTable(playerName);
				m_goToTableItem.setText(m_languageManager->getElementWithArg("go.to.table",StringUtil::toString(table)));
				m_menu->addItem(&m_goToTableItem);
			}

			if(isSelfInGame() && !isPlayerInGame(playerName))
			{
				m_invitePlayerItem.setText(
					m_languageManager->
					getElementWithArg("context.invite",playerName));
				m_menu->addItem(&m_invitePlayerItem);
			}

			m_removePlayerFromFriendsItem.setText(
				m_languageManager->
				getElementWithArg("context.removefriend",playerName));
			m_menu->addItem(&m_removePlayerFromFriendsItem);
			

			if(isMuted(playerName))
			{
				m_unMutePlayerItem.setText(
					m_languageManager->
					getElementWithArg("context.unmute",playerName));
				m_menu->addItem(&m_unMutePlayerItem);
			}
			else
			{
				m_mutePlayerItem.setText(
					m_languageManager->
					getElementWithArg("context.mute",playerName));
				m_menu->addItem(&m_mutePlayerItem);
			}
		}
	}

	void LobbyContextHandler::addEnemyItems( const std::string& playerName )
	{	
		m_viewPlayerProfileItem.setText(
			m_languageManager->
			getElementWithArg("context.viewprofile",playerName));
		m_menu->addItem(&m_viewPlayerProfileItem);

		if(!isSelfPlayer(playerName) ^ !isSelfPlayerG(playerName))
		{
			/*

			m_removePlayerFromEnemiesItem.setText(
				appendVar(m_languageManager->
				getElement("context.removeenemy"),playerName));
			m_menu->addItem(&m_removePlayerFromEnemiesItem);
			*/

			if(isMuted(playerName))
			{
				m_unMutePlayerItem.setText(
					m_languageManager->
					getElementWithArg("context.unmute",playerName));
				m_menu->addItem(&m_unMutePlayerItem);
			}
			else
			{
				m_mutePlayerItem.setText(
					m_languageManager->
					getElementWithArg("context.mute",playerName));
				m_menu->addItem(&m_mutePlayerItem);
			}
		}
	}

	void LobbyContextHandler::addInLobbyItems( const std::string& playerName, bool sitting )
	{
		
		m_viewPlayerProfileItem.setText(
			m_languageManager->
			getElementWithArg("context.viewprofile",playerName));
		m_menu->addItem(&m_viewPlayerProfileItem);
		
		if(!isSelfPlayer(playerName) && !isSelfPlayerG(playerName))
		{
			if(!isSelfInGame() && getPlayerTable(playerName) != -1 && !sitting)
			{
				int table = getPlayerTable(playerName);
				m_goToTableItem.setText(m_languageManager->getElementWithArg("go.to.table",StringUtil::toString(table)));
				m_menu->addItem(&m_goToTableItem);
			}

			if(isSelfInGame() && !isPlayerInGame(playerName))
			{
				m_invitePlayerItem.setText(
					m_languageManager->
					getElementWithArg("context.invite",playerName));
				m_menu->addItem(&m_invitePlayerItem);
			}

			
			m_addPlayerToFriendsItem.setText(
				m_languageManager->
				getElementWithArg("context.addfriend",playerName));
			m_menu->addItem(&m_addPlayerToFriendsItem);

			/*
			m_addPlayerToEnemiesItem.setText(
				appendVar(m_languageManager->
				getElement("context.addenemy"),playerName));
			m_menu->addItem(&m_addPlayerToEnemiesItem);
			*/

			if(isMuted(playerName) || isMutedG(playerName))
			{
				m_unMutePlayerItem.setText(
					m_languageManager->
					getElementWithArg("context.unmute",playerName));
				m_menu->addItem(&m_unMutePlayerItem);
			}
			else
			{
				m_mutePlayerItem.setText(
					m_languageManager->
					getElementWithArg("context.mute",playerName));
				m_menu->addItem(&m_mutePlayerItem);
			}
		}
		else
		{
			ClientPlayer* self = getPlayerByName("");
			if(self && self->getSeat() != -1)
			{
				m_leaveTableItem.setText(
					m_languageManager->
					getElement("leave.button"));
				m_menu->addItem(&m_leaveTableItem);
			}
		}
	}

	void LobbyContextHandler::addInGameItems( const std::string& playerName )
	{
		m_viewPlayerProfileItem.setText(
			m_languageManager->
			getElementWithArg("context.viewprofile",playerName));
		m_menu->addItem(&m_viewPlayerProfileItem);
		
		if(!isSelfPlayer(playerName) && !isSelfPlayerG(playerName))
		{
			m_addPlayerToFriendsItem.setText(
				m_languageManager->
				getElementWithArg("context.addfriend",playerName));
			m_menu->addItem(&m_addPlayerToFriendsItem);

			/*
			m_addPlayerToEnemiesItem.setText(
				appendVar(m_languageManager->
				getElement("context.addenemy"),playerName));
			m_menu->addItem(&m_addPlayerToEnemiesItem);
			*/

			if(isMuted(playerName) || isMutedG(playerName))
			{
				m_unMutePlayerItem.setText(
					m_languageManager->
					getElementWithArg("context.unmute",playerName));
				m_menu->addItem(&m_unMutePlayerItem);
			}
			else
			{
				m_mutePlayerItem.setText(
					m_languageManager->
					getElementWithArg("context.mute",playerName));
				m_menu->addItem(&m_mutePlayerItem);
			}
		}
	}

	void LobbyContextHandler::actionPerformed( const agui::ActionEvent &evt )
	{
		if(evt.getSource() == &m_viewPlayerProfileItem)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestShowProfile(m_player);
			DISPATCH_GAME_EVENT
				(*it)->requestShowProfile(m_player);

		}
		else if(evt.getSource() == &m_addPlayerToFriendsItem)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestAddFriend(m_player);
			DISPATCH_GAME_EVENT
				(*it)->requestAddFriend(m_player);
		}
		else if(evt.getSource() == &m_removePlayerFromFriendsItem)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestRemoveFriend(m_player);
			DISPATCH_GAME_EVENT
				(*it)->requestRemoveFriend(m_player);
		}
		else if(evt.getSource() == &m_addPlayerToEnemiesItem)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestAddEnemy(m_player);
			DISPATCH_GAME_EVENT
				(*it)->requestAddEnemy(m_player);
		}
		else if(evt.getSource() == &m_removePlayerFromEnemiesItem)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestRemoveEnemy(m_player);
			DISPATCH_GAME_EVENT
				(*it)->requestRemoveEnemy(m_player);
		}
		else if(evt.getSource() == &m_mutePlayerItem)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->setMuted(m_player,true);
			DISPATCH_GAME_EVENT
				(*it)->setMuted(m_player,true);
		}
		else if(evt.getSource() == &m_unMutePlayerItem)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->setMuted(m_player,false);
			DISPATCH_GAME_EVENT
				(*it)->setMuted(m_player,false);
		}
		else if(evt.getSource() == &m_kickPlayerItem)
		{
			DISPATCH_GAME_EVENT
			(*it)->kickPlayer(m_player);
		}
		else if(evt.getSource() == &m_bidTextItem)
		{
			DISPATCH_GAME_EVENT
				(*it)->showBidTooltip(m_mouseX,m_mouseY,m_invoker);
		}
		else if(evt.getSource() == &m_sendGiftTableItem)
		{
			DISPATCH_GAME_EVENT
				(*it)->showGiftDialog(-1,"");
		}
		else if(evt.getSource() == &m_sendGiftItem)
		{
		   DISPATCH_GAME_EVENT
			(*it)->showGiftDialog(m_botPos,m_player);
		}
		else if(evt.getSource() == &m_invitePlayerItem)
		{
			ClientPlayer* p = getPlayerByName("");

		  DISPATCH_LOBBY_EVENT
			  (*it)->sendInvitation(p->getTable(),m_player);

		  //cannot invite in game (duh)
		}
		else if(evt.getSource() == &m_goToTableItem)
		{
			int table = getPlayerTable(m_player);
			if(table != -1)
			{
				DISPATCH_LOBBY_EVENT
					(*it)->focusOnLobbyTable(table,true);
			}
		}
		else if(evt.getSource() == &m_removeGift)
		{
			DISPATCH_GAME_EVENT
				(*it)->removeGift();
		}
		else if(evt.getSource() == &m_lobbyChatItem)
		{
			DISPATCH_GAME_EVENT
				(*it)->gameChatStyleChanged("lobby");
		}
		else if(evt.getSource() == &m_gameChatItem)
		{
			DISPATCH_GAME_EVENT
				(*it)->gameChatStyleChanged("game");
		}
		else if(evt.getSource() == &m_noChatItem)
		{
			DISPATCH_GAME_EVENT
				(*it)->gameChatStyleChanged("off");
		}
		/*
		else if(evt.getSource() == &m_bidSetting)
		{
			bool lightsOn = m_settings->getBoolSetting("light.meter.on");

			DISPATCH_GAME_EVENT
				(*it)->bidMeterStyleChanged(!lightsOn);
		}
		else if(evt.getSource() == &m_showPlayerNamesItem)
		{
			bool show = m_settings->getBoolSetting("player.names.on");

			DISPATCH_GAME_EVENT
				(*it)->showPlayerNamesChanged(!show);
		}
		*/
		else if(evt.getSource() == &m_leaveTableItem)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestLeaveFromContext();
		}
	}

	void LobbyContextHandler::addChatItems( const std::string& playerName )
	{
		m_viewPlayerProfileItem.setText(
			m_languageManager->
			getElementWithArg("context.viewprofile",playerName));
		m_menu->addItem(&m_viewPlayerProfileItem);
		
		if(!isSelfPlayer(playerName) && !isSelfPlayerG(playerName))
		{
			if((isFriend(playerName) || isFriendG(playerName)) &&
				(!isSelfPlayer(playerName) || !isSelfPlayerG(playerName)))
			{
				m_removePlayerFromFriendsItem.setText(
					m_languageManager->
					getElementWithArg("context.removefriend",playerName));
				m_menu->addItem(&m_removePlayerFromFriendsItem);
			}
			/*
			else if((isEnemy(playerName) || isEnemyG(playerName)) && 
				(!isSelfPlayer(playerName) || !isSelfPlayerG(playerName)))
			{
				m_removePlayerFromEnemiesItem.setText(
					appendVar(m_languageManager->
					getElement("context.removeenemy"),playerName));
				m_menu->addItem(&m_removePlayerFromEnemiesItem);
			}
			*/
			else if(!isSelfPlayer(playerName) || !isSelfPlayerG(playerName))
			{
				m_addPlayerToFriendsItem.setText(
					m_languageManager->
					getElementWithArg("context.addfriend",playerName));
				m_menu->addItem(&m_addPlayerToFriendsItem);

				/*
				m_addPlayerToEnemiesItem.setText(
					appendVar(m_languageManager->
					getElement("context.addenemy"),playerName));
				m_menu->addItem(&m_addPlayerToEnemiesItem);
				*/
			}
			
			if(!isSelfInGame() && getPlayerTable(playerName) != -1)
			{
				int table = getPlayerTable(playerName);
				m_goToTableItem.setText(m_languageManager->getElementWithArg("go.to.table",StringUtil::toString(table)));
				m_menu->addItem(&m_goToTableItem);
			}

			if(isSelfInGame() && !isPlayerInGame(playerName))
			{
				m_invitePlayerItem.setText(
					m_languageManager->
					getElementWithArg("context.invite",playerName));
				m_menu->addItem(&m_invitePlayerItem);
			}

			if(!isSelfPlayerG(playerName) || !isSelfPlayer(playerName))
			{
				if(isMuted(playerName) || isMutedG(playerName))
				{
					m_unMutePlayerItem.setText(
						m_languageManager->
						getElementWithArg("context.unmute",playerName));
					m_menu->addItem(&m_unMutePlayerItem);
				}
				else
				{
					m_mutePlayerItem.setText(
						m_languageManager->
						getElementWithArg("context.mute",playerName));
					m_menu->addItem(&m_mutePlayerItem);
				}
			}
		}
	}

	void LobbyContextHandler::showPlayerGameContext( const std::string& playerName, int x, int y, PlayerMenuContext context, agui::Widget* invoker, int playerPos )
	{
		showContext(playerName,x,y,context,invoker);
        m_botPos = playerPos;
	}

	void LobbyContextHandler::showContext( const std::string& playerName, int x, int y, PlayerMenuContext context, agui::Widget* invoker )
	{
        m_botPos = -1;
		if(playerName != m_factory->getLanguageManager()->getElement("cyborg") && getPlayerByName(playerName) == NULL && getPlayerByNameG(playerName) == NULL)
			return;

		m_menu->clearItems();
		m_player = playerName;
		m_invoker = invoker;

		switch(context)
		{
		case LIST_FRIEND_RCLICK:
			addFriendItems(playerName);
			break;
		case LIST_ENEMY_RCLICK:
			addEnemyItems(playerName);
			break;
		case  LIST_LOBBY_RCLICK:
			addInLobbyItems(playerName,false);
			break;
		case  AT_TABLE_RCLICK:
			addInLobbyItems(playerName,true);
			break;
		case LIST_IN_GAME_RCLICK:
			addInGameItems(playerName);
			break;
		case CHAT_RCLICK:
			addChatItems(playerName);
			break;
		case AVATAR_RCLICK:
			addAvatarItems(playerName);
			break;
		case AVATAR_WATCHER_RCLICK:
			addAvatarWatchItems(playerName);
			break;
		case CHAT_SELECT_RCLICK:
			addChatSelectItems(playerName);
			break;
		}

		/*
		if(!isSelfPlayerG(playerName) || !isSelfPlayer(playerName))
		{
			m_menu->showPopUp(invoker,x,y);
		}
		*/

		m_menu->showPopUp(invoker,x,y);
		m_invokeX = m_menu->getAbsolutePosition().getX();
		m_invokeY = m_menu->getAbsolutePosition().getY();
	
	}

	void LobbyContextHandler::mouseMoveCB( agui::MouseEvent &evt )
	{
		m_mouseX = evt.getX();
		m_mouseY = evt.getY();
	}

	void LobbyContextHandler::addAvatarWatchItems( const std::string& playerName )
	{
		if(playerName == m_factory->getLanguageManager()->getElement("cyborg"))
		{
			m_bidTextItem.setText(m_languageManager->
				getElement("bid.text"));
			m_menu->addItem(&m_bidTextItem);
		}
		else
		{
			m_viewPlayerProfileItem.setText(
			m_languageManager->
			getElementWithArg("context.viewprofile",playerName));
		m_menu->addItem(&m_viewPlayerProfileItem);

		m_bidTextItem.setText(m_languageManager->
			getElement("bid.text"));
		m_menu->addItem(&m_bidTextItem);

		bool selfA = isSelfPlayer(playerName);
		bool selfB = isSelfPlayerG(playerName);

		if(!selfA && !selfB)
		{
			if((isFriend(playerName) || isFriendG(playerName)) &&
				(!isSelfPlayer(playerName) || !isSelfPlayerG(playerName)))
			{
				m_removePlayerFromFriendsItem.setText(
					m_languageManager->
					getElementWithArg("context.removefriend",playerName));
				m_menu->addItem(&m_removePlayerFromFriendsItem);
			}
			/*
			else if((isEnemy(playerName) || isEnemyG(playerName)) && 
				(!isSelfPlayer(playerName) || !isSelfPlayerG(playerName)))
			{
				m_removePlayerFromEnemiesItem.setText(
					appendVar(m_languageManager->
					getElement("context.removeenemy"),playerName));
				m_menu->addItem(&m_removePlayerFromEnemiesItem);
			}
			*/
			else if(!isSelfPlayer(playerName) || !isSelfPlayerG(playerName))
			{
				m_addPlayerToFriendsItem.setText(
					m_languageManager->
					getElementWithArg("context.addfriend",playerName));
				m_menu->addItem(&m_addPlayerToFriendsItem);

				/*
				m_addPlayerToEnemiesItem.setText(
					appendVar(m_languageManager->
					getElement("context.addenemy"),playerName));
				m_menu->addItem(&m_addPlayerToEnemiesItem);
				*/
			}

			if(!isSelfPlayerG(playerName) || !isSelfPlayer(playerName))
			{
				if(isMuted(playerName) || isMutedG(playerName))
				{
					m_unMutePlayerItem.setText(
						m_languageManager->
						getElementWithArg("context.unmute",playerName));
					m_menu->addItem(&m_unMutePlayerItem);
				}
				else
				{
					m_mutePlayerItem.setText(
						m_languageManager->
						getElementWithArg("context.mute",playerName));
					m_menu->addItem(&m_mutePlayerItem);
				}
			}
		}
	  }
/*
		if(m_settings->getBoolSetting("light.meter.on"))
		{
			m_bidSetting.setText(m_languageManager->getElement("show.numbers"));
		}
		else
		{
			m_bidSetting.setText(m_languageManager->getElement("show.lights"));
		}
		
		m_menu->addItem(&m_bidSetting);

		if(m_settings->getBoolSetting("player.names.on"))
		{
			m_showPlayerNamesItem.setText(m_languageManager->getElement("hide.player.names"));
		}
		else
		{
			m_showPlayerNamesItem.setText(m_languageManager->getElement("show.player.names"));
		}

		m_menu->addItem(&m_showPlayerNamesItem);
		*/
	}

	PopUpMenu* LobbyContextHandler::getPopup()
	{
		return m_menu;
	}
    
    int LobbyContextHandler::getPlayerPos() const {
        return m_botPos;
    }

	void LobbyContextHandler::addChatSelectItems( const std::string& playerName )
	{
		m_gameChatItem.setText(
			m_languageManager->
			getElement("game.chat"));
		m_menu->addItem(&m_gameChatItem);

		m_lobbyChatItem.setText(
			m_languageManager->
			getElement("chat.lobby"));
		m_menu->addItem(&m_lobbyChatItem);

		m_noChatItem.setText(
			m_languageManager->
			getElement("chat.off"));
		m_menu->addItem(&m_noChatItem);
	}
}
