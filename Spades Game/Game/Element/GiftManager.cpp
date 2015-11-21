#include "Game/Element/GiftManager.hpp"
#include "Game/Utility/File.hpp"
namespace cge
{
	GiftManager::~GiftManager(void)
	{
		destroyThumbs();
	}

	GiftManager::GiftManager( 
		const std::string& path, const std::string& thumbAppend,
		const std::string& spriteAppend, const std::string& soundPath, const std::string& bgFile )
	{
		_loadGifts(path,thumbAppend,spriteAppend, soundPath,bgFile);
	}

	bool GiftManager::_loadGifts( const std::string& path, const std::string& thumbAppend, const std::string& spriteAppend, const std::string& soundPath, const std::string& themeFile )
	{
		std::string fullPath = themeFile;
		std::string fullThumb = path + thumbAppend;
		std::string fullSprite = path + spriteAppend;

		_cleanGiftData();
		_cleanGifts();

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

			//name, gift,scale, sounds, category, gain, id 
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

				//need 7 valid attributes
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

			float values = 0.0f;

			int j = 0;
			for(int i = 2; i < 3; ++i, ++j)
			{
				std::stringstream ss;
				ss << attrib[i];
				ss >> values;
			}

			int id = -1;
			std::stringstream ss;
			ss << attrib[6];
			ss >> id;

			std::vector<std::string> paths;
			std::vector<std::string> sounds;
			std::vector<std::string> strGains;
			std::vector<float> gains;
			std::vector<std::string> pathsAlt;

			std::stringstream sstr(attrib[1]);
			std::string item;
			while (std::getline(sstr, item, ';')) {
				paths.push_back(fullSprite + item);
			}

			std::stringstream ssstr(attrib[1]);
			while (std::getline(ssstr, item, ';')) {
				pathsAlt.push_back(item);
			}

			std::stringstream sssstr(attrib[3]);
			while (std::getline(sssstr, item, ';')) {
				if(item.length() > 0)
				sounds.push_back(soundPath + item);
				else
					sounds.push_back("");
			}

			std::stringstream ssssstr(attrib[5]);
			while (std::getline(ssssstr, item, ';')) {
				if(item.length() > 0)
					strGains.push_back(item);
				else
					strGains.push_back("");
			}

			while(sounds.size() < paths.size())
				sounds.push_back("");

			while(strGains.size() < paths.size())
				strGains.push_back("");

			for(int i = 0; i < strGains.size(); i++)
			{
				std::stringstream fss;
				float f = 1.0f;
				if(strGains[i].length() > 0)
				{
					fss << strGains[i];
					fss >> f;
				}

				gains.push_back(f);
			}

			std::string thumbPath = fullThumb + pathsAlt[0];

			m_gifts.push_back(
				GiftData(attrib[0],id,thumbPath,paths,
				values,sounds,attrib[4],gains,attrib[4] == "category.animated"));
		}

		return true;
	}

	void GiftManager::_cleanGift( int index )
	{
		if(index >= 0 && index < MAX_GIFTS)
		{
			m_giftCache[index].m_thumbnail = NULL;
			delete m_giftCache[index].m_image;
			m_giftCache[index].m_image = NULL;
		}
	}

	void GiftManager::_cleanGifts()
	{
		for(int i = 0; i < MAX_GIFTS; i++)
		{
			_cleanGift(i);
		}
	}

	void GiftManager::destroyThumbs()
	{
		for(std::list<GiftData>::iterator it = m_gifts.begin();
			it != m_gifts.end(); ++it)
		{
			if(it->m_thumbnail)
			{
				delete it->m_thumbnail;
				it->m_thumbnail = NULL;
				it->m_img = agui::Allegro5Image();
			}
		}
	}

	void GiftManager::loadThumbs()
	{
		destroyThumbs();

		for(std::list<GiftData>::iterator it = m_gifts.begin();
			it != m_gifts.end(); ++it)
		{
			if(it->m_thumbPath.length() > 0)
			{
				it->m_thumbnail = new Sprite(it->m_thumbPath);
				it->m_img.setBitmap(it->m_thumbnail->getBitmap());
			}
		}
	}

	void GiftManager::_cleanGiftData()
	{
		destroyThumbs();
		m_gifts.clear();
	}

	GiftData* GiftManager::getGiftById( int id )
	{
		for(std::list<GiftData>::iterator it = m_gifts.begin();
			it != m_gifts.end(); ++it)
		{
			if(it->id == id)
			{
				return &(*it);
			}
		}

		return NULL;
	}

	std::list<GiftData>* GiftManager::getGifts()
	{
		return &m_gifts;
	}
}
