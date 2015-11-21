#ifndef CGE_SPADES_MESSAGE_SENDER_HPP
#define CGE_SPADES_MESSAGE_SENDER_HPP
#include "Game/SpadesGame/SpadesGameMessageSender.hpp"
#include <stdlib.h>
#include <vector>
namespace cge
{
	class SpadesMessageSender : public SpadesGameMessageSender
	{
	public:
		SpadesMessageSender(void);
			virtual ~SpadesMessageSender(void);
	};
}
#endif
