#include "Game/Element/BackgroundManager.hpp"
#include "Game/Utility/File.hpp"
#include <sstream>
namespace cge
{
	BackgroundManager::~BackgroundManager(void)
	{
		_cleanBackground();
		_cleanBackgrounds();
	}

	bool BackgroundManager::_loadBackgrounds( 
		const std::string& path, const std::string& bgFile )
	{
		std::string fullPath = bgFile;	

		_cleanBackground();
		_cleanBackgrounds();
		m_backgrounds.clear();

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

			//name, background, offsetX, offsetY, scale, thumbnail, id 
			std::string attrib[7];
			Sprite* bgThumbImg = NULL;

			for(int i = 0; i < 7; ++i)
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

				//need 6 valid attributes
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

			if(attrib[5] != "")
			{
				bgThumbImg = new Sprite(attrib[5]);
			}

			float values[3] = { 0.0f,0.0f,0.0f };

			int j = 0;
			for(int i = 2; i < 5; ++i, ++j)
			{
				std::stringstream ss;
				ss << attrib[i];
				ss >> values[j];
			}

			int id = -1;
			std::stringstream ss;
			ss << attrib[6];
			ss >> id;

			std::vector<std::string> paths;

				std::stringstream sstr(attrib[1]);
				std::string item;
				while (std::getline(sstr, item, ';')) {
					paths.push_back(path + item);
				}

			m_backgrounds.push_back(BackgroundData(bgThumbImg,paths,values[0],
				values[1],values[2],attrib[0],id));
		}

		return true;
	}

	void BackgroundManager::_cleanBackground()
	{
		for(int i = 0; i < m_currentBackground.size(); i++)
		{
			delete m_currentBackground[i];
		}

		m_currentBackground.clear();
	}

	void BackgroundManager::_cleanBackgrounds()
	{
		for(std::list<BackgroundData>::iterator it = m_backgrounds.begin();
			it != m_backgrounds.end(); ++it)
		{
			if(it->m_thumbnail)
			{
				delete it->m_thumbnail;
				it->m_thumbnail = NULL;
			}
		}
	}

	BackgroundManager::BackgroundManager( const std::string& path, const std::string& tableFile )
		: m_currentBackgroundData(NULL), m_currentBackground(NULL)
	{
		_loadBackgrounds(path,tableFile);
	}

	bool BackgroundManager::loadBackground( BackgroundData* data )
	{
		if(!data)
		{
			return false;
		}

		_cleanBackground();

		for(int i = 0; i < data->m_backgroundPath.size(); i++)
		{
			Sprite* bg = new Sprite(data->m_backgroundPath[i]);
			m_currentBackground.push_back(bg);
		}

		m_currentBackgroundData = data;

		return true;
	}

	bool BackgroundManager::isBackgroundLoaded() const
	{
		return m_currentBackgroundData != NULL;
	}

	BackgroundData* BackgroundManager::getCurrentBackgroundData() const
	{
		return m_currentBackgroundData;
	}

	const std::vector<Sprite*>& BackgroundManager::getCurrentBackground() const
	{
		return m_currentBackground;
	}

	BackgroundData* BackgroundManager::getBackgroundByName( const std::string& name )
	{
		for(std::list<BackgroundData>::iterator it = m_backgrounds.begin();
			it != m_backgrounds.end(); ++it)
		{
			if(it->m_name == name)
			{
				return &(*it);
			}
		}

		return NULL;
	}

	int BackgroundManager::getBackgroundCount() const
	{
		return (int)m_backgrounds.size();
	}

	std::list<BackgroundData>::iterator BackgroundManager::begin()
	{
		return m_backgrounds.begin();
	}

	std::list<BackgroundData>::iterator BackgroundManager::end()
	{
		return m_backgrounds.end();
	}

	BackgroundData* BackgroundManager::getBackgroundAt( int index )
	{
		int count = 0;
		for(std::list<BackgroundData>::iterator it = m_backgrounds.begin();
			it != m_backgrounds.end(); ++it)
		{
			if(count == index)
			{
				return &(*it);
			}

			count++;
		}

		return NULL;
	}

	BackgroundData* BackgroundManager::getBackgroundFromID( int id )
	{
		int count = 0;
		for(std::list<BackgroundData>::iterator it = m_backgrounds.begin();
			it != m_backgrounds.end(); ++it)
		{
			if(it->id == id)
			{
				return &(*it);
			}

			count++;
		}

		return NULL;
	}

	void BackgroundManager::unload()
	{
		_cleanBackground();
	}

}
