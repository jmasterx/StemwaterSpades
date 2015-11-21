#ifndef CGE_NET_MEDIATOR_HPP
#define CGE_NET_MEDIATOR_HPP
#include "Game/Handler/GameEventProvider.hpp"
#include "Game/Mediator/MessageMediator.hpp"
namespace cge
{
	class NetMediator : public MessageMediator,
		public GameEventProvider
	{
	public:
		virtual void sendGameMessage(SpadesCSMessageEnum msg,
			const std::vector<int>& args, SpadesGameMessageSender* sender);
		//from server
		virtual void gameMessageReceive(SpadesCSMessageEnum message, const std::vector<int>& params, int player);
		NetMediator(void);
		virtual ~NetMediator(void);
	};
}

#endif