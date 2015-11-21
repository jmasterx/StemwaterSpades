#include "Game/SpadesGame/SpadesGameInfo.hpp"

namespace cge
{

	bool SpadesGameInfo::isPartnership() const
	{
		return 
			m_type == PARTNERSHIP ||
			m_type == PARTNERSHIP_MIRROR ||
			m_type == PARTNERSHIP_SUICIDE;
	}

	bool SpadesGameInfo::isIndividual() const
	{
		return m_type == INDIVIDUAL || m_type == INDIVIDUAL_MIRROR;
	}

	bool SpadesGameInfo::isNilEnabled() const
	{
		return m_nilValue > 0;
	}

	bool SpadesGameInfo::isMoonshotEnabled() const
	{
		return m_moonshot == ALLOW_MOON_SHOT;
	}

	bool SpadesGameInfo::isDeckCutEnabled() const
	{
		return m_deckCut == WANT_DECK_CUT;
	}

	bool SpadesGameInfo::isMirror() const
	{
		return m_type == INDIVIDUAL_MIRROR ||
				m_type == PARTNERSHIP_MIRROR ||
				m_type == CUTTHROAT_MIRROR;
				
	}

	bool SpadesGameInfo::isSuicide() const
	{
		return m_type == PARTNERSHIP_SUICIDE;
	}

	bool SpadesGameInfo::isCutthroat() const
	{
		return m_type == CUTTHROAT || m_type == CUTTHROAT_MIRROR;
	}

	int SpadesGameInfo::getNumPlayers() const
	{
		if(m_type == CUTTHROAT || m_type == CUTTHROAT_MIRROR)
		{
			return 3;
		}
		else
		{
			return 4;
		}
	}

	bool SpadesGameInfo::isPassTwo() const
	{
		return m_passAmount == PASS_TWO && isPartnership();
	}

	int SpadesGameInfo::getMaxRoundPoints() const
	{
		if(isCutthroat())
		{
			return 17;
		}

		return 13;
	}

	std::vector<int> SpadesGameInfo::toIntArray() const
	{
		std::vector<int> info;
		info.push_back((int)m_type);
		info.push_back((int)m_endCondition);
		info.push_back((int)m_endConditionValue);
		info.push_back((int)m_passAmount);
		info.push_back((int)m_moonshot);
		info.push_back((int)m_nilValue);
		info.push_back((int)m_deckCut);
		info.push_back((int)m_allowWatch);
		info.push_back((int)m_joinPolicy);
		info.push_back((int)m_rated);
		info.push_back((int)m_botGame);
		info.push_back((int)m_botStrength);
		info.push_back(m_themeID);
		info.push_back(m_fee);
		info.push_back(m_minRating);

		return info;
	}

	bool SpadesGameInfo::isWatchingAllowed() const
	{
		return m_allowWatch;
	}

	cge::JoinPolicy SpadesGameInfo::getJoinPolicy() const
	{
		return m_joinPolicy;
	}

	SpadesGameInfo::SpadesGameInfo( const std::vector<int>& params )
		: m_type((SpadesGameTypeEnum)params[0]),
		m_endCondition((SpadesGameEndConditionEnum)params[1]),
		m_moonshot((SpadesGameMoonShotPolicyEnum)params[4]), 
		m_passAmount((SpadesGamePassEnum)params[3]),
		m_nilValue(params[5]),
		m_endConditionValue(params[2]),
		m_deckCut((SpadesGameDeckCutPolicyEnum)params[6]),
		m_allowWatch(params[7] == 0 ? false : true),
		m_joinPolicy((JoinPolicy)params[8]),
		m_rated(params[9] == 0 ? false : true),
		m_botGame(params[10] == 0 ? false : true),
		m_botStrength(params[11]),m_themeID(params[12]),m_fee(params[13]),
		m_minRating(params[14])
	{

	}

	bool SpadesGameInfo::isRated() const
	{
		return m_rated;
	}

	bool SpadesGameInfo::isSocial() const
	{
		return !m_rated;
	}

	bool SpadesGameInfo::isScoreEnded() const
	{
		return m_endCondition == SCORE_REACHED;
	}

	bool SpadesGameInfo::isTimeEnded() const
	{
		return m_endCondition == TIME_OUT;
	}

	bool SpadesGameInfo::isHandEnded() const
	{
		return m_endCondition == HAND_LIMIT_REACHED;
	}

	int SpadesGameInfo::getEndValue() const
	{
		return m_endConditionValue;
	}

	int SpadesGameInfo::getBotStrength() const
	{
		return m_botStrength;
	}

	bool SpadesGameInfo::isBotGame() const
	{
		return m_botGame;
	}

	int SpadesGameInfo::getThemeID() const
	{
		return m_themeID;
	}

	void SpadesGameInfo::setThemeID( int id )
	{
		m_themeID = id;
	}

	int SpadesGameInfo::getFee() const
	{
		return m_fee;
	}

	int SpadesGameInfo::getMinRating() const
	{
		return m_minRating;
	}

	void SpadesGameInfo::setMinRating( int minRating )
	{
		m_minRating = minRating;
	}

}
