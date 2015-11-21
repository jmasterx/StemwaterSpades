#include "Game/Net/MasterCore.hpp"
#include <iostream>
#include "Game/Utility/MD5.hpp"
#include "Game/Utility/StringUtil.hpp"
#include <set>
#include "Game/Utility/DataValidator.hpp"
#define HELLO_RESPONSE_LENGTH 32
#define MAX_PLAYERS 210
#define MAX_DUPE_IP 10
#include <algorithm>
namespace cge
{

	MasterCore::MasterCore(void)
	{
		m_helloString = "000000000";
		m_dec.addListener(this);
		m_firstAmount =  40;

		m_database = new Database("tcp://url:3306","user","password","swspades");
		m_db = new StemwaterDB;
		m_db->setDB(m_database);
		m_firstAmount = m_db->getIntSetting("new_account_amount");
	}

	MasterCore::~MasterCore(void)
	{
		delete m_db;
		delete m_database;
	}

	void MasterCore::onConnect( ServerPeer* client )
	{
		//allocate and create entity
		ServerEntity* p = m_serverMan.createEntity(client->getGUID());

		p->initClient(client->getInterface(),client->getGUID(),client->getAddress());
		//put him into the inactive entities
		m_serverMan.addToInactive(p);

		//set variables and associate entity to client
		p->setEncoder(&m_enc);

		p->getClient()->tag(p);
		std::string ipv4 = p->getClient()->getIP();
		std::cout <<  ipv4 << " connected" << std::endl;
		//m_db->eventAddEntry(-1,DBEVT_NEW_CONNECTION,ipv4);

		p->setIP(ipv4);
	}

	std::string MasterCore::generateClientChallenge()
	{
		std::stringstream ss;
		for(int i = 0; i < 19; ++i)
		{
			ss << rand() % 10;
		}

		ss << "6";
		std::string s = ss.str();

		std::random_shuffle(s.begin(),s.end());

		return s;
	}

	bool MasterCore::isClientResponseValid( const std::string& challenge, const std::string& response )
	{
		std::string responseStr = challenge + m_helloString;
		std::string verifyHash = md5(responseStr);

		return verifyHash == response;
	}

	void MasterCore::onDisconnect( ServerPeer* client )
	{
		ServerEntity* entity = m_serverMan.getEntity(client->getGUID());

		if(entity == NULL)
		{
			return;
		}

		setServerClient(entity->getClient());
		std::string ipv4 = entity->getIP();
		std::cout <<  ipv4 << " disconnected" << std::endl;

		if(entity->isActive())
		{
			entity->setActive(false);
			//DISPATCH_SERVER_EVENT
			//	(*it)->playerLostConnection(entity->getUsername());
		}

		m_serverMan.destroyEntity(entity);
	}

	void MasterCore::createAccountC( const std::string& username, const std::string& password, const std::string& email, const std::string& firstName, const std::string& lastName)
	{
		bool usernameTaken = false;
		bool badUsername = false;
		bool badPassword = false;
		bool badEmail = false;
		bool dbError = false;
		int dbResult = 0;
		int64 dbID = 0;

		DataValidator dv;
		bool botherToDB = true;
		if(!dv.isEmailValid(email))
		{
			botherToDB = false;
			badEmail = true;
		}

		if(!dv.usernameIsValid(username))
		{
			botherToDB = false;
			badUsername = true;
		}

		if(!dv.isPasswordValid(password))
		{
			botherToDB = false;
			badPassword = true;
		}

		if(!dv.isNameValid(firstName,lastName))
		{
			botherToDB = false;
		}

		if(botherToDB)
		{
			dbError = !m_db->playerCreate(firstName,lastName,username,password,email,1,&dbID,&dbResult);
			if(!dbError)
			{
				if(dbResult == 1)
				{
					m_db->playerSetEmailVerified(dbID,true);
				}
				else if(dbResult == -1)
				{
					dbError = true;
				}
				else if(dbResult == -3)
				{
					usernameTaken = true;
				}
				else if(dbResult == -2)
				{
					badEmail = true;
				}
			}
		}

		bool everythingWorked = !dbError && botherToDB &&
			!usernameTaken && !badUsername && !badEmail && !badPassword;

		m_enc.createAccountS(everythingWorked,usernameTaken,badUsername,badPassword,badEmail,false,false,false,false);
		sendToTarget();

		int64 newAmount = 0;
		m_db->playerAddCoins(dbID,m_firstAmount,&newAmount);
	}

	void MasterCore::onReceive( ServerPeer* client,const char * data, int size )
	{
		int dataPos = 0;
		
		ServerEntity* entity = m_serverMan.getEntity(client->getGUID());
		if(entity == NULL)
		{
			return;
		}

			if(!entity->getPacketParser().processData(&data[dataPos],size - dataPos))
			{
				entity->getClient()->close();
				return;
			}

			while(!entity->getPacketParser().empty())
			{
				if(!m_serverMan.guidExists(client->getGUID()))
					return;
				m_dec.dispatchMessage(entity->getPacketParser().getTop(),entity->getClient(),true); //true to bypass login issues
				if(!m_serverMan.guidExists(client->getGUID()))
					return;
				entity->getPacketParser().dequeue();
			}
	}

	void MasterCore::sendToAllActive()
	{
		ServerEntitySetArray* s = m_serverMan.getEntities(true);
		for(ServerEntitySetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			(*it)->send();
		}
	}

	void MasterCore::heartbeat()
	{

	}

	void MasterCore::tick()
	{

	}

	void MasterCore::prerun( int port )
	{
		//m_timer->start(60000);
	}

	void MasterCore::postrun()
	{

	}

	void MasterCore::runtick()
	{

	}

	ServerEntity* MasterCore::getServerEntity()
	{
		return (ServerEntity*)getServerClient()->tag();
	}

	void MasterCore::sendToTarget()
	{
		ServerEntity* p = getServerEntity();
		if(p)
		{
			p->send();
		}
	}

	void MasterCore::registerServerMC( 
		const std::string& name, const std::string& ip, int port, int capacity, 
		const std::vector<std::string>& currentUsers, const std::string& secretToken, int leagueID )
	{
		ServerEntity* e = getServerEntity();
		if(!e->isActive())
		{
			m_serverMan.moveToActive(e);
			e->setName(name);
			e->setIP(ip);
			e->setPort(port);
			e->setCapacity(capacity);
			e->setInitialPlayers(currentUsers);
			e->setLeagueID(leagueID);
		}
	}

	void MasterCore::playerLoggedInMC( const std::string& username )
	{
		ServerEntity* e = getServerEntity();

		if(e->isActive())
		{
			ServerEntitySetArray* s = m_serverMan.getEntities(true);
			for(ServerEntitySetArray::iterator it = s->begin(); it != s->end(); ++it)
			{
				ServerEntity* server = (*it);

				if(e != server && server->playerIsLoggedIn(username))
				{
					m_enc.logoutPlayerMS(username);
					server->send();
				}
			}

			e->playerLoggedIn(username);
		}
	}

	void MasterCore::playerLoggedOutMC( const std::string& username )
	{
		ServerEntity* e = getServerEntity();

		if(e->isActive())
		{
			e->playerLoggedOut(username);
		}
	}

	void MasterCore::fetchLoginServerMC( const std::string& username )
	{
		ServerEntitySetArray* s = m_serverMan.getEntities(true);
		ServerEntity* e = NULL;

		for(ServerEntitySetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			ServerEntity* server = (*it);

			if(server->playerIsLoggedIn(username))
			{
				m_enc.fetchLoginServerMS(username,"","duplicate.user",0);
				sendToTarget();
			}
		}

		for(ServerEntitySetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			ServerEntity* server = (*it);

			if(e == NULL)
			{
				e = server;
			}

			if(!server->isNearlyFull() && server->getNumUsers() > e->getNumUsers() && server->getLeagueID() == -1)
			{
				e = server;
			}
		}

		if(e)
		{
			m_enc.fetchLoginServerMS(username,e->getName(),e->getIP(),e->getPort());
			sendToTarget();
		}
		else // I got nothin' for you
		{
			m_enc.fetchLoginServerMS(username,"","server.offline",0);
			sendToTarget();
		}
	}

	void MasterCore::fetchServerListMC( const std::string& username )
	{
		ServerEntitySetArray* s = m_serverMan.getEntities(true);
		std::vector<std::string> names;
		std::vector<std::string> ips;
		std::vector<int> ports;
		std::vector<int> capacities;
		std::vector<int> users;
		std::vector<int> friends;

		std::set<std::string> friendList;

		int64 pid = -1;
		m_db->playerIdFromName(username,&pid);
		m_db->playerGetFriendNames(pid,friendList);

		for(ServerEntitySetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			ServerEntity* server = (*it);
			bool isInLeague = true;
			if(server->getLeagueID() != -1) {
				m_db->leaguePlayerIsMember(server->getLeagueID(),pid,&isInLeague);
			}
			if(isInLeague)
			{
				std::set<std::string> intersect;
				std::set_intersection(friendList.begin(),friendList.end(),server->getPlayers().begin(),server->getPlayers().end(),
					std::inserter(intersect,intersect.begin()));

				names.push_back(server->getName());
				ips.push_back(server->getIP());
				ports.push_back(server->getPort());
				capacities.push_back(server->getCapacity());
				users.push_back(server->isNearlyFull() ? server->getCapacity() : server->getNumUsers());
				friends.push_back(intersect.size());
			}
		}

		m_enc.fetchServerListMS(username,names,ips,ports,capacities,users,friends);
		sendToTarget();
	}

	void MasterCore::serverFullMC( const std::string& username, const std::string& ip, int port )
	{
		if(getServerEntity()->isActive())
		{
			bool didIt = false;
			ServerEntitySetArray* s = m_serverMan.getEntities(true);
			for(ServerEntitySetArray::iterator it = s->begin(); it != s->end(); ++it)
			{
				ServerEntity* server = (*it);

				if(server->getIP() == ip && server->getPort() == port)
				{
					didIt = true;
					bool full = server->isNearlyFull();

					m_enc.serverFullMS(username,server->getName(),ip,port,full);
					sendToTarget();
					break;
				}
			}

			if(!didIt) //just say it is full if not found
			{
				didIt = true;
				m_enc.serverFullMS(username,"",ip,port,true);
				sendToTarget();
			}
		}
	}

	void MasterCore::logoutC()
	{
		getServerEntity()->getClient()->close();
	}

	void MasterCore::fetchFriendServerListMC( const std::string& username )
	{
		ServerEntitySetArray* s = m_serverMan.getEntities(true);
		std::vector<std::string> names;
		std::vector<std::string> ips;
		std::vector<int> ports;
		std::vector<int> capacities;
		std::vector<int> users;
		std::vector<std::string> serverNames;

		std::set<std::string> friendList;

		int64 pid = -1;
		m_db->playerIdFromName(username,&pid);
		m_db->playerGetFriendNames(pid,friendList);

		for(ServerEntitySetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			ServerEntity* server = (*it);
			bool isInLeague = true;
			if(server->getLeagueID() != -1) {
				m_db->leaguePlayerIsMember(server->getLeagueID(),pid,&isInLeague);
			}

			if(isInLeague)
			{
				std::set<std::string> intersect;
				std::set_intersection(friendList.begin(),friendList.end(),server->getPlayers().begin(),server->getPlayers().end(),
					std::inserter(intersect,intersect.begin()));

				for(std::set<std::string>::iterator it = intersect.begin(); it != intersect.end(); ++it)
				{
					names.push_back(*it);
					ips.push_back(server->getIP());
					ports.push_back(server->getPort());
					capacities.push_back(server->getCapacity());
					users.push_back(server->isNearlyFull() ? server->getCapacity() : server->getNumUsers());
					serverNames.push_back(server->getName());
				}
			}
		}

		m_enc.fetchFriendServerListMS(username,names,ips,ports,capacities,users,serverNames);
		sendToTarget();
	}

	void MasterCore::isClientAliveSC()
	{
		ServerEntity* e = getServerEntity();
		if(e->isActive())
		{
			m_enc.checkClientAliveSC();
			sendToTarget();
		}
	}

}