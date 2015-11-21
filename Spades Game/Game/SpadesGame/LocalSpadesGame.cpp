#include "Game/SpadesGame/LocalSpadesGame.hpp"
#include <algorithm>
namespace cge
{
	LocalSpadesGame::LocalSpadesGame(void)
		: m_mediators(4,NULL)
	{
	}

	LocalSpadesGame::~LocalSpadesGame(void)
	{
	}

	void LocalSpadesGame::addMediator( LocalMessageMediator* mediator, int index )
	{
		m_mediators[index] = mediator;
		mediator->setGameMessenger(this);
		//send the joined message
		sendGameMessage(PLAYER_JOINED_C,std::vector<int>(),mediator);
	}

	void LocalSpadesGame::removeMediator( int index )
	{
		if(index < (int)m_mediators.size())
		{
			//send the disconnected message
			sendGameMessage(PLAYER_DISCONNECTED_C,std::vector<int>(),m_mediators[index]);
			m_mediators[index] = NULL;
		}
	}

	void LocalSpadesGame::removeMediator( LocalMessageMediator* mediator )
	{
		sendGameMessage(PLAYER_DISCONNECTED_C,std::vector<int>(),mediator);
		for(size_t i = 0; i < m_mediators.size(); ++i)
		{
			if(m_mediators[i] == mediator)
			{
				m_mediators[i] = NULL;
			}
		}
	}

	LocalMessageMediator* LocalSpadesGame::getMediator( int index )
	{
		if(index < (int)m_mediators.size())
		{
			return m_mediators[index];
		}

		return NULL;
	}

	void LocalSpadesGame::sendGameMessage( SpadesCSMessageEnum msg, 
		const std::vector<int>& args,
		SpadesGameMessageSender* sender )
	{
		//code to figure out which client and send accordingly
		for(size_t i = 0; i < m_mediators.size(); ++i)
		{
			if(m_mediators[i] == sender)
			{
				pushMessage(SpadesGameMessage(msg,args,i));
				break;
			}
		}
	
	}

	void LocalSpadesGame::sendMessageToPlayer( const SpadesGameMessage& message, bool distribute )
	{
		if(message.getPlayer() == -1)
		{
			return;
		}

		if(distribute)
		{
			for(size_t i = 0; i < m_mediators.size(); ++i)
			{
				if(m_mediators[i] != NULL)
					m_mediators[i]->sendGameMessage(
						message.getMessage(),message.getArgs(),this);
			}
		}
		else
		{
			for(size_t i = 0; i < m_mediators.size(); ++i)
			{
				if(m_mediators[i] != NULL)
				if(message.getPlayer() == i)
				{
					m_mediators[i]->sendGameMessage(
						message.getMessage(),message.getArgs(),this);
					break;
				}
			}
		}
	
	}

	void LocalSpadesGame::initBots( int numBots )
	{
		if(numBots < 2 || numBots > 3)
		{
			return;
		}

		for(size_t i = 0; i < m_aiMediators.size(); ++i)
		{
			removeMediator(&m_aiMediators[i]);
		}
		m_aiMediators.clear();
		m_bots.clear();


		for(int i = 0; i < numBots; ++i)
		{
			m_aiMediators.push_back(LocalMessageMediator());
			m_bots.push_back(AIBotSpadesClient());
		}

		for(int i = 0; i < numBots; ++i)
		{
			m_bots[i].setMediator(&m_aiMediators[i]);
			m_aiMediators[i].setClient(&m_bots[i]);
			m_aiMediators[i].setGameMessenger(this);

			addMediator(&m_aiMediators[i],i + 1);
		}
	}

}

