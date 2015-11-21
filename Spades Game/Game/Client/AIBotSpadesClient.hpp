#ifndef CGE_AI_BOT_SPADES_CLIENT
#define CGE_AI_BOT_SPADES_CLIENT
#include "Game/Client/SpadesClient.hpp"
#include "Game/Card/Card.hpp"
namespace cge
{
	class AIBotSpadesClient : public SpadesClient
	{
		int id;
		std::vector<Card> m_cards;
		std::vector<int> m_enabledCards;
	public:
		
		AIBotSpadesClient(void);
		virtual void processMessageFromGame(SpadesCSMessageEnum msg, 
			const std::vector<int>& args);
		virtual ~AIBotSpadesClient(void);
	};
}

#endif
