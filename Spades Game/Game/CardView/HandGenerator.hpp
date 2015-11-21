#ifndef CGE_HAND_GENERATOR_HPP
#define CGE_HAND_GENERATOR_HPP
#include "Game/CardView/ViewCard.hpp"
#include <cmath>
#include "Game/Utility/Vec2.hpp"
#include "Game/CardView/ViewCardContainer.hpp"
#include <vector>
namespace cge
{
	class HandGenerator
	{
		float m_shadowIntensity;
		Vec2 m_shadowNormal;
		float m_shadowDistance;
		ViewCardContainer* m_container;
	public:
		void releaseCards(std::vector<ViewCard*>& cards ) const;
		void generateHand(int playerNum, int numCards, 
			std::vector<ViewCard*>& cards) const;
		void generateDeck(std::vector<ViewCard*>& cards) const;
		HandGenerator(ViewCardContainer* container);
		virtual ~HandGenerator(void);
	};
}

#endif