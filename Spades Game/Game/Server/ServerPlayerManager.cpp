#include "Game/Server/ServerPlayerManager.hpp"
#include <algorithm>
namespace cge
{
	ServerPlayerManager::ServerPlayerManager(void)
	{
	}

	ServerPlayerManager::~ServerPlayerManager(void)
	{
		for(ServerPlayerSetArray::iterator it = 
			m_allocedPlayers.begin(); it != m_allocedPlayers.end(); ++it)
		{
			delete((*it));
		}
	}

	ServerPlayer* ServerPlayerManager::createPlayer(const RakNet::RakNetGUID& guid)
	{
		if(guidExists(guid))
			return NULL;

		ServerPlayer* p = new ServerPlayer();
		m_allocedPlayers.insert(p);
		assignGuid(guid,p);
		return p;
	}

	void ServerPlayerManager::destroyPlayer( ServerPlayer* player )
	{
		removeGuid(player->getClient()->getGUID());
		delete player;

		m_allocedPlayers.erase(
			std::remove(m_allocedPlayers.begin(),
			m_allocedPlayers.end(), player),
			m_allocedPlayers.end());

		m_inactivePlayers.erase(
			std::remove(m_inactivePlayers.begin(),
			m_inactivePlayers.end(), player),
			m_inactivePlayers.end());

		m_activePlayers.erase(
			std::remove(m_activePlayers.begin(),
			m_activePlayers.end(), player),
			m_activePlayers.end());
	}

	void ServerPlayerManager::moveToInactive( ServerPlayer* player )
	{
		m_activePlayers.erase(
			std::remove(m_activePlayers.begin(),
			m_activePlayers.end(), player),
			m_activePlayers.end());

		m_inactivePlayers.insert(player);
		player->setActive(false);
	}

	void ServerPlayerManager::moveToActive( ServerPlayer* player )
	{
		m_inactivePlayers.erase(
			std::remove(m_inactivePlayers.begin(),
			m_inactivePlayers.end(), player),
			m_inactivePlayers.end());

		m_activePlayers.insert(player);
		player->setActive(true);
	}

	void ServerPlayerManager::addToInactive( ServerPlayer* player )
	{
		m_inactivePlayers.insert(player);
		player->setActive(false);
	}

	ServerPlayerSetArray* ServerPlayerManager::getPlayers( bool active )
	{
		if(active)
		{
			return &m_activePlayers;
		}
		else
		{
			return &m_inactivePlayers;
		}
	}

	ServerPlayerSetArray* ServerPlayerManager::getAllocedPlayers()
	{
		return &m_allocedPlayers;
	}

	ServerPlayer* ServerPlayerManager::getActivePlayer( const std::string& name )
	{
		for(ServerPlayerSetArray::iterator it = 
			m_activePlayers.begin(); it != m_activePlayers.end(); ++it)
		{
			if((*it)->getUsername() == name)
			{
				return (*it);
			}
		}

		return NULL;
	}

	bool ServerPlayerManager::containsIP( const std::string& ip )
	{
		for(ServerPlayerSetArray::iterator it = 
			m_allocedPlayers.begin(); it != m_allocedPlayers.end(); ++it)
		{
			if((*it)->getIP() == ip)
				return true;
		}

		return false;
	}

	int ServerPlayerManager::getNumIPs( const std::string& ip )
	{
		int count = 0;
		for(ServerPlayerSetArray::iterator it = 
			m_allocedPlayers.begin(); it != m_allocedPlayers.end(); ++it)
		{
			if((*it)->getIP() == ip)
				count++;
		}

		return count;
	}

	bool ServerPlayerManager::guidExists( const RakNet::RakNetGUID& guid ) const
	{
		std::map<RakNet::RakNetGUID,ServerPlayer*>::const_iterator it = m_playerMap.find(guid);
		return it!=m_playerMap.end();
	}

	ServerPlayer* ServerPlayerManager::getPlayer( const RakNet::RakNetGUID& guid )
	{
		if(guidExists(guid))
			return m_playerMap[guid];

		return NULL;
	}

	void ServerPlayerManager::assignGuid( const RakNet::RakNetGUID& guid,ServerPlayer* player )
	{
		m_playerMap[guid] = player;
	}

	void ServerPlayerManager::removeGuid( const RakNet::RakNetGUID& guid )
	{
		std::map<RakNet::RakNetGUID,ServerPlayer*>::iterator iter = m_playerMap.find(guid);
		if( iter != m_playerMap.end() )
			m_playerMap.erase( iter );
	}



}

