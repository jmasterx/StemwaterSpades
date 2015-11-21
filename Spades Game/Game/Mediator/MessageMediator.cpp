#include "Game/Mediator/MessageMediator.hpp"
namespace cge
{
	MessageMediator::MessageMediator()
		: m_client(NULL)
	{
	}

	MessageMediator::~MessageMediator(void)
	{
	}

	SpadesMessageSender* MessageMediator::getClient()
	{
		return m_client;
	}

	void MessageMediator::setClient( SpadesMessageSender* client )
	{
		m_client = client;
	}

}
