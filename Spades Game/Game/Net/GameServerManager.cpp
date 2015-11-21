#include "Game/Net/GameServerManager.hpp"
namespace cge
{
	GameServerManager::GameServerManager(void)
	{
	}

	GameServerManager::~GameServerManager(void)
	{
		for(ServerEntitySetArray::iterator it = 
			m_allocedEntities.begin(); it != m_allocedEntities.end(); ++it)
		{
			delete((*it));
		}
	}

	ServerEntity* GameServerManager::createEntity(const RakNet::RakNetGUID& guid)
	{
		if(guidExists(guid))
			return NULL;

		ServerEntity* p = new ServerEntity();
		m_allocedEntities.insert(p);
		assignGuid(guid,p);
		return p;
	}

	void GameServerManager::destroyEntity( ServerEntity* entity )
	{
		removeGuid(entity->getClient()->getGUID());
		delete entity;

		m_allocedEntities.erase(
			std::remove(m_allocedEntities.begin(),
			m_allocedEntities.end(), entity),
			m_allocedEntities.end());

		m_inactiveEntities.erase(
			std::remove(m_inactiveEntities.begin(),
			m_inactiveEntities.end(), entity),
			m_inactiveEntities.end());

		m_activeEntities.erase(
			std::remove(m_activeEntities.begin(),
			m_activeEntities.end(), entity),
			m_activeEntities.end());
	}

	void GameServerManager::moveToInactive( ServerEntity* entity )
	{
		m_activeEntities.erase(
			std::remove(m_activeEntities.begin(),
			m_activeEntities.end(), entity),
			m_activeEntities.end());

		m_inactiveEntities.insert(entity);
		entity->setActive(false);
	}

	void GameServerManager::moveToActive( ServerEntity* entity )
	{
		m_inactiveEntities.erase(
			std::remove(m_inactiveEntities.begin(),
			m_inactiveEntities.end(), entity),
			m_inactiveEntities.end());

		m_activeEntities.insert(entity);
		entity->setActive(true);
	}

	void GameServerManager::addToInactive( ServerEntity* entity )
	{
		m_inactiveEntities.insert(entity);
		entity->setActive(false);
	}

	ServerEntitySetArray* GameServerManager::getEntities( bool active )
	{
		if(active)
		{
			return &m_activeEntities;
		}
		else
		{
			return &m_inactiveEntities;
		}
	}

	ServerEntitySetArray* GameServerManager::getAllocedEntities()
	{
		return &m_allocedEntities;
	}

	bool GameServerManager::containsIP( const std::string& ip )
	{
		for(ServerEntitySetArray::iterator it = 
			m_allocedEntities.begin(); it != m_allocedEntities.end(); ++it)
		{
			if((*it)->getIP() == ip)
				return true;
		}

		return false;
	}

	int GameServerManager::getNumIPs( const std::string& ip )
	{
		int count = 0;
		for(ServerEntitySetArray::iterator it = 
			m_allocedEntities.begin(); it != m_allocedEntities.end(); ++it)
		{
			if((*it)->getIP() == ip)
				count++;
		}

		return count;
	}

	bool GameServerManager::guidExists( const RakNet::RakNetGUID& guid ) const
	{
		std::map<RakNet::RakNetGUID,ServerEntity*>::const_iterator it = m_entityMap.find(guid);
		return it!=m_entityMap.end();
	}

	ServerEntity* GameServerManager::getEntity( const RakNet::RakNetGUID& guid )
	{
		if(guidExists(guid))
			return m_entityMap[guid];

		return NULL;
	}

	void GameServerManager::assignGuid( const RakNet::RakNetGUID& guid,ServerEntity* entity )
	{
		m_entityMap[guid] = entity;
	}

	void GameServerManager::removeGuid( const RakNet::RakNetGUID& guid )
	{
		std::map<RakNet::RakNetGUID,ServerEntity*>::iterator iter = m_entityMap.find(guid);
		if( iter != m_entityMap.end() )
			m_entityMap.erase( iter );
	}
}
