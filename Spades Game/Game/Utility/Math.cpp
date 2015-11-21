#include "Game/Utility/Math.hpp"
#include <math.h>

namespace cge
{


	float Math::valueAt( float valueA, float valueB, float t )
	{
		return (1.0f - t) * valueA + t * valueB;
	}

	Vec2 Math::rotateAroundPoint( Vec2 point, Vec2 center, float angle )
	{
		point -= center;
		Vec2 temp = point;
		point.setX((temp.getX() * cos(angle)) - (temp.getY() * sin(angle)));
		point.setY((temp.getX() * sin(angle)) + (temp.getY() * cos(angle)));

		point += center;
		return point;
	}

	float Math::angleFromVector( Vec2 a, Vec2 b )
	{
		return atan2(b.getY() - a.getY(), b.getX() - a.getX()) + (CGE_PI / 2.0f);
	}

	Vec2 Math::positionFromCenterToLineAt( Vec2 center, float dist,
		int total, int current, float angularDistance, float angularOffset ) 
	{
		float startAngle = -((angularDistance * (float)total) / 2.0f);
		float curAngle = startAngle + (angularDistance * (float)current);
		curAngle -= angularOffset;
		curAngle += angularDistance / 2.0f;
		curAngle += CGE_PI;

		Vec2 retVec = center;
		Vec2 angVec = Vec2(sin(curAngle),cos(curAngle));
		angVec *= dist;
		retVec += angVec;

		return retVec;
	}

	Vec2 Math::positionFromCenterToLineAt( Vec2 center, float dist,
		float totalAngular, float angularDistance, float angularOffset )
	{
		float startAngle = -((totalAngular) / 2.0f);
		float curAngle = startAngle + angularDistance;
		curAngle -= angularOffset;
		curAngle += angularDistance / 2.0f;
		curAngle += CGE_PI;

		Vec2 retVec = center;
		Vec2 angVec = Vec2(sin(curAngle),cos(curAngle));
		angVec *= dist;
		retVec += angVec;

		return retVec;
	}

	Vec2 Math::quadraticBezier(const Vec2& a, const Vec2& b, const Vec2& c, float t)
	{
		float x = (a.getX()-2.0f*b.getX()+c.getX()) * t * t  +2.0f * (b.getX()-a.getX()) * t + a.getX();
		float y = (a.getY()-2.0f*b.getY()+c.getY()) * t * t + 2.0f * (b.getY()-a.getY()) * t + a.getY();
		return Vec2(x,y);
	}

	double Math::angleFromWidth( double radius, double width )
	{
		double c = 2.0 * CGE_PI * radius;
		if( c == 0.0)
		{
			return 0.0;
		}
		return  2.0 * CGE_PI * (width / c); 
	}

}