#include "Game/Client/SpadesClient.hpp"

namespace cge
{
	SpadesClient::SpadesClient(void)
		:m_isHost(false)
	{
	}

	SpadesClient::~SpadesClient(void)
	{
	}

	SpadesGameMessageSender* SpadesClient::getMediator()
	{
		return m_mediator;
	}

	void SpadesClient::setMediator( SpadesGameMessageSender* mediator )
	{
		m_mediator = mediator;
	}

	void SpadesClient::sendMessageToGame( SpadesCSMessageEnum msg, const std::vector<int>& args )
	{
		getMediator()->sendGameMessage(msg,args,this);
	}

	void SpadesClient::sendGameMessage( SpadesCSMessageEnum msg,
		const std::vector<int>& args, SpadesGameMessageSender* sender )
	{
		processMessageFromGame(msg,args);
	}

	void SpadesClient::setAsHost( const std::vector<int>& gameInfo )
	{
		sendMessageToGame(SET_GAME_INFO_C,gameInfo);
		setGameInfo(gameInfo);
	}

	bool SpadesClient::isHost() const
	{
		return m_isHost;
	}

	bool SpadesClient::setGameInfo( const std::vector<int>& gameInfo )
	{
		m_gameInfo = SpadesGameInfo(gameInfo);

		return true;
	}

	const SpadesGameInfo& SpadesClient::getGameInfo() const
	{
		return m_gameInfo;
	}

}
