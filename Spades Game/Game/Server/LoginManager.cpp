#include "Game/Server/LoginManager.hpp"
#include "Game/Utility/SessionGenerator.hpp"
#include "Game/Net/ServerPlayer.hpp"
#include "Game/Utility/StringUtil.hpp"
#include <iostream>
#define MAX_PLAYERS 200
namespace cge
{
	LoginManager::LoginManager(void)
		: m_playerMan(NULL),m_leagueID(-1)
	{
		m_minVersMajorWin = 1;
		m_minVersMajorOsx = 1;
		m_minVersMajorIos = 1;
		m_minVersMinorWin = 0;
		m_minVersMinorOsx = 0;
		m_minVersMinorIos = 0;
	}

	LoginManager::~LoginManager(void)
	{
	}

	void LoginManager::loginRequestC( const std::string& userName, const std::string& password, int majorVersion, int minorVersion, int system )
	{	
		//Log::write() << "[LOGINATTEMPT] " << "Login Request For: " << userName << std::endl; 
		ServerPlayer* p = (ServerPlayer*)getServerClient()->tag();
		std::string uname = userName;
		StringUtil::toLower(uname);

		if(!checkVersion(majorVersion,minorVersion,system)) {
			DISPATCH_SERVER_EVENT
				(*it)->loginResult(userName,p,"update.required",false,false);
		}
		else if(getNumActiveUsers() >= MAX_PLAYERS)
		{
		//	Log::write() << "[LOGINFAIL] " <<  "Server Full: " << userName << std::endl;
			DISPATCH_SERVER_EVENT
				(*it)->loginResult(userName,p,"server.full",false,false);
		}
		else if(p->getUsername() != "")
		{
			//Log::write() << "[LOGINFAIL] "  << "Already logged in: " << userName << std::endl;
			DISPATCH_SERVER_EVENT
			(*it)->loginResult(userName,p,"duplicate.login",false,false);

			//DISPATCH_SERVER_EVENT
			//	(*it)->loginResult(p->getUsername(),p,getPlayerSession(p->getUsername()),true,true);
		}
		else
		{
			int64 pid = 0;
			int result = 0;
			bool success = false;
			bool dbSuccess = m_db->playerLogin(userName,password,&pid,&result);
			std::string answ;

			if(dbSuccess)
			{
				if(result == 1 && m_leagueID != -1)        
				{
					bool isMember = false;
					m_db->leaguePlayerIsMember(m_leagueID,pid,&isMember);
					if(!isMember)
					{
						result = -2;
					}
				}

				if(result == 1)
				{
					m_db->getPlayer(pid,&m_player);
					if(isPlayerLoggedIn(m_player.name))
					{
						answ = "duplicate.user";
					}
					else
					{
						p->setUsername(m_player.name);
						p->setAcceptedTerms(m_player.acceptedTerms);
						p->setId(pid);
						p->setAvatar(m_player.avatar);
						p->setTheme(m_player.theme);
						p->setChatDisabled(m_player.chatIsDisabled);
						p->setSystem(system);
						answ = assignUserSession(m_player.name);
						success = true;
					}

					//else
					//{
					//	answ = "duplicate.user";
					//}
			
				}
				else if(result == -1)
				{
					answ = "login.invalid.username";
				}
				else if(result == -2)
				{
					answ = "login.account.suspended";
				}
				else if(result == -3)
				{
					answ = "login.account.disabled";
				}
				else if(result == -4)
				{
					answ = "login.account.not.verified";
				}
				else if(result == -5)
				{
					answ = "login.invalid.password";
				}
			}
			else
			{
				answ = "database.access.error";
			}
			
//			Log::write() << "[LOGINSUCCESS] "  << "Login Success For: " << userName << std::endl;
			DISPATCH_SERVER_EVENT
				(*it)->loginResult(p->getUsername(),p,answ,success,true);
		}
	}

	bool LoginManager::isPlayerLoggedIn( const std::string& username )
	{
		m_lower = username;
		StringUtil::toLower(m_lower);
		return m_sessionsName.find(m_lower) != m_sessionsName.end();
	}

	std::string LoginManager::assignUserSession( const std::string& user )
	{
		std::string session = SessionGenerator::generate();
		m_lower = user;
		StringUtil::toLower(m_lower);
		//keeping sessions is expensive...
		m_sessionsName[m_lower] = std::make_pair(session,true);
		return session;
	}

	std::string LoginManager::getPlayerSession( const std::string& username )
	{
		m_lower = username;
		StringUtil::toLower(m_lower);

		if(isPlayerLoggedIn(m_lower))
		{
			return m_sessionsName[m_lower].first;
		}

		return std::string();
	}

	bool LoginManager::isPlayerSessionActive( const std::string& player )
	{
		m_lower = player;
		StringUtil::toLower(m_lower);

		if(isPlayerLoggedIn(m_lower))
		{
			return m_sessionsName[m_lower].second;
		}

		return false;
	}

	void LoginManager::setSessionActive( const std::string& player,bool active )
	{
		m_lower = player;
		StringUtil::toLower(m_lower);

		if(isPlayerLoggedIn(m_lower))
		{
			if(!active)
			m_sessionsName.erase(m_lower);
			else
			m_sessionsName[m_lower].second = true;
		}
	}

	void LoginManager::playerLostConnection( const std::string& name )
	{
		setSessionActive(name,false);
	}

	//returns the number off sessions in total
	int LoginManager::getNumActiveUsers()
	{
		return m_sessionsName.size();
	}

	void LoginManager::setPlayerManager( ServerPlayerManager* playerMan )
	{
		m_playerMan = playerMan;
	}

	void LoginManager::setDB( StemwaterDB* db )
	{
		m_db = db;
		onDatabaseSet();
	}

	void LoginManager::onDatabaseSet()
	{
		m_minVersMajorWin = m_db->getIntSetting("min_version_major_win");
		m_minVersMajorOsx = m_db->getIntSetting("min_version_major_osx");
		m_minVersMajorIos = m_db->getIntSetting("min_version_major_ios");

		m_minVersMinorWin = m_db->getIntSetting("min_version_minor_win");
		m_minVersMinorOsx = m_db->getIntSetting("min_version_minor_osx");
		m_minVersMinorIos = m_db->getIntSetting("min_version_minor_ios");
	}

	bool LoginManager::checkVersion( int clientMajor,int clientMinor, int system )
	{
		int minimumMajor = 0;
		int minimumMinor = 0;
		//0 = win, 1 = osx, 2 = ios
		if(system == 0)
		{
			minimumMajor = m_minVersMajorWin;
			minimumMinor = m_minVersMinorWin;
		}
		else if(system == 1)
		{
			minimumMajor = m_minVersMajorOsx;
			minimumMinor = m_minVersMinorOsx;
		}
		else if(system == 2)
		{
			minimumMajor = m_minVersMajorIos;
			minimumMinor = m_minVersMinorIos;
		}
		else
		{
			return false;
		}

		if (clientMajor > minimumMajor) return true;
		else if (clientMajor == minimumMajor &&
			clientMinor >= minimumMinor) return true;
		return false;
	}

	void LoginManager::setLeagueID( int id )
	{
		m_leagueID = id;
	}

}
