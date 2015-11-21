#ifndef CGE_CARD_SHUFFLER_HPP
#define CGE_CARD_SHUFFLER_HPP
#include "Game/Utility/MersenneTwister.hpp"
#include <vector>
#include "Game/Card/Card.hpp"
namespace cge
{
	class CardShuffler
	{
		MersenneTwister m_randomGenerator;
	public:
		CardShuffler(void);
		std::vector<Card> shuffleCards(int zeroBasedCut);
		long nextUInt();
		int nextRangeInt(int inclLowBound,int inclUpperBound);
		virtual ~CardShuffler(void);
	};
}

#endif
