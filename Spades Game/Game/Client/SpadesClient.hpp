#ifndef CGE_SPADES_CLIENT_HPP
#define CGE_SPADES_CLIENT_HPP
#include "Game/SpadesGame/SpadesMessageSender.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
namespace cge
{
	class SpadesClient : public SpadesMessageSender
	{
		bool m_isHost;
		SpadesGameInfo m_gameInfo;
		SpadesGameMessageSender* m_mediator;
	protected:
		SpadesGameMessageSender* getMediator();
	public:
		SpadesClient(void);
		bool isHost() const;
		void setMediator(SpadesGameMessageSender* sender);
		virtual void sendGameMessage(SpadesCSMessageEnum msg, 
			const std::vector<int>& args, SpadesGameMessageSender* sender);
		virtual void sendMessageToGame(SpadesCSMessageEnum msg,
			const std::vector<int>& args);	
		virtual void processMessageFromGame(SpadesCSMessageEnum msg,
			const std::vector<int>& args) = 0;
		//Host tells server the rules AFTER he has connected to the server
		virtual void setAsHost(const std::vector<int>& gameInfo);
		 bool setGameInfo(const std::vector<int>& gameInfo);
		 const SpadesGameInfo& getGameInfo() const;
		virtual ~SpadesClient(void);
	};
}

#endif
