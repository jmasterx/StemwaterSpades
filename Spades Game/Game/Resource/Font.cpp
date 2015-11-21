#include "Font.hpp"

namespace cge
{
	Font::Font( const std::string& fileName, int pxSize, int flags )
		: m_font(NULL),m_size(pxSize)
	{
		m_font = al_load_font(fileName.c_str(),pxSize,flags);
	}

	Font::~Font(void)
	{
		al_destroy_font(m_font);
	}

	int Font::getLineHeight() const
	{
		return al_get_font_line_height(m_font);
	}

	int Font::getWidth( const std::string& text ) const
	{
		return al_get_text_width(m_font,text.c_str());
	}

	int Font::getSize() const
	{
		return m_size;
	}

	bool Font::isLoaded() const
	{
		return m_font != NULL;
	}

	ALLEGRO_FONT* Font::getFont() const
	{
		return m_font;
	}

	ALLEGRO_FONT* Font::operator()() const
	{
		return m_font;
	}

}

