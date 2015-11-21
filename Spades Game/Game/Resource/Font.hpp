#ifndef CGE_FONT_HPP
#define CGE_FONT_HPP
#include "Game/platform.hpp"
#include <stdlib.h>
#include <string>
namespace cge
{
	class Font
	{
		ALLEGRO_FONT* m_font;
		int m_size;
	public:
		int getLineHeight() const;
		int getWidth(const std::string& text) const;
		int getSize() const;
		bool isLoaded() const;
		ALLEGRO_FONT* getFont() const;
		ALLEGRO_FONT* operator()() const;
		Font(const std::string& fileName, int pxSize, int flags);
		virtual ~Font(void);
	};
}

#endif
