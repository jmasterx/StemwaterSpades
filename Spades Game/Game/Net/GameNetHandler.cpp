#include "Game/Net/GameNetHandler.hpp"
#include <iostream>
#include "Game/Card/Card.hpp"
#ifdef CGE_IPHONE
#include "Game/platform/IAPHandler.h"
#endif
namespace cge
{
	GameNetHandler::GameNetHandler( NetEventEncoder* enc, ClientShared* shared )
		: m_enc(enc),m_shared(shared)
	{

	}

	GameNetHandler::~GameNetHandler(void)
	{
	}

	void GameNetHandler::sendChatMessageS( const std::string& playerName, const std::string& message,ChatAreaEnum area )
	{
		if(m_shared->getSceneMessenger()->getCurrentScene() == SceneManagerMessage::GAME && area == TABLE_CHAT)
		{
			area = GAME_CHAT;
		}

		if(area == GAME_CHAT && m_shared->getSceneMessenger()->getCurrentScene() == SceneManagerMessage::GAME)
		{
			TableMessage t;
			t.m_username = playerName;
			t.m_message = message;
			DISPATCH_GAME_EVENT
				(*it)->determineGamePlayerColor(t);

			m_shared->getPlayerManager()->queueTableMessage(t);

			DISPATCH_GAME_EVENT
				(*it)->chatReceiveMessage(message,playerName,area,t.m_gameColor);
		}
		else if(area == LOBBY_CHAT && m_shared->getSceneMessenger()->getCurrentScene() == SceneManagerMessage::GAME)
		{
			TableMessage t;
			t.m_username = playerName;
			t.m_message = message;
			DISPATCH_GAME_EVENT
				(*it)->determineLobbyPlayerColor(t);

			DISPATCH_GAME_EVENT
				(*it)->chatReceiveMessage(message,playerName,area,t.m_lobbyColor);
		}

	}

	void GameNetHandler::gameMessageSend( SpadesCSMessageEnum message, const std::vector<int>& params )
	{
		m_enc->gameMessageC(message,params);
	}

	void GameNetHandler::gameMessageS( SpadesCSMessageEnum message,const std::vector<int>& params, int player )
	{
		DISPATCH_GAME_EVENT
			(*it)->gameMessageReceive(message,params,player);
	}

	bool GameNetHandler::isMutedImpl( const std::string& playerName, bool* result )
	{
		*result = m_shared->getPlayerManager()->isMuted(playerName);
		return true;
	}

	void GameNetHandler::setMuted( const std::string& playerName, bool muted )
	{
		m_shared->getPlayerManager()->setMuted(playerName,muted);
	}

	void GameNetHandler::dispatchTableMessages()
	{
		for(int i = m_shared->getPlayerManager()->getTableMessageQueue().size() - 1; i >= 0 ; --i)
		{
			
			TableMessage* t = &m_shared->getPlayerManager()->getTableMessageQueue()[i];
			if(t->m_gameColorType == TMC_UNDETERMINED)
			{
				DISPATCH_GAME_EVENT
					(*it)->determineGamePlayerColor(*t);
			}

			DISPATCH_GAME_EVENT
				(*it)->chatReceiveMessage(t->m_message,t->m_username,GAME_CHAT,t->m_gameColor);
		}
	}

	void GameNetHandler::sendClientStateS( const GameClientState& state )
	{
		m_shared->getNewGameData()->setRestoring(true);
		m_shared->getNewGameData()->setRestoreState(state);

	}

	bool GameNetHandler::getTableNumberImpl( int* table )
	{
		if(m_shared->getPlayerManager()->getSelfPlayer()->isWatching() )
		*table = m_shared->getPlayerManager()->getSelfPlayer()->getWatchTableNo();
		else
		*table = m_shared->getPlayerManager()->getSelfPlayer()->getTable();
		return true;
	}

	bool GameNetHandler::getGameInfoImpl( SpadesGameInfo* info )
	{
		*info = m_shared->getNewGameData()->getGameInfo();
		return true;
	}

	void GameNetHandler::playerAbandonedGameS( const std::string& username )
	{
		DISPATCH_GAME_EVENT
			(*it)->playerAbandonedGame(username);
	}

	void GameNetHandler::abandonGame()
	{
		m_enc->playerAbandonedGameC();
	}

	void GameNetHandler::stopWatchingTable()
	{
		m_enc->watchTableC(-1);
	}

	void GameNetHandler::gameOverS( 
		const std::vector<int>& winningTeamOrder,
		const std::vector<int>& finalTeamScores,
		const std::vector<int>& newPlayerRatings, 
		const std::vector<std::string>& players, int64 netChange, int64 newAmount, int timeout )
	{
		DISPATCH_GAME_EVENT
			(*it)->gameIsOver(winningTeamOrder,finalTeamScores,newPlayerRatings,players,
			netChange, newAmount,timeout);
	}

	void GameNetHandler::wantPlayAgain()
	{
		m_enc->playerWantsPlayAgainC();
	}

	void GameNetHandler::wantLeave()
	{
		m_enc->playerStoodUpC();
	}

	void GameNetHandler::readyToPlayAgainS()
	{
		DISPATCH_GAME_EVENT
			(*it)->readyToPlayAgain();
	}

	void GameNetHandler::playerWatchingTableS( int tableNo, const std::string& player )
	{
		ClientPlayer* me = m_shared->getPlayerManager()->getSelfPlayer();

		if(player == me->getUsername() && tableNo == -1)
		{
			DISPATCH_GAME_EVENT
				(*it)->leaveFromTable();
		}
	}

	void GameNetHandler::enterGameS( const SpadesGameInfo& info )
	{
		readyToPlayAgainS();
	}

	void GameNetHandler::requestShowProfile( const std::string& playerName )
	{
		if(playerName == m_shared->getLanguageManager()->getElement("server"))
			return;

	//	m_enc->showProfileC(playerName);
	}

	void GameNetHandler::requestAddFriend( const std::string& playerName )
	{
		m_enc->modifyListC(playerName,FRIENDS_GROUP,true);
	}

	void GameNetHandler::requestRemoveFriend( const std::string& playerName )
	{
		m_enc->modifyListC(playerName,FRIENDS_GROUP,false);
	}

	void GameNetHandler::requestAddEnemy( const std::string& playerName )
	{
		m_enc->modifyListC(playerName,ENEMIES_GROUP,true);
	}

	void GameNetHandler::requestRemoveEnemy( const std::string& playerName )
	{
		m_enc->modifyListC(playerName,ENEMIES_GROUP,false);
	}

	bool GameNetHandler::isFriendGImpl( const std::string& playerName, bool* result )
	{
		*result = m_shared->getPlayerManager()->isFriend(playerName);
		return true;
	}

	bool GameNetHandler::isEnemyGImpl( const std::string& playerName, bool* result )
	{
		*result = m_shared->getPlayerManager()->isEnemy(playerName);
		return true;
	}

	bool GameNetHandler::getPlayerByNameGImpl( ClientPlayer* &player, const std::string& name )
	{
		player = m_shared->getPlayerManager()->getPlayerByName(name);
		return true;
	}

	void GameNetHandler::sendChatTypingS( const std::string& playerName, ChatAreaEnum area )
	{
		if(m_shared->getSceneMessenger()->getCurrentScene() == SceneManagerMessage::GAME && area == TABLE_CHAT)
		{
			area = GAME_CHAT;
		}

		if(area == GAME_CHAT && m_shared->getSceneMessenger()->getCurrentScene() == SceneManagerMessage::GAME)
		{
			TableMessage t;
			t.m_username = playerName;
			DISPATCH_GAME_EVENT
				(*it)->determineGamePlayerColor(t);

			DISPATCH_GAME_EVENT
				(*it)->chatReceiveTyping(playerName,area,t.m_gameColor);
		}
	}

	void GameNetHandler::sendMessageS(
		const std::string& title, const std::string& message, bool titleLocalized,bool messageLocalized )
	{
		DISPATCH_GAME_EVENT
			(*it)->displayMessageFromServer(title,message,titleLocalized,messageLocalized);
	}

	void GameNetHandler::showProfileS( int avatar, const std::vector<std::string>& data )
	{
		DISPATCH_GAME_EVENT
			(*it)->showProfile(avatar,data);
	}

	void GameNetHandler::chatMessageFromServerS(
		const std::string& message, const std::string& arg, bool messageLocalized, ChatAreaEnum area )
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

	void GameNetHandler::kickPlayer( const std::string& name )
	{
		m_enc->kickPlayerC(name);
	}

	void GameNetHandler::sendGift( int playerNum, int giftID )
	{
		m_enc->sendGiftC(playerNum,giftID);
	}

	void GameNetHandler::sendGiftS( int fromPlayer, int toPlayer, int giftID )
	{
		DISPATCH_GAME_EVENT
			(*it)->receiveGift(fromPlayer,toPlayer,giftID);
	}

	void GameNetHandler::updateGiftS( int player,int giftID, int frame )
	{
		DISPATCH_GAME_EVENT
			(*it)->receiveGiftUpdate(player,giftID,frame);
	}

	void GameNetHandler::sendGiftUpdate()
	{
		m_enc->updateGiftC();
	}

	void GameNetHandler::showCoinsDialogS( const std::vector<std::string>& coinAmounts, const std::vector<std::string>& prices, const std::vector<std::string>& salePercents, const std::vector<std::string>& coinPercents, const std::vector<std::string>& coinUrls )
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
		DISPATCH_GAME_EVENT
			(*it)->showCoinsDialog(titles, iosprices, discountA, discountB, prodID);
#else

		DISPATCH_GAME_EVENT
			(*it)->showCoinsDialog(coinAmounts,prices,salePercents,coinPercents,coinUrls);
#endif
	}

	void GameNetHandler::requestShowGiftDialog( int playerPos )
	{
		m_enc->showGiftDialogC(playerPos);
	}

	void GameNetHandler::showGiftDialogS( int playerNum, const std::string& price )
	{
		DISPATCH_GAME_EVENT
			(*it)->displayGiftDialog(playerNum,price);
	}

	void GameNetHandler::sendCoinAmountS( int64 amount, bool update, bool fromPurchase, bool spending )
	{
		m_shared->setCoinAmount(amount);
		DISPATCH_GAME_EVENT
			(*it)->coinAmountChangedG(amount,update,fromPurchase,spending);
	}

	void GameNetHandler::gameTimeChangedS( int secondsElapsed )
	{
		DISPATCH_GAME_EVENT
			(*it)->gameTimeChanged(secondsElapsed);
	}

	void GameNetHandler::requestCoinAmount(bool purchase)
	{
		m_enc->sendCoinAmountC(purchase);
	}

	void GameNetHandler::badAsyncRequestS()
	{
		DISPATCH_GAME_EVENT
			(*it)->badAsyncG();
	}

	void GameNetHandler::requestStrikesS( int player, int time, int numStrikes, bool beginAction )
	{
		DISPATCH_GAME_EVENT
			(*it)->showPlayerStrike(player,numStrikes,time,beginAction);
	}

	void GameNetHandler::disconnectionReturnedS( bool leave )
	{
		DISPATCH_GAME_EVENT
			(*it)->disconnectionReturned(leave);
	}

}
