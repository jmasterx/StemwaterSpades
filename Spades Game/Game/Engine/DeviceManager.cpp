#include "Game/Engine/DeviceManager.hpp"
#ifdef CGE_IPHONE
#include "Game/Platform/IAPHandler.h"
#endif
namespace cge
{
	DeviceManager::~DeviceManager(void)
	{
		m_client->setDecoder(NULL);
		delete m_audio;
		delete m_sceneHandler;
	}

	Display* DeviceManager::getDisplay() const
	{
		return m_display;
	}

	CardImageManager* DeviceManager::getCardImageManager() const
	{
		return m_cardImageMan;
	}

	GuiFontManager* DeviceManager::getFontManager() const
	{
		return m_fontManager;
	}

	LanguageManager* DeviceManager::getLanguageManager() const
	{
		return m_languageManager;
	}


	DeviceManager::DeviceManager( 
		Display* display, 
		CardImageManager* cardImageManager,
		GuiFontManager* fontManager,
		LanguageManager* languageManager,
		GuiColorManager* colorManager, 
		SpriteManager* spriteManager,
		ClientCore* client,
		ClientShared* clientShared )
		: m_display(display), m_cardImageMan(cardImageManager), m_fontManager(fontManager),
		m_languageManager(languageManager), m_colorManager(colorManager),
		m_spriteManager(spriteManager),m_client(client),m_lobbyHandler(&m_enc,clientShared),
		m_gameHandler(&m_enc,clientShared), m_censor("res/language/censor.fku"),m_audio(NULL),
		m_sceneHandler(NULL),m_clientShared(NULL)
	{
		m_clientShared = clientShared;
		m_client->setDecoder(&m_dec);
		m_enc.setServerClient(m_client);
		m_dec.addListener(&m_lobbyHandler);
		m_dec.addListener(&m_gameHandler);

		m_audio = new AudioManager("res/sound/sound_mapping.conf","res/sound/",clientShared->getSettingsManager());
		m_sceneHandler = new SceneEventHandler(&m_enc,this);
		m_dec.addListener(m_sceneHandler);
		clientShared->_setLanguageManager(languageManager);
        
#ifdef CGE_IPHONE
        IAPHandler::getInstance().setLobbyNetHandler(&m_lobbyHandler);
#endif
	}

	GuiColorManager* DeviceManager::getColorManager() const
	{
		return m_colorManager;
	}

	SpriteManager* DeviceManager::getSpriteManager() const
	{
		return m_spriteManager;
	}

	ClientCore* DeviceManager::getNetClient() const
	{
		return m_client;
	}

	NetEventDecoder* DeviceManager::getNetEventDecoder()
	{
		return &m_dec;
	}

	NetEventEncoder* DeviceManager::getNetEventEncoder()
	{
		return &m_enc;
	}

	LobbyNetHandler* DeviceManager::getLobbyNetHandler()
	{
		return &m_lobbyHandler;
	}

	ClientShared* DeviceManager::getShared()
	{
		return m_clientShared;
	}

	GameNetHandler* DeviceManager::getGameNetHandler()
	{
		return &m_gameHandler;
	}

	Censor* DeviceManager::getCensor()
	{
		return &m_censor;
	}

	AudioManager* DeviceManager::getAudioManager()
	{
		return m_audio;
	}

	SceneEventHandler* DeviceManager::getSceneHandler()
	{
		return m_sceneHandler;
	}

	void DeviceManager::setSceneMessenger( SceneManagerMessage* msg )
	{
		m_clientShared->_setSceneMessenger(msg);
	}

	void DeviceManager::setMasterIPAndPort()
	{
		m_client->setAddressAndPort(CGE_SERVER_ADDRESS,CGE_SERVER_PORT);
	}

}
