#include "Game/Client/HumanSpadesClient.hpp"
#include <iostream>
namespace cge
{
	HumanSpadesClient::HumanSpadesClient()
		: m_view(NULL) //must be set
		, m_deckCutPos(0), m_state(WAITING_FOR_SERVER_INPUT),
		m_active(0), m_dealer(0), m_numPerformPasses(0),
		m_lastPointReceiver(0), m_lastPointType(NOT_A_POINT),
		m_spadesBroken(false),m_restoring(false),m_firstPlayer(-1),
		m_watching(false),m_canQueueMsgs(true)
	{
		m_lastHand.resize(4,-1);
	}

	HumanSpadesClient::~HumanSpadesClient(void)
	{
	}
	
	void HumanSpadesClient::processMessageFromGame( 
		SpadesCSMessageEnum msg, 
		const std::vector<int>& args )
	{
		if(m_watching && !m_view->isAnimationQueueEmpty() && m_canQueueMsgs)
		{
			queueMessage(msg,args);
			return;
		}

		switch(msg)
		{
		case SEND_GAME_INFO_D_S:
			setGameInfo(args);
			break;
		case ROUND_BEGIN_S:
			msgRoundBegin();
			break;
		case REQUEST_GAME_INIT_S:
			msgRequestGameInit(msg,args);
			break;
		case REQUEST_DECK_CUT_POS_S:
			msgRequestDeckCutPos(msg,args);
			break;
		case DISPATCH_CARDS_A_D_S:
			msgDispatchCards(msg,args);
			break;
		case REQUEST_PLAYER_TRICK_COUNT_S:
			msgRequestTrickCount(msg,args);
			break;
		case SEND_PLAYER_TRICK_COUNT_D_S:
			msgGotTrickCount(msg,args);
			break;
		case REQUEST_CARD_SWAP_S:
			msgRequestCardSwap(msg,args);
			break;
		case PERFORM_SWAP_S:
			msgPerformSwap(msg,args);
			break;
		case PERFORM_CARD_ON_TABLE_A_D_S:
			msgPerformCardOnTable(msg,args);
			break;
		case REQUEST_SELECTED_CARD_S:
			msgRequestSelectedCard(msg,args);
			break;
		case PLAYER_MADE_TRICK_A_D_S:
			msgPlayerMadeTrick(msg,args);
			break;
		case FILTER_CARDS_S:
			msgFilterCards(msg,args);
			break;
		case SEND_ACTIVE_PLAYER_S:
			msgSendActivePlayer(msg,args);
			break;
		case SEND_ROUND_SCORES_D_S:
			msgSendRoundScores(msg,args);
			break;
		}
	}

	void HumanSpadesClient::processCardEvent( CardMessenger::CardMessage msg,
		int val )
	{
		switch(msg)
		{
		case CardMessenger::DECK_CUT_SELECTED:
			m_deckCutPos = val;
			break;
#ifdef CGE_MOBILE
		case CardMessenger::CARD_SELECTED:
			{
				if(getState() == WAITING_FOR_PLAYER_SELECT_CARD)
				sendSelectedCard();
			}
			break;
#endif
		case CardMessenger::END_CUT_DECK:
			{
				std::vector<int> deckCut(1,m_deckCutPos);
				sendMessageToGame(SEND_DECK_CUT_POS_C,deckCut);
			}
			break;
		case CardMessenger::BEGIN_THROW_CARD_ON_TABLE:
			{
				std::vector<int> eArgs;
				if(rand() % 2 == 0)
				dispatchSound("play.card");
				else
					dispatchSound("play.card2");
				m_view->setDisabledCards(eArgs);
			}
			break;
		case CardMessenger::BEGIN_DISPATCH_CARDS:
			dispatchSound("shuffle");
			break;
		case CardMessenger::END_DISPATCH_CARDS:
			{
				if(m_restoring)
				{
					if(hasCardsToThrow())
					{
						throwQueuedCard();
					}
					else
					{
						finishRestoreState();
					}
				}
				else
				{
					std::vector<int> dispatch;
					sendMessageToGame(FINISH_DISPATCH_CARDS_C,dispatch);
				}
			}
			break;
		case CardMessenger::BEGIN_SWAP_CARDS:
			dispatchSound("pass.cards");
			break;
		case CardMessenger::END_SWAP_CARDS:
			{
				//2 times because 1 to swap vert and one to swap horz
				m_numPerformPasses++;
				if(m_numPerformPasses == 2)
				{
					m_numPerformPasses = 0;
					std::vector<int> args;
					//only 1 now
					m_view->setMaxSelectedCards(1);
					sendMessageToGame(FINISHED_CARD_SWAP_C,args);
				}
			}
			break;
		case CardMessenger::BEGIN_MOVE_CARD_UP:
			{
				if(getState() == WAITING_FOR_PLAYER_SELECT_SWAP_CARDS)
				{
					if(m_view->getNumSelectedCards() == 2) //for pass 2
					{
						enablePassButton(true);
					}
				}
#ifndef CGE_MOBILE
				
				if(getState() == WAITING_FOR_PLAYER_SELECT_CARD)
				{
					if(m_view->getNumSelectedCards() == 1)
					{
						sendSelectedCard();
					}
				}
#endif/*
				else
				{
					if(rand() % 2 == 0)
						dispatchSound("play.card");
					else
						dispatchSound("play.card2");
				}
                   */
			}
			break;
		case CardMessenger::BEGIN_MOVE_CARD_DOWN:
			{
				if(getState() == WAITING_FOR_PLAYER_SELECT_SWAP_CARDS)
				{
					if(m_view->getNumSelectedCards() != 2) //for pass 2
					{
						enablePassButton(false);
					}
				}

				//dispatchSound("card.down");
			}
			break;
		case CardMessenger::END_THROW_CARD_ON_TABLE:
			{
				
				if(m_restoring)
				{
					if(hasCardsToThrow())
					{
						throwQueuedCard();
					}
					else
					{
						finishRestoreState();
					}
				}
				else
				{
					std::vector<int> eArgs;
					sendMessageToGame(FINISH_PERFORM_CARD_ON_TABLE_C,eArgs);
				}
			}
			break; 
		case CardMessenger::BEGIN_TABLE_CARDS_TO_PLAYER:
			dispatchSound("grab.cards");
			break;
		case CardMessenger::END_TABLE_CARDS_TO_PLAYER:
			{
				if(getState() == WAITING_FOR_GIVE_POINT)
				{
					std::vector<int> eArgs;
					addTrickToPlayer(m_lastPointReceiver,m_lastPointType);
					sendMessageToGame(FINISHED_PLAYER_MADE_TRICK_C,eArgs);
					setState(WAITING_FOR_SERVER_INPUT);
				}

				//regardless of the state, the last hand needs to be updated
				std::vector<Card> realLastHand;

				realLastHand = generateLastHand();

				//emit the event
				for(std::vector<ClientEventListener*>::iterator it = 
					m_listeners.begin();
					it != m_listeners.end(); ++it)
				{
					(*it)->lastHandChanged(realLastHand);
				}

				clearLastHand();
			}
			break;
		}
	}

	void HumanSpadesClient::setView( CardView* view )
	{
		if(m_view)
		{
			m_view->removeListener(this);
		}
		else
		{
			m_view = view;
			m_view->addListener(this);
		}
	}

	void HumanSpadesClient::doReplyGameInit()
	{
		sendMessageToGame(REPLY_INIT_GAME_C,std::vector<int>());
	}

	void HumanSpadesClient::msgRequestGameInit( 
		SpadesCSMessageEnum msg,const std::vector<int>& args )
	{
		/*
		//eventually this will cause a button to appear
		//but for now we just reply right away
		doReplyGameInit();

		//game begins here for now
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->gameBegin();
		}
		*/
	}

	void HumanSpadesClient::msgRequestDeckCutPos( 
		SpadesCSMessageEnum msg,const std::vector<int>& args )
	{
		m_view->evtSelectDeckCut();
		setState(WAITING_FOR_DECK_CUT_POS);
	}

	void HumanSpadesClient::setState( StateEnum state )
	{
		m_state = state;
	}

	HumanSpadesClient::StateEnum HumanSpadesClient::getState() const
	{
		return m_state;
	}

	void HumanSpadesClient::msgDispatchCards( 
		SpadesCSMessageEnum msg,const std::vector<int>& args )
	{

		std::vector<Card> pCards;
		//first 3 are opponent count
		//then Vector of card number
		//make the Vector of Card
		for(int i = 4; i < (int)args.size(); ++i)
		{
			pCards.push_back(Card::cardFromNumber(args[i]));
		}

		//set who the dealer is
		setDealer(m_dealer);

		//trigger the dispatch event
		m_view->evtDispatchCards(pCards,args[1],args[2],args[3],
			(CardView::CardHandEnum)args[0]);

		setState(WAITING_FOR_CARDS_DISPATCHED);
	}

	void HumanSpadesClient::setActivePlayer( int active )
	{
		m_active = active;

		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->activePlayerChanged(active);
		}
	}

	void HumanSpadesClient::setDealer( int dealer )
	{
		m_dealer = dealer;
	}

	void HumanSpadesClient::msgRequestTrickCount( 
		SpadesCSMessageEnum msg,const std::vector<int>& args )
	{
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->bidBegin(getGameInfo().getMaxRoundPoints(),getGameInfo().isNilEnabled());
		}

		m_view->disableMouseInput();

		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->playSound("turn");
		}

		setState(WAITING_FOR_TRICK_COUNT);

	}

	void HumanSpadesClient::msgGotTrickCount( SpadesCSMessageEnum msg,
		const std::vector<int>& args )
	{
		setState(WAITING_FOR_SERVER_INPUT);

		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->playerBidChanged(args[0],args[1]);
		}
	}

	void HumanSpadesClient::msgRequestCardSwap( 
		SpadesCSMessageEnum msg,const std::vector<int>& args )
	{
		//make sure everything is enabled
		m_view->setDisabledCards(std::vector<int>());

		//pass 2 needs 2
		m_view->setMaxSelectedCards(2);

		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->passCardsBegin();
		}

		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->playSound("turn");
		}

		setState(WAITING_FOR_PLAYER_SELECT_SWAP_CARDS);
	}

	void HumanSpadesClient::enablePassButton( bool enabled )
	{
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->passButtonEnabledChanged(enabled);
		}
	}

	void HumanSpadesClient::msgPerformSwap( 
		SpadesCSMessageEnum msg,const std::vector<int>& args )
	{
		if(args[0] == 0 || args[0] == 1) //does it need to actually swap?
		{
			std::vector<Card> incoming;
			std::vector<Card> outgoing;

			incoming.push_back(Card::cardFromNumber(args[1]));
			incoming.push_back(Card::cardFromNumber(args[2]));
			outgoing.push_back(Card::cardFromNumber(args[3]));
			outgoing.push_back(Card::cardFromNumber(args[4]));

			setSwapCards(incoming,outgoing);

			m_view->evtSwapCards(outgoing,incoming,CardView::PLAYER,CardView::TOP);

		}
	
		if(args[0] == 0 || args[0] == 2) //do we need to swap left to right?
		{
			std::vector<Card> dummy(2,Card(Card::SPADES,Card::ACE));
			m_view->evtSwapCards(dummy,dummy,CardView::LEFT,CardView::RIGHT);
		}


		m_numPerformPasses = args[0] == 0 ? 0 : 1; //will we be having 1 or 2 animations?
		setState(WAITING_FOR_PERFORM_SWAP);
	}

	void HumanSpadesClient::msgPerformCardOnTable( 
		SpadesCSMessageEnum msg,const std::vector<int>& args )
	{
		Card throwCard = Card::cardFromNumber(args[0]);
		appendLastHand(throwCard,args[1]);
		CardView::CardHandEnum player = (CardView::CardHandEnum)args[1];

		m_view->evtThrowCardOnTable(throwCard,player,-1);

		if(throwCard.getSuit() == Card::SPADES && !m_spadesBroken)
		{
			m_spadesBroken = true;
			for(std::vector<ClientEventListener*>::iterator it = 
				m_listeners.begin();
				it != m_listeners.end(); ++it)
			{
				(*it)->spadesBroken(args[1]);
			}
		}
	}
	void HumanSpadesClient::msgRequestSelectedCard(
		SpadesCSMessageEnum msg,const std::vector<int>& args )
	{
		//if a card is already selected, use it
#ifndef CGE_MOBILE
		if(m_view->getNumSelectedCards() == 1)
		{
			sendSelectedCard();
		}
		else
#endif
		{
			setState(WAITING_FOR_PLAYER_SELECT_CARD);
			
			for(std::vector<ClientEventListener*>::iterator it = 
				m_listeners.begin();
				it != m_listeners.end(); ++it)
			{
				(*it)->playSound("turn");
			}

#ifdef CGE_MOBILE
            m_view->setWaitForSelectCard(true);
#endif
#ifdef CGE_DEV_AUTOPLAY
			m_view->playRandomCard();
#endif
		}
	}

	void HumanSpadesClient::sendSelectedCard()
	{
		m_view->setWaitForSelectCard(false);
		Card card = m_view->getSelectedCard();
		std::vector<int> cardArg(1,m_view->getSelectedCardIndex());

		//do it now to avoid network delay
		m_view->evtThrowCardOnTable(card,CardView::PLAYER,m_view->getSelectedCardIndex());
		if(card.getSuit() == Card::SPADES && !m_spadesBroken)
		{
			m_spadesBroken = true;
			for(std::vector<ClientEventListener*>::iterator it = 
				m_listeners.begin();
				it != m_listeners.end(); ++it)
			{
				(*it)->spadesBroken(0);
			}

		}
		appendLastHand(card,0);
		sendMessageToGame(SEND_SELECTED_CARD_C,cardArg);

		setState(WAITING_FOR_SERVER_INPUT);
	}

	void HumanSpadesClient::msgPlayerMadeTrick(
		SpadesCSMessageEnum msg,const std::vector<int>& args )
	{
		//safety
		m_view->setWaitForSelectCard(false);
		m_view->evtSendTableCardsToPlayer((CardView::CardHandEnum)args[0]);
		m_lastPointReceiver = args[1];
		m_lastPointType = (SpadesPointEnum)args[2];

		setState(WAITING_FOR_GIVE_POINT);
	}

	void HumanSpadesClient::msgFilterCards( 
		SpadesCSMessageEnum msg,const std::vector<int>& args )
	{
		//disable the cards
		m_view->setDisabledCards(args);
	}

	void HumanSpadesClient::msgSendActivePlayer( 
		SpadesCSMessageEnum msg,const std::vector<int>& args )
	{
		setActivePlayer(args[0]);
	}

	void HumanSpadesClient::addTrickToPlayer( int player, SpadesPointEnum type )
	{
		//another output
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->playerMadeTrick(player,type);
		}

	}

	void HumanSpadesClient::msgSendRoundScores( SpadesCSMessageEnum msg,
		const std::vector<int>& args )
	{
		int numP = args.size() / 2;
		std::vector<int> scores;
		std::vector<int> bags;
		for(int i = 0; i < numP; ++i)
		{
			scores.push_back(args[i]);
			bags.push_back(args[i + numP]);
		}

		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->roundScoresChanged(scores,bags);
		}
	}

	void HumanSpadesClient::setSwapCards( 
		const std::vector<Card>& incoming,const std::vector<Card>& outgoing )
	{
		if(!m_watching)
		{
			for(std::vector<ClientEventListener*>::iterator it = 
				m_listeners.begin();
				it != m_listeners.end(); ++it)
			{
				(*it)->passTwoCardsChanged(incoming,outgoing);
			}
		}
	
	}

	void HumanSpadesClient::addListener( ClientEventListener* listener )
	{
		m_listeners.push_back(listener);
	}

	void HumanSpadesClient::removeListener( ClientEventListener* listener )
	{
		m_listeners.erase(
			std::remove(m_listeners.begin(),
			m_listeners.end(), listener),
			m_listeners.end());
	}

	void HumanSpadesClient::endPassCards()
	{
		if(getState() == WAITING_FOR_PLAYER_SELECT_SWAP_CARDS)
		{
			std::vector<Card> selectedCards = m_view->getSelectedCards();
			std::vector<int> args;
			for(size_t i = 0; i < selectedCards.size(); ++i)
			{
				args.push_back(selectedCards[i].getCardNumber());
			}

			sendMessageToGame(SEND_SWAPPED_CARDS_C,args);

			//disable the mouse
			m_view->disableMouseInput();
		}
	}

	void HumanSpadesClient::trickCountChanged( int bid )
	{
		std::vector<int> theBid(1,bid);
		sendMessageToGame(SEND_PLAYER_TRICK_COUNT_C,theBid);
		m_view->enableMouseInput();
		setState(WAITING_FOR_SERVER_INPUT);
	}

	void HumanSpadesClient::msgRoundBegin()
	{
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->roundBegan();
		}

		m_spadesBroken = false;
	}

	void HumanSpadesClient::beginGame()
	{
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->gameBegin();
		}

	}

	void HumanSpadesClient::restoreState( const GameClientState& state )
	{
		m_restoreState = state;
		m_watching = !state.playerHasCards();

		if(m_watching)
		{
			m_view->setMouseDisabled(true);
		}

		//set the swap cards
		std::vector<Card> incoming;
		std::vector<Card> outgoing;
		for(size_t i = 0; i < state.getPassedCards().size(); ++i)
		{
			outgoing.push_back(Card::cardFromNumber(state.getPassedCards()[i]));
		}
		for(size_t i = 0; i < state.getReceivedCards().size(); ++i)
		{
			incoming.push_back(Card::cardFromNumber(state.getReceivedCards()[i]));
		}

		if(incoming.size() == outgoing.size())
		setSwapCards(incoming,outgoing);

		//set the score
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->roundScoresChanged(state.getTeamScores(),state.getTeamBags());
		}


		bool restoringAllCards = false;

		int numCards = 0;

		for(int i = 0; i < state.getCardsOnTable().size(); ++i)
		{
			if(state.getCardsOnTable()[i] != -1)
			{
				numCards++;
			}
		}

		restoringAllCards = (numCards == getGameInfo().getNumPlayers());

		//set the prior hand
		std::vector<Card> lastHand;
		if(restoringAllCards)
		{
			m_firstPlayer = state.getFirstCardPayer();
			for(size_t i = 0; i < state.getPriorHand().size(); ++i)
			{
				m_lastHand[i] = state.getPriorHand()[i];
				lastHand.push_back(Card::cardFromNumber(state.getPriorHand()[i]));
			}
		}
		else
		{
			for(size_t i = 0; i < state.getPriorHand().size(); ++i)
			{
				lastHand.push_back(Card::cardFromNumber(state.getPriorHand()[i]));
			}
		}
	
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->lastHandChanged(lastHand);
		}

		//set dealer
		setDealer(state.getDealer());
	
		//set spades broken
		m_spadesBroken = state.isSpadesBroken();

		//set the bid
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->bidStateChanged(
				state.getPlayerPoints(0),state.getPlayerPoints(1),
				state.getPlayerPoints(2),state.getPlayerPoints(3),
				state.getPlayerRoundBids());
		}

		bool hasAllBids = true;
		int numP = getGameInfo().getNumPlayers();
		std::vector<int> bids = m_restoreState.getPlayerRoundBids();
		for(int i = 0; i < bids.size(); ++i)
		{
			if((bids[i] == -1 && numP  == 4) || (bids[i] == -1 && numP  == 3 && i != 2))
			{
				hasAllBids = false;
			}
		}

		GameStateEnum s = state.getGameState();
		//set active player
		setActivePlayer(-1);
		for(int i = 0; i < 4; i++)
		{
			if(state.isPlayerActive(i))
			{
				setActivePlayer(i);
			}
		}

		//subtract a card if we were given all the cards
		bool minusCard = 0;
		if(restoringAllCards)
			minusCard = 1;

		//restore gifts
		for(int i = 0; i < 4; i++)
		{
			for(std::vector<ClientEventListener*>::iterator it = 
				m_listeners.begin();
				it != m_listeners.end(); ++it)
			{
				(*it)->giftChanged(i,state.getGiftID(i),state.getGiftFrame(i));
			}

		}

		//if there are cards to dispatch
		if(state.getCards().size() > 0)
		{
			m_restoring = true;

			if(!restoringAllCards)
			generateThrowCards(state.getCardsOnTable());

			//set who the dealer is
			setDealer(m_dealer);
			std::vector<Card> pCards;
			if(!m_watching)
			{
				for(size_t i = 0; i < state.getCards().size(); ++i)
				{
					pCards.push_back(Card::cardFromNumber(state.getCards()[i]));
				}
			}
			else if(state.getCards().size() > 0)
			{
				for(size_t i = 0; i < state.getCards()[0] - minusCard; ++i)
				{
					pCards.push_back(Card::cardFromNumber(-1));
				}
			}
		
			if(pCards.size() > 0)
			{
				//trigger the dispatch event
				m_view->evtDispatchCards(pCards,state.getNumCards(1) - minusCard,state.getNumCards(2) - minusCard,state.getNumCards(3) - minusCard,
					(CardView::CardHandEnum)state.getDealer());
				setState(WAITING_FOR_CARDS_DISPATCHED);
			}
		
		}
		else
		{
			if(state.getGameState() == GAMESTATE_NEED_TO_CUT_DECK)
			{
				msgRequestDeckCutPos(REQUEST_DECK_CUT_POS_S,std::vector<int>());
			}
			finishRestoreState();
		}
	}

	void HumanSpadesClient::finishRestoreState()
	{
		m_restoring = false;

		GameStateEnum s = m_restoreState.getGameState();

		m_view->setDisabledCards(m_restoreState.getFilteredCards());
		m_state = WAITING_FOR_SERVER_INPUT;


		if(s == GAMESTATE_NEED_TO_BID)
		{
			msgRequestTrickCount(REQUEST_PLAYER_TRICK_COUNT_S,std::vector<int>());
		}
		else if(s == GAMESTATE_NEED_TO_PLAY_CARD)
		{
			msgRequestSelectedCard(REQUEST_SELECTED_CARD_S,std::vector<int>());
		}
		else if(s == GAMESTATE_WAITING_TO_BID)
		{
			m_view->disableMouseInput();
		}
		else if(s == GAMESTATE_NEED_CHOOSE_2_CARDS)
		{
			msgRequestCardSwap(REQUEST_CARD_SWAP_S,std::vector<int>());
		}
		else if(s == GAMESTATE_WAITING_TO_PASS_CARDS)
		{
			msgRequestCardSwap(REQUEST_CARD_SWAP_S,std::vector<int>());
			for(int i = 0; i < (int)m_restoreState.getPassedCards().size(); ++i)
			{
				Card c = Card::cardFromNumber(m_restoreState.getPassedCards()[i]);

				int index = m_view->getIndexOfCard(c);
				m_view->selectCard(index);
			}

			m_view->disableUnselectedCards();

			m_view->disableMouseInput();

			for(std::vector<ClientEventListener*>::iterator it = 
				m_listeners.begin();
				it != m_listeners.end(); ++it)
			{
				(*it)->passButtonVisibilityChanged(false);
			}
		}
	}


	void HumanSpadesClient::throwQueuedCard()
	{
		if(hasCardsToThrow())
		{
			Card c = m_cardsToThrow.front().first;
			int p = m_cardsToThrow.front().second;
			int index = -1;

			//if it's you, get the index
			if(p == 0)
			{
				index = m_view->getIndexOfCard(c);
			}

			m_view->evtThrowCardOnTable(c,(CardView::CardHandEnum)p,index);
		
			appendLastHand(c,p);
			m_active = p;

			m_cardsToThrow.pop();
		}
			
	}

	bool HumanSpadesClient::hasCardsToThrow() const
	{
		return !m_cardsToThrow.empty();
	}

	void HumanSpadesClient::generateThrowCards( std::vector<int> cards )
	{
		while(!m_cardsToThrow.empty())
		{
			m_cardsToThrow.pop();
		}

		bool empty = true;
		for(size_t i = 0; i < cards.size();++i)
		{
			if(cards[i] != -1)
			{
				empty = false; 
				break;
			}
		}

		if(empty)
		{
			return;
		}

		for(size_t i = m_restoreState.getFirstCardPayer(); i < cards.size(); ++i)
		{
			if(cards[i] != -1)
			{
				m_cardsToThrow.push(std::make_pair(Card::cardFromNumber(cards[i]),i));
			}
		}

		for(size_t i = 0; i < m_restoreState.getFirstCardPayer(); ++i)
		{
			if(cards[i] != -1)
			{
				m_cardsToThrow.push(std::make_pair(Card::cardFromNumber(cards[i]),i));
			}
		}

	}

	void HumanSpadesClient::appendLastHand( const Card& card, int pos )
	{
		if(lastHandEmpty())
		{
			m_firstPlayer = pos;
		}

		m_lastHand[pos] = card.getCardNumber();
	}

	bool HumanSpadesClient::lastHandEmpty() const
	{
		for(size_t i = 0; i < m_lastHand.size(); ++i)
		{
			if(m_lastHand[i] != -1)
			{
				return false;
			}
		}

		return true;
	}

	void HumanSpadesClient::clearLastHand()
	{
		m_firstPlayer = -1;
		for(size_t i = 0; i < m_lastHand.size(); ++i)
		{
			m_lastHand[i] = -1;
		}
	}

	std::vector<Card> HumanSpadesClient::generateLastHand()
	{
		std::vector<Card> c;
		if(m_firstPlayer == -1 || lastHandEmpty())
		{
			return c;
		}
		
		for(size_t i = 0; i < m_lastHand.size(); ++i)
		{
			if(m_lastHand[i] != -1)
			{
				c.push_back(Card::cardFromNumber(m_lastHand[i]));
			}
		}

		return c;
	}

	void HumanSpadesClient::processQueuedMessages()
	{
		m_canQueueMsgs = false;
		while(!m_qMsg.empty())
		{
			processMessageFromGame(m_qMsg.front().getMessage(),m_qMsg.front().getArgs());
			m_qMsg.pop();

			if(!m_view->isAnimationQueueEmpty())
			{
				break;
			}
		}
		m_canQueueMsgs = true;
	}

	void HumanSpadesClient::queueMessage( SpadesCSMessageEnum msg, const std::vector<int>& args )
	{
		m_qMsg.push(SpadesGameMessage(msg,args,0));
	}

	void HumanSpadesClient::sendMessageToGame( SpadesCSMessageEnum msg, const std::vector<int>& args )
	{
		if(!m_watching)
		{
			SpadesClient::sendMessageToGame(msg,args);
		}
	}

	bool HumanSpadesClient::isWatching() const
	{
		return m_watching;
	}

	void HumanSpadesClient::logic()
	{
		if(m_view->isAnimationQueueEmpty() || m_view->isOnlyAnimationShrink())
		{
			processQueuedMessages();
		}
	}

	void HumanSpadesClient::dispatchSound( const std::string& sound )
	{
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->playSound(sound);
		}
	}

	bool HumanSpadesClient::hasItemsQueued() const
	{
		bool empty = m_qMsg.empty() && (m_view->isAnimationQueueEmpty() || m_view->isOnlyAnimationShrink());

		return !empty;
	}

	void HumanSpadesClient::dispatchNilFailed( int player )
	{
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->playerFailedNil(player);
		}
	}

	void HumanSpadesClient::dispatchContractCompleted( int playerCompleter, bool partnership)
	{
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->contractCompleted(playerCompleter,partnership,isWatching());
		}
	}
}



