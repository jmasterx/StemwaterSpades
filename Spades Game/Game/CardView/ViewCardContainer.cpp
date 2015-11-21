#include "Game/CardView/ViewCardContainer.hpp"

namespace cge
{
	ViewCardContainer::ViewCardContainer( int numCards )
	{
		m_cards.reserve(numCards);
		for(int i = 0; i < numCards; ++i)
		{
			m_cards.push_back(std::make_pair(ViewCard(),false));
		}
	}

	ViewCardContainer::~ViewCardContainer(void)
	{
	}

	void ViewCardContainer::freeAll()
	{
		for(size_t i = 0; i < m_cards.size(); ++i)
		{
			m_cards[i].second = false;
		}
	}

	bool ViewCardContainer::release( ViewCard* card )
	{
		for(size_t i = 0; i < m_cards.size(); ++i)
		{
			if(&m_cards[i].first == card)
			{
				m_cards[i].second = false;
				return true;
			}
		}

		return false;
	}

	ViewCard* ViewCardContainer::request()
	{
		for(size_t i = 0; i < m_cards.size(); ++i)
		{
			if(!m_cards[i].second)
			{
				m_cards[i].second = true;
				return &m_cards[i].first;
			}
		}

			return NULL;
	}
}
