#ifndef CGE_SERVER_TABLE_MANAGER_HPP
#define CGE_SERVER_TABLE_MANAGER_HPP
#include "Game/Server/ServerTable.hpp"
#include "Game/Net/ServerListener.hpp"
namespace cge
{
	class ServerTableManager
	{
		ServerTableArray m_tables; //free
		int m_start;
		void freeTables();
	public:
		ServerTableManager(void);
		void createTables(int start, int quantity, ServerListener* listener);
		ServerTable* getTableAt(int i);
		int getTableStart() const;
		int getNumTables() const;
		ServerTableArray::iterator begin();
		ServerTableArray::iterator end();
		ServerTableArray* getTables();
		ServerTable* findEmptyTable();
		~ServerTableManager(void);
	};

}

#endif