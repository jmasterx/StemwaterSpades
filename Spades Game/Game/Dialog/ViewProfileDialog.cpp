#include "Game/Dialog/ViewProfileDialog.hpp"
#include "Game/Utility/StringUtil.hpp"
namespace cge
{
	ViewProfileDialog::~ViewProfileDialog(void)
	{
	}

	ViewProfileDialog::ViewProfileDialog( GuiFactory* factory )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),factory->getFontManager())
	{
		addWhitespace(5);
		m_profile = createProfileAvatar();
        if(!Platf::isRetina())
		m_profile->setSize(200,200);
        else
        m_profile->setSize(200 * 2.0f,200 * 2.0f);
		addWhitespace(5);

		addLabel("profile.basic.information");
		m_lblFirstName = createLabel("first.name","");
		m_lblLastName = createLabel("last.name","");
		m_lblUsername = createLabel("username","");
		m_lblEmail = createLabel("email","");
		m_lblMemberSince = createLabel("profile.member.since","");
		m_lblNetWorth = createLabel("profile.net.worth","");
		//m_lblKarma = createLabel("profile.karma.points","");

		addLabel("profile.lifetime.stats");
		m_lblLifeRating = createLabel("profile.rating","");
		m_lblLifeHighestRating = createLabel("profile.highest.rating","");
		m_lblLifeWins = createLabel("profile.wins","");
		m_lblLifeLosses = createLabel("profile.losses","");
		m_lblLifeGamesCompleted = createLabel("profile.games.completed","");
		m_lblLifeGamesAbandoned = createLabel("profile.games.abandoned","");
		m_lblLifeGamesSubstituted = createLabel("profile.games.substituted","");

		addLabel("profile.current.stats");
		m_lblCurrentRating = createLabel("profile.rating","");
		m_lblCurrentHighestRating = createLabel("profile.highest.rating","");
		m_lblCurrentWins = createLabel("profile.wins","");
		m_lblCurrentLosses = createLabel("profile.losses","");
		m_lblCurrentGamesCompleted = createLabel("profile.games.completed","");
		m_lblCurrentGamesAbandoned = createLabel("profile.games.abandoned","");
		m_lblCurrentGamesSubstituted = createLabel("profile.games.substituted","");
		m_lblCurrentRatingResetCount = createLabel("profile.rating.reset.count","");
		m_lblCurrentLastRatingResetDate = createLabel("profile.last.rating.reset","");

		addLabel("profile.detailed.partners");
		m_lblPartnershipWon = createLabel("profile.partnership.won","");
		m_lblPartnershipLost = createLabel("profile.partnership.lost","");
		m_lblPass2Won = createLabel("profile.pass2.won","");
		m_lblPass2Lost = createLabel("profile.pass2.lost","");

		addLabel("profile.detailed.individial");
		m_lblIndividualFirst = createLabel("profile.individual.first","");
		m_lblIndividualSecond = createLabel("profile.individual.second","");
		m_lblIndividualThird = createLabel("profile.individual.third","");
		m_lblIndividualFourth = createLabel("profile.individual.fourth","");
		m_lblCutthroatFirst = createLabel("profile.cutthroat.first","");
		m_lblCutthroatSecond = createLabel("profile.cutthroat.second","");
		m_lblCutthroatThird = createLabel("profile.cutthroat.third","");
	}

	void ViewProfileDialog::setData( int avatar, const std::vector<std::string>& data, AvatarCache* cache )
	{
		setUnlocalizedTitle(data[2]);

		if(data[0].length() > 0)
			m_lblFirstName->setText(data[0]);
		else
			m_lblFirstName->setText(getFactory()->getLanguageManager()->getElement("profile.hidden"));

		if(data[1].length() > 0)
			m_lblLastName->setText(data[1]);
		else
			m_lblLastName->setText(getFactory()->getLanguageManager()->getElement("profile.hidden"));

		m_lblUsername->setText(data[2]);

		if(data[3].length() > 0)
			m_lblEmail->setText(data[3]);
		else
			m_lblEmail->setText(getFactory()->getLanguageManager()->getElement("profile.hidden"));

		int start = 4;
		m_lblMemberSince->setText(data[start++]);
		m_lblNetWorth->setText(getFactory()->getLanguageManager()->getElementWithArg("n.coins",StringUtil::commaSeparate(data[start++])));
		start++;//m_lblKarma->setText(data[start++]);
		m_lblLifeRating->setText(data[start++]);
		m_lblLifeHighestRating->setText(data[start++]);
		m_lblLifeWins->setText(data[start++]);
		m_lblLifeLosses->setText(data[start++]);
		m_lblLifeGamesCompleted->setText(data[start++]);
		m_lblLifeGamesAbandoned->setText(data[start++]);
		m_lblLifeGamesSubstituted->setText(data[start++]);

		m_lblCurrentRating->setText(data[start++]);
		m_lblCurrentHighestRating->setText(data[start++]);
		m_lblCurrentWins->setText(data[start++]);
		m_lblCurrentLosses->setText(data[start++]);
		m_lblCurrentGamesCompleted->setText(data[start++]);
		m_lblCurrentGamesAbandoned->setText(data[start++]);
		m_lblCurrentGamesSubstituted->setText(data[start++]);
		m_lblCurrentRatingResetCount->setText(data[start++]);
		m_lblCurrentLastRatingResetDate->setText(data[start++]);

		m_lblIndividualFirst->setText(data[start++]);
		m_lblIndividualSecond->setText(data[start++]);
		m_lblIndividualThird->setText(data[start++]);
		m_lblIndividualFourth->setText(data[start++]);

		m_lblCutthroatFirst->setText(data[start++]);
		m_lblCutthroatSecond->setText(data[start++]);
		m_lblCutthroatThird->setText(data[start++]);

		m_lblPartnershipWon->setText(data[start++]);
		m_lblPartnershipLost->setText(data[start++]);
		m_lblPass2Won->setText(data[start++]);
		m_lblPass2Lost->setText(data[start++]);

		if(cache->avatarExists(avatar))
		{
			std::map<int,CachedAvatarImage>* images = cache->getCachedAvatars();
			m_avatarImage = (*images)[avatar];
			m_profile->setInside(&m_avatarImage.guiImg);
		}
	}

	void ViewProfileDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		hideGeneralFrame();
	}

	bool ViewProfileDialog::wantCloseButton() const
	{
		return false;
	}

}
