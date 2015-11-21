#ifndef CGE_LOGIN_MANAGER_HPP
#define CGE_LOGIN_MANAGER_HPP
#include <stdlib.h>
#include <string>
#include <map>
#include "Game/serverplatform.hpp"
#include "Game/Net/ServerEventProvider.hpp"
#include "Game/Net/NetEventListener.hpp"
#include "Game/Server/ServerPlayerManager.hpp"
#include "Game/Database/StemwaterDB.hpp"
#include "Game/Database/SWDBPlayer.hpp"

namespace cge
{
	class LoginManager : public ServerEventProvider, public NetEventListener
	{
		std::map<std::string,std::pair<std::string,bool> > m_sessionsName;
		std::string assignUserSession(const std::string& user);
		std::string m_lower;
		ServerPlayerManager* m_playerMan;
		StemwaterDB* m_db;
		SWDBPlayer m_player;

		int m_minVersMajorWin;
		int m_minVersMajorOsx;
		int m_minVersMajorIos;

		int m_minVersMinorWin;
		int m_minVersMinorOsx;
		int m_minVersMinorIos;

		int m_leagueID;

		void onDatabaseSet();
		bool checkVersion(int major,int minor, int system);
	public:
		std::string getPlayerSession(const std::string& username);
		void setSessionActive(const std::string& player,bool active);
		bool isPlayerLoggedIn(const std::string& username);
		bool isPlayerSessionActive(const std::string& player);
		virtual void loginRequestC( const std::string& userName, const std::string& password, int majorVersion, int minorVersion, int system );
		virtual void playerLostConnection(const std::string& name);
		int getNumActiveUsers();
		void setPlayerManager(ServerPlayerManager* playerMan);
		void setDB(StemwaterDB* db);
		void setLeagueID(int id);
		LoginManager(void);
		~LoginManager(void);
	};
}

#endif
