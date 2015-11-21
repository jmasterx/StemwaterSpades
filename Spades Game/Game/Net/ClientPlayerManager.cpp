#include "Game/Net/ClientPlayerManager.hpp"
#include <iostream>
#include <sstream>
#include <ctime>
namespace cge
{
	ClientPlayerManager::ClientPlayerManager(void)
		: m_maxTableMessages(150)
	{
		//for now, assign self random name
		m_self.setUserName("");
	}

	ClientPlayerManager::~ClientPlayerManager(void)
	{
		std::vector<ClientPlayer*> players = m_players;

		for(size_t i = 0; i < players.size(); ++i)
		{
			destroyPlayer(players[i]);
		}
	}

	bool ClientPlayerManager::playerExists( const std::string & userName ) const
	{
		if(userName == m_self.getUsername())
		{
			return true;
		}

		for(size_t i = 0; i < m_players.size(); ++i)
		{
			if(m_players[i]->getUsername() == userName)
			{
				return true;
			}
		}

		return false;
	}

	ClientPlayer* ClientPlayerManager::createPlayer( const std::string& userName )
	{
		ClientPlayer* player = new ClientPlayer();
		player->setUserName(userName);
		m_players.push_back(player);
		return player;
	}

	ClientPlayer* ClientPlayerManager::getPlayerByName( const std::string& userName )
	{
		if(userName == m_self.getUsername() || userName == "")
		{
			return &m_self;
		}

		for(size_t i = 0; i < m_players.size(); ++i)
		{
			if(m_players[i]->getUsername() == userName)
			{
				return m_players[i];
			}
		}

		return NULL;
	}

	bool ClientPlayerManager::destroyPlayer( const std::string& userName )
	{
		for(size_t i = 0; i < m_players.size(); ++i)
		{
			if(m_players[i]->getUsername() == userName)
			{
				delete m_players[i];
				m_players.erase(m_players.begin() + i);
				return true;
			}
		}

		return false;
	}

	bool ClientPlayerManager::destroyPlayer( ClientPlayer* player )
	{
		for(size_t i = 0; i < m_players.size(); ++i)
		{
			if(m_players[i] == player)
			{
				delete m_players[i];
				m_players.erase(m_players.begin() + i);
				return true;
			}
		}

		return false;
	}

	ClientPlayer* ClientPlayerManager::getSelfPlayer()
	{
		return &m_self;
	}

	bool ClientPlayerManager::_containsMute( const std::string& name )
	{
		return m_mutes.find(name) != m_mutes.end();
	}

	bool ClientPlayerManager::isMuted(const std::string& player)
	{
		if(_containsMute(player))
		{
			return m_mutes[player];
		}

		return false;
	}

	void ClientPlayerManager::setMuted( const std::string& player, bool mute )
	{
		m_mutes[player] = mute;
	}

	std::deque<TableMessage>& ClientPlayerManager::getTableMessageQueue()
	{
		return m_tableQ;
	}

	void ClientPlayerManager::queueTableMessage(const TableMessage& t)
	{
		m_tableQ.push_front(t);

		if(m_tableQ.size() > m_maxTableMessages)
		{
			m_tableQ.pop_back();
		}
	}

	void ClientPlayerManager::clearTableMessages()
	{
		m_tableQ.clear();
	}

	bool ClientPlayerManager::isFriend(const std::string& name) const
	{
		for(size_t i = 0; i < m_friends.size(); ++i)
		{
			if(m_friends[i] == name)
			{
				return true;
			}
		}

		return false;
	}

	bool ClientPlayerManager::isEnemy( const std::string& name ) const
	{
		for(size_t i = 0; i < m_enemies.size(); ++i)
		{
			if(m_enemies[i] == name)
			{
				return true;
			}
		}

		return false;
	}

	void ClientPlayerManager::addFriend( const std::string& name )
	{
		if(!isFriend(name))
		{
			m_friends.push_back(name);
		}
	}

	void ClientPlayerManager::addEnemy( const std::string& name )
	{
		if(!isEnemy(name))
		{
			m_enemies.push_back(name);
		}
	}

	void ClientPlayerManager::removeFriend( const std::string& name )
	{
		for(size_t i = 0; i < m_friends.size(); ++i)
		{
			if(m_friends[i] == name)
			{
				m_friends.erase(m_friends.begin() + i);
				return;
			}
		}
	}

	void ClientPlayerManager::removeEnemy( const std::string& name )
	{
		for(size_t i = 0; i < m_enemies.size(); ++i)
		{
			if(m_enemies[i] == name)
			{
				m_enemies.erase(m_enemies.begin() + i);
				return;
			}
		}
	}

	cge::GroupTypeEnum ClientPlayerManager::getPlayerGroup(ClientPlayer* player) const
	{
		if(isFriend(player->getUsername()))
		{
			return FRIENDS_GROUP;
		}
		else if(isEnemy(player->getUsername()))
		{
			return ENEMIES_GROUP;
		}
		else if(player->isInGame())
		{
			return IN_GAME_GROUP;
		}
		else
		{
			return IN_LOBBY_GROUP;
		}

	}

	int ClientPlayerManager::getNumPlayers() const
	{
		return m_players.size();
	}

	ClientPlayer* ClientPlayerManager::getPlayerAt( int i )
	{
		if(i < getNumPlayers())
		{
			return m_players[i];
		}

		return NULL;
	}

	void ClientPlayerManager::standPlayerUp( int tableNo, int seatNo )
	{
		for(size_t i = 0; i < m_players.size(); ++i)
		{
			if(m_players[i]->getTable() == tableNo && m_players[i]->getSeat() == seatNo)
			{
				m_players[i]->standUp();
			}
		}

		ClientPlayer* p = getSelfPlayer();

		if(p->getTable() == tableNo && p->getSeat() == seatNo)
		{
			p->standUp();
		}
	}

	std::string ClientPlayerManager::getPlayerByTable( int tableNo, int seatNo )
	{
		if(tableNo == -1 || seatNo == -1)
		{
			return "";
		}
		for(size_t i = 0; i < m_players.size(); ++i)
		{
			int tNo = -1;
			if(m_players[i]->isWatching())
			{
				tNo = m_players[i]->getWatchTableNo();
			}
			else
			{
				tNo = m_players[i]->getTable();
			}

			if(tNo == tableNo && m_players[i]->getSeat() == seatNo)
			{
				return m_players[i]->getUsername();
			}

		}

		return "";

	}

	void ClientPlayerManager::clear()
	{
		std::vector<ClientPlayer*> players = m_players;

		for(size_t i = 0; i < players.size(); ++i)
		{
			destroyPlayer(players[i]);
		}

		clearTableMessages();
		m_self = ClientPlayer();
		m_self.setUserName("");
		m_friends.clear();
		m_enemies.clear();
	}

}
