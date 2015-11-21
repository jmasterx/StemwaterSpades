#ifndef CGE_GAME_CLIENT_STATE_HPP
#define CGE_GAME_CLIENT_STATE_HPP
#include <stdlib.h>
#include <vector>
#include "Game/SpadesGame/SpadesGameMessages.hpp"
#define MAX_GAME_PLAYERS 4
namespace cge
{
	enum GameStateEnum
	{
		GAMESTATE_DEFAULT,
		GAMESTATE_NEED_TO_BID,
		GAMESTATE_WAITING_TO_BID,
		GAMESTATE_NEED_CHOOSE_2_CARDS,
		GAMESTATE_WAITING_TO_PASS_CARDS,
		GAMESTATE_WAITING_FOR_DECKCUT_POS,
		GAMESTATE_NEED_TO_CUT_DECK,
		GAMESTATE_NEED_TO_PLAY_CARD,
		GAMESTATE_WAITING_TO_PLAY_CARD
	};

	class GameClientState
	{
		GameStateEnum m_state;
		int m_numPlayers;
		//contains my cards then number of cards others have
		std::vector<int> m_playerCards;
		std::vector<int> m_opponentCardQuantities;
		std::vector<int> m_teamScores;
		std::vector<int> m_teamBags;
		
		std::vector<int> m_cardsPassed;
		std::vector<int> m_cardsReceived;
		std::vector<int> m_priorHand;
		std::vector<int> m_filteredCards;
		//should always have 4 cards
		std::vector<int> m_cardsOnTable;
		std::vector<SpadesPointEnum> m_pointsB;
		std::vector<SpadesPointEnum> m_pointsL;
		std::vector<SpadesPointEnum> m_pointsT;
		std::vector<SpadesPointEnum> m_pointsR;
		std::vector<int> m_playerRoundBid;
		int m_handsPlayed;
		int m_dealer;
		int m_activePlayer;
		bool m_spadesBroken;
		bool m_playerHasCards;
		int m_bidRequester;
		int m_firstCardPlayer;
		int m_lastPointReceiver;
		int m_lastPointWinner;
		SpadesPointEnum m_lastPoint;
		bool m_activePlayers[4];
		int m_giftID[4];
		int m_giftFrame[4];

	public:
		GameClientState();
		GameClientState(GameStateEnum state, int numPlayers,const std::vector<int>& selfCards,
			const std::vector<int>& opponentCardQuantities, const std::vector<int>& teamScores,
			const std::vector<int>& teamBags, const std::vector<int>& cardsPassed,
			const std::vector<int>& cardsReceived, const std::vector<int>& priorHand,
			const std::vector<int>& filteredCards, const std::vector<int>& cardsOnTable,
			const std::vector<SpadesPointEnum>& pointsB,
			const std::vector<SpadesPointEnum>& pointsL,
			const std::vector<SpadesPointEnum>& pointsT,
			const std::vector<SpadesPointEnum>& pointsR,
			int handsPlayed,int dealer,int activePlayer,bool spadesBroken,
			bool hasCard,const std::vector<int>& roundBid, int bidRequester, int firstCardPlayer,
			int lastPointWinner, int lastPointReceiver, SpadesPointEnum lastPoint,
			bool activePlayers[4], int giftID[4],int giftFrame[4]);
		bool isSpadesBroken() const;
		GameStateEnum getGameState() const;
		int getNumPlayers() const;
		int getNumTeams() const;
		int getNumCards(int player) const;
		const std::vector<int>& getCards() const;
		const std::vector<int>& getTeamScores() const;
		const std::vector<int>& getTeamBags() const;
		const std::vector<int>& getPassedCards() const;
		const std::vector<int>& getReceivedCards() const;
		const std::vector<int>& getPriorHand() const;
		const std::vector<int>& getFilteredCards() const;
		const std::vector<int>& getCardsOnTable() const;
		const std::vector<int>& getOpponentCardCounts() const;
		const std::vector<int>& getPlayerRoundBids() const;
		std::vector<SpadesPointEnum> getPlayerPoints(int player) const;
		int getNumHandsPlayed() const;
		int getDealer() const;
		int getActivePlayer() const;
		bool playerHasCards() const;
		int getPlayerRoundBid(int player) const;
		int getFirstCardPayer() const;

		int getBidRequester() const;

		int getLastPointReceiver() const;
		int getLastPointWinner() const;
		bool isPlayerActive(int p) const;
		SpadesPointEnum getLastPointType() const;
		int getGiftID(int player) const;
		int getGiftFrame(int player) const;

		~GameClientState(void);
	};
}

#endif