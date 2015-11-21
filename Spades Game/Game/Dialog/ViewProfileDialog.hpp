#ifndef CGE_VIEW_PROFILE_DIALOG_HPP
#define CGE_VIEW_PROFILE_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Avatar/AvatarCache.hpp"
namespace cge
{
	class ViewProfileDialog : public GeneralDialogBox
	{
		Label* m_lblFirstName;
		Label* m_lblLastName;
		Label* m_lblUsername;
		Label* m_lblEmail;
		Label* m_lblMemberSince;
		Label* m_lblNetWorth;
		//Label* m_lblKarma;

		Label* m_lblLifeRating;
		Label* m_lblLifeHighestRating;
		Label* m_lblLifeWins;
		Label* m_lblLifeLosses;
		Label* m_lblLifeGamesCompleted;
		Label* m_lblLifeGamesAbandoned;
		Label* m_lblLifeGamesSubstituted;

		Label* m_lblCurrentRating;
		Label* m_lblCurrentHighestRating;
		Label* m_lblCurrentWins;
		Label* m_lblCurrentLosses;
		Label* m_lblCurrentGamesCompleted;
		Label* m_lblCurrentGamesAbandoned;
		Label* m_lblCurrentGamesSubstituted;
		Label* m_lblCurrentRatingResetCount;
		Label* m_lblCurrentLastRatingResetDate;

		Label* m_lblIndividualFirst;
		Label* m_lblIndividualSecond;
		Label* m_lblIndividualThird;
		Label* m_lblIndividualFourth;
		Label* m_lblCutthroatFirst;
		Label* m_lblCutthroatSecond;
	    Label* m_lblCutthroatThird;
		Label* m_lblPartnershipWon;
		Label* m_lblPartnershipLost;
		Label* m_lblPass2Won;
		Label* m_lblPass2Lost;

		ProfileAvatar* m_profile;
		CachedAvatarImage m_avatarImage;

	public:
		ViewProfileDialog(GuiFactory* factory);
		void setData(
			int avatar, const std::vector<std::string>& data, AvatarCache* cache);

		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		virtual bool wantCloseButton() const;
		virtual ~ViewProfileDialog(void);
	};
};

#endif