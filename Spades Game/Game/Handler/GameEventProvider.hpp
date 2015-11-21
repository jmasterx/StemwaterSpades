#ifndef CGE_GAME_EVENT_PROVIDER_HPP
#define CGE_GAME_EVENT_PROVIDER_HPP
#include "Game/Handler/GameEventListener.hpp"
#include <stdlib.h>
#include <algorithm>
#include <vector>
namespace cge
{
	#define DISPATCH_GAME_EVENT	for(std::vector<GameEventListener*>::iterator it =  m_gameListeners.begin(); it != m_gameListeners.end(); ++it)
	class GameEventProvider : public GameEventListener
	{
	protected:
		std::vector<GameEventListener*> m_gameListeners;
	public:
		GameEventProvider(void);
		virtual void addGameListener( GameEventListener* listener );
		virtual void removeGameListener(GameEventListener* listener);
		virtual void clearGameListeners();
		bool isMutedG(const std::string& name);
		int getTableG();
		SpadesGameInfo getGameInfoG();
		bool isFriendG( const std::string& playerName);
		bool isEnemyG( const std::string& playerName);
		ClientPlayer* getPlayerByNameG( const std::string& name );
		bool isSelfPlayerG( ClientPlayer* client );
		bool isSelfPlayerG(const std::string& name);
		agui::Color getGamePlayerColor(const std::string& playerName);
		agui::Color getLobbyPlayerColor( const std::string& playerName );
		virtual ~GameEventProvider(void);
	};
}

#endif