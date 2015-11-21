#ifndef CGE_CORE_HPP
#define CGE_CORE_HPP
#include "Game/platform.hpp"
#include "Game/Engine/Display.hpp"
#include "Game/Engine/DeviceManager.hpp"
#include "Game/Engine/SceneManager.hpp"
#include "Game/CardView/CardImageManager.hpp"
#include "Game/Resource/Sprite.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Game/Engine/LanguageManager.hpp"
#include "Game/UI/TableManager.hpp"
#include "Game/UI/DynamicUIManager.hpp"
#include "Game/UI/GuiColorManager.hpp"
#include "Agui/Agui.hpp"
#include "Game/Utility/Exception.hpp"
#include "Game/Net/ClientCore.hpp"
#include "Game/Net/ClientShared.hpp"
#include <iostream>
#include "Game/Element/Background.hpp"
#include <stdexcept>

namespace cge
{
	class Core
	{
		void _initAllegro();
		void _deinitAllegro();
		void _initAgui();
		void applyDeviceSettings();
		void coreExc(const Exception& e);
		void guiExc(const agui::Exception& e);
		void runtimeExc(const std::runtime_error& e);
		void exc();
		void initSettingsPath();
		agui::Allegro5ImageLoader m_a5GuiImageLoader;
		agui::Allegro5FontLoader m_a5GuiFontLoader;
		Display* display;
		CardImageManager* m_cardImageManager;
		DeviceManager* settings;
		SceneManager* sceneManager;
		Sprite* appIcon;
		GuiFontManager* m_fontManager;
		LanguageManager* m_languageManager;
		GuiColorManager* m_colorManager;
		SpriteManager* m_spriteManager;
		ClientCore* m_client;
		ClientShared* m_clientShared;
		std::vector<std::string> m_args;
		std::string m_settingsPath;
		std::vector<Sprite*> m_mainBG;
		Background m_bgData;
		const std::string& getSettingsPath() const;
		void loadMainBG();
		DynamicUIManager* m_dynamicUIManager;
		ProportionsManager m_proportions;
	public:
		Core(int argc, char *argv[]);
        void renderLoading(int numDots);
		void run();
		virtual ~Core(void);
	};
}
#endif
