#include "Game/Engine/Core.hpp"
#include "Game/Utility/Exception.hpp"
#include "Game/Utility/File.hpp"
#include "Game/Engine/ListPairLoader.hpp"
#include "Game/Avatar/ArcText.hpp"

#ifdef CGE_IPHONE
#include "Game/Platform/IAPHandler.h"
#endif
#ifdef CGE_OSX
#include "Game/Platform/OSXHandler.h"
#endif

namespace cge
{
	Core::Core(int argc, char *argv[])
		:display(NULL),
		m_cardImageManager(NULL),
		m_fontManager(NULL),m_clientShared(NULL)
	{
		bool forced3d = false;
		bool noshaders = false;
		try
		{
			for(int i = 0; i < argc; ++i)
			{
				m_args.push_back(argv[i]);
				if(m_args.back() == "-forced3d")
					forced3d = true;
				if(m_args.back() == "-noshaders")
					noshaders = true;

			}

			//initialize Allegro 5
			_initAllegro();

			Log::create(getSettingsPath(),"sw_spades.log",-1,false);
			Log::write("Core","Log created");

			//initialize Agui
			_initAgui();

#ifdef CGE_IPHONE
            IAPHandler::getInstance().init();
#endif
			//init net stuff
			m_client = new ClientCore();
			m_clientShared = new ClientShared();
			std::string settingsPath = getSettingsPath();
			settingsPath += "settings.conf";
			m_clientShared->getSettingsManager()->setPath(settingsPath);
			m_clientShared->getSettingsManager()->load();
			m_clientShared->getSettingsManager()->setUnpersistantValues();

#ifdef CGE_OSX
appIcon = new Sprite("res/appico512.png");
#else
appIcon = new Sprite("res/appico.png");
#endif
			//create the display
			Vec2 optimalSize = Display::getOptimalResoluton();
			bool fullscreen = m_clientShared->getSettingsManager()->getBoolSetting("display.fullscreen");
			display = new Display(optimalSize.getX(),optimalSize.getY(),fullscreen,forced3d,noshaders);
			Log::write("Core","Created display");
		
#ifdef CGE_OSX
#ifndef CGE_DEV
            OSXHandler::getInstance().checkForDuplicates();
#endif
#endif
			display->setIcon(appIcon);

			//init font manager
			m_fontManager = new GuiFontManager("res/font.ttf",6,65,17,"res/blackcherry.ttf",15,75,18);
			m_fontManager->loadTempFont(7.0f * (display->getResolution().getX() / 1650.0f));
		
			//init language manager
			m_languageManager = new LanguageManager("res/language/");
			m_languageManager->loadLanguage(m_clientShared->getSettingsManager()->getStringSetting("user.language"));
			Log::write("Core","Loaded language");

			bool lowQuality = false;
            lowQuality = display->isLowQuality();

			loadMainBG();
			m_proportions.resize(display->getResolution().getX(),display->getResolution().getY());
			m_bgData.setProportions(&m_proportions);
			m_bgData.setBackground(0.0f,0.0f,1.0f,m_mainBG);

			renderLoading(0);

			m_fontManager->_loadFonts();
			agui::Widget::setGlobalFont(m_fontManager->getDefaultFont());
			Log::write("Core","Loaded fonts");

			//init gui color manager
			m_colorManager = new GuiColorManager("res/color/default.col");
			Log::write("Core","Loaded colors");
			renderLoading(1);
			m_dynamicUIManager = new DynamicUIManager(!lowQuality);
			m_dynamicUIManager->setMainBG(m_mainBG);
			Log::write("Core","Loaded dynamic UI");

			//create card images
			m_cardImageManager = new CardImageManager("res/cards/","256","default");
			Log::write("Core","Loaded card images");
			renderLoading(1);

			//create sprite manager
			m_spriteManager = new SpriteManager("res/sprites/sprites.spt","res/sprites/");
			Log::write("Core","Loaded Sprite Manager");

			renderLoading(2);
			
			//create the settings
			settings = new DeviceManager(display,m_cardImageManager,
				m_fontManager,m_languageManager,m_colorManager, m_spriteManager,m_client,
				m_clientShared);
			settings->setMasterIPAndPort();
			Log::write("Core","Loaded Device Manager");

			ListPairLoader::_initStrings("res/emoticons/emoticons.emo","res/emoticons/");

			//create the scene manager
			sceneManager = new SceneManager(settings, m_dynamicUIManager);
			display->setSceneMessenger(sceneManager);
			renderLoading(3);
			display->doneLoading();
			applyDeviceSettings();
			srand(time(NULL));
			Log::write("Core","Created");
		}
		catch(const Exception& e)
		{
			coreExc(e);
		}
		catch (const agui::Exception& e)
		{
			guiExc(e);
		}
		catch(std::runtime_error& e)
		{
			runtimeExc(e);
		}
		catch(...)
		{
			std::cout << "Unknown Error" << std::endl;
			exc();
		}
	}

	Core::~Core(void)
	{
		Log::write("Core","Halting...");
		delete sceneManager;
		delete settings;
		delete m_cardImageManager;
		delete m_languageManager;
		delete m_colorManager;
		delete m_dynamicUIManager;
		delete m_fontManager;
		delete m_spriteManager;
		delete appIcon;
		delete display;
		delete m_clientShared;
		delete m_client;
		for(size_t i = 0; i < m_mainBG.size(); i++)
			delete m_mainBG[i];
		m_mainBG.clear();
		ListPairLoader::clear();
		_deinitAllegro();
		Log::write("Core","Halted...");
	}

	void Core::_initAllegro()
	{
		//Init Allegro
		if(!al_init())
		{
			throw Exception("Allegro failed to initialize");
		}

		if(!al_init_image_addon())
		{
			throw Exception("Allegro failed to initialize Image Addon");
		}

		if(!al_init_primitives_addon())
		{
			throw Exception("Allegro failed to initialize Primitives Addon");
		}

		al_init_font_addon();

		if(!al_init_ttf_addon())
		{
			throw Exception("Allegro failed to initialize TTF Addon");
		}

		if(!al_init_acodec_addon())
		{
			throw Exception("Allegro failed to initialize Acodec Addon");
		}

		if(!al_install_audio())
		{
			throw Exception("Allegro failed to initialize Audio Addon");
		}

#ifdef CGE_PC
		if(!al_init_native_dialog_addon())
		{
			throw Exception("Allegro failed to initialize Dialog Addon");
		}
#endif

		if(!al_install_mouse())
		{
			throw Exception("Allegro failed to initialize the mouse");
		}

		if(!al_install_keyboard())
		{
			throw Exception("Allegro failed to initialize the keyboard");
		}

		if (!PHYSFS_init(m_args[0].c_str()))
		{
			throw Exception("Allegro PHYSFS failed to initialize");
		}

		if (!PHYSFS_addToSearchPath("resource.swp", 1))
		{
			throw Exception("Allegro PHYSFS failed to open resource file");
		}

		 initSettingsPath();
		 al_set_physfs_file_interface();

#ifdef CGE_MOBILE
		 al_install_touch_input();
         //al_install_joystick();
		 al_set_mouse_emulation_mode(ALLEGRO_MOUSE_EMULATION_INCLUSIVE);
#endif

	}

	void Core::_initAgui()
	{
		Log::write("Core","Starting GUI");
		//Set the image loader
		agui::Image::setImageLoader(&m_a5GuiImageLoader);

		//Set the font loader
		agui::Font::setFontLoader(&m_a5GuiFontLoader);

		//Allegro does not automatically premultiply alpha so let Agui do it
		agui::Color::setPremultiplyAlpha(true);
		Log::write("Core","GUI started");
	}

	void Core::run()
	{
		Log::write("Core","Entering main loop");
        sceneManager->setCore(this);
		sceneManager->run();
		Log::write("Core","Exited main loop");
	}

	void Core::_deinitAllegro()
	{
		Log::write("Core","Deiniting backend");
#ifdef CGE_MOBILE
		al_uninstall_touch_input();
    //    al_uninstall_joystick();
#endif
	    al_uninstall_keyboard();
		al_uninstall_mouse();
		al_uninstall_audio();
		al_uninstall_system();
		Log::write("Core","Deinited backend");
	}

	void Core::coreExc( const Exception& e )
	{
		std::string msg = e.getMessage();
		msg += "\n\nA common problem is a missing video driver.\nPlease visit one of the following based on your video card vendor:\n\nNvidia: http://www.nvidia.com/drivers\nAMD/ATi: http://www.amd.com/drivers\nIntel: http://downloadcenter.intel.com";
		Log::write("Core","Exception: " + e.getMessage());
#ifndef CGE_MOBILE
			al_show_native_message_box(NULL,"Problem starting the game",
				"There was a problem starting the game. \nHere is what we know:",
				msg.c_str(),NULL,0);
#endif
		exc();
	}

	void Core::guiExc( const agui::Exception& e )
	{
		Log::write("GUI","Exception: " + e.getMessage());
		exc();
	}

	void Core::runtimeExc( const std::runtime_error& e )
	{
		Log::write("Runtime",std::string("Exception: ") + e.what());
		exc();
	}

	void Core::exc()
	{
		throw Exception("HALT");

	}

	void Core::applyDeviceSettings()
	{
		bool enAudio = m_clientShared->getSettingsManager()->getBoolSetting("audio.enabled");
		settings->getAudioManager()->setAudioEnabled(enAudio);
	}

	void Core::initSettingsPath()
	{
		al_set_org_name("Stemwater Games");
		al_set_app_name("Stemwater Spades");
		ALLEGRO_PATH* path = al_get_standard_path(ALLEGRO_USER_DATA_PATH);
		std::string pathStr;
		pathStr += al_path_cstr(path,ALLEGRO_NATIVE_PATH_SEP);
		bool made = al_make_directory(pathStr.c_str());
		m_settingsPath = pathStr;
		al_destroy_path(path);
	}

	const std::string& Core::getSettingsPath() const
	{
		return m_settingsPath;
	}

	void Core::renderLoading(int numDots)
	{
		GraphicsContext g;
		g.clear(Color(0.0f,0.0f,0.0f));

		{
			m_bgData.render(&g);
		}

		ArcText arc;

		agui::Font* f = m_fontManager->getTempFont();

		std::string loading = m_languageManager->getElement("label.loading");

		for(int i = 0; i < numDots; i++)
		loading += ".";

		arc.setText(loading);
		arc.setFont(f);
		arc.setStrength(2.0f * f->getLineHeight());
		arc.setFontColor(agui::Color(255,255,255));
		arc.setShadowStrength(150);

		int x = (int)((display->getResolution().getX() / 2.0f) + 0.5f);
		int y = (int)((display->getResolution().getY() / 2.0f) + 0.5f);

		arc.setPos(x,y);
		arc.setDirection(2);
		arc.render(&g);
		al_flip_display();
	}

	void Core::loadMainBG()
	{
		m_mainBG.clear();
		std::string a = "res/sprites/mainbg/mainbg_";
		
		for(int i = 1; i <= 12; i++)
		{
			std::stringstream ss;
			ss << a;
			if(i < 10)
				ss << "0";
			ss << i;
			ss << ".png";
			m_mainBG.push_back(new Sprite(ss.str()));
		}
	}

}
