#ifndef CGE_PLATFORM_HPP
#define CGE_PLATFORM_HPP
//#define CGE_DEV
//#define CGE_DEV_AUTOPLAY
#define CGE_SERVER_PORT 8015
#define CGE_SERVER_ADDRESS "master.stemwatergames.com"
//#define CGE_SERVER_ADDRESS "Josh-PC"
#define CGE_WEB_ADDRESS "http://stemwatergames.com"
#define CGE_APP_WEB_ADDRESS "http://app.stemwatergames.com"
//#define CGE_WEB_ADDRESS "127.0.0.1"

#ifdef _WIN32
#define CGE_WINDOWS
//define something for Windows (32-bit and 64-bit, this part is common)
#ifdef _WIN64
#define CGE_WINDOWS_64
//define something for Windows (64-bit only)
#endif
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
#define CGE_IPHONE
#define CGE_IPHONE_SIMULATOR
#define CGE_MOBILE
#elif TARGET_OS_IPHONE
// iOS device
#define CGE_IPHONE
#define CGE_IPHONE_DEVICE
#define CGE_MOBILE
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#define CGE_OSX
#else
// Unsupported platform
#endif
#elif __linux
// linux
#define CGE_LINUX
#elif __unix // all unices not caught above
// Unix
#define CGE_UNIX
#elif __posix
// POSIX
#define CGE_POSIX
#endif

#ifndef CGE_MOBILE
#define CGE_PC
#endif
#include "Game/Platform/Platf.hpp"
#include "Game/Utility/Vec2.hpp"
#include "MessageIdentifiers.h"

#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "PacketLogger.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include "RakNetTypes.h"
//used to make mysql play nice with raknet
#ifndef HAVE_INT8_T
#define HAVE_INT8_T
#endif
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_physfs.h>
#ifdef CGE_PC
#include <allegro5/allegro_native_dialog.h>
#endif
#include <physfs.h>
#include "stdlib.h"
#include <string>
#include "Game/Utility/File.hpp"
#include "Game/Engine/Log.hpp"

namespace cge {
#define PI 3.14159265f
typedef ALLEGRO_DISPLAY DisplayContext; 
typedef long long int64;
typedef ALLEGRO_TIMER Timer;
	class Color 
	{
		ALLEGRO_COLOR m_color;
	public:
		const ALLEGRO_COLOR& getColor() const
		{
			return m_color;
		}

		Color()
			: m_color(al_map_rgb(0,0,0))
		{
		}
		const ALLEGRO_COLOR& operator()() const
		{
			return getColor();
		}
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
			: m_color(al_map_rgba(r,g,b,a))
		{
		}

		Color(float r, float g, float b, float a = 1.0f)
			: m_color(al_map_rgba_f(r,g,b,a))
		{
		}
	};

	enum DirectionEnum
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
}
#endif