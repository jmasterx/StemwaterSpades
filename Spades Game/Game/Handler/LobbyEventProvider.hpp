#ifndef CGE_LOBBY_EVENT_PROVIDER_HPP
#define CGE_LOBBY_EVENT_PROVIDER_HPP
#include "Game/Handler/LobbyEventListener.hpp"
#include "Game/Net/ClientPlayer.hpp"
#include <vector>
#define DISPATCH_LOBBY_EVENT	for(std::vector<LobbyEventListener*>::iterator it =  m_lobbyListeners.begin(); it != m_lobbyListeners.end(); ++it)
namespace cge
{
	class LobbyEventProvider : public LobbyEventListener
	{
	protected:
		std::vector<LobbyEventListener*> m_lobbyListeners;
	public:
		virtual ClientPlayer* getPlayerByName(const std::string& name);
		virtual bool isSelfPlayer(ClientPlayer* client);
		virtual void addLobbyListener(LobbyEventListener* listener);
		virtual void removeLobbyListener(LobbyEventListener* listener);
		virtual void clearLobbyListeners();
		bool isSelfPlayer(const std::string& name);
		bool isMuted(const std::string& name);
		bool isPlayerInGame(const std::string& playerName);
		bool isSelfInGame();
		bool isFriend(const std::string& name);
		bool isEnemy(const std::string& name);
		int getPlayerTable(const std::string& playerName);
		virtual bool isPlayerGroupToggled(GroupTypeEnum group);
		LobbyTable* getLobbyTable(int tableId);
		agui::Color getPlayerColor(const std::string& playerName);
		LobbyEventProvider(void);
		virtual ~LobbyEventProvider(void);
	};
}

#endif
