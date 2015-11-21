#include "Game/Net/LobbyNetHandler.hpp"
#ifdef CGE_IPHONE
#include "Game/Platform/IAPHandler.h"
#endif
namespace cge
{
	LobbyNetHandler::~LobbyNetHandler(void)
	{
	}

	LobbyNetHandler::LobbyNetHandler( NetEventEncoder* enc, ClientShared* shared )
		: m_enc(enc), m_shared(shared),m_updatingCache(true)
	{

	}

	void LobbyNetHandler::sendChatMessageS( const std::string& playerName, const std::string& message,ChatAreaEnum area )
	{	
		if(m_shared->getSceneMessenger()->getCurrentScene() == SceneManagerMessage::LOBBY && area == GAME_CHAT)
		{
			area = TABLE_CHAT;
		}

		if((area == TABLE_CHAT || area == LOBBY_CHAT) &&
			m_shared->getSceneMessenger()->getCurrentScene() == SceneManagerMessage::LOBBY)
		{
			TableMessage t;
			t.m_username = playerName;
			t.m_message = message;
			DISPATCH_LOBBY_EVENT
				(*it)->determineLobbyPlayerColor(t);
			
			if(area == TABLE_CHAT)
			{
				m_shared->getPlayerManager()->queueTableMessage(t);
			}
		
			
			DISPATCH_LOBBY_EVENT
				(*it)->chatReceiveMessage(message,playerName,area,t.m_lobbyColor);

		}

	}

	void LobbyNetHandler::chatSendMessage( const std::string& message, ChatAreaEnum chatArea )
	{
		m_enc->sendChatMessageC(message,chatArea);
	}

	void LobbyNetHandler::tableLockedS( int tableNo, int seatNo, bool locked )
	{
			DISPATCH_LOBBY_EVENT
				(*it)->tableLocked(tableNo,seatNo,locked);
	}

	void LobbyNetHandler::requestHostTable( int tableNo, int seatNo )
	{
		m_enc->requestTableC(tableNo,seatNo);
	}

	void LobbyNetHandler::requestShowProfile( const std::string& playerName )
	{
		if(playerName == m_shared->getLanguageManager()->getElement("server")) {
	DISPATCH_LOBBY_EVENT
		(*it)->badAsyncRequest();
		}

		m_enc->showProfileC(playerName);
	}

	void LobbyNetHandler::requestAddFriend( const std::string& playerName )
	{
		m_enc->modifyListC(playerName,FRIENDS_GROUP,true);
	}

	void LobbyNetHandler::requestRemoveFriend( const std::string& playerName )
	{
		m_enc->modifyListC(playerName,FRIENDS_GROUP,false);
	}

	void LobbyNetHandler::requestAddEnemy( const std::string& playerName )
	{
		m_enc->modifyListC(playerName,ENEMIES_GROUP,true);
	}

	void LobbyNetHandler::requestRemoveEnemy( const std::string& playerName )
	{
		m_enc->modifyListC(playerName,ENEMIES_GROUP,false);

	}

	bool LobbyNetHandler::isMutedImpl( const std::string& playerName, bool* result )
	{
		*result = m_shared->getPlayerManager()->isMuted(playerName);
		return true;
	}

	void LobbyNetHandler::setMuted( const std::string& playerName, bool muted )
	{
		m_shared->getPlayerManager()->setMuted(playerName,muted);
	}

	void LobbyNetHandler::requestSitAtTable( int tableNumber, int seatNumber )
	{
		m_enc->playerRequestSitC(tableNumber,seatNumber);
	}

	void LobbyNetHandler::requestLeaveTable()
	{
		m_enc->playerAbandonedGameC();
	}

	void LobbyNetHandler::playerStoodUpS( int tableNo,int seatNo )
	{

		//update if in a game
		ClientPlayer* p = m_shared->getPlayerManager()->getSelfPlayer();
		int tNo = -1;
		if(p->isWatching())
		{
			tNo = p->getWatchTableNo();
		}
		else
		{
			tNo = p->getTable();
		}


		if(tNo != -1)
		{
			std::string s = m_shared->getPlayerManager()->getPlayerByTable(tNo,seatNo);
			ClientPlayer* p = m_shared->getPlayerManager()->getPlayerByName(s);

			if(p)
			{
				DISPATCH_GAME_EVENT
					(*it)->playerLeftGame(p->getUsername(),p->getSeat());
			}
		}

		DISPATCH_LOBBY_EVENT
			(*it)->playerLeftTable(tableNo,seatNo);

		if(isUpdatingCache())
		{
			//update the cache
			LobbyCache::Table* t = m_shared->getLobbyCache()->getTableAt(tableNo);

			if(t)
			{
				if(seatNo != -1)
				{
					if(t->statusChange(false))
					{
						t->updatePlayersInGame(false);
						updateTableGroup(t);
					}
					t->getChairAt(seatNo)->setPayer(NULL);
				}
			}
		}

		m_shared->getPlayerManager()->standPlayerUp(tableNo,seatNo);
	}

	void LobbyNetHandler::hostTable( const SpadesGameInfo& info )
	{
		m_enc->hostGameC(info);
	}

	void LobbyNetHandler::playerSatDownS( const std::string& player, int tableNumber, int seatNumber )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->sitPlayerAtTable(tableNumber,seatNumber,player);

		//update the cache
		if(isUpdatingCache())
		{
			LobbyCache::Table* t = m_shared->getLobbyCache()->getTableAt(tableNumber);
			if(t)
			{
				ClientPlayer* p = m_shared->getPlayerManager()->getPlayerByName(player);
				if(p && seatNumber != -1)
				{
					t->getChairAt(seatNumber)->setPayer(p);

					if(t->statusChange(true))
					{
						t->updatePlayersInGame(true);
						updateTableGroup(t);
					}

				}
			}
		}

		//update if in a game
		ClientPlayer* p = m_shared->getPlayerManager()->getSelfPlayer();
	
		int tableNo = -1;
		if(p->isWatching())
		{
			tableNo = p->getWatchTableNo();
		}
		else
		{
			tableNo = p->getTable();
		}

		if(p->getUsername() == player && tableNumber != -1 && isUpdatingCache())
		{
			DISPATCH_LOBBY_EVENT
				(*it)->clearTableChat();
			clearTableMessages();
		}

		
		//player is sitting
		if(tableNo != -1 && tableNo == tableNumber)
		{
			DISPATCH_GAME_EVENT
				(*it)->playerJoinedGame(player,seatNumber);
		}
	}

	void LobbyNetHandler::tableStateChangedS( int tableNo,const SpadesGameInfo& info )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->setTableInfo(tableNo,info);

		//update the cache
		if(isUpdatingCache())
		{
			LobbyCache::Table* t = m_shared->getLobbyCache()->getTableAt(tableNo);
			if(t)
			{
				t->setTableInfo(info);
				t->setActive(true);
				t->setBeingSetup(false);
			}
		}
	}

	void LobbyNetHandler::loginRequest( const std::string& username, const std::string& password )
	{
		int major = 1;
		int minor = 4;
#ifdef CGE_WINDOWS
		int system = 0;
#endif
#ifdef CGE_OSX
		int system = 1;
#endif
#ifdef CGE_IPHONE
		int system = 2;
#endif
		m_enc->loginRequestC(username,password,major,minor,system);
	}

	void LobbyNetHandler::loginResultS( bool success, const std::string& sessionID, const std::string& username )
	{
		if(success)
		{
			//for now, require full update on login
			m_shared->getLobbyCache()->setNeedsFullUpdate(true);
			m_shared->getPlayerManager()->getSelfPlayer()->setUserName(username);
            
#ifdef CGE_IPHONE
            IAPHandler::getInstance().setUsername(username);
#endif
		}
	
		if(!success)
		{
			m_shared->_setLastLoginError(sessionID);
		}
		DISPATCH_LOBBY_EVENT
			(*it)->loginResult(success,sessionID,username);
	}

	void LobbyNetHandler::playerJoinedS( const SharedPlayer& player, GroupTypeEnum group )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->addPlayer(player.getUsername(),group);

		if(isUpdatingCache())
		{
			ClientPlayer* p = m_shared->getPlayerManager()->createPlayer(player.getUsername());
			p->setAvatar(player.getAvatar());
			if(m_shared->getPlayerManager()->getSelfPlayer()->getUsername() == player.getUsername())
			{
				m_shared->getPlayerManager()->getSelfPlayer()->setAvatar(player.getAvatar());
			}

			if(player.getTableId() != -1 && player.getSeat() != -1)
			{
				LobbyCache::Table* t = m_shared->getLobbyCache()->getTableAt(player.getTableId());
				if(t)
				{
					//server sends player who set up the table but don't sit him
					if(t->isBeingSetup())
					{
						p->standUp();
					}
					else
					{
						playerSatDownS(player.getUsername(),player.getTableId(),player.getSeat());
					}
				}
			}
			else if(player.getWatchTableId() != -1)
			{
				playerWatchingTableS(player.getWatchTableId(),player.getUsername());
			}
		}
	}

	void LobbyNetHandler::playerLeftS( const std::string& userName )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->removePlayer(userName);
		if(isUpdatingCache())
		{
			m_shared->getPlayerManager()->destroyPlayer(userName);
		}	
	}

	bool LobbyNetHandler::getPlayerByName( ClientPlayer* &player, const std::string& name )
	{
		player = m_shared->getPlayerManager()->getPlayerByName(name);
		return true;
	}

	void LobbyNetHandler::hostingDutiesChangedS( bool isHost )
	{
		m_shared->getPlayerManager()->getSelfPlayer()->setHosting(isHost);
	}

	void LobbyNetHandler::enterGameS( const SpadesGameInfo& info )
	{
		m_shared->getNewGameData()->setGameInfo(info);
		m_shared->getNewGameData()->setOnline(true);


		DISPATCH_LOBBY_EVENT
			(*it)->enterGame();
	}

	bool LobbyNetHandler::isPlayerInGameImpl( const std::string& playerName, bool* result )
	{
		ClientPlayer* p = m_shared->getPlayerManager()->getPlayerByName(playerName);
		if(p)
		{
			*result = p->isSitting();
		}
		
		return true;
	}

	void LobbyNetHandler::queueTableMessage(const TableMessage& t )
	{
		m_shared->getPlayerManager()->queueTableMessage(t);
	}

	void LobbyNetHandler::clearTableMessages()
	{
		m_shared->getPlayerManager()->clearTableMessages();
	}

	void LobbyNetHandler::tableIsInactiveS( int tableNo )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->setTableInfo(tableNo,SpadesGameInfo(),false);

		if(isUpdatingCache())
		{
			//update the cache
			LobbyCache::Table* t = m_shared->getLobbyCache()->getTableAt(tableNo);
			if(t)
			{
				t->setActive(false);
				t->setBeingSetup(false);

				for(int i = 0; i < t->getNumChairs(); ++i)
				{
					t->getChairAt(i)->setAsSubSeat(false);
				}
			}
		}
	}

	bool LobbyNetHandler::getPlayerTableImpl( const std::string& playerName, int* tableNo )
	{
		ClientPlayer* p = m_shared->getPlayerManager()->getPlayerByName(playerName);

		if(p)
		{
			*tableNo = p->getTable();
		}
		else
		{
			*tableNo = -1;
		}

		return true;
	}

	bool LobbyNetHandler::isFriendImpl( const std::string& playerName, bool* result )
	{
		*result = m_shared->getPlayerManager()->isFriend(playerName);
		return true;
	}

	bool LobbyNetHandler::isEnemyImpl( const std::string& playerName, bool* result )
	{
		*result = m_shared->getPlayerManager()->isEnemy(playerName);
		return true;
	}

	void LobbyNetHandler::modifiedListS( const std::string& player, GroupTypeEnum group, bool add )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->removePlayer(player);

		if(add)
		{
			if(group == FRIENDS_GROUP)
			{
				m_shared->getPlayerManager()->addFriend(player);
			}
			else if(group == ENEMIES_GROUP)
			{
				m_shared->getPlayerManager()->addEnemy(player);
			}
		}
		else
		{
			if(group == FRIENDS_GROUP)
			{
				m_shared->getPlayerManager()->removeFriend(player);
			}
			else if(group == ENEMIES_GROUP)
			{
				m_shared->getPlayerManager()->removeEnemy(player);
			}
		}

		ClientPlayer* p = m_shared->getPlayerManager()->getPlayerByName(player);
		if(p && !isSelfPlayer(p))
		{
			DISPATCH_LOBBY_EVENT
			(*it)->addPlayer(p->getUsername(),m_shared->getPlayerManager()->getPlayerGroup(p));
		}
	}

	void LobbyNetHandler::playerGroupToggleChanged( GroupTypeEnum group, bool toggled )
	{
		m_shared->getSettingsManager()->setGroupToggled(group,toggled);
	}

	bool LobbyNetHandler::isPlayerGroupToggledImpl( GroupTypeEnum group, bool *toggled )
	{
		*toggled = m_shared->getSettingsManager()->isGroupToggled(group);
		return true;
	}

	void LobbyNetHandler::cancelHostRequest()
	{
		m_enc->canceledHostingRequestC();
	}

	void LobbyNetHandler::readyForLobbyData()
	{
		m_enc->readyToReceiveLobbyDataC();
	}

	void LobbyNetHandler::beginSendLobbyDataS( int tableBegin, int numTables )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->createTables(tableBegin,numTables);

		if(isUpdatingCache())
		{
			m_shared->getLobbyCache()->clear();
			m_shared->getLobbyCache()->createTables(tableBegin,numTables);
		}
	}

	void LobbyNetHandler::tableBeingSetupS( int tableNo )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->tableBeingSetup(tableNo);


		//update the cache
		LobbyCache::Table* t = m_shared->getLobbyCache()->getTableAt(tableNo);
		if(t)
		{
			t->setBeingSetup(true);
		}
	}

	bool LobbyNetHandler::isUpdatingCache() const
	{
		return m_updatingCache;
	}

	void LobbyNetHandler::updateLobbyFromCache()
	{
		m_updatingCache = false;

		//create tables
		beginSendLobbyDataS(m_shared->getLobbyCache()->getTableBegin(),
			m_shared->getLobbyCache()->getNumTables());
		
		for(size_t i = 0; i < m_shared->getLobbyCache()->getNumTables(); ++i)
		{
			LobbyCache::Table* t = &m_shared->getLobbyCache()->getTables()[i];

			if(t->isBeingSetup())
			{
				tableBeingSetupS(t->getTableNo());
			}
			else if(t->isActive())
			{
				tableStateChangedS(t->getTableNo(),t->getGameInfo());
			}

		}

		for(size_t i = 0; i < m_shared->getLobbyCache()->getNumTables(); ++i)
		{
			LobbyCache::Table* t = &m_shared->getLobbyCache()->getTables()[i];

			for(int j = 0; j < t->getNumChairs(); ++j)
			{
				ClientPlayer* p = t->getChairAt(j)->getPlayer();
				if(p)
				{
					if(p == m_shared->getPlayerManager()->getSelfPlayer())
					{
						DISPATCH_LOBBY_EVENT
							(*it)->focusOnLobbyTable(t->getTableNo(),false);
					}
					playerSatDownS(p->getUsername(),t->getTableNo(),j);
				}

				if(t->getChairAt(j)->isSubSeat())
				{
					DISPATCH_LOBBY_EVENT
						(*it)->seatIsSubSeat(t->getTableNo(),j,true);
				}
			}
		}

		for(int i = 0; i < m_shared->getPlayerManager()->getNumPlayers(); ++i)
		{
			ClientPlayer* p = m_shared->getPlayerManager()->getPlayerAt(i);

			if(p)
			{
				DISPATCH_LOBBY_EVENT
				(*it)->addPlayer(p->getUsername(),
					m_shared->getPlayerManager()->getPlayerGroup(p));
			}
		}

		DISPATCH_LOBBY_EVENT
			(*it)->coinAmountChanged(m_shared->getCoinAmount(),false,false,false);

		m_updatingCache = true;
	}

	void LobbyNetHandler::updateTableGroup( LobbyCache::Table* t )
	{
		for(int i = 0; i < t->getNumChairs(); ++i)
		{
			ClientPlayer* p = t->getChairAt(i)->getPlayer();
			ClientPlayer* me = m_shared->getPlayerManager()->getSelfPlayer();

			if(p && p->getUsername() == me->getUsername())
			{
				//continue;
			}
			if(p)
			{
				DISPATCH_LOBBY_EVENT
				(*it)->removePlayer(p->getUsername());

				DISPATCH_LOBBY_EVENT
					(*it)->addPlayer(p->getUsername(),
					m_shared->getPlayerManager()->getPlayerGroup(p));
			}
		}
	}

	void LobbyNetHandler::seatIsSubSeatS( int tableNo, int seatNo, bool subseat )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->seatIsSubSeat(tableNo,seatNo,subseat);

		if(isUpdatingCache())
		{
			//update the cache
			LobbyCache::Table* t = m_shared->getLobbyCache()->getTableAt(tableNo);
			if(t)
			{
				LobbyCache::Chair* c = t->getChairAt(seatNo);
				if(c)
				{
					c->setAsSubSeat(subseat);
				}
			}
		}
	}

	void LobbyNetHandler::sendInviteS( int tableNo, const std::string& player )
	{
		if(!m_shared->getPlayerManager()->isEnemy(player))
		{
			DISPATCH_LOBBY_EVENT
				(*it)->invitationReceived(tableNo,player);
		}
	}

	void LobbyNetHandler::sendInvitation( int tableNo,const std::string& player )
	{
		m_enc->sendInviteC(tableNo,player);
	}

	void LobbyNetHandler::playerWatchingTableS( int tableNo, const std::string& player )
	{
		ClientPlayer* p = m_shared->getPlayerManager()->getPlayerByName(player);
		if(!p)
		{
			return;
		}

		//update if in a game
		ClientPlayer* me = m_shared->getPlayerManager()->getSelfPlayer();

		if(me->getUsername() == player && tableNo != -1 && isUpdatingCache())
		{
			DISPATCH_LOBBY_EVENT
				(*it)->clearTableChat();

			clearTableMessages();
		}

		int tNo = -1;
		if(me->isWatching())
		{
			tNo = me->getWatchTableNo();
		}
		else
		{
			tNo = me->getTable();
		}

		if(tNo != -1 && p->getWatchTableNo() != -1 && tableNo == -1)
		{
			DISPATCH_GAME_EVENT
				(*it)->playerLeftGame(player,-1);
		}

		if(isUpdatingCache())
		{
			if(p->getWatchTableNo() != -1)
			{
				LobbyCache::Table* t = m_shared->getLobbyCache()->getTableAt(p->getWatchTableNo());

				if(t)
				{
					t->removeWatcher(p);
				}
			}
			p->setWatchTableNo(tableNo);

			if(tableNo != -1)
			{
				LobbyCache::Table* t = m_shared->getLobbyCache()->getTableAt(p->getWatchTableNo());

				if(t)
				{
					t->addWatcher(p);
				}
			}
		}

		if(tNo != -1 && tableNo != -1 && tNo == tableNo)
		{
			DISPATCH_GAME_EVENT
				(*it)->playerJoinedGame(player,-1);
		}

		if(p && p->getUsername() != me->getUsername())
		{
				DISPATCH_LOBBY_EVENT
					(*it)->removePlayer(p->getUsername());

				DISPATCH_LOBBY_EVENT
					(*it)->addPlayer(p->getUsername(),
					m_shared->getPlayerManager()->getPlayerGroup(p));
		}	
	}

	void LobbyNetHandler::requestWatchTable( int tableNo )
	{
		m_enc->watchTableC(tableNo);
	}

	void LobbyNetHandler::processLobbyTableMessages()
	{
		for(int i = m_shared->getPlayerManager()->getTableMessageQueue().size() - 1; i >= 0 ; --i)
		{
			
			TableMessage* t = &m_shared->getPlayerManager()->getTableMessageQueue()[i];

			if(t->m_lobbyColorType == TMC_UNDETERMINED)
			{
				DISPATCH_LOBBY_EVENT
					(*it)->determineLobbyPlayerColor(*t);
			}

			DISPATCH_LOBBY_EVENT
				(*it)->chatReceiveMessage(t->m_message,t->m_username,TABLE_CHAT,t->m_lobbyColor);

		}
	}

	void LobbyNetHandler::isClientAliveSC()
	{

	}

	void LobbyNetHandler::logout(bool signal)
	{
		m_enc->logoutC();

		if(signal)
		m_shared->_setLastLoginError("logout");
	}

	void LobbyNetHandler::chatSendTyping( ChatAreaEnum chatArea )
	{
		m_enc->sendChatTypingC(chatArea);
	}

	void LobbyNetHandler::sendChatTypingS( const std::string& playerName, ChatAreaEnum area )
	{
		if(m_shared->getSceneMessenger()->getCurrentScene() == SceneManagerMessage::LOBBY && area == GAME_CHAT)
		{
			area = TABLE_CHAT;
		}

		if((area == TABLE_CHAT || area == LOBBY_CHAT) &&
			m_shared->getSceneMessenger()->getCurrentScene() == SceneManagerMessage::LOBBY)
		{
			TableMessage t;
			t.m_username = playerName;
			DISPATCH_LOBBY_EVENT
				(*it)->determineLobbyPlayerColor(t);

			DISPATCH_LOBBY_EVENT
				(*it)->chatReceiveTyping(playerName,area,t.m_lobbyColor);
		}
	}

	void LobbyNetHandler::connectionFailedCS()
	{
		DISPATCH_LOBBY_EVENT
			(*it)->connectionFailed();
	}

	void LobbyNetHandler::showProfileS( int avatar, const std::vector<std::string>& data )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->showProfile(avatar,data);
	}

	void LobbyNetHandler::playerAcceptedTerms( bool accept )
	{
		m_enc->playerAcceptedTermsC(accept);
	}

	void LobbyNetHandler::createAccountS(
		bool success, bool usernameTaken,bool badUsername,bool badPassword,bool badEmail, bool badGender,
		bool badBirthday, bool tooYoung, bool emailNeedsVerification )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->createAccountResponse(
			success,usernameTaken,badUsername,badPassword,badEmail,badGender,badBirthday,tooYoung,emailNeedsVerification);
	}

	void LobbyNetHandler::playerMustAcceptTermsS( const std::string& terms )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->playerMustAcceptTerms(terms);
	}

	void LobbyNetHandler::sendMessageS( const std::string& title, const std::string& message, bool titleLocalized,bool messageLocalized )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->displayMessageFromServer(title,message,titleLocalized,messageLocalized);
	}

	void LobbyNetHandler::waitForSubResponse( bool stay )
	{
		m_enc->waitForSubC(stay);
	}

	void LobbyNetHandler::waitForSubS( bool cancel )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->waitForSub(cancel);
	}

	void LobbyNetHandler::chatMessageFromServerS( const std::string& message, const std::string& arg, bool messageLocalized, ChatAreaEnum area )
	{
		std::string mess;

		if(messageLocalized)
		{
			mess = m_shared->getLanguageManager()->getElement(message);
		}
		else
		{
			mess = message;
		}

		if(arg.length() > 0)
		{
			mess = m_shared->getLanguageManager()->appendArg(mess,arg);
		}

		sendChatMessageS(m_shared->getServerName(),mess,area);
	}

	void LobbyNetHandler::requestAccountSettings()
	{
		m_enc->requestAccountSettingsC();
	}

	void LobbyNetHandler::requestChangePassword( const std::string& oldPassword, const std::string& newPassword )
	{
		m_enc->requestChangePasswordC(oldPassword,newPassword);
	}

	void LobbyNetHandler::requestChangeEmail( const std::string& newEmail )
	{
		m_enc->requestChangeEmailC(newEmail);
	}

	void LobbyNetHandler::requestResetRating()
	{
		m_enc->requestResetRatingC();
	}

	void LobbyNetHandler::requestChangeShowEmail( bool show )
	{
		m_enc->requestChangeShowEmailC(show);
	}

	void LobbyNetHandler::requestChangeFullName( const std::string& first, const std::string& last )
	{
		m_enc->requestChangeFullNameC(first,last);
	}

	void LobbyNetHandler::requestAccountSettingsS( const std::string& firstName, const std::string& lastName, bool showEmail )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->showAccountSettings(firstName,lastName,showEmail);
	}

	void LobbyNetHandler::accountSettingsChangedS( int dialogToClear )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->accountSettingsChanged(dialogToClear);
	}

	void LobbyNetHandler::changeAvatar( int avatar )
	{
		m_enc->changeAvatarC(avatar);
	}

	void LobbyNetHandler::changeAvatarS( int newAvatar, const std::string& playerName )
	{
		ClientPlayer* me = m_shared->getPlayerManager()->getSelfPlayer();
		if(me && me->getUsername() == playerName)
			me->setAvatar(newAvatar);

		ClientPlayer* p = m_shared->getPlayerManager()->getPlayerByName(playerName);
		if(p)
			p->setAvatar(newAvatar);

		DISPATCH_LOBBY_EVENT
			(*it)->avatarChanged(newAvatar,playerName);
	}

	void LobbyNetHandler::setAvatarS()
	{
		DISPATCH_LOBBY_EVENT
			(*it)->forceSetAvatar();
	}

	void LobbyNetHandler::showLeaderboard( int pageNo, int numPerPage, const std::string& dataColumn, bool alreadyShowing )
	{
		m_enc->showLeaderboardC(pageNo,numPerPage, dataColumn,alreadyShowing);
	}

	void LobbyNetHandler::showLeaderboardS( int pageNo, int numPerPage, const std::vector<std::string>& rank,
		const std::vector<std::string>& name, const std::vector<std::string>& rating, const std::string& dataColumn, bool alreadyShowing )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->showLeaderboardResponse(pageNo,numPerPage,rank,name,rating,dataColumn,alreadyShowing);
	}

	void LobbyNetHandler::changeTheme( int theme )
	{
		m_enc->changeThemeC(theme);
	}


	void LobbyNetHandler::sendFeeOptionsS( const std::vector<int>& fees, std::vector<int> minRatingValues, int highStakesFee )
	{
		m_shared->getLobbyCache()->setTableFees(fees);
		m_shared->getLobbyCache()->setHighStakesValue(highStakesFee);
		m_shared->getLobbyCache()->setMinRatingValues(minRatingValues);
		DISPATCH_LOBBY_EVENT
			(*it)->receiveFeeOptions(fees,minRatingValues,highStakesFee);
	}

	void LobbyNetHandler::showCoinsDialogS( const std::vector<std::string>& coinAmounts, 
		const std::vector<std::string>& prices, const std::vector<std::string>& salePercents, const std::vector<std::string>& coinPercents, const std::vector<std::string>& coinUrls )
	{
#ifdef CGE_IPHONE
		std::vector<IAPProduct> products = IAPHandler::getInstance().getProducts();
		std::vector<std::string> titles;
		std::vector<std::string> discountA;
		std::vector<std::string> discountB;
		std::vector<std::string> iosprices;
		std::vector<std::string> prodID;

		for(int i = 0; i < products.size(); i++)
		{
			titles.push_back(products[i].name);
			discountB.push_back(products[i].bonusPercent);
			discountA.push_back("");
			iosprices.push_back(products[i].priceSymbol + products[i].price + " " + products[i].priceLocale);
			prodID.push_back(products[i].productID);
		}
		DISPATCH_LOBBY_EVENT
			(*it)->showCoinsDialog(titles, iosprices, discountA, discountB, prodID);
#else

		DISPATCH_LOBBY_EVENT
			(*it)->showCoinsDialog(coinAmounts,prices,salePercents,coinPercents,coinUrls);
#endif
	}

	void LobbyNetHandler::requestCoinsDialog()
	{
		m_enc->showCoinsDialogC();
	}

	void LobbyNetHandler::removeGift()
	{
		m_enc->removeGiftC();
	}

	void LobbyNetHandler::sendCoinAmountS( int64 amount, bool update, bool fromPurchase, bool spending )
	{
		m_shared->setCoinAmount(amount);
		DISPATCH_LOBBY_EVENT
			(*it)->coinAmountChanged(amount,update,fromPurchase,spending);
	}

	void LobbyNetHandler::showThemeDialogS( bool showFirstMessage )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->showThemeDialog(showFirstMessage);
	}

	void LobbyNetHandler::sendFeedback( const std::string& otherPlayer,const std::string& comment, const std::string& data )
	{
		m_enc->sendFeedbackC(otherPlayer,comment,data);
	}

	void LobbyNetHandler::requestCoinAmount( bool purchase )
	{
        if(m_shared->getPlayerManager()->getSelfPlayer()->getUsername().length() > 0)
		m_enc->sendCoinAmountC(purchase);
	}

	void LobbyNetHandler::sendStoreLink( const std::string& storeID )
	{
		m_enc->sendStoreLinkC(storeID);
	}

	void LobbyNetHandler::sendStoreLinkS( const std::string& link )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->receiveStoreLink(link);
	}

	void LobbyNetHandler::fetchLoginServerMS( const std::string& username, const std::string& name, const std::string& ip, int port )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->receiveLoginServer(username,name,ip,port);
	}

	void LobbyNetHandler::requestLoginServer( const std::string& username )
	{
		m_enc->fetchLoginServerMC(username);
	}

	void LobbyNetHandler::requestServerList()
	{
		m_enc->requestServerListC();
	}

	void LobbyNetHandler::fetchServerListMS( const std::string& username,
		const std::vector<std::string>& names, const std::vector<std::string>& ips, 
		const std::vector<int>& ports, const std::vector<int>& capacities, 
		const std::vector<int>& numUsers, const std::vector<int>& numFriends )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->receiveServerList(username,names,ips,
			ports,capacities,numUsers,numFriends);
	}

	void LobbyNetHandler::requestServerFull( const std::string& ip, int port )
	{
		m_enc->serverFullC(ip,port);
	}

	void LobbyNetHandler::serverFullS( const std::string& serverName, const std::string& ip, int port, bool full )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->receiveServerFull(serverName,ip,port,full);
	}

	void LobbyNetHandler::sendCreateAccount( const std::string& username, 
		const std::string& password, const std::string& email,
		const std::string& firstName, const std::string& lastName)
	{
		m_enc->createAccountC(username,password,email,firstName,lastName);
	}

	void LobbyNetHandler::requestFriendServerList()
	{
		m_enc->requestFriendServerListC();
	}

	void LobbyNetHandler::fetchFriendServerListMS( const std::string& username, const std::vector<std::string>& names, const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<std::string>& serverNames )
	{
		DISPATCH_LOBBY_EVENT
			(*it)->receiveFriendServerList(username,names,ips,
			ports,capacities,numUsers,serverNames);
	}

	void LobbyNetHandler::requestSubS( int tableNo, int seatNo, const std::string& response, long long minCoin,long long subWinAmount,int minRating )
	{
		if(minCoin == 0 && subWinAmount == 0 && minRating == 0) {
			DISPATCH_LOBBY_EVENT
				(*it)->subSeatLackedCoins(response);
		}
	}

	void LobbyNetHandler::requestPlayerS( int tableNo, const std::string& playerName )
	{
		std::string p = playerName;
		if(playerName == "server") {
			p = m_shared->getLanguageManager()->getElement(playerName);
		}

		if(m_shared->getSceneMessenger()->getCurrentScene() == SceneManagerMessage::GAME &&
			m_shared->getSceneMessenger()->willSceneChange())
		{
			m_shared->queueRequest(p,tableNo);
		}
		else if(m_shared->getSceneMessenger()->getCurrentScene() == SceneManagerMessage::GAME &&
			tableNo == m_shared->getPlayerManager()->getSelfPlayer()->getTable())
		{
			m_shared->queueRequest(p,tableNo);
		}
		else
		{
			DISPATCH_LOBBY_EVENT
				(*it)->receivePlayerRequest(tableNo,p);
		}
	}

	void LobbyNetHandler::sendPlayerRequest()
	{
		m_enc->requestPlayerC();
	}

	void LobbyNetHandler::badAsyncRequestS()
	{
		DISPATCH_LOBBY_EVENT
			(*it)->badAsyncRequest();
	}

	void LobbyNetHandler::returnToGameS()
	{
		DISPATCH_LOBBY_EVENT
			(*it)->requestReturnToGame();
	}

	void LobbyNetHandler::respondReturnToGame( bool ret )
	{
		m_enc->returnToGameC(ret);
	}

}
