#ifndef CGE_RENDERED_CARD_CONTAINER_HPP
#define CGE_RENDERED_CARD_CONTAINER_HPP
#include "Game/CardView/ViewCard.hpp"
#include <stdlib.h>
#include <vector>

namespace cge
{
	class ViewCardContainer
	{
		std::vector<std::pair<ViewCard,bool> > m_cards;
	public:
		void freeAll();
		bool release(ViewCard* card);
		ViewCard* request();
		ViewCardContainer(int numCards);
		virtual ~ViewCardContainer(void);
	};
}
#endif