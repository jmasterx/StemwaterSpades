#include "Game/Net/NetEventDecoder.hpp"
#include "Game/Net/NetEventEnums.hpp"
#include <algorithm>
#include <iostream>
#define DISPATCH_NET_EVENT	for(std::vector<NetEventListener*>::iterator it =  m_listeners.begin(); it != m_listeners.end(); ++it)
namespace cge
{
	NetEventDecoder::NetEventDecoder(void)
	{
	}

	NetEventDecoder::~NetEventDecoder(void)
	{
	}

	void NetEventDecoder::addListener( NetEventListener* listener )
	{
		m_listeners.push_back(listener);
	}

	void NetEventDecoder::removeListener( NetEventListener* listener )
	{
		m_listeners.erase(
			std::remove(m_listeners.begin(),
			m_listeners.end(), listener),
			m_listeners.end());
	}


	void NetEventDecoder::dispatchMessage( const std::string& message, ServerPeer* client, bool hasAuthenticated )
	{
		try
		{
			if(!m_parser.parse(message))
			{
				return;
			}

			if(m_parser.getDataSize() < 1)
			{
				return;
			}

			if(!m_parser.getData(0).isMessageId())
			{
				return;
			}

			NetEventEnum evt = (NetEventEnum)m_parser.getData(0).getMessageID();

			if(!hasAuthenticated && (evt != MSG_LOGIN_REQUEST_C && evt != MSG_CREATE_ACCOUNT_C && evt != MSG_RESET_PASSWORD_C))
			{
				return;
			}
			int fst = 1;

			DISPATCH_NET_EVENT
				(*it)->setServerClient(client);

			switch(evt)
			{
			case MSG_SEND_CHAT_MSG_C:
				{
					DISPATCH_NET_EVENT
						(*it)->sendChatMessageC(
						m_parser.getData(fst).getString(),
						(ChatAreaEnum)m_parser.getData(fst + 1).getInt());
				}
				break;
			case MSG_SEND_CHAT_MSG_S:
				{
					DISPATCH_NET_EVENT
						(*it)->sendChatMessageS(
						m_parser.getData(fst).getString(),
						m_parser.getData(fst + 1).getString(),
						(ChatAreaEnum)m_parser.getData(fst + 2).getInt());
				}
				break;
			case MSG_REQUEST_TABLE_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestTableC(
						m_parser.getData(fst).getInt(),m_parser.getData(fst + 1).getInt());
				}
				break;
			case MSG_TABLE_LOCKED_S:
				{
					DISPATCH_NET_EVENT
						(*it)->tableLockedS(
						m_parser.getData(fst).getInt(),
						m_parser.getData(fst + 1).getInt(),
						m_parser.getData(fst + 2).getBool());
				}
				break;
			case MSG_HOST_GAME_C:
				{
					fillIntVec(fst);
					SpadesGameInfo info = SpadesGameInfo(m_fillIntVec);
					DISPATCH_NET_EVENT
						(*it)->hostGameC(info);
				}
				break;
			case MSG_TABLE_STATE_CHANGED_S:
				{
					fillIntVec(fst + 1);
					SpadesGameInfo info = SpadesGameInfo(m_fillIntVec);
					DISPATCH_NET_EVENT
						(*it)->tableStateChangedS(
						m_parser.getData(fst).getInt(),
						info);
				}
				break;
			case MSG_MODIFY_LIST_C:
				{
					DISPATCH_NET_EVENT
						(*it)->modifyListC(
						m_parser.getData(fst).getString(),
						(GroupTypeEnum)m_parser.getData(fst + 1).getInt(),
						m_parser.getData(fst + 2).getBool());
				}
				break;
			case MSG_MODIFIED_LIST_S:
				{
					DISPATCH_NET_EVENT
						(*it)->modifiedListS(
						m_parser.getData(fst).getString(),
						(GroupTypeEnum)m_parser.getData(fst + 1).getInt(),
						m_parser.getData(fst + 2).getBool());
				}
				break;
			case MSG_PLAYER_REQUEST_SIT_C:
				{
					DISPATCH_NET_EVENT
						(*it)->playerRequestSitC(
						m_parser.getData(fst).getInt(),
						m_parser.getData(fst + 1).getInt());
				}
				break;
			case MSG_PLAYER_SAT_DOWN_S:
				{
					DISPATCH_NET_EVENT
						(*it)->playerSatDownS(
						m_parser.getData(fst).getString(),
						m_parser.getData(fst + 1).getInt(),
						m_parser.getData(fst + 2).getInt());
				}
				break;
			case MSG_PLAYER_STOOD_UP_S:
				{
					DISPATCH_NET_EVENT
						(*it)->playerStoodUpS(m_parser.getData(fst).getInt(),
						m_parser.getData(fst + 1).getInt());
				}
				break;
			case MSG_PLAYER_STOOD_UP_C:
				{
					DISPATCH_NET_EVENT
						(*it)->playerStoodUpC();
				}
				break;
			case MSG_GAME_MESSAGE_S:
				{
					int args = fillIntVec(fst + 1);
					int pos = args;
					DISPATCH_NET_EVENT
						(*it)->gameMessageS(
						(SpadesCSMessageEnum)m_parser.getData(fst).getInt(),
						m_fillIntVec,
						m_parser.getData(pos).getInt());
				}
				break;
			case MSG_GAME_MESSAGE_C:
				{
					fillIntVec(fst + 1);
					DISPATCH_NET_EVENT
						(*it)->gameMessageC(
						(SpadesCSMessageEnum)m_parser.getData(fst).getInt(),
						m_fillIntVec);
				}
				break;
			case MSG_SHOW_PROFILE_C:
				{
					DISPATCH_NET_EVENT
						(*it)->showProfileC(
						m_parser.getData(fst).getString());
				}
				break;
			case MSG_LOGIN_REQUEST_C:
				{
					DISPATCH_NET_EVENT
						(*it)->loginRequestC(m_parser.getData(fst).getString(),
						m_parser.getData(fst + 1).getString(),
						m_parser.getData(fst + 2).getInt(),
						m_parser.getData(fst + 3).getInt(),
						m_parser.getData(fst + 4).getInt());
				}
				break;
			case MSG_LOGIN_RESULT_S:
				{
					DISPATCH_NET_EVENT
						(*it)->loginResultS(m_parser.getData(fst).getBool(),
						m_parser.getData(fst + 1).getString(),m_parser.getData(fst + 2).getString());
				}
				break;
			case MSG_PLAYER_JOINED_S:
				{
					SharedPlayer p;
					fst = decodeSharedPlayer(p,fst);
					DISPATCH_NET_EVENT
						(*it)->playerJoinedS(p,(GroupTypeEnum)m_parser.getData(fst).getInt());
				}
				break;
			case MSG_PLAYER_LEFT_S:
				{
					DISPATCH_NET_EVENT
						(*it)->playerLeftS(m_parser.getData(fst).getString());
				}
				break;
			case MSG_HOSTING_DUTIES_CHANGED_S:
				{
					DISPATCH_NET_EVENT
						(*it)->hostingDutiesChangedS(m_parser.getData(fst).getBool());
				}
				break;
			case MSG_ENTER_GAME_S:
				{
					fillIntVec(fst);
					SpadesGameInfo info = SpadesGameInfo(m_fillIntVec);
					DISPATCH_NET_EVENT
						(*it)->enterGameS(
						info);
				}
				break;
			case MSG_TABLE_IS_INACTIVE_S:
				{
					DISPATCH_NET_EVENT
						(*it)->tableIsInactiveS(m_parser.getData(fst).getInt());
				}
				break;
			case MSG_SEND_CLIENT_STATE_S:
				{
					int pos = fst;
					GameStateEnum state = (GameStateEnum)m_parser.getData(pos).getInt(); pos++;
					int numPlayers = m_parser.getData(pos).getInt(); pos++;
					pos = fillIntVec(pos);
					std::vector<int> playerCards = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> opponentCounts = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> teamScores = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> teamBags = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> passedCards = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> receivedCards = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> priorHand = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> filteredCards = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> tableCards = m_fillIntVec;
					std::vector<SpadesPointEnum> points[4];
					for(int i = 0; i < 4; ++i)
					{
						pos = fillIntVec(pos);
						for(int j = 0; j < (int)m_fillIntVec.size(); ++j)
						{
							points[i].push_back((SpadesPointEnum)m_fillIntVec[j]);
						}
					}

					int numHands = m_parser.getData(pos).getInt(); pos++;
					int dealer = m_parser.getData(pos).getInt(); pos++;
					int activePlayer = m_parser.getData(pos).getInt(); pos++;
					bool spadesBroken = m_parser.getData(pos).getBool(); pos++;
					bool hasCards = m_parser.getData(pos).getBool(); pos++;
					pos = fillIntVec(pos);
					std::vector<int> roundBids = m_fillIntVec;
					int bidRequester = m_parser.getData(pos).getInt(); pos++;
					int firstPlayer = m_parser.getData(pos).getInt(); pos++;
					int lastPointWinner = m_parser.getData(pos).getInt(); pos++;
					int lastPointReceiver = m_parser.getData(pos).getInt(); pos++;
					SpadesPointEnum lastPointType = (SpadesPointEnum)m_parser.getData(pos).getInt(); pos++;

					bool poses[4];
					poses[0] = m_parser.getData(pos).getBool(); pos++;
					poses[1] = m_parser.getData(pos).getBool(); pos++;
					poses[2] = m_parser.getData(pos).getBool(); pos++;
					poses[3] = m_parser.getData(pos).getBool(); pos++;

					int giftIDs[4];
					giftIDs[0] = m_parser.getData(pos).getInt(); pos++;
					giftIDs[1] = m_parser.getData(pos).getInt(); pos++;
					giftIDs[2] = m_parser.getData(pos).getInt(); pos++;
					giftIDs[3] = m_parser.getData(pos).getInt(); pos++;

					int giftFrames[4];
					giftFrames[0] = m_parser.getData(pos).getInt(); pos++;
					giftFrames[1] = m_parser.getData(pos).getInt(); pos++;
					giftFrames[2] = m_parser.getData(pos).getInt(); pos++;
					giftFrames[3] = m_parser.getData(pos).getInt(); pos++;

					GameClientState gs = GameClientState(state,numPlayers,playerCards,opponentCounts,
						teamScores,teamBags,passedCards,receivedCards,priorHand,filteredCards,tableCards,
						points[0],points[1],points[2],points[3],numHands,dealer,activePlayer,spadesBroken,
						hasCards,roundBids,bidRequester,firstPlayer,lastPointWinner,lastPointReceiver,lastPointType,
						poses,giftIDs,giftFrames);

					DISPATCH_NET_EVENT
						(*it)->sendClientStateS(gs);
				}
				break;
			case MSG_CANCEL_HOSTING_REQUEST_C:
				{
					DISPATCH_NET_EVENT
						(*it)->canceledHostingRequestC();
				}
				break;
			case MSG_BEGIN_SEND_LOBBY_DATA_S:
				{
					DISPATCH_NET_EVENT
						(*it)->beginSendLobbyDataS(m_parser.getData(fst).getInt(),
						m_parser.getData(fst + 1).getInt());
				}
				break;
			case MSG_END_SEND_LOBBY_DATA_S:
				{
					DISPATCH_NET_EVENT
						(*it)->endSendLobbyDataS();
				}
				break;
			case MSG_READY_TO_RECEIVE_GAME_DATA_C:
				{
					DISPATCH_NET_EVENT
						(*it)->readyToReceiveGameDataC();
				}
				break;
			case MSG_READY_TO_RECEIVE_LOBBY_DATA_C:
				{
					DISPATCH_NET_EVENT
						(*it)->readyToReceiveLobbyDataC();
				}
				break;
			case MSG_TABLE_BEING_SETUP_S:
				{
					DISPATCH_NET_EVENT
						(*it)->tableBeingSetupS(m_parser.getData(fst).getInt());
				}
				break;
			case MSG_PLAYER_ABANDONED_GAME_S:
				{
					DISPATCH_NET_EVENT
						(*it)->playerAbandonedGameS(m_parser.getData(fst).getString());
				}
				break;
			case MSG_PLAYER_ABANDONED_GAME_C:
				{
					DISPATCH_NET_EVENT
						(*it)->playerAbandonedGameC();
				}
				break;
			case MSG_SEAT_IS_SUB_SEAT_S:
				{
					DISPATCH_NET_EVENT
						(*it)->seatIsSubSeatS(m_parser.getData(fst).getInt(),
						m_parser.getData(fst + 1).getInt(),m_parser.getData(fst + 2).getBool());
				}
				break;
			case MSG_SEND_INVITE_S:
				{
					DISPATCH_NET_EVENT
						(*it)->sendInviteS(m_parser.getData(fst).getInt(),
						m_parser.getData(fst + 1).getString());
				}
				break;
			case MSG_SEND_INVITE_C:
				{
					DISPATCH_NET_EVENT
						(*it)->sendInviteC(m_parser.getData(fst).getInt(),
						m_parser.getData(fst + 1).getString());
				}
				break;
			case MSG_WATCH_TABLE_C:
				{
					DISPATCH_NET_EVENT
						(*it)->watchTableC(m_parser.getData(fst).getInt());
				}
				break;
			case MSG_PLAYER_WATCHING_TABLE_S:
				{
					DISPATCH_NET_EVENT
						(*it)->playerWatchingTableS(m_parser.getData(fst).getInt(),
						m_parser.getData(fst + 1).getString());
				}
				break;
			case MSG_GAME_OVER_S:
				{
					int pos = fst;
					std::vector<int> winningTeamOrder;
					pos = fillIntVec(pos);
					winningTeamOrder = m_fillIntVec;

					std::vector<int> finalTeamScores;
					pos = fillIntVec(pos);
					finalTeamScores = m_fillIntVec;

					std::vector<int> newPlayerRatings;
					pos = fillIntVec(pos);
					newPlayerRatings = m_fillIntVec;

					std::vector<std::string> players;
					pos = fillStrVec(pos);
					players = m_fillStrVec;

					long long netCoin = m_parser.getData(pos).getInt64(); pos++;
					long long newAmount = m_parser.getData(pos).getInt64(); pos++;
					int timeout = m_parser.getData(pos).getInt(); pos++;
					DISPATCH_NET_EVENT
						(*it)->gameOverS(winningTeamOrder,finalTeamScores,newPlayerRatings,players,
						netCoin,newAmount,timeout);
				}
				break;
			case MSG_PLAYER_WANTS_PLAY_AGAIN_C:
				{
					DISPATCH_NET_EVENT
						(*it)->playerWantsPlayAgainC();
				}
				break;
			case MSG_READY_TO_PLAY_AGAIN_S:
				{
					DISPATCH_NET_EVENT
						(*it)->readyToPlayAgainS();
				}
				break;
			case MSG_IS_CLIENT_ALIVE_S:
				{
					DISPATCH_NET_EVENT
						(*it)->isClientAliveSC();
				}
				break;
			case MSG_LOGOUT_C:
				{
					DISPATCH_NET_EVENT
						(*it)->logoutC();
				}
				break;
			case MSG_SEND_CHAT_TYPING_C:
				{
					DISPATCH_NET_EVENT
						(*it)->sendChatTypingC(
						(ChatAreaEnum)m_parser.getData(fst).getInt());
				}
				break;
			case MSG_SEND_CHAT_TYPING_S:
				{
					DISPATCH_NET_EVENT
						(*it)->sendChatTypingS(
						m_parser.getData(fst).getString(),
						(ChatAreaEnum)m_parser.getData(fst + 1).getInt());
				}
				break;
			case MSG_SHOW_PROFILE_S:
				{
					int c = fst;
					fst++;
					int avatar = m_parser.getData(c).getInt();
					fst = fillStrVec(fst);

					DISPATCH_NET_EVENT
						(*it)->showProfileS(avatar,m_fillStrVec);
				}
				break;
			case MSG_ACCEPT_TERMS_C:
				{
					DISPATCH_NET_EVENT
						(*it)->playerAcceptedTermsC(m_parser.getData(fst).getBool());
				}
				break;
			case MSG_CREATE_ACCOUNT_C:
				{
					DISPATCH_NET_EVENT
						(*it)->createAccountC(
						m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getString(),
						m_parser.getData(fst+2).getString(),
						m_parser.getData(fst+3).getString(),
						m_parser.getData(fst+4).getString());
				}
				break;
			case MSG_CREATE_ACCOUNT_S:
				{
					DISPATCH_NET_EVENT
						(*it)->createAccountS(
						m_parser.getData(fst).getBool(),
						m_parser.getData(fst+1).getBool(),
						m_parser.getData(fst+2).getBool(),
						m_parser.getData(fst+3).getBool(),
						m_parser.getData(fst+4).getBool(),
						m_parser.getData(fst+5).getBool(),
						m_parser.getData(fst+6).getBool(),
						m_parser.getData(fst+7).getBool(),
						m_parser.getData(fst+8).getBool()
						);
				}
				break;
			case MSG_ACCEPT_TERMS_S:
				{
					DISPATCH_NET_EVENT
						(*it)->playerMustAcceptTermsS(m_parser.getData(fst).getString());
				}
				break;
			case MSG_SEND_MESSSAGE_S:
				{
					DISPATCH_NET_EVENT
						(*it)->sendMessageS(
						m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getString(),
						m_parser.getData(fst+2).getBool(),
						m_parser.getData(fst+3).getBool());
				}
				break;
			case MSG_WAIT_FOR_SUB_C:
				{
					DISPATCH_NET_EVENT
						(*it)->waitForSubC(
						m_parser.getData(fst).getBool());
				}
				break;
			case MSG_WAIT_FOR_SUB_S:
				{
					DISPATCH_NET_EVENT
						(*it)->waitForSubS(
						m_parser.getData(fst).getBool());
				}
				break;
			case MSG_CHAT_FROM_SERVER_S:
				{
					DISPATCH_NET_EVENT
						(*it)->chatMessageFromServerS(
						m_parser.getData(fst).getString(),
						m_parser.getData(fst + 1).getString(),
						m_parser.getData(fst + 2).getBool(),
						(ChatAreaEnum)m_parser.getData(fst + 3).getInt()
						);
				}
				break;
			case MSG_KICK_PLAYER_C:
				{
					DISPATCH_NET_EVENT
						(*it)->kickPlayerC(
						m_parser.getData(fst).getString()
						);
				}
				break;
			case MSG_REQUEST_ACCOUNT_SETTINGS_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestAccountSettingsC();
				}
				break;
			case MSG_REQUEST_ACCOUNT_SETTINGS_S:
				{
					DISPATCH_NET_EVENT
						(*it)->requestAccountSettingsS(
						m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getString(),
						m_parser.getData(fst+2).getBool());
				}
				break;
			case MSG_REQUEST_CHANGE_PASSWORD_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestChangePasswordC(
						m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getString());
				}
				break;
			case MSG_REQUEST_CHANGE_EMAIL_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestChangeEmailC(
						m_parser.getData(fst).getString());
				}
				break;
			case MSG_REQUEST_RESET_RATING_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestResetRatingC();
				}
				break;
			case MSG_REQUEST_CHANGE_SHOW_EMAIL_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestChangeShowEmailC(m_parser.getData(fst).getBool());
				}
				break;
			case MSG_REQUEST_CHANGE_NAME_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestChangeFullNameC(m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getString());
				}
				break;
			case MSG_ACCOUNT_SETTINGS_CHANGED_S:
				{
					DISPATCH_NET_EVENT
						(*it)->accountSettingsChangedS(m_parser.getData(fst).getInt());
				}
				break;
			case MSG_CHANGE_AVATAR_C:
				{
					DISPATCH_NET_EVENT
						(*it)->changeAvatarC(m_parser.getData(fst).getInt());
				}
				break;
			case MSG_CHANGE_AVATAR_S:
				{
					DISPATCH_NET_EVENT
						(*it)->changeAvatarS(m_parser.getData(fst).getInt(),
						m_parser.getData(fst+1).getString());
				}
				break;
			case MSG_SET_AVATAR_S:
				{
					DISPATCH_NET_EVENT
						(*it)->setAvatarS();
				}
				break;
			case MSG_SHOW_LEADERBOARD_C:
				{
					DISPATCH_NET_EVENT
						(*it)->showLeaderboardC(m_parser.getData(fst).getInt(),
						m_parser.getData(fst+1).getInt(),
						m_parser.getData(fst+2).getString(),
						m_parser.getData(fst+3).getBool());
				}
				break;
			case MSG_SHOW_LEADERBOARD_S:
				{
					int pos = fst + 2;
					std::vector<std::string> rank;
					pos = fillStrVec(pos);
					rank = m_fillStrVec;

					std::vector<std::string> names;
					pos = fillStrVec(pos);
					names = m_fillStrVec;

					std::vector<std::string> rating;
					pos = fillStrVec(pos);
					rating = m_fillStrVec;

					DISPATCH_NET_EVENT
						(*it)->showLeaderboardS(m_parser.getData(fst).getInt(),
						m_parser.getData(fst+1).getInt(),
						rank,names,rating,
						m_parser.getData(pos).getString(),
						m_parser.getData(pos+1).getBool());
				}
				break;
			case MSG_CHANGE_THEME_C:
				{
					DISPATCH_NET_EVENT
						(*it)->changeThemeC(m_parser.getData(fst).getInt());
				}
				break;
			case MSG_SEND_GIFT_C:
				{
					DISPATCH_NET_EVENT
						(*it)->sendGiftC(m_parser.getData(fst).getInt(),m_parser.getData(fst+1).getInt());
				}
				break;
			case MSG_SEND_GIFT_S:
				{
					DISPATCH_NET_EVENT
						(*it)->sendGiftS(m_parser.getData(fst).getInt(),m_parser.getData(fst+1).getInt(),
						m_parser.getData(fst+2).getInt());
				}
				break;
			case MSG_UPDATE_GIFT_C:
				{
					DISPATCH_NET_EVENT
						(*it)->updateGiftC();
				}
				break;
			case MSG_UPDATE_GIFT_S:
				{
					DISPATCH_NET_EVENT
						(*it)->updateGiftS(m_parser.getData(fst).getInt(),m_parser.getData(fst+1).getInt(),
						m_parser.getData(fst+2).getInt());
				}
				break;
			case MSG_SEND_FEE_OPTIONS_S:
				{
					int pos = fst;
					std::vector<int> fees;
					pos = fillIntVec(pos);
					fees = m_fillIntVec;

					std::vector<int> mins;
					pos = fillIntVec(pos);
					mins = m_fillIntVec;

					int highStakes = m_parser.getData(pos).getInt();

					DISPATCH_NET_EVENT
						(*it)->sendFeeOptionsS(fees,mins,highStakes);
				}
				break;
			case MSG_SHOW_COINS_DIALOG_S:
				{
					int pos = fst;
					pos = fillStrVec(pos);
					std::vector<std::string> coinAmounts = m_fillStrVec;
					pos = fillStrVec(pos);
					std::vector<std::string> prices = m_fillStrVec;
					pos = fillStrVec(pos);
					std::vector<std::string> salePercents = m_fillStrVec;
					pos = fillStrVec(pos);
					std::vector<std::string> coinPercents = m_fillStrVec;
					pos = fillStrVec(pos);
					std::vector<std::string> coinUrls = m_fillStrVec;

					DISPATCH_NET_EVENT
						(*it)->showCoinsDialogS(coinAmounts,
						prices,salePercents,coinPercents,coinUrls);
				}
				break;
			case MSG_SHOW_COINS_DIALOG_C:
				{
					DISPATCH_NET_EVENT
						(*it)->showCoinsDialogC();
				}
				break;
			case MSG_SHOW_GIFT_DIALOG_C:
				{
					DISPATCH_NET_EVENT
						(*it)->showGiftDialogC(m_parser.getData(fst).getInt());
				}
				break;
			case MSG_SHOW_GIFT_DIALOG_S:
				{
					DISPATCH_NET_EVENT
						(*it)->showGiftDialogS(m_parser.getData(fst).getInt(),m_parser.getData(fst+1).getString());
				}
				break;
			case MSG_REMOVE_GIFT_C:
				{
					DISPATCH_NET_EVENT
						(*it)->removeGiftC();
				}
				break;
			case MSG_SEND_COIN_AMOUNT_S:
				{
					DISPATCH_NET_EVENT
						(*it)->sendCoinAmountS(m_parser.getData(fst).getInt64(),
						m_parser.getData(fst+1).getBool(),
						m_parser.getData(fst+2).getBool(),
						m_parser.getData(fst+3).getBool());
				}
				break;
			case MSG_SEND_COIN_AMOUNT_C:
				{
					DISPATCH_NET_EVENT
						(*it)->sendCoinAmountC(m_parser.getData(fst).getBool());
				}
				break;
			case MSG_SHOW_THEME_DIALOG_S:
				{
					DISPATCH_NET_EVENT
						(*it)->showThemeDialogS(m_parser.getData(fst).getBool());
				}
				break;
			case MSG_GAME_TIME_CHANGED_S:
				{
					DISPATCH_NET_EVENT
						(*it)->gameTimeChangedS(m_parser.getData(fst).getInt());
				}
				break;
			case MSG_SEND_FEEDBACK_C:
				{
					DISPATCH_NET_EVENT
						(*it)->sendFeedbackC(m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getString(),
						m_parser.getData(fst+2).getString());
				}
				break;
			case MSG_SEND_LINK_C:
				{
					DISPATCH_NET_EVENT
						(*it)->sendStoreLinkC(m_parser.getData(fst).getString());
				}
				break;
			case MSG_SEND_LINK_S:
				{
					DISPATCH_NET_EVENT
						(*it)->sendStoreLinkS(m_parser.getData(fst).getString());
				}
				break;
			case MSG_REGISTER_SERVER_MC:
				{
					int pos = fst+4;
					pos = fillStrVec(pos);
					std::vector<std::string> users = m_fillStrVec;

					DISPATCH_NET_EVENT
						(*it)->registerServerMC(m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getString(),
						m_parser.getData(fst+2).getInt(),
						m_parser.getData(fst+3).getInt(),
						users,m_parser.getData(pos).getString(),
						m_parser.getData(pos+1).getInt()
						);
				}
				break;
			case MSG_PLAYER_LOGGED_IN_MC:
				{
					DISPATCH_NET_EVENT
						(*it)->playerLoggedInMC(m_parser.getData(fst).getString());
				}
				break;
			case MSG_PLAYER_LOGGED_OUT_MC:
				{
					DISPATCH_NET_EVENT
						(*it)->playerLoggedOutMC(m_parser.getData(fst).getString());
				}
				break;
			case MSG_FETCH_SERVER_LIST_MC:
				{
					DISPATCH_NET_EVENT
						(*it)->fetchServerListMC(m_parser.getData(fst).getString());
				}
				break;
			case MSG_FETCH_SERVER_LIST_MS:
				{
					int pos = fst+1;
					pos = fillStrVec(pos);
					std::vector<std::string> names = m_fillStrVec;
					pos = fillStrVec(pos);
					std::vector<std::string> ips = m_fillStrVec;
					pos = fillIntVec(pos);
					std::vector<int> ports = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> capacities = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> numusers = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> numfriends = m_fillIntVec;
					DISPATCH_NET_EVENT
						(*it)->fetchServerListMS(m_parser.getData(fst).getString(),
						names,ips,ports,capacities,numusers,numfriends);
				}
				break;
			case MSG_FETCH_LOGIN_SERVER_MC:
				{
					DISPATCH_NET_EVENT
						(*it)->fetchLoginServerMC(m_parser.getData(fst).getString());
				}
				break;
			case MSG_FETCH_LOGIN_SERVER_MS:
				{
					DISPATCH_NET_EVENT
						(*it)->fetchLoginServerMS(m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getString(),m_parser.getData(fst+2).getString(), m_parser.getData(fst+3).getInt());
				}
				break;
			case MSG_REQUEST_SERVER_LIST_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestServerListC();
				}
				break;
			case MSG_SERVER_FULL_C:
				{
					DISPATCH_NET_EVENT
						(*it)->serverFullC(m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getInt());
				}
				break;
			case MSG_SERVER_FULL_S:
				{
					DISPATCH_NET_EVENT
						(*it)->serverFullS(m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getString(),m_parser.getData(fst+2).getInt(),
						m_parser.getData(fst+3).getBool());
				}
				break;
			case MSG_SERVER_FULL_MC:
				{
					DISPATCH_NET_EVENT
						(*it)->serverFullMC(m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getString(),
						m_parser.getData(fst+2).getInt());
				}
				break;
			case MSG_SERVER_FULL_MS:
				{
					DISPATCH_NET_EVENT
						(*it)->serverFullMS(m_parser.getData(fst).getString(),
						m_parser.getData(fst+1).getString(),
						m_parser.getData(fst+2).getString(),
						m_parser.getData(fst+3).getInt(),
						m_parser.getData(fst+4).getBool());
				}
				break;
			case MSG_LOGOUT_PLAYER_MS:
				{
					DISPATCH_NET_EVENT
						(*it)->logoutPlayerMS(m_parser.getData(fst).getString());
				}
				break;
			case MSG_FETCH_FRIEND_SERVER_LIST_MC:
				{
					DISPATCH_NET_EVENT
						(*it)->fetchFriendServerListMC(m_parser.getData(fst).getString());
				}
				break;
			case MSG_FETCH_FRIEND_SERVER_LIST_MS:
				{
					int pos = fst+1;
					pos = fillStrVec(pos);
					std::vector<std::string> names = m_fillStrVec;
					pos = fillStrVec(pos);
					std::vector<std::string> ips = m_fillStrVec;
					pos = fillIntVec(pos);
					std::vector<int> ports = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> capacities = m_fillIntVec;
					pos = fillIntVec(pos);
					std::vector<int> numusers = m_fillIntVec;
					pos = fillStrVec(pos);
					std::vector<std::string> numfriends = m_fillStrVec;
					DISPATCH_NET_EVENT
						(*it)->fetchFriendServerListMS(m_parser.getData(fst).getString(),
						names,ips,ports,capacities,numusers,numfriends);
				}
				break;
			case MSG_REQUEST_FRIEND_SERVER_LIST_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestFriendServerListC();
				}
				break;
			case MSG_REQUEST_SUB_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestSubC(m_parser.getData(fst).getInt(),
						m_parser.getData(fst+1).getInt());
				}
				break;
			case MSG_REQUEST_SUB_S:
				{
					DISPATCH_NET_EVENT
						(*it)->requestSubS(m_parser.getData(fst).getInt(),
						m_parser.getData(fst+1).getInt(),
						m_parser.getData(fst+2).getString(),
						m_parser.getData(fst+3).getInt64(),
						m_parser.getData(fst+4).getInt64(),
						m_parser.getData(fst+4).getInt());
				}
				break;
			case MSG_REQUEST_PLAYER_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestPlayerC();
				}
				break;
			case MSG_REQUEST_PLAYER_S:
				{
					DISPATCH_NET_EVENT
						(*it)->requestPlayerS(m_parser.getData(fst).getInt(),
						m_parser.getData(fst+1).getString());
				}
				break;
			case MSG_BAD_ASYNC_S:
				{
					DISPATCH_NET_EVENT
						(*it)->badAsyncRequestS();
				}
				break;
			case MSG_REQUEST_STRIKES_C:
				{
					DISPATCH_NET_EVENT
						(*it)->requestStrikesC();
				}
				break;
			case MSG_REQUEST_STRIKES_S:
				{
					DISPATCH_NET_EVENT
						(*it)->requestStrikesS(
						m_parser.getData(fst).getInt(),
						m_parser.getData(fst+1).getInt(),
						m_parser.getData(fst+2).getInt(),
						m_parser.getData(fst+3).getBool());
				}
				break;
			case MSG_RETURN_TO_GAME_C:
				{
					DISPATCH_NET_EVENT
						(*it)->returnToGameC(m_parser.getData(fst).getBool());
				}
				break;
			case MSG_RETURN_TO_GAME_S:
				{
					DISPATCH_NET_EVENT
						(*it)->returnToGameS();
				}
				break;
			case MSG_DISCONNECTION_RETURNED_S:
				{
					DISPATCH_NET_EVENT
						(*it)->disconnectionReturnedS(m_parser.getData(fst).getBool());
				}
				break;
			default:
				break;
			}
		}
		catch (std::exception& e)
		{
		}
	}
	int NetEventDecoder::fillIntVec( int startIndex )
	{
		m_fillIntVec.clear();
		int numArgs = m_parser.getData(startIndex).getInt();

		for(int i = 0; i < numArgs; ++i)
		{
			m_fillIntVec.push_back(m_parser.getData(startIndex + i + 1).getInt());
		}

		return (int)m_fillIntVec.size() + 1 + startIndex;
	}

	int NetEventDecoder::decodeSharedPlayer( SharedPlayer &player, int fst )
	{
		player.setUsername(m_parser.getData(fst).getString()); fst++;
		player.setTableId(m_parser.getData(fst).getInt()); fst++;
		player.setSeat(m_parser.getData(fst).getInt()); fst++;
		player.setWatchTableId(m_parser.getData(fst).getInt()); fst++;
		player.setAvatar(m_parser.getData(fst).getInt()); fst++;

		return fst;
	}

	int NetEventDecoder::fillStrVec( int startIndex )
	{
		m_fillStrVec.clear();
		int numArgs = m_parser.getData(startIndex).getInt();

		for(int i = 0; i < numArgs; ++i)
		{
			m_fillStrVec.push_back(m_parser.getData(startIndex + i + 1).getString());
		}

		return (int)m_fillStrVec.size() + 1 + startIndex;
	}

	void NetEventDecoder::dispatchDisconnect()
	{
		DISPATCH_NET_EVENT
			(*it)->lostConnectionCS();
	}

	void NetEventDecoder::dispatchConnectionFailed( bool wasConnected )
	{
		DISPATCH_NET_EVENT
			(*it)->connectionFailedCS();
	}

}
