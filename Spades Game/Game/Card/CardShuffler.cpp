#include "CardShuffler.hpp"
#include <iostream>
#include <ctime>
#include <algorithm>
namespace cge
{
	CardShuffler::CardShuffler(void)
	{
		m_randomGenerator.init_genrand((long)(
			time(NULL)));
	}

	CardShuffler::~CardShuffler(void)
	{
	}

	std::vector<Card> CardShuffler::shuffleCards( int zeroBasedCut )
	{
		/*
		m_randomGenerator.init_genrand((long)(
			time(NULL) + 
			zeroBasedCut + 
			m_randomGenerator.genrand_int31()));
			*/

		std::vector<Card> shufCards;
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 13; ++j)
			{
				m_randomGenerator.genrand_int31();
				shufCards.push_back(Card((Card::SuitEnum)i,(Card::RankEnum)j));
			}
		}

		std::random_shuffle(shufCards.begin(),shufCards.end());
		for(int i = shufCards.size() - 1; i >= 1; i--)
		{
			int j = m_randomGenerator.genrand_int31() % (i + 1);
			std::swap(shufCards[i],shufCards[j]);
		}

		std::vector<Card> cards;
		for(int i = 0; i < zeroBasedCut; ++i)
		{
			cards.push_back(shufCards[i]);
		}

		for(int i = zeroBasedCut; i < 52; ++i)
		{
			cards.push_back(shufCards[i]);
		}

		return cards;
	}

	long CardShuffler::nextUInt()
	{
		return m_randomGenerator.genrand_int31();
	}

	int CardShuffler::nextRangeInt( int inclLowBound,int inclUpperBound )
	{
		return inclLowBound + (nextUInt() % inclUpperBound);
	}

}
