#include "Game/SpadesGame/GameClientState.hpp"
namespace cge
{
	GameClientState::GameClientState(void)
	{
	}

	GameClientState::GameClientState( GameStateEnum state, int numPlayers,const std::vector<int>& selfCards,
		const std::vector<int>& opponentCardQuantities, const std::vector<int>& teamScores, 
		const std::vector<int>& teamBags, const std::vector<int>& cardsPassed, const std::vector<int>& cardsReceived,
		const std::vector<int>& priorHand, const std::vector<int>& filteredCards, const std::vector<int>& cardsOnTable, 
		const std::vector<SpadesPointEnum>& pointsB, const std::vector<SpadesPointEnum>& pointsL, 
		const std::vector<SpadesPointEnum>& pointsT, const std::vector<SpadesPointEnum>& pointsR, 
		int handsPlayed,int dealer,int activePlayer,bool spadesBroken,
		bool hasCard,const std::vector<int>& roundBid, int bidRequester,
		int firstCardPlayer, int lastPointWinner, int lastPointReceiver,
		SpadesPointEnum lastPoint, bool activePlayers[4], 
		int giftID[4],int giftFrame[4] )
		: m_state(state),m_numPlayers(numPlayers),m_playerCards(selfCards),
		m_opponentCardQuantities(opponentCardQuantities),m_teamScores(teamScores),
		m_teamBags(teamBags),m_cardsPassed(cardsPassed),m_cardsReceived(cardsReceived),
		m_priorHand(priorHand),m_filteredCards(filteredCards),m_cardsOnTable(cardsOnTable),
		m_pointsB(pointsB),m_pointsL(pointsL),m_pointsT(pointsT),m_pointsR(pointsR),
		m_handsPlayed(handsPlayed),m_dealer(dealer),m_activePlayer(activePlayer),
		m_spadesBroken(spadesBroken), m_playerHasCards(hasCard),
		m_playerRoundBid(roundBid),m_bidRequester(bidRequester),m_firstCardPlayer(firstCardPlayer),
		m_lastPointWinner(lastPointWinner),m_lastPointReceiver(lastPointReceiver),m_lastPoint(lastPoint)
	{
		for(int i = 0; i < 4; i++)
		{
			m_activePlayers[i] = activePlayers[i];
			m_giftID[i] = giftID[i];
			m_giftFrame[i] = giftFrame[i];
		}
	}

	GameClientState::~GameClientState(void)
	{
	}

	bool GameClientState::isSpadesBroken() const
	{
		return m_spadesBroken;
	}

	cge::GameStateEnum GameClientState::getGameState() const
	{
		return m_state;
	}

	int GameClientState::getNumPlayers() const
	{
		return m_numPlayers;
	}

	int GameClientState::getNumTeams() const
	{
		return m_teamScores.size();
	}

	int GameClientState::getNumCards( int player ) const
	{
		if(player == 0)
		{
			if(playerHasCards())
			return m_playerCards.size();
			else if(m_playerCards.size() > 0)
			return m_playerCards[0];
			else
			return 0;
		}

		return m_opponentCardQuantities[player];
	}

	const std::vector<int>& GameClientState::getCards() const
	{
		return m_playerCards;
	}

	const std::vector<int>& GameClientState::getTeamScores() const
	{
		return m_teamScores;
	}

	const std::vector<int>& GameClientState::getTeamBags() const
	{
		return m_teamBags;
	}

	const std::vector<int>& GameClientState::getPassedCards() const
	{
		return m_cardsPassed;
	}

	const std::vector<int>& GameClientState::getReceivedCards() const
	{
		return m_cardsReceived;
	}

	const std::vector<int>& GameClientState::getPriorHand() const
	{
		return m_priorHand;
	}

	const std::vector<int>& GameClientState::getFilteredCards() const
	{
		return m_filteredCards;
	}

	std::vector<SpadesPointEnum> GameClientState::getPlayerPoints( int player ) const
	{
		if(player == 0)
		{
			return m_pointsB;
		}
		else if(player == 1)
		{
			return m_pointsL;
		}
		else if(player == 2)
		{
			return m_pointsT;
		}
		else if(player == 3)
		{
			return m_pointsR;
		}

		return std::vector<SpadesPointEnum>();
	}

	int GameClientState::getNumHandsPlayed() const
	{
		return m_handsPlayed;
	}

	int GameClientState::getDealer() const
	{
		return m_dealer;
	}

	int GameClientState::getActivePlayer() const
	{
		return m_activePlayer;
	}

	bool GameClientState::playerHasCards() const
	{
		return m_playerHasCards;
	}

	int GameClientState::getPlayerRoundBid( int player ) const
	{
		if(player < (int)m_playerRoundBid.size())
		{
			return m_playerRoundBid[player];
		}

		return -1;
	}

	int GameClientState::getBidRequester() const
	{
		return m_bidRequester;
	}

	const std::vector<int>& GameClientState::getOpponentCardCounts() const
	{
		return m_opponentCardQuantities;
	}

	const std::vector<int>& GameClientState::getPlayerRoundBids() const
	{
		return m_playerRoundBid;
	}

	const std::vector<int>& GameClientState::getCardsOnTable() const
	{
		return m_cardsOnTable;
	}

	int GameClientState::getFirstCardPayer() const
	{
		return m_firstCardPlayer;
	}

	int GameClientState::getLastPointReceiver() const
	{
		return m_lastPointReceiver;
	}

	int GameClientState::getLastPointWinner() const
	{
		return m_lastPointWinner;
	}

	cge::SpadesPointEnum GameClientState::getLastPointType() const
	{
		return m_lastPoint;
	}

	bool GameClientState::isPlayerActive( int p ) const
	{
		return m_activePlayers[p];
	}

	int GameClientState::getGiftID( int player ) const
	{
		if(player >= 0 && player < 4)
		return m_giftID[player];
		return -1;
	}

	int GameClientState::getGiftFrame( int player ) const
	{
		if(player >= 0 && player < 4)
			return m_giftFrame[player];
		return -1;
	}

}
