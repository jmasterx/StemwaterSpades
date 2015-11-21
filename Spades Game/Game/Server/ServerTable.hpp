#ifndef CGE_SERVER_TABLE_HPP
#define CGE_SERVER_TABLE_HPP
#include "Game/Net/NetSpadesGame.hpp"
#include "Game/Net/ServerPlayer.hpp"
#include "Game/Server/ServerChair.hpp"
namespace cge
{
	typedef std::vector<ServerTable*> ServerTableArray;
	class ServerTable
	{
		NetSpadesGame m_game;
		SpadesGameInfo m_info;
		bool m_active;
		bool m_inProgress;
		bool m_paused;
		int m_tableNo;
		int64 m_gameId;
		bool m_beingSetup;
		std::vector<int> m_kickedPlayers;
		std::vector<std::pair<int,int> > m_disconnectedPlayers;
		std::vector<std::pair<int,int> > m_votes;
		std::vector<ServerChair> m_chairs;
		std::vector<std::pair<std::string,std::pair<bool,bool> > > m_waitList;
		std::vector<std::pair<std::string,std::pair<bool,bool> > > m_subList;
		float m_subTimeout;
		float m_subResponseTimeout;
		float m_requestTimeout;
	public:
		ServerTable(int tableNo);
		int getNumChairs() const;
		ServerChair* getChairAt(int i);
		void sitPlayer(int pos, ServerPlayer* p);
		void standPlayer(int pos);
		SpadesGameInfo& getInfo();
		NetSpadesGame& getGame();
		void setInfo(const SpadesGameInfo& info);
		void setActive(bool active);
		bool isActive() const;
		void setInProgress(bool inPrgress);
		bool isInProgress() const;
		void setPaused(bool paused);
		bool isPaused() const;
		int getTableNo() const;
		void setBeingSetup(bool beingSetup);
		bool isBeingSetup() const;
		int getNumOccupants() const;
		bool isEmpty() const;
		bool isReadyToPlay() const;
		void addWatcher(ServerPlayer* player);
		void removeWatcher(ServerPlayer* player);
		ServerPlayer* getWatcher(int watcher);
		int getNumWatchers() const;
		void clearWaitList();
		void clearSubWaitList();
		void verifyWaiter(const std::string& name, bool playAgain);
		void verifySubWaiter(const std::string& name, bool annulGame);
		void generateWaitList();
		void generateSubList();
		bool waitListDone() const;
		bool subWaitListDone() const;
		bool isWaitListActive() const;
		bool isSubListActive() const;
		bool allWantToPlayAgain() const;
		bool allWantToAnnulGame() const;
		bool anyWantToSubWait() const;
		void clearWatchers();
		void setGameId(int64 id);
		int64 getGameId() const;
		bool needsSub() const;
		void increaseSubTimeout(float amount);
		void resetSubTimeout();
		float getSubTimeout() const;
		void increaseSubResponseTimeout(float amount);
		void resetSubResponseTimeout();
		float getSubResponseTimeout() const;

		void increaseRequestTimeout(float amount);
		void resetRequestTimeout();
		float getRequestTimeout() const;
		void allowPlayerRequest();

		void clearKickedPlayers();
		void clearVotes();
		bool wasPlayerDisconnected(int pid) const;
		const std::vector<std::pair<int,int> >& getDisconnectedPlayers() const;
		void addDisconnectedPlayer(int pid, int seatNo);
		void clearDisconnectedPlayers();
		void removeDisconnectedPlayer(int pid);
		void castVoteAgainstPlayer(int pid, int castid);
		bool hasVoteBeenCastAgainstPlayer(int pid, int castid);
		bool isPlayerKicked(ServerPlayer* p) const;
		bool shouldPlayerBeKicked(int pid);
		int getDisconnectedPlayerSeat(int pid);
		int getPlayerIDFromName(const std::string& name);
		ServerPlayer* getPlayerFromName(const std::string& name);
		void kickPlayer(ServerPlayer* p);
		~ServerTable(void);
	};
}

#endif
