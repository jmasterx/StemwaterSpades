#ifndef CGE_CLIENT_EVENT_LISTENER_HPP
#define CGE_CLIENT_EVENT_LISTENER_HPP
#include "Game/SpadesGame/SpadesGameMessages.hpp"
#include "Game/Card/Card.hpp"
#include <stdlib.h>
#include <vector>
namespace cge
{
	class ClientEventListener
	{
	public:
		ClientEventListener(void);
		virtual void roundScoresChanged(const std::vector<int>& scores, const std::vector<int> bags) {}
		virtual void activePlayerChanged(int newActivePlayer) {}
		virtual void playerBidChanged(int player, int newBid) {}
		virtual void playerMadeTrick(int player, SpadesPointEnum trickType) {}
		virtual void passTwoCardsChanged(const std::vector<Card>& received, const std::vector<Card>& given) {}
		virtual void lastHandChanged(const std::vector<Card>& lastCards) {}
		virtual void passCardsBegin() {}
		virtual void passButtonEnabledChanged(bool enabled) {}
		virtual void passButtonVisibilityChanged(bool visible) {}
		virtual void bidBegin(int numButtons, bool nilActive) {}
		virtual void roundBegan() {}
		virtual void roundEnded() {} 
		virtual void gameBegin() {}
		virtual void spadesBroken(int playerWhoBroke)  {}
		virtual void playSound(const std::string& sound) {}
		virtual void giftChanged(int player, int giftID, int frame) {}
		virtual void bidStateChanged(const std::vector<SpadesPointEnum> &b, const std::vector<SpadesPointEnum> &l, const std::vector<SpadesPointEnum> &t, const std::vector<SpadesPointEnum> &r, std::vector<int> bids) {}
		virtual void playerFailedNil(int player) {}
		virtual void contractCompleted(int playerCompleter, bool partnership, bool watching) {}
		//inputs
		virtual void trickCountChanged(int bid) {}
		virtual void endPassCards() {} //TODO
		virtual ~ClientEventListener(void);
	};
}

#endif
