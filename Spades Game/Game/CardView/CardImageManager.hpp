#ifndef CGE_IMAGE_MANAGER_HPP
#define CGE_IMAGE_MANAGER_HPP
#include "Game/Resource/Sprite.hpp"
#include "Game/Card/Card.hpp"
#include <vector>
namespace cge
{
	class CardImageManager
	{
		std::vector<Sprite*> m_cards;
		std::vector<Sprite*> m_flip;
		Sprite* m_backCard;
		Sprite* m_shadow;
		void freeCardMemory();
		int cardNumber(Card::SuitEnum suit, Card::RankEnum rank) const;
		std::string cardFileName(Card::SuitEnum suit, Card::RankEnum rank) const;
	public:
		void loadCards(const std::string& path);
		Sprite* getBackCardImage() const;
		Sprite* getFlipImage(int imgNo) const;
		Sprite* getShadowImage() const;
		Sprite* getCardImage(const Card& card) const;
		Sprite* getCardImage(int cardNumber) const;

		CardImageManager(const std::string& basePath, const std::string& imageSize, const std::string& deck);
		virtual ~CardImageManager(void);
	};
}

#endif

