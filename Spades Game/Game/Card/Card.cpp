#include "Card.hpp"

namespace cge
{
	Card::Card( SuitEnum suit, RankEnum rank )
		: m_suit(suit), m_rank(rank)
	{
	}

	Card::~Card(void)
	{
	}

	bool Card::isBlack() const
	{
		return m_suit == CLUBS || m_suit == SPADES;
	}

	bool Card::isRed() const
	{
		return m_suit == HEARTS || m_suit == DIAMONDS;
	}

	Card::RankEnum Card::getRank() const
	{
		return m_rank;
	}

	Card::SuitEnum Card::getSuit() const
	{
		return m_suit;
	}

	bool Card::isRoyal() const
	{
		return m_rank == KING 
			|| m_rank == QUEEN 
			|| m_rank == JACK;
	}

	bool Card::isAce() const
	{
		return m_rank == ACE;
	}

	bool Card::isNumeric() const
	{
		return !isRoyal() && !isAce();
	}

	std::string Card::getSuitString() const
	{
		switch (getSuit())
		{
		case HEARTS:
			return "Hearts";
			break;
		case CLUBS:
			return "Clubs";
			break;
		case DIAMONDS:
			return "Diamonds";
			break;
		case SPADES:
			return "Spades";
			break;
		default:
			return "Unknown Suit";
		}
	}

	std::string Card::getRankString() const
	{
		switch(getRank())
		{
		case ACE:
			return "Ace";
			break;
		case TWO:
			return "2";
			break;
		case THREE:
			return "3";
			break;
		case FOUR:
			return "4";
			break;
		case FIVE:
			return "5";
			break;
		case SIX:
			return "6";
			break;
		case SEVEN:
			return "7";
			break;
		case EIGHT:
			return "8";
			break;
		case NINE:
			return "9";
			break;
		case TEN:
			return "10";
			break;
		case JACK:
			return "Jack";
			break;
		case QUEEN:
			return "Queen";
			break;
		case KING:
			return "King";
			break;
		default:
			return "Unknown Rank";
		}
	}

	void Card::setSuit( SuitEnum suit )
	{
		m_suit = suit;
	}

	void Card::setRank( RankEnum rank )
	{
		m_rank = rank;
	}

	int Card::getCardNumber() const
	{
		if(getRank() == ANY_RANK)
		{
			return -1;
		}

		int count = 0;
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 13; ++j)
			{
				if(i == (int)getSuit() && j == (int)getRank())
				{
					return count;
				}

					count++;
			}
		}

		return count;
	}

	bool Card::operator<( const Card& b ) const
	{
		if(getSuit() == b.getSuit())
		{
			return getRank() < b.getRank();
		}
		else
		{
			return getSuit() < b.getSuit();
		}
	}

	bool Card::operator==( const Card& b ) const
	{
		return getCardNumber() == b.getCardNumber();
	}

	bool Card::operator!=( const Card& b ) const
	{
		return !(*this == b);
	}

	cge::Card Card::cardFromNumber(int number)
	{
		int count = 0;
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 13; ++j)
			{
				if(count == number)
				{
					return Card((SuitEnum)i,(RankEnum)j);
				}

				count++;
			}
		}

		return Card(DIAMONDS,ANY_RANK);
	}

	std::string Card::getShortRankString() const
	{
		switch(getRank())
		{
		case ACE:
			return "A";
			break;
		case TWO:
			return "2";
			break;
		case THREE:
			return "3";
			break;
		case FOUR:
			return "4";
			break;
		case FIVE:
			return "5";
			break;
		case SIX:
			return "6";
			break;
		case SEVEN:
			return "7";
			break;
		case EIGHT:
			return "8";
			break;
		case NINE:
			return "9";
			break;
		case TEN:
			return "10";
			break;
		case JACK:
			return "J";
			break;
		case QUEEN:
			return "Q";
			break;
		case KING:
			return "K";
			break;
		default:
			return "Unknown Rank";
		}
	}

}
