#ifndef CGE_MESSAGE_MEDIATOR_HPP
#define CGE_MESSAGE_MEDIATOR_HPP
#include "Game/SpadesGame/SpadesMessageSender.hpp"
namespace cge
{
	class MessageMediator : public SpadesMessageSender
	{	
	SpadesMessageSender* m_client;
	protected:
		SpadesMessageSender* getClient();
	public:
		void setClient(SpadesMessageSender* client);
		MessageMediator();
		virtual ~MessageMediator(void);
	};
}

#endif