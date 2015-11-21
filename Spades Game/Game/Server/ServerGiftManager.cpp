#include "Game/Server/ServerGiftManager.hpp"
#include <fstream>
#include <string>
#include <sstream>
namespace cge
{
	ServerGiftManager::~ServerGiftManager(void)
	{
	}

	ServerGiftManager::ServerGiftManager( 
		const std::string& path)
	{
		_loadGifts(path);
	}

	bool ServerGiftManager::_loadGifts( const std::string& path)
	{
		std::string fullPath = path;
		_cleanGiftData();

		//open the file
		 std::ifstream ifs(fullPath.c_str());

		//ensure it is open
		if(!ifs.is_open())
		{
			return false;
		}
		std::string line;

		//read each line
		while( std::getline(ifs,line) )
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

			for(int i = 0; i < 7; ++i)
			{
				if(!std::getline(ifs,line))
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
				paths.push_back(item);
			}

			std::stringstream ssstr(attrib[1]);
			while (std::getline(ssstr, item, ';')) {
				pathsAlt.push_back(item);
			}

			std::stringstream sssstr(attrib[3]);
			while (std::getline(sssstr, item, ';')) {
				if(item.length() > 0)
					sounds.push_back(item);
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

			std::string thumbPath = pathsAlt[0];

			m_gifts.push_back(
				GiftData(attrib[0],id,thumbPath,paths,
				values,sounds,attrib[4],gains,attrib[4] == "category.animated"));
		}

		return true;
	}

	void ServerGiftManager::_cleanGiftData()
	{
		m_gifts.clear();
	}

	GiftData* ServerGiftManager::getGiftById( int id )
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

	std::list<GiftData>* ServerGiftManager::getGifts()
	{
		return &m_gifts;
	}
}