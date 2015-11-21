#ifndef CGE_SPADES_GAME_MESSAGE_HPP
#define CGE_SPADES_GAME_MESSAGE_HPP
#include "Game/SpadesGame/SpadesGameMessages.hpp"
#include <stdlib.h>
#include <vector>
namespace cge
{
	class SpadesGameMessage
	{
		SpadesCSMessageEnum m_message;
		std::vector<int> m_args;
		int m_player;
	public:
		SpadesGameMessage(SpadesCSMessageEnum message,
			const std::vector<int>& args, int player);
		SpadesGameMessage(SpadesCSMessageEnum message, int player);
		int getPlayer() const;
		const std::vector<int>& getArgs() const;
		SpadesCSMessageEnum getMessage() const;
	};
}

#endif
