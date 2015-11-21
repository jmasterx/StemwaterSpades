#include "Game/Element/ChairManager.hpp"
#include "Game/Utility/File.hpp"
#include <sstream>
namespace cge
{
	ChairManager::~ChairManager(void)
	{
		_cleanChair();
		_cleanChairs();
	}

	bool ChairManager::_loadChairs( 
		const std::string& path, const std::string& bgFile )
	{
		std::string fullPath = bgFile;	

		_cleanChair();
		_cleanChairs();
		m_chairs.clear();

		//open the file
		File ifs( fullPath.c_str() );

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

			//name, chair, offsetX, offsetY, scale, thumbnail, id 
			std::string attrib[10];
			Sprite* bgThumbImg = NULL;

			for(int i = 0; i < 10; ++i)
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

			if(attrib[8] != "")
			{
				bgThumbImg = new Sprite(attrib[8]);
			}

			float values[6] = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };

			int j = 0;
			for(int i = 2; i < 8; ++i, ++j)
			{
				std::stringstream ss;
				ss << attrib[i];
				ss >> values[j];
			}

			int id = -1;
			std::stringstream ss;
			ss << attrib[9];
			ss >> id;


			m_chairs.push_back(ChairData(bgThumbImg,path + attrib[1],values[0],
				values[1],values[2],attrib[0],id,values[5],values[3],values[4]));
		}

		return true;
	}

	void ChairManager::_cleanChair()
	{
		for(size_t i = 0; i < m_currentChair.size(); ++i)
		{
			if(m_currentChair[i])
			{
				delete m_currentChair[i];
				m_currentChair[i] = NULL;
			}

			m_currentChairData[i] = NULL;
		}

		m_currentChair.clear();
		m_currentChairData.clear();
		
	}

	void ChairManager::_cleanChairs()
	{
		for(std::list<ChairData>::iterator it = m_chairs.begin();
			it != m_chairs.end(); ++it)
		{
			if(it->m_thumbnail)
			{
				delete it->m_thumbnail;
				it->m_thumbnail = NULL;
			}
		}
	}

	ChairManager::ChairManager( const std::string& path, const std::string& tableFile )
	{
		_loadChairs(path,tableFile);
	}

	bool ChairManager::loadChair( const std::vector<ChairData*>& chairDatas )
	{
		_cleanChair();
		for(int i = 0; i < (int)chairDatas.size(); ++i)
		{
			if(chairDatas[i])
			{
				Sprite* bg = new Sprite(chairDatas[i]->m_chairPath);	

				if(!bg->getBitmap())
				{
					delete bg;
					_cleanChair();
					return false;
				}

				m_currentChair.push_back(bg);
				m_currentChairData.push_back(chairDatas[i]);
			}
			
		}
		
	

		return true;
	}

	ChairData* ChairManager::getCurrentChairData(int chairNo) const
	{
		if(chairNo >= m_currentChairData.size() || chairNo < 0)
		{
			return NULL;
		}

		return m_currentChairData[chairNo];
	}

	Sprite* ChairManager::getCurrentChair(int chairNo) const
	{
		if(chairNo >= m_currentChair.size() || chairNo < 0)
		{
			return NULL;
		}

		return m_currentChair[chairNo];
	}

	ChairData* ChairManager::getChairByName( const std::string& name )
	{
		for(std::list<ChairData>::iterator it = m_chairs.begin();
			it != m_chairs.end(); ++it)
		{
			if(it->m_name == name)
			{
				return &(*it);
			}
		}

		return NULL;
	}

	int ChairManager::getChairCount() const
	{
		return (int)m_chairs.size();
	}

	std::list<ChairData>::iterator ChairManager::begin()
	{
		return m_chairs.begin();
	}

	std::list<ChairData>::iterator ChairManager::end()
	{
		return m_chairs.end();
	}

	ChairData* ChairManager::getChairAt( int index )
	{
		int count = 0;
		for(std::list<ChairData>::iterator it = m_chairs.begin();
			it != m_chairs.end(); ++it)
		{
			if(count == index)
			{
				return &(*it);
			}

			count++;
		}

		return NULL;
	}

	int ChairManager::getLoadedChairCount() const
	{
		return m_currentChair.size();
	}

	ChairData* ChairManager::getChairFromID( int id )
	{
		int count = 0;
		for(std::list<ChairData>::iterator it = m_chairs.begin();
			it != m_chairs.end(); ++it)
		{
			if(it->id == id)
			{
				return &(*it);
			}

			count++;
		}

		return NULL;
	}

	void ChairManager::unload()
	{
		_cleanChair();
	}

}
