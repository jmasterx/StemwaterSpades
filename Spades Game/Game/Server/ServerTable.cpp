#include "Game/Server/ServerTable.hpp"
#include <iostream>
namespace cge
{
	ServerTable::ServerTable( int tableNo )
		: m_active(false), m_inProgress(false),
		m_tableNo(tableNo), m_beingSetup(false),
		m_paused(false),m_gameId(-1),m_subTimeout(0.0f),
		m_requestTimeout(0.0f),m_subResponseTimeout(0)
	{
		m_chairs.resize(4);
		m_game.setTableNo(tableNo);
		allowPlayerRequest();
	}

	ServerTable::~ServerTable(void)
	{
	}

	ServerChair* ServerTable::getChairAt( int i )
	{
		if(i < m_chairs.size())
		{
			return &m_chairs[i];
		}

		return NULL;
	}

	int ServerTable::getNumChairs() const
	{
		return m_chairs.size();
	}

	void ServerTable::setActive( bool active )
	{
		m_active = active;
	}

	bool ServerTable::isActive() const
	{
		return m_active;
	}

	void ServerTable::setInProgress( bool inProgress )
	{
		m_inProgress = inProgress;

		if(!inProgress)
		{
			clearKickedPlayers();
			clearVotes();
			resetSubTimeout();
			resetSubResponseTimeout();
			clearWaitList();
			clearSubWaitList();
			allowPlayerRequest();
			clearDisconnectedPlayers();
			for(int i = 0; i < getNumChairs(); ++i)
			{
				ServerChair* c = getChairAt(i);
				if(c)
				c->setAsSubSeat(false);
				if(c)
				c->setOriginalPid(-1);
				if(c)
					c->setLastPid(-1);
				if(c)
					c->clearGiftData();
				if(c)
					c->resetTurnData();
			}
		}
		else
		{
			for(int i = 0; i < getNumChairs(); ++i)
			{
				ServerChair* c = getChairAt(i);
				c->setOriginalPid(-1);
				if(c && c->getPlayer() != NULL)
					c->setOriginalPid(c->getPlayer()->getId());
			}
			allowPlayerRequest();
		}
	}

	bool ServerTable::isInProgress() const
	{
		return m_inProgress;
	}

	void ServerTable::setPaused( bool paused )
	{
		m_paused = paused;
	}

	bool ServerTable::isPaused() const
	{
		return m_paused;
	}

	int ServerTable::getTableNo() const
	{
		return m_tableNo;
	}

	void ServerTable::setBeingSetup( bool beingSetup )
	{
		m_beingSetup = beingSetup;
		if(beingSetup)
		{
			m_game.reset();
			setInProgress(false);
		}
		
	}

	bool ServerTable::isBeingSetup() const
	{
		return m_beingSetup;
	}

	SpadesGameInfo& ServerTable::getInfo()
	{
		return m_info;
	}

	void ServerTable::setInfo( const SpadesGameInfo& info )
	{
		m_info = info;
	}

	NetSpadesGame& ServerTable::getGame()
	{
		return m_game;
	}

	int ServerTable::getNumOccupants() const
	{
		int o = 0;
		for(size_t i = 0; i < m_chairs.size(); ++i)
		{
			if(m_chairs[i].isOccupied())
			{
				o++;
			}
		}

		return o;
	}

	bool ServerTable::isEmpty() const
	{
		return getNumOccupants() == 0;
	}

	bool ServerTable::isReadyToPlay() const
	{
		return isActive() && (getNumOccupants() == m_info.getNumPlayers() || m_info.isBotGame()) && !isWaitListActive();
	}

	void ServerTable::sitPlayer( int pos, ServerPlayer* p )
	{
		ServerChair* c = getChairAt(pos);

		if(c)
		{
			c->setPlayer(p);

			if(c->getLastPid() == -1)
			{
				if(c->getPlayer())
				{
					c->setLastPid(c->getPlayer()->getId());
				}
			}

			removeWatcher(p);
			getGame().setClient(p->getClient(),pos);

			if(isPaused() && isReadyToPlay())
			{
				setPaused(false);
			}
		}
	}

	void ServerTable::standPlayer( int pos )
	{
		ServerChair* c = getChairAt(pos);

		if(c)
		{
			if(c->getPlayer())
			{
				c->setLastPid(c->getPlayer()->getId());
			}
			c->setPlayer(NULL);
			getGame().setClient(NULL,pos);
			c->resetTurnData();
		
			if(!isPaused() && !isReadyToPlay() && isInProgress())
			{
				setPaused(true);
				resetSubTimeout();
			}
		}
	}

	void ServerTable::addWatcher( ServerPlayer* player )
	{
		m_game.addWatcher(player);
	}

	void ServerTable::removeWatcher( ServerPlayer* player )
	{
		m_game.removeWatcher(player);
	}

	ServerPlayer* ServerTable::getWatcher( int watcher )
	{
		return m_game.getWatcherAt(watcher);
	}

	int ServerTable::getNumWatchers() const
	{
		return m_game.getNumWatchers();
	}

	void ServerTable::clearWaitList()
	{
		m_waitList.clear();
	}

	void ServerTable::verifyWaiter( const std::string& name, bool playAgain )
	{
		for(size_t i = 0; i < m_waitList.size(); ++i)
		{
			if(m_waitList[i].first == name)
			{
				m_waitList[i].second.first = true;
				m_waitList[i].second.second = playAgain;
			}
		}
	}

	void ServerTable::generateWaitList()
	{
		m_waitList.clear();
		for(size_t i = 0; i < m_chairs.size(); ++i)
		{
			if(m_chairs[i].isOccupied())
			{
				m_waitList.push_back(
					std::make_pair(m_chairs[i].getPlayer()->getUsername(),std::make_pair(false,false)));
			}
		}
	}

	bool ServerTable::waitListDone() const
	{
		for(size_t i = 0; i < m_waitList.size(); ++i)
		{
			if(!m_waitList[i].second.first)
			{
				return false;
			}
		}

		return true;
	}

	bool ServerTable::isWaitListActive() const
	{
		return m_waitList.size() > 0;
	}

	bool ServerTable::allWantToPlayAgain() const
	{
		if(!isWaitListActive())
		{
			return false;
		}

		for(size_t i = 0; i < m_waitList.size(); ++i)
		{
			if(!m_waitList[i].second.first || !m_waitList[i].second.second)
			{
				return false;
			}
		}

		return true;
	}

	void ServerTable::clearWatchers()
	{
		getGame().clearWatchers();
	}

	void ServerTable::setGameId( int64 id )
	{
		m_gameId = id;
	}

	int64 ServerTable::getGameId() const
	{
		return m_gameId;
	}

	bool ServerTable::needsSub() const
	{
		return !m_info.isBotGame() && isActive() && isInProgress() && (getNumOccupants() != m_info.getNumPlayers()) && !isWaitListActive();
	}

	void ServerTable::increaseSubTimeout( float amount )
	{
		m_subTimeout += amount;
	}

	void ServerTable::resetSubTimeout()
	{
		m_subTimeout = 0.0f;
	}

	float ServerTable::getSubTimeout() const
	{
		return m_subTimeout;
	}

	void ServerTable::generateSubList()
	{
		m_subList.clear();
		for(size_t i = 0; i < m_chairs.size(); ++i)
		{
			if(m_chairs[i].isOccupied() /*&& m_chairs[i].getPlayer()->getId() == m_chairs[i].getOriginalPid()*/)
			{
				m_subList.push_back(
					std::make_pair(m_chairs[i].getPlayer()->getUsername(),std::make_pair(false,false)));
			}
		}
	}

	bool ServerTable::isSubListActive() const
	{
		return m_subList.size() > 0;
	}

	bool ServerTable::allWantToAnnulGame() const
	{
		if(!subWaitListDone() && getNumOccupants() > 1)
			return false;

		for(size_t i = 0; i < m_subList.size(); ++i)
		{
			if(!m_subList[i].second.first || !m_subList[i].second.second)
			{
				return false;
			}
		}

		return true;
	}

	void ServerTable::verifySubWaiter( const std::string& name, bool annulGame )
	{
		for(size_t i = 0; i < m_subList.size(); ++i)
		{
			if(m_subList[i].first == name)
			{
				m_subList[i].second.first = true;
				m_subList[i].second.second = annulGame;
			}
		}
	}

	void ServerTable::clearSubWaitList()
	{
		m_subList.clear();
	}

	bool ServerTable::subWaitListDone() const
	{
		if(!isSubListActive())
			return false;

		for(size_t i = 0; i < m_waitList.size(); ++i)
		{
			if(!m_waitList[i].second.first)
			{
				return false;
			}
		}

		return true;
	}

	bool ServerTable::anyWantToSubWait() const
	{
		for(size_t i = 0; i < m_subList.size(); ++i)
		{
			if(m_subList[i].second.first && !m_subList[i].second.second)
			{
				return true;
			}
		}

		return false;
	}

	void ServerTable::clearKickedPlayers()
	{
		m_kickedPlayers.clear();
	}

	bool ServerTable::isPlayerKicked( ServerPlayer* p ) const
	{
		for(int i = 0; i < m_kickedPlayers.size(); i++)
		{
			if(m_kickedPlayers[i] == p->getId())
			{
				return true;
			}
		}

		return false;
	}

	void ServerTable::clearVotes()
	{
		m_votes.clear();
	}

	void ServerTable::castVoteAgainstPlayer( int pid, int castid )
	{
		if(hasVoteBeenCastAgainstPlayer(pid,castid))
			return;

		m_votes.push_back(std::make_pair(pid,castid));
	}

	bool ServerTable::shouldPlayerBeKicked( int pid )
	{
		int p1id = -1;
		int p2id = -1;
		int p3id = -1;
		int p4id = -1;

		bool p1vote = false;
		bool p2vote = false;
		bool p3vote = false;
		bool p4vote = false;

		if(m_chairs[0].isOccupied() && m_chairs[0].getPlayer()->getId() != pid)
		{
			p1id = m_chairs[0].getPlayer()->getId();
		}
		else
		{
			p1vote = true;
		}

		if(m_chairs[1].isOccupied() && m_chairs[1].getPlayer()->getId() != pid)
		{
			p2id = m_chairs[1].getPlayer()->getId();
		}
		else
		{
			p2vote = true;
		}

		if(m_chairs[2].isOccupied() && m_chairs[2].getPlayer()->getId() != pid)
		{
			p3id = m_chairs[2].getPlayer()->getId();
		}
		else
		{
			p3vote = true;
		}

		if(m_chairs[3].isOccupied() && m_chairs[3].getPlayer()->getId() != pid)
		{
			p4id = m_chairs[3].getPlayer()->getId();
		}
		else
		{
			p4vote = true;
		}

		for(int i = 0; i < m_votes.size();i++)
		{
			if(m_votes[i].first == pid && m_votes[i].second == p1id)
				p1vote = true;

			if(m_votes[i].first == pid && m_votes[i].second == p2id)
				p2vote = true;

			if(m_votes[i].first == pid && m_votes[i].second == p3id)
				p3vote = true;

			if(m_votes[i].first == pid && m_votes[i].second == p4id)
				p4vote = true;
		}

		return p1vote && p2vote && p3vote && p4vote;
	}

	int ServerTable::getPlayerIDFromName( const std::string& name )
	{
		for(int i = 0; i < m_chairs.size(); i++)
		{
			if(m_chairs[i].isOccupied() && m_chairs[i].getPlayer()->getUsername() == name)
			{
				return m_chairs[i].getPlayer()->getId();
			}
		}

		return -1;
	}

	ServerPlayer* ServerTable::getPlayerFromName( const std::string& name )
	{
		for(int i = 0; i < m_chairs.size(); i++)
		{
			if(m_chairs[i].isOccupied() && m_chairs[i].getPlayer()->getUsername() == name)
			{
				return m_chairs[i].getPlayer();
			}
		}

		return NULL;
	}

	void ServerTable::kickPlayer( ServerPlayer* p)
	{
		if(isPlayerKicked(p))
			return;

		m_kickedPlayers.push_back(p->getId());
	}

	bool ServerTable::hasVoteBeenCastAgainstPlayer( int pid, int castid )
	{
		for(int i = 0; i < m_votes.size(); i++)
		{
			if(m_votes[i].first == pid && m_votes[i].second == castid)
			{
				return true;
			}
		}

		return false;
	}

	void ServerTable::increaseRequestTimeout( float amount )
	{
		m_requestTimeout += amount;
	}

	void ServerTable::resetRequestTimeout()
	{
		m_requestTimeout = 0;
	}

	float ServerTable::getRequestTimeout() const
	{
		return m_requestTimeout;
	}

	void ServerTable::allowPlayerRequest()
	{
		m_requestTimeout = 1000.0f;
	}

	void ServerTable::increaseSubResponseTimeout( float amount )
	{
		m_subResponseTimeout += amount;
	}

	void ServerTable::resetSubResponseTimeout()
	{
		m_subResponseTimeout = 0;
	}

	float ServerTable::getSubResponseTimeout() const
	{
		return m_subResponseTimeout;
	}

	bool ServerTable::wasPlayerDisconnected( int pid ) const
	{
		for(size_t i = 0; i < m_disconnectedPlayers.size(); i++)
		{
			if(m_disconnectedPlayers[i].first == pid)
				return true;
		}

		return false;
	}

	const std::vector<std::pair<int,int> >& ServerTable::getDisconnectedPlayers() const
	{
		return m_disconnectedPlayers;
	}

	void ServerTable::addDisconnectedPlayer( int pid, int seatNo )
	{
		if(!wasPlayerDisconnected(pid))
		{
			m_disconnectedPlayers.push_back(std::make_pair(pid,seatNo));
		}
	}

	void ServerTable::clearDisconnectedPlayers()
	{
		m_disconnectedPlayers.clear();
	}

	void ServerTable::removeDisconnectedPlayer( int pid )
	{
		for(int i = -1; i <= 4; i++)
		m_disconnectedPlayers.erase(std::remove(m_disconnectedPlayers.begin(), 
			m_disconnectedPlayers.end(), std::make_pair(pid,i)), m_disconnectedPlayers.end());
	}

	int ServerTable::getDisconnectedPlayerSeat( int pid )
	{
		for(size_t i = 0; i < m_disconnectedPlayers.size(); i++)
		{
			if(m_disconnectedPlayers[i].first == pid)
				return m_disconnectedPlayers[i].second;
		}

		return -1;
	}

}
