#include "Game/Handler/GameEventProvider.hpp"

namespace cge
{
	GameEventProvider::GameEventProvider(void)
	{
	}

	GameEventProvider::~GameEventProvider(void)
	{
	}

	void GameEventProvider::addGameListener( GameEventListener* listener )
	{
		m_gameListeners.push_back(listener);
	}

	void GameEventProvider::removeGameListener( GameEventListener* listener )
	{
		m_gameListeners.erase(
			std::remove(m_gameListeners.begin(),
			m_gameListeners.end(), listener),
			m_gameListeners.end());
	}

	void GameEventProvider::clearGameListeners()
	{
		m_gameListeners.clear();
	}

	bool GameEventProvider::isMutedG( const std::string& name )
	{
		bool result = false;

		DISPATCH_GAME_EVENT
		{
			if((*it)->isMutedImpl(name,&result))
			{
				return result;
			}
		}

		return result;
	}

	int GameEventProvider::getTableG()
	{
		int table = -1;
		DISPATCH_GAME_EVENT
		{
			if((*it)->getTableNumberImpl(&table))
			{
				return table;
			}
		}

		return table;
	}

	cge::SpadesGameInfo GameEventProvider::getGameInfoG()
	{
		SpadesGameInfo info;
		DISPATCH_GAME_EVENT
		{
			if((*it)->getGameInfoImpl(&info))
			{
				return info;
			}
		}

		return info;
	}

	agui::Color GameEventProvider::getGamePlayerColor( const std::string& playerName )
	{
		agui::Color c = agui::Color(0,0,0);
		DISPATCH_GAME_EVENT
		{
			if((*it)->getGamePlayerColorImpl(playerName,&c))
			{
				return c;
			}
		}

		return c;
	}

	agui::Color GameEventProvider::getLobbyPlayerColor( const std::string& playerName )
	{
		agui::Color c = agui::Color(0,0,0);
		DISPATCH_GAME_EVENT
		{
			if((*it)->getLobbyPlayerColorImpl(playerName,&c))
			{
				return c;
			}
		}

		return c;
	}

	bool GameEventProvider::isFriendG( const std::string& playerName )
	{
		bool result = false;

		DISPATCH_GAME_EVENT
		{
			if((*it)->isFriendGImpl(playerName,&result))
			{
				return result;
			}
		}

		return result;
	}

	bool GameEventProvider::isEnemyG( const std::string& playerName )
	{
		bool result = false;

		DISPATCH_GAME_EVENT
		{
			if((*it)->isEnemyGImpl(playerName,&result))
			{
				return result;
			}
		}

		return result;
	}

	ClientPlayer* GameEventProvider::getPlayerByNameG( const std::string& name )
	{
		ClientPlayer* client = NULL;
		DISPATCH_GAME_EVENT
		{
			if((*it)->getPlayerByNameGImpl(client,name))
				return client;
		}

		return client;
	}

	bool GameEventProvider::isSelfPlayerG( ClientPlayer* client )
	{
		ClientPlayer* c = getPlayerByNameG("");
		if(c == NULL || client == NULL)
			return false;
		return c == client;
	}

	bool GameEventProvider::isSelfPlayerG( const std::string& name )
	{
		ClientPlayer* p = NULL;
		p = getPlayerByNameG(name);
		return isSelfPlayerG(p);
	}


}
