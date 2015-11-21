#ifndef CGE_SHARED_PLAYER_HPP
#define CGE_SHARED_PLAYER_HPP
#include "stdlib.h"
#include <string>
#include "Game/Net/SharedPlayerEnums.hpp"
namespace cge
{
	class SharedPlayer
	{
		std::string m_username;
		int m_tableID;
		int m_seatID;
		int m_watchTableID;
		int m_avatar;
	public:
		SharedPlayer(void);
		void setUsername(const std::string& name);
		const std::string& getUsername() const;
		void setTableId(int tableId);
		void setSeat(int seat);
		int getTableId() const;
		int getSeat() const;
		void setWatchTableId(int table);
		int getWatchTableId() const;
		void setAvatar(int avatar);
		int getAvatar() const;
		~SharedPlayer(void);
	};
}

#endif
