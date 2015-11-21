#include "CardRenderer.hpp"

namespace cge
{
	CardRenderer::~CardRenderer(void)
	{
	}

	void CardRenderer::setCardImageManager( CardImageManager* manager )
	{
		m_cardImageManager = manager;
	}

	void CardRenderer::renderCardHands( 
		GraphicsContext* context, 
		const std::vector<std::vector<ViewCard*> >& cards,
		const std::vector<ViewCard*>& tableCards )
	{
		float scale = m_proportions->getScale();
		
		for(size_t i = 0; i < tableCards.size(); ++i)
		{
			m_tableCard.clear();
			m_tableCard.push_back(tableCards[i]);
			renderCardHand(context,m_tableCard, true,true);
			renderCardHand(context,m_tableCard, false,true);
		}
	
		renderCardHand(context,cards[1],true);
		renderCardHand(context,cards[1],false);
		renderCardHand(context,cards[0],true);
		renderCardHand(context,cards[0],false);
		renderCardHand(context,cards[2],true);
		renderCardHand(context,cards[2],false);
		renderCardHand(context,cards[3],true);
		renderCardHand(context,cards[3],false);
	}

	void CardRenderer::renderCardHand(
		GraphicsContext* context, const std::vector<ViewCard*>& cards, bool shaded, bool tableCards )
	{
		float scale = m_proportions->getScale();

		bool holdIt = shaded || ( cards.size() > 0 && 
			cards[0]->m_card == m_cardImageManager->getBackCardImage());

		//if we are drawing the same bitmap over and over, this speeds it up
		if(holdIt)
			context->holdDrawing();

		for(size_t i = 0; i < cards.size(); ++i)
		{
			Vec2 pos = m_proportions->getCardPosition(cards[i]);
			float shadeScale = shaded ? 0.96f : 1.0f;
			if(shaded)
			{
				pos.setX(pos.getX() + 
					(cards[i]->m_shadowNormal.getX() * cards[i]->m_shadowDistance * m_proportions->getScale()));
				pos.setY(pos.getY() + 
					(cards[i]->m_shadowNormal.getY() * cards[i]->m_shadowDistance * m_proportions->getScale()));
			}

			float xRatio = (m_proportions->getInitialCardSize().getX() / 
				(float)cards[i]->m_card->getWidth());
			float yRatio = (m_proportions->getInitialCardSize().getY() / 
				(float)cards[i]->m_card->getHeight());
			Sprite* img = shaded ? m_cardImageManager->getShadowImage() : cards[i]->m_card;
			Sprite* imgTop = m_cardImageManager->getShadowImage();

			float opacity = shaded ? cards[i]->m_shadowIntensity : 1.0f;
			float gray = shaded ? 0.0f : 1.0f;

			if(cards[i]->m_thrown)
			{
				cards[i]->m_toTableOpacity -= 0.13f;
				if(cards[i]->m_toTableOpacity < 0.0f)
				{
					cards[i]->m_toTableOpacity = 0.0f;
				}
			}

			if(!shaded && !tableCards && cards[i]->m_shadowIntensity > 0.1f)
			{
				context->drawTintedScaledRotatedSprite(
					imgTop,
					Color(0.0f,0.0f,0.0f,opacity * 0.45f * cards[i]->m_toTableOpacity),
					imgTop->getWidth() / 2,
					imgTop->getHeight() / 2,
					pos.getX() -
					(cards[i]->m_shadowNormal.getX() * cards[i]->m_shadowDistance * 0.13f * m_proportions->getScale()),
					pos.getY() + (cards[i]->m_shadowNormal.getX() * cards[i]->m_shadowDistance * 0.075f * m_proportions->getScale()),
					scale * cards[i]->m_scale * xRatio * shadeScale ,
					scale * cards[i]->m_scale * yRatio * shadeScale,
					cards[i]->m_angle,0);
			}

			if(cards[i]->m_enabled || shaded)
				context->drawTintedScaledRotatedSprite(
				img,
				Color(gray,gray,gray,opacity),
				img->getWidth() / 2,
				img->getHeight() / 2,
				pos.getX(),
				pos.getY(),
				scale * cards[i]->m_scale * xRatio * shadeScale ,
				scale * cards[i]->m_scale * yRatio * shadeScale,
				cards[i]->m_angle,0);
			else
				context->drawTintedScaledRotatedSprite(
				img,
				m_disabledColor,
				img->getWidth() / 2,
				img->getHeight() / 2,
				pos.getX(),
				pos.getY(),
				scale * cards[i]->m_scale * xRatio ,
				scale * cards[i]->m_scale * yRatio,
				cards[i]->m_angle,0);
		}

		if(holdIt)
			context->unholdDrawing();

	}

	CardRenderer::CardRenderer()
		: 
		m_proportions(NULL),
		m_cardImageManager(NULL),
		m_disabledColor(0.89f,0.88f,0.88f)
	{
	}

	void CardRenderer::setProportionsManager( ProportionsManager* manager )
	{
		m_proportions = manager;
	}

}
