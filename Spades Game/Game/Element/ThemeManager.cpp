#include "Game/Element/ThemeManager.hpp"
#include "Game/Utility/File.hpp"
#include <sstream>
namespace cge
{
	ThemeManager::~ThemeManager(void)
	{
		destroyThumbs();
	}

	bool ThemeManager::_loadThemes( const std::string& path, const std::string& themeFile )
	{
		std::string fullPath = themeFile;	

		destroyThumbs();
		m_themes.clear();

		//open the file
		File ifs( fullPath );

		//ensure it is open
		if(!ifs.isOpen())
		{
			return false;
		}
		std::string line;

		//read each line
		while( ifs.readLine(line) )
		{

			if(line.length() == 0)
			{
				continue;
			}
			else if(line.length() >= 1 && line[0] == '#')
			{
				continue;
			}

			if(line.length() >= 1 && line[0] != '@')
			{
				continue;
			}

			//name, table, chair, bg, thumbnail, song, vid, id 
			std::string attrib[8];
			Sprite* bgThumbImg = NULL;

			for(int i = 0; i < 8; ++i)
			{
				if(!ifs.readLine(line))
					return false;

				int pos = -1;
				for(size_t j = 0; j < line.length(); ++j)
				{
					if(line[j] == ':')
					{
						pos = j;
						break;
					}
				}

				//need 8 valid attributes
				if(pos == -1)
				{
					return false;
				}

				//move to first character
				pos+= 2;
				if(pos < (int)line.length())
				{
					attrib[i] = line.substr(pos);
				}
			}

			if(attrib[4] != "")
			{
				attrib[4] = path + attrib[4];
			}

			int values[3] = { 0,0,0 };

			int j = 0;
			for(int i = 1; i < 4; ++i, ++j)
			{
				std::stringstream ss;
				ss << attrib[i];
				ss >> values[j];
			}

			int vid = -1;
			std::stringstream sss;
			sss << attrib[6];
			sss >> vid;

			int id = -1;
			std::stringstream ss;
			ss << attrib[7];
			ss >> id;

			std::string strSong;
			if(attrib[5].length() > 0)
			{
				strSong += "music/" + attrib[5];
			}
			m_themes.push_back(ThemeData(attrib[0],values[1],values[0],values[2],id,NULL,attrib[4],strSong,vid));
		}

		return true;
	}

	void ThemeManager::destroyThumbs()
	{
		for(std::list<ThemeData>::iterator it = m_themes.begin();
			it != m_themes.end(); ++it)
		{
			if(it->m_thumbnail)
			{
				delete it->m_thumbnail;
				it->m_thumbnail = NULL;
				it->m_img = agui::Allegro5Image();
			}
		}
	}

	ThemeManager::ThemeManager( const std::string& path, const std::string& bgFile )
		: m_currentThemeData(NULL)
	{
		_loadThemes(path,bgFile);
	}

	std::list<ThemeData>* ThemeManager::getThemes()
	{
		return &m_themes;
	}

	ThemeData* ThemeManager::getTheme( int id )
	{
		for(std::list<ThemeData>::iterator it = m_themes.begin();
			it != m_themes.end(); ++it)
		{
			if(it->id == id)
			{
				return &(*it);
			}
		}
		return NULL;
	}

	void ThemeManager::loadTheme( ThemeData* data )
	{
		m_currentThemeData = data;
	}

	ThemeData* ThemeManager::getLoadedTheme()
	{
		return m_currentThemeData;
	}

	bool ThemeManager::isThemeLoaded() const
	{
		return m_currentThemeData != NULL;
	}

	void ThemeManager::loadThumbs()
	{
		destroyThumbs();

		for(std::list<ThemeData>::iterator it = m_themes.begin();
			it != m_themes.end(); ++it)
		{
			if(it->m_thumbPath.length() > 0)
			{
				it->m_thumbnail = new Sprite(it->m_thumbPath);
				it->m_img.setBitmap(it->m_thumbnail->getBitmap());
			}
		}
	}
}
