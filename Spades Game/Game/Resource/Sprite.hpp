#ifndef CGE_SPRITE_HPP
#define CGE_SPRITE_HPP
#include <stdlib.h>
#include <string>
#include "Game/platform.hpp"
#include <list>
namespace cge
{
	class Sprite
	{
		ALLEGRO_BITMAP* m_bitmap;
		int m_width;
		int m_height;
        std::string m_path;
        static std::list<Sprite*> allocedSprites;
	public:
		Sprite(const std::string& fileName);
		Sprite(int width, int height);
		int getWidth() const;
		int getHeight() const;
		ALLEGRO_BITMAP* getBitmap() const;
		bool isLoaded() const;
        void reload();
        void destroy();
		ALLEGRO_BITMAP* operator()() const;
        static void destroyAll();
        static void reloadAll();
		~Sprite(void);
	};
}

#endif