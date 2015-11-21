#ifndef CGE_CARD_ANIMATION_EVENT_HPP
#define CGE_CARD_ANIMATION_EVENT_HPP
#include "Game/CardView/CardAnimation.hpp"
#include "Game/CardView/CardMessenger.hpp"
namespace cge
{
	class CardAnimationEvent
	{
		CardMessenger* m_messenger;
		std::vector<CardAnimation> m_animations;
		CardMessenger::CardMessage m_message;
		int m_id;
		std::vector<ViewCard*> m_cards;
		bool m_noReturn;
		void cleanup();
		
	public:
		CardAnimationEvent(CardMessenger* messenger, 
			int id, CardMessenger::CardMessage message);
		void step();
		bool isFinished() const;
		int getId() const;
		bool contains(ViewCard* card);
		CardMessenger::CardMessage getMessage() const;
		void add(const CardAnimation& animation);
		ViewCard* getCard() const;
		const std::vector<ViewCard*>& getCards() const;
		virtual ~CardAnimationEvent(void);
	};
}

#endif
