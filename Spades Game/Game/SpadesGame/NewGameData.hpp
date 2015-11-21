#ifndef CGE_NEW_GAME_DATA_HPP
#define CGE_NEW_GAME_DATA_HPP
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Game/SpadesGame/GameClientState.hpp"
namespace cge
{
	class NewGameData
	{
		SpadesGameInfo m_gameInfo;
		bool m_online;
		bool m_restoring;
		GameClientState m_state;
	public:
		NewGameData(void);
		void setGameInfo(const SpadesGameInfo& info);
		SpadesGameInfo& getGameInfo();
		void setOnline(bool online);
		bool isOnline() const;
		void setRestoring(bool restoring);
		bool isRestoring() const;
		const GameClientState& getRestoreState() const;
		void setRestoreState(const GameClientState& state);
		virtual ~NewGameData(void);
	};
}

#endif
