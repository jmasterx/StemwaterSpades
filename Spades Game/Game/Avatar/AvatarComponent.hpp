#ifndef CGE_AVATAR_COMPONENT_HPP
#define CGE_AVATAR_COMPONENT_HPP
#include "Game/Resource/Sprite.hpp"
#include <stdlib.h>
#include <string>
namespace cge
{
	//not really used... part of original avatar system
	class AvatarComponent
	{
		float m_scale;
		float m_x;
		float m_y;
		std::string m_imgPath;
		Sprite* m_sprite;
	public:
		AvatarComponent(std::string imgPath, float scale,float x,float y);
		float getX() const;
		float getY() const;
		float getScale() const;
		const std::string& getImgPath() const;
		void setSprite(Sprite* sprite);
		Sprite* getSprite();
		const Sprite* getSprite() const;
		virtual ~AvatarComponent(void);
	};
}

#endif
