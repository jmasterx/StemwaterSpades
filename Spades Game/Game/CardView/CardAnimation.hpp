#ifndef CGE_CARD_ANIMATOR_HPP
#define CGE_CARD_ANIMATOR_HPP

#include "Game/CardView/ViewCard.hpp"
#include <vector>
namespace cge
{
	struct CardAnimationChange
	{
		int frame;
		Sprite* card;
		CardAnimationChange()
			: frame(0),card(NULL)
		{

		}
		CardAnimationChange(int theFrame, Sprite* theCard)
			: frame(theFrame),card(theCard)
		{

		}
	};
	class CardAnimation
	{
		ViewCard* m_targetCard;
		ViewCard m_startCard;
		ViewCard m_endCard;
		float m_interval;
		float m_rate;
		int m_stepDelay;
		bool m_unrestrictedAngle;
		int m_frames;
		std::vector<CardAnimationChange> m_change;
	public:
		CardAnimation(
			ViewCard* targetCard,
			const ViewCard& startCard,
			const ViewCard& endCard,
			float rate,
			int stepDelay = 0,
			bool unrestrictedAngle = false);
		void step();
		float getInterval() const;
		ViewCard* getCard() const;
		void setChanges(const std::vector<CardAnimationChange>& changes);
		virtual ~CardAnimation(void);
	};
}

#endif
