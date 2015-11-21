#include "Game/Net/NetMediator.hpp"
#include <iostream>
namespace cge
{
	NetMediator::NetMediator(void)
	{
	}

	NetMediator::~NetMediator(void)
	{
	}

	void NetMediator::sendGameMessage( SpadesCSMessageEnum msg, const std::vector<int>& args, SpadesGameMessageSender* sender )
	{
			if(sender == getClient())
			{
				DISPATCH_GAME_EVENT
					(*it)->gameMessageSend(msg,args);
			}
	}

	void NetMediator::gameMessageReceive( SpadesCSMessageEnum message, const std::vector<int>& params, int player )
	{
		getClient()->sendGameMessage(message,params,this);
	}

}
