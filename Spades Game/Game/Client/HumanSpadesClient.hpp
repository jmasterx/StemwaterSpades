#ifndef CGE_HUMAN_SPADES_CLIENT_HPP
#define CGE_HUMAN_SPADES_CLIENT_HPP
#include "Game/Client/SpadesClient.hpp"
#include "Game/CardView/CardView.hpp"
#include "Game/CardView/CardEventListener.hpp"
#include "Game/Handler/ClientEventListener.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Game/SpadesGame/GameClientState.hpp"
#include "Game/SpadesGame/SpadesGameMessage.hpp"
namespace cge
{
	class HumanSpadesClient : public SpadesClient, public CardEventListener,
		public ClientEventListener
	{
	public:
		enum StateEnum
		{
			WAITING_FOR_SERVER_INPUT,
			WAITING_FOR_DECK_CUT_POS,
			WAITING_FOR_CARDS_DISPATCHED,
			WAITING_FOR_TRICK_COUNT,
			WAITING_FOR_PLAYER_SELECT_SWAP_CARDS,
			WAITING_FOR_PLAYER_SELECT_CARD,
			WAITING_FOR_CARD_ON_TABLE,
			WAITING_FOR_CARD_TO_PLAYER,
			WAITING_FOR_PERFORM_SWAP,
			WAITING_FOR_GIVE_POINT
		};
	private:	
		GameClientState m_restoreState;
		bool m_watching;
		std::queue<std::pair<Card,int> > m_cardsToThrow;
		std::queue<SpadesGameMessage> m_qMsg;
		bool m_restoring;
		StateEnum m_state;
		CardView* m_view;
		int m_deckCutPos;
		int m_dealer;
		int m_active;
		int m_numPerformPasses;
		int m_lastPointReceiver;
		bool m_spadesBroken;
		bool m_canQueueMsgs;
		SpadesPointEnum m_lastPointType;	
		std::vector<int> m_lastHand;
		int m_firstPlayer;
		std::vector<ClientEventListener*> m_listeners;
		void setActivePlayer(int active);
		void setDealer(int dealer);
		virtual void processMessageFromGame(SpadesCSMessageEnum msg,
			const std::vector<int>& args);
		virtual void processCardEvent(CardMessenger::CardMessage msg, int val);
		void setState(StateEnum state);
		StateEnum getState() const;

		void appendLastHand(const Card& card, int pos);
		bool lastHandEmpty() const;
		void clearLastHand();
		std::vector<Card> generateLastHand();
	
		void msgRequestGameInit(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void doReplyGameInit();
		void msgRoundBegin();
		void msgRequestDeckCutPos(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void msgDispatchCards(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void msgRequestTrickCount(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void msgGotTrickCount(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void msgRequestCardSwap(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void msgPerformSwap(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void msgPerformCardOnTable(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void msgRequestSelectedCard(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void msgPlayerMadeTrick(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void msgFilterCards(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void msgSendActivePlayer(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void msgSendRoundScores(SpadesCSMessageEnum msg,const std::vector<int>& args);
		void enablePassButton(bool enabled);
		void setSwapCards(const std::vector<Card>& incoming,const std::vector<Card>& outgoing);
		void sendSelectedCard();
		void addTrickToPlayer(int player, SpadesPointEnum type);

		void throwQueuedCard();
		bool hasCardsToThrow() const;
		void processQueuedMessages();
		void queueMessage(SpadesCSMessageEnum msg,
			const std::vector<int>& args);

		void generateThrowCards(std::vector<int> cards);

		virtual void sendMessageToGame(SpadesCSMessageEnum msg,
			const std::vector<int>& args);	

		void dispatchSound(const std::string& sound);
	public:
		HumanSpadesClient();
		void setView(CardView* view);
		void addListener(ClientEventListener* listener);
		void removeListener(ClientEventListener* listener);
		virtual void endPassCards();
		virtual void trickCountChanged(int bid);

		virtual void restoreState(const GameClientState& state);
		void finishRestoreState();

		void beginGame();
		bool isWatching() const;
		void logic();

		bool hasItemsQueued() const;
		void dispatchNilFailed(int player);
		void dispatchContractCompleted(int playerCompleter, bool partnership);
		virtual ~HumanSpadesClient(void);
	};
}

#endif
