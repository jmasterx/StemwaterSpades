#include "Game/CardView/CardAnimations.hpp"
#include "Game/CardView/CardView.hpp"
namespace cge
{
	float anlerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	CardAnimations::CardAnimations( CardAnimationManager* manager)
		: m_manager(manager), m_dispatchShadowIntensity(0.002f),m_imageMan(NULL)
	{
	}


	CardAnimations::~CardAnimations(void)
	{
	}

	void CardAnimations::toggleCard( ViewCard* card )
	{
		if(card->m_selected)
		{
			moveCardDown(card);
		}
		else
		{
			moveCardUp(card);
		}
	}

	void CardAnimations::moveCardUp( ViewCard* card )
	{
		float X = sin((CGE_PI * 2.0f) - card->m_angle) * 0.036f;
		float Y = cos((CGE_PI * 2.0f) - card->m_angle) * 0.036f;

		Vec2 addPos = Vec2(X,Y);
		m_manager->pushAnimationEvent(
			CardMessenger::BEGIN_MOVE_CARD_UP,CardMessenger::END_MOVE_CARD_UP);
		m_manager->addAnimation(
			card,
			ViewCard(card->m_homePos - addPos,
			card->m_angle,
			card->m_scale,
			card->m_shadowIntensity,
			card->m_shadowNormal,
			card->m_shadowDistance,
			NULL),
			0.11f,
			0);

		m_manager->cancelAnimationEvent(card,CardMessenger::END_MOVE_CARD_DOWN);
		card->m_selected = true;
	}

	void CardAnimations::moveCardDown( ViewCard* card )
	{
		m_manager->pushAnimationEvent(
		CardMessenger::BEGIN_MOVE_CARD_DOWN,CardMessenger::END_MOVE_CARD_DOWN);
		m_manager->addAnimation(
			card,
			ViewCard(card->m_homePos,
			card->m_homeAngle,
			card->m_scale,
			card->m_shadowIntensity,
			card->m_shadowNormal,
			card->m_shadowDistance,
			NULL),
			0.1f,
			0);

		card->m_selected = false;

		m_manager->cancelAnimationEvent(card,CardMessenger::END_MOVE_CARD_UP);
	}

	void CardAnimations::handleCardScaleLogic( std::vector<ViewCard*>& cards,
		int cardUnderMouse )
	{
		float cardIncreaseRate = 0.025f;
		float cardDecreaseRate = 0.025f;
		float selectedCardMaxScale = 1.2f;
#ifndef CGE_MOBILE
		if(cardUnderMouse > -1 && cards[cardUnderMouse]->m_enabled)
		{
			float maxScale = selectedCardMaxScale * cards[cardUnderMouse]->m_homeScale;
			if(cards[cardUnderMouse]->m_scale < maxScale)
			{
				cards[cardUnderMouse]->m_scale += cardIncreaseRate;

				if(cards[cardUnderMouse]->m_scale > maxScale)
				{
					cards[cardUnderMouse]->m_scale = maxScale;
				}
			}
		}
#endif

		for(size_t i = 0; i <cards.size(); ++i)
		{
			if((int)i == cardUnderMouse && !(!cards[cardUnderMouse]->m_enabled))
				continue;

			if(cards[i]->m_scale > cards[i]->m_homeScale && !cards[i]->m_selected)
			{
				cards[i]->m_scale -= cardDecreaseRate;

				if(cards[i]->m_scale < cards[i]->m_homeScale)
				{
					cards[i]->m_scale = cards[i]->m_homeScale;
				}
			}
		}
	}

	void CardAnimations::throwCardOnTable( ViewCard* card, int playerNum )
	{
		ViewCard tempCard;
		switch(playerNum)
		{
		case 0: //bottom
			tempCard.m_angle = -3.2f;
			tempCard.m_position = Vec2(0.0f,0.04f);
			break;
		case 1: //left
			tempCard.m_angle = -1.77f;
			tempCard.m_position = Vec2(-0.1f,-0.01f);
			break;
		case 2: //top
			tempCard.m_angle = 0.11f;
			tempCard.m_position = Vec2(0.0f,-0.05f);
			break;
		case 3: //right
			tempCard.m_angle = 1.7f;
			tempCard.m_position = Vec2(0.1f,-0.01f);
			break;

		}

		//randomly add angle
		tempCard.m_angle += -0.05f + ((rand() % 11) / 100.0f);
		tempCard.m_scale = 0.9f;
		tempCard.m_shadowIntensity = 0.9f;
		tempCard.m_shadowDistance = 0.5f;

		m_manager->pushAnimationEvent(
			CardMessenger::BEGIN_THROW_CARD_ON_TABLE,CardMessenger::END_THROW_CARD_ON_TABLE);
		if(playerNum == 0)
		m_manager->addAnimation(card,tempCard,0.045f,0);
		else
		{
			std::vector<CardAnimationChange> changes;
			for(int i = 0; i < 2; i++)
			changes.push_back(CardAnimationChange(i,m_imageMan->getBackCardImage()));
			int flipFrame = 0;
			int maxFrames = 9;
			int numFrames = 13;
			for(int i = 0; i < numFrames; i++)
			{
				float t = (float)i / (float)(numFrames - 1);

				flipFrame = (int)(anlerp(0,maxFrames,t) + 0.5f);
				if(flipFrame > maxFrames)
					flipFrame = maxFrames;
				changes.push_back(CardAnimationChange(2 + i,m_imageMan->getFlipImage(flipFrame)));
			}
			m_manager->addAnimation(card,tempCard,0.045f,0,playerNum > 1,changes);
		}
	}

	void CardAnimations::dispatchCards( std::vector<ViewCard*> player 
		,std::vector<ViewCard*> left, std::vector<ViewCard*> top,
		std::vector<ViewCard*> right, int dealer )
	{
		std::vector<int> ids;
		const int numHands = 4;
		std::vector<ViewCard*> cards[numHands] = {player,left,top,right};

		int numCards = player.size() + left.size() + top.size() + right.size();
		int totalCards = numCards;

		const float rate = 0.04f;
		int numSteps = (int)(1.0f / rate);

		Vec2 cardPos = Vec2(0.0f,0.0f);
		float cardAngle = 0.0f;
		float cardScale = 0.8f;

		m_manager->pushAnimationEvent(
			CardMessenger::BEGIN_DISPATCH_CARDS,CardMessenger::END_DISPATCH_CARDS);

		while(numCards > 0)
		{
			for(int i = 0; i < numHands; ++i)
			{
				int pos = (dealer + i) % numHands;

				if(cards[pos].size() > 0)
				{
					ViewCard* startCard;
					startCard = cards[pos].back();
					startCard->m_position = cardPos;
					startCard->m_angle = cardAngle;
					startCard->m_scale = cardScale;
					ViewCard endCard = 
						ViewCard(
						startCard->m_homePos,
						startCard->m_homeAngle,
						startCard->m_homeScale,
						startCard->m_shadowIntensity,
						startCard->m_shadowNormal,
						startCard->m_shadowDistance,
						NULL);

					//lower intensity so shadow looks correct
					startCard->m_shadowIntensity = m_dispatchShadowIntensity;

					//the top cards should have unrestricted rotation
					bool topAngle = pos == 2;
					m_manager->addAnimation(startCard,endCard,rate,
						numSteps * (totalCards - numCards) * 0.05,
						topAngle);
					
					cards[pos].pop_back();
					numCards--;
				}
			}
		}
	}

	void CardAnimations::sendTableCardsToPlayer( 
		const std::vector<ViewCard*> &cards,int playerNum )
	{
		ViewCard tempCard;
		float dist = 0.3f;
		float lrdist = 0.3f;
		float tdist = -0.25f;
		float yDist = 0.0f;
		float endScale = 0.5f;
		Vec2 pos;
		switch(playerNum)
		{
		case 0: //bottom
			pos = Vec2(0.0f,dist);
			break;
		case 1: //left
			pos = Vec2(-lrdist,yDist);
			break;
		case 2: //top
			pos = Vec2(0.0f,tdist); //top needs more
			break;
		case 3: //right
			pos = Vec2(lrdist,yDist);
			break;
		}

		m_manager->pushAnimationEvent(
			CardMessenger::BEGIN_TABLE_CARDS_TO_PLAYER,
			CardMessenger::END_TABLE_CARDS_TO_PLAYER);

		for(size_t i = 0; i < cards.size(); ++i)
		{
			tempCard = *cards[i];
			tempCard.m_position = pos;
			tempCard.m_scale = endScale;

			m_manager->addAnimation(cards[i],tempCard,0.037f,16);
		}
	}

	void CardAnimations::swapCards( 
		const std::vector<ViewCard*> &cardsA ,
		const std::vector<ViewCard*> &cardsB )
	{
		if(cardsA.size() != cardsB.size() || cardsA.size() == 0)
		{
			return;
		}

		ViewCard tempCard = *cardsA[0];

		float rate = 0.022f;
		m_manager->pushAnimationEvent(
			CardMessenger::BEGIN_SWAP_CARDS,CardMessenger::END_SWAP_CARDS);
		for(size_t i = 0; i < cardsA.size(); ++i)
		{
			tempCard = *cardsB[i];
			cardsA[i]->m_selected = false;
			tempCard.m_position = tempCard.m_homePos;
			tempCard.m_angle = tempCard.m_homeAngle;
			tempCard.m_scale = tempCard.m_homeScale;
			m_manager->addAnimation(cardsA[i],tempCard,rate,0);
		}

		for(size_t i = 0; i < cardsB.size(); ++i)
		{
			tempCard = *cardsA[i];
			cardsB[i]->m_selected = false;
			tempCard.m_position = tempCard.m_homePos;
			tempCard.m_angle =    tempCard.m_homeAngle;
			tempCard.m_scale =    tempCard.m_homeScale;
			m_manager->addAnimation(cardsB[i],tempCard,rate,0);
		}
	}

	void CardAnimations::animateDeckCut( std::vector<ViewCard*>& cards, 
		int cardUnderMouse )
	{
		const float toCenterRate = 0.038f;
		float resultScale = 0.8f;

		ViewCard temp = *cards[cardUnderMouse];
		temp.m_position = Vec2(0.0f,0.0f);
		temp.m_angle = 0.0f;
		temp.m_scale = resultScale;
		temp.m_shadowIntensity = m_dispatchShadowIntensity;

		m_manager->pushAnimationEvent(
			CardMessenger::BEGIN_CUT_DECK,CardMessenger::END_CUT_DECK);
		/*
		m_manager->addAnimation(
			cards[cardUnderMouse],
			temp,
			toCenterRate,
			0);

		int extra = 7;
		for(size_t i = 0; i < cards.size(); ++i)
		{
			cards[i]->m_homeScale = resultScale;
			if(i != cardUnderMouse)
				m_manager->addAnimation
				(cards[i],
				temp,
				toCenterRate,
				(1.0f / toCenterRate) + (i * i * 0.025f) + extra);
		}
*/
	}

	void CardAnimations::shrinkCards( const std::vector<ViewCard*>& cards,
		const std::vector<ViewCard>& startCards)
	{
		m_manager->pushAnimationEvent(
			CardMessenger::BEGIN_SHRINK_HAND,CardMessenger::END_SHRINK_HAND);

		//animate the hand shrinking

		//interpolate from the starting properties to the new generated ones
		for(size_t i = 0; i < cards.size(); ++i)
		{
			ViewCard r = *cards[i];
			cards[i]->m_position = startCards[i].m_position;
			cards[i]->m_angle = startCards[i].m_angle;
			cards[i]->m_scale = startCards[i].m_scale;

			m_manager->addAnimation(
				cards[i],r,0.075f,0);
		}
	}

	void CardAnimations::setCardImageManager( CardImageManager* imgMan )
	{
		m_imageMan = imgMan;
	}

}
