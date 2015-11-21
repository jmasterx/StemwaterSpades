#include "Game/Handler/LobbyEventProvider.hpp"

namespace cge
{
	LobbyEventProvider::LobbyEventProvider(void)
	{
	}

	LobbyEventProvider::~LobbyEventProvider(void)
	{
	}

	void LobbyEventProvider::addLobbyListener( LobbyEventListener* listener )
	{
		m_lobbyListeners.push_back(listener);
	}

	void LobbyEventProvider::removeLobbyListener( LobbyEventListener* listener )
	{
		m_lobbyListeners.erase(
			std::remove(m_lobbyListeners.begin(),
			m_lobbyListeners.end(), listener),
			m_lobbyListeners.end());
	}

	void LobbyEventProvider::clearLobbyListeners()
	{
		m_lobbyListeners.clear();
	}

	ClientPlayer* LobbyEventProvider::getPlayerByName( const std::string& name )
	{
		ClientPlayer* client = NULL;
		DISPATCH_LOBBY_EVENT
		{
			if((*it)->getPlayerByName(client,name))
				return client;
			
		}

		return client;
	}

	bool LobbyEventProvider::isSelfPlayer( ClientPlayer* client )
	{
		ClientPlayer* c = getPlayerByName("");
		if(c == NULL || client == NULL)
			return false;
		return c == client;
	}

	bool LobbyEventProvider::isMuted(const std::string& name)
	{
		bool result = false;

		DISPATCH_LOBBY_EVENT
		{
			if((*it)->isMutedImpl(name,&result))
			{
				return result;
			}
		}

		return result;

	}

	bool LobbyEventProvider::isPlayerInGame( const std::string& playerName )
	{
		bool result = false;

		DISPATCH_LOBBY_EVENT
		{
			if((*it)->isPlayerInGameImpl(playerName,&result))
			{
				return result;
			}
		}

		return result;
	}

	bool LobbyEventProvider::isSelfInGame()
	{
		ClientPlayer* p = getPlayerByName("");
		if(p)
		{
			return isPlayerInGame(p->getUsername());
		}

		return false;
	}

	int LobbyEventProvider::getPlayerTable( const std::string& playerName )
	{
		int tableNo = -1;
		DISPATCH_LOBBY_EVENT
		{
			if((*it)->getPlayerTableImpl(playerName,&tableNo))
			{
				return tableNo;
			}
		}

		return tableNo;
			
	}

	bool LobbyEventProvider::isFriend( const std::string& name )
	{
		bool isfriend = false;
		DISPATCH_LOBBY_EVENT
		{
			if((*it)->isFriendImpl(name,&isfriend))
			{
				return isfriend;
			}
		}

		return isfriend;
	}

	bool LobbyEventProvider::isEnemy( const std::string& name )
	{
		bool isenemy = false;
		DISPATCH_LOBBY_EVENT
		{
			if((*it)->isEnemyImpl(name,&isenemy))
			{
				return isenemy;
			}
		}

		return isenemy;
	}

	bool LobbyEventProvider::isSelfPlayer( const std::string& name )
	{
		ClientPlayer* p = NULL;
		p = getPlayerByName(name);
		return isSelfPlayer(p);
	}

	bool LobbyEventProvider::isPlayerGroupToggled( GroupTypeEnum group )
	{
		bool b = false;

		DISPATCH_LOBBY_EVENT
		{
			if((*it)->isPlayerGroupToggledImpl(group,&b))
			{
				return b;
			}
		}

		return b;
	}

	LobbyTable* LobbyEventProvider::getLobbyTable( int tableId )
	{
		LobbyTable* l = NULL;

		DISPATCH_LOBBY_EVENT
		{
			if((*it)->getTableByIdImpl(l,tableId))
			{
				return l;
			}
		}

		return l;
	}

	agui::Color LobbyEventProvider::getPlayerColor( const std::string& playerName )
	{
		agui::Color c = agui::Color(0,0,0);
		DISPATCH_LOBBY_EVENT
		{
			if((*it)->getPlayerColorImpl(playerName,&c))
			{
				return c;
			}
		}

		return c;
	
	}

}
