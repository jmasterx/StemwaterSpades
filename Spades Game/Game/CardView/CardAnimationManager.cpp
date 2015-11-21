#include "Game/CardView/CardAnimationManager.hpp"

namespace cge
{
	CardAnimationManager::CardAnimationManager( CardMessenger* messenger )
		: m_messenger(messenger), m_animationIDCount(0)
	{

	}

	CardAnimationManager::~CardAnimationManager(void)
	{
	}

	CardAnimationEvent* CardAnimationManager::getAnimationById( int id )
	{
		for(size_t i = 0; i < m_cardAnims.size(); ++i)
		{
			if(m_cardAnims[i].getId() == id)
			{
				return &m_cardAnims[i];
			}
		}

		return NULL;
	}
	void CardAnimationManager::stepAnimations()
	{
		for(size_t i = 0; i < m_cardAnims.size(); ++i)
		{
			m_cardAnims[i].step();
		}
	}

	int CardAnimationManager::pushAnimationEvent(
		CardMessenger::CardMessage beginMessage, CardMessenger::CardMessage message )
	{
		int id = m_animationIDCount;
		m_cardAnims.push_back(CardAnimationEvent(m_messenger,id,message));
		m_animationIDCount++;

		if(m_animationIDCount > 1000000)
		{
			m_animationIDCount = 1000;
		}

		m_messenger->sendMessage(beginMessage,id);
		m_messenger->sendMessage(CardMessenger::BEGIN_ANIMATED_EVENT,id);

		return id;
	}

	void CardAnimationManager::addAnimation( ViewCard* card,
		const ViewCard& end,float rate, 
		int stepDelay, bool unrestrictedAngle /*= false*/ )
	{
		if(m_cardAnims.size() == 0)
		{
			return;
		}

		m_cardAnims.back().add(CardAnimation(
			card,*card,end,rate,stepDelay,unrestrictedAngle));
	}

	void CardAnimationManager::addAnimation( ViewCard* card, const ViewCard& end,float rate, int stepDelay, bool unrestrictedAngle, const std::vector<CardAnimationChange>& changes )
	{
		if(m_cardAnims.size() == 0)
		{
			return;
		}

		CardAnimation c = CardAnimation(
			card,*card,end,rate,stepDelay,unrestrictedAngle);
		c.setChanges(changes);
		m_cardAnims.back().add(c);
	}

	bool CardAnimationManager::cancelAnimationEvent( CardMessenger::CardMessage message )
	{
		for(size_t i = 0; i < m_cardAnims.size(); ++i)
		{
			if(m_cardAnims[i].getMessage() == message)
			{
				m_cardAnims.erase(m_cardAnims.begin() + i);
				return true;
			}
		}

		return false;
	}

	bool CardAnimationManager::cancelAnimationEvent( ViewCard* card, 
		CardMessenger::CardMessage message )
	{
		for(size_t i = 0; i < m_cardAnims.size(); ++i)
		{
			if(m_cardAnims[i].getMessage() == message &&
				m_cardAnims[i].contains(card))
			{
				m_cardAnims.erase(m_cardAnims.begin() + i);
				return true;
			}
		}

		return false;
	}

	bool CardAnimationManager::removeAnimationEvent( int id )
	{
		for(size_t i = 0; i < m_cardAnims.size(); ++i)
		{
			if(m_cardAnims[i].getId() == id)
			{
				m_cardAnims.erase(m_cardAnims.begin() + i);
				return true;
			}
		}

		return false;
	}

	int CardAnimationManager::getCount() const
	{
		return m_cardAnims.size();
	}

	bool CardAnimationManager::hasAnimationType( CardMessenger::CardMessage msg ) const
	{
		for(size_t i = 0; i < m_cardAnims.size(); ++i)
		{
			if(m_cardAnims[i].getMessage() == msg)
			{
				return true;
			}
		}

		return false;
	}

}
