#ifndef CGE_SERVER_PLAYER_HPP
#define CGE_SERVER_PLAYER_HPP
#include "Game/serverplatform.hpp"
#include "Game/Net/NetPacketParser.hpp"
#include "Game/Net/NetEventEncoder.hpp"
#include "Game/Net/ServerInstance.hpp"
#include <set>
#include <vector>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

namespace cge
{
	class ServerTable;
	class ServerPlayer : public ServerInstance
	{
		SharedPlayer m_sharedPlayer;
		ServerTable* m_serverTable;
		bool m_acceptedTerms;
		int m_msgCount;
		bool m_canChat;
		clock_t m_timeSinceLastMessage;
		int64 m_id;
		int m_theme;
		bool m_active;
		bool m_chatDisabled;
		std::set<std::string> m_friends;
		int m_system;
	public:
		ServerPlayer(void);
		void setUsername(const std::string& name);
		const std::string& getUsername() const;
		void setTable(int tableNo);
		int getTable() const;
		void setSeat(int seatNo);
		int getSeat() const;
		bool isSitting() const;
		void standUp();
		bool isAtTable() const;
		bool isLoggedIn() const;
		void setWatchTable(int table);
		int getWatchTable() const;
		bool isWatching() const;
		void setServerTable(ServerTable* table);
		ServerTable* getServerTable();
		SharedPlayer* getSharedPlayer();
		void setId(int64 id);
		int getId() const;
		bool canChat() const;
		void setCanChat(bool can);
		double getTimeSinceLastMessage() const;
		void attemptSendChatMessage();
		bool chatAbuseDetected() const;
		void setAcceptedTerms(bool accept);
		bool hasAcceptedTerms() const;
		void setFriends(const std::set<std::string>& friends);
		bool isFriend(const std::string& name) const;
		std::set<std::string>& getFriends();
		const std::set<std::string>& getFriends() const;
		void setAvatar(int avatar);
		int getAvatar() const;
		void setTheme(int theme);
		int getTheme() const;
		void setActive(bool active);
		bool isActive() const;
		bool isChatDisabled() const;
		void setChatDisabled(bool disabled);
		int getSystem() const;
		void setSystem(int system);
		~ServerPlayer(void);
	};

	typedef std::set<ServerPlayer*> ServerPlayerSetArray;
	typedef std::vector<ServerPlayer*> ServerPlayerVecArray;
}
#endif
