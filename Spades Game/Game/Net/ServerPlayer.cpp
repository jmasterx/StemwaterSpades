#include "Game/Net/ServerPlayer.hpp"
#define MAX_MSG_PER_INTERVAL 6
#define MSG_INTERVAL 7.0f
#define MSG_TIMEOUT_INTERVAL 180.0f
#define MAX_MSG_FOR_KICK 11
namespace cge
{
	ServerPlayer::ServerPlayer(void)
		: m_msgCount(0),m_timeSinceLastMessage(clock()),
		m_canChat(true), m_id(-1),m_acceptedTerms(false),
		m_theme(0),m_chatDisabled(false),m_system(-1)
	{
	}

	ServerPlayer::~ServerPlayer(void)
	{
	}

	void ServerPlayer::setUsername( const std::string& name )
	{
		m_sharedPlayer.setUsername(name);
	}

	const std::string& ServerPlayer::getUsername() const
	{
		return m_sharedPlayer.getUsername();
	}

	void ServerPlayer::setSeat( int seatNo )
	{
		m_sharedPlayer.setSeat(seatNo);
	}

	int ServerPlayer::getSeat() const
	{
		return m_sharedPlayer.getSeat();
	}

	void ServerPlayer::setTable( int table )
	{
		m_sharedPlayer.setTableId(table);
	}

	int ServerPlayer::getTable() const
	{
		return m_sharedPlayer.getTableId();
	}

	bool ServerPlayer::isSitting() const
	{
		return isAtTable() && getSeat() != -1;
	}

	void ServerPlayer::standUp()
	{
		setSeat(-1);
		setTable(-1);
	}

	bool ServerPlayer::isAtTable() const
	{
		return  getTable() != -1;
	}

	bool ServerPlayer::isLoggedIn() const
	{
		return getSessionID().length() > 0;
	}

	void ServerPlayer::setServerTable( ServerTable* table )
	{
		m_serverTable = table;
	}

	ServerTable* ServerPlayer::getServerTable()
	{
		return m_serverTable;
	}

	SharedPlayer* ServerPlayer::getSharedPlayer()
	{
		return &m_sharedPlayer;
	}

	void ServerPlayer::setWatchTable( int table )
	{
		m_sharedPlayer.setWatchTableId(table);
	}

	int ServerPlayer::getWatchTable() const
	{
		return m_sharedPlayer.getWatchTableId();
	}

	bool ServerPlayer::isWatching() const
	{
		return getWatchTable() !=  -1;
	}

	bool ServerPlayer::canChat() const
	{
		return m_canChat;
	}

	void ServerPlayer::setCanChat( bool can )
	{
		m_canChat = can;
	}

	double ServerPlayer::getTimeSinceLastMessage() const
	{
		clock_t dt = clock() - m_timeSinceLastMessage;

		return (float)dt / CLOCKS_PER_SEC;
	}

	void ServerPlayer::attemptSendChatMessage()
	{
		if(canChat())
		{
			m_msgCount++;

			if(getTimeSinceLastMessage() > MSG_INTERVAL)
			{
				if(m_msgCount > MAX_MSG_PER_INTERVAL)
				{
					m_timeSinceLastMessage = clock();
					setCanChat(false);
					m_msgCount = 1;
				}

				m_msgCount = 1;
				m_timeSinceLastMessage = clock();
			}
		}
		else
		{
			if(getTimeSinceLastMessage() > MSG_TIMEOUT_INTERVAL)
			{
				setCanChat(true);
			}

			m_timeSinceLastMessage = clock();
			m_msgCount++;
		}
	}

	bool ServerPlayer::chatAbuseDetected() const
	{
		return m_msgCount >= MAX_MSG_FOR_KICK;
	}

	void ServerPlayer::setId( int64 id )
	{
		m_id = id;
	}

	int ServerPlayer::getId() const
	{
		return m_id;
	}

	void ServerPlayer::setAcceptedTerms( bool accept )
	{
		m_acceptedTerms = accept;
	}

	bool ServerPlayer::hasAcceptedTerms() const
	{
		return m_acceptedTerms;
	}

	void ServerPlayer::setFriends( const std::set<std::string>& friends )
	{
		m_friends = friends;
	}

	bool ServerPlayer::isFriend( const std::string& name ) const
	{
		return m_friends.find(name) != m_friends.end();
	}

	std::set<std::string>& ServerPlayer::getFriends()
	{
		return m_friends;
	}

	const std::set<std::string>& ServerPlayer::getFriends() const
	{
		return m_friends;
	}

	void ServerPlayer::setAvatar( int avatar )
	{
		m_sharedPlayer.setAvatar(avatar);
	}

	int ServerPlayer::getAvatar() const
	{
		return m_sharedPlayer.getAvatar();
	}

	void ServerPlayer::setTheme( int theme )
	{
		m_theme = theme;
	}

	int ServerPlayer::getTheme() const
	{
		return m_theme;
	}

	bool ServerPlayer::isActive() const
	{
		return isLoggedIn() && m_active;
	}

	void ServerPlayer::setActive( bool active )
	{
		m_active = active;
	}

	void ServerPlayer::setChatDisabled( bool disabled )
	{
		m_chatDisabled = disabled;
	}

	bool ServerPlayer::isChatDisabled() const
	{
		return m_chatDisabled;
	}

	int ServerPlayer::getSystem() const
	{
		return m_system;
	}

	void ServerPlayer::setSystem( int system )
	{
		m_system = system;
	}

}
