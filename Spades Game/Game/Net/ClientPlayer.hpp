#ifndef CGE_CLIENT_PLAYER_HPP
#define CGE_CLIENT_PLAYER_HPP
#include <stdlib.h>
#include <string>
#include <vector>
namespace cge
{
	class ClientPlayer
	{
		std::vector<std::string> m_friends;
		std::vector<std::string> m_enemies;
		std::string m_username;

		int m_tableNo;
		int m_seatNo;
		int m_watchTableNo;
		int m_avatar;
		bool m_host;
		bool m_inGame;
		bool m_bot;
	public:
		ClientPlayer(void);
		void setUserName(const std::string& userName);
		const std::string& getUsername() const;
		void setTable(int table);
		int getTable() const;
		void setSeat(int seat);
		int getSeat() const;
		bool isSitting() const;
		bool isAtTable() const;
		void setHosting(bool hosting);
		void standUp();
		bool isHosting() const;
		void setInGame(bool inGame);
		bool isInGame() const;
		void setWatchTableNo(int tableNo);
		int getWatchTableNo() const;
		bool isWatching() const;
		void setAvatar(int avatar);
		int getAvatar() const;
		bool isBot() const;
		void setIsBot(bool bot);

		~ClientPlayer(void);
	};
}

#endif
