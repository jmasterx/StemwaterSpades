#include "Game/CardView/CardSettings.hpp"
#include "Game/Platform/Platf.hpp"
namespace cge
{

	float mul = 1.0f;
	static float x = 0.25f;
	float CardSettings::getHandScale( int player )
	{
		return player > 0 ? 0.58f: 1.2f;
	}

	float CardSettings::getHandDistance( int player )
	{
        float mobile = 0.0f;
        if(Platf::isMobile())
            mobile = 0.12f;
		
		//x += 0.01f;
		return player > 0 ? 0.35f : 0.25f + mobile + x;
	}

	Vec2 CardSettings::getHandCenter( int player )
	{
        float mobile = 0.0f;
        if(Platf::isMobile())
            mobile = 0.12f;
        
		switch(player)
		{
		case 0: //South
			return Vec2(0.0f,0.64f + x - 0.09f + mobile);
			break;
		case 1: //West
			return Vec2(-0.7f,0.0f);
			break;
		case 2: //North
			return Vec2(0.0f,-0.65f);
			break;
		case 3: //East
			return Vec2(0.7f,0.0f);
		default: 
			return Vec2(0.0f,0.0f);
			break;
		}
	}

	float CardSettings::getHandAngleOffset( int player )
	{
		switch(player)
		{
		case 0: //South
			return 0.0f;
			break;
		case 1: //West
			return (2.0f * CGE_PI) * 0.25f;
			break;
		case 2: //North
			return (2.0f * CGE_PI) * 0.5f;
			break;
		case 3: //East
			return (2.0f * CGE_PI) * 0.75f;
		default:
			return 0.0f;
		}
	}

	float CardSettings::getHandAngularDistance( int player )
	{
		if(player > 0)
			return getHandScale(player) * 0.09f;
		return 0.1;
	}

	Vec2 CardSettings::getPosCenter( int player )
	{
		switch(player)
		{
		case 0: //South
			return Vec2(0.0f,0.64f);
			break;
		case 1: //West
			return Vec2(-0.7f,0.0f);
			break;
		case 2: //North
			return Vec2(0.0f,-0.65f);
			break;
		case 3: //East
			return Vec2(0.7f,0.0f);
		default: 
			return Vec2(0.0f,0.0f);
			break;
		}
	}

}
