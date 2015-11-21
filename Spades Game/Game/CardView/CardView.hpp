#ifndef CGE_CARD_VIEW_HPP
#define CGE_CARD_VIEW_HPP
#include "Game/Engine/GraphicsContext.hpp"
#include "Game/Utility/Vec2.hpp"
#include "Game/Utility/Math.hpp"
#include "Game/Card/Card.hpp"
#include "Game/CardView/ViewCard.hpp"
#include "Game/CardView/CardMessenger.hpp"
#include "Game/CardView/CardAnimation.hpp"
#include "Game/CardView/CardAnimations.hpp"
#include "Game/CardView/HandGenerator.hpp"
#include "Game/CardView/ViewCardContainer.hpp"
#include "Game/CardView/CardImageManager.hpp"
#include "Game/CardView/CardRenderer.hpp"
#include "Game/CardView/ProportionsManager.hpp"
#include "Game/Element/DynamicElement.hpp"
#include "Game/Engine/AudioManager.hpp"

#include <vector>
#include <queue>
namespace cge
{
	class CardView : public CardMessenger, public DynamicElement
	{
	public:
		//do not modify the order
		enum CardHandEnum
		{
			PLAYER,
			LEFT,
			TOP,
			RIGHT
		};
	private:
		Vec2 m_lastMousePos;
		bool m_mouseEnabled;
		bool m_isDeckCutEvt;
		bool m_singleSelectionBehaviour;
		int m_maxSelectedCards;
		int m_timeUntilNextClick;
		int m_mouseClickInterval;
		bool m_mouseDisabled;
		bool m_waitingToThrowCard;
		CardAnimationManager m_animationManager;
		CardAnimations m_cardAnimations;
		HandGenerator m_handGenerator;
		ViewCardContainer m_cardContainer;
		CardImageManager* m_cardImageManager;
		CardRenderer m_cardRenderer;
		std::vector<std::vector<ViewCard*> > m_cards;
		std::vector<ViewCard*> m_tableCards;
		std::vector<Card> m_playerCards;
		std::vector<int> m_disabledCards;
		std::vector<CardEventListener*> m_cardEventListeners;
		AudioManager* m_audioMan;
		int m_cardUnderMouse;
		bool m_waitSelectCard;

		void renderCardHand(const std::vector<ViewCard*> &cards,GraphicsContext* context);
		bool pointInRect(float x, float y, float w, float h, Vec2 p) const;

		void assignCardImages(CardHandEnum player);

		void handleThrowCardEndLogic(int id);
		void handleSendTableCardsLogic(int id);
		void handleSwapCardsLogic(int id);
		void handleSwapCardsEndLogic();
		void handleEnableMouse(CardMessenger::CardMessage msg, int val);
		void handleDeckCutLogic();
		void handleRedisableCards(CardMessenger::CardMessage msg);
		void handleClickLogic(int x, int y);
		void interpolatedSwap(const std::vector<ViewCard*> &cardsA
			,const std::vector<ViewCard*> &cardsB);

		bool isMouseEnabled() const;

		void disableDisabledCards();
		
		void verifyDisabledCards();
	public:
		virtual void processMessage(CardMessenger::CardMessage message,int val);
		void setSingleSelectionBehaviour(bool singleSelection);
		bool isSingleSelectionBehaviour() const;
	    void evtDispatchCards(std::vector<Card> playerCards, 
			int leftCards, int rightCards, int topCards, CardHandEnum dealer);
		bool evtThrowCardOnTable(const Card& card,CardHandEnum player, int cardNum);
		void evtSendTableCardsToPlayer(CardHandEnum player);
		void evtSelectDeckCut();
		void evtSwapCards(
			const std::vector<Card>& aCards,
			const std::vector<Card>& bCards,
			CardHandEnum handA, CardHandEnum handB);
		int getNumSelectedCards() const;
		void setMaxSelectedCards(int max);
		int getSelectedCardIndex() const;
		std::vector<int> getSelectedCardIndexes() const;
		const Card getSelectedCard() const;
		std::vector<Card> getSelectedCards() const;
		void mouseMove(int x, int y);
		void mouseClick(int x, int y, bool lmb);
		void logic();
		bool pointOnCard(const ViewCard* card,int x, int y) const;
		void setCardImageManager(CardImageManager* cardImageManager);
		void setDisabledCards(const std::vector<int> cards);
		void disableAllCards();
		void disableMouseInput();
		void enableMouseInput();
		void reset();
		CardView(void);
		void render(GraphicsContext* context);
		virtual void setProportions(ProportionsManager* proportions);
		int getIndexOfCard(const Card& c);
		void selectCard(int index);
		void disableUnselectedCards();
		void setMouseDisabled(bool disabled);
		bool isAnimationQueueEmpty() const;
		void setWaitingToThroward(bool waiting);
		bool isOnlyAnimationShrink() const;
		int selectRandomValidCard();
		void playRandomCard();
		void setCardUnderMouse(int x, int y);
		void setAudioManager(AudioManager* audio);
		void setWaitForSelectCard(bool waiting);
		virtual ~CardView(void);
	};

}

#endif
