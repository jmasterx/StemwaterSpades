#include "Game/Net/NetAIPartnershipSpadesBot.hpp"
#include "Game/Net/NetSpadesGame.hpp"
#include "Game/Utility/console.hpp"
static console botcons(false);
namespace cge
{
	NetAIPartnershipSpadesBot::NetAIPartnershipSpadesBot(void)
		: m_game(NULL)
	{
	}

	NetAIPartnershipSpadesBot::~NetAIPartnershipSpadesBot(void)
	{
	}

	void NetAIPartnershipSpadesBot::receive( const SpadesGameMessage& msg )
	{
		switch(msg.getMessage())
		{
		case SEND_GAME_INFO_D_S:
			setGameInfo(msg.getArgs());
			break;
		case REQUEST_GAME_INIT_S:
			//eventually this will cause a button to appear
			//but for now we just reply right away
			sendMessageToGame(REPLY_INIT_GAME_C,std::vector<int>());
			break;
		case REQUEST_DECK_CUT_POS_S:
			//eventually we let the player pick a card
			{
				std::vector<int> deckCutPos;
				deckCutPos.push_back(rand() % 52);
				sendMessageToGame(SEND_DECK_CUT_POS_C,deckCutPos);
			}
			break;
		case DISPATCH_CARDS_A_D_S:
			{
				//give bot his cards
				std::vector<Card> cards;
				for(size_t i = 4; i < msg.getArgs().size(); i++)
				{
					cards.push_back(Card::cardFromNumber(msg.getArgs()[i]));
				}
				m_cards = cards;
				sendMessageToGame(FINISH_DISPATCH_CARDS_C,std::vector<int>());
			}
			break;
		case REQUEST_PLAYER_TRICK_COUNT_S:
			{
				std::vector<int> cardCount;
				if(m_info.isPassTwo())
				cardCount.push_back(pass2Bid());
				else if(m_info.isPartnership())
					cardCount.push_back(regularBid());
				else
					cardCount.push_back(1);
				sendMessageToGame(SEND_PLAYER_TRICK_COUNT_C,cardCount);
			}
			break;
		case SEND_PLAYER_TRICK_COUNT_D_S:
			{
				int player = serverToClientPos(msg.getArgs()[0]);
				int bid = msg.getArgs()[1];
				m_playerBid[player] = bid;
			}
			break;
		case REQUEST_SELECTED_CARD_S:
			{
				std::vector<int> cardCount;
				int r = -1;
				if(m_info.isPartnership() || m_info.isPassTwo())
				r = playRegularCard();

				bool found = false;
				for(int i = 0; i < m_enabledCardIndexes.size(); i++)
				{
					if(m_enabledCardIndexes[i] == r)
						found = true;
				}

				if(!found)
				{
					botcons << "ILLEGAL INDEX" << std::endl;
					for(int i = 0; i < m_enabledCardIndexes.size(); i++)
					{
						r = m_enabledCardIndexes[i];
					}
				}

				cardCount.push_back(r);
				m_cards.erase(m_cards.begin() + r);
				std::sort(m_cards.begin(),m_cards.end());
				sendMessageToGame(SEND_SELECTED_CARD_C,cardCount);
				std::vector<int> cc;
				sendMessageToGame(FINISH_PERFORM_CARD_ON_TABLE_C,cc);
			}
			break;
		case FILTER_CARDS_S:
			{
				m_enabledCardIndexes.clear();
				for(int i = 0; i < m_cards.size(); ++i)
				{
					m_enabledCardIndexes.push_back(i);
				}

				for(int i = 0; i < msg.getArgs().size(); i++)
				{
					for(int j = 0; j < m_enabledCardIndexes.size(); ++j)
					{
						if(m_enabledCardIndexes[j] == msg.getArgs()[i])
						{
							m_enabledCardIndexes.erase(m_enabledCardIndexes.begin() + j);
							break;
						}
					}
				}

				m_enabledCards.clear();
				for(int i = 0; i < m_enabledCardIndexes.size(); i++)
				{
					m_enabledCards.push_back(m_cards[m_enabledCardIndexes[i]]);
				}
			}
			break;
		case PERFORM_CARD_ON_TABLE_A_D_S:
			{
				std::vector<int> cardCount;
				m_tableIndexes[serverToClientPos(msg.getArgs()[1])] = msg.getArgs()[0];
				m_tableCards[serverToClientPos(msg.getArgs()[1])] = Card::cardFromNumber(msg.getArgs()[0]);
				m_playedCards.push_back(Card::cardFromNumber(msg.getArgs()[0]));
				sendMessageToGame(FINISH_PERFORM_CARD_ON_TABLE_C,cardCount);
			}
			break;
		case REQUEST_CARD_SWAP_S:
			{
				std::vector<int> cardCount;
				int r = rand() % m_cards.size();
				int  p = rand() % m_cards.size();

				NetAIPoint point = pass2SwapCards();
				if(point.x != -1 && point.y != -1)
				{
					r = point.x;
					p = point.y;

					if(p > r)
						p--;
				}

				if(r < 0 || r >= m_cards.size())
				{
					botcons << "Swap problemA" << std::endl;
					r = 0;
				}
				cardCount.push_back(m_cards[r].getCardNumber());
				m_cards.erase(m_cards.begin() + r);


				if(p < 0 || p >= m_cards.size())
				{
					botcons << "Swap problemB" << std::endl;
					p = 0;
				}

				cardCount.push_back(m_cards[p].getCardNumber());
				m_cards.erase(m_cards.begin() + p);

				sendMessageToGame(SEND_SWAPPED_CARDS_C,cardCount);
			}
			break;
		case PERFORM_SWAP_S:
			{
				if(msg.getArgs()[0] == 0 || msg.getArgs()[0] == 1) //does it need to swap?
				{
					std::vector<Card> incoming;

					incoming.push_back(Card::cardFromNumber(msg.getArgs()[1]));
					incoming.push_back(Card::cardFromNumber(msg.getArgs()[2]));

					m_cards.push_back(incoming[0]);
					m_cards.push_back(incoming[1]);
					std::sort(m_cards.begin(),m_cards.end());
				}

				std::vector<int> cardCount;

				sendMessageToGame(FINISHED_CARD_SWAP_C,cardCount);

			}
			break;
		case PLAYER_MADE_TRICK_A_D_S:
			{
				m_playerTrickCount[serverToClientPos(msg.getArgs()[1])]++;
				std::vector<int> cardCount;
				sendMessageToGame(FINISHED_PLAYER_MADE_TRICK_C,cardCount);
				m_tableIndexes.clear();
				m_tableCards.clear();
				for(int i = 0; i < m_info.getNumPlayers(); i++)
				{
					m_tableIndexes.push_back(-1);
					m_tableCards.push_back(Card::cardFromNumber(1));
				}
			}
			break;
		case ROUND_BEGIN_S:
			{
				m_playerBid.clear();
				m_playerTrickCount.clear();
				m_tableIndexes.clear();
				m_playedCards.clear();
				m_tableCards.clear();
				for(int i = 0; i < m_info.getNumPlayers(); i++)
				{
					m_playerBid.push_back(-1);
					m_playerTrickCount.push_back(0);
					m_tableIndexes.push_back(-1);
					m_tableCards.push_back(Card::cardFromNumber(1));
				}
			}
			break;
		}
	}

	void NetAIPartnershipSpadesBot::setGame( NetSpadesGame* game )
	{
		m_game = game;
	}

	void NetAIPartnershipSpadesBot::sendMessageToGame( const SpadesCSMessageEnum& msg, const std::vector<int>& args )
	{
		if(m_game)
		{
			m_game->botSendMessageToGame(msg,args,this);
		}
	}

	int NetAIPartnershipSpadesBot::getNumCardsInRank( Card::RankEnum rank ) const
	{
		int count = 0;
		for(int i = 0; i < m_cards.size(); i++)
		{
			if(m_cards[i].getRank() == rank)
				count++;
		}

		return count;
	}

	int NetAIPartnershipSpadesBot::getNumCardsInSuit( Card::SuitEnum suit ) const
	{
		int count = 0;
		for(int i = 0; i < m_cards.size(); i++)
		{
			if(m_cards[i].getSuit() == suit)
				count++;
		}

		return count;
	}

	int NetAIPartnershipSpadesBot::getNumCardsInRankAndSuitGreaterOrEqualTo( Card::RankEnum rank, Card::SuitEnum suit ) const
	{
		int count = 0;
		for(int i = 0; i < m_cards.size(); i++)
		{
			if(m_cards[i].getSuit() == suit && m_cards[i].getRank() >= rank)
			{
				count++;
			}
		}

		return count;
	}

	bool NetAIPartnershipSpadesBot::nilRegular() const
	{
		if(hasPartnerBid() && getPartnerBid() < 2)
			return false;

		if(getNumCardsInSuit(Card::SPADES) > 2)
			return false;
		if(getNumCardsInRankAndSuitGreaterOrEqualTo(Card::JACK,Card::SPADES) > 0)
			return false;

		if(getNumCardsInSuitBetween(Card::SPADES,Card::EIGHT,Card::TEN) >= 2)
			return false;

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			if(getNumCardsInSuit((Card::SuitEnum)i) <= 3 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::ACE,(Card::SuitEnum)i) >= 1)
				return false;

			if(getNumCardsInSuit((Card::SuitEnum)i) <= 3 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::TEN,(Card::SuitEnum)i) >= 2)
				return false;

			if(getNumCardsInSuit((Card::SuitEnum)i) <= 2 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::JACK,(Card::SuitEnum)i) >= 1)
				return false;

			if(getNumCardsInSuit((Card::SuitEnum)i) <= 5 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::TEN,(Card::SuitEnum)i) >= 3)
				return false;
		}

		return true;
	}

	int NetAIPartnershipSpadesBot::getNumCardsInSuitBetween( Card::SuitEnum suit,Card::RankEnum minimum, Card::RankEnum maximum ) const
	{
		int count = 0;
		for(int i = 0; i < m_cards.size(); i++)
		{
			if(m_cards[i].getSuit() == suit && m_cards[i].getRank() >= minimum && m_cards[i].getRank() <= maximum)
			{
				count++;
			}
		}
		return count;
	}

	bool NetAIPartnershipSpadesBot::nilPass2() const
	{
		if(getPartnerBid() == 0)
			return false;

		if(getNumCardsInSuit(Card::SPADES) >= 5)
			return false;
		if(getNumCardsInRankAndSuitGreaterOrEqualTo(Card::TEN,Card::SPADES) >= 4)
			return false;


		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			if(getNumCardsInSuit((Card::SuitEnum)i) <= 3 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::TEN,(Card::SuitEnum)i) >= 2)
				return false;

			if(getNumCardsInSuit((Card::SuitEnum)i) <= 5 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::TEN,(Card::SuitEnum)i) >= 3)
				return false;
		}

		if(getNumCardsInRank(Card::ACE) >= 3)
			return false;
		if(getNumCardsInRank(Card::KING) >= 3)
			return false;

		return true;

	}

	int NetAIPartnershipSpadesBot::regularBid() const
	{
		if(nilRegular())
			return 0;
			return calcMathBid();
		int bid = calcBasicBid();

		/*
		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			Card::SuitEnum s = (Card::SuitEnum)i;
			if(getNumCardsInSuit(s) == 1 && !hasCard(Card::ACE,s))
				bid++;
		}
		*/

		if(isLastToBid())
		{
			int total = getBidSum();
			if(getBidSum() + bid > 12)
			{
				bid = 12 - getBidSum();
			}
		}
		if(bid <= 0)
			bid = 1;

		return bid;
	}

	int NetAIPartnershipSpadesBot::pass2Bid() const
	{
		if(nilPass2())
			return 0;

		return calcMathBid();

		int bb = calcBasicBid();
		
		//if(getPartnerBid() == 0)
		//	bb+=1;
	//	else
		{
			if(getPartnerBid() == -1)
			{
				bb+= 1;
			}
			else if(getPartnerBid() > 0 && bb > 2)
			{
				bb -= 1;
			}
		}

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			Card::SuitEnum s = (Card::SuitEnum)i;
			if(getNumCardsInSuit(s) == 1 && !hasCard(Card::ACE,s))
			{
				if(hasCard(Card::JACK,s) || hasCard(Card::QUEEN,s) || hasCard(Card::KING,s))
				{
					bb++;
				}
			}
		}

		//4 cases

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			Card::SuitEnum caseSuit = (Card::SuitEnum)i;
			if(caseSuit == Card::SPADES)
				continue;

			bool case1 = hasCard(Card::ACE,caseSuit) && 
				hasCard(Card::QUEEN,caseSuit) && 
				hasCard(Card::JACK,caseSuit) && 
				!hasCard(Card::KING,caseSuit) && 
				getNumCardsInSuitBetween(caseSuit,Card::TWO,Card::TEN) >= 1;

			bool case2 = hasCard(Card::ACE,caseSuit) && 
				hasCard(Card::QUEEN,caseSuit) && 
				hasCard(Card::TEN,caseSuit) && 
				!hasCard(Card::KING,caseSuit) && 
				getNumCardsInSuitBetween(caseSuit,Card::TWO,Card::NINE) >= 1;

			bool case3 = hasCard(Card::ACE,caseSuit) && 
				hasCard(Card::KING,caseSuit) && 
				hasCard(Card::JACK,caseSuit) && 
				!hasCard(Card::QUEEN,caseSuit) && 
				getNumCardsInSuitBetween(caseSuit,Card::TWO,Card::TEN) >= 1;

			bool case4 = hasCard(Card::ACE,caseSuit) && 
				hasCard(Card::KING,caseSuit) && 
				hasCard(Card::TEN,caseSuit) && 
				!hasCard(Card::QUEEN,caseSuit) && 
				getNumCardsInSuitBetween(caseSuit,Card::TWO,Card::NINE) >= 1;

			bool caseMatch = case1 || case2 || case3 || case4;
			if(caseMatch)
			{
				if(!hasPartnerBid())
				{
					bb++;
				}
				else
				{
					bb++;
				}
			}
		}

		if(isLastToBid())
		{
			if(getNumNils() == 2 && getBidSum() + bb < 11)
			{
				bb = 11 - getBidSum();
			}
			int total = getBidSum();
			if(getBidSum() + bb > 12)
			{
				bb = 12 - getBidSum();
			}
		}
		
		if(bb > 7)
			bb = 7;

		if(bb <= 0)
			bb = 1;

		return bb;
	}

	cge::NetAIPoint NetAIPartnershipSpadesBot::pass2SwapCards() const
	{
		bool youBidNil = getPartnerBid() > 0;
		if(youBidNil)
		{
			if(getNumCardsInSuit(Card::SPADES) >= 2 && !hasSuitWithAceAndLessThanNCards(3,true))
			{
				NetAIPoint p;
				p.x = getLastNthCardIndexInSuit(Card::SPADES,0);
				p.y = getLastNthCardIndexInSuit(Card::SPADES,1);

				return p;
			}
			else if(getNumCardsInSuit(Card::SPADES) == 1 || hasSuitWithAceAndLessThanNCards(3,true))
			{

				NetAIPoint p;
				p.x = getLastNthCardIndexInSuit(Card::SPADES,0);
				Card::SuitEnum s2 = getNthSuitWithLowestCountMin(0,true,Card::TEN);
				p.y = getLastNthCardIndexInSuit(s2,0);
				return p;
			}
			else
			{
				NetAIPoint p;
				Card::SuitEnum s1 = getNthSuitWithLowestCountMin(0,true,Card::TEN);
				Card::SuitEnum s2 = getNthSuitWithLowestCountMin(1,true,Card::TEN);

				if(s2 == s1 || s2 == Card::SUIT_ENUM_END)
				{
					p.x = getLastNthCardIndexInSuit(s1,0);
					p.y = getLastNthCardIndexInSuit(s1,1);
				}
				else
				{
					p.x = getLastNthCardIndexInSuit(s1,0);
					p.y = getLastNthCardIndexInSuit(s2,0);
				}

				return p;
			}
		}
		else
		{
			NetAIPoint p;
			Card::SuitEnum s1 = getNthSuitWithLowestCountMax(0,true,Card::NINE);
			Card::SuitEnum s2 = getNthSuitWithLowestCountMax(1,true,Card::NINE);

			if(s1 == Card::SUIT_ENUM_END) //only spades
			{
				p.x = getFirstNthCardIndexInSuit(Card::SPADES,0);
				p.y = getFirstNthCardIndexInSuit(Card::SPADES,1);
			}
			else if(s2 == Card::SUIT_ENUM_END || s2 == s1)
			{
				p.x = getFirstNthCardIndexInSuit(s1,0);
				p.y = getFirstNthCardIndexInSuit(s1,1);
			}
			else
			{
				p.x = getFirstNthCardIndexInSuit(s1,0);
				p.y = getFirstNthCardIndexInSuit(s2,0);
			}

			return p;
		}
	}

	int NetAIPartnershipSpadesBot::getLastNthCardIndexInSuit( Card::SuitEnum suit, int countFromLast ) const
	{
		Card::SuitEnum s = Card::SUIT_ENUM_END;
		int index = 0;

		for(int i = m_cards.size() - 1; i >= 0; i--)
		{
			index++;
			if(m_cards[i].getSuit() != s)
			{
				s = m_cards[i].getSuit();
				index = 0;
			}

			if(s == suit && index == countFromLast)
			{
				return i;
			}
		}
		return -1;
	}

	cge::Card::SuitEnum NetAIPartnershipSpadesBot::getNthSuitWithLowestCount( int countFromLast, bool skipSpades ) const
	{
		std::pair<int,Card::SuitEnum> suits[Card::SUIT_ENUM_END];
		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			suits[i] = std::make_pair(0,Card::SUIT_ENUM_END);
		}

		for(int i = 0; i < Card::SUIT_ENUM_END;i++)
		{
			suits[i] = std::make_pair(getNumCardsInSuit((Card::SuitEnum)i),(Card::SuitEnum)i);
		}

		std::sort(suits,suits + Card::SUIT_ENUM_END);

		int count = 0;

		for(int i = 0; i < Card::SUIT_ENUM_END;i++)
		{
			if(suits[i].first == 0 || (skipSpades && suits[i].second == Card::SPADES))
				continue;

			if(count == countFromLast)
				return suits[i].second;

			count++;
		}
		return Card::SUIT_ENUM_END;
	}

	bool NetAIPartnershipSpadesBot::onlyHasSuit( Card::SuitEnum suit ) const
	{
		if(getNumCardsInSuit(suit) == 0)
			return false;

		for(int i = 0; i < m_cards.size(); i++)
		{
			if(m_cards[i].getSuit() != suit)
			{
				return false;
			}
		}

		return true;
	}

	int NetAIPartnershipSpadesBot::getFirstNthCardIndexInSuit( Card::SuitEnum suit, int countFromFirst ) const
	{
		Card::SuitEnum s = Card::SUIT_ENUM_END;
		int index = 0;
		botcons << "Suit:" << suit << " count:" << countFromFirst << std::endl; 

		for(int i = 0; i < m_cards.size(); i++)
		{
			index++;
			if(m_cards[i].getSuit() != s)
			{
				s = m_cards[i].getSuit();
				index = 0;
			}

			if(s == suit && index == countFromFirst)
			{
				botcons << "i:" << i << std::endl; 
				return i;
			}
		}
		botcons << "bad branch" << std::endl; 
		return -1;
	}

	void NetAIPartnershipSpadesBot::setGameInfo( const std::vector<int>& args )
	{
		m_info = SpadesGameInfo(args);
		m_playerBid.clear();
		m_playerTrickCount.clear();
		for(int i = 0; i < m_info.getNumPlayers(); i++)
		{
			m_playerBid.push_back(-1);
			m_playerTrickCount.push_back(-1);
		}
	}

	bool NetAIPartnershipSpadesBot::hasPartnerBidNil() const
	{
		return m_playerBid[2] == 0;
	}

	bool NetAIPartnershipSpadesBot::hasPartnerBid() const
	{
		return m_playerBid[2] >= 0;
	}

	int NetAIPartnershipSpadesBot::getPartnerBid() const
	{
		return m_playerBid[2];
	}

	int NetAIPartnershipSpadesBot::calcBasicBid() const
	{
		int bid = 0;
		std::vector<Card> cardsAdded;
		if(getNumCardsInSuit(Card::SPADES) < 6)
		{
			for(int i = 0; i < m_cards.size(); i++)
			{
				if(m_cards[i].getSuit() == Card::SPADES)
					cardsAdded.push_back(m_cards[i]);
			}

			int stopPos = 0;
			for(int i = Card::ANY_RANK - 1; i >= 0; i--)
			{
				if(hasCard((Card::RankEnum)i,Card::SPADES))
				{
					bid++;
					stopPos++;
					for(int j = 0; j < cardsAdded.size(); j++)
					{
						if(cardsAdded[j].getRank() == (Card::RankEnum)i)
						{
							cardsAdded.erase(cardsAdded.begin() + j);
							break;
						}
					}
				}
				else
				{
					break;
				}
			}

			int num10ToA = getNumCardsInSuitBetween(Card::SPADES,Card::TEN,Card::ACE);
			int num2To9 = getNumCardsInSuitBetween(Card::SPADES,Card::TWO,Card::NINE);


			int cardCost = 0;
			int usedCount = 0;
			int remainingCount = num2To9;
			botcons << "num between = " << num2To9 << std::endl;
			for(int i = cardsAdded.size() - 1; i >= 0; i--)
			{
				Card c = cardsAdded[i];

				botcons << c.getRankString() << std::endl;
				if(c.getRank() >= Card::TEN)
				{
					int cost = getCardCost(c);
					cardCost += cost;
					botcons << "Card Cost" << cardCost << std::endl;
					if(cardCost <= num2To9 || 
						(c.getRank() == Card::JACK && hasCard(Card::TEN,Card::SPADES) && cardCost - 1 <= num2To9))
					{
						usedCount += cost;
						remainingCount = num2To9 - usedCount;
						bid++;
						botcons << "added from cost" << std::endl;
					}
				}
				else //2 - 9
				{
					if(i <= usedCount - 1)
						break;
				}
			}
		}
		else
		{
			bid += getNumCardsInSuit(Card::SPADES) / 2;
		}
	
		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			if((Card::SuitEnum)i == Card::SPADES)
			{
				continue;
			}

			if(hasCard(Card::ACE,(Card::SuitEnum)i))
			{
				bid++;

				if(hasCard(Card::KING,(Card::SuitEnum)i) && getNumCardsInSuit((Card::SuitEnum)i))
				{
					bid++;
				}

				if(hasCard(Card::QUEEN,(Card::SuitEnum)i) && getNumCardsInSuit((Card::SuitEnum)i) &&
					getNumCardsInSuitBetween((Card::SuitEnum)i,Card::TWO,Card::NINE) >= 1)
				{
					bid++;
				}
			}
			else if(hasCard(Card::KING,(Card::SuitEnum)i) && getNumCardsInSuit((Card::SuitEnum)i) >= 3)
			{
				bid++;
			}
		}

		if(bid == 0)
			bid = 1;

		return bid;
	}

	bool NetAIPartnershipSpadesBot::hasCard( Card::RankEnum rank, Card::SuitEnum suit ) const
	{
		for(int i = 0; i < m_cards.size(); i++)
		{
			if(m_cards[i].getRank() == rank && m_cards[i].getSuit() == suit)
				return true;
		}

		return false;
	}

	int NetAIPartnershipSpadesBot::getCardCost( const Card& c ) const
	{
		int cost = 0;
		for(int i = c.getRank() + 1; i < Card::ANY_RANK; i++)
		{
			if(hasCard((Card::RankEnum)i,c.getSuit()))
			{
				botcons << "cost" << cost << std::endl;
				return cost;
			}
			else
			{
				cost++;
			}
		}
		botcons << "cost" << cost << std::endl;
		return cost;
	}

	Card::SuitEnum NetAIPartnershipSpadesBot::getNthSuitWithLowestCountMin( int countFromLast, bool skipSpades, Card::RankEnum minimum ) const
	{
		std::pair<int,Card::SuitEnum> suits[Card::SUIT_ENUM_END];
		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			suits[i] = std::make_pair(0,Card::SUIT_ENUM_END);
		}

		for(int i = 0; i < Card::SUIT_ENUM_END;i++)
		{
			suits[i] = std::make_pair(getNumCardsInSuit((Card::SuitEnum)i),(Card::SuitEnum)i);
		}

		std::sort(suits,suits + Card::SUIT_ENUM_END);

		int count = 0;

		for(int i = 0; i < Card::SUIT_ENUM_END;i++)
		{
			if(suits[i].first == 0 || (skipSpades && suits[i].second == Card::SPADES))
				continue;

			int cIndex = getLastNthCardIndexInSuit(suits[i].second,0);
			Card card = Card::cardFromNumber(1);
			if(cIndex != -1)
				card = m_cards[cIndex];

			if(cIndex != -1 && card.getRank() < minimum)
				continue;

			if(count == countFromLast)
				return suits[i].second;

			count++;
		}
		return getNthSuitWithLowestCount(countFromLast,skipSpades);
	}

	Card::SuitEnum NetAIPartnershipSpadesBot::getNthSuitWithLowestCountMax( int countFromLast, bool skipSpades, Card::RankEnum maximum ) const
	{
		std::pair<int,Card::SuitEnum> suits[Card::SUIT_ENUM_END];
		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			suits[i] = std::make_pair(0,Card::SUIT_ENUM_END);
		}

		for(int i = 0; i < Card::SUIT_ENUM_END;i++)
		{
			suits[i] = std::make_pair(getNumCardsInSuit((Card::SuitEnum)i),(Card::SuitEnum)i);
		}

		std::sort(suits,suits + Card::SUIT_ENUM_END);

		int count = 0;

		for(int i = 0; i < Card::SUIT_ENUM_END;i++)
		{
			if(suits[i].first == 0 || (skipSpades && suits[i].second == Card::SPADES))
				continue;

			int cIndex = getFirstNthCardIndexInSuit(suits[i].second,0);
			Card card = Card::cardFromNumber(1);
			if(cIndex != -1)
				card = m_cards[cIndex];

			if(cIndex != -1 && card.getRank() > maximum)
				continue;

			if(count == countFromLast)
				return suits[i].second;

			count++;
		}
		return getNthSuitWithLowestCount(countFromLast,skipSpades);
	}

	int NetAIPartnershipSpadesBot::playRegularCard() const
	{
		if(getBid(0) > 0) //you did not nil
		{
			if(isFirstToPlay())
			{
				if(getPartnerBid() == 0) //partner did nil
				{
					botcons << "A" << std::endl;
					return playHighestCard();
				}
				else //partner did not nil
				{
					Card::SuitEnum s = hasAceKingEnabledInSuit();

					if(s != Card::SUIT_ENUM_END)
					{
						botcons << "B" << std::endl;
						return getCardIndexOf(Card::ACE,s);
					}
					else
					{
						botcons << "C" << std::endl;
						for(int i = 0; i < Card::SUIT_ENUM_END; i++)
						{
							if((Card::SuitEnum)i == Card::SPADES)
								continue;

							if(hasAnyEnabledInSuit((Card::SuitEnum)i) && getNumCardsInSuit((Card::SuitEnum)i) <= 2)
							{
								return playLowestCardInSuit((Card::SuitEnum)i);
							}
						}
						return playHighestCard();
					}
					botcons << "XoO" << std::endl;
				}
				botcons << "XXXX" << std::endl;
			}
			else //not first to play
			{
				if(didLead(3) && m_playerBid[3] == 0 && m_playerTrickCount[3] == 0)
				{
					botcons << "KILNIL" << std::endl;
					return playLowestCardFollowingSuit();

				}
				//kill his nil
				if(didLead(1) && m_playerBid[3] == 0 && m_playerTrickCount[3] == 0
					&& playedHigherCardThan(3,2) && playedHigherCardThan(3,1))
				{
					botcons << "FUKNIL" << std::endl;
					return playLowestCardFollowingSuit();
				}

				if(didLead(1) && m_playerBid[1] == 0 && m_playerTrickCount[1] == 0
					&& playedHigherCardThan(1,2) && playedHigherCardThan(1,3))
				{
					botcons << "FUKNIL2" << std::endl;
					return playLowestCardFollowingSuit();
				}

				if(didLead(3) && m_playerBid[1] == 0 && m_playerTrickCount[1] == 0
					&& m_tableCards[3].getRank() <= Card::SEVEN && 
					(!hasAnyEnabledInSuit(getLeadingSuit()) || getNumCardsInSuitBetween(getLeadingSuit(),Card::TWO,Card::SIX) > 0))
				{
					botcons << "FUKNIL3" << std::endl;
					return playLowestCardFollowingSuit();
				}

				if(didLead(2) && m_playerBid[1] == 0 && m_playerTrickCount[1] == 0
					&& m_tableCards[3].getRank() <= Card::SEVEN && m_tableCards[2].getRank() <= Card::SEVEN && m_playerBid[2] > 0 &&
					(!hasAnyEnabledInSuit(getLeadingSuit()) || getNumCardsInSuitBetween(getLeadingSuit(),Card::TWO,Card::SIX) > 0))
				{
					botcons << "FUKNIL3" << std::endl;
					return playLowestCardFollowingSuit();
				}

				if(didLead(2) && m_playerBid[3] == 0 && m_playerTrickCount[3] == 0
					&& playedHigherCardThan(3,2))
				{
					botcons << "FUKNIL4" << std::endl;
					return playLowestCardFollowingSuit();
				}


				if(!isFirstToPlay() && hasAnyEnabledInSuit(getLeadingSuit()) && hasCardHigherThanTable(getLeadingSuit()) == -1)
				{
					botcons << "PLAYLOW" << std::endl;
					return playLowestCardInSuit(getLeadingSuit());
				}

				if(getTrickSum() >= 10 && (getBid(0) + getBid(2)) <= getTricks(0) + getTricks(2) &&
					getLeadingSuit() == Card::SPADES
					&& hasAnyEnabledInSuit(getLeadingSuit()) &&
					(getBid(1) + getBid(3)) > getTricks(1) + getTricks(3))
				{ 
					botcons << "BAGS" << std::endl;
					return playLowestCardFollowingSuit();
				}

				if((getBid(0) + getBid(2)) > getTricks(0) + getTricks(2) &&
					didLead(1) && getLeadingSuit() != Card::SPADES
					&& !hasAnyEnabledInSuit(getLeadingSuit()) && hasAnyEnabledInSuit(Card::SPADES))
				{ 
					botcons << "DATSPADETHO" << std::endl;
					if(!isSuitOnTable(Card::SPADES))
					{
						return playLowestCardInSuit(Card::SPADES);
					}
					else
					{
						return playLowestAboveTableInSpades();
					}
				}

				if(getPartnerBid() == 0) //partner did nil
				{
					//protect him
					if(didLead(1) && playedHigherCardThan(2,1) && playedHigherCardThan(2,3))
					{
						botcons << "PROTECTMII" << std::endl;
						return playLowestCardAbovePlayer(2);
					}
					else if(didLead(2) && playedHigherCardThan(2,3))
					{
						botcons << "PROTECTMII2" << std::endl;
						return playLowestCardAbovePlayer(2);
					}

					if(didLead(3) && m_tableCards[3].getRank() == Card::ACE
						&& hasAnyEnabledInSuit(getLeadingSuit()))
					{
						botcons << "FUKDATACE" << std::endl;
						return playLowestCardFollowingSuit();
					}

					if(didLead(3) && m_tableCards[3].getRank() <= Card::SIX && hasAnyEnabledInSuit(getLeadingSuit()))
					{
						botcons << "POH" << std::endl;
						return playHighestCardFollowingSuit();
					}
					else if(didLead(3) &&  m_tableCards[3].getRank() <= Card::SIX &&
						!hasAnyEnabledInSuit(getLeadingSuit()) && getLeadingSuit() != Card::SPADES && hasAnyEnabledInSuit(Card::SPADES))
					{
						botcons << "PIQ" << std::endl;
						return playLowestCardInSuit(Card::SPADES);
					}

					//contract not completed
					if((getBid(0) + getBid(2)) > getTricks(0) + getTricks(2) && getTrickSum() >= 2)
					{ 
						botcons << "POP" << std::endl;
						return tryToMakePoint();
					}
					else
					{
						if(didPartnerLead())
						{
							if(m_tableIndexes[2] != -1 && allCardsBetterThanCard(m_tableCards[2]))
							{
								botcons << "D" << std::endl;
								return playLowestCardFollowingSuit();
							}
							else if(playedLowerCardThan(3,2))
							{
								botcons << "E" << std::endl;
								return playLowestCardAbovePlayer(2);
							}
							else //p3 played higher than p2
							{
								botcons << "F" << std::endl;
								return playLowestCardFollowingSuit();
							}
							botcons << "XXX" << std::endl;
						}
						else //p1 or p3 did lead
						{
							if(m_tableIndexes[2] != -1 && allCardsBetterThanCard(m_tableCards[2]))
							{
								botcons << "R" << std::endl;
								return playLowestCardFollowingSuit();
							}
							else 
							{
								if(didLead(1))
								{
									if(playedLowerCardThan(1,2) && playedLowerCardThan(3,2))
									{
										botcons << "S" << std::endl;
										return playLowestCardAbovePlayer(2);
									}
									else //(p1 or p3 > p2)
									{
										botcons << "T" << std::endl;
										return playLowestCard();
									}
								}
								else //p3 did lead
								{
									if(hasAnyEnabledInSuit(getLeadingSuit()))
									{
										botcons << "U" << std::endl;
										return playHighestCardFollowingSuit();
									}
									else
									{
										botcons << "V" << std::endl;
										return playLowestCardInSuit(Card::SPADES);
									}
								}
							}
						}
					}

					botcons << "XXXX" << std::endl;
				}
				else //partner did not bid nil
				{
					if(getBid(0) + getBid(2) > getTricks(0) + getTricks(2))
					{
						//do not cut partner
						if(didLead(1))
						{
							if(playedHigherCardThan(2,1) &&
								playedHigherCardThan(2,3))
							{
								botcons << "h" << std::endl;
								return tryNotToMakePoint();
							}
							else
							{
								botcons << "i" << std::endl;
								return tryToMakePoint();
							}
						}
						else
						{
							if(m_tableIndexes[2] != -1 && !anyCardsBetterThanCard(m_tableCards[2]))
							{
								botcons << "j" << std::endl;
								return tryNotToMakePoint();
							}
							else
							{
								botcons << "K" << std::endl;
								return tryToMakePoint();
							}
						}

					}
					else //contract is done
					{
						botcons << "L" << std::endl;
						if(!isFirstToPlay() && hasAnyEnabledInSuit(getLeadingSuit()))
							return playHighestCardUnderTable();
						else
							return tryNotToMakePoint();
					}
				}
			}
		}
		else //I did bid nil
		{
			if(getTricks(0) == 0) //nil not screwed
			{
				if(!isFirstToPlay()) //not first to play
				{
					if(hasAnyEnabledInSuit(getLeadingSuit()))
					{
						botcons << "M" << std::endl;
						return playHighestCardUnderTable();
					}
					else //(you dont have any in the suit on the table)
					{
						int highestSpade = hasCardHigherThanTable(Card::SPADES);
						if(isSuitOnTable(Card::SPADES) && highestSpade != -1)
						{
							botcons << "N" << std::endl;
							return highestSpade;
						}
						else
						{
							botcons << "O" << std::endl;
							return playHighestCardWithLeast(true);
						}
					}
				}
				else
				{
					botcons << "P" << std::endl;
					return playLowestCardWithMostLeadVersion(true,true);
				}
			}
			else //nil is screwed
			{
				botcons << "Q" << std::endl;
				return playLowestCardWithLeast(true,false);
			}
		}

		botcons << "ended" << std::endl;
		return -1;
	}

	int NetAIPartnershipSpadesBot::serverToClientPos( int pos ) const
	{
		if(m_info.isCutthroat() && pos == 3)
			pos = 2;

		return pos;
	}

	bool NetAIPartnershipSpadesBot::didPartnerLead() const
	{
		return didLead(2);
	}

	bool NetAIPartnershipSpadesBot::hasPartnerPlayed() const
	{
		return hasPlayed(2);
	}

	bool NetAIPartnershipSpadesBot::isFirstToPlay() const
	{
		for(int i = 0; i < m_tableIndexes.size(); i++)
		{
			if(m_tableIndexes[i] != -1)
				return false;
		}
		return true;
	}

	bool NetAIPartnershipSpadesBot::didLead( int playerNo ) const
	{
		return m_tableIndexes[(m_tableIndexes.size() + 
			(playerNo - 1)) % m_tableIndexes.size()] == -1 && m_tableIndexes[playerNo] != -1;
	}

	bool NetAIPartnershipSpadesBot::hasPlayed(int playerNo) const
	{
		return  m_tableIndexes[playerNo] != -1;
	}

	bool NetAIPartnershipSpadesBot::isCardOnTable( Card::RankEnum rank, Card::SuitEnum suit ) const
	{
		Card c = Card(suit,rank);
		int index = c.getCardNumber();

		for(int i = 0; i < m_tableIndexes.size();i++)
		{
			if(index == m_tableIndexes[i])
				return true;
		}

		return false;
	}

	bool NetAIPartnershipSpadesBot::hasCardBeenPlayed( Card::RankEnum rank, Card::SuitEnum suit,bool notOnTable /*= false*/ ) const
	{
			Card c = Card(suit,rank);
			for(int i = 0; i < m_playedCards.size();i++)
			{
				if(c == m_playedCards[i])
				{
					if(notOnTable && isCardOnTable(rank,suit))
					{
						return false;
					}

					return true;
				}
			}
			return false;
	}

	int NetAIPartnershipSpadesBot::getBid( int playerNo ) const
	{
		if(playerNo < m_playerBid.size())
		return m_playerBid[playerNo];
		return -1;
	}

	int NetAIPartnershipSpadesBot::getTricks( int playerNo ) const
	{
		if(playerNo < m_playerTrickCount.size())
			return m_playerTrickCount[playerNo];
		return -1;
	}

	bool NetAIPartnershipSpadesBot::hasEnabledCard( Card::RankEnum rank, Card::SuitEnum suit ) const
	{
		for(int i = 0; i < m_enabledCards.size(); i++)
		{
			if(m_enabledCards[i].getRank() == rank && m_enabledCards[i].getSuit() == suit)
				return true;
		}

		return false;
	}

	Card::SuitEnum NetAIPartnershipSpadesBot::hasAceKingEnabledInSuit() const
	{
		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			Card::SuitEnum s = (Card::SuitEnum)i;
			if(hasEnabledCard(Card::ACE,s) && hasEnabledCard(Card::KING,s))
				return s;
		}

		return Card::SUIT_ENUM_END;
	}

	int NetAIPartnershipSpadesBot::getCardIndexOf( Card::RankEnum rank, Card::SuitEnum suit ) const
	{
		for(int i = 0; i < m_cards.size(); i++)
		{
			if(m_cards[i].getRank() == rank && m_cards[i].getSuit() == suit)
			{
				return i;
			}
		}

		return -1;
	}

	int NetAIPartnershipSpadesBot::playHighestCard() const
	{
		if(!isFirstToPlay())
			return playHighestCardFollowingSuit();

		bool canPlaySpade = onlyHasSuit(Card::SPADES);
		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			if((Card::SuitEnum)i == Card::SPADES && !canPlaySpade)
				continue;

			Card::SuitEnum s = (Card::SuitEnum)i;

			if(hasEnabledCard(Card::ACE,s) && 
				hasEnabledCard(Card::KING,s) && 
				hasEnabledCard(Card::QUEEN,s) &&
				hasEnabledCard(Card::JACK,s))
			{
				return getCardIndexOf(Card::ACE,s);
			}
		}

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			if((Card::SuitEnum)i == Card::SPADES && !canPlaySpade)
				continue;

			Card::SuitEnum s = (Card::SuitEnum)i;

			if(hasEnabledCard(Card::ACE,s) && 
				hasEnabledCard(Card::KING,s) && 
				hasEnabledCard(Card::QUEEN,s))
			{
				return getCardIndexOf(Card::ACE,s);
			}
		}

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			if((Card::SuitEnum)i == Card::SPADES && !canPlaySpade)
				continue;

			Card::SuitEnum s = (Card::SuitEnum)i;

			if(hasEnabledCard(Card::ACE,s) && 
				hasEnabledCard(Card::KING,s))
			{
				return getCardIndexOf(Card::ACE,s);
			}
		}
		return playHighestCardWithMost(!canPlaySpade);
	}

	bool NetAIPartnershipSpadesBot::playedLowerCardThan( int p1,int p2 ) const
	{
		if(m_tableIndexes[p1] == -1 || m_tableIndexes[p2] == -1)
		return false;

		Card p1c = m_tableCards[p1];
		Card p2c = m_tableCards[p2];
		return playedLowerCardThan(p1c,p2c);
	}

	bool NetAIPartnershipSpadesBot::playedLowerCardThan( const Card& p1c, const Card& p2c ) const
	{

		if(p1c.getSuit() != Card::SPADES && p2c.getSuit() == Card::SPADES)
			return true;

		if(p1c.getSuit() == p2c.getSuit())
			return p1c.getRank() < p2c.getRank();

		if(getLeadingSuit() != Card::SUIT_ENUM_END && p2c.getSuit() != getLeadingSuit() &&
			p1c.getSuit() == getLeadingSuit())
		{
			return false;
		}
		return p1c < p2c;
	}

	int NetAIPartnershipSpadesBot::playLowestCardAbovePlayer( int player ) const
	{
		if(m_tableIndexes[player] == -1)
			return playLowestCardFollowingSuit();
		botcons << "LOA" << std::endl;
		Card c = m_tableCards[player]; //partners card

		if(hasAnyEnabledInSuit(getLeadingSuit()) && c.getSuit() == getLeadingSuit())
		{
			for(int i = 0; i < Card::ANY_RANK; i++)
			{
				if(hasEnabledCard((Card::RankEnum)i,getLeadingSuit()) &&
					i > c.getRank())
				{
					botcons << "LOC" << std::endl;
					return getCardIndexOf((Card::RankEnum)i,getLeadingSuit());
				}
			}
		}
		
		if(c.getSuit() == getLeadingSuit() && !hasAnyEnabledInSuit(getLeadingSuit())
			&& c.getSuit() != Card::SPADES && hasAnyEnabledInSuit(Card::SPADES))
		{
			botcons << "LOG" << std::endl;
			return playLowestCardInSuit(Card::SPADES);
		}

		if(c.getSuit() != getLeadingSuit() && !hasAnyEnabledInSuit(getLeadingSuit())
			&& c.getSuit() != Card::SPADES)
		{
			botcons << "LOB" << std::endl;
			return playLowestCardWithLeast(true,true);
		}
		
		if(c.getSuit() == Card::SPADES && hasAnyEnabledInSuit(Card::SPADES) &&
			(!hasAnyEnabledInSuit(getLeadingSuit()) || getLeadingSuit() == Card::SPADES))
		{
			for(int i = 0; i < Card::ANY_RANK; i++)
			{
				if(hasEnabledCard((Card::RankEnum)i,Card::SPADES) &&
					i > c.getRank())
				{
					botcons << "LOD" << std::endl;
					return getCardIndexOf((Card::RankEnum)i,Card::SPADES);
				}
			}
		}

		botcons << "LOE" << std::endl;
		return playHighestCardWithLeast(true);
	}

	int NetAIPartnershipSpadesBot::playLowestCardFollowingSuit() const
	{
		if(!hasAnyEnabledInSuit(getLeadingSuit()))
		{
			for(int i = 0; i < Card::SUIT_ENUM_END; i++)
			{
				if(hasAnyEnabledInSuit((Card::SuitEnum)i))
				{
					botcons << "ROA" << std::endl;
					return playLowestCardInSuit((Card::SuitEnum)i);
				}
			}
		}
		botcons << "ROB" << std::endl;
		return playLowestCardInSuit(getLeadingSuit());
	}

	int NetAIPartnershipSpadesBot::playLowestCard() const
	{
		if(hasAnyEnabledInSuit(getLeadingSuit()))
		{
			return playLowestCardFollowingSuit();
		}

		return playLowestCardWithLeast(true,true);
	}

	bool NetAIPartnershipSpadesBot::hasAnyEnabledInSuit( Card::SuitEnum suit ) const
	{
		return getNumEnabledCardsInSuit(suit) > 0;
	}

	int NetAIPartnershipSpadesBot::getNumEnabledCardsInSuit( Card::SuitEnum suit ) const
	{
		int count = 0;
		for(int i = 0; i < m_enabledCards.size(); i++)
		{
			if(m_enabledCards[i].getSuit() == suit)
				count++;
		}
		return count;
	}

	int NetAIPartnershipSpadesBot::playLowestCardInSuit( Card::SuitEnum suit ) const
	{
		if(hasAnyEnabledInSuit(suit))
		{
			Card c = m_cards[getFirstNthCardIndexInSuit(suit,0)];
			botcons << c.getRankString() << " " << c.getSuitString() << std::endl;
			botcons << "COA" << std::endl;
			return getCardIndexOf(c.getRank(),c.getSuit());
		}

		botcons << "COB" << std::endl;
		return playLowestCardFollowingSuit();
	}

	Card::SuitEnum NetAIPartnershipSpadesBot::getLeadingSuit() const
	{
		for(int i = 0; i < m_tableCards.size(); i++)
		{
			if(didLead(i))
			{
				return m_tableCards[i].getSuit();
			}
		}
		return Card::SUIT_ENUM_END;
	}

	int NetAIPartnershipSpadesBot::playHighestCardFollowingSuit() const
	{
		if(!hasAnyEnabledInSuit(getLeadingSuit()))
		{
			for(int i = 0; i < Card::SUIT_ENUM_END; i++)
			{
				if(hasAnyEnabledInSuit((Card::SuitEnum)i))
				{
					return playHighestCardInSuit((Card::SuitEnum)i);
				}
			}
		}

		return playHighestCardInSuit(getLeadingSuit());
	}

	int NetAIPartnershipSpadesBot::playHighestCardInSuit( Card::SuitEnum suit ) const
	{
		if(hasAnyEnabledInSuit(suit))
		{
			Card c = m_cards[getLastNthCardIndexInSuit(suit,0)];
			return getCardIndexOf(c.getRank(),c.getSuit());
		}

		return playHighestCardFollowingSuit();
	}

	int NetAIPartnershipSpadesBot::tryToMakePoint() const
	{
		//stub
		if(isFirstToPlay())
			return playHighestCard();

		if(hasPlayed(1) && hasPlayed(3) && hasPlayed(2) && hasAnyEnabledInSuit(getLeadingSuit()))
		{
			Card c = m_tableCards[2];
			if(playedLowerCardThan(m_tableCards[3],c) && playedLowerCardThan(m_tableCards[1],c))
			{
				return tryNotToMakePoint();
			}
		}
		if(hasPlayed(2) && getPartnerBid() > 0 && m_tableCards[2].getSuit() == getLeadingSuit() &&
			m_tableCards[2].getRank() == Card::ACE && playedLowerCardThan(m_tableCards[3],m_tableCards[2]))
		{
			return tryNotToMakePoint();
		}

		if(hasPlayed(2) && getPartnerBid() > 0 && m_tableCards[2].getSuit() == getLeadingSuit() &&
			m_tableCards[2].getRank() == Card::KING && playedLowerCardThan(m_tableCards[3],m_tableCards[2])
			&& hasCard(Card::ACE,getLeadingSuit()))
		{
			return tryNotToMakePoint();
		}

		if(hasAnyEnabledInSuit(getLeadingSuit()))
		{
			bool hasAnyThatCanBeatTable = false;

			for(int i = 0; i < Card::ANY_RANK; i++)
			{
				if(hasCard((Card::RankEnum)i,getLeadingSuit()))
				{
					Card c = Card(getLeadingSuit(),(Card::RankEnum)i);
					bool beatThemAll = true;
					for(int j = 0; j < m_tableIndexes.size(); j++)
					{
						if(m_tableIndexes[j] != -1)
						{
							Card d = m_tableCards[j];
							if(playedLowerCardThan(c,d))
							{
								beatThemAll = false;
								break;
							}
						}
					}

					if(beatThemAll)
					{
						hasAnyThatCanBeatTable = true;
						break;
					}
				}
			}

			if(!hasAnyThatCanBeatTable)
			{
				return tryNotToMakePoint();
			}
		}


		if(!hasAnyEnabledInSuit(getLeadingSuit()) && getLeadingSuit() != Card::SPADES
			&& hasAnyEnabledInSuit(Card::SPADES))
		{
			int c = -1;
			if(hasPlayed(1) && hasPlayed(3))
			{
				Card a = m_tableCards[1];
				Card b = m_tableCards[3];
				c = playedLowerCardThan(a,b) ? 3 : 1;
			}
			else if(hasPlayed(1))
			{
				c = 1;
			}
			else
			{
				c = 3;
			}

			if(m_tableCards[c].getSuit() == Card::SPADES)
			{
				return playLowestCardAbovePlayer(c);
			}
			else
			{
				return playLowestCardInSuit(Card::SPADES);
			}
		}

		if(!hasAnyEnabledInSuit(getLeadingSuit()) && hasAnyEnabledInSuit(Card::SPADES))
		{
			return playLowestCardInSuit(Card::SPADES);
		}
		if(!hasAnyEnabledInSuit(getLeadingSuit()) && !hasAnyEnabledInSuit(Card::SPADES))
		{
			return playLowestCard();
		}
		return playHighestCardInSuit(getLeadingSuit());
	}

	int NetAIPartnershipSpadesBot::tryNotToMakePoint() const
	{
		//stub
		if(isFirstToPlay())
			return playLowestCard();
		return playLowestCardInSuit(getLeadingSuit());
	}

	int NetAIPartnershipSpadesBot::playHighestCardUnderTable() const
	{
		if(!hasAnyEnabledInSuit(getLeadingSuit()))
			return -1;

		if(getLeadingSuit() != Card::SPADES && isSuitOnTable(Card::SPADES))
		{
			return playHighestCardFollowingSuit();
		}

		for(int i = m_cards.size() - 1; i >= 0; i--)
		{
			Card c = m_cards[i];

			if(c.getSuit() == getLeadingSuit())
			{
				for(int i = 0; i < m_tableCards.size(); i++)
				{
					if(m_tableIndexes[i] > -1 
						&& m_tableCards[i].getSuit() == getLeadingSuit() && m_tableCards[i].getRank() > c.getRank())
					{
						return getCardIndexOf(c.getRank(),c.getSuit());
					}
				}
			}
		}
		return playLowestCardFollowingSuit();
	}

	bool NetAIPartnershipSpadesBot::isSuitOnTable( Card::SuitEnum suit ) const
	{
		for(int i = 0; i < m_tableCards.size(); i++)
		{
			if(m_tableIndexes[i] > -1)
			{
				if(m_tableCards[i].getSuit() == suit)
				{
					return true;
				}
			}
		}
		return false;
	}

	int NetAIPartnershipSpadesBot::hasCardHigherThanTable( Card::SuitEnum suit ) const
	{
		for(int i = m_cards.size() - 1; i >= 0; i--)
		{
			Card c = m_cards[i];

			if(c.getSuit() == suit)
			{
				for(int i = 0; i < m_tableCards.size(); i++)
				{
					if(m_tableIndexes[i] > -1 
						&& m_tableCards[i].getSuit() == suit && m_tableCards[i].getRank() > c.getRank())
					{
						return getCardIndexOf(c.getRank(),c.getSuit());
					}
				}
			}
		}
		return -1;
	}

	int NetAIPartnershipSpadesBot::playHighestCardWithLeast( bool avoidSpades ) const
	{
		if(onlyHasSuit(Card::SPADES) && avoidSpades)
		{
			botcons << "FOA" << std::endl;
			return playLowestCardInSuit(Card::SPADES);
		}

		for(int i = Card::ANY_RANK - 1; i >= 0; i--)
		{
			Card::SuitEnum s = getNthSuitWithLowestCountHaving(0,avoidSpades,(Card::RankEnum)i);

			if(s != Card::SUIT_ENUM_END && hasEnabledCard((Card::RankEnum)i,s))
			{
				botcons << "FOB" << std::endl;
				return getCardIndexOf((Card::RankEnum)i,s);
			}
		}

		botcons << "FOC" << std::endl;
		return playLowestCardInSuit(Card::SPADES);
	}

	Card::SuitEnum NetAIPartnershipSpadesBot::getNthSuitWithLowestCountHaving( int countFromLast, bool skipSpades, Card::RankEnum having ) const
	{
		std::pair<int,Card::SuitEnum> suits[Card::SUIT_ENUM_END];
		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			suits[i] = std::make_pair(0,Card::SUIT_ENUM_END);
		}

		for(int i = 0; i < Card::SUIT_ENUM_END;i++)
		{
			suits[i] = std::make_pair(getNumCardsInSuit((Card::SuitEnum)i),(Card::SuitEnum)i);
		}

		std::sort(suits,suits + Card::SUIT_ENUM_END);

		int count = 0;

		for(int i = 0; i < Card::SUIT_ENUM_END;i++)
		{
			if(suits[i].first == 0 || (skipSpades && suits[i].second == Card::SPADES))
				continue;

			int cIndex = getLastNthCardIndexInSuit(suits[i].second,0);
			Card card = Card::cardFromNumber(1);
			if(cIndex != -1)
				card = m_cards[cIndex];

			if(cIndex != -1 && card.getRank() != having)
				continue;

			if(count == countFromLast)
				return suits[i].second;

			count++;
		}
		return Card::SUIT_ENUM_END;
	}

	int NetAIPartnershipSpadesBot::playLowestCardWithLeast( bool avoidSpades, bool haveMin ) const
	{
		if(onlyHasSuit(Card::SPADES) && avoidSpades)
			return playLowestCardInSuit(Card::SPADES);

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			Card::SuitEnum s = getNthSuitWithLowestCount(i,true);

			if(s != Card::SUIT_ENUM_END)
			{
				if(haveMin && getNumCardsInSuitBetween(s,Card::TWO,Card::NINE))
				{
					return playLowestCardInSuit(s);
				}
				else
				{
					return playLowestCardInSuit(s);
				}
			}
		}

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			Card::SuitEnum s = getNthSuitWithLowestCount(i,true);

			if(s != Card::SUIT_ENUM_END)
			{
				if(getNumCardsInSuitBetween(s,Card::TWO,Card::NINE))
				{
					return playLowestCardInSuit(s);
				}
			}
		}
		return playLowestCardInSuit(Card::SPADES);
	}

	Card::SuitEnum NetAIPartnershipSpadesBot::getNthSuitWithHighestCountHaving( int countFromLast, bool skipSpades, Card::RankEnum having ) const
	{
		std::pair<int,Card::SuitEnum> suits[Card::SUIT_ENUM_END];
		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			suits[i] = std::make_pair(0,Card::SUIT_ENUM_END);
		}

		for(int i = 0; i < Card::SUIT_ENUM_END;i++)
		{
			suits[i] = std::make_pair(getNumCardsInSuit((Card::SuitEnum)i),(Card::SuitEnum)i);
		}

		std::sort(suits,suits + Card::SUIT_ENUM_END,std::greater<std::pair<int,Card::SuitEnum> >());

		int count = 0;

		for(int i = 0; i < Card::SUIT_ENUM_END;i++)
		{
			if(suits[i].first == 0 || (skipSpades && suits[i].second == Card::SPADES))
				continue;

			int cIndex = getLastNthCardIndexInSuit(suits[i].second,0);
			Card card = Card::cardFromNumber(1);
			if(cIndex != -1)
				card = m_cards[cIndex];

			if(cIndex != -1 && (having != Card::ANY_RANK && card.getRank() != having))
				continue;

			if(count == countFromLast)
				return suits[i].second;

			count++;
		}
		return Card::SUIT_ENUM_END;
	}

	int NetAIPartnershipSpadesBot::playHighestCardWithMost(bool avoidSpades) const
	{
		if(onlyHasSuit(Card::SPADES) && avoidSpades)
			return playLowestCardInSuit(Card::SPADES);

		for(int i = Card::ANY_RANK - 1; i >= 0; i--)
		{
			Card::SuitEnum s = getNthSuitWithHighestCountHaving(0,avoidSpades,(Card::RankEnum)i);

			if(s != Card::SUIT_ENUM_END)
			{
				return getCardIndexOf((Card::RankEnum)i,s);
			}
		}
		return playHighestCardInSuit(Card::SPADES);
	}
	int NetAIPartnershipSpadesBot::playLowestCardWithMost(bool avoidSpades) const
	{
		if(onlyHasSuit(Card::SPADES) && avoidSpades)
			return playLowestCardInSuit(Card::SPADES);

		for(int i = 0; i < Card::ANY_RANK; i++)
		{
			Card::SuitEnum s = getNthSuitWithHighestCountHaving(0,avoidSpades,(Card::RankEnum)i);

			if(s != Card::SUIT_ENUM_END)
			{
				return getCardIndexOf((Card::RankEnum)i,s);
			}
		}

		return playLowestCardInSuit(Card::SPADES);
	}

	bool NetAIPartnershipSpadesBot::playedHigherCardThan( int p1,int p2 ) const
	{
		return playedLowerCardThan(p2,p1);
	}

	bool NetAIPartnershipSpadesBot::anyCardsBetterThanCard( const Card& c ) const
	{
		std::vector<Card> unplayedCards;

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			for(int j = 0; j < Card::ANY_RANK; j++)
			{
				Card card = Card((Card::SuitEnum)i,(Card::RankEnum)j);

				if(!hasCardBeenPlayed((Card::RankEnum)j,(Card::SuitEnum)i,true)
					&& card != c && !hasCard((Card::RankEnum)j,(Card::SuitEnum)i))
				{
					unplayedCards.push_back(card);
				}
			}
		}

		for(int i = 0; i < unplayedCards.size();i++)
		{
			if(playedLowerCardThan(c,unplayedCards[i]))
				return true;
		}

		return false;
	}

	bool NetAIPartnershipSpadesBot::allCardsBetterThanCard( const Card& c ) const
	{
		std::vector<Card> unplayedCards;

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			for(int j = 0; j < Card::ANY_RANK; j++)
			{
				Card card = Card((Card::SuitEnum)i,(Card::RankEnum)j);

				if(!hasCardBeenPlayed((Card::RankEnum)j,(Card::SuitEnum)i,true)
					&& card != c)
				{
					unplayedCards.push_back(card);
				}
			}
		}

		for(int i = 0; i < unplayedCards.size();i++)
		{
			if(playedLowerCardThan(unplayedCards[i],c))
				return false;
		}
		return true;
	}

	Card::SuitEnum NetAIPartnershipSpadesBot::hasSuitWithAceAndLessThanNCards( int numCards, bool avoidSpades ) const
	{
		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			if(avoidSpades && (Card::SuitEnum)i == Card::SPADES)
				continue;

			if(hasCard(Card::ACE,(Card::SuitEnum)i) && getNumCardsInSuit((Card::SuitEnum)i) <= numCards)
				return (Card::SuitEnum)i;
		}

		return Card::SUIT_ENUM_END;
	}

	int NetAIPartnershipSpadesBot::isLastToBid() const
	{
		for(int i = 1; i < m_playerBid.size(); i++)
		{
			if(m_playerBid[i] == -1)
				return false;
		}

		return true;
	}

	int NetAIPartnershipSpadesBot::getBidSum() const
	{
		int c = 0;
		for(int i = 1; i < m_playerBid.size(); i++)
		{
			if(m_playerBid[i] > 0)
				c += m_playerBid[i];
		}

		return c;
	}
	bool NetAIPartnershipSpadesBot::isThirdToBid() const
	{
		return m_playerBid[1] == -1 &&
			m_playerBid[2] >= 0
			&& m_playerBid[3] >= 0;
	}

	int NetAIPartnershipSpadesBot::getNumNils() const
	{
		int num = 0;
		for(int i = 0; i < 4; i++)
		{
			if(m_playerBid[i] == 0)
				num++;
		}

		return num;
	}

	int NetAIPartnershipSpadesBot::getTrickSum() const
	{
		int sum = 0;
		for(int i = 0; i < 4; i++)
		{
			if(m_playerTrickCount[i] > -1)
			{
				sum += m_playerTrickCount[i];
			}
		}
		return sum;
	}

	int NetAIPartnershipSpadesBot::playLowestAboveTableInSpades() const
	{
		if(!hasAnyEnabledInSuit(Card::SPADES))
			return playLowestCardFollowingSuit();

		if(!hasPlayed(1) || !hasPlayed(2) || !hasPlayed(3))
			return playHighestCardInSuit(Card::SPADES);

		int theWinner = -1;
		for(int i = 1; i < 4; i++)
		{
			int count = 0;

			for(int j = 1; j < 4; j++)
			{
				if(i != j && playedHigherCardThan(i,j))
				{
					count++;
				}
			}

			if(count >= 2)
			{
				theWinner = i;
			}
		}

		if(theWinner != -1)
		{
			return playLowestCardAbovePlayer(theWinner);
		}
		
		return playHighestCardInSuit(Card::SPADES);
	}

	int NetAIPartnershipSpadesBot::calcMathBid() const
	{
		float b = 0;
		//spades

		
		int num10ToA = getNumCardsInSuitBetween(Card::SPADES,Card::TEN,Card::ACE);
		int num2To9 = getNumCardsInSuitBetween(Card::SPADES,Card::TWO,Card::NINE);


		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			if((Card::SuitEnum)i == Card::SPADES)
			{
				continue;
			}

			Card::SuitEnum s = (Card::SuitEnum)i;
			int count = getNumCardsInSuit(s);
			if(hasCard(Card::ACE,s))
			{
				if(count < 7)
				{
					b += 1.0f; //incr
				}
				else if(count == 7 || count == 8)
				{
					b += 0.5f; //incr
				}
			}
			if(hasCard(Card::KING,s))
			{
				if(count == 1)
				{
					b += 0.5f; //incr
				}
				else if(count == 2 || count == 3 || count == 4)
				{
					b += 1.0f; //incr
				}
				else if(count == 5)
				{
					b += 0.25f;
				}

				if(count < 6)
				if(hasCard(Card::ACE,s) || hasCard(Card::QUEEN,s) || hasCard(Card::JACK,s))
				{
					b += 0.25f;
				}
			}

			if(hasCard(Card::ACE,s) || hasCard(Card::QUEEN,s) || hasCard(Card::KING,s))
			{
				b += 0.5f;
			}
			else if(hasCard(Card::QUEEN,s) && count <= 5 )
			{
				b += 0.25f;
			}

			if(num2To9 >= 2 && count < 2)
			{
				b += 1.5f; //used to be 1
			}
		}


		//spades
		{
			/*
			for(int i = 0; i < m_cards.size(); i++)
			{
				if(m_cards[i].getSuit() == Card::SPADES)
					cardsAdded.push_back(m_cards[i]);
			}


			int stopPos = 0;
			for(int i = Card::ANY_RANK - 1; i >= 0; i--)
			{
				if(hasCard((Card::RankEnum)i,Card::SPADES))
				{
					bid++;
					stopPos++;
					for(int j = 0; j < cardsAdded.size(); j++)
					{
						if(cardsAdded[j].getRank() == (Card::RankEnum)i)
						{
							cardsAdded.erase(cardsAdded.begin() + j);
							break;
						}
					}
				}
				else
				{
					break;
				}
			}

			*/

			int cardCost = 0;
			int usedCount = 0;
			int remainingCount = num2To9;
			std::vector<Card> cardsAdded;

			botcons << "num between = " << num2To9 << std::endl;
			for(int i = cardsAdded.size() - 1; i >= 0; i--)
			{
				Card c = cardsAdded[i];

				botcons << c.getRankString() << std::endl;
				if(c.getRank() >= Card::TEN)
				{
					int cost = getCardCost(c);
					cardCost += cost;
					botcons << "Card Cost" << cardCost << std::endl;
					if(cardCost <= num2To9 || 
						(c.getRank() == Card::JACK && hasCard(Card::TEN,Card::SPADES) && cardCost - 1 <= num2To9))
					{
						usedCount += cost;
						remainingCount = num2To9 - usedCount;
						b += 1.0f;
						botcons << "added from cost" << std::endl;
					}
					else if(cardCost - 1 <= num2To9 && cardCost - 1 > 0)
					{
						b += 0.5f;
					}
					else if(cardCost - 2 <= num2To9 && cardCost - 2 > 0)
					{
						b += 0.25f;
					}
				}
				else //2 - 9
				{
					if(i <= usedCount - 1)
						break;
				}
			}

			if(getNumCardsInSuit(Card::SPADES) == 6)
			{
				b += 1.0f;
			}
			else if(getNumCardsInSuit(Card::SPADES) > 6)
			{
				b += 1.0f * (getNumCardsInSuit(Card::SPADES) - 6);
			}

			if(isLastToBid() && getNumNils() > 0)
			{
				b += 0.5f;
			}
			else if(isLastToBid() && getNumNils() > 0)
			{
				b += 0.75f;
			}
			
			if(isLastToBid() && getBidSum() + b < 10)
			{
				b += 1.75f;
			}

			if(getNumNils()  == 2 && getBidSum() + b < 10)
			{
				b += 1.25f;
			}
			int ret = 0;
			ret = (int)b;

		
			if(getBidSum() + ret > 12)
			{
				ret = 12 - getBidSum();
			}

			if(ret <= 0)
				ret = 1;

			return ret;
		}
	}

	int NetAIPartnershipSpadesBot::playLowestCardWithMostLeadVersion( bool avoidSpades, bool haveMin ) const
	{
		if(onlyHasSuit(Card::SPADES) && avoidSpades)
			return playLowestCardInSuit(Card::SPADES);

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			Card::SuitEnum s = getNthSuitWithHighestCountHaving(i,true,Card::ANY_RANK);

			if(s != Card::SUIT_ENUM_END)
			{
				if(haveMin && getNumCardsInSuitBetween(s,Card::TWO,Card::SIX) > 0)
				{
					return playLowestCardInSuit(s);
				}
				else
				{
					return playLowestCardInSuit(s);
				}
			}
		}

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			Card::SuitEnum s = getNthSuitWithHighestCountHaving(i,true,Card::ANY_RANK);

			if(s != Card::SUIT_ENUM_END)
			{
				return playLowestCardInSuit(s);
			}
		}

		return playLowestCardInSuit(Card::SPADES);
	}

}
