#ifndef CGE_SERVER_LISTENER_HPP
#define CGE_SERVER_LISTENER_HPP
#include <stdlib.h>
#include <string>
#include <vector>
namespace cge
{
	class ServerPlayer;
	class ServerListener
	{
	public:
		ServerListener(void);
		//from login manager
		virtual void loginResult(const std::string& name, ServerPlayer* player, const std::string& sessionID, bool success, bool newSession){}
		virtual void playerLostConnection(const std::string& name) {}
		virtual void gameCompleted(int tableId) {}
		virtual void receivedServerListFromMaster( 
			const std::string& username, 
			const std::vector<std::string>& names, 
			const std::vector<std::string>& ips, 
			const std::vector<int>& ports, 
			const std::vector<int>& capacities, 
			const std::vector<int>& numUsers, 
			const std::vector<int>& numFriends) {}
		virtual void receivedFriendServerListFromMaster( 
			const std::string& username, 
			const std::vector<std::string>& names, 
			const std::vector<std::string>& ips, 
			const std::vector<int>& ports, 
			const std::vector<int>& capacities, 
			const std::vector<int>& numUsers, 
			const std::vector<std::string>& serverNames) {}
		virtual void receiveServerFullFromMaster(const std::string& username, const std::string& serverName, const std::string& ip, int port, bool full) {}
		virtual void receiveLogoutPlayerFromMaster(const std::string& username) {}
		virtual void playerRequestDoAction(int tableNo, int playerNo) {}
		virtual void playerFinishedDoingAction(int tableNo, int playerNo) {}
		virtual void lostConnectionToMaster() {}
		virtual void connectionFailedToMaster() {}
		virtual void masterConfirmAlive() {}
		virtual ~ServerListener(void);
	};
}

#endif
