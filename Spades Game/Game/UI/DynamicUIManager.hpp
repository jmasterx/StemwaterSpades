#ifndef CGE_DYNAMIC_UI_MANAGER_HPP
#define CGE_DYNAMIC_UI_MANAGER_HPP
#include "Game/UI/TableManager.hpp"
#include "Game/Element/BackgroundManager.hpp"
#include "Game/Element/ChairManager.hpp"
#include "Game/Avatar/AvatarCache.hpp"
#include "Game/Element/ThemeManager.hpp"
#include "Game/Element/GiftManager.hpp"
namespace cge
{
	class DynamicUIManager
	{
		TableManager* m_tableManager;
		BackgroundManager* m_backgroundManager;
		ChairManager* m_chairManager;
		AvatarCache* m_avatarCache;
		ThemeManager* m_themeManager;
		GiftManager* m_giftManager;
		std::vector<Sprite*> m_mainBG;
	public:
		DynamicUIManager(bool highQuality);
		TableManager* getTableManager();
		BackgroundManager* getBackgroundManager();
		ChairManager* getChairManager();
		AvatarCache* getAvatarCache();
		ThemeManager* getThemeManager();
		GiftManager* getGiftManager();
		void setMainBG(const std::vector<Sprite*>& mainbg);
		const std::vector<Sprite*>& getMainBG() const;
		virtual ~DynamicUIManager(void);
	};
}

#endif
