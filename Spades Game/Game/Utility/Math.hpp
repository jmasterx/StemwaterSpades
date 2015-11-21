#ifndef CGE_MATH_HPP
#define CGE_MATH_HPP
#define CGE_PI 3.1415926f
#include "Game/Utility/Vec2.hpp"
namespace cge
{
	class Math
	{
	public:
		static float valueAt( float valueA, float valueB, float t );
		static Vec2 rotateAroundPoint( Vec2 point, Vec2 center, float angle );
		static float angleFromVector(Vec2 a, Vec2 b);
		static Vec2 positionFromCenterToLineAt(Vec2 center,
			float dist, 
			int total, int current,
			float angularDistance, float angularOffset);
		static double angleFromWidth(double radius, double width);
		static Vec2 positionFromCenterToLineAt(Vec2 center,
			float dist, 
			float totalAngular,
			float angularDistance, float angularOffset);
		static Vec2 quadraticBezier(const Vec2& a, const Vec2& b, const Vec2& c, float t);
	};
}

#endif
