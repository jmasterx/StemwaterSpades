#include "Game/Mediator/LocalMessageMediator.hpp"

namespace cge
{
	LocalMessageMediator::LocalMessageMediator(void)
		: m_gameMessenger(NULL)
	{
	}

	LocalMessageMediator::~LocalMessageMediator(void)
	{
	}

	SpadesGameMessageSender* LocalMessageMediator::getGameMessenger()
	{
		return m_gameMessenger;
	}

	void LocalMessageMediator::sendGameMessage( SpadesCSMessageEnum msg, 
		const std::vector<int>& args, SpadesGameMessageSender* sender )
	{
		if(sender == getGameMessenger())
		{
			getClient()->sendGameMessage(msg,args, this );
		}
		else if(sender == getClient())
		{
			getGameMessenger()->sendGameMessage(msg,args, this );
		}
	}

	void LocalMessageMediator::setGameMessenger( SpadesGameMessageSender* gameMessenger )
	{
		m_gameMessenger = gameMessenger;
	}

}
