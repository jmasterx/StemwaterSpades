#ifndef  CGE_CARD_RENDERER_HPP
#define  CGE_CARD_RENDERER_HPP
#include "Game/CardView/ViewCard.hpp"
#include "Game/Resource/Sprite.hpp"
#include "Game/Engine/GraphicsContext.hpp"
#include "Game/CardView/CardImageManager.hpp"
#include "Game/CardView/ProportionsManager.hpp"
#include <vector>
namespace cge
{
	class CardRenderer
	{
		Color m_disabledColor;
		CardImageManager* m_cardImageManager;
		ProportionsManager* m_proportions;
		std::vector<ViewCard*> m_tableCard;
		void renderCardHand(GraphicsContext* context, 
			const std::vector<ViewCard*>& cards, bool shaded, bool tableCards = false);
	public:
		CardRenderer();
		void renderCardHands(GraphicsContext* g,
			const std::vector<std::vector<ViewCard*> >& cards, 
			const std::vector<ViewCard*>& tableCards);
		void setCardImageManager(CardImageManager* manager);
		void setProportionsManager(ProportionsManager* manager);
		virtual ~CardRenderer(void);
	};
}

#endif
