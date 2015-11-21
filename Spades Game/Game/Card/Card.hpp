#ifndef CGE_CARD_HPP
#define CGE_CARD_HPP
#include <stdlib.h>
#include <string>
namespace cge
{
	class Card
	{
	public:
		enum SuitEnum
		{
			DIAMONDS,
			CLUBS,
			HEARTS,
			SPADES,
			SUIT_ENUM_END
		};

		enum RankEnum
		{
			TWO,
			THREE,
			FOUR,
			FIVE,
			SIX,
			SEVEN,
			EIGHT,
			NINE,
			TEN,
			JACK,
			QUEEN,
			KING,
			ACE,
			ANY_RANK
		};


	private:
		SuitEnum m_suit;
		RankEnum m_rank;
	public:
		static Card cardFromNumber(int number);
		bool isBlack() const;
		bool isRed() const;
		bool isRoyal() const;
		bool isAce() const;
		bool isNumeric() const;
		RankEnum getRank() const;
		SuitEnum getSuit() const;
		std::string getSuitString() const;
		std::string getRankString() const;
		std::string getShortRankString() const;
		int getCardNumber() const;
		void setSuit(SuitEnum suit);
		void setRank(RankEnum rank);
		Card(SuitEnum suit, RankEnum rank);
		bool operator<(const Card& b) const;
		bool operator==(const Card& b) const;
		bool operator!=(const Card& b) const;	
		virtual ~Card(void);
	};
}
#endif
