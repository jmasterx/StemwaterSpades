#ifndef CGE_RAW_GAME_LISTENER_HPP
#define CGE_RAW_GAME_LISTENER_HPP
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Game/SpadesGame/SpadesGame.hpp"
namespace cge
{
	class RawGameListener
	{
	public:
		virtual void newGame(const SpadesGameInfo& info) {}
		virtual void bidsReceived(const std::vector<SpadesGame::Team>& teams,
			const std::vector<SpadesGame::Player>& players) {}
		virtual void roundBegan(const std::vector<SpadesGame::Team>& teams,
			const std::vector<SpadesGame::Player>& players) {}
		virtual void roundEnded(const std::vector<SpadesGame::Team>& teams,
			const std::vector<SpadesGame::Player>& players) {}
		virtual void playerMadeTrick(const std::vector<SpadesGame::Team>& teams,
			const std::vector<SpadesGame::Player>& players, int trickWinner) {}
		virtual void gameIsOver(const std::vector<SpadesGame::Team>& teams,
			const std::vector<SpadesGame::Player>& players,
			const std::vector<int>& winningTeams, const std::vector<int>& winningScores) {}
		RawGameListener(void);
		virtual ~RawGameListener(void);
	};
}
#endif
