#include "Game/CardView/CardAnimationEvent.hpp"

namespace cge
{
	CardAnimationEvent::~CardAnimationEvent(void)
	{
	}

	void CardAnimationEvent::step()
	{
		if(isFinished() && m_noReturn)
		{
			return;
		}
		cleanup();
		if(isFinished())
		{
			m_messenger->sendMessage(m_message,getId());
			m_messenger->sendMessage(CardMessenger::END_ANIMATED_EVENT,getId());
			m_noReturn = true;
		}
		for(size_t i = 0; i < m_animations.size(); ++i)
		{
			m_animations[i].step();
		}
	}

	bool CardAnimationEvent::isFinished() const
	{
		return m_animations.empty();
	}

	void CardAnimationEvent::cleanup()
	{
		bool needsCleaning = true;
		do 
		{
			needsCleaning = false;

			for(size_t i = 0; i < m_animations.size(); ++i)
			{
				if(m_animations[i].getInterval() == 1.0f)
				{
					needsCleaning = true;
					m_animations.erase(m_animations.begin() + i);
				}
			}
		} while (needsCleaning);
	}

	int CardAnimationEvent::getId() const
	{
		return m_id;
	}

	void CardAnimationEvent::add( const CardAnimation& animation )
	{
		m_animations.push_back(animation);
		m_cards.push_back(m_animations.back().getCard());
	}

	CardAnimationEvent::CardAnimationEvent( CardMessenger* messenger, 
		int id, CardMessenger::CardMessage message )
		: m_messenger(messenger), m_message(message),m_id(id),
		m_noReturn(false)
	{

	}

	bool CardAnimationEvent::contains( ViewCard* card )
	{
		for(size_t i = 0; i < m_animations.size(); ++i)
		{
			if(m_animations[i].getCard() == card)
			{
				return true;
			}
		}

		return false;
	}

	CardMessenger::CardMessage CardAnimationEvent::getMessage() const
	{
		return m_message;
	}

	ViewCard* CardAnimationEvent::getCard() const
	{
		if(m_cards.size() > 0)
		return m_cards.back();
		else
		return NULL;
	}

	const std::vector<ViewCard*>& CardAnimationEvent::getCards() const
	{
		return m_cards;
	}

}
