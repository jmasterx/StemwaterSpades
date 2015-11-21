#ifndef CGE_NET_AI_PARTNERSHIP_SPADES_BOT_HPP
#define CGE_NET_AI_PARTNERSHIP_SPADES_BOT_HPP
#include "Game/SpadesGame/SpadesGameMessage.hpp"
#include "Game/Card/Card.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Game/Net/NetAISpadesBot.hpp"
#include <vector>
namespace cge
{
	struct NetAIPoint
	{
		int x;
		int y;
		NetAIPoint()
			: x(0),y(0)
		{

		}
	};
	class NetSpadesGame;
	class NetAIPartnershipSpadesBot : public NetAISpadesBot
	{
		NetSpadesGame* m_game;
		std::vector<Card> m_cards;
		std::vector<int> m_enabledCardIndexes;
		std::vector<Card> m_enabledCards;
		std::vector<int> m_playerBid;
		std::vector<int> m_playerTrickCount;
		std::vector<int> m_tableIndexes;
		std::vector<Card> m_tableCards;
		std::vector<Card> m_playedCards;
		SpadesGameInfo m_info;
	public:
		NetAIPartnershipSpadesBot(void);
		int serverToClientPos(int pos) const;
		void setGameInfo(const std::vector<int>& args);
		void sendMessageToGame(const SpadesCSMessageEnum& msg, const std::vector<int>& args);
		virtual void receive(const SpadesGameMessage& msg);
		void setGame(NetSpadesGame* game);
		int getNumCardsInRank(Card::RankEnum rank) const;
		int getNumCardsInSuit(Card::SuitEnum suit) const;
		int getNumCardsInRankAndSuitGreaterOrEqualTo(Card::RankEnum rank, Card::SuitEnum suit) const;
		int getNumCardsInSuitBetween(Card::SuitEnum suit,Card::RankEnum minimum, Card::RankEnum maximum) const;
		int getLastNthCardIndexInSuit(Card::SuitEnum suit, int countFromLast) const;
		int getFirstNthCardIndexInSuit(Card::SuitEnum suit, int countFromFirst) const;
		Card::SuitEnum getNthSuitWithLowestCount( int countFromLast, bool skipSpades ) const;
		Card::SuitEnum getNthSuitWithLowestCountMin( int countFromLast, bool skipSpades, Card::RankEnum minimum ) const;
		Card::SuitEnum getNthSuitWithLowestCountMax( int countFromLast, bool skipSpades, Card::RankEnum maximum ) const;
		bool onlyHasSuit(Card::SuitEnum suit) const;
		bool hasPartnerBidNil() const;
		bool hasPartnerBid() const;
		int getPartnerBid() const;
		bool hasCard(Card::RankEnum rank, Card::SuitEnum suit) const;
		bool hasEnabledCard(Card::RankEnum rank, Card::SuitEnum suit) const;
		int getCardCost(const Card& c) const;
		bool didPartnerLead() const;
		bool hasPartnerPlayed() const;
		bool isFirstToPlay() const;
		bool didLead(int playerNo) const;
		bool hasPlayed(int playerNo) const;
		bool isCardOnTable(Card::RankEnum rank, Card::SuitEnum suit) const;
		bool hasCardBeenPlayed(Card::RankEnum rank, Card::SuitEnum suit,bool notOnTable = false) const;
		int getBid(int playerNo) const;
		int getTricks(int playerNo) const;
		Card::SuitEnum hasAceKingEnabledInSuit() const;
		int getCardIndexOf(Card::RankEnum rank, Card::SuitEnum suit) const;
		bool playedLowerCardThan(int p1,int p2) const;
		bool playedLowerCardThan(const Card& p1c, const Card& p2c) const;
		bool hasAnyEnabledInSuit(Card::SuitEnum suit) const;
		int getNumEnabledCardsInSuit(Card::SuitEnum suit) const;
		Card::SuitEnum getLeadingSuit() const;
		bool isSuitOnTable(Card::SuitEnum suit) const;
		int hasCardHigherThanTable(Card::SuitEnum suit) const;
		bool playedHigherCardThan(int p1,int p2) const;
		Card::SuitEnum getNthSuitWithLowestCountHaving(int countFromLast, bool skipSpades, Card::RankEnum having) const;
		Card::SuitEnum getNthSuitWithHighestCountHaving(int countFromLast, bool skipSpades, Card::RankEnum having) const;
		bool anyCardsBetterThanCard(const Card& c) const;
		bool allCardsBetterThanCard(const Card& c) const;
		Card::SuitEnum hasSuitWithAceAndLessThanNCards(int numCards, bool avoidSpades) const;
		int isLastToBid() const;
		int getBidSum() const;
		
		int playHighestCard() const;
		int playLowestCard() const;
		int playLowestCardAbovePlayer(int player) const;
		int playLowestCardFollowingSuit() const;
		int playLowestCardInSuit(Card::SuitEnum suit) const;
		int playHighestCardFollowingSuit() const;
		int playHighestCardInSuit(Card::SuitEnum suit) const;
		int playHighestCardUnderTable() const;
		int tryToMakePoint() const;
		int tryNotToMakePoint() const;
		int playHighestCardWithLeast(bool avoidSpades) const;
		int playLowestCardWithLeast(bool avoidSpades, bool hasMin) const;
		int playHighestCardWithMost(bool avoidSpades) const;
		int playLowestCardWithMost(bool avoidSpades) const;
		int playLowestCardWithMostLeadVersion(bool avoidSpades, bool hasMin) const;
		int playLowestAboveTableInSpades() const;

		int getTrickSum() const;
		bool isThirdToBid() const;
		int getNumNils() const;

		bool nilRegular() const;
		bool nilPass2() const;
		int regularBid() const;
		int pass2Bid() const;
		NetAIPoint pass2SwapCards() const;
		int calcBasicBid() const;
		int playRegularCard() const;

		int calcMathBid() const;
		virtual ~NetAIPartnershipSpadesBot(void);
	};
}

#endif
