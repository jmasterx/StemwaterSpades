#include "Game/Avatar/AvatarCache.hpp"
#include "Game/Utility/File.hpp"
#include <sstream>
namespace cge
{
	AvatarCache::AvatarCache( const std::string& path, const std::string& hdPath, const std::string& sdPath,
		const std::string& filename )
	{
		_loadAvatars(path,hdPath,sdPath,filename);
	}

	AvatarCache::~AvatarCache(void)
	{
		_clearAvatarData();
		_clearAvatarImages();
		clearGameCache();
		_clearSelectionCache();
	}

	bool AvatarCache::_loadAvatars( const std::string& path, const std::string& hdPath,
		const std::string& sdPath, const std::string& filename )
	{
		std::string sdFullPath = path + sdPath;
		std::string hdFullPath = path + hdPath;

		std::string fullPath = path + filename;

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

			//name, id, gender, ethnicity 
			std::string attrib[4];

			for(int i = 0; i < 4; ++i)
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

				//need 4 valid attributes
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

			int id = -1;

				std::stringstream ss;
				ss << attrib[1];
				ss >> id;

				if(ss.fail())
					return false;

				CachedAvatarData data;
				data.id = id;
				data.ethnicity = attrib[3] == "w" ?
                    AVATAR_CACHE_ETHNICITY_WHITE :
					AVATAR_CACHE_ETHNICITY_BLACK;

					data.gender = data.ethnicity = attrib[2] == "m" ?
					AVATAR_CACHE_GENDER_MALE :
					AVATAR_CACHE_GENDER_FEMALE;
				data.hdPath = hdFullPath + attrib[0];
				data.sdPath = sdFullPath + attrib[0];

				if(!avatarExists(id))
				{
					m_avatarData[id] = data;
					m_avatarImages[id] = loadImage(data,false);
				}
		}

		return true;
	}

	void AvatarCache::_clearAvatarData()
	{
		m_avatarData.clear();
	}

	void AvatarCache::_clearAvatarImages()
	{
		std::map<int,CachedAvatarImage>::iterator iter;

		for(iter = m_avatarImages.begin(); iter != m_avatarImages.end(); iter++)
		{
			delete iter->second.image;
			iter->second.image = NULL;
		}

		m_avatarImages.clear();
	}

	void AvatarCache::clearGameCache()
	{
		for(int i = 0; i < 4; i++)
		{
			delete m_gameCache[i].image;
			m_gameCache[i].image = NULL;
		}
	}

	void AvatarCache::clearGameCacheAtPos( int pos )
	{
		delete m_gameCache[pos].image;
		m_gameCache[pos].image = NULL;
	}

	void AvatarCache::_clearSelectionCache()
	{
		delete m_selectionCache.image;
		m_selectionCache.image = NULL;
	}

	bool AvatarCache::setAvatarSelection( int id )
	{
		_clearSelectionCache();
		if(avatarExists(id))
		{
			CachedAvatarData data = m_avatarData[id];
			m_selectionCache = loadImage(data,true);
			return true;
		}

		return false;
	}

	bool AvatarCache::avatarExists( int id )
	{
		return m_avatarData.count(id) > 0;
	}

	cge::CachedAvatarImage AvatarCache::loadImage( const CachedAvatarData& data, bool hd )
	{
		CachedAvatarImage image;
		image.ethnicity = data.ethnicity;
		image.gender = data.gender;
		image.id = data.id;
		image.reserved = data.reserved;

		image.reserved = image.id <= 4;
		if(hd)
		{
			image.image = new Sprite(data.hdPath);
		}
		else
		{
			image.image = new Sprite(data.sdPath);
		}

		image.guiImg.setBitmap(image.image->getBitmap());
		return image;

	}

	void AvatarCache::clearAvatarSelection()
	{
		_clearSelectionCache();
	}

	std::map<int,CachedAvatarImage>* AvatarCache::getCachedAvatars()
	{
		return &m_avatarImages;
	}

	bool AvatarCache::setGameAvatar( int id, int index )
	{
		if(index >= 4)
			return false;

		if(avatarExists(id))
		{
			clearGameCacheAtPos(index);
			CachedAvatarData data = m_avatarData[id];
			m_gameCache[index] = loadImage(data,true);
			return true;
		}
		return false;
	}

	std::map<int,CachedAvatarData>* AvatarCache::getCachedAvatarData()
	{
		return &m_avatarData;
	}

	CachedAvatarImage* AvatarCache::getGameAvatarAt( int index )
	{
		return &m_gameCache[index];
	}

}
