#ifndef CGE_CARD_MESSENGER_HPP
#define CGE_CARD_MESSENGER_HPP
#include <stdlib.h>
#include "Game/Card/Card.hpp"
#include <queue>
namespace cge
{
	class CardEventListener;
	class CardMessenger
	{
	public:
		enum CardMessage 
		{
			BEGIN_ANIMATED_EVENT,
			END_ANIMATED_EVENT,
			BEGIN_SILENT_EVENT,
			END_SILENT_EVENT,
			BEGIN_INIT_ROUND,
			END_INIT_ROUND,
			BEGIN_CUT_DECK,
			END_CUT_DECK,
			DECK_CUT_SELECTED,
			BEGIN_DISPATCH_CARDS,
			END_DISPATCH_CARDS,
			BEGIN_THROW_CARD_ON_TABLE,
			END_THROW_CARD_ON_TABLE,
			BEGIN_TABLE_CARDS_TO_PLAYER,
			END_TABLE_CARDS_TO_PLAYER,
			BEGIN_SWAP_CARDS,
			END_SWAP_CARDS,
			BEGIN_MOVE_CARD_UP,
			END_MOVE_CARD_UP,
			BEGIN_MOVE_CARD_DOWN,
			END_MOVE_CARD_DOWN,
			BEGIN_SHRINK_HAND,
			END_SHRINK_HAND,
			CARD_SELECTED
		};
	private:
		std::queue<std::pair<CardMessage,int> > m_q;
		std::vector<CardEventListener*> m_listeners;
	public:
		CardMessenger(void);
		virtual void sendMessage(CardMessage message, int val = 0);
		virtual void processMessages();
		virtual void processMessage(CardMessage message, int val) = 0;
		void addListener(CardEventListener* listener);
		void removeListener(CardEventListener* listener);
	
		virtual ~CardMessenger(void);
	};
}

#endif
