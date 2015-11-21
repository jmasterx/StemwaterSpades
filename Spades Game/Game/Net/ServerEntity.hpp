#ifndef CGE_SERVER_ENTITY_HPP
#define CGE_SERVER_ENTITY_HPP
#include "Game/Net/ServerInstance.hpp"
#include <vector>
#include <set>

namespace cge
{
	class ServerEntity : public ServerInstance
	{
		bool m_active;
		std::set<std::string> m_players;
		std::string m_ip;
		int m_port;
		int m_capacity;
		std::string m_name;
		int m_leagueID;
	public:
		ServerEntity(void);
		void setActive(bool active);
		bool isActive() const;
		void setIP(const std::string& ip);
		const std::string& getIP() const;
		void setPort(int port);
		int getPort() const;
		void setName(const std::string& name);
		const std::string& getName() const;
		void setCapacity(int capacity);
		int getCapacity() const;
		void setInitialPlayers(const std::vector<std::string>& players);
		void playerLoggedIn(const std::string& player);
		void playerLoggedOut(const std::string& player);
		bool isFull() const;
		bool isNearlyFull() const;
		int getNumUsers() const;
		const std::set<std::string>& getPlayers() const;
		bool playerIsLoggedIn(const std::string& p);
		void setLeagueID(int id);
		int getLeagueID() const;
		~ServerEntity(void);
	};

	typedef std::set<ServerEntity*> ServerEntitySetArray;
	typedef std::vector<ServerEntity*> ServerEntityVecArray;
}
#endif
