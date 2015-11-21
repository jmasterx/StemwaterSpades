#include "Game/Engine/Display.hpp"
#include "Game/Utility/Exception.hpp"
#include "Game/Engine/SceneManager.hpp"
#include "Game/Utility/StringUtil.hpp"
#include "Game/Platform/Platf.hpp"

#ifdef CGE_WINDOWS
#include <allegro5/allegro_windows.h>
#endif

#ifdef CGE_OSX
#include "Game/platform/OSXHandler.h"
#endif

namespace cge
{
	Display::~Display(void)
	{
		al_destroy_display(context);
		context = NULL;
	}

	Display::Display( int width, int height, bool fullscreen, bool forceD3D, bool disableShaders )
	: context(NULL), minWidth(800),minHeight(585),
	m_sceneMessenger(NULL),m_caption("Stemwater Spades")
	{
		bool usingGL = true;
#ifdef CGE_WINDOWS
		if(forceD3D)
			usingGL = false;
#endif
#ifndef CGE_IPHONE
		int flags = ALLEGRO_GENERATE_EXPOSE_EVENTS;
		if(!disableShaders)
			flags |= ALLEGRO_PROGRAMMABLE_PIPELINE;
		if(usingGL)
			flags |=  ALLEGRO_OPENGL;
		if(fullscreen)
			flags |= ALLEGRO_FULLSCREEN_WINDOW;
		al_set_new_display_flags(flags);
		al_set_new_bitmap_flags(
			ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | 
			ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP | ALLEGRO_CONVERT_BITMAP);
#else
		al_set_new_display_option(ALLEGRO_SUPPORTED_ORIENTATIONS,ALLEGRO_DISPLAY_ORIENTATION_LANDSCAPE, ALLEGRO_REQUIRE);
        int flags = ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE;
        if(fullscreen)
            flags |= ALLEGRO_FULLSCREEN_WINDOW;
        al_set_new_display_flags(flags);
        al_set_new_bitmap_flags(
                                ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR |
                                ALLEGRO_MAG_LINEAR);
#endif
		 Log::write("Display","Flags set");

		 if(isAppAlreadyRunning())
		 {
			exit(0);
		 }
		context = al_create_display(width,height);
		bool contextFail = false;
		if(!context)
		{
#ifdef CGE_WINDOWS
			if(!forceD3D)
			{
				int flags = ALLEGRO_GENERATE_EXPOSE_EVENTS;
				usingGL = false;
				if(!disableShaders)
					flags |= ALLEGRO_PROGRAMMABLE_PIPELINE;
				if(fullscreen)
					flags |= ALLEGRO_FULLSCREEN_WINDOW;
				al_set_new_display_flags(flags);
				context = al_create_display(width,height);
			}
			if(!context)
			{
				throw Exception("Please go to your programs in the start menu and under Stemwater Games, try one of the compatibility modes.");
			}
			contextFail = true;
#else
			throw Exception("GL Display could not be created.");
#endif
			
		}
		Log::write("Display","Created");
		//initContext(context);
		//toggleMaximizeButton(false);
        
        if(Platf::isMobile() && getResolution().getX() >= 2048)
        {
            Platf::_setRetina(true);
        }

		if(!Platf::isMobile() && getResolution().getX() >= 1900)
		{
			Platf::_setHD(true);
		}

#ifdef CGE_OSX
            setWindowCaption(m_caption);
        OSXHandler::getInstance().setWindowTitle(context,m_caption);
#else
    		setWindowCaption(m_caption);
#endif
		Shader::setEnabled(!disableShaders);
#ifdef CGE_WINDOWS
		Shader::setGlsl(usingGL);
#else
		Shader::setGlsl(true);
#endif
		al_use_shader(NULL);
		//Shader::getDefaultShader().load("res/shader/default","res/shader/default");

		Log::write("Display",std::string("Constraints: ") + "Min Width:" + 
			StringUtil::toString(minWidth)  + " Min Height:" + StringUtil::toString(minHeight));
		Log::write("Display","Max Texture Size: " + StringUtil::toString(getMaxTextureSize()));
		Log::write("Display","Monitor Resolution: Width:" + 
			StringUtil::toString(getMonitorResolution().getX()) + " Height:" + 
			StringUtil::toString(getMonitorResolution().getY()));
		Log::write("Display","Vendor: " + getVendor());
		Log::write("Display","Model: " + getGraphicsModel());
	}

	void Display::setResolution( int width, int height )
	{
		al_resize_display(context,width,height);
	}

	void Display::setResolution( const Vec2& size )
	{
		setResolution(size.getX(),size.getY());
	}

	Vec2 Display::getResolution() const
	{
		return Vec2(
			(float)al_get_display_width(context),
			(float)al_get_display_height(context));
	}

	Vec2 Display::getMonitorResolution()
	{
		ALLEGRO_MONITOR_INFO info;
		al_get_monitor_info(0,&info);
		return Vec2(
			float(info.x2 - info.x1),
			float(info.y2 - info.y1));
	}

	void Display::setWindowCaption( const std::string& caption )
	{
		al_set_window_title(context,caption.c_str());
		Log::write("Display","Window Caption Changed: " + caption);
		m_caption = caption;
	}

	DisplayContext* Display::getContext() const
	{
		return context;
	}

	void Display::setPosition( int x, int y )
	{
		al_set_window_position(context,x,y);
	}

	void Display::setPosition( const Vec2& position )
	{
		al_set_window_position(context,position.getX(),position.getY());
	}

	Vec2 Display::getPosition() const
	{
		int x = 0;
		int y = 0;
		al_get_window_position(context,&x,&y);
		return Vec2(float(x),float(y));
	}

	Vec2 Display::getMinResolution() const
	{
		return Vec2(minWidth,minHeight);
	}

	void Display::setIcon( Sprite* icon )
	{
		al_set_display_icon(getContext(),icon->getBitmap());
	}

	Vec2 Display::getOptimalResoluton()
	{
		Vec2 v = getMonitorResolution();

		v.setX(v.getX() * 0.95f);
		v.setY(v.getY() * 0.84f);

		return v;
	}

	bool Display::toggleFullscreen( bool fullscreen )
	{
		bool result = al_toggle_display_flag(context,ALLEGRO_FULLSCREEN_WINDOW,fullscreen);
		if(!fullscreen)
		{
			al_destroy_bitmap(al_clone_bitmap(al_get_backbuffer(context)));
		}
		/*
		if(!fullscreen)
		{
			m_sceneMessenger->sendContextWillChangedMessage();
			Vec2 resolution = getResolution();
			al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_WINDOWED);
			al_set_new_bitmap_flags(
				ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | 
				ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP | ALLEGRO_CONVERT_BITMAP);
			al_destroy_display(context);
			context = al_create_display(resolution.getX(),resolution.getY());
			al_set_new_bitmap_flags(
				ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | 
				ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP | ALLEGRO_CONVERT_BITMAP);
			al_convert_bitmaps();
			m_sceneMessenger->sendContextChangedMessage();
		}
		*/
		if(result)
		{
			resizeSceneForFullscreen(getResolution().getX(),getResolution().getY());
			if(fullscreen)
			Log::write("Display","Window is now Fullscreen");
			else
			Log::write("Display","Window is now Windowed");
		}
		//initContext(context);
		return result;

	}

	void Display::initContext( DisplayContext* c )
	{
		/*
		if(!(al_get_display_flags(context) & ALLEGRO_FULLSCREEN_WINDOW))
		{
#ifdef _WIN32
			HWND win = al_get_win_window_handle(context);
			SetWindowLong(win, GWL_EXSTYLE,  WS_EX_APPWINDOW|WS_EX_WINDOWEDGE);
			SetWindowPos(win,NULL,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
#endif
		}
		*/
	}

	void Display::setSceneMessenger( SceneManagerMessage* messenger )
	{
		m_sceneMessenger = messenger;
	}

	void Display::resizeSceneForFullscreen(int w, int h)
	{
		if(m_sceneMessenger)
		{
			m_sceneMessenger->sendResizeMessage(w,h);
		}
	}

	bool Display::isFullscreen() const
	{
		return al_get_display_flags(context) & ALLEGRO_FULLSCREEN_WINDOW;
	}

	int Display::getMaxTextureSize() const
	{
		if(context)
		{
			int m = al_get_display_option(context,ALLEGRO_MAX_BITMAP_SIZE);
            return m;
		}

		return 0;
	}

	void Display::toggleMaximized( bool maximized )
	{
#ifdef _WIN32
		HWND hwnd = al_get_win_window_handle(context);
		if(maximized)
		{
			SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		else
		{
			SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		}
#endif
	}

	bool Display::isMaximized() const
	{
#ifdef _WIN32
		HWND hwnd = al_get_win_window_handle(context);
		WINDOWPLACEMENT p;
		GetWindowPlacement(hwnd,&p);
		return p.showCmd == SW_MAXIMIZE;
#endif

		return false;
	}

	void Display::toggleMaximizeButton( bool on )
	{
#ifdef _WIN32
		HWND hwnd = al_get_win_window_handle(context);
		if(!on)
		{
			SetWindowLong(hwnd, GWL_STYLE,
				GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
		}
		else
		{
			SetWindowLong(hwnd, GWL_STYLE,
				GetWindowLong(hwnd, GWL_STYLE) | WS_MAXIMIZEBOX);
		}
		
		
#endif
	}

	void Display::doneLoading()
	{
		//toggleMaximizeButton(true);
		Log::write("Display","Done loading");
	}

	std::string Display::getVendor() const
	{
		std::string s;
		char* result = (char*)glGetString(GL_VENDOR);
		if(result != NULL)
		s += result;
		else
			s += "NOT FOUND";
		return s;
	}

	std::string Display::getGraphicsModel() const
	{
		std::string s;
		char* result = (char*)glGetString(GL_RENDERER);
		if(result != NULL)
			s += result;
		else
			s += "NOT FOUND";
		return s;
	}

	bool Display::isAppAlreadyRunning() const
	{
#ifdef CGE_DEV
		return false;
#endif
		#ifdef _WIN32
		char szClassName[] = "Stemwater Spades";
		HWND hWndPrev;
		HANDLE hMutex;
		hMutex = CreateMutexA(NULL, TRUE, szClassName); 
		if(GetLastError() == ERROR_ALREADY_EXISTS) 
		{     
			hWndPrev = FindWindowA(NULL, getWindowCaption().c_str());
			if(hWndPrev != NULL)
				SetForegroundWindow(hWndPrev);
			return true;   // this instance exits 
		} 
#endif

		return false;
	}

	const std::string& Display::getWindowCaption() const
	{
		return m_caption;
	}

	int Display::getVramAmount() const
	{
		return 0;
	}

	bool Display::isIntelGMA4000() const
	{
#ifdef CGE_WINDOWS
		bool v = getVendor() == "Intel";
		bool g = getGraphicsModel().find("Intel(R) HD Graphics ") != std::string::npos;
		return v && g;
#else
        return false;
#endif
	}
    
    bool Display::isLowQuality() const
    {
        return Platf::isMobile() && getResolution().getX() <= 1024;
    }
    
    void Display::resetCaption()
    {
        setWindowCaption(m_caption);
    }

}


