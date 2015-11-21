#ifndef CGE_DEVICE_MANAGER_HPP
#define CGE_DEVICE_MANAGER_HPP
#include "Game/Engine/Display.hpp"
#include "Game/CardView/CardImageManager.hpp"
#include "Agui/Agui.hpp"
#include "Agui/Backends/Allegro5/Allegro5.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Game/Engine/LanguageManager.hpp"
#include "Game/UI/GuiColorManager.hpp"
#include "Game/Resource/SpriteManager.hpp"
#include "Game/Net/ClientCore.hpp"
#include "Game/Net/NetEventDecoder.hpp"
#include "Game/Net/NetEventEncoder.hpp"
#include "Game/Net/LobbyNetHandler.hpp"
#include "Game/Net/LobbyNetHandler.hpp"
#include "Game/Net/GameNetHandler.hpp"
#include "Game/Engine/Censor.hpp"
#include "Game/Engine/AudioManager.hpp"
#include "Game/Net/ClientShared.hpp"
#include "Game/Handler/SceneEventHandler.hpp"
namespace cge
{
	class DeviceManager
	{
		Display* m_display;
		CardImageManager* m_cardImageMan;
		GuiFontManager* m_fontManager;
		LanguageManager* m_languageManager;
		GuiColorManager* m_colorManager;
		SpriteManager* m_spriteManager;
		ClientCore* m_client;
		ClientShared* m_clientShared;

		NetEventEncoder m_enc;
		NetEventDecoder m_dec;

		LobbyNetHandler m_lobbyHandler;
		GameNetHandler m_gameHandler;
		SceneEventHandler *m_sceneHandler;

		Censor m_censor;

		AudioManager* m_audio;
	public:
		Display* getDisplay() const;
		CardImageManager* getCardImageManager() const;
		GuiFontManager* getFontManager() const;
		LanguageManager* getLanguageManager() const;
		GuiColorManager* getColorManager() const;
		SpriteManager* getSpriteManager() const;
		ClientCore* getNetClient() const;
		NetEventDecoder* getNetEventDecoder();
		NetEventEncoder* getNetEventEncoder();
		LobbyNetHandler* getLobbyNetHandler();
		GameNetHandler* getGameNetHandler();
		SceneEventHandler* getSceneHandler();
		ClientShared* getShared();
		Censor* getCensor();
		AudioManager* getAudioManager();
		void setSceneMessenger(SceneManagerMessage* msg);
		void setMasterIPAndPort();

		DeviceManager(Display* display, CardImageManager* cardImageManager,
			GuiFontManager* fontManager, LanguageManager* languageManager,
			GuiColorManager* colorManager, SpriteManager* spriteManager,
			ClientCore* client, ClientShared* clientShared);
		virtual ~DeviceManager(void);
	};
}

#endif