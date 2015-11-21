#include "Game/SpadesGame/NewGameData.hpp"

namespace cge
{
	NewGameData::NewGameData(void)
		: m_gameInfo(PARTNERSHIP,SCORE_REACHED,ALLOW_MOON_SHOT,
		PASS_TWO,100,500,WANT_DECK_CUT,false,ANYONE_JOIN,false,false,0,0,0,0),
		m_online(false),m_restoring(false)
	{
	}

	NewGameData::~NewGameData(void)
	{
	}

	void NewGameData::setGameInfo( const SpadesGameInfo& info )
	{
		m_gameInfo = info;
	}

	SpadesGameInfo& NewGameData::getGameInfo()
	{
		return m_gameInfo;
	}

	void NewGameData::setOnline( bool online )
	{
		m_online = online;
	}

	bool NewGameData::isOnline() const
	{
		return m_online;
	}

	void NewGameData::setRestoring( bool restoring )
	{
		m_restoring = restoring;
	}

	bool NewGameData::isRestoring() const
	{
		return m_restoring;
	}

	const GameClientState& NewGameData::getRestoreState() const
	{
		return m_state;
	}

	void NewGameData::setRestoreState( const GameClientState& state )
	{
		m_state = state;
	}

}
