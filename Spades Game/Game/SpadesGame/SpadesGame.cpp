#include "Game/SpadesGame/SpadesGame.hpp"
#include "Game/Net/RawGameListener.hpp"
#include <algorithm>
#include <iostream>
namespace cge
{
	SpadesGame::SpadesGame(void)
		: m_activePlayer(1), m_host(0),
		m_gameInfoIsSet(false), m_deckCutPos(0),
		m_spadesBroken(false),
		m_state(WAITING_FOR_NUM_PLAYERS),
		m_lastPointWinner(0), m_lastPointReceiver(0),
		m_lastPointType(NOT_A_POINT),
		m_roundActivePlayer(0),
		m_handCount(0),m_time(0),
		m_requestedBidFromPlayer(-1),
		m_requestCardFromPlayer(-1),
		m_cardOffset(0),
		m_lastMoonshotWinner(-1),m_lastGameWinner(-1),
		m_endTimeStamp(0),m_rawListener(NULL)

	{
		clearWaitBools();
		m_pActive[0] = false;
		m_pActive[1] = false;
		m_pActive[2] = false;
		m_pActive[3] = false;

		m_orderedTableCards.resize(4,-1);
	}

	SpadesGame::~SpadesGame(void)
	{
	}

	void SpadesGame::setState( StateEnum state )
	{
		m_state = state;
	}

	void SpadesGame::pushMessage( const SpadesGameMessage& msg )
	{
		m_msgQ.push(msg);
	}

	void SpadesGame::processMessages()
	{
		while (!m_msgQ.empty())
		{
			processMessage(m_msgQ.front());
			m_msgQ.pop();
		}
	}

	void SpadesGame::processMessage( const SpadesGameMessage& msg )
	{
		//code to process event
		switch(msg.getMessage())
		{
		case SET_GAME_INFO_C:
			msgSetGameInfo(msg);
			break;
		case REPLY_INIT_GAME_C:
			msgReplyInitGame(msg);
			break;
		case PLAYER_JOINED_C:
			msgJoined(msg);
			break;
		case SEND_DECK_CUT_POS_C:
			msgGotDeckCutPos(msg);
			break;
		case FINISH_DISPATCH_CARDS_C:
			msgCardsDispatched(msg);
			break;
		case SEND_PLAYER_TRICK_COUNT_C:
			msgGotTrickCountFromPlayer(msg);
			break;
		case SEND_SWAPPED_CARDS_C:
			msgGotSwappedCardsFromPlayer(msg);
			break;
		case FINISHED_CARD_SWAP_C:
			msgFinishedCardSwap(msg);
			break;
		case SEND_SELECTED_CARD_C:
			msgGotTableCardFromPlayer(msg);
			break;
		case FINISH_PERFORM_CARD_ON_TABLE_C:
			msgPerformedCardOnTable(msg);
			break;
		case FINISHED_PLAYER_MADE_TRICK_C:
			msgPerformedGivePoint(msg);
			break;
		}
	}

	void SpadesGame::setActivePlayer( int player )
	{
		if(player < 4 && m_player[player].isAlive())
		{
			m_activePlayer = player;
		}
	}

	int SpadesGame::getActivePlayer() const
	{
		return m_activePlayer;
	}

	SpadesGame::StateEnum SpadesGame::getState() const
	{
		return m_state;
	}

	void SpadesGame::msgSetGameInfo( const SpadesGameMessage& msg )
	{
		if(!setGameInfo(msg.getArgs()))
		{
			return;
		}

		m_host = msg.getPlayer();
		m_gameInfoIsSet = true;
		setState(WAITING_FOR_NUM_PLAYERS);
	}

	int SpadesGame::mapServerToClientPos( int frontPayer, int targetPlayer ) const
	{
		int pos =  0;
		if(m_gameInfo.getNumPlayers() == 3)
		{
			if(targetPlayer == 3)
			{
				return 2;
			}

			pos = (3 + targetPlayer - frontPayer) % 3;
			if(pos == 2)
			{
				pos++;
			}
		}
		else
		{
			pos = (4 + targetPlayer - frontPayer) % 4;
		}
			
		return pos;
	}

	int SpadesGame::getConnectedPlayerCount() const
	{
		int count = 0;
		for(int i = 0; i < 4; ++i)
		{
			if(m_player[i].isAlive())
			{
				count++;
			}
		}

		return count;
	}

	bool SpadesGame::setGameInfo( const std::vector<int>& gameInfo )
	{
		m_gameInfo = SpadesGameInfo(gameInfo);

		return true;
	}

	void SpadesGame::setGameInfo( const SpadesGameInfo& info )
	{
		m_gameInfo = info;
	}

	std::vector<int> SpadesGame::getGameInfoVect() const
	{

		return m_gameInfo.toIntArray();
	}

	void SpadesGame::sendGameInfoToPlayer( int player )
	{
		//send player the game info and who the host is
		std::vector<int> gameInfo = getGameInfoVect();
		gameInfo.push_back(mapServerToClientPos(player,m_host)); //host
		sendMessageToPlayer(SpadesGameMessage(SEND_GAME_INFO_D_S,gameInfo,player));
	}

	void SpadesGame::msgJoined( const SpadesGameMessage& msg )
	{
		if(m_gameInfoIsSet)
		{
			sendGameInfoToPlayer(msg.getPlayer());
		}

		m_player[msg.getPlayer()].setAlive(true);

		if( getState() == WAITING_FOR_NUM_PLAYERS && 
			getConnectedPlayerCount() == m_gameInfo.getNumPlayers())
		{
			msgReplyInitGame(msg);
			/*
			sendMessageToPlayer(SpadesGameMessage(REQUEST_GAME_INIT_S,
				std::vector<int>(),m_host));
			setState(WAITING_FOR_GAME_INIT);
			*/
		}
	}

	void SpadesGame::msgReplyInitGame( const SpadesGameMessage& msg )
	{
		doNewGame();
		doNewRound();

	}

	int SpadesGame::getDealer() const
	{
		int p = 0;
		int count = 0;
		do
		{
			p = (m_gameInfo.getNumPlayers() + (getActivePlayer() - count) - 1)
				% m_gameInfo.getNumPlayers();
			count++;
		}
		while(!m_player[p].isAlive() && count < 3);
		return p;
	}

	int SpadesGame::getDeckCutter() const
	{
		int p = 0;
		int count = 0;
		do
		{
			p = (m_gameInfo.getNumPlayers() + 
				(getDealer() - count) - 1) % m_gameInfo.getNumPlayers();
			count++;
		}
		while(!m_player[p].isAlive() && count < 3);
			return p;
	}

	void SpadesGame::clearWaitBools()
	{
		for(int i = 0; i < 4; ++i)
		{
			m_waitBools[i] = false;
		}
	}

	void SpadesGame::setWaitBool( int waitBool, bool val /*= true*/ )
	{
		m_waitBools[waitBool] = val;
	}

	bool SpadesGame::isWaitBoolFinished() const
	{
		for(int i = 0; i < 4; ++i)
		{
			if(m_player[i].isAlive() && !m_waitBools[i])
			{
				return false;
			}
		}

		return true;
	}

	void SpadesGame::msgGotDeckCutPos( const SpadesGameMessage& msg )
	{
		if(msg.getArgs().size() >= 1)
		{
			m_deckCutPos = msg.getArgs()[0];
			if(m_deckCutPos < 0 || m_deckCutPos > 51)
				m_deckCutPos = m_cardShuffler.nextRangeInt(0,50);
		}

		playerFinishedAction(msg.getPlayer());
		doCardDispatch();
	}

	void SpadesGame::doCardDispatch()
	{
		std::vector<Card> cards = 
			m_cardShuffler.shuffleCards(m_deckCutPos);

		//we discard a card in cutthroat
		if(m_gameInfo.isCutthroat())
		{
			cards.erase(cards.begin() + (cards.size() - 1));
		}

		std::vector<Card> playerCards[4];

		//give each player a card until we run out of cards
		while(!cards.empty())
		{
			for(int i = 0; i < m_gameInfo.getNumPlayers(); ++i)
			{
				playerCards[i].push_back(cards.back());
				if(!cards.empty())
					cards.erase(cards.begin() + (cards.size() - 1));
			}
		}

		
		//std::cout << "CARDS" << std::endl;
		//set the cards on server side and do some clean up
		for(int i = 0; i < 4; ++i)
		{
			if(m_player[i].isAlive())
			{
				m_player[i].setCards(playerCards[i]);
				m_player[i].clearRoundTricks();

					//std::cout << std::endl;
					//std::cout << "-----------" << std::endl;
					//std::cout << "Player " << i << std::endl;
					//std::cout << "-----------" << std::endl;
					//std::cout << std::endl;
					
				//for(int j = 0;j < playerCards[i].size(); j++ )
				//{
				//	Card c = Card::cardFromNumber(m_player[i].getCardNumbers()[j]);
			//		std::cout << c.getRankString() << " of " << c.getSuitString() << std::endl;
			//	}
			}	
		}

		clearWaitBools(); //we will be waiting for all players
		
		//send each player their cards
		for(int i = 0; i < 4; ++i)
		{
			if(!m_player[i].isAlive())
			{
				continue;
			}

			std::vector<int> pCards;
			//add the dealer
			pCards.push_back(mapServerToClientPos(i,getDealer()));

			//number of cards others have
			for(int j = 1; j <= 3; ++j)
			{
				int pos = mapClientToServer(i, j);
				if(m_player[pos].isAlive())
				{
					pCards.push_back(m_player[pos].getCardCount());
				}
				else
				{
					pCards.push_back(0);
				}
				
			}
			for(size_t j = 0; j < m_player[i].getCardCount(); ++j)
			{
				pCards.push_back(m_player[i].getCardNumberOf(j));
			}

			sendMessageToPlayer(SpadesGameMessage(DISPATCH_CARDS_A_D_S,pCards,i));
			setState(WAITING_FOR_CARDS_DISPATCHED);
		}
	}

	void SpadesGame::msgCardsDispatched( const SpadesGameMessage& msg )
	{
		setWaitBool(msg.getPlayer(),true);

		if(isWaitBoolFinished())
		{
			//do bidding requests
			doBidding();
			
		}
	}

	void SpadesGame::doBidding()
	{
		//Mirror games means bid is number of spades in hand
		if(m_gameInfo.isMirror())
		{
			std::vector<int> trickCount(1);

			for(int i = 0; i < 4; ++i)
			{
				if(!m_player[i].isAlive())
				{
					continue;
				}

				trickCount[0] = m_player[i].getQuantityOfSuit(Card::SPADES);
				pushMessage(SpadesGameMessage(SEND_PLAYER_TRICK_COUNT_C,trickCount,i));
			}
		}
		else
		{
			//ask first player for their bid
			sendMessageToPlayer(SpadesGameMessage(REQUEST_PLAYER_TRICK_COUNT_S,getActivePlayer()));
			setRequestBidder(getActivePlayer());
			sendActivePlayersNotBid();
		}

		//in all cases, wait until everyone has sent their trick count
		clearWaitBools();
		setState(WAITING_FOR_PLAYER_TRICK_COUNT);
	}

	void SpadesGame::msgGotTrickCountFromPlayer( const SpadesGameMessage& msg )
	{
		setWaitBool(msg.getPlayer(),true);
		
		int bid = 2;

		if(msg.getArgs().size() > 0)
			bid = msg.getArgs()[0];

		if(bid < 0 || bid > getGameInfo().getMaxRoundPoints())
			bid = 2;

		//set number of tricks
		m_player[msg.getPlayer()].setRoundBid(bid);

		std::vector<int> playerBid(1); //player number

		playerBid.push_back(bid); //player bid

		playerFinishedAction(msg.getPlayer());

		//tell everyone about this
		for(int i = 0; i < 4; ++i)
		{
			if(!m_player[i].isAlive())
			{
				continue;
			}
			
			playerBid[0] = mapServerToClientPos(i,msg.getPlayer());

			sendMessageToPlayer(SpadesGameMessage(
				SEND_PLAYER_TRICK_COUNT_D_S,playerBid,i));
		}

		if(m_gameInfo.isSuicide() && !isWaitBoolFinished())
		{
			//send the other player the bid or let him bid
			if(getNextPlayer(msg.getPlayer()) == getPartner(getActivePlayer()) ||
				getNextPlayer(msg.getPlayer()) == getPartner(getNextPlayer(getActivePlayer())))
			{
				int partner = getPartner(getNextPlayer(msg.getPlayer()));

				//player bid nil, ask partner to bid
				if(m_player[partner].getRoundBid() == 0)
				{
					sendMessageToPlayer(SpadesGameMessage(REQUEST_PLAYER_TRICK_COUNT_S,
						getNextPlayer(msg.getPlayer())));
					setRequestBidder(getNextPlayer(msg.getPlayer()));
					sendActivePlayersNotBid();
				}
				else //act like the partner bid nil
				{
					std::vector<int> trickCount(1,0);
					pushMessage(SpadesGameMessage(SEND_PLAYER_TRICK_COUNT_C,trickCount,
						getNextPlayer(msg.getPlayer())));
				}
			}
			else
			{
				sendMessageToPlayer(
					SpadesGameMessage(REQUEST_PLAYER_TRICK_COUNT_S, getNextPlayer(msg.getPlayer())));
				setRequestBidder(getNextPlayer(msg.getPlayer()));
				sendActivePlayersNotBid();
			}
		}
		else if(!isWaitBoolFinished() && !m_gameInfo.isMirror())
		{
			sendMessageToPlayer(
				SpadesGameMessage(REQUEST_PLAYER_TRICK_COUNT_S, getNextPlayer(msg.getPlayer())));
			setRequestBidder(getNextPlayer(msg.getPlayer()));
				sendActivePlayersNotBid();
		}

		//if everyone has their bid, move on to passing or the actual game
		if(isWaitBoolFinished())
		{
			if(m_rawListener) {
				std::vector<Team> t;
				std::vector<Player> p;
				for(size_t i = 0; i < getNumberOfTeams(); i++)
					t.push_back(m_team[i]);
				for(size_t i = 0; i < m_gameInfo.getNumPlayers(); i++)
					p.push_back(m_player[i]);
				m_rawListener->bidsReceived(t,p);
			}

			if(m_gameInfo.isPassTwo() && getNumberOfTeamsPassingTwo() > 0)
			{
				doPassTwoRequest();
			}
			else
			{
				doRequestForCard();
			}
		}
	}

	int SpadesGame::mapClientToServer( int serverClient,int clientPos, bool shiftOnThree) const
	{
		//assumes there is no top player
		if(m_gameInfo.getNumPlayers() == 3)
		{
			if(shiftOnThree)
			{
				return (3 + serverClient + clientPos) % 3;
			}

			if(clientPos == 0)
			{
				return serverClient;
			}
			else if(clientPos == 1)
			{
				return (3 + serverClient + 1) % 3;
			}
			else if(clientPos == 3)
			{
				return (3 + serverClient - 1) % 3;
			}
			else
			{
				return 3; //3 is always dead on the server in 3 p spades
			}
		}
		else
		{
			//circular shift
			return (4 + serverClient + clientPos) % 4;
		}
	}

	int SpadesGame::getNextPlayer( int player ) const
	{
		return (player + 1) % m_gameInfo.getNumPlayers();
	}

	int SpadesGame::getPartner( int player ) const
	{
		return (player + 2) % 4;
	}

	void SpadesGame::doPassTwoRequest()
	{
		bool teamPassing[2];
		for(int i = 0; i < 2; ++i)
		{
			teamPassing[i] = isTeamPassingTwo(i);
		}

		std::vector<int> numSwapCards(1,2); //pass 2 cards

		clearWaitBools(); //wait until everyone sends their cards

		m_swappingPlayers.clear();

		//make the request only if it is applicable
		bool activePlayers[4];
		for(int i = 0; i < 4; i++)
			activePlayers[i] = false;
		bool someonePassed = false;
		for(int i = 0; i < 2; ++i)
		{
			if(teamPassing[i])
			{
				m_swappingPlayers.push_back(i);
				m_swappingPlayers.push_back(getPartner(i));
				m_player[i].setIncomingSwapCardNumbers(std::vector<int>());
				m_player[i].setOutgoingSwapCardNumbers(std::vector<int>());
				m_player[getPartner(i)].setIncomingSwapCardNumbers(std::vector<int>());
				m_player[getPartner(i)].setOutgoingSwapCardNumbers(std::vector<int>());

				someonePassed = true;
				activePlayers[i] = true;
				activePlayers[getPartner(i)] = true;
				sendMessageToPlayer(SpadesGameMessage(
					REQUEST_CARD_SWAP_S,i));
				sendMessageToPlayer(SpadesGameMessage(
					REQUEST_CARD_SWAP_S,getPartner(i)));

				playerRequestDoAction(i);
				playerRequestDoAction(getPartner(i));
			}
			else
			{
				setWaitBool(i,true);
				setWaitBool(getPartner(i),true);
			}
		}

		if(someonePassed)
		{
			sendActivePlayers(activePlayers[0],activePlayers[1],activePlayers[2],activePlayers[3]);
		}
		setState(WAITING_FOR_SWAP_POS);
	}

	void SpadesGame::msgGotSwappedCardsFromPlayer( const SpadesGameMessage& msg )
	{
		//set incoming and outgoing cards
		std::vector<int> swappers(2,-1);
		if(msg.getArgs().size() > 1)
		{
			int index1 = m_player[msg.getPlayer()].getIndexFromCardNumber(msg.getArgs()[0]);
			int index2 = m_player[msg.getPlayer()].getIndexFromCardNumber(msg.getArgs()[1]);
			swappers[0] = getLegalCardIndexForPlayer(msg.getPlayer(),index1,-1);
			swappers[1] = getLegalCardIndexForPlayer(msg.getPlayer(),index2,swappers[0]);
		}
		else
		{
			swappers[0] = getLegalCardIndexForPlayer(msg.getPlayer(),-1,-1);
			swappers[1] = getLegalCardIndexForPlayer(msg.getPlayer(),-1,swappers[0]);
		}

		swappers[0] = m_player[msg.getPlayer()].getCardNumberOf(swappers[0]);
		swappers[1] = m_player[msg.getPlayer()].getCardNumberOf(swappers[1]);

		m_player[getPartner(msg.getPlayer())].setIncomingSwapCardNumbers(swappers);
		m_player[msg.getPlayer()].setOutgoingSwapCardNumbers(swappers);
		setWaitBool(msg.getPlayer(),true);
		playerFinishedAction(msg.getPlayer());

		if(isWaitBoolFinished())
		{
			std::vector<int> swapCards;
			swapCards.reserve(4);
			if(getNumberOfTeamsPassingTwo() == 2)
			{
				//ask each player to swap the cards and wait
				for(int i = 0; i < 4; ++i)
				{
					if(!m_player[i].isAlive())
					{
						continue;
					}
					swapCards.clear();

					//add the cards

					swapCards.push_back(0); //Both are swapping
					for(size_t j = 0; j < m_player[i].getIncomingSwapCardNumbers().size(); ++j)
					{
						swapCards.push_back(m_player[i].getIncomingSwapCardNumbers()[j]);
					}

					for(size_t j = 0; j < m_player[i].getOutgoingSwapCardNumbers().size(); ++j)
					{
						swapCards.push_back(m_player[i].getOutgoingSwapCardNumbers()[j]);
					}

					sendMessageToPlayer(SpadesGameMessage(PERFORM_SWAP_S,swapCards,i));
				}
			}
			else
			{
				//either 0 or 1 is swapping
				int swapTeam = isTeamPassingTwo(0) ? 0 : 1;
				int tbTeam[2];
				int lrTeam[2];
				tbTeam[0] = swapTeam;
				tbTeam[1] = getPartner(swapTeam);

				lrTeam[0] = getNextPlayer(swapTeam);
				lrTeam[1] = getPartner(lrTeam[0]);

				for(int i = 0; i < 2; ++i)
				{
					swapCards.clear();

					//add the cards
					swapCards.push_back(1); //Top-Bottom only swap
					for(size_t j = 0; j < m_player[tbTeam[i]].getIncomingSwapCardNumbers().size(); ++j)
					{
						swapCards.push_back(m_player[tbTeam[i]].getIncomingSwapCardNumbers()[j]);
					}

					for(size_t j = 0; j < m_player[tbTeam[i]].getOutgoingSwapCardNumbers().size(); ++j)
					{
						swapCards.push_back(m_player[tbTeam[i]].getOutgoingSwapCardNumbers()[j]);
					}

					sendMessageToPlayer(SpadesGameMessage(PERFORM_SWAP_S,swapCards,tbTeam[i]));
				}

				swapCards.clear();
				swapCards.push_back(2); //Left to right swap

				//team that did not pass 2 wont swap
				for(int i = 0; i < 2; ++i)
				{
					sendMessageToPlayer(SpadesGameMessage(PERFORM_SWAP_S,swapCards,lrTeam[i]));
				}
			}
	
			//wait for everyone to have performed it
			setState(WAITING_FOR_SWAP_PERFORMED);
			clearWaitBools();

			sendActivePlayer(-1);
		}
	}

	void SpadesGame::msgFinishedCardSwap( const SpadesGameMessage& msg )
	{
		setWaitBool(msg.getPlayer(),true);
		m_player[msg.getPlayer()].swapCards();

		if(isWaitBoolFinished())
		{
			//tell everyone who the active player is
			//ask active player to play a card
			doRequestForCard();
		}
	}

	void SpadesGame::doRequestForCard()
	{
		if(m_tableCards.empty())
		{
			m_cardOffset = getActivePlayer();
		}
		
		//tell everyone who the active player is
		sendActivePlayer(getActivePlayer());

		//break spades if that is all he has
		if(m_player[getActivePlayer()].getCardCount() == 
			m_player[getActivePlayer()].getQuantityOfSuit(Card::SPADES))
		{
			setSpadesBroken(true);
		}

		//filter the active player's cards
		sendMessageToPlayer(SpadesGameMessage(
			FILTER_CARDS_S,
			doCardFiltering(getActivePlayer())
			,getActivePlayer()));

		//ask active player for his card
		sendMessageToPlayer(SpadesGameMessage(
			REQUEST_SELECTED_CARD_S,
			getActivePlayer()));
		setCardRequester(getActivePlayer());
		//wait until everyone has shown the animation of card on table
		clearWaitBools();

		setState(WAITING_FOR_PLAYER_CARD);

		playerRequestDoAction(getActivePlayer());

	}

	std::vector<int> SpadesGame::doCardFiltering(int player)
	{
		//fresh round, only filter spades if it has not been broken
		if(m_tableCards.empty())
		{
			if(!m_player[player].onlyHasSpades() && !isSpadesBroken() && getState() != WAITING_FOR_SWAP_POS)
			return m_player[player].getIndexesOfSuit(Card::SPADES);
			else
			return std::vector<int>(); //no filter
		}
		else
		{
			//limit player to the leading suit
			if(m_player[player].hasSuit(m_tableCards[0].getSuit()))
			{
				return m_player[player].getIndexesOfNotSuit(m_tableCards[0].getSuit());
			}
			else //let the player play anything
			{
				return std::vector<int>(); //no filter
			}
		}
	}

	bool SpadesGame::isSpadesBroken() const
	{
		return m_spadesBroken;
	}

	void SpadesGame::setSpadesBroken( bool broken )
	{
		m_spadesBroken = broken;
	}

	void SpadesGame::msgGotTableCardFromPlayer( const SpadesGameMessage& msg )
	{
			//get the card from the player
		int properIndex = 0;
		if(msg.getArgs().size() > 0)
			properIndex = getLegalCardIndexForPlayer(msg.getPlayer(),msg.getArgs()[0]);
		else
		properIndex = getLegalCardIndexForPlayer(msg.getPlayer(),-1);

			Card card = Card::cardFromNumber(
			m_player[msg.getPlayer()].getCardNumberOf(properIndex));

			//remove the card
			m_player[msg.getPlayer()].removeCard(properIndex);

			//add the card to the table
			m_tableCards.push_back(card);

			setOrderedCard(card.getCardNumber(),msg.getPlayer());

			//break spades if it is a spade
			if(card.getSuit() == Card::SPADES)
			{
				setSpadesBroken(true);
			}
			
			//tell everyone about it
			std::vector<int> cardAndPlayer(2,0);
			for(int i = 0; i < m_gameInfo.getNumPlayers(); ++i)
			{
				cardAndPlayer[0] = card.getCardNumber();
				cardAndPlayer[1] = mapServerToClientPos(i,msg.getPlayer());

				//no need to tell the player, he already did it
				//but send it to the watchers
				if(i == msg.getPlayer())
				{
					if(msg.getPlayer() == 0)
					{
						sendMessageToPlayer(
							SpadesGameMessage(
							PERFORM_CARD_ON_TABLE_A_D_S,
							cardAndPlayer,
							-1));
					}	
				}
				else
				{
					sendMessageToPlayer(
						SpadesGameMessage(
						PERFORM_CARD_ON_TABLE_A_D_S,
						cardAndPlayer,
						i));
				}

			}

			//wait bools already set in card request
			setState(WAITING_FOR_CARD_THROWN);
			playerFinishedAction(msg.getPlayer());
	}

	void SpadesGame::msgPerformedCardOnTable( const SpadesGameMessage& msg )
	{
		setWaitBool(msg.getPlayer(),true);
		if(isWaitBoolFinished())
		{
			//let everyone play a card
			if(m_tableCards.size() != m_gameInfo.getNumPlayers())
			{
				//let the next player throw a card
				setActivePlayer(getNextPlayer(getActivePlayer()));
				doRequestForCard();
			}
			else
			{
				//we are ready to give someone a point
				doGivePoint();
			}
		}
	}

	int SpadesGame::getWinningCardIndex( const std::vector<Card>& cards ) const
	{
		if(cards.empty())
		{
			return -1;
		}

		std::vector<std::pair<Card,int> > cardPairs;
		//keep track of the indexes
		for(size_t i = 0; i < cards.size(); ++i)
		{
			cardPairs.push_back(std::make_pair(cards[i],i));
		}

		//are there any spades
		int numSpades = 0;

		for(size_t i = 0; i < cardPairs.size(); ++i)
		{
			if(cardPairs[i].first.getSuit() == Card::SPADES)
			{
				numSpades++;
			}
		}

		//if there are spades only keep the spades
		//otherwise keep only cards of the leading suit
		Card::SuitEnum suit = numSpades > 0 ? Card::SPADES : cardPairs[0].first.getSuit();

		//filter out cards that have no chance of winning
		for(int i = 0; i < m_gameInfo.getNumPlayers(); ++i)
		{
			for(size_t j = 0; j < cardPairs.size(); ++j)
			{
				if(cardPairs[j].first.getSuit() != suit)
				{
					cardPairs.erase(cardPairs.begin() + j);
					break;
				}
			}
		}

		//find the highest ranked card
		int largest = 0;
		for(size_t i = 0; i < cardPairs.size(); ++i)
		{
			if(cardPairs[i].first.getRank() > cardPairs[largest].first.getRank())
			{
				largest = i;
			}
		}

		//return the index of the highest ranked card
		return cardPairs[largest].second;

	}

	int SpadesGame::getCardWinnerPlayer() const
	{
		if(m_tableCards.empty())
		{
			return -1;
		}
		
		int tableIndex = getWinningCardIndex(m_tableCards);

		return (getActivePlayer() + tableIndex + 1) %
			m_gameInfo.getNumPlayers();
	}

	void SpadesGame::doGivePoint()
	{
		int winner = getCardWinnerPlayer();

		int pointReceiver = winner;
		SpadesPointEnum point = 
			m_player[pointReceiver].hasMadeBid() ? OVERTRICK_POINT : REGULAR_POINT;

		//if the winner has made all his tricks but the partner has not,
		//give the parter the point
		if(m_gameInfo.isPartnership() &&
			m_player[winner].hasMadeBid() && 
			m_player[winner].getRoundBid() > 0 &&
			!m_player[getPartner(winner)].hasMadeBid())
		{
			pointReceiver = getPartner(winner);
			point = POINT_FROM_PARTNER;
		}

		//give a point
		m_player[pointReceiver].addRoundTrick(point);

		//set the last point receiver for use when making a new point
		setLastPointWinner(winner);
		setLastPointReceiver(pointReceiver);

		//we now need to tell everyone to give the cards and point
		//and wait for that to be finished
		std::vector<int> pointArgs(3); //int sendCardsTo, int givePointTo, point enum
		pointArgs[2] = (int)point;

		for(int i = 0; i < m_gameInfo.getNumPlayers(); ++i)
		{
			pointArgs[0] = mapServerToClientPos(i,winner);
			pointArgs[1] = mapServerToClientPos(i,pointReceiver);

			sendMessageToPlayer(SpadesGameMessage(PLAYER_MADE_TRICK_A_D_S,pointArgs,i));
		}

		clearWaitBools();
		setState(WAITING_FOR_POINT_GIVEN);

		if(m_rawListener) {
			std::vector<Team> t;
			std::vector<Player> p;
			for(size_t i = 0; i < getNumberOfTeams(); i++)
				t.push_back(m_team[i]);
			for(size_t i = 0; i < m_gameInfo.getNumPlayers(); i++)
				p.push_back(m_player[i]);
			m_rawListener->playerMadeTrick(t,p,winner);
		}

	}

	void SpadesGame::msgPerformedGivePoint( const SpadesGameMessage& msg )
	{
		if(getState() != WAITING_FOR_POINT_GIVEN)
			return;

		setWaitBool(msg.getPlayer(),true);

		//the player has been given a point
		//either go for another point or end the round
		if(isWaitBoolFinished())
		{
			createPriorHand();
			if(m_player[0].getCardCount() > 0)
			{
				setActivePlayer(getLastPointWinner());
				m_tableCards.clear();
				clearOrderedCards();
				doRequestForCard();
			}
			else
			{
				doEndOfRound();
			}
		}
	}

	void SpadesGame::doNewRound()
	{
		for(int i = 0; i < m_gameInfo.getNumPlayers(); i++)
		{
			m_player[i].clearRoundTricks();
			m_player[i].setRoundBid(-1);
			m_player[i].setIncomingSwapCardNumbers(std::vector<int>());
			m_player[i].setOutgoingSwapCardNumbers(std::vector<int>());
		}

		m_swappingPlayers.clear();

		m_roundActivePlayer++;
		setActivePlayer((m_roundActivePlayer - 1) % m_gameInfo.getNumPlayers());
		m_tableCards.clear();
		clearOrderedCards();
		setSpadesBroken(false);
		m_priorCards.clear();

		sendMessageToPlayer(SpadesGameMessage(ROUND_BEGIN_S,0),true);
		doRequestDeckCutPos();

		if(m_rawListener) {
			std::vector<Team> t;
			std::vector<Player> p;
			for(size_t i = 0; i < getNumberOfTeams(); i++)
				t.push_back(m_team[i]);
			for(size_t i = 0; i < m_gameInfo.getNumPlayers(); i++)
				p.push_back(m_player[i]);
			m_rawListener->roundBegan(t,p);
		}
	}

	void SpadesGame::doRequestDeckCutPos()
	{
		if(m_gameInfo.isDeckCutEnabled())
		{
			sendMessageToPlayer(SpadesGameMessage(REQUEST_DECK_CUT_POS_S,getDeckCutter()));
			sendActivePlayer(getDeckCutter());
			playerRequestDoAction(getDeckCutter());
		}
		else
		{
			std::vector<int> cutPos(1,0); // fake deck cut pos
			pushMessage(SpadesGameMessage(SEND_DECK_CUT_POS_C,cutPos,getDeckCutter()));
		}

		setState(WAITING_FOR_DECK_CUT_POS);
	}

	void SpadesGame::setLastPointWinner( int receiver )
	{
		m_lastPointWinner = receiver;
	}

	int SpadesGame::getLastPointWinner() const
	{
		return m_lastPointWinner;
	}

	void SpadesGame::doNewGame()
	{
		m_roundActivePlayer = 
			m_cardShuffler.nextRangeInt(0, m_gameInfo.getNumPlayers());
		setActivePlayer(m_roundActivePlayer);

		for(int i = 0; i < m_gameInfo.getNumPlayers(); i++)
		{
			m_player[i].clearRoundTricks();
			m_player[i].setRoundBid(-1);
		}

		for(int i = 0; i < m_gameInfo.getNumPlayers(); i++)
		{
			m_team[i].clearBags();
			m_team[i].clearScore();
		}

		m_handCount = 0;

		if(m_gameInfo.m_endCondition == TIME_OUT)
		{
			setEndTime(m_gameInfo.m_endConditionValue);
		}
		else
		{
			setEndTime(0);
		}

		clearWaitBools();

		if(m_rawListener)
			m_rawListener->newGame(m_gameInfo);
	}

	bool SpadesGame::isTimeExpired() const
	{
		return m_time > m_endTimeStamp;
	}

	void SpadesGame::setEndTime( int numMins )
	{
		int seconds = numMins * 60;
		m_endTimeStamp = seconds;
	}

	int SpadesGame::getRoundScoreForTeam( int teamNo,int numBagBusts )
	{
		const int BAG_BUST_VAL = -100;
		const int POINT_VAL = 10;
		const int OVERTRICK_VAL = 1;

		int score = 0;
		if((m_gameInfo.isIndividual() || m_gameInfo.isCutthroat()) && 
			teamNo >= 0 && teamNo < m_gameInfo.getNumPlayers())
		{
			//too many bags causes you to lose points
			
			score += numBagBusts * BAG_BUST_VAL;

			//made his nil
			if(m_player[teamNo].getRoundBid() == 0 && 
				m_player[teamNo].getRoundTricks() == 0)
			{
				score += m_gameInfo.m_nilValue;
			}
			//nil failed
			else if((m_player[teamNo].getRoundBid() == 0 && 
				m_player[teamNo].getRoundTricks() > 0))
			{
				//lose nil amount
				score -= m_gameInfo.m_nilValue;

				//point for each bag
				score += (OVERTRICK_VAL *  m_player[teamNo].getRoundTricks());
			}
			else if(m_player[teamNo].getRoundBid() > 0)
			{
				//he did not make his bid
				if(m_player[teamNo].getRoundTricks() < 
					m_player[teamNo].getRoundBid())
				{
					score -= POINT_VAL * m_player[teamNo].getRoundBid();
				}
				else if(m_player[teamNo].getRoundTricks() >= 
					m_player[teamNo].getRoundBid())
				{
					//point for each bag
					score += OVERTRICK_VAL *  
						(m_player[teamNo].getRoundTricks() - 
						m_player[teamNo].getRoundBid());
					//points for each trick
					score += POINT_VAL * m_player[teamNo].getRoundBid();
				}
			}
		}
		else if(m_gameInfo.isPartnership() && (teamNo == 0 || teamNo == 1))
		{
			//bags hurt
			score += numBagBusts * BAG_BUST_VAL;

			int p1 = teamNo;
			int p2 = getPartner(p1);

			int contractTotal = 
				m_player[p1].getRoundBid() + m_player[p2].getRoundBid();

			int trickTotal = 0;

			int players[] = {p1,p2}; 

			//get the number of tricks each made
			for(int i = 0; i < 2; ++i)
			{
				if(m_player[players[i]].getRoundBid() != 0)
				{
					trickTotal += m_player[players[i]].getRoundTricks();
				}
			}

			//deal with nil case
			for(int i = 0; i < 2; ++i)
			{
				if(m_player[players[i]].getRoundBid() == 0)
				{
					if(m_player[players[i]].getRoundTricks() == 0)
					{
						score += m_gameInfo.m_nilValue;
					}
					else
					{
						score -= m_gameInfo.m_nilValue;
						score += OVERTRICK_VAL * m_player[players[i]].getRoundTricks();
					}
				}
			}

			//made the contract
			if(trickTotal >= contractTotal)
			{
				//points for tricks
				score += contractTotal * POINT_VAL;
				//point for bag
				score += (trickTotal - contractTotal) * OVERTRICK_VAL;
			}
			else //did not make contract
			{
				//lose points for tricks
				score -= contractTotal * POINT_VAL;
			}
		}

		return score;
	}

	int SpadesGame::getRoundBagsForTeam( int teamNo )
	{
		if((m_gameInfo.isIndividual() || m_gameInfo.isCutthroat()) && 
			teamNo >= 0 && teamNo < m_gameInfo.getNumPlayers())
		{
			if(m_player[teamNo].getRoundTricks() >= m_player[teamNo].getRoundBid())
			{
				return m_player[teamNo].getRoundTricks() - 
					m_player[teamNo].getRoundBid();
			}
			else
			{
				return 0;
			}
		}
		else if(m_gameInfo.isPartnership() && (teamNo == 0 || teamNo == 1))
		{
			int p1 = teamNo;
			int p2 = getPartner(p1);
			int bags = 0;

			int contractTotal = 
				m_player[p1].getRoundBid() + m_player[p2].getRoundBid();

			int trickTotal = 0;

			int players[] = {p1,p2}; 

			//get the number of tricks each made
			for(int i = 0; i < 2; ++i)
			{
				if(m_player[players[i]].getRoundBid() != 0)
				{
					trickTotal += m_player[players[i]].getRoundTricks();
				}
				else
				{
					bags += m_player[players[i]].getRoundTricks();
				}
			}

			if(trickTotal >= contractTotal)
			{
				return (trickTotal - contractTotal) + bags;
			}
			else if(bags > 0)
			{
				return bags;
			}
			else
			{
				return 0;
			}
		}

		//error
		return -1;
	}

	bool SpadesGame::isGameOver()
	{
		if(getMoonshotWinnerPlayer() != -1)
		{
			 //moonshot overrules everything!
			//end the game!
			return true;
		}

		int winner = getWinner();
		int loser = getLoser();

		//if there is a draw, keep playing
		if(winner == -1)
		{
			return false;
		}

		switch(m_gameInfo.m_endCondition)
		{
		case TIME_OUT:
			{
				return isTimeExpired() && m_handCount >= 2;
			}
			break;
		case HAND_LIMIT_REACHED:
			{
				return m_handCount >= m_gameInfo.m_endConditionValue;
			}
			break;
		case SCORE_REACHED:
			{
				return m_team[winner].getScore() >= m_gameInfo.m_endConditionValue ||
					(m_team[loser].getScore() <= -m_gameInfo.m_endConditionValue &&
					(getGameInfo().isPartnership() || getGameInfo().isPassTwo()));
			}
			break;
		}

		return false;
	}

	int SpadesGame::getWinner()
	{
		//returns -1 on draw

		//get number of players or 2 teams
		int numPlayers = getNumberOfTeams();

		int highest = m_team[0].getScore();
		int pNum = 0;

		for(int i = 0; i < numPlayers; ++i)
		{
			//find the 'winner'
			if(m_team[i].getScore() > highest)
			{
				highest = m_team[i].getScore();
				pNum = i;
			}
		}

		//check for draw
		for(int i = 0; i < numPlayers; ++i)
		{
			//it's a draw, return -1
			if(m_team[i].getScore() == highest && i != pNum)
			{
				m_lastGameWinner = -1;
				return -1;
			}
		}

		m_lastGameWinner = pNum;
		return pNum;
	}

	int SpadesGame::getLoser()
	{
		//get number of players or 2 teams
		int numPlayers = getNumberOfTeams();

		int lowest = m_team[0].getScore();
		int pNum = 0;

		for(int i = 0; i < numPlayers; ++i)
		{
			//find the 'loser'
			if(m_team[i].getScore() < lowest)
			{
				lowest = m_team[i].getScore();
				pNum = i;
			}
		}

		return pNum;
	}

	void SpadesGame::updateRoundScores()
	{
		const int MAX_BAGS = 10;

		//get number of players or 2 teams
		int numPlayers = getNumberOfTeams();

		for(int i = 0; i < numPlayers; i++)
		{
			int numBagBusts = 0;
			int bags;
			int totalBags;

			bags = getRoundBagsForTeam(i);
			totalBags = m_team[i].getBags() + bags;

			//how many times do they lose points?
			numBagBusts = totalBags / MAX_BAGS;

			//calculate new bag count
			int newBagCount = totalBags % MAX_BAGS;
			m_team[i].setBags(newBagCount);

			//calculate the score
			int scoreIncrease = getRoundScoreForTeam(i,numBagBusts);
			m_team[i].increaseScore(scoreIncrease);
		}
	}

	void SpadesGame::doEndOfRound()
	{
		//another round is complete
		m_handCount++;

		//update scores
		updateRoundScores();

		//send each player the team scores
		std::vector<int> scores;
		for(int i = 0; i < getNumberOfTeams(); ++i)
		{
			scores.push_back(m_team[i].getScore());
		}

		//send each player the team bags
		for(int i = 0; i < getNumberOfTeams(); ++i)
		{
			scores.push_back(m_team[i].getBags());
		}

		sendMessageToPlayer(
			SpadesGameMessage(SEND_ROUND_SCORES_D_S,scores,0),true);

		sendActivePlayer(-1);

		if(m_rawListener) {
			std::vector<Team> t;
			std::vector<Player> p;
			for(size_t i = 0; i < getNumberOfTeams(); i++)
				t.push_back(m_team[i]);
			for(size_t i = 0; i < m_gameInfo.getNumPlayers(); i++)
				p.push_back(m_player[i]);
			m_rawListener->roundEnded(t,p);
		}

		if(isGameOver())
		{
			//TODO : CODE TO END GAME
			doGameOver();
		}
		else
		{
			doNewRound();
		}
	}

	int SpadesGame::getNumberOfTeams() const
	{
		if(m_gameInfo.isIndividual() || m_gameInfo.isCutthroat())
		{
			return m_gameInfo.getNumPlayers();
		}
		
		return 2;
	}

	int SpadesGame::getMoonshotWinnerPlayer()
	{
		int moonshotTeam = -1;

		//check for moonshot
		if(m_gameInfo.isMoonshotEnabled())
		{
			if(m_gameInfo.isPartnership())
			{
				for(int i = 0; i < 2; ++i)
				{
					int numContract = 0;
					numContract += m_player[i].getRoundBid();
					numContract += m_player[getPartner(i)].getRoundBid();

					int numScored = 0;
					numScored += m_player[i].getRoundTricks();
					numScored += m_player[getPartner(i)].getRoundTricks();

					if(numContract  == m_gameInfo.getMaxRoundPoints() &&
						numScored == m_gameInfo.getMaxRoundPoints())
					{
						moonshotTeam = i;
						break;
					}
				}
			}
			else
			{
				for(int i = 0; i < getNumberOfTeams(); ++i)
				{
					if(m_player[i].getRoundBid() == m_gameInfo.getMaxRoundPoints() &&
						m_player[i].getRoundTricks() == m_gameInfo.getMaxRoundPoints())
					{
						moonshotTeam = i;
						break;
					}
				}
			}
		}

		m_lastMoonshotWinner = moonshotTeam;
		m_lastGameWinner = moonshotTeam;
		return moonshotTeam;
	}

	bool SpadesGame::isTeamPassingTwo( int teamNo ) const
	{
		return m_player[teamNo].getRoundBid() == 0 ||
			m_player[getPartner(teamNo)].getRoundBid() == 0;
	}

	int SpadesGame::getNumberOfTeamsPassingTwo() const
	{
		int count = 0;

		for(int i = 0; i < getNumberOfTeams(); ++i)
		{
			if(isTeamPassingTwo(i))
			{
				count++;
			}
		}

		return count;
	}

	cge::GameClientState SpadesGame::generateState( int player,bool watching, int giftIDs[4], int giftFrames[4] )
	{
		GameClientState state;

		std::vector<int> teamScores;
		std::vector<int> teamBags;
		for(int i = 0; i < getNumberOfTeams(); ++i)
		{
			teamScores.push_back(m_team[i].getScore());
			teamBags.push_back(m_team[i].getBags());
		}

		std::vector<int> priorCards;
		if(m_priorCards.size() == m_gameInfo.getNumPlayers())
		{
			for(size_t i = 0; i < m_priorCards.size();++i)
			{
				int p = mapClientToServerLinear(player,i);
				priorCards.push_back(m_priorCards[p]);
			}
		}

		std::vector<int> tableCards;
		tableCards.resize(4,-1);

		for(size_t i = 0; i < m_orderedTableCards.size(); ++i)
		{
			int p = mapServerToClientPos(player,i);
			tableCards[p] = m_orderedTableCards[i];

		}

		std::vector<int> playerCards;
		if(watching)
		{
			playerCards.push_back(m_player[player].getCardCount());
			//if I played a card, add it
			if(tableCards[0] != -1)
			{
				playerCards.back()++;
			}
		}
		else
		{
			playerCards = m_player[player].getCardNumbers();

			//if I played a card, add it
			if(tableCards[0] != -1)
			{
				std::vector<Card> actualCards;
				for(size_t i = 0; i < playerCards.size(); ++i)
				{
					actualCards.push_back(Card::cardFromNumber(playerCards[i]));
				}

				actualCards.push_back(Card::cardFromNumber(tableCards[0]));

				std::sort(actualCards.begin(),actualCards.end());
				playerCards.clear();
				for(size_t i = 0; i < actualCards.size(); ++i)
				{
					playerCards.push_back(actualCards[i].getCardNumber());
				}
			}
		}

		std::vector<int> cardQuantities;
		cardQuantities.resize(4,0);

		for(int i = 0; i < 4;++i)
		{
			int p = mapServerToClientPos(player,i);

			cardQuantities[p] = m_player[i].getCardCount();

			//increase if on table
			if(m_orderedTableCards[i] != -1)
			{
				cardQuantities[p]++;
			}
		}


		std::vector<int> filteredCards;
		if(m_state == WAITING_FOR_PLAYER_CARD && m_activePlayer == player && !watching)
		{
			filteredCards = doCardFiltering(player);
		}


		std::vector<SpadesPointEnum> points[4];
		for (int i = 0; i < 4; ++i)
		{
			int p = mapServerToClientPos(player,i);
			points[p] = m_player[i].getRoundTricksVec();
		}

			GameStateEnum gstate = GAMESTATE_DEFAULT;

			if(!watching && getState() == WAITING_FOR_PLAYER_TRICK_COUNT && m_requestedBidFromPlayer == player)
			{
				gstate = GAMESTATE_NEED_TO_BID;
			}
			else if(getState() == WAITING_FOR_PLAYER_TRICK_COUNT && m_requestedBidFromPlayer != player)
			{
				if(m_player[player].hasBid())
				{
					gstate = GAMESTATE_WAITING_TO_PLAY_CARD;
				}
				else
				{
					gstate = GAMESTATE_WAITING_TO_BID;
				}
			}
			else if(!watching && getState() == WAITING_FOR_SWAP_POS && 
				m_player[player].getOutgoingSwapCardNumbers().size() == 0 && isSwappingPlayer(player))
			{
				gstate = GAMESTATE_NEED_CHOOSE_2_CARDS;
			}
			else if(getState() == WAITING_FOR_SWAP_POS)
			{
				gstate = GAMESTATE_WAITING_TO_PASS_CARDS;
			}
			else if(getState() == WAITING_FOR_DECK_CUT_POS && getDeckCutter() == player)
			{
				gstate = GAMESTATE_NEED_TO_CUT_DECK;
			}
			else if(getState() == WAITING_FOR_DECK_CUT_POS && getDeckCutter() != player)
			{
				gstate = GAMESTATE_WAITING_FOR_DECKCUT_POS;
			}
			else if(!watching && (getState() == WAITING_FOR_CARD_THROWN || getState() == WAITING_FOR_PLAYER_CARD) && m_requestCardFromPlayer == player)
			{
				gstate = GAMESTATE_NEED_TO_PLAY_CARD;
			}
			else if((getState() == WAITING_FOR_CARD_THROWN || getState() == WAITING_FOR_PLAYER_CARD) && m_requestCardFromPlayer != player)
			{
				gstate = GAMESTATE_WAITING_TO_PLAY_CARD;
			}
			else if(getState() == WAITING_FOR_CARDS_DISPATCHED)
			{
				gstate = GAMESTATE_WAITING_TO_BID;
			}
			else if(getState() == WAITING_FOR_SWAP_PERFORMED)
			{
				gstate = GAMESTATE_WAITING_TO_PLAY_CARD;
			}
			else if(getState() == WAITING_FOR_POINT_GIVEN && !m_waitBools[player])
			{
				gstate = GAMESTATE_WAITING_TO_PLAY_CARD;
			}
			int bidder = mapServerToClientPos(player,m_requestedBidFromPlayer);

			std::vector<int> roundBids;
			roundBids.resize(4,0);

			for(int i = 0; i < 4; ++i)
			{
				int p = mapServerToClientPos(player,i);
				if(i == 3 && m_gameInfo.getNumPlayers() == 3)
					roundBids[p] = -1;
				else
					roundBids[p] = m_player[i].getRoundBid();
				
			}

			std::vector<int> inSwap;
			std::vector<int> outSwap;

			//only send card swaps if you have both
			if(m_player[player].getOutgoingSwapCardNumbers().size()
				== m_player[player].getIncomingSwapCardNumbers().size())
			{
				outSwap = m_player[player].getOutgoingSwapCardNumbers();
				inSwap = m_player[player].getIncomingSwapCardNumbers();

				if(watching)
				{
					for(size_t i = 0; i < outSwap.size(); ++i)
					{
						outSwap[i] = -1;
						inSwap[i] = -1;
					}
				}
			}

			int firstPlayer = -1;
			if(m_tableCards.size() > 0)
			{
				firstPlayer = mapServerToClientPos(player,m_cardOffset);
			}

			int activePlayer = mapServerToClientPos(player,getActivePlayer());
			int dealer = mapServerToClientPos(player,getDealer());

			int lastWinner = mapServerToClientPos(player,getLastPointWinner());
			int lastReceiver = mapServerToClientPos(player,getLastPointReceiver());

			bool activePoses[4];
			int giftPos[4];
			int framePos[4];
			for(int i = 0; i < 4; i++)
			{
				activePoses[mapServerToClientPos(player,i)] = m_pActive[i];
				giftPos[mapServerToClientPos(player,i)] = giftIDs[i];
				framePos[mapServerToClientPos(player,i)] = giftFrames[i];
			}

		state = GameClientState(gstate,m_gameInfo.getNumPlayers(),playerCards,
			cardQuantities,teamScores,teamBags,m_player[player].getOutgoingSwapCardNumbers(),
			m_player[player].getIncomingSwapCardNumbers(),priorCards,filteredCards,
			tableCards,points[0],points[1],points[2],points[3],m_handCount,dealer,
			activePlayer,isSpadesBroken(),!watching,roundBids,bidder,firstPlayer,
			lastWinner,lastReceiver,getLastPointType(),activePoses,giftPos,framePos);

		return state;
	}

	void SpadesGame::createPriorHand()
	{
		m_priorCards.clear();
		//regardless of the state, the last hand needs to be updated
		std::vector<int> realLastHand;

		int actualActive = m_cardOffset;

		//put them in the right order
		for(int i = (int)m_tableCards.size() - actualActive; i < m_tableCards.size(); ++i)
		{
			realLastHand.push_back(m_tableCards[i].getCardNumber());
		}

		for(int i = 0; i < (int)m_tableCards.size() - actualActive; ++i)
		{
			realLastHand.push_back(m_tableCards[i].getCardNumber());
		}

		m_priorCards = realLastHand;
	}

	void SpadesGame::setRequestBidder( int player )
	{
		m_requestedBidFromPlayer = player;
		playerRequestDoAction(player);
	}

	void SpadesGame::setCardRequester( int player )
	{
		m_requestCardFromPlayer = player;
	}

	bool SpadesGame::isSwappingPlayer( int player ) const
	{
		for(size_t i = 0; i < m_swappingPlayers.size(); ++i)
		{
			if(m_swappingPlayers[i] == player)
			{
				return true;
			}
		}

		return false;
	}

	bool SpadesGame::clientReconnected( int player )
	{
		bool didSomething = false;

			didSomething = false;

			if(getState() == WAITING_FOR_CARDS_DISPATCHED && !m_waitBools[player])
			{
				pushMessage(SpadesGameMessage(FINISH_DISPATCH_CARDS_C,player));
				didSomething = true;
			}
			else if(getState() == WAITING_FOR_SWAP_PERFORMED && !m_waitBools[player])
			{
				pushMessage(SpadesGameMessage(FINISHED_CARD_SWAP_C,player));
				didSomething = true;
			}
			else if(getState() == WAITING_FOR_CARD_THROWN && !m_waitBools[player])
			{
				pushMessage(SpadesGameMessage(FINISH_PERFORM_CARD_ON_TABLE_C,player));
				didSomething = true;
			}
			else if(getState() == WAITING_FOR_POINT_GIVEN && !m_waitBools[player])
			{
				pushMessage(SpadesGameMessage(FINISHED_PLAYER_MADE_TRICK_C,player));
				didSomething = true;
			}
			else if(getState() == WAITING_FOR_NUM_PLAYERS) {
				pushMessage(SpadesGameMessage(REPLY_INIT_GAME_C,player));
				didSomething = true;
			}

			processMessages();

			return didSomething;

	}

	int SpadesGame::mapClientToServerLinear( int serverClient,int clientPos ) const
	{
		//assumes there is no top player
		if(m_gameInfo.getNumPlayers() == 3)
		{

			if(clientPos == 0)
			{
				return serverClient;
			}
			else if(clientPos == 1)
			{
				return (3 + serverClient + 1) % 3;
			}
			else if(clientPos == 2)
			{
				return (3 + serverClient - 1) % 3;
			}
			else
			{
				return 3; //3 is always dead on the server in 3 p spades
			}
		}
		else
		{
			//circular shift
			return (4 + serverClient + clientPos) % 4;
		}
	}

	void SpadesGame::setOrderedCard( int card, int pos )
	{
		m_orderedTableCards[pos] = card;
	}

	void SpadesGame::clearOrderedCards()
	{
		for(size_t i = 0; i < m_orderedTableCards.size(); ++i)
		{
			m_orderedTableCards[i] = -1;
		}
	}

	void SpadesGame::reset()
	{
		m_activePlayer = 1;
		m_time = 0;
		m_host = 0;
		m_gameInfoIsSet = false;
		m_deckCutPos = 0;
		m_spadesBroken = false;
		m_state = WAITING_FOR_NUM_PLAYERS;
		m_lastPointWinner = 0;
		m_roundActivePlayer = 0;
		m_handCount = 0; 
		m_endTimeStamp = 0;
		m_requestedBidFromPlayer = -1;
		m_requestCardFromPlayer = -1;
		m_cardOffset = 0;
		m_lastMoonshotWinner = -1;
		int m_lastGameWinner = -1;
		for(int i = 0; i < 4; i++)
		{
			m_pActive[i] = false;
		}
		clearWaitBools();
		m_orderedTableCards.resize(4,-1);

		for(int i = 0; i < 4; ++i)
		{
			m_player[i] = Player();
			m_team[i] = Team();
		}

		m_tableCards.clear();
		m_priorCards.clear();
		m_swappingPlayers.clear();
	}

	const SpadesGameInfo& SpadesGame::getGameInfo() const
	{
		return m_gameInfo;
	}

	void SpadesGame::doGameOver()
	{
		std::vector<int> over; //win team, bool moonshot win
		if(m_lastMoonshotWinner != -1)
		{
			over.push_back(m_lastMoonshotWinner);
			over.push_back(1);
		}
		else
		{
			over.push_back(m_lastGameWinner);
			over.push_back(0);
		}

		m_winningTeams.clear();
		m_finalScores.clear();

		m_winningTeams.push_back(m_lastGameWinner);
		m_finalScores.push_back(m_team[m_lastGameWinner].getScore());

		std::vector<std::pair<int,int> > teamScore;
		for(int i = 0; i < getNumberOfTeams(); ++i)
		{
			if(i != m_lastGameWinner)
			{
				teamScore.push_back(std::make_pair(m_team[i].getScore(),i));
			}
		}

		std::sort(teamScore.rbegin(),teamScore.rend());
		for(int i = 0; i < teamScore.size(); ++i)
		{
			m_winningTeams.push_back(teamScore[i].second);
			m_finalScores.push_back(teamScore[i].first);
		}

		sendMessageToPlayer(SpadesGameMessage(GAME_OVER_S,over,0),true);
		sendActivePlayer(-1);

		if(m_rawListener) {
			std::vector<Team> t;
			std::vector<Player> p;
			for(size_t i = 0; i < getNumberOfTeams(); i++)
				t.push_back(m_team[i]);
			for(size_t i = 0; i < m_gameInfo.getNumPlayers(); i++)
				p.push_back(m_player[i]);
			m_rawListener->gameIsOver(t,p,
				m_winningTeams,m_finalScores);
		}
	}

	const std::vector<int>& SpadesGame::getWinningTeams() const
	{
		return m_winningTeams;
	}

	const std::vector<int>& SpadesGame::getWinningScores() const
	{
		return m_finalScores;
	}

	void SpadesGame::setLastPointReceiver( int receiver )
	{
		m_lastPointReceiver = receiver;
	}

	int SpadesGame::getLastPointReceiver() const
	{
		return m_lastPointReceiver;
	}

	void SpadesGame::setLastPointType( SpadesPointEnum point )
	{
		m_lastPointType = point;
	}

	SpadesPointEnum SpadesGame::getLastPointType() const
	{
		return m_lastPointType;
	}

	void SpadesGame::increaseTime( float amount )
	{
		m_time += amount;
	}

	void SpadesGame::sendActivePlayer( int active )
	{
		std::vector<int> activePlayer(1,0);
		for(int i = 0; i < m_gameInfo.getNumPlayers(); ++i)
		{
			activePlayer[0] = -1;
			sendMessageToPlayer(SpadesGameMessage(
				SEND_ACTIVE_PLAYER_S,activePlayer,i));
		}

		if(active != -1)
		{
			for(int i = 0; i < m_gameInfo.getNumPlayers(); ++i)
			{
				activePlayer[0] = mapServerToClientPos(i,active);
				sendMessageToPlayer(SpadesGameMessage(
					SEND_ACTIVE_PLAYER_S,activePlayer,i));
			}
		}

		for(int i = 0; i < 4; i++)
		{
			if(i == active)
			{
				m_pActive[i] = true;
			}
			else
			{
				m_pActive[i] = false;
			}
		}
	}

	void SpadesGame::sendActivePlayers( bool p0, bool p1, bool p2, bool p3 )
	{
		bool a[4];
		a[0] = p0;
		a[1] = p1;
		a[2] = p2;
		a[3] = p3;

		for(int i = 0; i < 4; i++)
		{
			m_pActive[i] = a[i];
		}

		std::vector<int> activePlayer(1,0);
		for(int i = 0; i < m_gameInfo.getNumPlayers(); ++i)
		{
			activePlayer[0] = -1;
			sendMessageToPlayer(SpadesGameMessage(
				SEND_ACTIVE_PLAYER_S,activePlayer,i));
		}
		for(int j = 0; j < 4; j++)
		{
			std::vector<int> activePlayer(1,0);
			for(int i = 0; i < m_gameInfo.getNumPlayers(); ++i)
			{
				if(a[j])
				{
					activePlayer[0] = mapServerToClientPos(i,j);
					sendMessageToPlayer(SpadesGameMessage(
						SEND_ACTIVE_PLAYER_S,activePlayer,i));
				}
			}
		}
	}

	bool SpadesGame::canKickPlayer( int playerNo ) const
	{
		if(playerNo < 0)
			return true;
		if(getNumberOfTeams() == 2)
			return true;
		if(m_handCount == 0)
			return true;
		if(playerNo >= getNumberOfTeams())
			return true;

		int highestScore = -1;
		for(int i = 0; i < getNumberOfTeams(); i++)
		{
			if(m_team->getScore() > highestScore)
				highestScore = m_team->getScore();
		}

		return m_team[playerNo].getScore() < highestScore;
	}

	void SpadesGame::sendActivePlayersNotBid()
	{
		bool needsToBid[4];
		for(int i = 0; i < 4; i++)
			needsToBid[i] = false;

		for(int i = 0; i < m_gameInfo.getNumPlayers(); i++)
		{
			//if(m_player[i].getRoundBid() == -1)
			  if(i == m_requestedBidFromPlayer)
				needsToBid[i] = true;
		}

		sendActivePlayers(needsToBid[0],needsToBid[1],needsToBid[2],needsToBid[3]);
	}

	float SpadesGame::getTimeElapsed() const
	{
		return m_time;
	}

	int SpadesGame::getLegalCardIndexForPlayer( int player, int givenIndex, int dontPick /*= -1*/ )
	{
		std::vector<int> invalidIndexes = doCardFiltering(player);
		std::vector<int> validIndexes = getOppositeIndexes(player,invalidIndexes);

		for(size_t i = 0; i < validIndexes.size(); i++)
		{
			if(validIndexes[i] == givenIndex)
				return givenIndex;
		}

		if(validIndexes.size() == 0)
		{
			validIndexes = getOppositeIndexes(player,invalidIndexes);
			return -1;
		}

		if(validIndexes[0] == dontPick)
		return validIndexes[1];
		else
			return validIndexes[0];
	}
	std::vector<int> SpadesGame::getOppositeIndexes(int player, const std::vector<int>& cur )
	{
		std::vector<int> theCards;

		for(size_t i = 0; i < m_player[player].getCardCount(); i++)
		{
			bool has = false;

			for(size_t j = 0; j < cur.size(); j++)
			{
				if(cur[j] == i)
				{
					has = true;
					break;
				}
			}

			if(!has)
			{
				theCards.push_back(i);
			}
		}

		return theCards;
	}

	void SpadesGame::setRawListener( RawGameListener* listener )
	{
		m_rawListener = listener;
	}

	void SpadesGame::Player::setCards( const std::vector<Card>& cards )
	{
		m_cards = cards;
		sortCards();
	}

	void SpadesGame::Player::sortCards()
	{
		std::sort(m_cards.begin(),m_cards.end());
	}

	int SpadesGame::Player::getCardNumberOf( int card ) const
	{
		if(card < (int)getCardCount() && card >= 0)
		{
			return m_cards[card].getCardNumber();
		}
		else
		{
			return -1;
		}
	}

	size_t SpadesGame::Player::getCardCount() const
	{
		return m_cards.size();
	}

	SpadesGame::Player::Player()
		: m_alive(false),m_roundBid(-1)
	{
	}

	void SpadesGame::Player::setRoundBid( int bid )
	{
		m_roundBid = bid;
	}

	int SpadesGame::Player::getRoundBid() const
	{
		return m_roundBid;
	}

	int SpadesGame::Player::getRoundTricks() const
	{
		return m_tricks.size();
	}

	const std::vector<SpadesPointEnum>& SpadesGame::Player::getRoundTricksVec() const
	{
		return m_tricks;
	}

	void SpadesGame::Player::addRoundTrick( SpadesPointEnum point )
	{
		m_tricks.push_back(point);
	}

	void SpadesGame::Player::setAlive( bool active )
	{
		m_alive = active;
	}

	bool SpadesGame::Player::isAlive() const
	{
		return m_alive;
	}


	void SpadesGame::Player::setCard( int index,const Card& card )
	{
		if(index < (int)m_cards.size())
		{
			m_cards[index] = card;
			sortCards();
		}
	}

	int SpadesGame::Player::getQuantityOfSuit( Card::SuitEnum suit ) const
	{
		int count = 0;
		for(size_t i = 0; i < getCardCount(); ++i)
		{
			if(m_cards[i].getSuit() == suit)
			{
				count++;
			}
		}

		return count;
	}

	void SpadesGame::Player::setIncomingSwapCardNumbers( const std::vector<int>& cardNumbers )
	{
		m_incomingSwapCardNumbers = cardNumbers;
	}

	const std::vector<int>& SpadesGame::Player::getIncomingSwapCardNumbers() const
	{
		return m_incomingSwapCardNumbers;
	}

	void SpadesGame::Player::swapCards()
	{
		if(getIncomingSwapCardNumbers().size() != getOutgoingSwapCardNumbers().size())
		{
			return;
		}

		for(size_t i = 0; i < getOutgoingSwapCardNumbers().size(); ++i)
		{
			for(size_t j = 0; j < getCardCount(); ++j)
			{
				if(m_cards[j].getCardNumber() == getOutgoingSwapCardNumbers()[i])
				{
					setCard(j,Card::cardFromNumber(getIncomingSwapCardNumbers()[i]));
				}
			}
		}
	}

	void SpadesGame::Player::setOutgoingSwapCardNumbers( const std::vector<int>& cardNumbers )
	{
		m_outgoingSwapCardNumbers = cardNumbers;
	}

	const std::vector<int>& SpadesGame::Player::getOutgoingSwapCardNumbers() const
	{
		return m_outgoingSwapCardNumbers;
	}

	std::vector<int> SpadesGame::Player::getIndexesOfSuit(Card::SuitEnum suit) const
	{
		std::vector<int> indexes;
		for(size_t i = 0; i < getCardCount(); ++i)
		{
			if(m_cards[i].getSuit() == suit)
			{
				indexes.push_back(i);
			}
		}

		return indexes;
	}

	bool SpadesGame::Player::hasSuit( Card::SuitEnum suit ) const
	{
		for(size_t i = 0; i < getCardCount(); ++i)
		{
			if(m_cards[i].getSuit() == suit)
			{
				return true;
			}
		}

		return false;
	}

	std::vector<int> SpadesGame::Player::getIndexesOfNotSuit( Card::SuitEnum suit ) const
	{
		std::vector<int> indexes;
		for(size_t i = 0; i < getCardCount(); ++i)
		{
			if(m_cards[i].getSuit() != suit)
			{
				indexes.push_back(i);
			}
		}

		return indexes;
	}

	void SpadesGame::Player::removeCard( int index )
	{
		m_cards.erase(m_cards.begin() + index);
		sortCards();
	}

	void SpadesGame::Player::clearRoundTricks()
	{
		m_tricks.clear();
	}

	bool SpadesGame::Player::hasMadeBid() const
	{
		return getRoundTricks() >= getRoundBid();
	}

	std::vector<int> SpadesGame::Player::getCardNumbers() const
	{
		std::vector<int> cards;

		for(size_t i = 0; i < m_cards.size(); ++i)
		{
			cards.push_back(m_cards[i].getCardNumber());
		}

		return cards;
	}

	bool SpadesGame::Player::hasBid() const
	{
		return getRoundBid() != -1;
	}

	int SpadesGame::Player::getIndexFromCardNumber( int cardNumber ) const
	{
		for(size_t i = 0; i < m_cards.size(); i++)
		{
			int num = m_cards[i].getCardNumber();
			if(num == cardNumber)
				return i;
		}

		return -1;
	}

	bool SpadesGame::Player::onlyHasSpades() const
	{
		for(size_t i = 0; i < m_cards.size(); i++)
		{
			if(m_cards[i].getSuit() != Card::SPADES)
				return false;
		}

		return true;
	}

	SpadesGame::Team::Team()
		:m_score(0), m_bags(0)
	{

	}

	void SpadesGame::Team::clearScore()
	{
		m_score = 0;
	}

	void SpadesGame::Team::clearBags()
	{
		m_bags = 0;
	}

	void SpadesGame::Team::increaseBags( int amount )
	{
		m_bags += amount;
	}

	void SpadesGame::Team::increaseScore( int amount )
	{
		m_score += amount;
	}

	void SpadesGame::Team::setBags( int bags )
	{
		m_bags = bags;
	}

	void SpadesGame::Team::setScore( int score )
	{
		m_score = score;
	}

	int SpadesGame::Team::getScore() const
	{
		return m_score;
	}

	int SpadesGame::Team::getBags() const
	{
		return m_bags;
	}

}

