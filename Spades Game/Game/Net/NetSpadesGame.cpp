#include "Game/Net/NetSpadesGame.hpp"
namespace cge
{
	NetSpadesGame::NetSpadesGame(void)
		: m_listener(NULL),m_tableNo(-1)
	{
		for(int i = 0; i < MAX_SPADE_CLIENTS; ++i)
		{
			m_clients[i].setAsNull();
		}

		for(int i = 0; i < MAX_SPADE_BOTS; ++i)
		{
			m_partnerBots[i].setGame(this);
			m_individualBots[i].setGame(this);
		}

	}

	NetSpadesGame::~NetSpadesGame(void)
	{
	}

	void NetSpadesGame::setClient( ServerPeer* client, int pos )
	{
		if(pos < MAX_SPADE_CLIENTS)
		{
			if(client)
			m_clients[pos].setAsClient(client);
			else
				m_clients[pos].setAsNull();
		}
	}

	void NetSpadesGame::sendMessageToPlayer( const SpadesGameMessage& message, bool distribute /*= false*/ )
	{	
		if(message.getMessage() == GAME_OVER_S  && m_listener)
		{
			m_listener->gameCompleted(m_tableNo);
			return;
		}

		if(message.getPlayer() == -1 || message.getPlayer() == 0 || distribute)
		{
			handleWatchers(message,distribute);
			
			if(message.getPlayer() == -1)
			{
				return;
			}
		}

		m_enc.gameMessageS(message.getMessage(),message.getArgs(),message.getPlayer());
		if(distribute)
		{
			for(int i = 0; i < MAX_SPADE_CLIENTS; ++i)
			{
				if(!m_clients[i].isNull())
				{
					m_clients[i].write(m_enc.getLastMessage().c_str());
					m_clients[i].write(message);
				}
			}
		}
		else if(!m_clients[message.getPlayer()].isNull())
		{
			m_clients[message.getPlayer()].write(m_enc.getLastMessage().c_str());
			m_clients[message.getPlayer()].write(message);
		}
	}

	void NetSpadesGame::sendMessageToGame( const SpadesGameMessage& msg )
	{
		pushMessage(msg);
		processMessages();
	}

	void NetSpadesGame::sendGameInfo( const SpadesGameInfo& info )
	{
		sendMessageToGame(SpadesGameMessage(SET_GAME_INFO_C,info.toIntArray(),0));

		if(info.isBotGame())
		{
			setBots(info);
		}
	}

	void NetSpadesGame::reset()
	{
		SpadesGame::reset();

		for(int i = 0; i < MAX_SPADE_CLIENTS; ++i)
		{
			m_clients[i].setAsNull();
		}

	}

	int NetSpadesGame::mapPos( int pos )
	{
		if(getGameInfo().getNumPlayers() < 4)
		{
			if(pos == 3)
			{
				pos = 2;
			}
			else if(pos == 2)
			{
				pos = 3;
			}
		}

		return pos;
	}
	void NetSpadesGame::addWatcher( ServerPlayer* player )
	{
		m_watchers.push_back(player);
	}

	void NetSpadesGame::removeWatcher( ServerPlayer* player )
	{
		for (size_t i = 0; i < m_watchers.size(); ++i)
		{
			if(m_watchers[i] == player)
			{
				m_watchers.erase(m_watchers.begin() + i);
			}
		}
	}

	void NetSpadesGame::handleWatchers( const SpadesGameMessage& message, bool distribute )
	{
		SpadesCSMessageEnum msg = message.getMessage();
		bool properMessage = false;
		switch(msg)
		{
		case DISPATCH_CARDS_A_D_S:
		case PERFORM_SWAP_S:
		case PLAYER_MADE_TRICK_A_D_S:
		case SEND_PLAYER_TRICK_COUNT_D_S:
		case SEND_ROUND_SCORES_D_S:
		case SEND_ACTIVE_PLAYER_S:
		case PERFORM_CARD_ON_TABLE_A_D_S:
		case ROUND_BEGIN_S:
		case GAME_OVER_S:
			properMessage = true;
			break;
		default:
			break;
		}

		if(!properMessage)
		{
			return;
		}

		std::vector<int> args = message.getArgs();
		if(message.getMessage() == DISPATCH_CARDS_A_D_S && args.size() > 4)
		{
			for(size_t i = 4; i < args.size(); ++i)
			{
				args[i] = -1;
			}
		}
		else if(message.getMessage() == PERFORM_SWAP_S && args.size() > 1)
		{
			if(args[0] != 2)
			{
				for(size_t i = 1; i < args.size();++i)
				{
					args[i] = -1;
				}
			}
		}

		m_enc.gameMessageS(message.getMessage(),args,0);
		for(size_t i = 0; i < m_watchers.size(); ++i)
		{
			m_watchers[i]->getClient()->write(m_enc.getLastMessage().c_str());
		}
	}

	ServerPlayer* NetSpadesGame::getWatcherAt( int index )
	{
		if(index >=0 && index < m_watchers.size())
		{
			return m_watchers[index];
		}
		else
		{
			return NULL;
		}
	}

	int NetSpadesGame::getNumWatchers() const
	{
		return m_watchers.size();
	}

	void NetSpadesGame::setServerListener( ServerListener* listener )
	{
		m_listener = listener;
	}

	void NetSpadesGame::setTableNo( int no )
	{
		m_tableNo = no;
	}

	void NetSpadesGame::restart()
	{
		SpadesGame::reset();
	}

	void NetSpadesGame::clearWatchers()
	{
		m_watchers.clear();
	}

	void NetSpadesGame::botSendMessageToGame( const SpadesCSMessageEnum& msg, const std::vector<int>& args, NetAISpadesBot* bot )
	{
		int pos = -1;

		for(int i = 0; i < MAX_SPADE_CLIENTS; i++)
		{
			if(bot == m_clients[i].getBot())
			{
				pos = i;
			}
		}

		if(pos != -1)
		{
			sendMessageToGame(SpadesGameMessage(msg,args,pos));
		}
	}

	void NetSpadesGame::setBots(const SpadesGameInfo& info, bool allBots)
	{
		int numToDo = info.getNumPlayers() - 1;
		if(allBots)
			numToDo = info.getNumPlayers();
		int botIndex = 0;

		for(int i = 0; i < info.getNumPlayers(); i++)
		{
			if(!m_clients[i].isClient())
			{
				if(info.isPartnership() || info.isPassTwo()) {
					m_clients[i].setAsBot(&m_partnerBots[botIndex]);
				}
				else {
					m_clients[i].setAsBot(&m_individualBots[botIndex]);
				}
				
				sendMessageToGame(SpadesGameMessage(PLAYER_JOINED_C,i));
				botIndex++;
			}

			if(botIndex == numToDo)
			{
				break;
			}
		}
	}

	void NetSpadesGame::flushBotCache()
	{
		//flush bot messages
		bool didSomething = true;
		while(didSomething)
		{
			didSomething = false;
			for(int i = 0; i < MAX_SPADE_CLIENTS; i++)
			{
				if(m_clients[i].flush())
				{
					didSomething = true;
				}
			}
		}
	}

	SpadesClientWrapper* NetSpadesGame::getPlayerAtPos( int pos )
	{
		for(int i = 0; i < MAX_SPADE_CLIENTS; i++)
		{
			if(i == pos)
				return &m_clients[i];
		}

		return NULL;
	}

	void NetSpadesGame::playerRequestDoAction( int playerNo )
	{
		SpadesClientWrapper* w = getPlayerAtPos(playerNo);
		if(w && w->isClient() && m_listener)
		{
			m_listener->playerRequestDoAction(m_tableNo,playerNo);
		}
	}

	void NetSpadesGame::playerFinishedAction( int playerNo )
	{
		SpadesClientWrapper* w = getPlayerAtPos(playerNo);
		if(w && w->isClient() && m_listener)
		{
			m_listener->playerFinishedDoingAction(m_tableNo,playerNo);
		}
	}

	void SpadesClientWrapper::setAsClient( ServerPeer* client )
	{
		m_bot = NULL;
		m_client = client;
		m_isClient = true;
	}

	void SpadesClientWrapper::setAsBot( NetAISpadesBot* bot )
	{
		m_bot = bot;
		m_client = NULL;
		m_isClient = false;
	}

	SpadesClientWrapper::SpadesClientWrapper()
	{
		setAsNull();
	}

	void SpadesClientWrapper::setAsNull()
	{
		m_bot = NULL;
		m_client = NULL;
		m_isClient = NULL;
		
		while(!m_botMsgs.empty())
		{
			m_botMsgs.pop();
		}
	}

	bool SpadesClientWrapper::isNull() const
	{
		return m_bot == NULL && m_client == NULL;
	}

	void SpadesClientWrapper::write( const char* str )
	{
		if(!isNull())
		{
			if(m_isClient && m_client)
			{
				m_client->write(str);
			}
		}
	}

	void SpadesClientWrapper::write( const SpadesGameMessage& msg )
	{
		if(!m_isClient && m_bot)
		{
			m_botMsgs.push(msg);
		}
	}

	NetAISpadesBot* SpadesClientWrapper::getBot() const
	{
		return m_bot;
	}

	bool SpadesClientWrapper::isClient() const
	{
		return m_isClient;
	}

	bool SpadesClientWrapper::flush()
	{
		bool ret = !m_botMsgs.empty();
		while(!m_botMsgs.empty())
		{
			if(m_bot)
			{
				m_bot->receive(m_botMsgs.front());
			}
			m_botMsgs.pop();
		}

		return ret;
	}

	ServerPeer* SpadesClientWrapper::getClient()
	{
		return m_client;
	}

}
