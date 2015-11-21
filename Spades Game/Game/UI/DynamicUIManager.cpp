#include "Game/UI/DynamicUIManager.hpp"

namespace cge
{
	DynamicUIManager::~DynamicUIManager(void)
	{
		delete m_tableManager;
		delete m_backgroundManager;
		delete m_chairManager;
		delete m_avatarCache;
		delete m_themeManager;
		delete m_giftManager;
	}

	TableManager* DynamicUIManager::getTableManager()
	{
		return m_tableManager;
	}

	DynamicUIManager::DynamicUIManager( bool highQuality )
	{
		std::string avatarLobbySize;
		std::string avatarGameSize;

		if(highQuality)
		{
			avatarLobbySize = "256/";
			avatarGameSize = "512/";
		}
		else
		{
			avatarLobbySize = "128/";
			avatarGameSize = "256/";
		}

		m_tableManager = new TableManager("res/table/1024/","res/table/tables.tbl");

		m_backgroundManager = new BackgroundManager("res/background/1024/","res/background/backgrounds.bks");

		m_chairManager = new ChairManager("res/chair/512/","res/chair/chairs.chs");

		m_themeManager = new ThemeManager("res/theme/256/","res/theme/themes.thm");
		m_themeManager->loadThumbs();

		//create avatar cache
		m_avatarCache = new AvatarCache("res/avatar/",avatarGameSize,avatarLobbySize,"avatars.avt");

		m_giftManager = new GiftManager("res/gift/","128/","512/","res/sound/gift/","res/gift/gifts.gft");
		m_giftManager->loadThumbs();
	}
	BackgroundManager* DynamicUIManager::getBackgroundManager()
	{
		return m_backgroundManager;
	}

	ChairManager* DynamicUIManager::getChairManager()
	{
		return m_chairManager;
	}

	AvatarCache* DynamicUIManager::getAvatarCache()
	{
		return m_avatarCache;
	}

	ThemeManager* DynamicUIManager::getThemeManager()
	{
		return m_themeManager;
	}

	GiftManager* DynamicUIManager::getGiftManager()
	{
		return m_giftManager;
	}

	void DynamicUIManager::setMainBG( const std::vector<Sprite*>& data)
	{
		m_mainBG = data;
	}

	const std::vector<Sprite*>& DynamicUIManager::getMainBG() const
	{
		return m_mainBG;
	}

}
