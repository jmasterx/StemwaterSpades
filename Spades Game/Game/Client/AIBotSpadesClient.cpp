#include "Game/Client/AIBotSpadesClient.hpp"
#include <stdlib.h>
#include <iostream>
#include <algorithm>
namespace cge
{
	AIBotSpadesClient::AIBotSpadesClient(void)
	{
		id = rand() % 1500;
	}

	AIBotSpadesClient::~AIBotSpadesClient(void)
	{
	}

	void AIBotSpadesClient::processMessageFromGame(
		SpadesCSMessageEnum msg, const std::vector<int>& args )
	{
		//code to process event

		switch(msg)
		{
		case SEND_GAME_INFO_D_S:
			setGameInfo(args);
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
				for(size_t i = 4; i < args.size(); i++)
				{
					cards.push_back(Card::cardFromNumber(args[i]));
				}
				m_cards = cards;
				sendMessageToGame(FINISH_DISPATCH_CARDS_C,std::vector<int>());
			}
			break;
		case REQUEST_PLAYER_TRICK_COUNT_S:
			{
				std::vector<int> cardCount;
				cardCount.push_back(2);
				sendMessageToGame(SEND_PLAYER_TRICK_COUNT_C,cardCount);
			}
			break;
		case SEND_PLAYER_TRICK_COUNT_D_S:
			{
	
			}
			break;
		case REQUEST_SELECTED_CARD_S:
			{
				std::vector<int> cardCount;
				int r = 0;
				r = rand() % (int)m_enabledCards.size();

				cardCount.push_back(m_enabledCards[r]);
				m_cards.erase(m_cards.begin() + m_enabledCards[r]);
				std::sort(m_cards.begin(),m_cards.end());
				sendMessageToGame(SEND_SELECTED_CARD_C,cardCount);
				std::vector<int> cc;
				sendMessageToGame(FINISH_PERFORM_CARD_ON_TABLE_C,cc);
			}
			break;
		case FILTER_CARDS_S:
			{
				m_enabledCards.clear();
				for(int i = 0; i < m_cards.size(); ++i)
				{
					m_enabledCards.push_back(i);
				}

				for(int i = 0; i < args.size(); i++)
				{
					for(int j = 0; j < m_enabledCards.size(); ++j)
					{
						if(m_enabledCards[j] == args[i])
						{
							m_enabledCards.erase(m_enabledCards.begin() + j);
							break;
						}
					}
				}

			}
			break;
		case PERFORM_CARD_ON_TABLE_A_D_S:
			{
				std::vector<int> cardCount;
				sendMessageToGame(FINISH_PERFORM_CARD_ON_TABLE_C,cardCount);
			}
			break;
		case REQUEST_CARD_SWAP_S:
			{
				std::vector<int> cardCount;
				int r = rand() % m_cards.size();
					cardCount.push_back(m_cards[r].getCardNumber());
				m_cards.erase(m_cards.begin() + r);
				int  p = rand() % m_cards.size();
				cardCount.push_back(m_cards[p].getCardNumber());
				m_cards.erase(m_cards.begin() + p);
				

				sendMessageToGame(SEND_SWAPPED_CARDS_C,cardCount);
			}
			break;
		case PERFORM_SWAP_S:
			{
				if(args[0] == 0 || args[0] == 1) //does it need to swap?
				{
					std::vector<Card> incoming;

					incoming.push_back(Card::cardFromNumber(args[1]));
					incoming.push_back(Card::cardFromNumber(args[2]));

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
				std::vector<int> cardCount;
				sendMessageToGame(FINISHED_PLAYER_MADE_TRICK_C,cardCount);
			}
			
			break;
		}

	}

}
