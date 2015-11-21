#ifndef CGE_SPADES_GAME_MESSAGE_SENDER_HPP
#define CGE_SPADES_GAME_MESSAGE_SENDER_HPP
#include "Game/SpadesGame/SpadesGameMessages.hpp"
#include <stdlib.h>
#include <vector>
namespace cge
{
	class SpadesGameMessageSender
	{
	public:
		SpadesGameMessageSender(void);
		virtual void sendGameMessage(SpadesCSMessageEnum msg,
					const std::vector<int>& args, SpadesGameMessageSender* sender) = 0;
		virtual ~SpadesGameMessageSender(void);
	};
}

#endif
