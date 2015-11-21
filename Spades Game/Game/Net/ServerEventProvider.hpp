#ifndef CGE_SERVER_EVENT_PROVIDER_HPP
#define CGE_SERVER_EVENT_PROVIDER_HPP
#include "Game/Net/ServerListener.hpp"
#include <stdlib.h>
#include <vector>
#define DISPATCH_SERVER_EVENT	for(std::vector<ServerListener*>::iterator it =  m_serverListeners.begin(); it != m_serverListeners.end(); ++it)
namespace cge
{
	class ServerCore;
	class ServerEventProvider : public ServerListener
	{
		std::vector<ServerEventProvider*> m_serverEventProviders;
		std::vector<ServerListener*> m_serverEventListeners;
		ServerCore* m_core;
	protected:
			std::vector<ServerListener*> m_serverListeners;
	public:
		virtual void addServerListener(ServerListener* listener);
		virtual void removeServerListener(ServerListener* listener);
		virtual void clearServerListeners();
		virtual void addServerEventProvider(ServerEventProvider* listener);
		virtual void addServerEventListener(ServerListener* listener);
		void linkServerListeners();
		void unlinkServerListeners();
		void setServerCore(ServerCore* server);
		ServerCore* getServerCore();
		ServerEventProvider(void);
		virtual ~ServerEventProvider(void);
	};
}

#endif
