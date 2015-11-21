#ifndef CGE_SWDB_PLAYER_HPP
#define CGE_SWDB_PLAYER_HPP
#include <stdlib.h>
#include "Game/Database/optional.hpp"
#include "Game/Database/DateTime.h"
#include "Game/serverplatform.hpp"
#include <set>
using namespace boost;
namespace cge
{
	struct SWDBPlayer
	{
		int64 id;
		std::string firstName;
		std::string lastName;
		std::string name;
		std::string password;
		std::string email;
		bool emailIsVerified;
		int avatar;
		DateTime joinDate;
		bool isAdministrator;
		int karmaPoints;
		int currentRating;
		int currentWins;
		int currentLoses;
		int currentGamesCompleted;
		int currentGamesAbandoned;
		int currentGamesSubstituted;
		int currentHighestRating;
		int lifetimeRating;
		int lifetimeWins;
		int lifetimeLoses;
		int lifetimeGamesCompleted;
		int lifetimeGamesAbandoned;
		int lifetimeGamesSubstituted;
		int lifetimeHighestRating;
		optional<DateTime> lastRatingResetDate;
		int ratingResetCount;
		int firstPlaceSingleCount;
		int secondPlaceSingleCount;
		int thirdPlaceSingleCount;
		int fourthPlaceSingleCount;
		int firstPlaceCutthroatCount;
		int secondPlaceCutthroatCount;
		int thirdPlaceCutthroatCount;
		int partnershipWinCount;
		int partnershipLossCount;
		int pass2WinCount;
		int pass2LossCount;
		optional<DateTime> chatSuspendEndDate;
		optional<DateTime> lastLoginDate;
		optional<DateTime> lastGameCompletedDate;
		optional<DateTime> suspendedEndDate;
		bool chatIsDisabled;
		bool isDisabled;
		int loginCount;
		int loginAttemptCount;
		bool acceptedTerms;
		bool showEmail;
		int theme;
		int64 coins;
		int64 tempCoins;
		DateTime lastCoinReceiveDate;
		std::set<std::string> friends;
		bool isFriend(const std::string& name) const;
		SWDBPlayer(void);
		~SWDBPlayer(void);
	};
}

#endif
