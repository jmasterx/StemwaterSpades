#ifndef CGE_SERVER_CHAIR_HPP
#define CGE_SERVER_CHAIR_HPP
#include "Game/Net/ServerPlayer.hpp"
namespace cge
{
	struct ServerGiftData
	{
		int giftID;
		int frame;
		int fromPlayer;

		ServerGiftData()
			: giftID(-1),
			frame(-1),
			fromPlayer(-1)
		{

		}

		void clear()
		{
			giftID = -1;
			fromPlayer = -1;
			frame = -1;
		}

	};
	class ServerChair
	{
		ServerPlayer* m_player;
		int64 m_originalPid;
		int64 m_lastPid;
		bool m_subSeat;
		ServerGiftData m_giftDat;
		float m_botGiftTimeout;
		float m_botGiftInterval;
		double m_turnTime;
		bool m_isTurn;
		int m_numTimeouts;
		bool m_sentFirstStrike;
	public:
		ServerChair(void);
		void setPlayer(ServerPlayer* player);
		void setAsSubSeat(bool subseat);
		bool isSubSeat() const;
		bool isOccupied() const;
		void setOriginalPid(int64 id);
		int64 getOriginalPid() const;
		void setLastPid(int64 pid);
		int64 getLastPid() const;
		ServerPlayer* getPlayer();
		void clearGiftData();
		const ServerGiftData& getGiftData() const;
		void setGiftData(int fromPlayer, int giftID);
		void updateGiftData(int newFrame);
		bool isGiftDataSet() const;
		bool updateBotGift(float t);
		void turnStarted();
		void turnEnded();
		void resetTurnData();
		void increaseTurnTime(double t);
		void resetTurnTime();
		double getTurnTime() const;
		void addTurnStrike();
		int getTurnStrikes() const;
		bool isTurn() const;
		bool sentFirstStrike() const;
		void sendFirstStrike();
		~ServerChair(void);
	};
}

#endif
