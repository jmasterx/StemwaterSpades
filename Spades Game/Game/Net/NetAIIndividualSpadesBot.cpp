#include "Game/Net/NetAIIndividualSpadesBot.hpp"
#include "Game/Net/NetSpadesGame.hpp"
#include "Game/Utility/console.hpp"
static console botcons(false);
namespace cge
{
	NetAIIndividualSpadesBot::NetAIIndividualSpadesBot(void)
		: m_game(NULL)
	{
		m_bidRatios.push_back(1.0f);
		m_bidRatios.push_back(0.25f);
		m_bidRatios.push_back(0.25f);
		m_bidRatios.push_back(0.5f);
		m_bidRatios.push_back(0.25f);
		m_bidRatios.push_back(0.25f);
		m_bidRatios.push_back(0.5f);
		m_bidRatios.push_back(0.25f);
		m_bidRatios.push_back(1.25f);
		m_bidRatios.push_back(1.0f);
		m_bidRatios.push_back(0.5f);
		m_bidRatios.push_back(0.25f);
		m_bidRatios.push_back(1.25f);
		m_bidRatios.push_back(1.0f);
		m_bidRatios.push_back(0.5f);
		m_bidRatios.push_back(0.7f);
		m_bidRatios.push_back(1.0f);
		m_bidRatios.push_back(1.0f);

		for(int i = 0; i < m_bidRatios.size(); i++)
		{
			m_bidRatios[i] *= 0.9f;
			m_bidRatios[i] += 0.1f;
		}

		m_cutRatios.push_back(0.9f);
		m_cutRatios.push_back(0.75f);
		m_cutRatios.push_back(0.75f);
		m_cutRatios.push_back(1.0f);
		m_cutRatios.push_back(0.75f);
		m_cutRatios.push_back(0.75f);
		m_cutRatios.push_back(1.0f);
		m_cutRatios.push_back(0.5f);
		m_cutRatios.push_back(1.0f);
		m_cutRatios.push_back(1.0f);
		m_cutRatios.push_back(0.75f);
		m_cutRatios.push_back(0.75f);
		m_cutRatios.push_back(1.0f);
		m_cutRatios.push_back(1.0f);
		m_cutRatios.push_back(0.75f);
		m_cutRatios.push_back(0.75f);
		m_cutRatios.push_back(1.0f);
		m_cutRatios.push_back(1.0f);

		for(int i = 0; i < m_cutRatios.size(); i++)
		{
			m_cutRatios[i] *= 1.1f;
			m_cutRatios[i] -= 0.05f;
		}
	}

	NetAIIndividualSpadesBot::~NetAIIndividualSpadesBot(void)
	{
	}

	void NetAIIndividualSpadesBot::receive( const SpadesGameMessage& msg )
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
				if(m_info.isIndividual())
				cardCount.push_back(regularBid());
				else if(m_info.isCutthroat())
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
				if(m_info.isIndividual() || m_info.isCutthroat())
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

	void NetAIIndividualSpadesBot::setGame( NetSpadesGame* game )
	{
		m_game = game;
	}

	void NetAIIndividualSpadesBot::sendMessageToGame( const SpadesCSMessageEnum& msg, const std::vector<int>& args )
	{
		if(m_game)
		{
			m_game->botSendMessageToGame(msg,args,this);
		}
	}

	int NetAIIndividualSpadesBot::getNumCardsInRank( Card::RankEnum rank ) const
	{
		int count = 0;
		for(int i = 0; i < m_cards.size(); i++)
		{
			if(m_cards[i].getRank() == rank)
				count++;
		}

		return count;
	}

	int NetAIIndividualSpadesBot::getNumCardsInSuit( Card::SuitEnum suit ) const
	{
		int count = 0;
		for(int i = 0; i < m_cards.size(); i++)
		{
			if(m_cards[i].getSuit() == suit)
				count++;
		}

		return count;
	}

	int NetAIIndividualSpadesBot::getNumCardsInRankAndSuitGreaterOrEqualTo( Card::RankEnum rank, Card::SuitEnum suit ) const
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

	bool NetAIIndividualSpadesBot::nilRegular() const
	{
		if(m_info.isCutthroat())
			return nilCutthroat();

		if(getNumCardsInSuit(Card::SPADES) > 2)
			return false;
		if(getNumCardsInRankAndSuitGreaterOrEqualTo(Card::JACK,Card::SPADES) > 0)
			return false;

		if(getNumCardsInSuitBetween(Card::SPADES,Card::EIGHT,Card::TEN) >= 3)
			return false;

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			if(getNumCardsInSuit((Card::SuitEnum)i) <= 4 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::ACE,(Card::SuitEnum)i) >= 1)
				return false;

			if(getNumCardsInSuit((Card::SuitEnum)i) <= 4 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::TEN,(Card::SuitEnum)i) >= 2)
				return false;

			if(getNumCardsInSuit((Card::SuitEnum)i) <= 4 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::JACK,(Card::SuitEnum)i) >= 1)
				return false;

			if(getNumCardsInSuit((Card::SuitEnum)i) <= 5 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::TEN,(Card::SuitEnum)i) >= 3)
				return false;
		}

		return true;
	}

	int NetAIIndividualSpadesBot::getNumCardsInSuitBetween( Card::SuitEnum suit,Card::RankEnum minimum, Card::RankEnum maximum ) const
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

	int NetAIIndividualSpadesBot::regularBid() const
	{
		if(nilRegular())
			return 0;
		if(m_info.isCutthroat())
			return calcMathBidCutthroat();

			return calcMathBid();
	}

	
	int NetAIIndividualSpadesBot::getLastNthCardIndexInSuit( Card::SuitEnum suit, int countFromLast ) const
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

	cge::Card::SuitEnum NetAIIndividualSpadesBot::getNthSuitWithLowestCount( int countFromLast, bool skipSpades ) const
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

	bool NetAIIndividualSpadesBot::onlyHasSuit( Card::SuitEnum suit ) const
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

	int NetAIIndividualSpadesBot::getFirstNthCardIndexInSuit( Card::SuitEnum suit, int countFromFirst ) const
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

	void NetAIIndividualSpadesBot::setGameInfo( const std::vector<int>& args )
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

	int NetAIIndividualSpadesBot::calcBasicBid() const
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

	bool NetAIIndividualSpadesBot::hasCard( Card::RankEnum rank, Card::SuitEnum suit ) const
	{
		for(int i = 0; i < m_cards.size(); i++)
		{
			if(m_cards[i].getRank() == rank && m_cards[i].getSuit() == suit)
				return true;
		}

		return false;
	}

	int NetAIIndividualSpadesBot::getCardCost( const Card& c ) const
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

	Card::SuitEnum NetAIIndividualSpadesBot::getNthSuitWithLowestCountMin( int countFromLast, bool skipSpades, Card::RankEnum minimum ) const
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

	Card::SuitEnum NetAIIndividualSpadesBot::getNthSuitWithLowestCountMax( int countFromLast, bool skipSpades, Card::RankEnum maximum ) const
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

	int NetAIIndividualSpadesBot::playRegularCard() const
	{
		if(getBid(0) > 0) //you did not nil
		{
			if(isFirstToPlay())
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
			else //not first to play
			{
				if(getBid(0) > getTricks(0))
				{
					//do not cut partner
					return tryToMakePoint();
				}
				else //contract is done
				{
					if(!isFirstToPlay() && hasAnyEnabledInSuit(getLeadingSuit()))
						return playHighestCardUnderTable();
					else
						return tryNotToMakePoint();
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

	int NetAIIndividualSpadesBot::serverToClientPos( int pos ) const
	{
		if(m_info.isCutthroat() && pos == 3)
			pos = 2;

		return pos;
	}

	bool NetAIIndividualSpadesBot::isFirstToPlay() const
	{
		for(int i = 0; i < m_info.getNumPlayers(); i++)
		{
			if(m_tableIndexes[i] != -1)
				return false;
		}
		return true;
	}

	bool NetAIIndividualSpadesBot::didLead( int playerNo ) const
	{
		return m_tableIndexes[(m_info.getNumPlayers() + 
			(playerNo - 1)) % m_info.getNumPlayers()] == -1 && m_tableIndexes[playerNo] != -1;
	}

	bool NetAIIndividualSpadesBot::hasPlayed(int playerNo) const
	{
		return  m_tableIndexes[playerNo] != -1;
	}

	bool NetAIIndividualSpadesBot::isCardOnTable( Card::RankEnum rank, Card::SuitEnum suit ) const
	{
		Card c = Card(suit,rank);
		int index = c.getCardNumber();

		for(int i = 0; i < m_info.getNumPlayers();i++)
		{
			if(index == m_tableIndexes[i])
				return true;
		}

		return false;
	}

	bool NetAIIndividualSpadesBot::hasCardBeenPlayed( Card::RankEnum rank, Card::SuitEnum suit,bool notOnTable /*= false*/ ) const
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

	int NetAIIndividualSpadesBot::getBid( int playerNo ) const
	{
		if(playerNo < m_playerBid.size())
		return m_playerBid[playerNo];
		return -1;
	}

	int NetAIIndividualSpadesBot::getTricks( int playerNo ) const
	{
		if(playerNo < m_playerTrickCount.size())
			return m_playerTrickCount[playerNo];
		return -1;
	}

	bool NetAIIndividualSpadesBot::hasEnabledCard( Card::RankEnum rank, Card::SuitEnum suit ) const
	{
		for(int i = 0; i < m_enabledCards.size(); i++)
		{
			if(m_enabledCards[i].getRank() == rank && m_enabledCards[i].getSuit() == suit)
				return true;
		}

		return false;
	}

	Card::SuitEnum NetAIIndividualSpadesBot::hasAceKingEnabledInSuit() const
	{
		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			Card::SuitEnum s = (Card::SuitEnum)i;
			if(hasEnabledCard(Card::ACE,s) && hasEnabledCard(Card::KING,s))
				return s;
		}

		return Card::SUIT_ENUM_END;
	}

	int NetAIIndividualSpadesBot::getCardIndexOf( Card::RankEnum rank, Card::SuitEnum suit ) const
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

	int NetAIIndividualSpadesBot::playHighestCard() const
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

	bool NetAIIndividualSpadesBot::playedLowerCardThan( int p1,int p2 ) const
	{
		if(m_tableIndexes[p1] == -1 || m_tableIndexes[p2] == -1)
		return false;

		Card p1c = m_tableCards[p1];
		Card p2c = m_tableCards[p2];
		return playedLowerCardThan(p1c,p2c);
	}

	bool NetAIIndividualSpadesBot::playedLowerCardThan( const Card& p1c, const Card& p2c ) const
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

	int NetAIIndividualSpadesBot::playLowestCardAbovePlayer( int player ) const
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

	int NetAIIndividualSpadesBot::playLowestCardFollowingSuit() const
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

	int NetAIIndividualSpadesBot::playLowestCard() const
	{
		if(hasAnyEnabledInSuit(getLeadingSuit()))
		{
			return playLowestCardFollowingSuit();
		}

		return playLowestCardWithLeast(true,true);
	}

	bool NetAIIndividualSpadesBot::hasAnyEnabledInSuit( Card::SuitEnum suit ) const
	{
		return getNumEnabledCardsInSuit(suit) > 0;
	}

	int NetAIIndividualSpadesBot::getNumEnabledCardsInSuit( Card::SuitEnum suit ) const
	{
		int count = 0;
		for(int i = 0; i < m_enabledCards.size(); i++)
		{
			if(m_enabledCards[i].getSuit() == suit)
				count++;
		}
		return count;
	}

	int NetAIIndividualSpadesBot::playLowestCardInSuit( Card::SuitEnum suit ) const
	{
		if(hasAnyEnabledInSuit(suit))
		{
			int ind = getFirstNthCardIndexInSuit(suit,0);
			botcons << "ind " << ind << std::endl;
			Card c = m_cards[ind];
			botcons << c.getRankString() << " " << c.getSuitString() << std::endl;
			botcons << "COA" << std::endl;
			int coaVal = getCardIndexOf(c.getRank(),c.getSuit());
			botcons << "coaval " << coaVal << std::endl;
			return coaVal;
		}

		botcons << "COB" << std::endl;
		return playLowestCardFollowingSuit();
	}

	Card::SuitEnum NetAIIndividualSpadesBot::getLeadingSuit() const
	{
		for(int i = 0; i < m_info.getNumPlayers(); i++)
		{
			if(didLead(i))
			{
				return m_tableCards[i].getSuit();
			}
		}
		return Card::SUIT_ENUM_END;
	}

	int NetAIIndividualSpadesBot::playHighestCardFollowingSuit() const
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

	int NetAIIndividualSpadesBot::playHighestCardInSuit( Card::SuitEnum suit ) const
	{
		if(hasAnyEnabledInSuit(suit))
		{
			Card c = m_cards[getLastNthCardIndexInSuit(suit,0)];
			return getCardIndexOf(c.getRank(),c.getSuit());
		}

		return playHighestCardFollowingSuit();
	}

	int NetAIIndividualSpadesBot::tryToMakePoint() const
	{
		//stub
		if(isFirstToPlay())
			return playHighestCard();


		if(hasAnyEnabledInSuit(getLeadingSuit()))
		{
			bool hasAnyThatCanBeatTable = false;

			for(int i = 0; i < Card::ANY_RANK; i++)
			{
				if(hasCard((Card::RankEnum)i,getLeadingSuit()))
				{
					Card c = Card(getLeadingSuit(),(Card::RankEnum)i);
					bool beatThemAll = true;
					for(int j = 0; j < m_info.getNumPlayers(); j++)
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
			Card highest = Card::cardFromNumber(1);
			int player = -1;

			for(size_t i = 1; i < m_info.getNumPlayers(); i++) {
				if(hasPlayed(i) && !(m_tableCards[i] < highest)) {
					highest = m_tableCards[i];
					player = i;
				}
			}

			if(player != -1 && highest.getSuit() == Card::SPADES)
			{
				return playLowestCardAbovePlayer(player);
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

	int NetAIIndividualSpadesBot::tryNotToMakePoint() const
	{
		//stub
		if(isFirstToPlay())
			return playLowestCard();
		return playLowestCardInSuit(getLeadingSuit());
	}

	int NetAIIndividualSpadesBot::playHighestCardUnderTable() const
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
				for(int i = 0; i < m_info.getNumPlayers(); i++)
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

	bool NetAIIndividualSpadesBot::isSuitOnTable( Card::SuitEnum suit ) const
	{
		for(int i = 0; i < m_info.getNumPlayers(); i++)
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

	int NetAIIndividualSpadesBot::hasCardHigherThanTable( Card::SuitEnum suit ) const
	{
		for(int i = m_cards.size() - 1; i >= 0; i--)
		{
			Card c = m_cards[i];

			if(c.getSuit() == suit)
			{
				for(int i = 0; i < m_info.getNumPlayers(); i++)
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

	int NetAIIndividualSpadesBot::playHighestCardWithLeast( bool avoidSpades ) const
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

	Card::SuitEnum NetAIIndividualSpadesBot::getNthSuitWithLowestCountHaving( int countFromLast, bool skipSpades, Card::RankEnum having ) const
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

	int NetAIIndividualSpadesBot::playLowestCardWithLeast( bool avoidSpades, bool haveMin ) const
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

	Card::SuitEnum NetAIIndividualSpadesBot::getNthSuitWithHighestCountHaving( int countFromLast, bool skipSpades, Card::RankEnum having ) const
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

	int NetAIIndividualSpadesBot::playHighestCardWithMost(bool avoidSpades) const
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
	int NetAIIndividualSpadesBot::playLowestCardWithMost(bool avoidSpades) const
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

	bool NetAIIndividualSpadesBot::playedHigherCardThan( int p1,int p2 ) const
	{
		return playedLowerCardThan(p2,p1);
	}

	bool NetAIIndividualSpadesBot::anyCardsBetterThanCard( const Card& c ) const
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

	bool NetAIIndividualSpadesBot::allCardsBetterThanCard( const Card& c ) const
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

	Card::SuitEnum NetAIIndividualSpadesBot::hasSuitWithAceAndLessThanNCards( int numCards, bool avoidSpades ) const
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

	int NetAIIndividualSpadesBot::isLastToBid() const
	{
		for(int i = 1; i < m_playerBid.size(); i++)
		{
			if(m_playerBid[i] == -1)
				return false;
		}

		return true;
	}

	int NetAIIndividualSpadesBot::getBidSum() const
	{
		int c = 0;
		for(int i = 1; i < m_playerBid.size(); i++)
		{
			if(m_playerBid[i] > 0)
				c += m_playerBid[i];
		}

		return c;
	}
	bool NetAIIndividualSpadesBot::isThirdToBid() const
	{
		return m_playerBid[1] == -1 &&
			m_playerBid[2] >= 0
			&& m_playerBid[3] >= 0;
	}

	int NetAIIndividualSpadesBot::getNumNils() const
	{
		int num = 0;
		for(int i = 0; i < m_info.getNumPlayers(); i++)
		{
			if(m_playerBid[i] == 0)
				num++;
		}

		return num;
	}

	int NetAIIndividualSpadesBot::getTrickSum() const
	{
		int sum = 0;
		for(int i = 0; i < m_info.getNumPlayers(); i++)
		{
			if(m_playerTrickCount[i] > -1)
			{
				sum += m_playerTrickCount[i];
			}
		}
		return sum;
	}

	int NetAIIndividualSpadesBot::playLowestAboveTableInSpades() const
	{
		if(!hasAnyEnabledInSuit(Card::SPADES))
			return playLowestCardFollowingSuit();

		if(!hasAllPlayed())
			return playHighestCardInSuit(Card::SPADES);

		int theWinner = -1;
		for(int i = 1; i < m_info.getNumPlayers(); i++)
		{
			int count = 0;

			for(int j = 1; j < m_info.getNumPlayers(); j++)
			{
				if(i != j && playedHigherCardThan(i,j))
				{
					count++;
				}
			}

			if(count >= m_info.getNumPlayers() - 1)
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

	int NetAIIndividualSpadesBot::calcMathBid() const
	{
		float b = 0.25f;
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
					//b += 0.75f; //incr
					b += m_bidRatios[0];
				}
				else if(count == 7 || count == 8)
				{
					//b += 0.25f; //incr
					b += m_bidRatios[1];
				}
			}
			if(hasCard(Card::KING,s))
			{
				if(count == 1)
				{
					//b += 0.25f; //incr
					b += m_bidRatios[2];
				}
				else if(count == 2 || count == 3 || count == 4)
				{
					//b += 0.75f; //incr
					b += m_bidRatios[3];
				}
				else if(count == 5)
				{
					//b += 0.25f;
					b += m_bidRatios[4];
				}

				if(count < 6)
				if(hasCard(Card::ACE,s) || hasCard(Card::QUEEN,s) || hasCard(Card::JACK,s))
				{
					//b += 0.25f;
					b += m_bidRatios[5];
				}
			}

			if(hasCard(Card::ACE,s) || hasCard(Card::QUEEN,s) || hasCard(Card::KING,s))
			{
				//b += 0.5f;
				b += m_bidRatios[6];
			}
			else if(hasCard(Card::QUEEN,s) && count <= 5 )
			{
				//b += 0.25f;
				b += m_bidRatios[7];
			}

			if(num2To9 >= 2 && count < 2)
			{
				//b += 1.5f; //used to be 1
				b += m_bidRatios[8];
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
						//b += 1.0f;
						b += m_bidRatios[9];
						botcons << "added from cost" << std::endl;
					}
					else if(cardCost - 1 <= num2To9 && cardCost - 1 > 0)
					{
						//b += 0.5f;
						b += m_bidRatios[10];
					}
					else if(cardCost - 2 <= num2To9 && cardCost - 2 > 0)
					{
						//b += 0.25f;
						b += m_bidRatios[11];
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
				//b += 1.0f;
				b += m_bidRatios[12];
			}
			else if(getNumCardsInSuit(Card::SPADES) > 6)
			{
				//b += 1.0f * (getNumCardsInSuit(Card::SPADES) - 6);
				b += m_bidRatios[13] * (getNumCardsInSuit(Card::SPADES) - 6);
			}

			if(isLastToBid() && getNumNils() > 0)
			{
				//b += 0.5f;
				b += m_bidRatios[14];
			}
			else if(isLastToBid() && getNumNils() > 0)
			{
				//b += 0.75f;
				b += m_bidRatios[15];
			}
			
			if(isLastToBid() && getBidSum() + b < 10)
			{
				//b += 1.75f;
				b += m_bidRatios[16];
			}

			if(getNumNils()  == 2 && getBidSum() + b < 10)
			{
				//b += 1.25f;
				b += m_bidRatios[17];
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

	int NetAIIndividualSpadesBot::playLowestCardWithMostLeadVersion( bool avoidSpades, bool haveMin ) const
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

	bool NetAIIndividualSpadesBot::hasAllPlayed() const
	{
		for(int i = 1; i < m_info.getNumPlayers(); i++)
		{
			if(!hasPlayed(i))
				return false;
		}

		return true;
	}

	int NetAIIndividualSpadesBot::calcMathBidCutthroat() const
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
				if(count < 9)
				{
					b += m_cutRatios[0]; //incr
				}
				else if(count == 7 || count == 8)
				{
					b += m_cutRatios[1]; //incr
				}
			}
			if(hasCard(Card::KING,s))
			{
				if(count == 1)
				{
					b += m_cutRatios[2]; //incr
				}
				else if(count == 2 || count == 3 || count == 4 || count == 5 || count == 6 || count == 7 || count == 8)
				{
					b += m_cutRatios[3]; //incr
				}
				else if(count == 8)
				{
					b += m_cutRatios[4];
				}

				if(count < 8)
				if(hasCard(Card::ACE,s) || hasCard(Card::QUEEN,s) || hasCard(Card::JACK,s))
				{
					b += m_cutRatios[5];
				}
			}

			if(hasCard(Card::ACE,s) || hasCard(Card::QUEEN,s) || hasCard(Card::KING,s))
			{
				b += m_cutRatios[6];
			}
			else if(hasCard(Card::QUEEN,s) && count <= 8 )
			{
				b += m_cutRatios[7];
			}

			if(num2To9 >= 2 && count < 2)
			{
				b += m_cutRatios[8]; //used to be 1
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
						b += m_cutRatios[9];
						botcons << "added from cost" << std::endl;
					}
					else if(cardCost - 1 <= num2To9 && cardCost - 1 > 0)
					{
						b += m_cutRatios[10];
					}
					else if(cardCost - 2 <= num2To9 && cardCost - 2 > 0)
					{
						b += m_cutRatios[11];
					}
				}
				else //2 - 9
				{
					if(i <= usedCount - 1)
						break;
				}
			}

			if(getNumCardsInSuit(Card::SPADES) == 8)
			{
				b += m_cutRatios[12];
			}
			else if(getNumCardsInSuit(Card::SPADES) > 8)
			{
				b += m_cutRatios[13] * (getNumCardsInSuit(Card::SPADES) - 8);
			}

			if(isLastToBid() && getNumNils() > 0)
			{
				b += m_cutRatios[14];
			}
			else if(isLastToBid() && getNumNils() > 0)
			{
				b += m_cutRatios[15];
			}
			
			if(isLastToBid() && getBidSum() + b < 10)
			{
				b += m_cutRatios[16];
			}

			if(getNumNils()  == 2 && getBidSum() + b < 10)
			{
				b += m_cutRatios[17];
			}

			int ret = 0;
			ret = (int)b;
		
			if(getBidSum() + ret > 16)
			{
				ret = 16 - getBidSum();
			}

			if(ret <= 0)
				ret = 1;

			if(ret > 6)
			{
				ret = 6;
			}

			return ret;
		}
	}

	bool NetAIIndividualSpadesBot::nilCutthroat() const
	{
		if(getNumCardsInSuit(Card::SPADES) > 2)
			return false;
		if(getNumCardsInRankAndSuitGreaterOrEqualTo(Card::JACK,Card::SPADES) > 0)
			return false;

		if(getNumCardsInSuitBetween(Card::SPADES,Card::EIGHT,Card::TEN) >= 3)
			return false;

		for(int i = 0; i < Card::SUIT_ENUM_END; i++)
		{
			if(getNumCardsInSuit((Card::SuitEnum)i) <= 7 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::ACE,(Card::SuitEnum)i) >= 1)
				return false;

			if(getNumCardsInSuit((Card::SuitEnum)i) <= 6 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::TEN,(Card::SuitEnum)i) >= 2)
				return false;

			if(getNumCardsInSuit((Card::SuitEnum)i) <= 6 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::JACK,(Card::SuitEnum)i) >= 1)
				return false;

			if(getNumCardsInSuit((Card::SuitEnum)i) <= 6 && 
				getNumCardsInRankAndSuitGreaterOrEqualTo(Card::TEN,(Card::SuitEnum)i) >= 3)
				return false;
		}

		return true;
	}

}
