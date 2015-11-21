#include "Game/Avatar/AvatarComponent.hpp"
namespace cge
{

	AvatarComponent::~AvatarComponent(void)
	{
	}

	AvatarComponent::AvatarComponent( std::string imgPath, float scale,float x,float y )
		: m_scale(scale),m_x(x),m_y(y),m_imgPath(imgPath), m_sprite(NULL)
	{
	}

	float AvatarComponent::getX() const
	{
		return m_x;
	}

	float AvatarComponent::getY() const
	{
		return m_y;
	}

	float AvatarComponent::getScale() const
	{
		 return m_scale;
	}

	const std::string& AvatarComponent::getImgPath() const
	{
		return m_imgPath;
	}

	void AvatarComponent::setSprite( Sprite* sprite )
	{
		m_sprite = sprite;
	}

	Sprite* AvatarComponent::getSprite()
	{
		return m_sprite;
	}

	const Sprite* AvatarComponent::getSprite() const
	{
		return m_sprite;
	}

}
