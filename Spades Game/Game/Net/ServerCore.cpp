#include "Game/Net/ServerCore.hpp"
#include "Game/Utility/Exception.hpp"
#include <string>
#include <sstream>
#include "Game/Utility/MD5.hpp"
#include <ctime>
#include <string>
#include <fstream>
#include <streambuf>
#include "Game/Utility/DataValidator.hpp"
#include "Game/Utility/StringUtil.hpp"
#include "Game/Handler/HostingConstants.hpp"
#include "Game/Database/SWDBStoreItem.hpp"
#include <cmath>
#define LANG_SEP_CHAR 2
#define HELLO_RESPONSE_LENGTH 32
#define MAX_PLAYERS 210
#define MAX_DUPE_IP 10
#define KARMA_FOR_QUIT -2
#define WEB_URL "http://stemwatergames.com/"

namespace cge
{
	ServerCore::ServerCore( const std::string& dbIP,const std::string& dbUser,const std::string& dbPass,const std::string& masterIP, int masterPort, const std::string& externIP, int leagueID )
		: m_port(0),m_masterIP(masterIP),m_masterPort(masterPort),m_externIP(externIP),m_gameOverTimeout(30),m_leagueID(leagueID)
	{
		//m_database = new Database("tcp://127.0.0.1:3306","user","password","stemwater_spades");
		m_database = new Database(dbIP,dbUser,dbPass,"swspades");
		m_db = new StemwaterDB;
		m_db->setDB(m_database);
		m_layer.setDB(m_db);
		m_helloString = "000000000";
		m_dec.addListener(this);
		m_highStakesMinimum = 0;
		m_stemletCost = 1;

		m_lastChatN = 30;

		m_tickCount = 0;

		m_masterRequestTime = 0;
		m_masterResponseTime = 0;
		m_waitingForMasterResponse = false;
		m_maxMasterRequestTime = 60;
		m_maxMasterResponseTime = 15;

		m_dec.addListener(&m_login);
		m_login.setServerCore(this);
		m_login.setPlayerManager(&m_playerMan);
		m_login.setDB(m_db);
		m_login.setLeagueID(m_leagueID);
		addServerEventProvider(&m_login);
		addServerEventProvider(this);

		//create the tables
		m_tableMan.createTables(1,100,this);

		m_leaderboardSize = m_db->getIntSetting("leaderboard_size");

		if(m_leaderboardSize == 0)
			m_leaderboardSize = 25;

		m_highStakesTimeout = 42;
		m_highStakesTimeoutShow = 42;
		m_normalTimeout = 42;
		m_normalTimeoutShow = 42;

		generateFeeArray();
		generatePaymentArrays();
		generateCoinAwardRateArrays();
		generateTimeouts();

		srand(time(NULL));

		m_dailyBonusHours = 24.0f;
		m_dailyBonusAmount = 0;

		m_giftMan = new ServerGiftManager("gifts.gft");

		m_clientCore = new ClientCore();
		m_clientCore->setDecoder(&m_clientDec);
		m_clientEnc.setServerClient(m_clientCore);
		addServerEventProvider(&m_clientHandler);
		m_clientDec.addListener(&m_clientHandler);

		m_subResponseTimeLimit = 90;
	}

	ServerCore::~ServerCore()
	{
		delete m_clientCore;
		delete m_giftMan;
		delete m_db;
		delete m_database;
	}

	void ServerCore::onConnect( ServerPeer* client )
	{
		std::string ipv4 = client->getIP();
		std::cout << "Connected " << ipv4 << std::endl; 
		//m_db->eventAddEntry(-1,DBEVT_NEW_CONNECTION,ipv4);

		//allocate and create player
		ServerPlayer* p = m_playerMan.createPlayer(client->getGUID());
		//put him into the inactive players
		m_playerMan.addToInactive(p);

		//set variables and associate player to client
		p->initClient(client->getInterface(),client->getGUID(),client->getAddress());
		p->setEncoder(&m_enc);

		p->getClient()->tag(p);

		p->setIP(ipv4);

		setServerClient(p->getClient());

		if(m_playerMan.getAllocedPlayers()->size() >= MAX_PLAYERS)
		{
			client->close();
			return;
		}

#ifndef CGE_DEV

		int ipCount = m_playerMan.getNumIPs(ipv4);
		if(ipCount >= MAX_DUPE_IP)
		{
			client->close();
			return;
		}
		else if(ipCount > 1)
		{
			p->setHasDuplicateIP(true);
		}
#endif
	
	}

	void ServerCore::onDisconnect( ServerPeer* client )
	{
		ServerPlayer* player = m_playerMan.getPlayer(client->getGUID());
		if(!player)
			return;
		ServerPeer* peer = player->getClient();
		setServerClient(peer);
		ServerPlayer* orig = getServerPlayer();
		std::string ipv4 = peer->getIP();

		std::cout << "Disconnected " << ipv4 << std::endl; 

		if(player && player->getId() > -1)
		{
			std::stringstream ss;
			ss << "s="<<player->getSystem() << ";" << "ip=" << player->getIP();
			m_db->eventAddEntry(player->getId(),DBEVT_DISCONNECTION,ss.str());
		}
		else
		{
			//m_db->eventAddEntry(-1,DBEVT_DISCONNECTION,ipv4);
		}

		if(player == NULL)
		{
			return;
		}

		setServerClient(peer);
		
		if(player->isActive())
		{
			player->setActive(false);
			DISPATCH_SERVER_EVENT
				(*it)->playerLostConnection(player->getUsername());

			if(player->isSitting())
			{
				ServerTable* t = m_tableMan.getTableAt(player->getTable());
				if(t && t->isInProgress())
				{
					playerAbandonedGameC(false,true,false);
					//playerAbandonedGameC();
				}
				else if(t)
				{
					if(t->isBeingSetup())
					{
						canceledHostingRequestC();
					}
					else
					{
						playerStoodUpC();
					}
				}
			}

			if(player->isWatching())
			{
				watchTableC(-1);
			}

			m_enc.playerLeftS(player->getUsername());
			sendToActiveNotTarget();
		}

		//master server needs to know
		m_clientEnc.playerLoggedOutMC(player->getUsername());
		m_playerMan.destroyPlayer(player);
		client->tag(NULL);

		if(orig != player && orig != NULL)
		setServerClient(orig->getClient());
	}

	void ServerCore::onReceive( 
		ServerPeer* client,const char * data, int size )
	{		
		int dataPos = 0;
		ServerPlayer* player = m_playerMan.getPlayer(client->getGUID());
		if(player == NULL)
		{
			return;
		}

		if(!player->getPacketParser().processData(&data[dataPos],size - dataPos))
		{
			player->getClient()->close();
			return;
		}
	
		while(!player->getPacketParser().empty())
		{
			if(!m_playerMan.guidExists(client->getGUID()))
				return;
			m_dec.dispatchMessage(player->getPacketParser().getTop(),player->getClient(),player->isLoggedIn());

			if(!m_playerMan.guidExists(client->getGUID()))
				return;
			player->getPacketParser().dequeue();
		}
	}

	void ServerCore::sendChatMessageC( const std::string& message,ChatAreaEnum area )
	{
		ServerPlayer* player = getServerPlayer();

		if(player->chatAbuseDetected() && area == LOBBY_CHAT)
		{
			return;
		}

		bool couldChat = player->canChat();
		
		if(area == LOBBY_CHAT)
		player->attemptSendChatMessage();

		if(area == LOBBY_CHAT && !couldChat && !player->canChat())
			return;

		if((!player->canChat() || player->chatAbuseDetected()) && area == LOBBY_CHAT)
		{
			if(!player->canChat())
			m_enc.sendChatMessageS("SERVER",player->getUsername() + "'s chat has been disabled for 3 minutes due to flooding the lobby chat.",area);
			else
			m_enc.sendChatMessageS("SERVER",player->getUsername() + "'s chat has been disabled due to  due to flooding the lobby chat.",area);

			if(area == LOBBY_CHAT)
			{
				if(player->isChatDisabled())
					sendToTarget();
				else
					sendToAllActive();
			}
			else
			{
				if(!player->isChatDisabled())
				{
					if(player->getTable() != -1)
						sendToTable(player->getTable());
					else if(player->getWatchTable() != -1)
						sendToTable(player->getWatchTable());
				}
				else
				{
					sendToTarget();
				}
			}
		}
		else if(player->canChat()  || area != LOBBY_CHAT)
		{
			//if(area != LOBBY_CHAT)
			{
				m_enc.sendChatMessageS(player->getUsername(),message,area);

				std::string place;
				if(area == GAME_CHAT)
					place = "game";
				else if (area == LOBBY_CHAT)
					place = "lobby";
				else
					place = "table";
				m_db->chatAddEntry(player->getId(),place,message);
				if(area == LOBBY_CHAT)
				{
					if(player->isChatDisabled())
						sendToTarget();
					else
						sendToAllActive();
				}
				else
				{
					if(!player->isChatDisabled())
					{
						if(player->getTable() != -1)
							sendToTable(player->getTable());
						else if(player->getWatchTable() != -1)
							sendToTable(player->getWatchTable());
					}
					else
					{
						sendToTarget();
					}
				}
			}
		}
		else // spam detected
		{
			/*
			m_enc.sendChatMessageS("SERVER","SPAM DETECTED...WAIT 1 MIN TO CHAT, DO NOT SEND MORE MESSAGES OR YOU WILL BE FORCED TO RESTART",area);
			sendToTarget();
			*/
		}
	}

	void ServerCore::requestTableC( int tableNo, int seatNo )
	{

		ServerPlayer* player = getServerPlayer();

		if(player->isSitting())
		{
			return;
		}

		//pressed host button, find and seat him
		if(tableNo == -1)
		{
			ServerTable* emptyT = m_tableMan.findEmptyTable();

			if(!emptyT)
			{
				m_enc.tableLockedS(tableNo,seatNo,false);
				sendToTarget();
				return;
			}
			else
			{
				tableNo = emptyT->getTableNo();
				seatNo = 0;
			}
		}

		ServerTable* t = m_tableMan.getTableAt(tableNo);
		bool tableInUse = false;
		if(t && (t->isActive() || t->isBeingSetup()))
		{
			tableInUse = true;
		}
		else if(!t)
		{
			tableInUse = true;
		}


		if(player->isAtTable() || tableInUse)
		{
			m_enc.tableLockedS(tableNo,seatNo,false);
			sendToTarget();
			return;
		}

		//update coin amount
		notifyCoinAmountChanged(player,true,false,false);

		m_enc.tableBeingSetupS(tableNo);
		sendToAllActive();

		t->setBeingSetup(true);
		player->setTable(tableNo);
		player->setSeat(seatNo);
		m_enc.tableLockedS(tableNo,seatNo,true);
		sendToTarget();
	}

	void ServerCore::hostGameC( const SpadesGameInfo& inf )
	{
		SpadesGameInfo info = inf;
	
		ServerPlayer* player = getServerPlayer();
		ServerTable* t = m_tableMan.getTableAt(player->getTable());

		if(!t)
			return;
		if(!t->isBeingSetup())
			return;

		int64 numCoins = 0;
		if(inf.getFee() > 0)
		{
			m_db->playerGetCoinAmount(player->getId(),&numCoins);
		}

		if(numCoins < inf.getFee())
		{
			canceledHostingRequestC();
			showPaymentDialog(player);
			return;
		}

		info.setThemeID(player->getTheme());

		int curRating = 0;
		if(inf.getMinRating() > 0)
		{
			m_db->playerGetRating(player->getId(),&curRating);
		}

		computeMinRating(info,curRating);

		if(!validateGame(info))
		{
			logoutC();
			return;
		}

		//reassign him because you cannot sit there
		if(player->getSeat() == 3 && info.getNumPlayers() < 4)
		{
			player->setSeat(0);
		}

		if(info.isBotGame())
		{
			player->setSeat(0);
		}

		t->setInfo(info);
		t->setActive(true);
		t->setBeingSetup(false);
		
		t->sitPlayer(player->getSeat(),player);

		ServerPlayerSetArray* s = m_playerMan.getPlayers(true);
		for(ServerPlayerSetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			m_enc.tableStateChangedS(player->getTable(),generateInfoForPlayer(info,(*it)));
			(*it)->send();
		}

		m_enc.playerSatDownS(player->getUsername(),player->getTable(),player->getSeat());
		sendToAllActive();

		if(info.isBotGame())
		{
			t->setInProgress(true);
			gameHasStarted(t);

			t->getGame().sendGameInfo(t->getInfo());
			m_enc.enterGameS(generateInfoForPlayer(info,getServerPlayer()));
			sendToTarget();
		}
	}

	void ServerCore::playerRequestSitC( int tableNumber, int seatNumber, bool returning )
	{
		ServerTable* t = m_tableMan.getTableAt(tableNumber);
		if(!t)
		{
			return;
		}
		else
		{
			ServerChair* chair = t->getChairAt(seatNumber);
			if(chair)
			{
				if(chair->isOccupied())
				{
					m_enc.sendMessageS("sub.seat.taken.title","sub.seat.taken.text",true,true);
					sendToTarget();
					return;
				}
			}

			for(int i = 0; i < t->getNumChairs(); i++)
			{
#ifndef CGE_DEV

				if(t->getChairAt(i)->isOccupied() && t->getChairAt(i)->getPlayer()->getIP() == getServerPlayer()->getIP())
				{
					m_enc.sendMessageS("msg.duplicate.ip","msg.duplicate.ip.text",true,true);
					sendToTarget();
					return;
				}
#endif

				if(t->isPlayerKicked(getServerPlayer()))
				{
					m_enc.sendMessageS("msg.cannot.rejoin","msg.cannot.rejoin.kick.text",true,true);
					sendToTarget();
					return;
				}
				else if(t->getChairAt(i)->getOriginalPid() == getServerPlayer()->getId() && !returning)
				{
					m_enc.sendMessageS("msg.cannot.rejoin","msg.cannot.rejoin.text",true,true);
					sendToTarget();
					return;
				}
			}
		}

		ServerPlayer* player = getServerPlayer();
		if(player->isSitting())
		{
			return;
		}

		if(player->isWatching())
		{
			return;
		}
		ServerChair* chair = t->getChairAt(seatNumber);

		if(!chair)
			return;

		if(!t->isInProgress())
		{
			if(!canPlayerAffordTable(t,player))
			{
				showPaymentDialog(player);
				m_enc.sendMessageS("not.enough.coins.join.title","not.enough.coins.join.text",true,true);
				sendToTarget();
				return;
			}
		}
		else if(chair->isSubSeat() && !returning) {
			int64 amount = 0;
			m_db->playerGetCoinAmount(player->getId(),&amount);
			int64 reqAmt = t->getInfo().getFee();
			if(amount < m_feeArray[0]) {
				std::stringstream args;
				args << StringUtil::commaSeparate(StringUtil::toString(m_feeArray[0])) << (char)LANG_SEP_CHAR << 
					StringUtil::commaSeparate(StringUtil::toString(amount)) << (char)LANG_SEP_CHAR << StringUtil::commaSeparate(StringUtil::toString(m_feeArray[0]))
					<< (char)LANG_SEP_CHAR << StringUtil::commaSeparate(StringUtil::toString(m_feeArray[0]));
				m_enc.requestSubS(tableNumber,seatNumber,args.str(),0,0,0);
				sendToTarget();
				return;
			}
		}

		if(t->getInfo().getMinRating() > 0 && !returning)
		{
			int curRating = -1;
			m_db->playerGetRating(player->getId(),&curRating);

			if(curRating >= 0 && curRating < t->getInfo().getMinRating())
			{
				m_enc.sendMessageS("min.rating.required.title","min.rating.required.text",true,true);
				sendToTarget();
				return;
			}
		}

		player->setSeat(seatNumber);
		player->setTable(tableNumber);

		m_enc.playerSatDownS(player->getUsername(),player->getTable(),player->getSeat());
		sendToAllActive();

		t->sitPlayer(seatNumber,player);

		if(chair)
		{
			if(chair->isSubSeat())
			{
				m_enc.seatIsSubSeatS(tableNumber,seatNumber,false);
				sendToAllActive();
				chair->setAsSubSeat(false);
				if(!returning)
				{
					m_enc.chatMessageFromServerS("joined.sub",player->getUsername(),true,GAME_CHAT);
					sendToTable(t->getTableNo());
				}
			//	m_enc.chatMessageFromServerS("thanks.sub",player->getUsername(),true,GAME_CHAT);
			//	sendToTarget();
			}
		}

		if(t->isReadyToPlay())
		{
			if(t->isInProgress())
			{
				t->clearSubWaitList();
				bool didSomething = false;
				do 
				{
					didSomething = false;

					for(int i = 0; i < t->getNumChairs(); ++i)
					{
						if(t->getChairAt(i)->isOccupied())
						{
							if(t->getGame().clientReconnected(i))
								didSomething = true;
						}
					}

				} while (didSomething);

				int gifts[4];
				int frames[4];

				for(int i = 0; i < t->getNumChairs(); ++i)
				{
					ServerChair* c = t->getChairAt(i);
					if(c)
					{
						gifts[i] = c->getGiftData().giftID;
						frames[i] = c->getGiftData().frame;
					}
					else
					{
						gifts[i] = -1;
						frames[i] = -1;
					}
				}
				for(int i = 0; i < t->getNumChairs(); ++i)
				{
					if(t->getChairAt(i)->isOccupied())
					{
						m_enc.sendClientStateS(t->getGame().generateState(i,false,gifts,frames));
						t->getChairAt(i)->getPlayer()->send();
					}
				}
			}	
			
			//make everyone pay up
			if(!t->isInProgress())
			{
				bool paymentSuccess = verifyAndProcessTableFee(t);

				if(!paymentSuccess)
				{
					if(t->isReadyToPlay())
					{
						//database error because no one was kicked
						standTableUp(t);
					}
					else
					{
						//someone was kicked... keep waiting
					}

					return;
				}
			}

			if(!t->isInProgress())
			{
				t->getGame().sendGameInfo(t->getInfo());

				//tell the watchers that we are starting another
				m_enc.readyToPlayAgainS();
				for(int i = 0; i < t->getNumWatchers(); ++i)
				{
					t->getWatcher(i)->send();
				}
			}
			
			if(!t->isInProgress())
			{
				t->setInProgress(true);
				gameHasStarted(t);
			}

			for(int i = 0; i < t->getNumChairs(); ++i)
			{
				if(t->getChairAt(i)->isOccupied())
				{
						m_enc.enterGameS(generateInfoForPlayer(t->getInfo(),
							t->getChairAt(i)->getPlayer()));
					t->getChairAt(i)->getPlayer()->send();
				}
			}
			if(returning)
			{
				m_db->playerSetTempCoins(getServerPlayer()->getId(),t->getInfo().getFee());

				//m_enc.disconnectionReturnedS(false);
				//sendToTable(t->getTableNo());
				m_enc.chatMessageFromServerS("player.has.returned",getServerPlayer()->getUsername(),true,GAME_CHAT);
				sendToTable(t->getTableNo());
			}
		}
	}

	void ServerCore::playerRequestSitC( int tableNumber, int seatNumber )
	{
		playerRequestSitC(tableNumber,seatNumber,false);
	}

	void ServerCore::playerStoodUpC(bool swapping)
	{
		ServerPlayer* player = getServerPlayer();

		m_enc.playerStoodUpS(player->getTable(),player->getSeat());
		sendToAllActive();

		ServerTable* t = m_tableMan.getTableAt(player->getTable());

		if(t && t->isInProgress())
		{
			ServerChair* chair = t->getChairAt(player->getSeat());

			sendGiftRemove(t->getTableNo(),player->getSeat());

			if(chair && !t->isWaitListActive() && !swapping)
			{
				chair->setAsSubSeat(true);
				m_enc.seatIsSubSeatS(player->getTable(),player->getSeat(),true);
				sendToAllActive();
			}
		}
		
		if(player->isSitting())
		{
			ServerTable* t = m_tableMan.getTableAt(player->getTable());
			if(t)
			{
				if(!swapping)
				{
					m_enc.playerAbandonedGameS(player->getUsername());
					sendToTable(t->getTableNo());
				}

				int seat = player->getSeat();

				t->standPlayer(seat);
				if(t->isEmpty())
				{
					bool isBotGame = t->getInfo().isBotGame();
					punishDisconnectedPlayersForTable(t);
					t->setActive(false);
					t->setInProgress(false);
					m_enc.tableIsInactiveS(player->getTable());
					sendToAllActive();

					std::vector<ServerPlayer*> watchertmp;
					//kick out the watchers
					for(int i = 0; i < t->getNumWatchers(); ++i)
					{
						watchertmp.push_back(t->getWatcher(i));
					}

					for(size_t i = 0; i < watchertmp.size(); ++i)
					{
						setServerClient(watchertmp[i]->getClient());
						watchTableC(-1);

						if(isBotGame)
						{
							m_enc.sendMessageS("game.ended.title","game.ended.text",true,true);
							sendToPlayer(watchertmp[i]);
						}
					}
				}
				else if(t && t->getNumOccupants() == t->getInfo().getNumPlayers() - 1 && t->isInProgress() && !swapping)
				{
					m_enc.requestPlayerS(t->getTableNo(),"server");
					sendToAllActive();
					t->resetRequestTimeout();
				}

				if(t->isWaitListActive())
				{
					t->verifyWaiter(player->getUsername(),false);
					m_enc.chatMessageFromServerS("play.again.no",player->getUsername(),true,GAME_CHAT);
					sendToTable(t->getTableNo());
					if(t->waitListDone())
					{
						t->clearWaitList();
					}
				}
			}
		
			setServerClient(player->getClient());
			player->standUp();

			if(swapping)
			{
				m_enc.disconnectionReturnedS(true);
				sendToTarget();
			}
		}
	}

	void ServerCore::playerStoodUpC()
	{
		playerStoodUpC(false);
	}

	void ServerCore::gameMessageC( SpadesCSMessageEnum message, const std::vector<int>& params )
	{
		ServerPlayer* p = getServerPlayer();
		if(p->getTable() != -1)
		{
			m_tableMan.getTableAt(p->getTable())->getGame().
				flushBotCache();
			m_tableMan.getTableAt(p->getTable())->getGame().
				sendMessageToGame(SpadesGameMessage(message,params,p->getSeat()));
			m_tableMan.getTableAt(p->getTable())->getGame().
				flushBotCache();
		}
		
	}

	void ServerCore::modifyListC( const std::string& player, GroupTypeEnum group, bool add )
	{
		ServerPlayer* p = getServerPlayer();
		if(!p)
			return;
		int64 id = -1;
		if(group == FRIENDS_GROUP)
		{
			if(!m_db->playerIdFromName(player,&id))
				return;
			if(id == -1)
				return;

			if(add)
			{
				m_db->playerAddFriend(p->getId(),id);
			}
			else
			{
				m_db->playerRemoveFriend(p->getId(),id);
			}

			m_db->playerGetFriendNames(p->getId(),p->getFriends());
		}
		if((group == FRIENDS_GROUP &&playerIsOnline(id) && id != -1) 
			|| group != FRIENDS_GROUP)
		{
			m_enc.modifiedListS(player,group,add);
			p->send();
		}
	}

	void ServerCore::canceledHostingRequestC()
	{
		ServerPlayer* p = getServerPlayer();
		m_enc.tableIsInactiveS(p->getTable());
		ServerTable* t = m_tableMan.getTableAt(p->getTable());

		//unlock the table for further use
		if(t)
		{
			t->setActive(false);
			t->setBeingSetup(false);
		}
		sendToAllActive();
		p->standUp();
	}

	void ServerCore::loginResult( const std::string& name, ServerPlayer* player,
		const std::string& sessionID, bool success, bool newSession )
	{
		if(!success)
		{
			m_enc.loginResultS(false,sessionID,"");
			sendToTarget();
			return;
		}
		player->setSessionID(sessionID);
		//he needs friends
		m_db->playerGetFriendNames(player->getId(),player->getFriends());
		player->setActive(success);
		m_playerMan.moveToActive(player);

		m_enc.loginResultS(success,sessionID,player->getUsername());
		sendToTarget();
		//master server needs to know
		m_clientEnc.playerLoggedInMC(player->getUsername());

		std::stringstream ss;
		ss << "s="<<player->getSystem() << ";" << "ip=" << player->getIP();
		m_db->eventAddEntry(player->getId(),DBEVT_NEW_CONNECTION,ss.str());
	}

	void ServerCore::readyToReceiveLobbyDataC()
	{
		//send stuff to player
		ServerPlayer* player = getServerPlayer();

		if(!player)
		{
			return;
		}

		//ask player to create the tables
		m_enc.beginSendLobbyDataS(m_tableMan.getTableStart(),m_tableMan.getNumTables());
		sendToTarget();

		ServerTableArray* tables = m_tableMan.getTables();

		//send the table infos
		for(size_t i = 0; i < tables->size(); ++i)
		{
			if(tables->at(i)->isActive())
			{
				m_enc.tableStateChangedS(tables->at(i)->getTableNo(),
					generateInfoForPlayer(tables->at(i)->getInfo(),getServerPlayer()));
				sendToTarget();
			}
			else
			{
				m_enc.tableIsInactiveS(tables->at(i)->getTableNo());
				sendToTarget();
				if(tables->at(i)->isBeingSetup())
				{
					m_enc.tableBeingSetupS(tables->at(i)->getTableNo());
					sendToTarget();
				}
			}

			for(int j = 0; j < tables->at(i)->getNumChairs(); ++j)
			{
				if(tables->at(i)->getChairAt(j)->isSubSeat())
				{
					m_enc.seatIsSubSeatS(tables->at(i)->getTableNo(),j,true);
				}
				sendToTarget();
			}
		}

		ServerPlayerSetArray* s = m_playerMan.getPlayers(true);
		//send him the table infos
		for (ServerPlayerSetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			//if((*it) != player)
			{
				GroupTypeEnum  g = IN_LOBBY_GROUP;
				//client will use this to sit him
				m_enc.playerJoinedS(*(*it)->getSharedPlayer(),g);
				sendToTarget();
				if(player->isFriend((*it)->getUsername()))
				{
					m_enc.modifiedListS((*it)->getUsername(),FRIENDS_GROUP,true);
					sendToTarget();
				}
			}
		}

		GroupTypeEnum  g = IN_LOBBY_GROUP;
		m_enc.playerJoinedS(*player->getSharedPlayer(),g);
		sendToActiveNotTarget();

		for(ServerPlayerSetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			if(player != (*it))
			{
				if((*it)->isFriend(player->getUsername()))
				{
					m_enc.modifiedListS(player->getUsername(),FRIENDS_GROUP,true);
					(*it)->send();
				}
			}	
		}

		notifyCoinAmountChanged(player,false,false,false);

		/*
		int64 amountReceived = 0;
		bool giveSuccess = m_db->playerTryReceiveDailyBonus(
			player->getId(),m_dailyBonusAmount,m_dailyBonusHours,&amountReceived);

		//he got a daily bonus
		if(giveSuccess && amountReceived > 0)
		{
			notifyCoinAmountChanged(player,true,false,false);
		}
		*/

		sendFeeOptionsToTarget();

		sendLobbyLoginMessage();
/*
		std::vector<SWDBChatMessage> msgs;
		m_db->chatFromLastN(m_lastChatN,msgs);
		for(int i = 0; i < msgs.size(); i++)
		{
			m_enc.sendChatMessageS(msgs[i].username,msgs[i].message,LOBBY_CHAT);
			sendToTarget();
		}
		*/

		if(!player->hasAcceptedTerms())
		{
			m_enc.playerMustAcceptTermsS("1. Terms\n\nBy accessing this web site, you are agreeing to be bound by these \nweb site Terms and Conditions of Use, all applicable laws and regulations, \nand agree that you are responsible for compliance with any applicable local \nlaws. If you do not agree with any of these terms, you are prohibited from \nusing or accessing this site. The materials contained in this web site are \nprotected by applicable copyright and trade mark law.\n\n2. Use License\nPermission is granted to temporarily download one copy of the materials (information or software) on Stemwater Games's web site for personal, non-commercial transitory viewing only. This is the grant of a license, not a transfer of title, and under this license you may not:\n\n*modify or copy the materials;\n*use the materials for any commercial purpose, or for any public display (commercial or non-commercial);\n*attempt to decompile or reverse engineer any software contained on Stemwater Games's web site;\n*remove any copyright or other proprietary notations from the materials; or\n*transfer the materials to another person or \"mirror\" the materials on any other server.\n\nThis license shall automatically terminate if you violate any of these restrictions and may be terminated by Stemwater Games at any time. Upon terminating your viewing of these materials or upon the termination of this license, you must destroy any downloaded materials in your possession whether in electronic or printed format.\n\n3. Disclaimer\n\nThe materials on Stemwater Games's web site are provided \"as is\". Stemwater Games makes no warranties, expressed or implied, and hereby disclaims and negates all other warranties, including without limitation, implied warranties or conditions of merchantability, fitness for a particular purpose, or non-infringement of intellectual property or other violation of rights. Further, Stemwater Games does not warrant or make any representations concerning the accuracy, likely results, or reliability of the use of the materials on its Internet web site or otherwise relating to such materials or on any sites linked to this site.\n\n\n4. Limitations\n\nIn no event shall Stemwater Games or its suppliers be liable for any damages (including, without limitation, damages for loss of data or profit, or due to business interruption,) arising out of the use or inability to use the materials on Stemwater Games's Internet site, even if Stemwater Games or a Stemwater Games authorized representative has been notified orally or in writing of the possibility of such damage. Because some jurisdictions do not allow limitations on implied warranties, or limitations of liability for consequential or incidental damages, these limitations may not apply to you.\n\n5. Revisions and Errata\n\nThe materials appearing on Stemwater Games's web site could include technical, typographical, or photographic errors. Stemwater Games does not warrant that any of the materials on its web site are accurate, complete, or current. Stemwater Games may make changes to the materials contained on its web site at any time without notice. Stemwater Games does not, however, make any commitment to update the materials.\n\n6. Links\n\nStemwater Games has not reviewed all of the sites linked to its Internet web site and is not responsible for the contents of any such linked site. The inclusion of any link does not imply endorsement by Stemwater Games of the site. Use of any such linked web site is at the user's own risk.\n\n7. Site Terms of Use Modifications\n\nStemwater Games may revise these terms of use for its web site at any time without notice. By using this web site you are agreeing to be bound by the then current version of these Terms and Conditions of Use.\n\n8. Governing Law\nAny claim relating to Stemwater Games's web site shall be governed by the laws of the Province of Quebec without regard to its conflict of law provisions.\nGeneral Terms and Conditions applicable to Use of a Web Site.\n\n9. Server Rules and Terms of Use\n\nBy accessing the game's servers and signing in with a username and password, you agree to be bound by the following:\n\n*Stemwater Games reserves the right to alter or terminate any account without notice; This includes accounts that have made one or more in-app purchases;\n*Stemwater Games is not responsible for in-app purchases that are not credited correctly or that are not credited at all;\n*Stemwater Games will not refund in-app purchases under any circumstance;\n*Stemwater Games is not responsible for any losses directly or indirectly related to server errors, server bugs, server inaccuracies, disconnections, connection problems and anything else not covered;\n*Stemwater Games reserves the right to take its servers offline at any time for any reason without refunding any in-app purchases;\n*Stemwater Games reserves the right to alter usernames that are offensive or inappropriate;\n*Stemwater Games reserves the right to ban any individual and their accounts without reason or motive;\n*Stemwater Games servers are governed by the same terms as stemwatergames.com Web Site.\n\n\nPrivacy Policy\n\nYour privacy is very important to us. Accordingly, we have developed this Policy in order for you to understand how we collect, use, communicate and disclose and make use of personal information. The following outlines our privacy policy.\n\n*Before or at the time of collecting personal information, we will identify the purposes for which information is being collected.\n*We will collect and use of personal information solely with the objective of fulfilling those purposes specified by us and for other compatible purposes, unless we obtain the consent of the individual concerned or as required by law.\n*We will only retain personal information as long as necessary for the fulfillment of those purposes. \n*We will collect personal information by lawful and fair means and, where appropriate, with the knowledge or consent of the individual concerned. \n*Personal data should be relevant to the purposes for which it is to be used, and, to the extent necessary for those purposes, should be accurate, complete, and up-to-date. \n*We will protect personal information by reasonable security safeguards against loss or theft, as well as unauthorized access, disclosure, copying, use or modification.\n*We will make readily available to customers information about our policies and practices relating to the management of personal information. \n\nWe are committed to conducting our business in accordance with these principles in order to ensure that the confidentiality of personal information is protected and maintained.");
			sendToTarget();
		}
		else if(player->getAvatar() == 1)
		{
			m_enc.setAvatarS();
			sendToTarget();
		}
		else
		{
			checkPlayerDisconnection();
		}
	}

	void ServerCore::sendToTarget()
	{
		ServerPlayer* p = getServerPlayer();
		if(p)
		{
			p->send();
		}
	}

	void ServerCore::sendToAllActive()
	{
		ServerPlayerSetArray* s = m_playerMan.getPlayers(true);
		for(ServerPlayerSetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			(*it)->send();
		}
	}

	void ServerCore::sendToActiveNotTarget()
	{
		ServerPlayerSetArray* s = m_playerMan.getPlayers(true);
		ServerPlayer* p = getServerPlayer();
		for(ServerPlayerSetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			if(p != (*it))
			{
				(*it)->send();
			}	
		}
	}

	void ServerCore::playerAbandonedGameC()
	{
		playerAbandonedGameC(false,false,false);
	}

	void ServerCore::playerAbandonedGameC( bool timeout, bool disconnected, bool unsubbing )
	{
		int tableNo = getServerPlayer()->getTable();
		ServerTable* t = m_tableMan.getTableAt(tableNo);
		ServerPlayer* him = getServerPlayer();
		if(t)
		{
			if(!t->isInProgress())
			{
				playerStoodUpC();
				return;
			}

			bool foundHim = false;
			if(!t->getInfo().isBotGame() && t->isInProgress())
			{
				for(int i = 0; i < t->getNumChairs(); i++)
				{
					if(t->getChairAt(i)->getPlayer() != NULL && t->getChairAt(i)->getOriginalPid() == getServerPlayer()->getId())
					{
						foundHim = true;
						break;
					}
				}

				for(int i = 0; i < t->getNumChairs(); i++)
				{
					//when someone leaves, the current player gets full time to play
					if(t->getChairAt(i)->isTurn())
						t->getChairAt(i)->turnStarted();

					if(t->getChairAt(i)->getPlayer() == him) {
						bool playing = t->getChairAt(i)->isTurn();
						t->getChairAt(i)->resetTurnData();
						if(playing)
							t->getChairAt(i)->turnStarted();
					}
				}

				if(t->getNumOccupants() > 1)
				{
					if(!unsubbing)
					{
						if(!disconnected)
							m_layer.punishQuitter(getServerPlayer()->getId(),KARMA_FOR_QUIT,t->getInfo(),t,!foundHim);
						else
							t->addDisconnectedPlayer(him->getId(),him->getSeat());
					}
				}
				else
				{
					foundHim = false;
				}

				if(t->getNumOccupants() == t->getInfo().getNumPlayers() && t->isSubListActive())
				{
					t->clearSubWaitList();
				}

				if(t->isSubListActive())
				{
					t->verifySubWaiter(getServerPlayer()->getUsername(),true);
					subWaitlistChanged(t);
				}
				else if(t->getNumOccupants() == 1)
				{
					subWaitlistChanged(t);
				}

				if(!t->isPlayerKicked(him) && !unsubbing)
				{
					if(foundHim)
					{
						if(disconnected)
						m_enc.chatMessageFromServerS("left.disconnected",getServerPlayer()->getUsername(),true,GAME_CHAT);
						else
						m_enc.chatMessageFromServerS("left.quit",getServerPlayer()->getUsername(),true,GAME_CHAT);
					}
					else
					{
						m_enc.chatMessageFromServerS("left.sub",getServerPlayer()->getUsername(),true,GAME_CHAT);
					}

					sendToTable(t->getTableNo());
				}
			}

			playerStoodUpC(false);

			if(!unsubbing)
			{
				if(t->getNumOccupants() > 0 && t->getNumOccupants() == t->getInfo().getNumPlayers() - 1 && !t->isPlayerKicked(getServerPlayer()))
				{
					for(size_t i = 0; i < t->getNumChairs(); i++)
					{
						ServerChair* ch = t->getChairAt(i);

						if(ch && ch->isOccupied() && ch->getPlayer() != getServerPlayer())
						{
							if(foundHim) {
								if(disconnected)
									m_enc.sendMessageS("player.disconnect.title","player.disconnect.text",true,true);
								else
									m_enc.sendMessageS("player.abandon.title","player.abandon.text",true,true);
							}
							else
								m_enc.sendMessageS("sub.abandon.title","sub.abandon.text",true,true);

							sendToPlayer(ch->getPlayer());
						}
					}
				}

				if(foundHim)
				{
					if(t->isPlayerKicked(him))
					{
						if(!timeout)
							m_enc.sendMessageS("you.kicked.title","you.kicked.text",true,true);
						else
							m_enc.sendMessageS("you.kicked.timeout.title","you.kicked.timeout.text",true,true);
					}
					else
					{
						m_enc.sendMessageS("abandoned.game.title","abandoned.game.text",true,true);
					}
					sendToTarget();
				}
				else if(t->isPlayerKicked(getServerPlayer()))
				{
					if(!timeout)
						m_enc.sendMessageS("you.kicked.title","you.kicked.text",true,true);
					else
						m_enc.sendMessageS("you.kicked.timeout.title","you.kicked.timeout.text",true,true);

					sendToTarget();
				}
			}

			clearPlayerTempCoins(him);

			if(unsubbing)
			{
				m_enc.sendMessageS("player.returned.title","player.returned.text",true,true);
				sendToTarget();
			}
		}
	}

	void ServerCore::playerAbandonedGameC( bool timedOut )
	{
		playerAbandonedGameC(timedOut,false,false);
	}

	void ServerCore::sendInviteC( int tableNo, const std::string& player )
	{
		ServerTable* t = m_tableMan.getTableAt(getServerPlayer()->getTable());
		ServerPlayer* c = m_playerMan.getActivePlayer(player);
		
		if(t && (t->needsSub() || !t->isInProgress()) && t->getNumOccupants() > 0 
			&& t->getNumOccupants() < t->getInfo().getNumPlayers() && c && !c->isAtTable())
		{
			m_enc.sendInviteS(tableNo,getServerPlayer()->getUsername());
			c->send();
		}
	}

	void ServerCore::sendToTable( int tableNo )
	{
		ServerTable* t = m_tableMan.getTableAt(tableNo);
		if(!t)
		{
			return;
		}

		for(int i = 0; i < t->getNumChairs(); ++i)
		{
			ServerChair* c = t->getChairAt(i);

			if(c && c->isOccupied())
			{
				c->getPlayer()->send();
			}
		}

		for(int i = 0; i < t->getNumWatchers(); ++i)
		{
			ServerPlayer* p = t->getWatcher(i);
			if(p)
			{
				p->send();
			}
		}
	}

	void ServerCore::watchTableC( int tableNo )
	{
		ServerPlayer* player = getServerPlayer();

		if(tableNo == -1 && player->isWatching())
		{
			ServerTable* t = m_tableMan.getTableAt(player->getWatchTable());
			player->setWatchTable(-1);
			m_enc.playerWatchingTableS(tableNo,player->getUsername());
			sendToAllActive();

			if(t)
			{
				t->removeWatcher(player);
			}

			m_enc.chatMessageFromServerS("not.watching",player->getUsername(),true,GAME_CHAT);
			sendToTable(t->getTableNo());

			return;
		}

		ServerTable* t = m_tableMan.getTableAt(tableNo);

		if(!t)
		{
			return;
		}

		if((!t->isInProgress() && !t->isWaitListActive()) || !t->getInfo().isWatchingAllowed() || player->isWatching())
		{
			return;
		}

		for(int i = 0; i < t->getNumChairs(); i++)
		{
#ifndef CGE_DEV

			if(t->getChairAt(i)->isOccupied() && t->getChairAt(i)->getPlayer()->getIP() == getServerPlayer()->getIP())
			{
				m_enc.sendMessageS("msg.duplicate.ip","msg.duplicate.ip.watch.text",true,true);
				sendToTarget();
				return;
			}
#endif

			if(t->isPlayerKicked(player))
			{
				m_enc.sendMessageS("msg.cannot.rejoin","msg.cannot.rejoin.watch.kick.text",true,true);
				sendToTarget();
				return;
			}
			/*
			else if(t->getChairAt(i)->getOriginalPid() == getServerPlayer()->getId())
			{
				m_enc.sendMessageS("msg.cannot.rejoin","msg.cannot.rejoin.watch.text",true,true);
				sendToTarget();
				return;
			}
			*/

		}

		t->addWatcher(player);
		player->setWatchTable(tableNo);

		int gifts[4];
		int frames[4];

		for(int i = 0; i < t->getNumChairs(); ++i)
		{
			ServerChair* c = t->getChairAt(i);
			if(c)
			{
				gifts[i] = c->getGiftData().giftID;
				frames[i] = c->getGiftData().frame;
			}
			else
			{
				gifts[i] = -1;
				frames[i] = -1;
			}
		}

		m_enc.playerWatchingTableS(tableNo,player->getUsername());
		sendToAllActive();

		m_enc.sendClientStateS(t->getGame().generateState(0,true,gifts,frames));
		sendToTarget();

		m_enc.enterGameS(generateInfoForPlayer(t->getInfo(),getServerPlayer()));
		sendToTarget();

		m_enc.chatMessageFromServerS("now.watching",player->getUsername(),true,GAME_CHAT);
		sendToTable(t->getTableNo());
	}

	void ServerCore::gameCompleted( int tableId )
	{
		ServerTable* t = m_tableMan.getTableAt(tableId);
		if(!t)
		{
			//uh ohs :O
			return;
		}

		std::vector<int> winners;
		std::vector<int> scores;
		std::vector<std::string> names;
		std::vector<int64> ids;
		std::vector<int64> oldids;
		std::vector<int> ratings;
		std::vector<int64> netChange;
		std::vector<int64> newAmount;

		winners = t->getGame().getWinningTeams();
		scores = t->getGame().getWinningScores();
		for(int i = 0; i < t->getNumChairs();++i)
		{
			if(t->getChairAt(i)->isOccupied())
			{
				names.push_back(t->getChairAt(i)->getPlayer()->getUsername());
				ids.push_back(t->getChairAt(i)->getPlayer()->getId());
				oldids.push_back(t->getChairAt(i)->getOriginalPid());
			}
			else if(t->getInfo().isBotGame())
			{
				names.push_back("cyborg");
			}
		}

		for(int i = 0; i < t->getNumChairs();++i)
		{
			if(t->getChairAt(i)->isOccupied())
			{
				ServerPlayer* pl = t->getChairAt(i)->getPlayer();
				clearPlayerTempCoins(pl);
			}
		}

		if(!t->getInfo().isBotGame())
		{
			m_layer.gameIsOver(t,winners,scores,ids,oldids,ratings);
			m_db->getDB()->endTransaction(false);

			int subAmount = 0;
			if(t->getInfo().getFee() > 0)
				subAmount = m_feeArray[0];

			m_layer.awardCoins(t,winners,ids, oldids,subAmount,netChange,newAmount,
				m_individualCoinAwardRate,
				m_cutthroatCoinAwardRate,
				m_partnershipCoinAwardRate,m_houseFee);
			m_db->getDB()->endTransaction(false);
		}
		
		//for now
		if(ratings.empty())
		for(int i = 0; i < names.size(); ++i)
		{
			ratings.push_back(0);
		}

		if(netChange.empty())
		for(int i = 0; i < names.size(); ++i)
		{
			netChange.push_back(0);
			newAmount.push_back(0);
		}

		punishDisconnectedPlayersForTable(t);

		t->setInProgress(false);
		t->getGame().restart();
		t->setInProgress(false);
		t->generateWaitList();


		int sendCount = 0;
		for(int i = 0; i < t->getNumChairs();++i)
		{
			if(t->getChairAt(i)->isOccupied())
			{
				ServerPlayer* pl = t->getChairAt(i)->getPlayer();
				if(sendCount < netChange.size())
				{
					m_enc.gameOverS(winners,scores,ratings,names,netChange[sendCount],newAmount[sendCount],m_gameOverTimeout);
					pl->send();
				}
				else
				{
					m_enc.gameOverS(winners,scores,ratings,names,0,0,m_gameOverTimeout);
					pl->send();
				}
				notifyCoinAmountChanged(pl,true,false,false);
				sendCount++;
			}
		}

		for(int i = 0; i < t->getNumWatchers(); ++i)
		{
			ServerPlayer* p = t->getWatcher(i);
			if(p)
			{
				m_enc.gameOverS(winners,scores,ratings,names,0,0,m_gameOverTimeout);
				p->send();
			}
		}

		//remove gifts from table
		for(int i = 0; i < t->getNumChairs(); i++)
		{
			ServerChair* ch = t->getChairAt(i);
			SpadesClientWrapper* ww = t->getGame().getPlayerAtPos(i);
			if(ch && ww && !ww->isNull())
			{
				sendGiftRemove(t->getTableNo(),i);
			}
		}
	}

	void ServerCore::playerWantsPlayAgainC()
	{
		ServerPlayer* p = getServerPlayer();

		ServerTable* t = m_tableMan.getTableAt(p->getTable());

		if(t && t->isWaitListActive())
		{
			if(!canPlayerAffordTable(t,p))
			{
				playerStoodUpC();
				showPaymentDialog(p);
				m_enc.sendMessageS("not.enough.coins.join.title","not.enough.coins.join.text",true,true);
				sendToTarget();
				return;
			}

			t->verifyWaiter(p->getUsername(),true);

			m_enc.chatMessageFromServerS("play.again.yes",p->getUsername(),true,GAME_CHAT);
			sendToTable(t->getTableNo());

			if(t->waitListDone())
			{
				bool playAgain = t->allWantToPlayAgain();
				t->clearWaitList();

				if(playAgain)
				{
					if(!t->getInfo().isBotGame() && !verifyAndProcessTableFee(t))
					{
						if(t->isReadyToPlay())
						{
							//database error
							standTableUp(t);
						}
						else
						{
							//someone was kicked
						}

						return;
					}

					t->setInProgress(true);
					gameHasStarted(t);
					m_enc.readyToPlayAgainS();
					sendToTable(t->getTableNo());

					//bots must send joined message
					if(t->getInfo().isBotGame())
					{
						t->getGame().setBots(t->getInfo());
					}
				}
				else if(t->getNumOccupants() == t->getInfo().getNumPlayers())
				{
					t->setInProgress(true);

					for(int i = 0; i < t->getNumChairs(); ++i)
					{
						if(t->getChairAt(i)->isOccupied())
						{
								m_enc.enterGameS(generateInfoForPlayer(t->getInfo(),t->getChairAt(i)->getPlayer()));
							t->getChairAt(i)->getPlayer()->send();
						}
					}	

					//tell the watchers that we are starting another
					m_enc.readyToPlayAgainS();
					for(int i = 0; i < t->getNumWatchers(); ++i)
					{
						t->getWatcher(i)->send();
					}
				}
			}
		}
	}

	std::string ServerCore::generateClientChallenge()
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

	bool ServerCore::isClientResponseValid( const std::string& challenge, const std::string& response )
	{
		std::string responseStr = challenge + m_helloString;
		std::string verifyHash = md5(responseStr);

		return verifyHash == response;
	}

	void ServerCore::heartbeat()
	{
		m_enc.checkClientAliveSC();
		sendToAllActive();
	}

	void ServerCore::logoutC()
	{
		ServerPlayer* p = getServerPlayer();
		if(p)
		{
			if(p->isAtTable())
			{
				ServerTable* t = m_tableMan.getTableAt(p->getTable());
				if(t && t->isInProgress())
				{
					playerAbandonedGameC();
				}
			}

			p->getClient()->close();
		}
	}

	void ServerCore::sendLobbyLoginMessage()
	{
		std::ifstream t("message.mess");

		if(!t.is_open())
		{
			return;
		}

		std::string str;

		t.seekg(0, std::ios::end);   
		str.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());

		if(str.length() > 0)
		{
			m_enc.chatMessageFromServerS(str,"",false,LOBBY_CHAT);
			sendToTarget();
		}
	}

	void ServerCore::sendChatTypingC( ChatAreaEnum area )
	{
		ServerPlayer* player = getServerPlayer();
		if(player->chatAbuseDetected())
		{
			return;
		}

		if(player->canChat())
		{
			m_enc.sendChatTypingS(player->getUsername(),area);

			if(area == LOBBY_CHAT)
			{
				sendToAllActive();
			}
			else
			{
				if(player->getTable() != -1)
					sendToTable(player->getTable());
				else if(player->getWatchTable() != -1)
					sendToTable(player->getWatchTable());
			}
		}
	}

	void ServerCore::showProfileC( const std::string& player )
	{
		int64 pid = -1;
		m_db->playerIdFromName(player,&pid);
		if(pid == -1)
		{
			return;
		}

		if(!m_db->getPlayer(pid,&m_player))
		{
			return;
		}

		std::string resetDate;
		std::string email;
		if(!(m_player.lastRatingResetDate == boost::none))
		{
			resetDate = m_player.lastRatingResetDate->dateToString();
		}
		if(m_player.showEmail)
		{
			email = m_player.email;
		}

		m_profileVec.clear();
		int avatar = m_player.avatar;
		m_profileVec.push_back(m_player.firstName);
		m_profileVec.push_back(m_player.lastName);
		m_profileVec.push_back(m_player.name);
		m_profileVec.push_back(email);
		m_profileVec.push_back(m_player.joinDate.dateToString());
		m_profileVec.push_back(StringUtil::toString(m_player.coins));
		m_profileVec.push_back(StringUtil::toString(m_player.karmaPoints));
		m_profileVec.push_back(StringUtil::toString(m_player.lifetimeRating));
		m_profileVec.push_back(StringUtil::toString(m_player.lifetimeHighestRating));
		m_profileVec.push_back(StringUtil::toString(m_player.lifetimeWins));
		m_profileVec.push_back(StringUtil::toString(m_player.lifetimeLoses));
		m_profileVec.push_back(StringUtil::toString(m_player.lifetimeGamesCompleted));
		m_profileVec.push_back(StringUtil::toString(m_player.lifetimeGamesAbandoned));
		m_profileVec.push_back(StringUtil::toString(m_player.lifetimeGamesSubstituted));
		m_profileVec.push_back(StringUtil::toString(m_player.currentRating));
		m_profileVec.push_back(StringUtil::toString(m_player.currentHighestRating));
		m_profileVec.push_back(StringUtil::toString(m_player.currentWins));
		m_profileVec.push_back(StringUtil::toString(m_player.currentLoses));
		m_profileVec.push_back(StringUtil::toString(m_player.currentGamesCompleted));
		m_profileVec.push_back(StringUtil::toString(m_player.currentGamesAbandoned));
		m_profileVec.push_back(StringUtil::toString(m_player.currentGamesSubstituted));
		m_profileVec.push_back(StringUtil::toString(m_player.ratingResetCount));
		m_profileVec.push_back(resetDate);
		m_profileVec.push_back(StringUtil::toString(m_player.firstPlaceSingleCount));
		m_profileVec.push_back(StringUtil::toString(m_player.secondPlaceSingleCount));
		m_profileVec.push_back(StringUtil::toString(m_player.thirdPlaceSingleCount));
		m_profileVec.push_back(StringUtil::toString(m_player.fourthPlaceSingleCount));
		m_profileVec.push_back(StringUtil::toString(m_player.firstPlaceCutthroatCount));
		m_profileVec.push_back(StringUtil::toString(m_player.secondPlaceCutthroatCount));
		m_profileVec.push_back(StringUtil::toString(m_player.thirdPlaceCutthroatCount));
		m_profileVec.push_back(StringUtil::toString(m_player.partnershipWinCount));
		m_profileVec.push_back(StringUtil::toString(m_player.partnershipLossCount));
		m_profileVec.push_back(StringUtil::toString(m_player.pass2WinCount));
		m_profileVec.push_back(StringUtil::toString(m_player.pass2LossCount));

		m_enc.showProfileS(avatar,m_profileVec);
		sendToTarget();
	}

	void ServerCore::playerAcceptedTermsC(bool accepted)
	{
		ServerPlayer* p = getServerPlayer();
		if(!p)
			return;

		if(!p->hasAcceptedTerms() && accepted)
		{
			m_db->playerAcceptedTerms(p->getId());
			p->setAcceptedTerms(true);

			if(p->getAvatar() == 1)
			{
				m_enc.setAvatarS();
				sendToTarget();
			}
		}
	}

	void ServerCore::sendToPlayer( ServerPlayer* p )
	{
		p->send();
	}

	void ServerCore::gameHasStarted( ServerTable* table )
	{
		if(table->getInfo().isBotGame())
		{
			table->setGameId(-1);
			return;
		}

		table->setGameId(m_layer.generateDBGame(table->getInfo()));
	}

	void ServerCore::tick()
	{
		m_tickCount++;
		for(ServerTableArray::iterator it = m_tableMan.begin(); it != m_tableMan.end(); ++it)
		{
			if(m_tickCount % 30 == 0) {
				bool found = false;
				if((*it)->isBeingSetup())
				{
					ServerPlayerSetArray* s = m_playerMan.getPlayers(true);
					for(ServerPlayerSetArray::iterator itp = s->begin(); itp != s->end(); ++itp)
					{
						if((*itp)->getTable() == (*it)->getTableNo())
						{
							found = true;
						}
					}

					if(!found)
					{
						m_enc.tableIsInactiveS((*it)->getTableNo());
						//unlock the table for further use
						(*it)->setActive(false);
						(*it)->setBeingSetup(false);
						sendToAllActive();
					}
				}
			}
			if((*it)->needsSub() || (!(*it)->isInProgress() && (*it)->isActive()))
			{
				(*it)->increaseRequestTimeout(1.0f);
			}
			if((*it)->needsSub())
			{
				(*it)->increaseSubTimeout(1.0f);

				//we wait 2 minutes for a sub before prompting them to annul
				if((*it)->getSubTimeout() >= 120.0f && !(*it)->isSubListActive())
				{
					m_enc.waitForSubS(false);
					sendToTableNotWatchers((*it)->getTableNo());
					(*it)->resetSubTimeout();
					(*it)->resetSubResponseTimeout();
					(*it)->generateSubList();
				}
				else if((*it)->isSubListActive())
				{
					(*it)->increaseSubResponseTimeout(1.0f);

					if((*it)->getSubResponseTimeout() >= m_subResponseTimeLimit)
					{
						subWaitlistChanged((*it));
					}
				}
			}

			if((*it)->isReadyToPlay())
			{
				(*it)->getGame().increaseTime(1.0f);

				if((*it)->getInfo().isBotGame())
				{
					for(size_t i = 0; i < (*it)->getInfo().getNumPlayers(); i++)
					{
						ServerChair* c = (*it)->getChairAt(i);

						if(c)
						{
							bool needsChange = c->updateBotGift(1.0f);

							if(needsChange)
							{
								const ServerGiftData* gd = &c->getGiftData();

								if(gd)
								{
									GiftData* d = m_giftMan->getGiftById(gd->giftID);

									if(d)
									{
										if(gd->frame + 1 < d->getNumFrames())
										{
											updateGift((*it),i);
										}
										else
										{
											sendGiftRemove((*it)->getTableNo(),i);
										}
									}
								}
							}
						}
					}
				}
				else
				{
					for(size_t i = 0; i < (*it)->getInfo().getNumPlayers(); i++)
					{
						ServerChair* c = (*it)->getChairAt(i);

						if(c && c->isOccupied() && c->isTurn())
						{
							c->increaseTurnTime(1.0f);

							if(getPlayerTimeoutTime((*it)->getInfo()) - c->getTurnTime() == getPlayTimeoutShowTime((*it)->getInfo()))
							{
								if(!c->sentFirstStrike()) {
								sendStrike((*it),i,c->getTurnStrikes(),getPlayerTimeoutTime((*it)->getInfo()) - c->getTurnTime() + 1,true);
								c->sendFirstStrike();
								}
							}

							if(c->getTurnTime() >= getPlayerTimeoutTime((*it)->getInfo()))
							{
								c->resetTurnTime();
								c->addTurnStrike();

								if(c->getTurnStrikes() < 3)
								sendStrike((*it),i,c->getTurnStrikes(),getPlayerTimeoutTime((*it)->getInfo()) - c->getTurnTime() + 1,false);

								if(c->getTurnStrikes() == 1)
								{
									m_enc.chatMessageFromServerS("strikes.1",
										c->getPlayer()->getUsername() + (char)LANG_SEP_CHAR + c->getPlayer()->getUsername(),
										true,GAME_CHAT);
									sendToTable((*it)->getTableNo());
								}
								else if(c->getTurnStrikes() == 2)
								{
									m_enc.chatMessageFromServerS("strikes.2",
										c->getPlayer()->getUsername() + (char)LANG_SEP_CHAR + c->getPlayer()->getUsername(),
										true,GAME_CHAT);
									sendToTable((*it)->getTableNo());
								}
								else if(c->getTurnStrikes() >= 3)
								{
									doKickPlayer(*it,c->getPlayer(),true);
									c->resetTurnData();
									c->turnStarted();
								}
							}
						}
					}
				}

				if((*it)->getInfo().isTimeEnded())
				{
					m_enc.gameTimeChangedS((int)(*it)->getGame().getTimeElapsed());
					sendToTable((*it)->getTableNo());
				}
			}
		}

		if(m_clientCore->connected())
		{
			if(!m_waitingForMasterResponse)
			{
				m_masterRequestTime++;
				if(m_masterRequestTime == m_maxMasterRequestTime)
				{
					m_masterRequestTime = 0;
					m_masterResponseTime = 0;
					m_waitingForMasterResponse = true;
					m_clientEnc.checkClientAliveSC();
				}
			}
			else
			{
				m_masterResponseTime++;
				if(m_masterResponseTime == m_maxMasterResponseTime)
				{
					m_masterResponseTime = 0;
					m_waitingForMasterResponse = false;
					m_masterRequestTime = 0;
					m_clientCore->disconnect();
				}
			}
		}
	}

	void ServerCore::sendToTableNotSubs( int tableNo )
	{
		ServerTable* t = m_tableMan.getTableAt(tableNo);
		if(!t)
		{
			return;
		}

		for(int i = 0; i < t->getNumChairs(); ++i)
		{
			ServerChair* c = t->getChairAt(i);

			if(c && c->isOccupied() && c->getPlayer()->getId() == c->getOriginalPid())
			{
				c->getPlayer()->send();
			}
		}
	}

	void ServerCore::waitForSubC( bool stay )
	{
		ServerPlayer* p = getServerPlayer();

		ServerTable* t = m_tableMan.getTableAt(p->getTable());

		if(t && t->isSubListActive())
		{
			t->verifySubWaiter(p->getUsername(),!stay);
			subWaitlistChanged(t);
		}
	}

	void ServerCore::subWaitlistChanged( ServerTable* t )
	{
		ServerPlayer* p = getServerPlayer();
		if(t->allWantToAnnulGame() || t->getSubResponseTimeout() >= m_subResponseTimeLimit)
		{
			t->clearSubWaitList();
			m_enc.waitForSubS(true);
			sendToTableNotSubs(t->getTableNo());
			t->resetSubTimeout();
			t->resetSubResponseTimeout();
			m_tempList = getTablePlayers(t->getTableNo());
			for(int i = 0; i < t->getNumChairs(); ++i)
			{
				ServerChair* c = t->getChairAt(i);

				if(c && c->isOccupied())
				{
					setServerClient(c->getPlayer()->getClient());
					refundPlayerTempCoins(getServerPlayer());
					notifyCoinAmountChanged(getServerPlayer(),true,false,false);
					playerStoodUpC();
				}
			}

			m_enc.sendMessageS("game.annuled.title","game.annuled.text",true,true);
			for(int i = 0; i < m_tempList.size(); i++)
			{
				sendToPlayer(m_tempList[i]);
			}
			m_tempList.clear();
		}
		else if (t->anyWantToSubWait())
		{
			t->clearSubWaitList();
			m_enc.waitForSubS(true);
			sendToTableNotSubs(t->getTableNo());
			t->resetSubTimeout();
			t->resetSubResponseTimeout();
		}

		setServerClient(p->getClient());
	}

	std::vector<ServerPlayer*> ServerCore::getTablePlayers( int tableNo )
	{
		std::vector<ServerPlayer*> p;

		ServerTable* t = m_tableMan.getTableAt(tableNo);
		if(!t)
		{
			return p;
		}

		for(int i = 0; i < t->getNumChairs(); ++i)
		{
			ServerChair* c = t->getChairAt(i);

			if(c && c->isOccupied())
			{
				p.push_back(c->getPlayer());
			}
		}

		for(int i = 0; i < t->getNumWatchers(); ++i)
		{
			ServerPlayer* pl = t->getWatcher(i);
			if(pl)
			{
				p.push_back(pl);
			}
		}

		return p;
	}

	void ServerCore::kickPlayerC( const std::string& playerName )
	{
		if(!getServerPlayer())
			return;
		ServerPlayer * p = getServerPlayer();
		ServerTable* t = m_tableMan.getTableAt(p->getTable());
		if(!t)
			return;

		ServerPlayer* player = t->getPlayerFromName(playerName);
		if(!player)
			return;
		ServerPlayer* kicker = t->getPlayerFromName(getServerPlayer()->getUsername());
		if(!kicker)
			return;

		if(t->isPlayerKicked(player))
			return;

		if(t->hasVoteBeenCastAgainstPlayer(player->getId(),kicker->getId()))
		{
			m_enc.chatMessageFromServerS("player.vote.already",player->getUsername(),true,GAME_CHAT);
			sendToTarget();
		}
		else if(t->getGame().canKickPlayer(player->getSeat()))
		{
			t->castVoteAgainstPlayer(player->getId(),kicker->getId());
			std::string concat;
			concat += kicker->getUsername();
			concat += LANG_SEP_CHAR;
			concat += player->getUsername();
			m_enc.chatMessageFromServerS("player.vote.kick",concat,true,GAME_CHAT);
			sendToTable(t->getTableNo());
		}
		else
		{
			m_enc.chatMessageFromServerS("player.cannot.kick",player->getUsername(),true,GAME_CHAT);
			sendToTarget();
		}

		if(t->shouldPlayerBeKicked(player->getId()))
		{
			doKickPlayer(t,player,false);
		}
	}

	void ServerCore::sendToTableNotWatchers( int tableNo )
	{
		ServerTable* t = m_tableMan.getTableAt(tableNo);
		if(!t)
		{
			return;
		}

		for(int i = 0; i < t->getNumChairs(); ++i)
		{
			ServerChair* c = t->getChairAt(i);

			if(c && c->isOccupied())
			{
				c->getPlayer()->send();
			}
		}
	}

	void ServerCore::sendToTableOnlyWatchers( int tableNo )
	{
		ServerTable* t = m_tableMan.getTableAt(tableNo);
		if(!t)
		{
			return;
		}

		for(int i = 0; i < t->getNumWatchers(); ++i)
		{
			ServerPlayer* p = t->getWatcher(i);
			if(p)
			{
				p->send();
			}
		}
	}

	bool ServerCore::validateGame( const SpadesGameInfo& info )
	{
		if(info.isBotGame() && info.isRated())
			return false;
		if(info.isPassTwo() && info.isMirror())
			return false;
		if(info.isScoreEnded() && !(
			info.getEndValue() == HOSTING_SCORE_0 ||
			info.getEndValue() == HOSTING_SCORE_1 ||
			info.getEndValue() == HOSTING_SCORE_2 ||
			info.getEndValue() == HOSTING_SCORE_3 ||
			info.getEndValue() == HOSTING_SCORE_4
			))
			return false;
		/*
		if(info.isTimeEnded() && !(
			info.getEndValue() == HOSTING_TIME_0 ||
			info.getEndValue() == HOSTING_TIME_1 ||
			info.getEndValue() == HOSTING_TIME_2 ||
			info.getEndValue() == HOSTING_TIME_3 ||
			info.getEndValue() == HOSTING_TIME_4
			))
			return false;
		if(info.isHandEnded() && !(
			info.getEndValue() == HOSTING_HAND_0 ||
			info.getEndValue() == HOSTING_HAND_1 ||
			info.getEndValue() == HOSTING_HAND_2 ||
			info.getEndValue() == HOSTING_HAND_3 ||
			info.getEndValue() == HOSTING_HAND_4
			))
			return false;
			*/

		if(info.isRated() && info.isScoreEnded() && info.getEndValue() == HOSTING_SCORE_0)
			return false;
		/*
		if(info.isRated() && info.isTimeEnded() && info.getEndValue() == HOSTING_TIME_0)
			return false;
		if(info.isRated() && info.isHandEnded() && info.getEndValue() == HOSTING_HAND_0)
			return false;
			*/

	//	if(!info.isScoreEnded() && !info.isHandEnded() && !info.isTimeEnded())
		if(!info.isScoreEnded())
			return false;

		if(info.isDeckCutEnabled())
			return false;

		if(info.isBotGame() && info.getFee() != 0)
			return false;

		if(!info.isBotGame())
		{
			bool has = false;

			for(int i = 0; i < m_feeArray.size(); i++)
			{
				if(m_feeArray[i] == info.getFee())
				{
					has = true;
				}
			}

			if(info.getFee() == 0 && info.isSocial())
				has = true;

			if(!has)
				return false;
		}

		return true;
	}

	void ServerCore::requestAccountSettingsC()
	{
		ServerPlayer* p = getServerPlayer();
		if(!p)
			return;
		if(!m_db->getPlayer(p->getId(),&m_player))
			return;

		m_enc.requestAccountSettingsS(m_player.firstName,m_player.lastName,m_player.showEmail);
		sendToTarget();
	}

	void ServerCore::requestChangePasswordC( const std::string& oldPassword, const std::string& newPassword )
	{
		DataValidator dv;

		if(!dv.isPasswordValid(newPassword))
		{
			m_enc.sendMessageS("setting.saved.failed.title","account.invalid.password",true,true);
			sendToTarget();
		}
		else
		{
			int result = -1;
			bool success = m_db->playerChangePassword(getServerPlayer()->getId(),oldPassword,newPassword,&result);

			if(!success)
			{
				m_enc.sendMessageS("setting.saved.failed.title","setting.saved.failed.text",true,true);
			}
			else if(result == 1)
			{
				m_enc.sendMessageS("setting.saved.title","setting.saved.password",true,true);
			}

			else if(result == -1)
			{
				m_enc.sendMessageS("setting.saved.failed.title","setting.failed.old.password",true,true);
			}

			sendToTarget();

			if(result == 1)
			{
				m_enc.accountSettingsChangedS(1);
				sendToTarget();
			}
		}
	}

	void ServerCore::requestChangeEmailC( const std::string& newEmail )
	{
		DataValidator dv;
		if(!dv.isEmailValid(newEmail))
		{
			m_enc.sendMessageS("setting.saved.failed.title","email.invalid.validate",true,true);
			sendToTarget();
		}
		else
		{
			int result = -1;
			bool success = m_db->playerChangeEmail(getServerPlayer()->getId(),newEmail,&result);

			if(!success)
			{
				m_enc.sendMessageS("setting.saved.failed.title","email.invalid.validate",true,true);
				sendToTarget();
			}
			else if(result != 1)
			{
				m_enc.sendMessageS("setting.saved.failed.title","account.invalid.email",true,true);
				sendToTarget();
			}
			else
			{
				m_enc.sendMessageS("setting.saved.title","setting.saved.email",true,true);
				sendToTarget();

				m_enc.accountSettingsChangedS(2);
				sendToTarget();
			}
		}
	}

	void ServerCore::requestResetRatingC()
	{
		if(!m_db->getPlayer(getServerPlayer()->getId(),&m_player))
		{
			m_enc.sendMessageS("setting.saved.failed.title","setting.saved.failed.text",true,true);
			sendToTarget();
		}
		else
		{
			bool canDoIt = true;
			if(!(m_player.lastRatingResetDate == boost::none))
			{
				DateTime dt = m_player.lastRatingResetDate.get_value_or(DateTime::now());
				DateTime now = DateTime::now();
				DateTimeDuration dd = now.timeBetween(dt);
				if(dd.days() < 7)
					canDoIt = false;
			}

			if(!canDoIt)
			{
				m_enc.sendMessageS("setting.saved.failed.title","setting.failed.rating",true,true);
				sendToTarget();
			}
			else
			{
				if(!m_db->playerResetRating(getServerPlayer()->getId()))
				{
					m_enc.sendMessageS("setting.saved.failed.title","setting.saved.failed.text",true,true);
					sendToTarget();
				}
				else
				{
					m_enc.sendMessageS("setting.saved.title","setting.saved.rating",true,true);
					sendToTarget();
				}
			}
		}
	}

	void ServerCore::requestChangeShowEmailC( bool show )
	{
		bool success = m_db->playerSetShowEmail(getServerPlayer()->getId(),show);

		if(!success)
		{
			m_enc.sendMessageS("setting.saved.failed.title","setting.saved.failed.text",true,true);
		}
		else
		{
			m_enc.sendMessageS("setting.saved.title","setting.saved.generic",true,true);
		}
		sendToTarget();
	}

	void ServerCore::requestChangeFullNameC( const std::string& first, const std::string& last )
	{
		DataValidator dv;

		if(!dv.isNameValid(first,last))
		{
			m_enc.sendMessageS("setting.saved.failed.title","setting.saved.failed.text",true,true);
			sendToTarget();
		}
		else
		{
		  bool success = m_db->playerSetName(getServerPlayer()->getId(),first,last);

		  if(!success)
		  {
			  m_enc.sendMessageS("setting.saved.failed.title","setting.saved.failed.text",true,true);
		  }
		  else
		  {
			  m_enc.sendMessageS("setting.saved.title","setting.saved.name",true,true);
		  }
		  sendToTarget();

		  if(success)
		  {
			  m_enc.accountSettingsChangedS(0);
			  sendToTarget();
		  }
		}
	}

	void ServerCore::changeAvatarC( int newAvatar )
	{
		if(newAvatar > 4 && newAvatar <= 117)
		{
			bool firstAvatar = getServerPlayer()->getAvatar() == 1;

			m_db->playerSetAvatar(getServerPlayer()->getId(),newAvatar);
			getServerPlayer()->setAvatar(newAvatar);
			m_enc.changeAvatarS(newAvatar,getServerPlayer()->getUsername());
			sendToAllActive();

			if(firstAvatar)
			{
				m_enc.showThemeDialogS(true);
				sendToTarget();
			}
		}
	}

	void ServerCore::showLeaderboardC( int pageNo, int numPerPage, const std::string& dataColumn, bool alreadyShowing )
	{
		std::vector<std::string> rank;
		std::vector<std::string> name;
		std::vector<std::string> rating;

		std::string destCol;

		if(dataColumn == "leaderboard.rating")
			destCol = "PlayerLifetimeRating";
		else if(dataColumn == "leaderboard.coins")
			destCol = "PlayerCoins";
		else if(dataColumn == "leaderboard.games.completed")
			destCol = "PlayerLifetimeGamesCompleted";
		else if(dataColumn == "leaderboard.games.abandoned")
			destCol = "PlayerLifetimeGamesAbandoned";
		else if(dataColumn == "leaderboard.games.substituted")
			destCol = "PlayerLifetimeGamesSubstituted";

		if(destCol.length() > 0)
		{
			int pg = 1;
			int numPg = m_leaderboardSize;
			bool success = m_db->getLeaderboardData(rank,name,rating,pg,numPg,-1,destCol);
			m_enc.showLeaderboardS(pg,numPg,rank,name,rating,dataColumn,alreadyShowing);
			sendToTarget();
		}
	}

	cge::SpadesGameInfo ServerCore::generateInfoForPlayer( const SpadesGameInfo& info, ServerPlayer* player )
	{
		SpadesGameInfo i = info;
		return i;
	}

	void ServerCore::changeThemeC( int newTheme )
	{
		if(newTheme >= 0)
		{
			if(m_db->playerSetTheme(getServerPlayer()->getId(),newTheme))
			{
				getServerPlayer()->setTheme(newTheme);
			}

		}
	}

	void ServerCore::sendGiftC( int playerNum, int giftID )
	{
		ServerPlayer* p = getServerPlayer();
		if(!p)
			return;

		ServerTable* t = m_tableMan.getTableAt(p->getTable());

		if(!t)
			return;

		if(!t->isInProgress())
			return;

		int serverPos = playerNum;
		int pServerPos = p->getSeat();

		//begin payment processing
		int64 coinAmount = 0;
		int64 realPrice = 0;

		if(playerNum == -1)
		{
			realPrice = getGiftCost(t->getInfo().getNumPlayers() - 1,t);
		}
		else
		{
			realPrice = getGiftCost(1,t);
		}

	
		GiftData* giftDat = m_giftMan->getGiftById(giftID);

		if(giftDat == NULL)
			return;

		if(!m_db->playerGetCoinAmount(p->getId(),&coinAmount))	
		{
			return;
		}

		//cant afford it, kick him
		if(coinAmount < realPrice)
		{
			m_enc.sendMessageS("not.enough.coins.title","not.enough.coins.text",
				true,true);
			sendToTarget();
			return;
		}

		int64 newAmt = 0;
		//somehow spending failed???
		if(!m_db->playerSpendCoins(p->getId(),realPrice,&newAmt))
		{
			return;
		}

		if(newAmt == coinAmount)
			return;

		//end payment processing

		notifyCoinAmountChanged(p,true,false,true);

		if(playerNum > -1)
		{
			serverPos = t->getGame().mapClientToServer(pServerPos,playerNum);
		}

		ServerChair* c = t->getChairAt(serverPos);
		SpadesClientWrapper* ww = t->getGame().getPlayerAtPos(serverPos);
		if(playerNum > -1 && (!c || !ww || ww->isNull()))
			return;

		if(!giftDat->m_animated)
		{
			if(playerNum > -1)
				c->setGiftData(pServerPos,giftID);
			else
			{
				for(int i = 0; i < t->getNumChairs(); i++)
				{
					ServerChair* ch = t->getChairAt(i);
					SpadesClientWrapper* ww = t->getGame().getPlayerAtPos(i);
					if(ch && ww && !ww->isNull() && ch->getPlayer() != p)
					{
						ch->setGiftData(pServerPos,giftID);
					}
				}
			}
		}

		for(int i = 0; i < 4; i++)
		{
			SpadesClientWrapper* w = t->getGame().getPlayerAtPos(i);

			if(!w || w->isNull() || !w->isClient())
				continue;

			int pos = serverPos;
			int fromPos = t->getGame().mapServerToClientPos(i,pServerPos);
			
			if(playerNum > -1)
			{
				pos = t->getGame().mapServerToClientPos(i,serverPos);
			}

			ServerPeer* cl = w->getClient();
			if(cl)
			{
				m_enc.sendGiftS(fromPos,pos,giftID);
				setServerClient(cl);
				sendToTarget();
			}
		}

		for(int i = 0; i < t->getNumWatchers(); ++i)
		{
			ServerPlayer* w = t->getWatcher(i);
			if(w)
			{
				int pos = serverPos;
				int fromPos = t->getGame().mapServerToClientPos(0,pServerPos);

				if(playerNum > -1)
				{
					pos = t->getGame().mapServerToClientPos(0,serverPos);
				}

				ServerPeer* cl = w->getClient();
				if(cl)
				{
					m_enc.sendGiftS(fromPos,pos,giftID);
					setServerClient(cl);
					sendToTarget();
				}
			}
		}

		setServerClient(p->getClient());
	}

	void ServerCore::updateGiftC()
	{
		ServerPlayer* p = getServerPlayer();
		if(!p)
			return;

		ServerTable* t = m_tableMan.getTableAt(p->getTable());

		if(!t)
			return;

		if(!t->isInProgress())
			return;

		int pServerPos = p->getSeat();

		updateGift(t,pServerPos);
	}

	void ServerCore::generateFeeArray()
	{
		m_feeArray.clear();
		m_db->tableFeesGet(m_feeArray);
		if(m_feeArray.empty())
		{
			m_feeArray.push_back(25);
		}

		m_highStakesMinimum = m_db->getIntSetting("high_stakes_minimum_fee");
		if(m_highStakesMinimum == 0)
			m_highStakesMinimum = 1000;

		m_minRatingArray.clear();
		m_minRatingArray.push_back(0);
		m_minRatingArray.push_back(1);
		m_minRatingArray.push_back(100);
		m_minRatingArray.push_back(200);
		m_minRatingArray.push_back(300);
	}

	void ServerCore::sendFeeOptionsToTarget()
	{
		m_enc.sendFeeOptionsS(m_feeArray,m_minRatingArray,m_highStakesMinimum);;
		sendToTarget();
	}

	void ServerCore::sendGiftRemove( int table, int player )
	{
		ServerPlayer* p = getServerPlayer();
		ServerTable* t = m_tableMan.getTableAt(table);

		if(!t)
			return;

		ServerChair* c = t->getChairAt(player);
		if(c)
		{
			c->clearGiftData();
		}

		int pServerPos = player;
		int giftId = -1;
		int frame = 0;

		for(int i = 0; i < 4; i++)
		{
			SpadesClientWrapper* w = t->getGame().getPlayerAtPos(i);

			if(!w || w->isNull() || !w->isClient())
				continue;

			int pos = t->getGame().mapServerToClientPos(i,pServerPos);

			ServerPeer* cl = w->getClient();
			if(cl)
			{
				m_enc.updateGiftS(pos,giftId,frame);
				setServerClient(cl);
				sendToTarget();
			}
		}

		for(int i = 0; i < t->getNumWatchers(); ++i)
		{
			ServerPlayer* w = t->getWatcher(i);
			if(w)
			{
				int pos = t->getGame().mapServerToClientPos(0,pServerPos);

				ServerPeer* cl = w->getClient();
				if(cl)
				{
					m_enc.updateGiftS(pos,giftId,frame);
					setServerClient(cl);
					sendToTarget();
				}
			}
		}

		setServerClient(p->getClient());
	}

	bool ServerCore::verifyAndProcessTableFee( ServerTable* t )
	{
		if(!t || t->isInProgress() || !t->isReadyToPlay())
			return false;

		if(t->getInfo().isBotGame())
			return true;

		int fee = t->getInfo().getFee();

		m_db->getDB()->beginTransaction();
		int64 coinAmounts[4];
		for(int i = 0; i < 4; i++)
			coinAmounts[i] = 0;

		bool everyoneOk = true;

		for(int i = 0; i < t->getNumOccupants(); i++)
		{
			if(!m_db->playerGetCoinAmount(t->getChairAt(i)->getPlayer()->getId(),&coinAmounts[i]))	
			{
				m_db->getDB()->endTransaction(false);
				return false;
			}
		}

		for(int i = 0; i < t->getNumOccupants(); i++)
		{
			//cant afford it, kick him
			if(coinAmounts[i] < fee)
			{
				ServerPlayer* p = getServerPlayer();
				setServerClient(t->getChairAt(i)->getPlayer()->getClient());
				playerStoodUpC();
				showPaymentDialog(p);
				setServerClient(p->getClient());
				everyoneOk = false;
			}
		}

		if(!everyoneOk)
		{
			m_db->getDB()->endTransaction(false);
			return false;
		}

		//if we get here, everyone can afford it so charge them
		for(int i = 0; i < t->getNumOccupants(); i++)
		{
			ServerPlayer* p = t->getChairAt(i)->getPlayer();

			int64 newAmt = 0;
			//somehow spending failed???
			if(!m_db->playerSpendCoins(p->getId(),fee,&newAmt))
			{
				m_db->getDB()->endTransaction(false);
				return false;
			}

			if(!m_db->playerSetTempCoins(p->getId(),fee))
			{
				m_db->getDB()->endTransaction(false);
				return false;
			}
		}

		for(int i = 0; i < t->getNumOccupants(); i++)
		{
			ServerPlayer* p = t->getChairAt(i)->getPlayer();

			notifyCoinAmountChanged(p,true,false,true);
		}

		//everyone paid, all is well
		m_db->getDB()->endTransaction(true);
		return true;
	}

	void ServerCore::standTableUp( ServerTable* t )
	{
		if(!t)
			return;

		ServerPlayer* p = getServerPlayer();
		for(int i = 0; i < t->getNumChairs(); i++)
		{
			if(t->getChairAt(i)->isOccupied())
			{
				setServerClient(t->getChairAt(i)->getPlayer()->getClient());
				playerStoodUpC();
			}
		}

		setServerClient(p->getClient());
	}

	void ServerCore::generatePaymentArrays()
	{
		std::vector<SWDBStoreItem> items;
		m_db->storeItemsEnabled(items);

		m_paymentArrays.coinAmounts.clear();
		m_paymentArrays.coinPercents.clear();
		m_paymentArrays.ids.clear();
		m_paymentArrays.prices.clear();
		m_paymentArrays.salePercents.clear();

		for(size_t i = 0; i < items.size(); i++)
		{
			m_paymentArrays.coinAmounts.push_back(StringUtil::toString(items[i].m_coinValue));
			m_paymentArrays.coinPercents.push_back(items[i].m_morePercent);
			m_paymentArrays.ids.push_back(StringUtil::toString(items[i].m_id));
			m_paymentArrays.prices.push_back(items[i].m_price);
			m_paymentArrays.salePercents.push_back(items[i].m_morePercent);
		}
	}

	void ServerCore::showPaymentDialog( ServerPlayer* p )
	{
		if(!p)
			return;
		ServerPeer* sc = getServerClient();
		setServerClient(p->getClient());

		m_enc.showCoinsDialogS(
			m_paymentArrays.coinAmounts,
			m_paymentArrays.prices,
			m_paymentArrays.salePercents,
			m_paymentArrays.coinPercents,
			m_paymentArrays.ids);
		sendToTarget();
		setServerClient(sc);
	}

	void ServerCore::showCoinsDialogC()
	{
		showPaymentDialog(getServerPlayer());
	}

	bool ServerCore::canPlayerAffordTable( ServerTable* t, ServerPlayer* p )
	{
		if(!t || !p)
			return false;

		int64 fee = t->getInfo().getFee();
		int64 amount = 0;

		if(!m_db->playerGetCoinAmount(p->getId(),&amount))	
			return false;

		return amount >= fee;
	}

	bool ServerCore::clearPlayerTempCoins( ServerPlayer* p )
	{
		if(!p)
			return false;

		return m_db->playerSetTempCoins(p->getId(),0);
	}

	bool ServerCore::refundPlayerTempCoins( ServerPlayer* p )
	{
		if(!p)
			return false;

		int64 amount = 0;
		bool result = m_db->playerRefundTempCoins(p->getId(),&amount);
		return result;
	}

	void ServerCore::notifyCoinAmountChanged( ServerPlayer* p, bool update, bool fromPurchase, bool spending )
	{
		if(!p)
			return;
		
		int64 amount = -1;
		bool success = m_db->playerGetCoinAmount(p->getId(),&amount);

		if(success && amount >= 0)
		{
			m_enc.sendCoinAmountS(amount,update,fromPurchase,spending);
			sendToPlayer(p);
		}
	}

	void ServerCore::showGiftDialogC( int playerNum )
	{
		int realPrice = 0;
		std::string price;

		ServerTable* t = m_tableMan.getTableAt(getServerPlayer()->getTable());
		if(!t)
			return;

		if(playerNum == -1)
		{
			realPrice = getGiftCost(t->getInfo().getNumPlayers() - 1,t);
		}
		else
		{
			realPrice = getGiftCost(1,t);
		}

		price = StringUtil::toString(realPrice);

		m_enc.showGiftDialogS(playerNum,price);
		sendToTarget();
	}

	int ServerCore::getGiftCost( int numPlayers,ServerTable* t ) const
	{
		int basePrice = m_stemletCost;
		if(t->getInfo().getFee() > 0)
		{
			basePrice = (int)((float)(t->getInfo().getFee() * m_stemletRate) + 0.5f);
			if(basePrice < 1)
				basePrice = 1;
		}
		return numPlayers * basePrice;
	}

	bool ServerCore::playerIsOnline( int64 id )
	{
		ServerPlayerSetArray* s = m_playerMan.getPlayers(true);
		for(ServerPlayerSetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			if((*it)->getId() == id)
				return true;
		}

		return false;
	}

	void ServerCore::removeGiftC()
	{
		ServerPlayer* p = getServerPlayer();
		if(!p)
			return;

		ServerTable* t = m_tableMan.getTableAt(p->getTable());

		if(!t)
			return;

		int pServerPos = p->getSeat();

		sendGiftRemove(t->getTableNo(),pServerPos);
	}

	void ServerCore::sendFeedbackC( const std::string& otherPlayer,const std::string& comment, const std::string& data )
	{
		if(otherPlayer.length() > 100 || comment.length() > 1000 || data.length() > 2000)
			return;

		std::string ipData = data;
		ipData += "  - IP = " + getServerPlayer()->getIP();
		m_db->feedbackAddEntry(getServerPlayer()->getId(),otherPlayer,comment,ipData);
	}

	void ServerCore::updateGift( ServerTable* t, int player )
	{
		int pServerPos = player;
		ServerChair* c = t->getChairAt(pServerPos);

		ServerPlayer* p = getServerPlayer();

		if(!c)
			return;

		ServerGiftData gd = c->getGiftData();

		if(gd.giftID == -1)
			return;

		GiftData* giftData = m_giftMan->getGiftById(gd.giftID);

		if(!giftData || gd.frame + 1 >= giftData->getNumFrames())
			return;

		c->updateGiftData(gd.frame + 1);
		int frame = c->getGiftData().frame;
		int giftId = c->getGiftData().giftID;

		for(int i = 0; i < 4; i++)
		{
			SpadesClientWrapper* w = t->getGame().getPlayerAtPos(i);

			if(!w || w->isNull() || !w->isClient())
				continue;

			int pos = t->getGame().mapServerToClientPos(i,pServerPos);

			ServerPeer* cl = w->getClient();
			if(cl)
			{
				m_enc.updateGiftS(pos,giftId,frame);
				setServerClient(cl);
				sendToTarget();
			}
		}

		for(int i = 0; i < t->getNumWatchers(); ++i)
		{
			ServerPlayer* w = t->getWatcher(i);
			if(w)
			{
				int pos = t->getGame().mapServerToClientPos(0,pServerPos);

				ServerPeer* cl = w->getClient();
				if(cl)
				{
					m_enc.updateGiftS(pos,giftId,frame);
					setServerClient(cl);
					sendToTarget();
				}
			}
		}
		setServerClient(p->getClient());
	}

	void ServerCore::sendCoinAmountC(bool purchase)
	{
		notifyCoinAmountChanged(getServerPlayer(),true,purchase,false);
	}

	void ServerCore::sendStoreLinkC( const std::string& storeID )
	{
		ServerPlayer* p = getServerPlayer();
		std::string baseURL = std::string(WEB_URL) +"/Login.aspx";
		std::string appendUrl = "?token=";
		std::string token;
		m_db->tokenCreate(StringUtil::toString(p->getId()),1,&token);
		appendUrl += StringUtil::urlEncode(token);
		appendUrl += "&userid=" + StringUtil::toString(p->getId());
		std::string redirectUrl = std::string(WEB_URL) +"/Product.aspx";
		redirectUrl += "?product=" + storeID;
		redirectUrl = StringUtil::urlEncode(redirectUrl);
		appendUrl += "&redirecturl=" + redirectUrl;

		std::string finalURL = baseURL + appendUrl;

		m_enc.sendStoreLinkS(finalURL);
		sendToTarget();
	}

	void ServerCore::prerun(int port)
	{
		Log::write() << "Server started on port " << port << std::endl;
		//m_timer->start(60000);
		//m_tickTimer->start(1000);
		m_port = port;
		m_clientCore->setAddressAndPort(m_masterIP,m_masterPort);
		m_clientCore->connect();
		registerToMaster();
	}

	void ServerCore::postrun()
	{
		//timer stuff
	}

	void ServerCore::runtick()
	{
		m_clientCore->tick();
	}

	ServerPlayer* ServerCore::getServerPlayer()
	{
		if(getServerClient()->tag())
		{
			return (ServerPlayer*)getServerClient()->tag();
		}

		return NULL;
	}

	void ServerCore::requestServerListC()
	{
		if(!verifyMasterConnection()) return;
		m_clientEnc.fetchServerListMC(getServerPlayer()->getUsername());
	}

	void ServerCore::receivedServerListFromMaster( const std::string& username, const std::vector<std::string>& names, 
		const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities, 
		const std::vector<int>& numUsers, const std::vector<int>& numFriends )
	{
		ServerPlayer* p = m_playerMan.getActivePlayer(username);

		if(p)
		{
			m_enc.fetchServerListMS(username,names,ips,ports,capacities,numUsers,numFriends);
			sendToPlayer(p);
		}
	}

	void ServerCore::setName( const std::string& name )
	{
		m_serverName = name;
	}

	void ServerCore::serverFullC( const std::string& ip, int port )
	{
		if(!verifyMasterConnection()) return;
		m_clientEnc.serverFullMC(getServerPlayer()->getUsername(),ip,port);
	}

	void ServerCore::receiveServerFullFromMaster( const std::string& username, const std::string& serverName, const std::string& ip, int port, bool full )
	{
		ServerPlayer* p = m_playerMan.getActivePlayer(username);

		if(p)
		{
			m_enc.serverFullS(serverName,ip,port,full);
			sendToPlayer(p);
		}
	}

	void ServerCore::receiveLogoutPlayerFromMaster( const std::string& username )
	{
		ServerPlayerSetArray* s = m_playerMan.getPlayers(true);
		std::string lowerA;
		std::string lowerB = username;
		StringUtil::toLower(lowerB);
		std::vector<ServerPlayer*> removals;
		for(ServerPlayerSetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			lowerA = (*it)->getUsername();
			StringUtil::toLower(lowerA);
			if(lowerA == lowerB)
			{
				removals.push_back(*it);
			}
		}

		for(size_t i = 0; i < removals.size(); i++)
		{
			removals[i]->getClient()->close();
		}
	}

	void ServerCore::computeMinRating( SpadesGameInfo& info,int curRating )
	{
		if(curRating <= 0)
		{
			info.setMinRating(0);
		}
		else
		{
			if(info.getMinRating() > 0)
			{
				if(info.getFee() >= m_highStakesMinimum)
				{
					if(info.getMinRating() == 1)
					{
						info.setMinRating(curRating);
					}
					else
					{
						info.setMinRating(curRating - info.getMinRating());
					}
				}
			}

			if(info.getMinRating() < 0)
			{
				info.setMinRating(0);
			}

			if(info.getMinRating() > curRating)
			{
				info.setMinRating(curRating);
			}
		}
	}

	void ServerCore::receivedFriendServerListFromMaster( const std::string& username, const std::vector<std::string>& names, 
		const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities,
		const std::vector<int>& numUsers, const std::vector<std::string>& serverNames )
	{
		ServerPlayer* p = m_playerMan.getActivePlayer(username);

		if(p)
		{
			m_enc.fetchFriendServerListMS(username,names,ips,ports,capacities,numUsers,serverNames);
			sendToPlayer(p);
		}
	}

	void ServerCore::requestFriendServerListC()
	{
		if(!verifyMasterConnection()) return;
		m_clientEnc.fetchFriendServerListMC(getServerPlayer()->getUsername());
	}

	void ServerCore::requestPlayerC()
	{
		ServerPlayer* player = getServerPlayer();
		ServerTable* t = m_tableMan.getTableAt(player->getTable());

		if(!t)
			return;

		if(t->getInfo().isBotGame())
			return;

		if(t->getNumOccupants() == t->getInfo().getNumPlayers())
			return;

		if(t->getRequestTimeout() >= 60.0f)
		{
			t->resetRequestTimeout();
			m_enc.requestPlayerS(t->getTableNo(),player->getUsername());
			sendToAllActive();
		}
		else
		{
			m_enc.sendMessageS("request.timeout.title","request.timeout.text",
				true,true);
			sendToTarget();
		}
	}

	void ServerCore::playerRequestDoAction( int tableNo, int playerNo )
	{
		ServerTable* t = m_tableMan.getTableAt(tableNo);
		if(!t)
			return;
		ServerChair* c = t->getChairAt(playerNo);
		if(!c)
			return;
		c->turnStarted();
	}

	void ServerCore::playerFinishedDoingAction( int tableNo, int playerNo )
	{
		ServerTable* t = m_tableMan.getTableAt(tableNo);
		if(!t)
			return;
		ServerChair* c = t->getChairAt(playerNo);
		if(!c)
			return;

		if(c->sentFirstStrike())
		{
			sendStrike(t,playerNo,c->getTurnStrikes(),0,false);
		}
		c->turnEnded();
	}

	void ServerCore::doKickPlayer( ServerTable* t,
		ServerPlayer* p, bool timeout )
	{
		t->kickPlayer(p);
		ServerPlayer* cur = getServerPlayer();
		setServerClient(p->getClient());
		playerAbandonedGameC(timeout);

		m_enc.chatMessageFromServerS("player.kicked.short",p->getUsername(),true,GAME_CHAT);
		sendToTable(t->getTableNo());
		m_enc.sendMessageS("player.kicked.title","player.kicked",true,true);
		sendToTableNotWatchers(t->getTableNo());

		setServerClient(cur->getClient());
	}

	int ServerCore::getPlayTimeoutShowTime( const SpadesGameInfo& info ) const
	{
		if(info.getFee() >= m_highStakesMinimum)
		{
			return m_highStakesTimeoutShow;
		}
		else
		{
			return m_normalTimeoutShow;
		}
	}

	int ServerCore::getPlayerTimeoutTime( const SpadesGameInfo& info ) const
	{
		if(info.getFee() >= m_highStakesMinimum)
		{
			return m_highStakesTimeout;
		}
		else
		{
			return m_normalTimeout;
		}
	}

	void ServerCore::sendStrike( ServerTable* t, int player, int numStrikes, int time, bool beginAction )
	{
		ServerPeer* serverCl = getServerClient();

		for(int i = 0; i < 4; i++)
		{
			SpadesClientWrapper* w = t->getGame().getPlayerAtPos(i);

			if(!w || w->isNull() || !w->isClient())
				continue;

			int pos = t->getGame().mapServerToClientPos(i,player);

			ServerPeer* cl = w->getClient();
			if(cl)
			{
				m_enc.requestStrikesS(pos,time,numStrikes,beginAction);
				setServerClient(cl);
				sendToTarget();
			}
		}

		setServerClient(serverCl);
	}

	void ServerCore::lostConnectionToMaster()
	{
		//lost connection to master server
		std::cout << "Lost connection to master" << std::endl;
		m_enc.badAsyncRequestS();
		sendToAllActive();
		m_clientCore->connect();
		registerToMaster();
	}

	void ServerCore::connectionFailedToMaster()
	{
		std::cout << "Fail connection to master" << std::endl;
		m_clientCore->connect();
		registerToMaster();
	}

	void ServerCore::registerToMaster()
	{
		//m_clientEnc.registerServerMC(m_name,"54.149.137.202",port,
		m_clientEnc.registerServerMC(m_serverName,m_externIP,m_port,
			MAX_PLAYERS - 10,std::vector<std::string>(),"",m_leagueID);

		ServerPlayerSetArray* s = m_playerMan.getPlayers(true);
		for(ServerPlayerSetArray::iterator it = s->begin(); it != s->end(); ++it)
		{
			ServerPlayer* s = (*it);
			m_clientEnc.playerLoggedInMC(s->getUsername());
		}
	}

	bool ServerCore::verifyMasterConnection()
	{
		if(!m_clientCore->connected())
		{
			m_enc.badAsyncRequestS();
			sendToTarget();
			return false;
		}

		return true;
	}

	void ServerCore::generateCoinAwardRateArrays()
	{
		m_houseFee = -1.0f;
		m_houseFee = m_db->getFloatSetting("award_house_cut");
		if(m_houseFee < 0)
			m_houseFee = 0.2f;

		m_stemletCost = m_db->getIntSetting("stemlet_cost");

		m_stemletRate = m_db->getFloatSetting("stemlet_rate");

		if(m_stemletRate <= 0)
			m_stemletRate = 0.1f;

		m_partnershipCoinAwardRate.push_back(m_db->getFloatSetting("award_partnership_first"));
		m_partnershipCoinAwardRate.push_back(m_db->getFloatSetting("award_partnership_second"));

		m_individualCoinAwardRate.push_back(m_db->getFloatSetting("award_individual_first"));
		m_individualCoinAwardRate.push_back(m_db->getFloatSetting("award_individual_second"));
		m_individualCoinAwardRate.push_back(m_db->getFloatSetting("award_individual_third"));
		m_individualCoinAwardRate.push_back(m_db->getFloatSetting("award_individual_fourth"));

		m_cutthroatCoinAwardRate.push_back(m_db->getFloatSetting("award_cutthroat_first"));
		m_cutthroatCoinAwardRate.push_back(m_db->getFloatSetting("award_cutthroat_second"));
		m_cutthroatCoinAwardRate.push_back(m_db->getFloatSetting("award_cutthroat_third"));
	}

	void ServerCore::masterConfirmAlive()
	{
		m_waitingForMasterResponse = false;
		m_masterResponseTime = 0;
		m_masterRequestTime = 0;
	}

	void ServerCore::generateTimeouts()
	{
		m_highStakesTimeout = m_db->getIntSetting("high_stakes_timeout");
		m_highStakesTimeoutShow = m_db->getIntSetting("high_stakes_timeout_show");
		m_normalTimeout = m_db->getIntSetting("normal_timeout");
		m_normalTimeoutShow = m_db->getIntSetting("normal_timeout_show");
	}

	void ServerCore::punishDisconnectedPlayersForTable( ServerTable* t )
	{
		if(!t)
			return;

		if(t->getInfo().isBotGame())
		{
			t->clearDisconnectedPlayers();
			return;
		}
		const std::vector<std::pair<int, int> >& disconnected  = t->getDisconnectedPlayers();

		for(size_t i = 0; i < disconnected.size();i++)
		{
			m_layer.punishQuitter(disconnected[i].first,KARMA_FOR_QUIT,t->getInfo(),t,false);
		}

		t->clearDisconnectedPlayers();
	}

	bool ServerCore::checkPlayerDisconnection()
	{
		for(std::vector<ServerTable*>::iterator it = m_tableMan.begin(); it != m_tableMan.end();++it)
		{
			ServerTable* t = (*it);
			if(t && t->wasPlayerDisconnected(getServerPlayer()->getId()))
			{
				bool subber = false;
				for(int i = 0;  i < t->getNumChairs(); i++) {
					if(t->getChairAt(i)->isOccupied() && t->getChairAt(i)->getOriginalPid() == getServerPlayer()->getId())
					{
						subber = true;
					}
				}
				if(!subber)
				{
					m_enc.returnToGameS();
					sendToTarget();
				}
				else
				{
					t->removeDisconnectedPlayer(getServerPlayer()->getId());
					int64 newamt = 0;
					m_db->playerAddCoins(getServerPlayer()->getId(),t->getInfo().getFee(),&newamt);
					notifyCoinAmountChanged(getServerPlayer(),true,false,false);
				}
		
				return true;
			}
		}

		return false;
	}

	void ServerCore::returnToGameC( bool ret )
	{
		ServerTable* t = NULL;
		for(std::vector<ServerTable*>::iterator it = m_tableMan.begin(); it != m_tableMan.end();++it)
		{
			ServerTable* table = (*it);
			if(table && table->wasPlayerDisconnected(getServerPlayer()->getId()))
			{
				t = table;
				break;
			}
		}
		if(!t) {
			m_enc.sendMessageS("table.gone.title","table.gone.text",true,true);
			sendToTarget();
			return;
		}

		if(!ret)
		{
			m_layer.punishQuitter(getServerPlayer()->getId(),KARMA_FOR_QUIT,t->getInfo(),t,false);
			t->removeDisconnectedPlayer(getServerPlayer()->getId());
			return;
		}

		int seat = t->getDisconnectedPlayerSeat(getServerPlayer()->getId());
		t->removeDisconnectedPlayer(getServerPlayer()->getId());

		if(!t->isInProgress() || seat < 0 || seat >= t->getInfo().getNumPlayers())
			return;

		ServerChair* c = t->getChairAt(seat);

		if(!c || c->getOriginalPid() != getServerPlayer()->getId())
			return;
/*
		if(t->getNumOccupants() == 1)
		{
			m_enc.sendMessageS("table.almost.gone.title","table.almost.gone.text",true,true);
			sendToTarget();
			int64 newamt = 0;
			m_db->playerAddCoins(getServerPlayer()->getId(),t->getInfo().getFee(),&newamt);
			notifyCoinAmountChanged(getServerPlayer(),true,false,false);
			return;
		}
		*/
		ServerPlayer* s = c->getPlayer();
		if(s)
		{
			t->removeDisconnectedPlayer(getServerPlayer()->getId());
			int64 newamt = 0;
			m_db->playerAddCoins(getServerPlayer()->getId(),t->getInfo().getFee(),&newamt);
			notifyCoinAmountChanged(getServerPlayer(),true,false,false);
			m_enc.sendMessageS("sub.seat.taken.title","sub.seat.taken.text",true,true);
			sendToTarget();
			return;
			ServerPeer* p = getServerClient();
			setServerClient(s->getClient());
			playerAbandonedGameC(false,false,true);
			setServerClient(p);
		}

		if(!t->isInProgress())
		{
			m_enc.sendMessageS("table.gone.title","table.gone.text",true,true);
			m_layer.punishQuitter(getServerPlayer()->getId(),KARMA_FOR_QUIT,t->getInfo(),t,false);
			sendToTarget();
			return;
		}

		playerRequestSitC(t->getTableNo(),seat,true);
	}

}

