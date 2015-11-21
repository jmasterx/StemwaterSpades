#ifndef CGE_CLIENT_PLAYER_MANAGER_HPP
#define CGE_CLIENT_PLAYER_MANAGER_HPP
#include "Game/Net/ClientPlayer.hpp"
#include "Game/Client/LobbyEventEnums.hpp"
#include "Agui/Color.hpp"
#include <vector>
#include <map>
#include <deque>
namespace cge
{
	enum TableMessageColorEnum
	{
		TMC_UNDETERMINED,
		TMC_WATCHER,
		TMC_RED_TEAM,
		TMC_BLUE_TEAM,
		TMC_GREEN_TEAM,
		TMC_YELLOW_TEAM,
		TMC_FRIEND,
		TMC_ENEMY,
		TMC_NORMAL_LOBBY,
		TMC_OFFLINE,
		TMC_GAME
	};
	struct TableMessage
	{
		std::string m_username;
		std::string m_message;
		TableMessageColorEnum m_lobbyColorType;
		TableMessageColorEnum m_gameColorType;
		agui::Color m_lobbyColor;
		agui::Color m_gameColor;

		TableMessage()
			: m_lobbyColorType(TMC_UNDETERMINED), m_gameColorType(TMC_UNDETERMINED)
		{

		}
	};


	class ClientPlayerManager
	{
		std::vector<ClientPlayer*> m_players;
		ClientPlayer m_self;
		std::map<std::string,bool> m_mutes;
		std::deque<TableMessage> m_tableQ;
		bool _containsMute(const std::string& name);
		int m_maxTableMessages;

		std::vector<std::string> m_friends;
		std::vector<std::string> m_enemies;
	public:
		void clear();
		bool playerExists(const std::string & userName) const;
		ClientPlayer* createPlayer(const std::string& userName);
		bool destroyPlayer(const std::string& userName);
		bool destroyPlayer(ClientPlayer* player);
		//can return NULL, returns self on ""
		ClientPlayer* getPlayerByName(const std::string& userName);
		int getNumPlayers() const;
		ClientPlayer* getPlayerAt(int i);
		ClientPlayer* getSelfPlayer();
		bool isMuted(const std::string& player);
		void setMuted(const std::string& player, bool mute);
		std::deque<TableMessage>& getTableMessageQueue();
		void queueTableMessage(const TableMessage &t);
		void clearTableMessages();
		TableMessage* getLastTableMessage();
		bool isFriend(const std::string& name) const;
		bool isEnemy(const std::string& name) const;
		void addFriend(const std::string& name);
		void addEnemy(const std::string& name);
		void removeFriend(const std::string& name);
		void removeEnemy(const std::string& name);
		GroupTypeEnum getPlayerGroup(ClientPlayer* player) const;
		void standPlayerUp(int tableNo, int seatNo);
		std::string getPlayerByTable(int tableNo, int seatNo);
		ClientPlayerManager(void);
		~ClientPlayerManager(void);
	};
}

#endif
