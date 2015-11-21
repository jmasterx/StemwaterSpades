#ifndef CGE_CARD_ANIMATIONS_HPP
#define CGE_CARD_ANIMATIONS_HPP
#include "Game/CardView/ViewCard.hpp"
#include "Game/CardView/CardAnimationManager.hpp"
#include "Game/CardView/CardImageManager.hpp"
#include <cmath>
#include <vector>
namespace cge
{
	class CardAnimations
	{
		CardAnimationManager* m_manager;
		CardImageManager* m_imageMan;
		float m_dispatchShadowIntensity;
	public:
		CardAnimations(CardAnimationManager* manager);
		void toggleCard(ViewCard* card);
		void moveCardUp(ViewCard* card);
		void moveCardDown(ViewCard* card);
		void throwCardOnTable(ViewCard* card, int playerNum);
		void handleCardScaleLogic(std::vector<ViewCard*>& cards, int cardUnderMouse);
		void dispatchCards(
			std::vector<ViewCard*> player
			,std::vector<ViewCard*> left,
			std::vector<ViewCard*> top,
			std::vector<ViewCard*> right,
			int dealer);
		void sendTableCardsToPlayer(const std::vector<ViewCard*> &cards,int playerNum);
		void swapCards(const std::vector<ViewCard*> &cardsA
			,const std::vector<ViewCard*> &cardsB);
		void animateDeckCut(std::vector<ViewCard*>& cards, int cardUnderMouse);
		void shrinkCards(const std::vector<ViewCard*>& cards,
			const std::vector<ViewCard>& startCards);
		void setCardImageManager(CardImageManager* imgMan);

		virtual ~CardAnimations(void);
	};
}

#endif