#include "Game/SpadesGame/SpadesGameMessage.hpp"

namespace cge
{
	SpadesGameMessage::SpadesGameMessage( SpadesCSMessageEnum message,
		const std::vector<int>& args, int player ) 
		: m_message(message), m_args(args), m_player(player)
	{

	}

	SpadesGameMessage::SpadesGameMessage( SpadesCSMessageEnum message, int player )
		: m_message(message),m_player(player)
	{
	}

	int SpadesGameMessage::getPlayer() const
	{
		return m_player;
	}

	const std::vector<int>& SpadesGameMessage::getArgs() const
	{
		return m_args;
	}

	SpadesCSMessageEnum SpadesGameMessage::getMessage() const
	{
		return m_message;
	}


}
