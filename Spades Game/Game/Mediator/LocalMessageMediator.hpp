#ifndef CGE_LOCAL_MESSAGE_MEDIATOR_HPP
#define CGE_LOCAL_MESSAGE_MEDIATOR_HPP
#include "Game/Mediator/MessageMediator.hpp"
namespace cge
{
	class LocalMessageMediator : public MessageMediator
	{
		SpadesGameMessageSender* m_gameMessenger;
	protected:
		SpadesGameMessageSender* getGameMessenger();
	public:
		LocalMessageMediator();
		void setGameMessenger(SpadesGameMessageSender* gameMessenger);
		virtual void sendGameMessage(SpadesCSMessageEnum msg,
			const std::vector<int>& args, SpadesGameMessageSender* sender);
		virtual ~LocalMessageMediator(void);
	};
}

#endif
