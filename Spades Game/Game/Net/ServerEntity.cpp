#include "Game/Net/ServerEntity.hpp"
#include "Game/Utility/StringUtil.hpp"
#define  USER_OVERFLOW_AMT 10
namespace cge
{
	ServerEntity::ServerEntity(void)
		: m_active(false),m_port(0),m_capacity(0),m_leagueID(-1)
	{
	}

	ServerEntity::~ServerEntity(void)
	{
	}

	void ServerEntity::setActive( bool active )
	{
		m_active = active;
	}

	bool ServerEntity::isActive() const
	{
		return m_active;
	}

	void ServerEntity::setIP( const std::string& ip )
	{
		m_ip = ip;
	}

	const std::string& ServerEntity::getIP() const
	{
		return m_ip;
	}

	void ServerEntity::setPort( int port )
	{
		m_port = port;
	}

	int ServerEntity::getPort() const
	{
		return m_port;
	}

	void ServerEntity::setName( const std::string& name )
	{
		m_name = name;
	}

	const std::string& ServerEntity::getName() const
	{
		return m_name;
	}

	void ServerEntity::setCapacity( int capacity )
	{
		m_capacity = capacity;
	}

	int ServerEntity::getCapacity() const
	{
		return m_capacity;
	}

	void ServerEntity::setInitialPlayers( const std::vector<std::string>& players )
	{
		m_players.clear();
		for(size_t i = 0; i < players.size(); i++)
		{
			m_players.insert(players[i]);
		}
	}

	void ServerEntity::playerLoggedIn( const std::string& player )
	{
		m_players.insert(player);
	}

	void ServerEntity::playerLoggedOut( const std::string& player )
	{
		m_players.erase(player);
	}

	int ServerEntity::getNumUsers() const
	{
		return m_players.size();
	}

	bool ServerEntity::isFull() const
	{
		return getNumUsers() >= getCapacity();
	}

	bool ServerEntity::isNearlyFull() const
	{
		return getNumUsers() + USER_OVERFLOW_AMT >= getCapacity();
	}

	const std::set<std::string>& ServerEntity::getPlayers() const
	{
		return m_players;
	}

	bool ServerEntity::playerIsLoggedIn( const std::string& p )
	{
		std::string lower;
		std::string lowerP = p;
		StringUtil::toLower(lowerP);
		for (std::set<std::string>::iterator it=m_players.begin(); it!=m_players.end(); ++it) {
			lower = *it;
			StringUtil::toLower(lower);
			if(lower == lowerP)
				return true;
		}

		return false;
	}

	void ServerEntity::setLeagueID( int id )
	{
		m_leagueID = id;
	}

	int ServerEntity::getLeagueID() const
	{
		return m_leagueID;
	}

}