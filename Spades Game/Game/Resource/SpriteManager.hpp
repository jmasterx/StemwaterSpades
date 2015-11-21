#ifndef CGE_SPRITE_MANAGER_HPP
#define CGE_SPRITE_MANAGER_HPP
#include "Game/Resource/Sprite.hpp"
#include <map>
namespace cge
{
	class SpriteManager
	{
		std::map<std::string,Sprite*> m_images;
		bool _loadImages(const std::string& path, const std::string& appendPath);
	public:
		bool mapImage(const std::string& imageName, const std::string& imagePath);
		Sprite* getImage(const std::string& imageName);
			SpriteManager(const std::string& path,const std::string& appendPath);
		virtual ~SpriteManager(void);
	};
}


#endif
