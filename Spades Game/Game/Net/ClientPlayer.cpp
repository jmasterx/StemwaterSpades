#include "Game/Net/ClientPlayer.hpp"
namespace cge
{
	ClientPlayer::ClientPlayer(void)
		: m_tableNo(-1),m_seatNo(-1),
		m_host(false),m_inGame(false),
		m_watchTableNo(-1),m_avatar(1),
		m_bot(false)
	{
	}

	ClientPlayer::~ClientPlayer(void)
	{
	}

	const std::string& ClientPlayer::getUsername() const
	{
		return m_username;
	}

	void ClientPlayer::setUserName( const std::string& userName )
	{
		m_username = userName;
	}

	void ClientPlayer::setTable( int table )
	{
		m_tableNo = table;
	}

	int ClientPlayer::getTable() const
	{
		return m_tableNo;
	}

	void ClientPlayer::setSeat( int seat )
	{
		m_seatNo = seat;
	}

	int ClientPlayer::getSeat() const
	{
		return m_seatNo;
	}

	void ClientPlayer::setHosting( bool hosting )
	{
		m_host = hosting;
	}

	bool ClientPlayer::isSitting() const
	{
		return isAtTable() && getSeat() != -1;
	}

	void ClientPlayer::standUp()
	{
		setSeat(-1);
		setTable(-1);
		m_inGame = false;
	}

	bool ClientPlayer::isAtTable() const
	{
		return  getTable() != -1;
	}

	void ClientPlayer::setInGame( bool inGame )
	{
		m_inGame = inGame;
	}

	bool ClientPlayer::isInGame() const
	{
		return m_inGame;
	}

	void ClientPlayer::setWatchTableNo( int tableNo )
	{
		m_watchTableNo = tableNo;
		setInGame(tableNo != -1);
	}

	int ClientPlayer::getWatchTableNo() const
	{
		return m_watchTableNo;
	}

	bool ClientPlayer::isWatching() const
	{
		return m_watchTableNo != -1;
	}

	void ClientPlayer::setAvatar( int avatar )
	{
		m_avatar = avatar;
	}

	int ClientPlayer::getAvatar() const
	{
		return m_avatar;
	}

	bool ClientPlayer::isBot() const
	{
		return m_bot;
	}

	void ClientPlayer::setIsBot( bool bot )
	{
		m_bot = bot;
	}

}
