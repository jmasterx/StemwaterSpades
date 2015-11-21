#ifndef CGE_RENDERED_CARD_HPP
#define CGE_RENDERED_CARD_HPP
#include "Game/Resource/Sprite.hpp"
#include "Game/Utility/Vec2.hpp"
namespace cge
{
	class ViewCard
	{
	public:
		Vec2 m_position;
		float m_angle;
		float m_scale;
		bool m_enabled;
		bool m_selected;
		bool m_thrown;
		Sprite* m_card;

		Vec2 m_homePos;
		float m_homeScale;
		float m_homeAngle;

		float m_shadowIntensity;
		Vec2 m_shadowNormal;
		float m_shadowDistance;
		float m_toTableOpacity;
		bool m_headingToTable;

		float m_homeShadowIntensity;
		Vec2 m_homeShadowNormal;
		float m_homeShadowDistance;

		Sprite* m_tempCard;
		ViewCard()
			:m_angle(0.0f),
			m_card(NULL),
			m_enabled(true),
			m_scale(1.0f),
			m_thrown(false),
			m_selected(false),
			m_homeScale(1.0f),
			m_homeAngle(0.0f),
			m_shadowIntensity(0.0f),
			m_homeShadowIntensity(0.0f),
			m_homeShadowDistance(0.0f),
			m_shadowDistance(0.0f),
			m_toTableOpacity(1.0f),
			m_tempCard(NULL)
		{
		}

		ViewCard(Vec2 position, float angle, float scale, 
			float shadowIntensity, Vec2 shadowNormal, float shadowDistance,
			Sprite* image)
		{
			m_position = position;
			m_angle = angle;
			m_card = image;
			m_scale = scale;
			m_toTableOpacity = 1.0f;
			m_enabled = true;
			m_selected = false;
			m_thrown = false;

			m_shadowIntensity = shadowIntensity;
			m_shadowNormal = shadowNormal;
			m_shadowDistance = shadowDistance;

			m_homeScale = m_scale,
			m_homeAngle = m_angle;
			m_homePos = m_position;
			m_tempCard = NULL;
		}
	};
}

#endif
