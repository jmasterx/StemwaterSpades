#ifndef CGE_LOCAL_SPADES_GAME_HPP
#define CGE_LOCAL_SPADES_GAME_HPP
#include "Game/SpadesGame/SpadesGameMessageSender.hpp"
#include "Game/Mediator/LocalMessageMediator.hpp"
#include "Game/SpadesGame/SpadesGame.hpp"
#include "Game/Client/AIBotSpadesClient.hpp"
namespace cge
{
	class LocalSpadesGame : 
		public SpadesGame, public SpadesGameMessageSender
	{
		std::vector<LocalMessageMediator*> m_mediators;
		std::vector<LocalMessageMediator> m_aiMediators;
		std::vector<AIBotSpadesClient> m_bots;
	public:
		LocalSpadesGame(void);
		virtual void sendGameMessage(SpadesCSMessageEnum msg, 
			const std::vector<int>& args,SpadesGameMessageSender* sender );
		virtual void sendMessageToPlayer(const SpadesGameMessage& message,
			bool distribute = false);
		void addMediator(LocalMessageMediator* mediator, int index);
		void removeMediator(int index);
		void removeMediator(LocalMessageMediator* mediator);
		LocalMessageMediator* getMediator(int index);
		void initBots(int numBots);
		virtual ~LocalSpadesGame(void);
	};
}

#endif
