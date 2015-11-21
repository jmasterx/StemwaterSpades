#ifndef CGE_LOBBY_CACHE_HPP
#define CGE_LOBBY_CACHE_HPP
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Game/Net/ClientPlayer.hpp"
#include "Game/Client/LobbyEventEnums.hpp"
namespace cge
{
	class LobbyCache
	{
	public:
		class Chair
		{
			ClientPlayer* m_player;
			bool m_subSeat;
		public:
			Chair();
			void setPayer(ClientPlayer* player);
			void setAsSubSeat(bool subseat);
			bool isSubSeat() const;
			bool isOccupied() const;
			ClientPlayer* getPlayer();
		};
		class Table
		{
			int m_tableNo;
			SpadesGameInfo m_info;
			bool m_active;
			bool m_beingSetup;
			std::vector<Chair> m_chairs;
			std::vector<ClientPlayer*> m_watchers;
		public:
			Table(int tableNo);
			const SpadesGameInfo& getGameInfo() const;
			int getTableNo() const;
			void setTableInfo(const SpadesGameInfo& info);
			void setActive(bool active);
			bool isActive() const;
			void setBeingSetup(bool setup);
			bool isBeingSetup() const;
			Chair* getChairAt(int chairNo);
			int getNumChairs() const;
			bool statusChange(bool sitting);
			void updatePlayersInGame(bool newStatus);
			int getNumOccupants();
			void addWatcher(ClientPlayer* p);
			void removeWatcher(ClientPlayer* p);
			void clearWatchers();
			ClientPlayer* getWatcherAt(int watcherNo);
			int getNumWatchers() const;
			bool isPlayerWatching(const std::string& player) const;
			int getPlayerPos(const std::string& player);
		};
	private:
		std::vector<TableFilterEnum> m_filters;
		std::vector<Table> m_tables;
		bool m_needsFullUpdate;
		int m_tableBegin;
		std::vector<int> m_tableFees;
		std::vector<int> m_minRatings;
		int m_highStakesVal;
	public:
		LobbyCache(void);
		void setFilters(const std::vector<TableFilterEnum>& filters);
		std::vector<TableFilterEnum>& getTableFilters();
		void createTables(int start, int quantity);
		std::vector<Table>& getTables();
		Table* getTableAt(int tableNo);
		void clear();
		void setNeedsFullUpdate(bool update);
		bool needsFullUpdate() const;
		int getTableBegin() const;
		int getNumTables() const;
		const std::vector<int>& getTableFees() const;
		void setTableFees(const std::vector<int>& fees);
		void setHighStakesValue(int val);
		int getHighStakesVal() const;
		void setMinRatingValues(const std::vector<int>& values);
		const std::vector<int>& getMinRatingValues() const;
		~LobbyCache(void);
	};
}

#endif
