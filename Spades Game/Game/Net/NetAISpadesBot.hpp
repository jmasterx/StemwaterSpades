#ifndef CGE_NET_AI_SPADES_BOT_HPP
#define CGE_NET_AI_SPADES_BOT_HPP
#include "Game/SpadesGame/SpadesGameMessage.hpp"
namespace cge
{
	class NetAISpadesBot
	{
	protected:
	public:
		virtual void receive(const SpadesGameMessage& msg) = 0;
		NetAISpadesBot(void) {}
		virtual ~NetAISpadesBot(void) {}
	};
}
#endif
