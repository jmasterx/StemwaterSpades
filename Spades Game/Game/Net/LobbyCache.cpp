#include "Game/Net/LobbyCache.hpp"
#include <algorithm>
#include <iostream>
namespace cge
{
	LobbyCache::LobbyCache(void)
		: m_needsFullUpdate(false),
		m_tableBegin(-1),m_highStakesVal(0)
	{
	}

	LobbyCache::~LobbyCache(void)
	{
	}

	void LobbyCache::setFilters( const std::vector<TableFilterEnum>& filters )
	{
		m_filters = filters;
	}

	void LobbyCache::createTables( int start, int quantity )
	{
		m_tables.clear();
		m_tableBegin = start;

		for(int i = start; i < start + quantity; ++i)
		{
			m_tables.push_back(Table(i));
		}
	}

	std::vector<LobbyCache::Table>& LobbyCache::getTables()
	{
		return m_tables;
	}

	LobbyCache::Table* LobbyCache::getTableAt( int tableNo )
	{
		for(size_t i = 0; i < m_tables.size(); ++i)
		{
			if(m_tables[i].getTableNo() == tableNo)
			{
				return &m_tables[i];
			}
		}

		return NULL;
	}

	std::vector<TableFilterEnum>& LobbyCache::getTableFilters() 
	{
		return m_filters;
	}

	void LobbyCache::clear()
	{
		m_tables.clear();
		m_tableBegin = -1;
	}

	void LobbyCache::setNeedsFullUpdate( bool update )
	{
		m_needsFullUpdate = update;
	}

	bool LobbyCache::needsFullUpdate() const
	{
		return m_needsFullUpdate;
	}

	int LobbyCache::getTableBegin() const
	{
		return m_tableBegin;
	}

	int LobbyCache::getNumTables() const
	{
		return m_tables.size();
	}

	const std::vector<int>& LobbyCache::getTableFees() const
	{
		return m_tableFees;
	}

	void LobbyCache::setTableFees( const std::vector<int>& fees )
	{
		m_tableFees = fees;
	}

	void LobbyCache::setHighStakesValue( int val )
	{
		m_highStakesVal = val;
	}

	int LobbyCache::getHighStakesVal() const
	{
		return m_highStakesVal;
	}

	void LobbyCache::setMinRatingValues( const std::vector<int>& values )
	{
		m_minRatings = values;
	}

	const std::vector<int>& LobbyCache::getMinRatingValues() const
	{
		return m_minRatings;
	}

	LobbyCache::Chair::Chair()
		: m_player(NULL),m_subSeat(false)
	{

	}

	void LobbyCache::Chair::setPayer( ClientPlayer* player )
	{
		m_player = player;
	}

	void LobbyCache::Chair::setAsSubSeat( bool subseat )
	{
		m_subSeat = subseat;
	}

	bool LobbyCache::Chair::isSubSeat() const
	{
		return m_subSeat;
	}

	bool LobbyCache::Chair::isOccupied() const
	{
		return m_player != NULL;
	}

	ClientPlayer* LobbyCache::Chair::getPlayer()
	{
		return m_player;
	}


	LobbyCache::Table::Table( int tableNo )
		: 	m_tableNo(tableNo),m_active(false),
		m_beingSetup(false)
	{
		for(int i = 0; i < 4; ++i)
		{
			m_chairs.push_back(Chair());
		}
	}

	int LobbyCache::Table::getTableNo() const
	{
		return m_tableNo;
	}

	void LobbyCache::Table::setTableInfo( const SpadesGameInfo& info )
	{
		m_info = info;
	}

	void LobbyCache::Table::setActive( bool active )
	{
		m_active = active;
	}

	bool LobbyCache::Table::isActive() const
	{
		return m_active;
	}

	void LobbyCache::Table::setBeingSetup( bool setup )
	{
		m_beingSetup = setup;
	}

	bool LobbyCache::Table::isBeingSetup() const
	{
		return m_beingSetup;
	}

	LobbyCache::Chair* LobbyCache::Table::getChairAt( int chairNo )
	{
		if(chairNo < getNumChairs())
		{
			return &m_chairs[chairNo];
		}

		return NULL;
	}

	int LobbyCache::Table::getNumChairs() const
	{
		return m_chairs.size();
	}

	const SpadesGameInfo& LobbyCache::Table::getGameInfo() const
	{
		return m_info;
	}

	bool LobbyCache::Table::statusChange( bool sitting )
	{
		int numPlayers = getNumOccupants();
		int numOc = numPlayers;
		if(sitting)
		{
			numOc--;
		}

		bool status = numOc == m_info.getNumPlayers();

		int incr = sitting ? 0 : -1;

		bool newStatus = numPlayers + incr == m_info.getNumPlayers() || m_info.isBotGame();

		return status != newStatus;
	}

	int LobbyCache::Table::getNumOccupants()
	{
		int o = 0;
		for(int i = 0; i < getNumChairs(); ++i)
		{
			if(getChairAt(i)->getPlayer() != NULL)
			{
				o++;
			}
		}

		return o;
	}

	void LobbyCache::Table::updatePlayersInGame( bool newStatus )
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
			if(getChairAt(i)->getPlayer() != NULL)
			{
				getChairAt(i)->getPlayer()->setInGame(newStatus);
			}
		}
	}

	void LobbyCache::Table::addWatcher( ClientPlayer* p )
	{
		removeWatcher(p);
		m_watchers.push_back(p);
	}

	void LobbyCache::Table::removeWatcher( ClientPlayer* p )
	{
		m_watchers.erase(
			std::remove(m_watchers.begin(),
			m_watchers.end(), p),
			m_watchers.end());
	}

	void LobbyCache::Table::clearWatchers()
	{
		m_watchers.clear();
	}

	bool LobbyCache::Table::isPlayerWatching( const std::string& player ) const
	{
		for(size_t i = 0; i < m_watchers.size(); ++i)
		{
			if(m_watchers[i]->getUsername() == player)
			{
				return true;
			}
		}

		return false;
	}

	int LobbyCache::Table::getPlayerPos( const std::string& player ) 
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
			if(getChairAt(i)->getPlayer() != NULL)
			{
				if(getChairAt(i)->getPlayer()->getUsername() == player)
				{
					return i;
				}
			}
		}

		return -1;
	}

	int LobbyCache::Table::getNumWatchers() const
	{
		return m_watchers.size();
	}

	ClientPlayer* LobbyCache::Table::getWatcherAt( int watcherNo )
	{
		if(watcherNo >= 0 && watcherNo < m_watchers.size())
		{
			return m_watchers[watcherNo];
		}

		return NULL;
	}

}
