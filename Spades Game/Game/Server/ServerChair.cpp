#include "Game/Server/ServerChair.hpp"
namespace cge
{
	ServerChair::ServerChair(void)
		: m_player(NULL), m_subSeat(false),m_originalPid(-1),
		m_lastPid(-1),m_botGiftTimeout(0.0f),m_botGiftInterval(13.0f),
		m_turnTime(0.0f),m_numTimeouts(0),m_isTurn(false),m_sentFirstStrike(false)
	{
	}

	ServerChair::~ServerChair(void)
	{
	}

	void ServerChair::setPlayer( ServerPlayer* player )
	{
		m_player = player;
	}

	void ServerChair::setAsSubSeat( bool subseat )
	{
		m_subSeat = subseat;
	}

	bool ServerChair::isSubSeat() const
	{
		return m_subSeat;
	}

	bool ServerChair::isOccupied() const
	{
		return m_player != NULL;
	}

	ServerPlayer* ServerChair::getPlayer()
	{
		return m_player;
	}

	void ServerChair::setOriginalPid( int64 id )
	{
		m_originalPid = id;
	}

	int64 ServerChair::getOriginalPid() const
	{
		return m_originalPid;
	}

	void ServerChair::clearGiftData()
	{
		m_giftDat.clear();
	}

	const ServerGiftData& ServerChair::getGiftData() const
	{
		return m_giftDat;
	}

	void ServerChair::setGiftData( int fromPlayer, int giftID )
	{
		m_giftDat.frame = 0;
		m_giftDat.fromPlayer = fromPlayer;
		m_giftDat.giftID = giftID;
		m_botGiftTimeout = 0.0f;
		m_botGiftInterval = 9 + (rand() % (45 - 9));
	}

	bool ServerChair::isGiftDataSet() const
	{
		return m_giftDat.giftID > -1;
	}

	void ServerChair::updateGiftData( int newFrame )
	{
		m_giftDat.frame = newFrame;
		m_botGiftInterval = 9 + (rand() % (45 - 9));
	}

	void ServerChair::setLastPid( int64 pid )
	{
		m_lastPid = pid;
	}

	int64 ServerChair::getLastPid() const
	{
		return m_lastPid;
	}

	bool ServerChair::updateBotGift( float t )
	{
		if(isOccupied() || m_giftDat.giftID < 0)
			return false;

		m_botGiftTimeout += t;

		if(m_botGiftTimeout >= m_botGiftInterval)
		{
			m_botGiftTimeout = 0;
			return true;
		}

		return false;
	}

	void ServerChair::turnStarted()
	{
		m_turnTime = 0.0f;
		m_isTurn = true;
		m_sentFirstStrike = false;
	}

	void ServerChair::turnEnded()
	{
		m_turnTime = 0.0f;
		m_isTurn = false;
		m_sentFirstStrike = true;
	}

	void ServerChair::resetTurnData()
	{
		m_isTurn = false;
		m_turnTime = 0.0;
		m_numTimeouts = 0;
		m_sentFirstStrike = false;
	}

	void ServerChair::increaseTurnTime( double t )
	{
		m_turnTime += t;
	}

	void ServerChair::resetTurnTime()
	{
		m_turnTime = 0.0;
	}

	double ServerChair::getTurnTime() const
	{
		return m_turnTime;
	}

	void ServerChair::addTurnStrike()
	{
		m_numTimeouts++;
	}

	int ServerChair::getTurnStrikes() const
	{
		return m_numTimeouts;
	}

	bool ServerChair::isTurn() const
	{
		return m_isTurn;
	}

	bool ServerChair::sentFirstStrike() const
	{
		return m_sentFirstStrike;
	}

	void ServerChair::sendFirstStrike()
	{
		m_sentFirstStrike = true;
	}

}
