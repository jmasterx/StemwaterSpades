#ifndef CGE_RESOURCE_MANAGER_HPP
#define CGE_RESOURCE_MANAGER_HPP

#include "Game/platform.hpp"
#include "Game/Resource/Sprite.hpp"
#include "Game/Resource/Sound.hpp"
#include "Game/Resource/Stream.hpp"
#include <map>
namespace cge
{
	class ResourceManager
	{
		std::map<std::string,std::pair<Sprite*,int> > spriteMap;
		std::map<std::string,std::pair<Sound*,int> > soundMap;
		std::map<std::string,std::pair<Stream*,int> > stream_map;
	public:
		enum ResourceTypeEnum
		{
			SPRITE,
			SOUND,
			STREAM
		};
		bool contains(ResourceTypeEnum type, const std::string& path);
		bool addResource(ResourceTypeEnum type, const std::string& path);
		bool removeResource(ResourceTypeEnum type, const std::string& path);
		Sprite* requestSprite(const std::string& path);
		void releaseSprite(const std::string& path);
		Sound* requestSound(const std::string& path);
		void releaseSound(const std::string& path);
		Stream* requestStream(const std::string& path);
		void releaseStream(const std::string& path);
		Sprite* getSprite(const std::string& spriteName);
		Sound* getSound(const std::string& soundName);
		Stream* getStream(const std::string& stream_name);
		ResourceManager(void);
		virtual ~ResourceManager(void);
	};
}
#endif
