#ifndef CGE_MASTER_CORE_HPP
#define CGE_MASTER_CORE_HPP
#include "Game/Net/ServerHost.hpp"
#include "Game/Net/NetEventListener.hpp"
#include "Game/Net/NetEventEncoder.hpp"
#include "Game/Net/NetEventDecoder.hpp"
#include "Game/Net/GameServerManager.hpp"
#include "Game/Database/Database.hpp"
#include "Game/Database/StemwaterDB.hpp"
#include "Game/Net/ServerPeer.hpp"
namespace cge
{
	class MasterCore : public ServerHost, public NetEventListener
	{
		NetEventDecoder m_dec;
		NetEventEncoder m_enc;
		std::string m_helloString;
		GameServerManager m_serverMan;
		std::vector<ServerEntity*> m_deathList;
		Database* m_database;
		StemwaterDB* m_db;
		int m_firstAmount;
	public:
		MasterCore(void);
		virtual void onConnect (ServerPeer* client);
		virtual void onDisconnect (ServerPeer* client);
		virtual void onReceive (ServerPeer* client,const char * data, int size);
		virtual void heartbeat();
		virtual void tick();
		virtual void prerun(int port);
		virtual void postrun();
		virtual void runtick();
		std::string generateClientChallenge();
		bool isClientResponseValid(const std::string& challenge, const std::string& response);
		ServerEntity* getServerEntity();
		virtual void registerServerMC(const std::string& name, const std::string& ip, int port, int capacity, 
			const std::vector<std::string>& currentUsers, const std::string& secretToken, int leagueID);
		virtual void playerLoggedInMC(const std::string& username);
		virtual void playerLoggedOutMC(const std::string& username);
		virtual void fetchLoginServerMC(const std::string& username);
		virtual void fetchServerListMC(const std::string& username);
		virtual void fetchFriendServerListMC(const std::string& username);
		virtual void serverFullMC(const std::string& username, const std::string& ip, int port);
		virtual void createAccountC(const std::string& username, const std::string& password, const std::string& email, const std::string& firstName, const std::string& lastName);
		virtual void isClientAliveSC();
		virtual void logoutC();
		void sendToTarget();
		void sendToAllActive();
		virtual ~MasterCore(void);
	};
}
#endif
