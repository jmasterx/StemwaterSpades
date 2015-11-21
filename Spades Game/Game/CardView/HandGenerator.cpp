#include "Game/CardView/HandGenerator.hpp"
#include "Game/platform.hpp"
#include "Game/Utility/Math.hpp"
#include "Game/CardView/CardSettings.hpp"
namespace cge
{
	HandGenerator::HandGenerator( ViewCardContainer* container )
		: m_container(container), m_shadowIntensity(0.84f), m_shadowDistance(55.0f),
		m_shadowNormal(0.75f,0.65f)

	{

	}

	HandGenerator::~HandGenerator(void)
	{
	}

	void HandGenerator::generateHand( int playerNum, int numCards, 
		std::vector<ViewCard*>& cards ) const
	{
		playerNum %= 4;
		float dist =  CardSettings::getHandDistance(playerNum);
		float scale = CardSettings::getHandScale(playerNum);
		float angDist = CardSettings::getHandAngularDistance(playerNum);

		Vec2 center = CardSettings::getHandCenter(playerNum);
		float angOffset = CardSettings::getHandAngleOffset(playerNum);

		//release the pointers
		releaseCards(cards);

		Vec2 pos;
		float ang = 0.0f;
		ViewCard* curCard = NULL;
		for(int i = numCards - 1; i >= 0; --i)
		{
			pos = Math::positionFromCenterToLineAt(
				center,dist,numCards,i,angDist,angOffset);
			ang = Math::angleFromVector(center,pos);
			ang += 0.022f;

			curCard = m_container->request();
			*curCard = ViewCard(pos,ang,scale,
				playerNum == 1 ? m_shadowIntensity * 0.94f : m_shadowIntensity,
				m_shadowNormal,playerNum != 1 ? m_shadowDistance : m_shadowDistance * 0.85f,NULL);
			cards.push_back(curCard);
		}
	}

	void HandGenerator::releaseCards( std::vector<ViewCard*>& cards ) const
	{
		for(size_t i = 0; i < cards.size(); ++i)
		{
			m_container->release(cards[i]);
		}

		cards.clear();
	}

	void HandGenerator::generateDeck( std::vector<ViewCard*>& cards ) const
	{
		float dist =  0.4f;
		float scale = 0.65f;
		float angDist = 0.12f * scale;
		int numCards = 52;

		Vec2 center;
		center.setY(0.05f);

		//release the pointers
		releaseCards(cards);

		Vec2 pos;
		float ang = 0.0f;
		ViewCard* curCard = NULL;
		for(int i = numCards - 1; i >= 0; --i)
		{
			pos = Math::positionFromCenterToLineAt(
				center,dist,numCards,i,angDist,0.0f);
			ang = Math::angleFromVector(center,pos);
			curCard = m_container->request();
			*curCard = ViewCard(pos,ang,scale, 
				m_shadowIntensity, m_shadowNormal, m_shadowDistance,NULL);
			cards.push_back(curCard);
		}
	}


}
