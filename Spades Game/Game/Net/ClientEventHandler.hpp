#ifndef CGE_CLIENT_EVENT_HANDLER_HPP
#define CGE_CLIENT_EVENT_HANDLER_HPP
#include "Game/Net/ServerEventProvider.hpp"
#include "Game/Net/NetEventListener.hpp"
namespace cge
{
	class ClientEventHandler : public ServerEventProvider,
		public NetEventListener
	{
	public:
		ClientEventHandler(void);
		virtual void fetchServerListMS(const std::string& username, const std::vector<std::string>& names, const std::vector<std::string>& ips, const std::vector<int>& ports,
			 const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<int>& numFriends);
		virtual void fetchFriendServerListMS(const std::string& username, const std::vector<std::string>& names,
			const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities,
			const std::vector<int>& numUsers, const std::vector<std::string>& serverNames);
		virtual void serverFullMS(const std::string& username, const std::string& serverName, const std::string& ip, int port, bool full);
		virtual void logoutPlayerMS(const std::string& username);
		virtual void lostConnectionCS();
		virtual void connectionFailedCS();
		virtual void isClientAliveSC();
		virtual ~ClientEventHandler(void);
	};
}
#endif
