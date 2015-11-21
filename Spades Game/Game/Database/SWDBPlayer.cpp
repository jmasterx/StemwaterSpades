#include "Game/Database/SWDBPlayer.hpp"
#include <algorithm>
namespace cge
{
	SWDBPlayer::SWDBPlayer(void)
	{
	}

	SWDBPlayer::~SWDBPlayer(void)
	{
	}

	bool SWDBPlayer::isFriend( const std::string& name ) const
	{
		return friends.find(name) != friends.end();
	}

}
