#include "Game/CardView/CardImageManager.hpp"

namespace cge
{
	CardImageManager::CardImageManager( 
		const std::string& basePath, const std::string& imageSize, const std::string& deck )
		:m_backCard(NULL), m_shadow(NULL)
	{
		loadCards(basePath + imageSize + std::string("/") + deck + std::string("/"));
	}

	CardImageManager::~CardImageManager(void)
	{
		freeCardMemory();
	}

	void CardImageManager::loadCards( const std::string& path )
	{
		freeCardMemory();
		m_cards = std::vector<Sprite*>(52,NULL);

		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 13; ++j)
			{
				int cardNum = cardNumber((Card::SuitEnum)i,(Card::RankEnum)j);

				if(cardNum < 52 && m_cards[cardNum] == NULL)
				{
					m_cards[cardNum] = 
						new Sprite(path + cardFileName((Card::SuitEnum)i,(Card::RankEnum)j));
				}
			}
		}

		if(m_backCard == NULL)
		{
			m_backCard = new Sprite(path + "cb.png");
		}

		if(m_shadow == NULL)
		{
			m_shadow = new Sprite(path + "cs.png");
		}

		m_flip.push_back(new Sprite(path + "cbf0.png"));
		m_flip.push_back(new Sprite(path + "cbf1.png"));
		m_flip.push_back(new Sprite(path + "cbf2.png"));
		m_flip.push_back(new Sprite(path + "cbf3.png"));
		m_flip.push_back(new Sprite(path + "cbf4.png"));
		m_flip.push_back(new Sprite(path + "cbf5.png"));
		m_flip.push_back(new Sprite(path + "cbf6.png"));
		m_flip.push_back(new Sprite(path + "cbf7.png"));
		m_flip.push_back(new Sprite(path + "cbf8.png"));
		m_flip.push_back(new Sprite(path + "cbf9.png"));
	}

	void CardImageManager::freeCardMemory()
	{
		for (size_t i = 0; i < m_cards.size(); ++i)
		{
			delete m_cards[i];
		}

		m_cards.clear();

		for (size_t i = 0; i < m_flip.size(); ++i)
		{
			delete m_flip[i];
		}

		m_flip.clear();

		if(m_backCard)
		{
			delete m_backCard;
			m_backCard = NULL;
		}

		if(m_shadow)
		{
			delete m_shadow;
			m_shadow = NULL;
		}
	}

	int CardImageManager::cardNumber( Card::SuitEnum suit, Card::RankEnum rank ) const
	{
		Card c = Card(suit,rank);
		return c.getCardNumber();
	}

	std::string CardImageManager::cardFileName( Card::SuitEnum suit, Card::RankEnum rank ) const
	{
		std::string suitStr;
		std::string rankStr;

		switch (suit)
		{
		case Card::HEARTS:
			suitStr = "h";
			break;
		case Card::CLUBS:
			suitStr = "c";
			break;
		case Card::DIAMONDS:
			suitStr = "d";
			break;
		case Card::SPADES:
			suitStr = "s";
			break;
		default:
			suitStr = "?";
		}

		switch(rank)
		{
		case Card::ACE:
			rankStr = "a";
			break;
		case Card::TWO:
			rankStr = "2";
			break;
		case Card::THREE:
			rankStr = "3";
			break;
		case Card::FOUR:
			rankStr = "4";
			break;
		case Card::FIVE:
			rankStr = "5";
			break;
		case Card::SIX:
			rankStr = "6";
			break;
		case Card::SEVEN:
			rankStr = "7";
			break;
		case Card::EIGHT:
			rankStr = "8";
			break;
		case Card::NINE:
			rankStr = "9";
			break;
		case Card::TEN:
			rankStr = "10";
			break;
		case Card::JACK:
			rankStr = "j";
			break;
		case Card::QUEEN:
			rankStr = "q";
			break;
		case Card::KING:
			rankStr = "k";
			break;
		default:
			rankStr = "Unknown Rank";
		}

		//png file extension
		return rankStr + suitStr + ".png";
	}

	Sprite* CardImageManager::getCardImage( const Card& card ) const
	{
		if(card.getCardNumber() == -1)
		{
			return m_backCard;
		}

		if(card.getCardNumber() < 52)
		{
			return m_cards[card.getCardNumber()];
		}

		return NULL;
	}

	Sprite* CardImageManager::getCardImage( int cardNumber ) const
	{
		if(cardNumber == -1)
		{
			return m_backCard;
		}

		if(cardNumber < 52)
		{
			return m_cards[cardNumber];
		}

		return NULL;
	}

	Sprite* CardImageManager::getBackCardImage() const
	{
		return m_backCard;
	}

	Sprite* CardImageManager::getShadowImage() const
	{
		return m_shadow;
	}

	Sprite* CardImageManager::getFlipImage( int imgNo ) const
	{
		if(imgNo < m_flip.size() && imgNo >= 0)
			return m_flip[imgNo];
		return NULL;
	}

}