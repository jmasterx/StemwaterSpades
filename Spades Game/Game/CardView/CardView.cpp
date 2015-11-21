#include "Game/CardView/CardView.hpp"
#include <cmath>
#include <iostream>
namespace cge
{
	CardView::CardView(void)
		: 
		m_cardUnderMouse(-1),
		m_lastMousePos(-10000,-10000),
		m_maxSelectedCards(1),
		m_cardAnimations(NULL),
		m_animationManager(NULL),
		m_cardContainer(104),
		m_handGenerator(&m_cardContainer),
		m_cards(4),
		m_cardImageManager(NULL),
		m_isDeckCutEvt(false),
		m_singleSelectionBehaviour(true),
		m_mouseEnabled(true),
		m_timeUntilNextClick(0),
		m_mouseClickInterval(30),
		m_mouseDisabled(false),
		m_audioMan(NULL),
		m_waitSelectCard(false)
	{
		m_animationManager = CardAnimationManager(this);
		m_cardAnimations = CardAnimations(&m_animationManager);
	}

	CardView::~CardView(void)
	{
	}

	void CardView::render( GraphicsContext* context )
	{
		m_cardRenderer.renderCardHands(context,m_cards,m_tableCards);
	}

	bool CardView::pointOnCard( const ViewCard* card,int x, int y ) const
	{
		if(card == NULL)
			return false;

		Vec2 cardPosition = getProportions()->getCardPosition(card);
		Vec2 cardSize = Vec2(
			getProportions()->getInitialCardSize().getX() * 
			getProportions()->getCardScale(card) * 0.95f,
			getProportions()->getInitialCardSize().getY() * 
			getProportions()->getCardScale(card));
		Vec2 point = Vec2(x,y);
		point = Math::rotateAroundPoint(point,cardPosition,(CGE_PI * 2.0f) - card->m_angle);

		return pointInRect(
			cardPosition.getX() - (cardSize.getX() / 2.0f),
			cardPosition.getY() - (cardSize.getY() / 2.0f),
			cardSize.getX(),
			cardSize.getY(),
			point);
	}

	bool CardView::pointInRect( float x, float y, float w, float h, Vec2 p ) const
	{
		if(p.getX() < x) return false;
		if(p.getY() < y) return false;
		if(p.getX() >= x + w) return false;
		if(p.getY() >= y + h) return false;
		return true;
	}

	void CardView::mouseMove( int x, int y )
	{
		m_lastMousePos = Vec2(x,y);

		if(!isMouseEnabled() || m_mouseDisabled)
		{
			return;
		}

		int cardUnder = m_cardUnderMouse;
		setCardUnderMouse(x,y);	
		/*
		//deck cut sound (annulled)
		if(m_cardUnderMouse != -1 && m_cardUnderMouse != cardUnder && m_isDeckCutEvt)
		{
			float pitch = (float)(90 + (rand() % 20)) / 100.0f;
			//m_audioMan->stop("flick");
			if(rand() % 2 == 0)
			m_audioMan->play("play.card",pitch);
			else
			m_audioMan->play("play.card2",pitch);
		}

		*/
	}

	void CardView::logic()
	{
		if(isMouseEnabled())
		{
			setCardUnderMouse(m_lastMousePos.getX(),m_lastMousePos.getY());
		}
		
		m_cardAnimations.handleCardScaleLogic(m_cards[PLAYER],m_cardUnderMouse);
		m_animationManager.stepAnimations();
		verifyDisabledCards();
		processMessages();

		//used to limit clicks for a smoother experience
		m_timeUntilNextClick = m_timeUntilNextClick > 0 ? m_timeUntilNextClick - 1 : 0;
	}

	void CardView::processMessage( CardMessenger::CardMessage message,int val )
	{
		handleEnableMouse(message, val);

		switch(message)
		{
		case CardMessenger::END_ANIMATED_EVENT:
			m_animationManager.removeAnimationEvent(val);
			break;
		case CardMessenger::BEGIN_ANIMATED_EVENT:
			break;
		case CardMessenger::END_THROW_CARD_ON_TABLE:
			handleThrowCardEndLogic(val);
			break;
		case CardMessenger::END_TABLE_CARDS_TO_PLAYER:
			handleSendTableCardsLogic(val);
			break;
		case CardMessenger::END_SWAP_CARDS:
			handleSwapCardsLogic(val);
			break;
		default:
			break;
		}

		handleRedisableCards(message);
	}

	void CardView::mouseClick( int x, int y, bool lmb )
	{
		
		if(!isMouseEnabled() || m_mouseDisabled)
		{
			return;
		}

		setCardUnderMouse(x,y);

		if(m_timeUntilNextClick > 0)
		{
			//return;
		}
		else
		{
			m_timeUntilNextClick = m_mouseClickInterval;
		}

		if(m_isDeckCutEvt && lmb)
		{
			handleDeckCutLogic();
		}
		else if(m_cardUnderMouse != -1 && lmb)
		{
			handleClickLogic(x,y);
		}

	}

	void CardView::setCardUnderMouse( int x, int y )
	{
		if(m_mouseDisabled)
		{
			m_cardUnderMouse = -1;
		}

		int z = -1;
		for(int i = m_cards[PLAYER].size() - 1; i >=0; --i)
		{
			if(pointOnCard(m_cards[PLAYER][i],x,y))
			{
				z = i;
				break;
			}
		}

		m_cardUnderMouse = z;
	}


	void CardView::setCardImageManager( CardImageManager* cardImageManager )
	{
		m_cardImageManager = cardImageManager;
		m_cardRenderer.setCardImageManager(cardImageManager);
		m_cardAnimations.setCardImageManager(cardImageManager);
	}

	void CardView::evtDispatchCards( std::vector<Card> playerCards, 
		int leftCards, int rightCards, int topCards, CardHandEnum dealer )
	{
		if(playerCards.size() == 0 && leftCards == 0 && rightCards == 0 && topCards == 0)
		{
			return;
		}

		//clean up any cards
		reset();

		m_playerCards = playerCards;

		int cardCount[4] = {static_cast<int>(playerCards.size()),leftCards,rightCards,topCards};

		//set the pretty pictures
		for(int i = 0; i < 4; ++i)
		{
			if(cardCount[i] > 0)
			{
				m_handGenerator.generateHand(i,cardCount[i],m_cards[i]);
				assignCardImages((CardHandEnum)i);
			}
			
		}
        
		m_cardAnimations.dispatchCards(
			m_cards[PLAYER],m_cards[LEFT],m_cards[TOP],m_cards[RIGHT],0);
		
	}


	void CardView::handleEnableMouse( CardMessenger::CardMessage msg, int val )
	{
		switch(msg)
		{
		case CardMessenger::BEGIN_DISPATCH_CARDS:
		case CardMessenger::BEGIN_SWAP_CARDS:
			disableMouseInput();
			break;
		//case CardMessenger::END_DISPATCH_CARDS:
		case CardMessenger::END_THROW_CARD_ON_TABLE:
		case CardMessenger::END_SWAP_CARDS:
			enableMouseInput();
			break;

		}
	}

	void CardView::disableMouseInput()
	{
		m_cardUnderMouse = -1;
		m_mouseEnabled = false;
	}

	void CardView::enableMouseInput()
	{
		m_mouseEnabled = true;
	}

	bool CardView::isMouseEnabled() const
	{
		return m_mouseEnabled;
	}

	bool CardView::evtThrowCardOnTable( const Card& card,CardHandEnum player, int cardNum )
	{
		if(cardNum == -1)
		{
			cardNum = m_cards[player].size() / 2;
		}
		if((int)m_cards[player].size() <= cardNum)
		{
			return false;
		}

		//set the face card image
		m_cards[player][cardNum]->m_card = m_cardImageManager->getCardImage(card);
		
		//null pointer on image
		if(!m_cards[player][cardNum]->m_card)
		{
			return false;
		}

		m_cardAnimations.throwCardOnTable(m_cards[player][cardNum],(int)player);
		if(player == PLAYER)
		{
			disableMouseInput();
		}
		m_cards[player][cardNum]->m_thrown = true;
		return true;
	}

	void CardView::handleThrowCardEndLogic( int id )
	{

		CardAnimationEvent* animation = m_animationManager.getAnimationById(id);
		ViewCard* card = animation->getCard();

		if(!card)
		{
			//something went very wrong
			return;
		}

		//end logic

		//find card owner
		int pos = -1;
		CardHandEnum player = PLAYER;
		for(size_t i = 0; i < 4; ++i )
		{
			for(size_t j = 0; j < m_cards[i].size(); ++j)
			{
				if(m_cards[i][j] == card)
				{
					pos = j;
					player = (CardHandEnum)i;
				}
			}
		}

		//found card owner
		if(pos != -1)
		{
			//move the card to the table array
			m_tableCards.push_back(m_cards[player][pos]);
			m_cards[player].erase(m_cards[player].begin() + pos);

			//player keeps track of Cards
			if(player == PLAYER)
			{
				m_playerCards.erase(m_playerCards.begin() + pos);
			}

			//animate the hand shrinking and regenerate the hand
			std::vector<ViewCard> startCards;
			for(size_t i = 0; i < m_cards[player].size();++i)
			{
				startCards.push_back(*m_cards[player][i]);
			}
				
			m_handGenerator.generateHand(
				(int)player,m_cards[player].size(),m_cards[player]);

			m_cardAnimations.shrinkCards(m_cards[player],startCards);

			//cards regenerated so images need to be reassigned
			assignCardImages(player);
		}
	}

	void CardView::assignCardImages(CardHandEnum player)
	{
		if(player == PLAYER && !m_isDeckCutEvt)
		{
			for(size_t i = 0; i < m_playerCards.size(); ++i)
			{
				m_cards[PLAYER][i]->m_card = 
					m_cardImageManager->getCardImage(m_playerCards[i]);
			}
		}
		else
		{
			for(size_t i = 0; i < m_cards[player].size(); ++i)
			{
				m_cards[player][i]->m_card = 
					m_cardImageManager->getBackCardImage();
			}
		}
	}

	void CardView::handleSendTableCardsLogic( int id )
	{
		CardAnimationEvent* anim = m_animationManager.getAnimationById(id);
		std::vector<ViewCard*> cards = anim->getCards();

		for(size_t j = 0; j < cards.size(); ++j)
		{
			for(size_t i = 0; i < m_tableCards.size(); ++i)
			{
				if(m_tableCards[i] == cards[j])
				{
					m_cardContainer.release(cards[j]);
					m_tableCards.erase(m_tableCards.begin() + i);
					break;
				}
			}
		}
	}

	void CardView::evtSendTableCardsToPlayer( CardHandEnum player )
	{
		m_cardAnimations.sendTableCardsToPlayer(m_tableCards,(int)player);
	}

	std::vector<Card> CardView::getSelectedCards() const
	{
		std::vector<int> indexes = getSelectedCardIndexes();
		std::vector<Card> cards;

		for(size_t i = 0; i < indexes.size(); ++i)
		{
			cards.push_back(m_playerCards[indexes[i]]);
		}

		return cards;
	}

	void CardView::evtSwapCards( const std::vector<Card>& aCards, 
		const std::vector<Card>& bCards, CardHandEnum handA, CardHandEnum handB )
	{
		//put everything in an array to use a for loop
		std::vector<ViewCard*> swap[2];
		CardHandEnum hands[2] = {handA,handB};
		const std::vector<Card> zCards[2] = {aCards,bCards};

		std::vector<int> indexes[2];

		//for the 2 players
		for(int h = 0; h < 2; h++)
		{
			if(hands[h] == PLAYER && zCards[h].size() > 0 && zCards[h][0].getCardNumber() != -1)
			{
				//swap the player Cards
				for(size_t i = 0; i < m_playerCards.size(); ++i)
				{
					for(size_t j = 0; j < zCards[h].size(); ++j)
					{
						if(m_playerCards[i] == zCards[h][j])
						{
							//assign card
							m_playerCards[i] = zCards[(h + 1) % 2][j];
							swap[h].push_back(m_cards[hands[h]][i]);
							indexes[h].push_back(i);
						}
					}
				}
			}
			else //show from the middle of the hand
			{
				std::vector<int>pos;
				int middle = (m_cards[hands[h]].size() / 2) + 1;
				middle -= zCards[h].size() / 2;
				for(size_t i = 0; i < zCards[h].size();++i)
				{
					pos.push_back( middle + i );
				}


				//set the card images
				for(size_t i = 0; i < zCards[h].size(); ++i)
				{
					swap[h].push_back(m_cards[hands[h]][pos[i]]);
					/*
					swap[h].back()->m_card = 
						m_cardImageManager->getCardImage(zCards[h][i]);
					*/

					indexes[h].push_back(pos[i]);
				}	
			}
		}

		//literally swap the player and opponent cards
		for(size_t i = 0; i < indexes[1].size(); ++i)
		{
			std::swap(
				*m_cards[hands[0]][indexes[0][i]],
				*m_cards[hands[1]][indexes[1][i]]);
		}

		//if the player is not involved do simple swap
		if(handA != PLAYER)
		{
			m_cardAnimations.swapCards(swap[0],swap[1]);
		}
		else //otherwise put the cards directly in the player's hand at the right place
		{
			interpolatedSwap(swap[0],swap[1]);
		}
		
	}

	void CardView::handleSwapCardsLogic( int id )
	{
		//sends the message and performs event's end logic
		handleSwapCardsEndLogic();
	}

	void CardView::handleSwapCardsEndLogic()
	{
		//all 3 opponents
		for(int i = 1; i < 4; ++i)
		{
			m_handGenerator.generateHand(i,m_cards[i].size(),m_cards[i]);
			assignCardImages((CardHandEnum)i);
		}
	}

	void CardView::interpolatedSwap( const std::vector<ViewCard*> &cardsA ,
		const std::vector<ViewCard*> &cardsB )
	{
		m_animationManager.pushAnimationEvent(
			BEGIN_SWAP_CARDS,END_SWAP_CARDS);

		//initialize a temporary card
		ViewCard tempCard = *cardsA[0];

		//animation rate
		float rate = 0.0165f;

		//animate player's cards going to the opponent
		for(size_t i = 0; i < cardsB.size(); ++i)
		{
			tempCard = *cardsA[i];
			cardsB[i]->m_selected = false;
			tempCard.m_position = tempCard.m_homePos;
			tempCard.m_angle =    tempCard.m_homeAngle;
			tempCard.m_scale =    tempCard.m_homeScale;
			m_animationManager.addAnimation(cardsB[i],tempCard,rate,0);
		}
	
		std::vector<ViewCard> curPos;
		std::vector<ViewCard> newPos;

		//used to find the indexes of the new card order
		std::vector<std::pair<Card, int> > posChanges;
		for(size_t i = 0; i < m_playerCards.size(); ++i)
		{
			posChanges.push_back(std::make_pair(m_playerCards[i],i));
		}

		for(size_t i = 0; i < m_cards[PLAYER].size(); ++i)
		{
			curPos.push_back(*m_cards[PLAYER][i]);
		}

		//sort the cards and keep track of index changes
		std::sort(m_playerCards.begin(),m_playerCards.end());
		std::sort(posChanges.begin(),posChanges.end());

		//generate the new card positions
		m_handGenerator.generateHand(0,m_playerCards.size(),m_cards[PLAYER]);

		assignCardImages(PLAYER);

		//store the new positions
		for(size_t i = 0; i < m_cards[PLAYER].size(); ++i)
		{
			newPos.push_back(*m_cards[PLAYER][i]);
		}

		//animate opponent's cards going directly into the player's hand at the correct spot
		for(size_t i = 0; i < m_cards[PLAYER].size(); ++i)
		{
			//start the cards at their old positions
			int pos = posChanges[i].second;
			m_cards[PLAYER][i]->m_position = curPos[pos].m_position;
			m_cards[PLAYER][i]->m_angle = curPos[pos].m_angle;
			m_cards[PLAYER][i]->m_scale = curPos[pos].m_scale;

			//animate them to their new positions
			m_animationManager.addAnimation(
				m_cards[PLAYER][i],
				newPos[i],
				rate,
				0);
		}
	}

	void CardView::evtSelectDeckCut()
	{
		//clean up
		reset();

		m_isDeckCutEvt = true;
		m_handGenerator.generateDeck(m_cards[PLAYER]);
		for (size_t i = 0; i < m_cards.size(); ++i)
		{
			//back of the card
			assignCardImages(PLAYER);
		}

	}

	void CardView::handleDeckCutLogic()
	{
		if(m_cardUnderMouse != -1)
		{
			sendMessage(DECK_CUT_SELECTED,m_cardUnderMouse);
			m_isDeckCutEvt = false;
			m_cardAnimations.animateDeckCut(m_cards[PLAYER],m_cardUnderMouse);
			disableMouseInput();
		}
	}

	void CardView::handleClickLogic( int x, int y )
	{
		//the card is disabled, do nothing
		if(!m_cards[PLAYER][m_cardUnderMouse]->m_enabled)
		{
			return;
		}
		
		if(m_cards[PLAYER][m_cardUnderMouse]->m_selected)
		{
			if(!m_cards[PLAYER][m_cardUnderMouse]->m_thrown)
			{
				if(!m_waitSelectCard)
				{
					m_cardAnimations.toggleCard(m_cards[PLAYER][m_cardUnderMouse]);
				}
				else
				{
					sendMessage(CardMessenger::CARD_SELECTED,m_cardUnderMouse);
				}
				disableDisabledCards();
			}

		}
		else //card is being selected
		{
			if(m_maxSelectedCards == 1)
			{
				for(size_t i = 0; i < m_cards[PLAYER].size(); ++i)
				{
					if(m_cards[PLAYER][i]->m_selected || i == m_cardUnderMouse)
					{
						if(!m_cards[PLAYER][i]->m_thrown)
						{
							m_cardAnimations.toggleCard(m_cards[PLAYER][i]);	
						}
					}
				}
			}
			else
			{
				m_cardAnimations.toggleCard(m_cards[PLAYER][m_cardUnderMouse]);

				if(getNumSelectedCards() >= m_maxSelectedCards)
				{
					disableUnselectedCards();
				}
			}
		}
	}

	void CardView::setDisabledCards( const std::vector<int> cards )
	{
		m_disabledCards = cards;
		disableDisabledCards();
		enableMouseInput();
	}

	void CardView::disableDisabledCards()
	{
		for(size_t i = 0; i < m_cards[PLAYER].size(); ++i)
		{
			m_cards[PLAYER][i]->m_enabled = true;
		}


		for(size_t i = 0; i < m_disabledCards.size(); ++i)
		{
			int pos = m_disabledCards[i];

			if(pos < (int)m_cards[PLAYER].size())
			{
				m_cards[PLAYER][pos]->m_enabled = false;

				if(m_cards[PLAYER][pos]->m_selected)
				{
					m_cardAnimations.toggleCard(m_cards[PLAYER][pos]);
				}
			}
		}
	}

	void CardView::setMaxSelectedCards(int max)
	{
		if(max < 1)
		{
			max = 1;
		}

		m_maxSelectedCards = max;
	}

	int CardView::getNumSelectedCards() const
	{
		int count = 0;
		for(size_t i = 0; i < m_cards[PLAYER].size(); ++i)
		{
			if(m_cards[PLAYER][i]->m_selected)
			{
				count++;
			}
		}

		return count;
	}

	void CardView::disableUnselectedCards()
	{
		for(size_t i = 0; i < m_cards[PLAYER].size(); ++i)
		{
			if(!m_cards[PLAYER][i]->m_selected)
			{
				m_cards[PLAYER][i]->m_enabled = false;
			}
			
		}
	}

	int CardView::getSelectedCardIndex() const
	{
		for(size_t i = 0; i < m_cards[PLAYER].size(); ++i)
		{
			if(m_cards[PLAYER][i]->m_selected)
			{
				return i;
			}
		}

		return -1;
	}

	std::vector<int> CardView::getSelectedCardIndexes() const
	{
		std::vector<int> indexes;

		for(size_t i = 0; i < m_cards[PLAYER].size(); ++i)
		{
			if(m_cards[PLAYER][i]->m_selected)
			{
				indexes.push_back(i);
			}
		}

		return indexes;
	}

	const Card CardView::getSelectedCard() const
	{
		int index = getSelectedCardIndex();
		if(index > -1)
		{
			return m_playerCards[index];
		}

		return Card(Card::DIAMONDS,Card::TWO);
	}

	void CardView::handleRedisableCards( CardMessenger::CardMessage msg )
	{
		switch(msg)
		{
		case END_DISPATCH_CARDS:
		case END_THROW_CARD_ON_TABLE:
		case END_SHRINK_HAND:
		case END_SWAP_CARDS:
		case END_TABLE_CARDS_TO_PLAYER:
			disableDisabledCards();
		default:
			break;
		}
	}

	void CardView::setSingleSelectionBehaviour( bool singleSelection )
	{
		m_singleSelectionBehaviour = singleSelection;
	}

	bool CardView::isSingleSelectionBehaviour() const
	{
		return m_singleSelectionBehaviour;
	}

	void CardView::reset()
	{
		m_tableCards.clear();
		for(size_t i = 0; i < m_cards.size(); ++i)
		{
			m_cards[i].clear();
		}
		m_cardContainer.freeAll();
		m_playerCards.clear();

	}

	void CardView::verifyDisabledCards()
	{
		for (size_t i = 0; i < m_cards[PLAYER].size(); ++i)
		{
			if(!m_cards[PLAYER][i]->m_enabled && m_cards[PLAYER][i]->m_selected)
			{
				m_cardAnimations.toggleCard(m_cards[PLAYER][i]);
			}
		}
	}

	void CardView::setProportions( ProportionsManager* proportions )
	{
		DynamicElement::setProportions(proportions);
		m_cardRenderer.setProportionsManager(proportions);
	}

	void CardView::disableAllCards()
	{
		std::vector<int> cards;
		for(size_t i = 0; i < m_playerCards.size(); ++i)
		{
			cards.push_back(i);
		}

		setDisabledCards(cards);
	}

	int CardView::getIndexOfCard( const Card& c )
	{
		for(size_t i = 0; i < m_playerCards.size(); ++i)
		{
			if(m_playerCards[i].getCardNumber() == c.getCardNumber())
			{
				return i;
			}
		}

		return -1;
	}

	void CardView::selectCard( int index )
	{
		if(index != -1 && index < m_cards[PLAYER].size())
		m_cardAnimations.toggleCard(m_cards[PLAYER][index]);
	}

	void CardView::setMouseDisabled( bool disabled )
	{
		m_mouseDisabled = true;
		m_cardUnderMouse = -1;
	}

	bool CardView::isAnimationQueueEmpty() const
	{
		return m_animationManager.getCount() == 0;
	}

	bool CardView::isOnlyAnimationShrink() const
	{
		return m_animationManager.hasAnimationType(END_SHRINK_HAND) 
			&& m_animationManager.getCount() == 1;
	}

	int CardView::selectRandomValidCard()
	{
		if(m_playerCards.size() == m_disabledCards.size())
			return -1;

		int card = -1;
		
		do 
		{
			int tmp = rand() % m_playerCards.size();
			bool found = true;

			for(int i = 0; i < m_disabledCards.size(); ++i)
			{
				if(m_disabledCards[i] == tmp)
				{
					found = false;
					break;
				}
			}
			if(found)
			{
				card = tmp;
			}

		} while (card == -1);

		return card;
	}

	void CardView::playRandomCard()
	{
		int card = selectRandomValidCard();
		m_cardUnderMouse = card;
		if(m_cardUnderMouse != -1)
		{
			handleClickLogic(-1,-1);
		}
	}

	void CardView::setAudioManager( AudioManager* audio )
	{
		m_audioMan = audio;
	}

	void CardView::setWaitForSelectCard( bool waiting )
	{
		m_waitSelectCard = waiting;
	}
}
