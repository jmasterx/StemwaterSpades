#ifndef CGE_CARD_SETTINGS_HPP
#define CGE_CARD_SETTINGS_HPP
#include "Game/Utility/Math.hpp"
namespace cge
{
	class CardSettings
	{
	public:
		static float getHandScale(int player);
		static float getHandDistance(int player);
		static Vec2 getPosCenter(int player);
		static Vec2 getHandCenter(int player);
		static float getHandAngleOffset(int player);
		static float getHandAngularDistance(int player);
	};
}

#endif
