#ifndef CGE_CLIENT_SHARED_HPP
#define CGE_CLIENT_SHARED_HPP
#include "Game/SpadesGame/NewGameData.hpp"
#include "Game/Net/ClientPlayerManager.hpp"
#include "Game/Client/SettingsManager.hpp"
#include "Game/Net/LobbyCache.hpp"
#include "Game/Engine/SceneManagerMessage.hpp"
#include "Game/Engine/LanguageManager.hpp"
#include <queue>
#include "Game/platform.hpp"
namespace cge
{
	struct StoredMessageBox
	{
		std::string message;
		std::string title;
	};

	struct StoredRequest
	{
		std::string player;
		int tableID;
	};

	class ClientShared
	{
		NewGameData m_gameData;
		ClientPlayerManager m_playerManager;
		SettingsManager m_settings;
		LobbyCache m_lobbyCache;
		SceneManagerMessage* m_scene;
		std::string m_lastLoginErrorMsg;
		LanguageManager* m_lang;
		std::queue<StoredMessageBox> m_storedMessage;
		std::queue<StoredRequest> m_storedRequest;
		bool m_needToShowPayment;
		bool m_canAutologin;
		int64 m_coins;
		bool m_plannedDisconnection;
		std::string m_cachedUsername;
		std::string m_cachedPassword;
		std::string m_lobbyName;
	public:
		ClientShared();
		NewGameData* getNewGameData();
		ClientPlayerManager* getPlayerManager();
		SettingsManager* getSettingsManager();
		LobbyCache* getLobbyCache();
		SceneManagerMessage* getSceneMessenger();
		void _setSceneMessenger(SceneManagerMessage* m);
		void _setLastLoginError(const std::string& error);
		void _setLanguageManager(LanguageManager* lang);
		LanguageManager* getLanguageManager();
		const std::string& getLastLoginError() const;
		StoredMessageBox dequeueStoredMessage();
		bool hasQueuedMessageBoxes() const;
		void queueMessageBox(const std::string& title, const std::string& message);
		StoredRequest dequeueStoredRequest();
		bool hasQueuedRequests() const;
		void queueRequest(const std::string& player, int tableID);
		std::string getServerName();
		void setShowPayment(bool show);
		bool needToShowPayment() const;
		bool canAutologin() const;
		void setCanAutologin(bool can);
		void setCoinAmount(int64 amount);
		int64 getCoinAmount() const;
		void setPlannedDisconnection(bool planned);
		bool isDisconnectionPlanned() const;
		void setCachedUsername(const std::string& username);
		const std::string& getCachedUsername() const;
		void setCachedPassword(const std::string& password);
		const std::string& getCachedPassword() const;
		void prepareLobbyChange();
		void setLobbyName(const std::string& name);
		const std::string& getLobbyName() const;
		~ClientShared(void);
	};
}

#endif
