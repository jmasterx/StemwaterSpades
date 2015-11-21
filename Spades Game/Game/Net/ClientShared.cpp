#include "Game/Net/ClientShared.hpp"
namespace cge
{
	ClientShared::~ClientShared(void)
	{
	}

	NewGameData* ClientShared::getNewGameData()
	{
		return &m_gameData;
	}

	ClientPlayerManager* ClientShared::getPlayerManager()
	{
		return &m_playerManager;
	}

	SettingsManager* ClientShared::getSettingsManager()
	{
		return &m_settings;
	}

	LobbyCache* ClientShared::getLobbyCache()
	{
		return &m_lobbyCache;
	}

	SceneManagerMessage* ClientShared::getSceneMessenger()
	{
		return m_scene;
	}

	ClientShared::ClientShared()
		: m_scene(NULL),m_lang(NULL),m_needToShowPayment(false),
		m_canAutologin(true),m_coins(0),m_plannedDisconnection(false)
	{
		
	}

	void ClientShared::_setSceneMessenger( SceneManagerMessage* m )
	{
		m_scene = m;
	}

	void ClientShared::_setLastLoginError( const std::string& error )
	{
		m_lastLoginErrorMsg = error;
	}

	const std::string& ClientShared::getLastLoginError() const
	{
		return m_lastLoginErrorMsg;
	}

	void ClientShared::_setLanguageManager( LanguageManager* lang )
	{
		m_lang = lang;
	}

	LanguageManager* ClientShared::getLanguageManager()
	{
		return m_lang;
	}

	cge::StoredMessageBox ClientShared::dequeueStoredMessage()
	{
		StoredMessageBox s;

		if(!m_storedMessage.empty())
		{
			s = m_storedMessage.front();
			m_storedMessage.pop();
		}
		return s;
	}

	bool ClientShared::hasQueuedMessageBoxes() const
	{
		return !m_storedMessage.empty();
	}

	void ClientShared::queueMessageBox( const std::string& title, const std::string& message )
	{
		StoredMessageBox s;
		s.title = title;
		s.message = message;
		m_storedMessage.push(s);
	}

	std::string ClientShared::getServerName()
	{
		return m_lang->getElement("server");
	}

	void ClientShared::setShowPayment( bool show )
	{
		m_needToShowPayment = show;
	}

	bool ClientShared::needToShowPayment() const
	{
		return m_needToShowPayment;
	}

	bool ClientShared::canAutologin() const
	{
		return m_canAutologin;
	}

	void ClientShared::setCanAutologin( bool can )
	{
		m_canAutologin = can;
	}

	void ClientShared::setCoinAmount( int64 amount )
	{
		m_coins = amount;
	}

	int64 ClientShared::getCoinAmount() const
	{
		return m_coins;
	}

	void ClientShared::setPlannedDisconnection( bool planned )
	{
		m_plannedDisconnection = planned;
	}

	bool ClientShared::isDisconnectionPlanned() const
	{
		return m_plannedDisconnection;
	}

	void ClientShared::setCachedUsername( const std::string& username )
	{
		m_cachedUsername = username;
	}

	const std::string& ClientShared::getCachedUsername() const
	{
		return m_cachedUsername;
	}

	void ClientShared::setCachedPassword( const std::string& password )
	{
		m_cachedPassword = password;
	}

	const std::string& ClientShared::getCachedPassword() const
	{
		return m_cachedPassword;
	}

	void ClientShared::prepareLobbyChange()
	{
		getLobbyCache()->clear();
		getLobbyCache()->setNeedsFullUpdate(false);
		getPlayerManager()->clear();
	}

	void ClientShared::setLobbyName( const std::string& name )
	{
		m_lobbyName = name;
	}

	const std::string& ClientShared::getLobbyName() const
	{
		return m_lobbyName;
	}

	void ClientShared::queueRequest( const std::string& player, int tableID )
	{
		StoredRequest r;
		r.tableID = tableID;
		r.player = player;
		m_storedRequest.push(r);
	}

	bool ClientShared::hasQueuedRequests() const
	{
		return !m_storedRequest.empty();
	}

	cge::StoredRequest ClientShared::dequeueStoredRequest()
	{
		StoredRequest r;

		if(hasQueuedRequests())
		{
			r = m_storedRequest.front();
			m_storedRequest.pop();
		}

		return r;
	}

}
