#include "Game/Engine/ResourceManager.hpp"


namespace cge
{
	ResourceManager::ResourceManager(void)
	{
	}

	ResourceManager::~ResourceManager(void)
	{
		for(std::map<std::string, std::pair<Sprite*,int> >::iterator it = spriteMap.begin();
			it != spriteMap.end(); ++it)
		{
			delete it->second.first;
		}

		for(std::map<std::string, std::pair<Sound*,int> >::iterator it = soundMap.begin();
			it != soundMap.end(); ++it)
		{
			delete it->second.first;
		}

		for(std::map<std::string, std::pair<Stream*,int> >::iterator it = stream_map.begin();
			it != stream_map.end(); ++it)
		{
			delete it->second.first;
		}
	}
	bool ResourceManager::contains( ResourceTypeEnum type, const std::string& path )
	{
		switch (type)
		{
		case SPRITE:
			return spriteMap.find(path) != spriteMap.end();
			break;
		case SOUND:
			return soundMap.find(path) != soundMap.end();
			break;
		case STREAM:
			return stream_map.find(path) != stream_map.end();
			break;
		default:
			return false;
			break;
		}

	}

	bool ResourceManager::addResource( ResourceTypeEnum type, const std::string& path )
	{
		switch(type)
		{
		case SPRITE:
			if(!contains(SPRITE,path))
			{
				Sprite* sprite = new Sprite(path);
				if(sprite->isLoaded())
				{
					spriteMap[path] = std::pair<Sprite*,int>(sprite,0);
					return true;
				}
				else
				{
					delete sprite;
				}
			}
			break;
		case SOUND:
			if(!contains(SOUND,path))
			{
				Sound* sound = new Sound(path);
				if(sound)
				{
					soundMap[path] = std::pair<Sound*,int>(sound,0);
					return true;
				}
			}
			break;
		case STREAM:
			if(!contains(STREAM,path))
			{
				//split into 2 parts
				Stream* stream = new Stream(path.c_str(),2,1024);
				if(stream)
				{
					stream_map[path] = std::pair<Stream*,int>(stream,0);
					return true;
				}
			}
			break;
		default:
			return false;
		}

		return false;
	}

	bool ResourceManager::removeResource( ResourceTypeEnum type, const std::string& path )
	{
		switch(type)
		{
		case SPRITE:
			if(contains(SPRITE,path))
			{
				Sprite* sprite = spriteMap[path].first;
				delete sprite;
				spriteMap.erase(path);
				return true;
			}
			break;
		case SOUND:
			if(contains(SOUND,path))
			{
				Sound* sound = soundMap[path].first;
				delete sound;
				soundMap.erase(path);
				return true;
			}
			break;
		case STREAM:
			if(contains(STREAM,path))
			{
				Stream* stream = stream_map[path].first;
				delete stream;
				stream_map.erase(path);
				return true;
			}
			break;
		default:
			return false;
		}

		return false;
	}

	Sprite* ResourceManager::getSprite( const std::string& spriteName )
	{
		if(contains(SPRITE,spriteName))
		{
			return spriteMap[spriteName].first;
		}
		else
		{
			return NULL;
		}
	}

	Sound* ResourceManager::getSound( const std::string& soundName )
	{
		if(contains(SOUND,soundName))
		{
			return soundMap[soundName].first;
		}
		else
		{
			return NULL;
		}
	}

	Stream* ResourceManager::getStream( const std::string& stream_name )
	{
		if(contains(STREAM,stream_name))
		{
			return stream_map[stream_name].first;
		}
		else
		{
			return NULL;
		}
	}

	Sprite* ResourceManager::requestSprite( const std::string& path )
	{
		if(contains(SPRITE,path))
		{
			spriteMap[path].second++;
			return spriteMap[path].first;
		}
		else
		{
			//add it if it is not there
			if(addResource(SPRITE,path))
			{
				spriteMap[path].second++;
				return spriteMap[path].first;
			}
			return NULL;
		}
	}

	void ResourceManager::releaseSprite( const std::string& path )
	{
		if(contains(SPRITE,path))
		{
			spriteMap[path].second--;
			
			//reference counting
			if(spriteMap[path].second <= 0)
			{
				removeResource(SPRITE,path);
			}
		}
	}

	Sound* ResourceManager::requestSound( const std::string& path )
	{
		if(contains(SOUND,path))
		{
			soundMap[path].second++;
			return soundMap[path].first;
		}
		else
		{
			//add it if it is not there
			if(addResource(SOUND,path))
			{
				soundMap[path].second++;
				return soundMap[path].first;
			}
			return NULL;
		}
	}

	void ResourceManager::releaseSound( const std::string& path )
	{
		if(contains(SOUND,path))
		{
			soundMap[path].second--;
			//reference counting
			if(soundMap[path].second <= 0)
			{
				removeResource(SOUND,path);
			}
		}
	}

	Stream* ResourceManager::requestStream( const std::string& path )
	{
		if(contains(STREAM,path))
		{
			stream_map[path].second++;
			return stream_map[path].first;
		}
		else
		{
			//add it if it is not there
			if(addResource(STREAM,path))
			{
				stream_map[path].second++;
				return stream_map[path].first;
			}
			return NULL;
		}
	}

	void ResourceManager::releaseStream( const std::string& path )
	{
		if(contains(STREAM,path))
		{
			stream_map[path].second--;

			//reference counting
			if(stream_map[path].second <= 0)
			{
				removeResource(STREAM,path);
			}
		}
	}

}