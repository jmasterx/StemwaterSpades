#include "Game/CardView/CardMessenger.hpp"
#include "Game/CardView/CardEventListener.hpp"
namespace cge
{
	CardMessenger::CardMessenger(void)
	{
	}

	CardMessenger::~CardMessenger(void)
	{
	}

	void CardMessenger::sendMessage( CardMessage message, int val /*= 0*/ )
	{
		m_q.push(std::make_pair(message,val));
	}

	void CardMessenger::processMessages()
	{
		while(!m_q.empty())
		{
			processMessage(m_q.front().first,m_q.front().second);
			for(std::vector<CardEventListener*>::iterator it = m_listeners.begin();
				it != m_listeners.end(); ++it)
			{
				(*it)->processCardEvent(m_q.front().first,m_q.front().second);
			}
			m_q.pop();
		}
	}

	void CardMessenger::addListener( CardEventListener* listener )
	{
		if(!listener)
		{
			return;
		}
		for(std::vector<CardEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			if((*it) == listener)
				return;
		}

		m_listeners.push_back(listener);
	}

	void CardMessenger::removeListener( CardEventListener* listener )
	{
		m_listeners.erase(
			std::remove(m_listeners.begin(),
			m_listeners.end(), listener),
			m_listeners.end());
	}

}
