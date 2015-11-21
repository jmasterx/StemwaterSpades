#include "Game/UI/GuiFontManager.hpp"
#include "Agui/Backends/Allegro5/Allegro5.hpp"
namespace cge
{
	GuiFontManager::~GuiFontManager(void)
	{
		for(size_t i = 0; i < m_fonts.size(); ++i)
		{
			delete m_fonts[i];
		}

		delete m_tempFont;
		m_tempFont = NULL;
	}

	agui::Font* GuiFontManager::getDefaultFont()
	{
		return getFont((int)m_defaultSize);
	}

	agui::Font* GuiFontManager::getFont( int size )
	{
		if(m_fonts.empty())
		{
			return NULL;
		}

		int pos = size - m_beginRange;

		if(pos < 0)
		{
			return getFontAt(0);
		}
		else if(pos >= (int)m_fonts.size())
		{
			return getFontAt(m_fonts.size() - 1);
		}
		
		return getFontAt(pos);

	}

	agui::Font* GuiFontManager::getFont( float scale )
	{
		int size = (int)((float)m_defaultSize * scale);
		return getFont(size);
	}

	agui::Font* GuiFontManager::getFont(const agui::Font* ref, float scale )
	{
		int fontSz = -1;
		for(size_t i = 0; i < m_fonts.size(); ++i)
		{
			if(m_fonts[i] == ref)
			{
				fontSz = i;
				break;
			}
		}

		if(fontSz == -1)
		{
			return NULL;
		}

		fontSz += m_beginRange;

		int size = (int)((float)fontSz * scale);

		return getFont(size);
	}

	GuiFontManager::GuiFontManager( const std::string& fontPath, int beginRange, int endRange, int defaultSize, 
		const std::string& fontPathPretty, int beginRangePretty, int endRangePretty, int defaultSizePretty )
		: m_beginRange(beginRange), m_endRange(endRange), m_defaultSize(defaultSize),
		m_fontPath(fontPath),m_tempFont(NULL),m_beginRangePretty(beginRangePretty),
		m_endRangePretty(endRangePretty),m_defaultSizePretty(defaultSizePretty),
		m_fontPathPretty(fontPathPretty)
	{

	}


	void GuiFontManager::loadTempFont( float scale )
	{
		if(m_tempFont)
		{
			delete m_tempFont;
			m_tempFont = NULL;
		}

		m_tempFont = agui::Font::load("res/blackcherry.ttf",m_defaultSize * scale * 1.3f);
	}

	agui::Font* GuiFontManager::getTempFont()
	{
		return m_tempFont;
	}

	void GuiFontManager::_loadFonts()
	{
		for(int i = m_beginRange; i <= m_endRange; ++i)
		{
			m_fonts.push_back(agui::Font::load(m_fontPath,i));
		}

		for(int i = m_beginRangePretty; i <= m_endRangePretty; ++i)
		{
			m_prettyFonts.push_back(agui::Font::load(m_fontPathPretty,i));
		}
	}

	agui::Font* GuiFontManager::getFontAt( int index )
	{
		agui::Font* f = m_fonts[index];
		if(f == NULL)
			m_fonts[index] = agui::Font::load(m_fontPath, m_beginRange + index);

		return m_fonts[index];
	}

	agui::Font* GuiFontManager::getPrettyFontAt( int index )
	{
		agui::Font* f = m_prettyFonts[index];
		if(f == NULL)
			m_prettyFonts[index] = agui::Font::load(m_fontPathPretty, m_beginRangePretty + index);

		return m_prettyFonts[index];
	}

	agui::Font* GuiFontManager::getPrettyDefaultFont()
	{
		return getPrettyFont((int)m_defaultSizePretty);
	}

	agui::Font* GuiFontManager::getPrettyFont( int size )
	{
		if(m_prettyFonts.empty())
		{
			return NULL;
		}

		int pos = size - m_beginRangePretty;

		if(pos < 0)
		{
			return getPrettyFontAt(0);
		}
		else if(pos >= (int)m_prettyFonts.size())
		{
			return getPrettyFontAt(m_prettyFonts.size() - 1);
		}

		return getPrettyFontAt(pos);
	}

	agui::Font* GuiFontManager::getPrettyFont( float scale )
	{
		int size = (int)((float)m_defaultSizePretty * scale);
		return getPrettyFont(size);
	}

	agui::Font* GuiFontManager::getPrettyFont( const agui::Font* ref, float scale )
	{
		int fontSz = -1;
		for(size_t i = 0; i < m_prettyFonts.size(); ++i)
		{
			if(m_prettyFonts[i] == ref)
			{
				fontSz = i;
				break;
			}
		}

		if(fontSz == -1)
		{
			return NULL;
		}

		fontSz += m_beginRangePretty;

		int size = (int)((float)fontSz * scale);

		return getPrettyFont(size);
	}


}
