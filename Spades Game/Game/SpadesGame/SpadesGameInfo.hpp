#ifndef CGE_SPADES_GAME_INFO_HPP
#define CGE_SPADES_GAME_INFO_HPP
#include "Game/SpadesGame/SpadesGameMessages.hpp"
#include <vector>
namespace cge
{
	enum JoinPolicy
	{
		NO_ENEMIES_JOIN,
		FRIENDS_ONLY_JOIN,
		ANYONE_JOIN
	};

	struct SpadesGameInfo
	{
		SpadesGameTypeEnum m_type;
		SpadesGameEndConditionEnum m_endCondition;
		SpadesGameMoonShotPolicyEnum m_moonshot;
		SpadesGamePassEnum m_passAmount;
		SpadesGameDeckCutPolicyEnum m_deckCut;
		int m_nilValue;
		int m_endConditionValue;
		bool m_allowWatch;
		JoinPolicy m_joinPolicy;
		bool m_rated;
		bool m_botGame;
		int m_botStrength;
		int m_themeID;
		int m_fee;
		int m_minRating;

		bool isPartnership() const;
		bool isIndividual() const;
		bool isCutthroat() const;
		bool isNilEnabled() const;
		bool isPassTwo() const;
		bool isMoonshotEnabled() const;
		bool isDeckCutEnabled() const;
		bool isMirror() const;
		bool isSuicide() const;
		int getNumPlayers() const;
		int getMaxRoundPoints() const;
		bool isWatchingAllowed() const;
		bool isRated() const;
		bool isSocial() const;
		bool isScoreEnded() const;
		bool isTimeEnded() const;
		bool isHandEnded() const;
		int getEndValue() const;
		JoinPolicy getJoinPolicy() const;
		int getBotStrength() const;
		bool isBotGame() const;
		int getThemeID() const;
		std::vector<int> toIntArray() const;
		void setThemeID(int id);
		int getFee() const;
		int getMinRating() const;
		void setMinRating(int minRating);

		SpadesGameInfo()
			: m_type(INDIVIDUAL), 
			m_endCondition(SCORE_REACHED),
			m_passAmount(NO_PASS),
			m_nilValue(0),
			m_endConditionValue(0),
			m_moonshot(ALLOW_MOON_SHOT),
			m_deckCut(WANT_DECK_CUT),
			m_allowWatch(true),
			m_joinPolicy(NO_ENEMIES_JOIN),
			m_rated(true),m_botGame(false),
			m_botStrength(0),m_themeID(0),m_fee(0),
			m_minRating(0)
		{
		}

		SpadesGameInfo(SpadesGameTypeEnum type,
			SpadesGameEndConditionEnum endCondition, 
			SpadesGameMoonShotPolicyEnum moonshot,
			SpadesGamePassEnum passAmount,
			int nilValue,
			int endConditionValue,
			SpadesGameDeckCutPolicyEnum deckCut,
			bool allowWatch, JoinPolicy joinPolicy, bool rated,
			bool botGame,int botStrength,int themeID,int fee, int minRating
			)
			: m_type(type), m_endCondition(endCondition),
			m_moonshot(moonshot), m_passAmount(passAmount),
			m_nilValue(nilValue),
			m_endConditionValue(endConditionValue),
			m_deckCut(deckCut), m_allowWatch(allowWatch),
			m_joinPolicy(joinPolicy), m_rated(rated),m_botGame(botGame),
			m_botStrength(botStrength),m_themeID(themeID),m_fee(fee),
			m_minRating(minRating)
		{
		}
		SpadesGameInfo(const std::vector<int>& params);
	};
}

#endif
