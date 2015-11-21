#include "Game/Net/NetEventEncoder.hpp"
namespace cge
{
	NetEventEncoder::NetEventEncoder(void)
		: m_client(NULL)
	{
	}

	NetEventEncoder::~NetEventEncoder(void)
	{
	}

	void NetEventEncoder::setServerClient( ServerClient* client )
	{
		m_client = client;
	}

	void NetEventEncoder::eventEncoded( const std::string& evt )
	{
		if(m_client)
		{
			m_client->send(evt);
		}

		m_lastEncodedMessage = evt;
	}

	void NetEventEncoder::eventEncoded()
	{
		eventEncoded(m_enc.endMessage());
	}

	void NetEventEncoder::sendChatMessageC( const std::string& message,ChatAreaEnum area )
	{
		m_enc.beginMessage(MSG_SEND_CHAT_MSG_C);
		m_enc.pushString(message);
		m_enc.pushInt(area);
		eventEncoded();
	}

	void NetEventEncoder::sendChatMessageS( const std::string& playerName, const std::string& message,ChatAreaEnum area )
	{
		m_enc.beginMessage(MSG_SEND_CHAT_MSG_S);
		m_enc.pushString(playerName);
		m_enc.pushString(message);
		m_enc.pushInt(area);
		eventEncoded();
	}

	void NetEventEncoder::requestTableC( int tableNo, int seatNo )
	{
		m_enc.beginMessage(MSG_REQUEST_TABLE_C);
		m_enc.pushInt(tableNo);
		m_enc.pushInt(seatNo);
		eventEncoded();
	}

	void NetEventEncoder::tableLockedS( int tableNo, int seatNo, bool locked )
	{
		m_enc.beginMessage(MSG_TABLE_LOCKED_S);
		m_enc.pushInt(tableNo);
		m_enc.pushInt(seatNo);
		m_enc.pushBool(locked);
		eventEncoded();
	}

	void NetEventEncoder::hostGameC( const SpadesGameInfo& info )
	{
		m_enc.beginMessage(MSG_HOST_GAME_C);
		m_enc.pushIntArray(info.toIntArray());
		eventEncoded();
	}

	void NetEventEncoder::tableStateChangedS( int tableNo, const SpadesGameInfo& info )
	{
		m_enc.beginMessage(MSG_TABLE_STATE_CHANGED_S);
		m_enc.pushInt(tableNo);
		m_enc.pushIntArray(info.toIntArray());
		eventEncoded();
	}

	void NetEventEncoder::modifyListC( const std::string& player, GroupTypeEnum group, bool add )
	{
		m_enc.beginMessage(MSG_MODIFY_LIST_C);
		m_enc.pushString(player);
		m_enc.pushInt(group);
		m_enc.pushBool(add);
		eventEncoded();
	}

	void NetEventEncoder::modifiedListS( const std::string& player, GroupTypeEnum group, bool add )
	{
		m_enc.beginMessage(MSG_MODIFIED_LIST_S);
		m_enc.pushString(player);
		m_enc.pushInt(group);
		m_enc.pushBool(add);
		eventEncoded();
	}

	void NetEventEncoder::playerRequestSitC( int tableNumber, int seatNumber )
	{
		m_enc.beginMessage(MSG_PLAYER_REQUEST_SIT_C);
		m_enc.pushInt(tableNumber);
		m_enc.pushInt(seatNumber);
		eventEncoded();
	}

	void NetEventEncoder::playerSatDownS( const std::string& player, int tableNumber, int seatNumber )
	{
		m_enc.beginMessage(MSG_PLAYER_SAT_DOWN_S);
		m_enc.pushString(player);
		m_enc.pushInt(tableNumber);
		m_enc.pushInt(seatNumber);
		eventEncoded();
	}

	void NetEventEncoder::playerStoodUpS( int tableNo, int seatNo )
	{
		m_enc.beginMessage(MSG_PLAYER_STOOD_UP_S);
		m_enc.pushInt(tableNo);
		m_enc.pushInt(seatNo);
		eventEncoded();
	}

	void NetEventEncoder::playerStoodUpC()
	{
		m_enc.beginMessage(MSG_PLAYER_STOOD_UP_C);
		eventEncoded();
	}

	void NetEventEncoder::gameMessageS( SpadesCSMessageEnum message, const std::vector<int>& params,int player )
	{
		m_enc.beginMessage(MSG_GAME_MESSAGE_S);
		m_enc.pushInt(message);
		m_enc.pushIntArray(params);
		m_enc.pushInt(player);
		eventEncoded();
	}

	void NetEventEncoder::gameMessageC( SpadesCSMessageEnum message, 
		const std::vector<int>& params )
	{
		m_enc.beginMessage(MSG_GAME_MESSAGE_C);
		m_enc.pushInt(message);
		m_enc.pushIntArray(params);
		eventEncoded();
	}

	const std::string& NetEventEncoder::getLastMessage() const
	{
		return m_lastEncodedMessage;
	}

	void NetEventEncoder::showProfileC( const std::string& player )
	{
		m_enc.beginMessage(MSG_SHOW_PROFILE_C);
		m_enc.pushString(player);
		eventEncoded();
	}

	void NetEventEncoder::loginRequestC( const std::string& userName, const std::string& password, int majorVersion, int minorVersion, int system )
	{
		m_enc.beginMessage(MSG_LOGIN_REQUEST_C);
		m_enc.pushString(userName);
		m_enc.pushString(password);
		m_enc.pushInt(majorVersion);
		m_enc.pushInt(minorVersion);
		m_enc.pushInt(system);
		eventEncoded();
	}

	void NetEventEncoder::loginResultS( bool success, const std::string& message, const std::string& username )
	{
		m_enc.beginMessage(MSG_LOGIN_RESULT_S);
		m_enc.pushBool(success);
		m_enc.pushString(message);
		m_enc.pushString(username);
		eventEncoded();
	}


	void NetEventEncoder::playerJoinedS( const SharedPlayer& player, GroupTypeEnum group )
	{
		m_enc.beginMessage(MSG_PLAYER_JOINED_S);
		encodeSharedPlayer(player);
		m_enc.pushInt((int)group);
		eventEncoded();
	}

	void NetEventEncoder::playerLeftS( const std::string& userName )
	{
		m_enc.beginMessage(MSG_PLAYER_LEFT_S);
		m_enc.pushString(userName);
		eventEncoded();
	}

	void NetEventEncoder::hostingDutiesChangedS( bool isHost )
	{
		m_enc.beginMessage(MSG_HOSTING_DUTIES_CHANGED_S);
		m_enc.pushBool(isHost);
		eventEncoded();
	}

	void NetEventEncoder::enterGameS( const SpadesGameInfo& info )
	{
		m_enc.beginMessage(MSG_ENTER_GAME_S);
		m_enc.pushIntArray(info.toIntArray());
		eventEncoded();
	}

	void NetEventEncoder::tableIsInactiveS( int tableNo )
	{
		m_enc.beginMessage(MSG_TABLE_IS_INACTIVE_S);
		m_enc.pushInt(tableNo);
		eventEncoded();
	}

	void NetEventEncoder::sendClientStateS( const GameClientState& state )
	{
	
		m_enc.beginMessage(MSG_SEND_CLIENT_STATE_S);
		m_enc.pushInt((int)state.getGameState());
		m_enc.pushInt(state.getNumPlayers());
		m_enc.pushIntArray(state.getCards());
		m_enc.pushIntArray(state.getOpponentCardCounts());
		m_enc.pushIntArray(state.getTeamScores());
		m_enc.pushIntArray(state.getTeamBags());
		m_enc.pushIntArray(state.getPassedCards());
		m_enc.pushIntArray(state.getReceivedCards());
		m_enc.pushIntArray(state.getPriorHand());
		m_enc.pushIntArray(state.getFilteredCards());
		m_enc.pushIntArray(state.getCardsOnTable());
		
		std::vector<int> pint;
		for(int i = 0; i < 4; ++i)
		{
			std::vector<SpadesPointEnum> p = state.getPlayerPoints(i);
			pint.clear();
			
			for(size_t j = 0; j < p.size();++j)
			{
				pint.push_back((int)p[j]);
			}

			m_enc.pushIntArray(pint);
		}

		m_enc.pushInt(state.getNumHandsPlayed());
		m_enc.pushInt(state.getDealer());
		m_enc.pushInt(state.getActivePlayer());
		m_enc.pushBool(state.isSpadesBroken());
		m_enc.pushBool(state.playerHasCards());
		m_enc.pushIntArray(state.getPlayerRoundBids());
		m_enc.pushInt(state.getBidRequester());
		m_enc.pushInt(state.getFirstCardPayer());
		m_enc.pushInt(state.getLastPointWinner());
		m_enc.pushInt(state.getLastPointReceiver());
		m_enc.pushInt(state.getLastPointType());
		m_enc.pushBool(state.isPlayerActive(0));
		m_enc.pushBool(state.isPlayerActive(1));
		m_enc.pushBool(state.isPlayerActive(2));
		m_enc.pushBool(state.isPlayerActive(3));
		m_enc.pushInt(state.getGiftID(0));
		m_enc.pushInt(state.getGiftID(1));
		m_enc.pushInt(state.getGiftID(2));
		m_enc.pushInt(state.getGiftID(3));
		m_enc.pushInt(state.getGiftFrame(0));
		m_enc.pushInt(state.getGiftFrame(1));
		m_enc.pushInt(state.getGiftFrame(2));
		m_enc.pushInt(state.getGiftFrame(3));

		eventEncoded();
	}

	void NetEventEncoder::canceledHostingRequestC()
	{
		m_enc.beginMessage(MSG_CANCEL_HOSTING_REQUEST_C);

		eventEncoded();
	}

	void NetEventEncoder::setBuffer()
	{
		setBuffer(m_lastEncodedMessage);
	}

	void NetEventEncoder::setBuffer( const std::string& s )
	{
		m_strBuffer.resize(s.length() + 1);
		m_strBuffer[m_strBuffer.size() - 1] = '\0';
		for (size_t i = 0; i < s.length(); ++i)
		{
			m_strBuffer[i] = s[i];
		}
	}

	char* NetEventEncoder::getBuffer()
	{
		return &m_strBuffer[0];
	}

	void NetEventEncoder::beginSendLobbyDataS( int tableBegin, int numTables )
	{
		m_enc.beginMessage(MSG_BEGIN_SEND_LOBBY_DATA_S);
		m_enc.pushInt(tableBegin);
		m_enc.pushInt(numTables);
		eventEncoded();
	}

	void NetEventEncoder::encodeSharedPlayer( const SharedPlayer& player )
	{
		m_enc.pushString(player.getUsername());
		m_enc.pushInt(player.getTableId());
		m_enc.pushInt(player.getSeat());
		m_enc.pushInt(player.getWatchTableId());
		m_enc.pushInt(player.getAvatar());
	}

	void NetEventEncoder::endSendLobbyDataS()
	{
		m_enc.beginMessage(MSG_END_SEND_LOBBY_DATA_S);
		eventEncoded();
	}

	void NetEventEncoder::readyToReceiveGameDataC()
	{
		m_enc.beginMessage(MSG_READY_TO_RECEIVE_GAME_DATA_C);
		eventEncoded();
	}

	void NetEventEncoder::readyToReceiveLobbyDataC()
	{
		m_enc.beginMessage(MSG_READY_TO_RECEIVE_LOBBY_DATA_C);
		eventEncoded();
	}

	void NetEventEncoder::tableBeingSetupS( int tableNo )
	{
		m_enc.beginMessage(MSG_TABLE_BEING_SETUP_S);
		m_enc.pushInt(tableNo);
		eventEncoded();
	}

	void NetEventEncoder::playerAbandonedGameS( const std::string& username )
	{
		m_enc.beginMessage(MSG_PLAYER_ABANDONED_GAME_S);
		m_enc.pushString(username);
		eventEncoded();
	}

	void NetEventEncoder::playerAbandonedGameC()
	{
		m_enc.beginMessage(MSG_PLAYER_ABANDONED_GAME_C);
		eventEncoded();
	}

	void NetEventEncoder::seatIsSubSeatS( int tableNo, int seatNo, bool subseat )
	{
		m_enc.beginMessage(MSG_SEAT_IS_SUB_SEAT_S);
		m_enc.pushInt(tableNo);
		m_enc.pushInt(seatNo);
		m_enc.pushBool(subseat);
		eventEncoded();
	}

	void NetEventEncoder::sendInviteS( int tableNo, const std::string& player )
	{
		m_enc.beginMessage(MSG_SEND_INVITE_S);
		m_enc.pushInt(tableNo);
		m_enc.pushString(player);
		eventEncoded();
	}

	void NetEventEncoder::sendInviteC( int tableNo, const std::string& player )
	{
		m_enc.beginMessage(MSG_SEND_INVITE_C);
		m_enc.pushInt(tableNo);
		m_enc.pushString(player);
		eventEncoded();
	}

	void NetEventEncoder::watchTableC( int tableNo )
	{
		m_enc.beginMessage(MSG_WATCH_TABLE_C);
		m_enc.pushInt(tableNo);
		eventEncoded();
	}

	void NetEventEncoder::playerWatchingTableS( int tableNo, const std::string& player )
	{
		m_enc.beginMessage(MSG_PLAYER_WATCHING_TABLE_S);
		m_enc.pushInt(tableNo);
		m_enc.pushString(player);
		eventEncoded();
	}

	void NetEventEncoder::gameOverS( const std::vector<int>& winningTeamOrder, const std::vector<int>& finalTeamScores, 
		const std::vector<int>& newPlayerRatings, const std::vector<std::string>& players, long long netCoinChange, long long newCoinAmount, int timeout )
	{
		m_enc.beginMessage(MSG_GAME_OVER_S);
		m_enc.pushIntArray(winningTeamOrder);
		m_enc.pushIntArray(finalTeamScores);
		m_enc.pushIntArray(newPlayerRatings);
		m_enc.pushStringArray(players);
		m_enc.pushInt64(netCoinChange);
		m_enc.pushInt64(newCoinAmount);
		m_enc.pushInt(timeout);
		eventEncoded();
	}

	void NetEventEncoder::playerWantsPlayAgainC()
	{
		m_enc.beginMessage(MSG_PLAYER_WANTS_PLAY_AGAIN_C);
		eventEncoded();
	}

	void NetEventEncoder::readyToPlayAgainS()
	{
		m_enc.beginMessage(MSG_READY_TO_PLAY_AGAIN_S);
		eventEncoded();
	}

	void NetEventEncoder::checkClientAliveSC()
	{
		m_enc.beginMessage(MSG_IS_CLIENT_ALIVE_S);
		eventEncoded();
	}

	ServerClient* NetEventEncoder::getClient()
	{
		return m_client;
	}

	void NetEventEncoder::logoutC()
	{
		m_enc.beginMessage(MSG_LOGOUT_C);
		eventEncoded();
	}

	void NetEventEncoder::sendChatTypingC( ChatAreaEnum area )
	{
		m_enc.beginMessage(MSG_SEND_CHAT_TYPING_C);
		m_enc.pushInt(area);
		eventEncoded();
	}

	void NetEventEncoder::sendChatTypingS( const std::string& playerName, ChatAreaEnum area )
	{
		m_enc.beginMessage(MSG_SEND_CHAT_TYPING_S);
		m_enc.pushString(playerName);
		m_enc.pushInt(area);
		eventEncoded();
	}

	void NetEventEncoder::showProfileS( int avatar, const std::vector<std::string>& data )
	{
		m_enc.beginMessage(MSG_SHOW_PROFILE_S);
		m_enc.pushInt(avatar);
		m_enc.pushStringArray(data);
		eventEncoded();
	}

	void NetEventEncoder::playerAcceptedTermsC( bool accepted )
	{
		m_enc.beginMessage(MSG_ACCEPT_TERMS_C);
		m_enc.pushBool(accepted);
		eventEncoded();
	}

	void NetEventEncoder::createAccountC( const std::string& username, const std::string& password, const std::string& email, const std::string& firstName, const std::string& lastName )
	{
		m_enc.beginMessage(MSG_CREATE_ACCOUNT_C);
		m_enc.pushString(username);
		m_enc.pushString(password);
		m_enc.pushString(email);
		m_enc.pushString(firstName);
		m_enc.pushString(lastName);
		eventEncoded();
	}

	void NetEventEncoder::createAccountS( bool success, bool usernameTaken,bool badUsername,bool badPassword,bool badEmail, bool badGender, bool badBirthday, bool tooYoung, bool emailNeedsVerification )
	{
		m_enc.beginMessage(MSG_CREATE_ACCOUNT_S);
		m_enc.pushBool(success);
		m_enc.pushBool(usernameTaken);
		m_enc.pushBool(badUsername);
		m_enc.pushBool(badPassword);
		m_enc.pushBool(badEmail);
		m_enc.pushBool(badGender);
		m_enc.pushBool(badBirthday);
		m_enc.pushBool(tooYoung);
		m_enc.pushBool(emailNeedsVerification);
		eventEncoded();
	}

	void NetEventEncoder::playerMustAcceptTermsS( const std::string& terms )
	{
		m_enc.beginMessage(MSG_ACCEPT_TERMS_S);
		m_enc.pushString(terms);
		eventEncoded();
	}

	void NetEventEncoder::sendMessageS( 
		const std::string& title, const std::string& message, bool titleLocalized,bool messageLocalized )
	{
		m_enc.beginMessage(MSG_SEND_MESSSAGE_S);
		m_enc.pushString(title);
		m_enc.pushString(message);
		m_enc.pushBool(titleLocalized);
		m_enc.pushBool(messageLocalized);
		eventEncoded();
	}

	void NetEventEncoder::waitForSubS( bool cancel )
	{
		m_enc.beginMessage(MSG_WAIT_FOR_SUB_S);
		m_enc.pushBool(cancel);
		eventEncoded();
	}

	void NetEventEncoder::waitForSubC( bool stay )
	{
		m_enc.beginMessage(MSG_WAIT_FOR_SUB_C);
		m_enc.pushBool(stay);
		eventEncoded();
	}

	void NetEventEncoder::chatMessageFromServerS( 
		const std::string& message, const std::string& arg, bool messageLocalized, ChatAreaEnum area )
	{
		m_enc.beginMessage(MSG_CHAT_FROM_SERVER_S);
		m_enc.pushString(message);
		m_enc.pushString(arg);
		m_enc.pushBool(messageLocalized);
		m_enc.pushInt(area);
		eventEncoded();
	}

	void NetEventEncoder::kickPlayerC( const std::string& playerName )
	{
		m_enc.beginMessage(MSG_KICK_PLAYER_C);
		m_enc.pushString(playerName);
		eventEncoded();

	}

	void NetEventEncoder::requestAccountSettingsC()
	{
		m_enc.beginMessage(MSG_REQUEST_ACCOUNT_SETTINGS_C);
		eventEncoded();
	}

	void NetEventEncoder::requestAccountSettingsS( const std::string& firstName, const std::string& lastName, bool showEmail )
	{
		m_enc.beginMessage(MSG_REQUEST_ACCOUNT_SETTINGS_S);
		m_enc.pushString(firstName);
		m_enc.pushString(lastName);
		m_enc.pushBool(showEmail);
		eventEncoded();
	}

	void NetEventEncoder::requestChangePasswordC( const std::string& oldPassword, const std::string& newPassword )
	{
		m_enc.beginMessage(MSG_REQUEST_CHANGE_PASSWORD_C);
		m_enc.pushString(oldPassword);
		m_enc.pushString(newPassword);
		eventEncoded();
	}

	void NetEventEncoder::requestChangeEmailC( const std::string& newEmail)
	{
		m_enc.beginMessage(MSG_REQUEST_CHANGE_EMAIL_C);
		m_enc.pushString(newEmail);
		eventEncoded();
	}

	void NetEventEncoder::requestResetRatingC()
	{
		m_enc.beginMessage(MSG_REQUEST_RESET_RATING_C);
		eventEncoded();
	}

	void NetEventEncoder::requestChangeShowEmailC( bool show )
	{
		m_enc.beginMessage(MSG_REQUEST_CHANGE_SHOW_EMAIL_C);
		m_enc.pushBool(show);
		eventEncoded();
	}

	void NetEventEncoder::requestChangeFullNameC( const std::string& first, const std::string& last )
	{
		m_enc.beginMessage(MSG_REQUEST_CHANGE_NAME_C);
		m_enc.pushString(first);
		m_enc.pushString(last);
		eventEncoded();
	}

	void NetEventEncoder::accountSettingsChangedS( int dialogToClear )
	{
		m_enc.beginMessage(MSG_ACCOUNT_SETTINGS_CHANGED_S);
		m_enc.pushInt(dialogToClear);
		eventEncoded();
	}

	void NetEventEncoder::changeAvatarC( int newAvatar )
	{
		m_enc.beginMessage(MSG_CHANGE_AVATAR_C);
		m_enc.pushInt(newAvatar);
		eventEncoded();
	}

	void NetEventEncoder::changeAvatarS( int newAvatar, const std::string& playerName )
	{
		m_enc.beginMessage(MSG_CHANGE_AVATAR_S);
		m_enc.pushInt(newAvatar);
		m_enc.pushString(playerName);
		eventEncoded();
	}

	void NetEventEncoder::setAvatarS()
	{
		m_enc.beginMessage(MSG_SET_AVATAR_S);
		eventEncoded();
	}

	void NetEventEncoder::showLeaderboardC( int pageNo, int numPerPage, const std::string& dataColumn, bool alreadyShowing )
	{
		m_enc.beginMessage(MSG_SHOW_LEADERBOARD_C);
		m_enc.pushInt(pageNo);
		m_enc.pushInt(numPerPage);
		m_enc.pushString(dataColumn);
		m_enc.pushBool(alreadyShowing);
		eventEncoded();
	}

	void NetEventEncoder::showLeaderboardS( int pageNo, int numPerPage, const std::vector<std::string>& rank, 
		const std::vector<std::string>& name, const std::vector<std::string>& rating, const std::string& dataColumn, bool alreadyShowing )
	{
		m_enc.beginMessage(MSG_SHOW_LEADERBOARD_S);
		m_enc.pushInt(pageNo);
		m_enc.pushInt(numPerPage);
		m_enc.pushStringArray(rank);
		m_enc.pushStringArray(name);
		m_enc.pushStringArray(rating);
		m_enc.pushString(dataColumn);
		m_enc.pushBool(alreadyShowing);
		eventEncoded();
	}

	void NetEventEncoder::changeThemeC( int newTheme )
	{
		m_enc.beginMessage(MSG_CHANGE_THEME_C);
		m_enc.pushInt(newTheme);
		eventEncoded();
	}

	void NetEventEncoder::sendGiftC( int playerNum, int giftID )
	{
		m_enc.beginMessage(MSG_SEND_GIFT_C);
		m_enc.pushInt(playerNum);
		m_enc.pushInt(giftID);
		eventEncoded();
	}

	void NetEventEncoder::sendGiftS( int fromPlayer, int toPlayer, int giftID )
	{
		m_enc.beginMessage(MSG_SEND_GIFT_S);
		m_enc.pushInt(fromPlayer);
		m_enc.pushInt(toPlayer);
		m_enc.pushInt(giftID);
		eventEncoded();
	}

	void NetEventEncoder::updateGiftC()
	{
		m_enc.beginMessage(MSG_UPDATE_GIFT_C);
		eventEncoded();
	}

	void NetEventEncoder::updateGiftS( int player,int giftID, int frame )
	{
		m_enc.beginMessage(MSG_UPDATE_GIFT_S);
		m_enc.pushInt(player);
		m_enc.pushInt(giftID);
		m_enc.pushInt(frame);
		eventEncoded();
	}

	void NetEventEncoder::sendFeeOptionsS( const std::vector<int>& fees, std::vector<int> minRatingValues, int highStakesFee )
	{
		m_enc.beginMessage(MSG_SEND_FEE_OPTIONS_S);
		m_enc.pushIntArray(fees);
		m_enc.pushIntArray(minRatingValues);
		m_enc.pushInt(highStakesFee);
		eventEncoded();
	}

	void NetEventEncoder::showCoinsDialogS( const std::vector<std::string>& coinAmounts,
		const std::vector<std::string>& prices, const std::vector<std::string>& salePercents,
		const std::vector<std::string>& coinPercents, const std::vector<std::string>& coinUrls )
	{
		m_enc.beginMessage(MSG_SHOW_COINS_DIALOG_S);
		m_enc.pushStringArray(coinAmounts);
		m_enc.pushStringArray(prices);
		m_enc.pushStringArray(salePercents);
		m_enc.pushStringArray(coinPercents);
		m_enc.pushStringArray(coinUrls);
		eventEncoded();
	}

	void NetEventEncoder::showCoinsDialogC()
	{
		m_enc.beginMessage(MSG_SHOW_COINS_DIALOG_C);
		eventEncoded();
	}

	void NetEventEncoder::showGiftDialogC( int playerNum )
	{
		m_enc.beginMessage(MSG_SHOW_GIFT_DIALOG_C);
		m_enc.pushInt(playerNum);
		eventEncoded();
	}

	void NetEventEncoder::showGiftDialogS( int playerNum, const std::string& price )
	{
		m_enc.beginMessage(MSG_SHOW_GIFT_DIALOG_S);
		m_enc.pushInt(playerNum);
		m_enc.pushString(price);
		eventEncoded();
	}

	void NetEventEncoder::removeGiftC()
	{
		m_enc.beginMessage(MSG_REMOVE_GIFT_C);
		eventEncoded();
	}

	void NetEventEncoder::sendCoinAmountS( long long amount, bool update, bool fromPurchase, bool spending )
	{
		m_enc.beginMessage(MSG_SEND_COIN_AMOUNT_S);
		m_enc.pushInt64(amount);
		m_enc.pushBool(update);
		m_enc.pushBool(fromPurchase);
		m_enc.pushBool(spending);
		eventEncoded();
	}

	void NetEventEncoder::sendCoinAmountC( bool purchase )
	{
		m_enc.beginMessage(MSG_SEND_COIN_AMOUNT_C);
		m_enc.pushBool(purchase);
		eventEncoded();
	}

	void NetEventEncoder::showThemeDialogS( bool showFirstMessage )
	{
		m_enc.beginMessage(MSG_SHOW_THEME_DIALOG_S);
		m_enc.pushInt(showFirstMessage);
		eventEncoded();
	}

	void NetEventEncoder::gameTimeChangedS( int newTime )
	{
		m_enc.beginMessage(MSG_GAME_TIME_CHANGED_S);
		m_enc.pushInt(newTime);
		eventEncoded();
	}

	void NetEventEncoder::sendFeedbackC( const std::string& otherPlayer,const std::string& comment, const std::string& data )
	{
		m_enc.beginMessage(MSG_SEND_FEEDBACK_C);
		m_enc.pushString(otherPlayer);
		m_enc.pushString(comment);
		m_enc.pushString(data);
		eventEncoded();
	}

	void NetEventEncoder::sendStoreLinkC( const std::string& storeID )
	{
		m_enc.beginMessage(MSG_SEND_LINK_C);
		m_enc.pushString(storeID);
		eventEncoded();
	}

	void NetEventEncoder::sendStoreLinkS( const std::string& link )
	{
		m_enc.beginMessage(MSG_SEND_LINK_S);
		m_enc.pushString(link);
		eventEncoded();
	}

	void NetEventEncoder::registerServerMC( const std::string& name, const std::string& ip, int port,
		int capacity, const std::vector<std::string>& currentUsers, const std::string& secretToken, int leagueID )
	{
		m_enc.beginMessage(MSG_REGISTER_SERVER_MC);
		m_enc.pushString(name);
		m_enc.pushString(ip);
		m_enc.pushInt(port);
		m_enc.pushInt(capacity);
		m_enc.pushStringArray(currentUsers);
		m_enc.pushString(secretToken);
		m_enc.pushInt(leagueID);
		eventEncoded();
	}

	void NetEventEncoder::playerLoggedInMC( const std::string& username)
	{
		m_enc.beginMessage(MSG_PLAYER_LOGGED_IN_MC);
		m_enc.pushString(username);
		eventEncoded();
	}

	void NetEventEncoder::playerLoggedOutMC( const std::string& username )
	{
		m_enc.beginMessage(MSG_PLAYER_LOGGED_OUT_MC);
		m_enc.pushString(username);
		eventEncoded();
	}

	void NetEventEncoder::fetchLoginServerMC( const std::string& username )
	{
		m_enc.beginMessage(MSG_FETCH_LOGIN_SERVER_MC);
		m_enc.pushString(username);
		eventEncoded();
	}

	void NetEventEncoder::fetchLoginServerMS( const std::string& username, const std::string& name, const std::string& ip, int port )
	{
		m_enc.beginMessage(MSG_FETCH_LOGIN_SERVER_MS);
		m_enc.pushString(username);
		m_enc.pushString(name);
		m_enc.pushString(ip);
		m_enc.pushInt(port);
		eventEncoded();
	}

	void NetEventEncoder::fetchServerListMC( const std::string& username )
	{
		m_enc.beginMessage(MSG_FETCH_SERVER_LIST_MC);
		m_enc.pushString(username);
		eventEncoded();
	}

	void NetEventEncoder::fetchServerListMS( 
		const std::string& username, 
		const std::vector<std::string>& names, 
		const std::vector<std::string>& ips, 
		const std::vector<int>& ports, 
		const std::vector<int>& capacities, 
		const std::vector<int>& numUsers, 
		const std::vector<int>& numFriends )
	{
		m_enc.beginMessage(MSG_FETCH_SERVER_LIST_MS);
		m_enc.pushString(username);
		m_enc.pushStringArray(names);
		m_enc.pushStringArray(ips);
		m_enc.pushIntArray(ports);
		m_enc.pushIntArray(capacities);
		m_enc.pushIntArray(numUsers);
		m_enc.pushIntArray(numFriends);
		eventEncoded();
	}

	void NetEventEncoder::requestServerListC()
	{
		m_enc.beginMessage(MSG_REQUEST_SERVER_LIST_C);
		eventEncoded();
	}

	void NetEventEncoder::serverFullC( const std::string& ip, int port )
	{
		m_enc.beginMessage(MSG_SERVER_FULL_C);
		m_enc.pushString(ip);
		m_enc.pushInt(port);
		eventEncoded();
	}

	void NetEventEncoder::serverFullMC( const std::string& username, const std::string& ip, int port )
	{
		m_enc.beginMessage(MSG_SERVER_FULL_MC);
		m_enc.pushString(username);
		m_enc.pushString(ip);
		m_enc.pushInt(port);
		eventEncoded();
	}

	void NetEventEncoder::serverFullMS( const std::string& username, const std::string& serverName, const std::string& ip, int port, bool full )
	{
		m_enc.beginMessage(MSG_SERVER_FULL_MS);
		m_enc.pushString(username);
		m_enc.pushString(serverName);
		m_enc.pushString(ip);
		m_enc.pushInt(port);
		m_enc.pushBool(full);
		eventEncoded();
	}

	void NetEventEncoder::serverFullS( const std::string& serverName, const std::string& ip, int port, bool full )
	{
		m_enc.beginMessage(MSG_SERVER_FULL_S);
		m_enc.pushString(serverName);
		m_enc.pushString(ip);
		m_enc.pushInt(port);
		m_enc.pushBool(full);
		eventEncoded();
	}

	void NetEventEncoder::logoutPlayerMS( const std::string& username )
	{
		m_enc.beginMessage(MSG_LOGOUT_PLAYER_MS);
		m_enc.pushString(username);
		eventEncoded();
	}

	void NetEventEncoder::fetchFriendServerListMC( const std::string& username )
	{
		m_enc.beginMessage(MSG_FETCH_FRIEND_SERVER_LIST_MC);
		m_enc.pushString(username);
		eventEncoded();
	}

	void NetEventEncoder::fetchFriendServerListMS( const std::string& username, const std::vector<std::string>& names, const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<std::string>& serverNames )
	{
		m_enc.beginMessage(MSG_FETCH_FRIEND_SERVER_LIST_MS);
		m_enc.pushString(username);
		m_enc.pushStringArray(names);
		m_enc.pushStringArray(ips);
		m_enc.pushIntArray(ports);
		m_enc.pushIntArray(capacities);
		m_enc.pushIntArray(numUsers);
		m_enc.pushStringArray(serverNames);
		eventEncoded();
	}

	void NetEventEncoder::requestFriendServerListC()
	{
		m_enc.beginMessage(MSG_REQUEST_FRIEND_SERVER_LIST_C);
		eventEncoded();
	}

	void NetEventEncoder::requestSubC( int tableNo,int seatNo )
	{
		m_enc.beginMessage(MSG_REQUEST_SUB_C);
		m_enc.pushInt(tableNo);
		m_enc.pushInt(seatNo);
		eventEncoded();
	}

	void NetEventEncoder::requestSubS( int tableNo, int seatNo, const std::string& response, long long minCoin,long long subWinAmount,int minRating )
	{
		m_enc.beginMessage(MSG_REQUEST_SUB_S);
		m_enc.pushInt(tableNo);
		m_enc.pushInt(seatNo);
		m_enc.pushString(response);
		m_enc.pushInt64(minCoin);
		m_enc.pushInt64(subWinAmount);
		m_enc.pushInt(minRating);
		eventEncoded();
	}

	void NetEventEncoder::requestPlayerC()
	{
		m_enc.beginMessage(MSG_REQUEST_PLAYER_C);
		eventEncoded();
	}

	void NetEventEncoder::requestPlayerS( int tableNo, const std::string& playerName )
	{
		m_enc.beginMessage(MSG_REQUEST_PLAYER_S);
		m_enc.pushInt(tableNo);
		m_enc.pushString(playerName);
		eventEncoded();
	}

	void NetEventEncoder::badAsyncRequestS()
	{
		m_enc.beginMessage(MSG_BAD_ASYNC_S);
		eventEncoded();
	}

	void NetEventEncoder::requestStrikesC()
	{
		m_enc.beginMessage(MSG_REQUEST_STRIKES_C);
		eventEncoded();
	}

	void NetEventEncoder::requestStrikesS( int player, int time, int numStrikes, bool beginAction )
	{
		m_enc.beginMessage(MSG_REQUEST_STRIKES_S);
		m_enc.pushInt(player);
		m_enc.pushInt(time);
		m_enc.pushInt(numStrikes);
		m_enc.pushBool(beginAction);
		eventEncoded();
	}

	void NetEventEncoder::returnToGameC(bool ret)
	{
		m_enc.beginMessage(MSG_RETURN_TO_GAME_C);
		m_enc.pushBool(ret);
		eventEncoded();
	}

	void NetEventEncoder::returnToGameS()
	{
		m_enc.beginMessage(MSG_RETURN_TO_GAME_S);
		eventEncoded();
	}

	void NetEventEncoder::disconnectionReturnedS( bool leave )
	{
		m_enc.beginMessage(MSG_DISCONNECTION_RETURNED_S);
		m_enc.pushBool(leave);
		eventEncoded();
	}

}
