#include "Game/Net/ClientEventHandler.hpp"
#include <iostream>
namespace cge
{
	ClientEventHandler::ClientEventHandler(void)
	{
	}

	ClientEventHandler::~ClientEventHandler(void)
	{
	}

	void ClientEventHandler::fetchServerListMS( const std::string& username,
		const std::vector<std::string>& names, const std::vector<std::string>& ips,
		const std::vector<int>& ports, const std::vector<int>& capacities, 
		const std::vector<int>& numUsers, const std::vector<int>& numFriends )
	{
		DISPATCH_SERVER_EVENT
			(*it)->receivedServerListFromMaster(username,
			names,ips,ports,capacities,numUsers,numFriends);
	}

	void ClientEventHandler::serverFullMS( const std::string& username, const std::string& serverName, const std::string& ip, int port, bool full )
	{
		DISPATCH_SERVER_EVENT
			(*it)->receiveServerFullFromMaster(username,serverName,ip,port,full);
	}

	void ClientEventHandler::logoutPlayerMS( const std::string& username )
	{
		DISPATCH_SERVER_EVENT
			(*it)->receiveLogoutPlayerFromMaster(username);
	}

	void ClientEventHandler::fetchFriendServerListMS( const std::string& username,
		const std::vector<std::string>& names, 
		const std::vector<std::string>& ips, const std::vector<int>& ports, 
		const std::vector<int>& capacities, const std::vector<int>& numUsers,
		const std::vector<std::string>& serverNames )
	{
		DISPATCH_SERVER_EVENT
			(*it)->receivedFriendServerListFromMaster(username,
			names,ips,ports,capacities,numUsers,serverNames);
	}

	void ClientEventHandler::lostConnectionCS()
	{
		DISPATCH_SERVER_EVENT
			(*it)->lostConnectionToMaster();
	}

	void ClientEventHandler::connectionFailedCS()
	{
		DISPATCH_SERVER_EVENT
			(*it)->connectionFailedToMaster();
	}

	void ClientEventHandler::isClientAliveSC()
	{
		DISPATCH_SERVER_EVENT
			(*it)->masterConfirmAlive();
	}

}