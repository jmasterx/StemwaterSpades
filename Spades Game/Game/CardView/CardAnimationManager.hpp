#ifndef CGE_CARD_ANIMATION_MANAGER_HPP
#define CGE_CARD_ANIMATION_MANAGER_HPP
#include "Game/CardView/CardAnimationEvent.hpp"

namespace cge
{
	class CardAnimationManager
	{
	   std::vector<CardAnimationEvent> m_cardAnims;
	   int m_animationIDCount;
	   CardMessenger* m_messenger;
	public:
		CardAnimationManager(CardMessenger* messenger);
		int pushAnimationEvent(
			CardMessenger::CardMessage beginMessage,
			CardMessenger::CardMessage message);
		void addAnimation(ViewCard* card,
			const ViewCard& end,float rate, int stepDelay,
			bool unrestrictedAngle = false);
		void addAnimation(ViewCard* card,
			const ViewCard& end,float rate, int stepDelay,
			bool unrestrictedAngle, const std::vector<CardAnimationChange>& changes);
		bool cancelAnimationEvent(CardMessenger::CardMessage message);
		bool cancelAnimationEvent(ViewCard* card, CardMessenger::CardMessage message);
		CardAnimationEvent* getAnimationById(int id);
		void stepAnimations();
		bool removeAnimationEvent(int id);
		int getCount() const;
		bool hasAnimationType(CardMessenger::CardMessage msg) const;
		virtual ~CardAnimationManager(void);
	};
}

#endif
