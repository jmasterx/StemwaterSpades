#include "Game/Net/ServerEventProvider.hpp"
#include "Game/Net/ServerCore.hpp"
namespace cge
{
	ServerEventProvider::ServerEventProvider(void)
		: m_core(NULL)
	{
	}

	ServerEventProvider::~ServerEventProvider(void)
	{
	}

	void ServerEventProvider::addServerListener( ServerListener* listener )
	{
		m_serverListeners.push_back(listener);
	}

	void ServerEventProvider::removeServerListener( ServerListener* listener )
	{
		m_serverListeners.erase(
			std::remove(m_serverListeners.begin(),
			m_serverListeners.end(), listener),
			m_serverListeners.end());
	}

	void ServerEventProvider::clearServerListeners()
	{
		m_serverListeners.clear();
	}

	void ServerEventProvider::addServerEventProvider( ServerEventProvider* listener )
	{
		m_serverEventProviders.push_back(listener);
		linkServerListeners();
	}

	void ServerEventProvider::addServerEventListener( ServerListener* listener )
	{
		m_serverEventListeners.push_back(listener);
		linkServerListeners();
	}

	void ServerEventProvider::linkServerListeners()
	{
		unlinkServerListeners();

		for(size_t i = 0; i < m_serverEventProviders.size(); ++i)
		{
			for(size_t j = 0; j < m_serverEventProviders.size(); ++j)
			{
				if(m_serverEventProviders[j] != m_serverEventProviders[i])
				{
					m_serverEventProviders[i]->addServerListener(m_serverEventProviders[j]);
				}
			}

			for(size_t j = 0; j < m_serverEventListeners.size(); ++j)
			{
				if(m_serverEventProviders[i] != m_serverEventListeners[j])
				{
					m_serverEventProviders[i]->addServerListener(m_serverEventListeners[j]);
				}
			}
		}
	}

	void ServerEventProvider::unlinkServerListeners()
	{
		for(size_t i = 0; i < m_serverEventProviders.size(); ++i)
		{
			for(size_t j = 0; j < m_serverEventProviders.size(); ++j)
			{
				if(m_serverEventProviders[j] != m_serverEventProviders[i])
				{
					m_serverEventProviders[i]->removeServerListener(m_serverEventProviders[j]);
				}
			}

			for(size_t j = 0; j < m_serverEventListeners.size(); ++j)
			{
				if(m_serverEventProviders[i] != m_serverEventListeners[j])
				{
					m_serverEventProviders[i]->removeServerListener(m_serverEventListeners[j]);
				}
			}
		}
	}

	void ServerEventProvider::setServerCore( ServerCore* server )
	{
		m_core = server;
	}

	ServerCore* ServerEventProvider::getServerCore()
	{
		return m_core;
	}

}
