#ifndef CGE_CARD_EVENT_LISTENER_HPP
#define CGE_CARD_EVENT_LISTENER_HPP
#include "Game/CardView/CardMessenger.hpp"
#include "Game/CardView/CardEventListener.hpp"
namespace cge
{
	class CardEventListener
	{
	public:
		virtual void processCardEvent(CardMessenger::CardMessage msg, int val) = 0;
		CardEventListener(void);
		virtual ~CardEventListener(void);
	};
}
#endif
