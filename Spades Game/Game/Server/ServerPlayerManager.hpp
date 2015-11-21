#ifndef CGE_SERVER_PLAYER_MANAGER_HPP
#define CGE_SERVER_PLAYER_MANAGER_HPP
#include "Game/Net/ServerPlayer.hpp"
#include "Game/serverplatform.hpp"
#include <vector>
#include <map>
namespace cge
{
	class ServerPlayerManager
	{
		ServerPlayerSetArray m_allocedPlayers; // free ptrs

		ServerPlayerSetArray m_inactivePlayers;
		ServerPlayerSetArray m_activePlayers;
		std::map<RakNet::RakNetGUID,ServerPlayer*> m_playerMap;
		void removeGuid(const RakNet::RakNetGUID& guid);
		void assignGuid(const RakNet::RakNetGUID& guid,ServerPlayer* player);
	public:
		ServerPlayerManager(void);
		ServerPlayer* createPlayer(const RakNet::RakNetGUID& guid);
		void destroyPlayer(ServerPlayer* player);
		void addToInactive(ServerPlayer* player);
		void moveToInactive(ServerPlayer* player);
		void moveToActive(ServerPlayer* player);
		ServerPlayer* getActivePlayer(const std::string& name);
		ServerPlayerSetArray* getPlayers(bool active);
		ServerPlayerSetArray* getAllocedPlayers();
		bool containsIP(const std::string& ip);
		int getNumIPs(const std::string& ip);
		ServerPlayer* getPlayer(const RakNet::RakNetGUID& guid);
				bool guidExists(const RakNet::RakNetGUID& guid) const;
		~ServerPlayerManager(void);
	};
}

#endif