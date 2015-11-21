#ifndef CGE_SPADES_GAME_HPP
#define CGE_SPADES_GAME_HPP
#include "Game/Card/Card.hpp"
#include "Game/SpadesGame/SpadesGameMessage.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include <vector>
#include <queue>
#include "Game/Card/CardShuffler.hpp"
#include <stdio.h>
#include <time.h>
#include "Game/SpadesGame/GameClientState.hpp"

namespace cge
{
	class RawGameListener;
	class SpadesGame
	{
	public:
		class Player
		{
			std::vector<Card> m_cards;
			std::vector<int> m_incomingSwapCardNumbers;
			std::vector<int> m_outgoingSwapCardNumbers;
			bool m_alive;
			std::vector<SpadesPointEnum> m_tricks;
			int m_roundBid;
		public:
			Player();
			const std::vector<SpadesPointEnum>& getRoundTricksVec() const;
			void setCards(const std::vector<Card>& cards);
			void setCard(int index,const Card& card);
			std::vector<int> getCardNumbers() const;
			void removeCard(int index);
			void sortCards();
			size_t getCardCount() const;
			int getIndexFromCardNumber(int cardNumber) const;
			int getCardNumberOf(int card) const;
			int getQuantityOfSuit(Card::SuitEnum suit) const;
			bool hasSuit(Card::SuitEnum suit) const;
			std::vector<int> getIndexesOfSuit(Card::SuitEnum suit) const;
			std::vector<int> getIndexesOfNotSuit(Card::SuitEnum suit) const;

			void setRoundBid(int bid);
			int getRoundBid() const;
			int getRoundTricks() const;
			void addRoundTrick(SpadesPointEnum point);
			void clearRoundTricks();

			bool hasMadeBid() const;
			bool hasBid() const;

			void setAlive(bool active);
			bool isAlive() const;
			bool onlyHasSpades() const;

			void setIncomingSwapCardNumbers(const std::vector<int>& cardNumbers);
			const std::vector<int>& getIncomingSwapCardNumbers() const;
			void setOutgoingSwapCardNumbers(const std::vector<int>& cardNumbers);
			const std::vector<int>& getOutgoingSwapCardNumbers() const;

			void swapCards();

		};

		class Team
		{
			int m_score;
			int m_bags;
		public:
			Team();
			void clearScore();
			void clearBags();
			void increaseBags(int amount);
			void increaseScore(int amount);
			void setBags(int bags);
			void setScore(int score);
			int getScore() const;
			int getBags()const;
		};

		enum StateEnum
		{
			WAITING_FOR_GAME_INFO,
			WAITING_FOR_NUM_PLAYERS,
			WAITING_FOR_CARDS_DISPATCHED,
			WAITING_FOR_DECK_CUT_POS,
			WAITING_FOR_PLAYER_TRICK_COUNT,
			WAITING_FOR_SWAP_POS,
			WAITING_FOR_SWAP_PERFORMED,
			WAITING_FOR_PLAYER_CARD,
			WAITING_FOR_CARD_THROWN,
			WAITING_FOR_POINT_GIVEN
		};
	
	private:
		std::queue<SpadesGameMessage> m_msgQ;
		RawGameListener* m_rawListener;
		StateEnum m_state;
		Player m_player[4];
		Team   m_team[4];
		std::vector<Card> m_tableCards;
		std::vector<int> m_orderedTableCards;
		std::vector<int> m_priorCards;
		std::vector<int> m_swappingPlayers;
		bool m_spadesBroken;
		int m_activePlayer;
		int m_roundActivePlayer;
		SpadesGameInfo m_gameInfo;
		int m_host;
		bool m_gameInfoIsSet;
		int m_deckCutPos;
		int m_lastPointWinner;
		int m_lastPointReceiver;
		SpadesPointEnum m_lastPointType;
		bool m_waitBools[4];
		CardShuffler m_cardShuffler;
		int m_cardOffset;

		int m_handCount;
		float m_time;
		float m_endTimeStamp;
		int m_requestedBidFromPlayer;
		int m_requestCardFromPlayer;

		int m_lastMoonshotWinner;
		int m_lastGameWinner;

		std::vector<int> m_winningTeams;
		std::vector<int> m_finalScores;

		bool m_pActive[4];

		void sendActivePlayer(int active);
		void sendActivePlayers(bool p0, bool p1, bool p2, bool p3);
		void sendActivePlayersNotBid();
		//processes a message received from player
		virtual void processMessage(const SpadesGameMessage& msg);
		void setState(StateEnum state);
		StateEnum getState() const;
		bool isTimeExpired() const;
		void setEndTime(int numMins);
		void doNewGame();
		int getWinner();
		int getLoser();
		bool isGameOver();
		int getNumberOfTeams() const;
		void setLastPointWinner(int receiver);
		int getLastPointWinner() const;
		void setLastPointReceiver(int receiver);
		int getLastPointReceiver() const;
		void setLastPointType(SpadesPointEnum point);
		SpadesPointEnum getLastPointType() const;
		void setActivePlayer(int player);
		void sendGameInfoToPlayer(int player);
		int getActivePlayer() const;
		int getDealer() const;
		int getDeckCutter() const;
		int getNextPlayer(int player) const;
		int getPartner(int player) const;
		bool isSpadesBroken() const;
		void setSpadesBroken(bool broken);
		void msgSetGameInfo(const SpadesGameMessage& msg);
		void msgJoined(const SpadesGameMessage& msg);
		void msgReplyInitGame(const SpadesGameMessage& msg);
		void msgGotDeckCutPos(const SpadesGameMessage& msg);
		void msgCardsDispatched(const SpadesGameMessage& msg);
		void msgGotTrickCountFromPlayer(const SpadesGameMessage& msg);
		void msgGotSwappedCardsFromPlayer(const SpadesGameMessage& msg);
		void msgFinishedCardSwap(const SpadesGameMessage& msg);
		void msgGotTableCardFromPlayer(const SpadesGameMessage& msg);
		void msgPerformedCardOnTable(const SpadesGameMessage& msg);
		void msgPerformedGivePoint(const SpadesGameMessage& msg);
		int getCardWinnerPlayer() const;
		int getMoonshotWinnerPlayer();
		bool isTeamPassingTwo(int teamNo) const;
		int getNumberOfTeamsPassingTwo() const;
		void doEndOfRound();
		void doNewRound();
		void doRequestDeckCutPos();
		void doGivePoint();
		void doCardDispatch();
		void doBidding();
		void doPassTwoRequest();
		void doRequestForCard();
		void doGameOver();

		int getRoundScoreForTeam(int teamNo,int numBagBusts);
		int getRoundBagsForTeam(int teamNo);
		void updateRoundScores();

		void setRequestBidder(int player);
		void setCardRequester(int player);

		void createPriorHand();

		bool isSwappingPlayer(int player) const;

		std::vector<int> doCardFiltering(int player);
		int mapInverseClientToServer(int serverClient,int clientPos) const;
		int getConnectedPlayerCount() const;
		bool setGameInfo(const std::vector<int>& args);
		std::vector<int> getGameInfoVect() const;
		void setOrderedCard(int card, int pos);
		void clearOrderedCards();
		void clearWaitBools();
		void setWaitBool(int waitBool, bool val = true);
		bool isWaitBoolFinished() const;
		int getLegalCardIndexForPlayer(int player, int givenIndex, int dontPick = -1);
		std::vector<int> getOppositeIndexes(int player, const std::vector<int>& cur);
		
	protected:
	
		void pushMessage(const SpadesGameMessage& msg);
		
	public:
		SpadesGame(void);
		int getWinningCardIndex(const std::vector<Card>& cards) const;
		void processMessages(); //called regularly
		virtual void sendMessageToPlayer(const SpadesGameMessage& message,
			bool distribute = false) = 0;
		GameClientState generateState(int player,bool watching,int giftIDs[4], int giftFrames[4]);
		bool clientReconnected(int player);
		const SpadesGameInfo& getGameInfo() const;

		const std::vector<int>& getWinningTeams() const;
		const std::vector<int>& getWinningScores() const;

		virtual void reset();
		int mapServerToClientPos(int frontPayer, int targetPlayer) const;
		int mapClientToServer(int serverClient,int clientPos,bool shiftOnThree = false) const;
		int mapClientToServerLinear(int serverClient,int clientPos) const;
		void setGameInfo(const SpadesGameInfo& info);
		void increaseTime(float amount);
		bool canKickPlayer(int playerNo) const;
		float getTimeElapsed() const;
		virtual void playerRequestDoAction(int playerNo) {}
		virtual void playerFinishedAction(int playerNo) {}
		void setRawListener(RawGameListener* listener);
		virtual ~SpadesGame(void);
	};
}

#endif
