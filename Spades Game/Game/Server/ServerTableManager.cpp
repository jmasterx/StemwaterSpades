#include "Game/Server/ServerTableManager.hpp"
namespace cge
{
	ServerTableManager::ServerTableManager(void)
		: m_start(0)
	{
		
	}

	ServerTableManager::~ServerTableManager(void)
	{
		freeTables();
	}

	void ServerTableManager::createTables( int start, int quantity, ServerListener* listener )
	{
		freeTables();
		m_start = start;

		for(int i = start; i < quantity + start; ++i)
		{
			m_tables.push_back(new ServerTable(i));
			m_tables.back()->getGame().setServerListener(listener);
		}
	}

	void ServerTableManager::freeTables()
	{
		for(size_t i = 0; i < m_tables.size(); ++i)
		{
			delete m_tables[i];
		}

		m_tables.clear();
	}

	ServerTable* ServerTableManager::getTableAt( int i )
	{
		int index = i - m_start;

		if(index >= 0 && index < m_tables.size())
		{
			return m_tables[index];
		}

		return NULL;
	}

	int ServerTableManager::getTableStart() const
	{
		return m_start;
	}

	int ServerTableManager::getNumTables() const
	{
		return m_tables.size();
	}

	ServerTableArray::iterator ServerTableManager::begin()
	{
		return m_tables.begin();
	}

	ServerTableArray::iterator ServerTableManager::end()
	{
		return m_tables.end();
	}

	ServerTableArray* ServerTableManager::getTables()
	{
		return &m_tables;
	}

	ServerTable* ServerTableManager::findEmptyTable()
	{
		for(size_t i = 0; i < m_tables.size(); ++i)
		{
			if(!m_tables[i]->isActive() && !m_tables[i]->isInProgress() && 
				!m_tables[i]->isBeingSetup() && m_tables[i]->getNumOccupants() == 0)
			{
				return m_tables[i];
			}
		}

		return NULL;
	}
}
