#ifndef CGE_GAME_SERVER_MANAGER_HPP
#define CGE_GAME_SERVER_MANAGER_HPP
#include "Game/Net/ServerEntity.hpp"
#include <vector>
#include <map>
namespace cge
{
	class GameServerManager
	{
		ServerEntitySetArray m_allocedEntities; // free ptrs

		ServerEntitySetArray m_inactiveEntities;
		ServerEntitySetArray m_activeEntities;
		std::map<RakNet::RakNetGUID,ServerEntity*> m_entityMap;
		void removeGuid(const RakNet::RakNetGUID& guid);
		void assignGuid(const RakNet::RakNetGUID& guid,ServerEntity* entity);
	public:
		GameServerManager(void);
		ServerEntity* createEntity(const RakNet::RakNetGUID& guid);
		void destroyEntity(ServerEntity* entity);
		void addToInactive(ServerEntity* entity);
		void moveToInactive(ServerEntity* entity);
		void moveToActive(ServerEntity* entity);
		ServerEntitySetArray* getEntities(bool active);
		ServerEntitySetArray* getAllocedEntities();
		bool containsIP(const std::string& ip);
		int getNumIPs(const std::string& ip);
		ServerEntity* getEntity(const RakNet::RakNetGUID& guid);
		bool guidExists(const RakNet::RakNetGUID& guid) const;
		~GameServerManager(void);
	};
}
#endif
