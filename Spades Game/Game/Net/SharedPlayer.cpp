#include "Game/Net/SharedPlayer.hpp"

namespace cge
{
	SharedPlayer::SharedPlayer(void)
		: m_tableID(-1),
		m_seatID(-1),m_watchTableID(-1),m_avatar(1)
	{
	}

	SharedPlayer::~SharedPlayer(void)
	{
	}

	void SharedPlayer::setUsername( const std::string& name )
	{
		m_username = name;
	}

	const std::string& SharedPlayer::getUsername() const
	{
		return m_username;
	}

	void SharedPlayer::setTableId( int tableId )
	{
		m_tableID = tableId;
	}

	void SharedPlayer::setSeat( int seat )
	{
		m_seatID = seat;
	}

	int SharedPlayer::getTableId() const
	{
		return m_tableID;
	}

	int SharedPlayer::getSeat() const
	{
		return m_seatID;
	}

	void SharedPlayer::setWatchTableId( int table )
	{
		m_watchTableID = table;
	}

	int SharedPlayer::getWatchTableId() const
	{
		return m_watchTableID;
	}

	void SharedPlayer::setAvatar( int avatar )
	{
		m_avatar = avatar;
	}

	int SharedPlayer::getAvatar() const
	{
		return m_avatar;
	}
}

