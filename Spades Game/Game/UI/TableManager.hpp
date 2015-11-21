#ifndef CGE_TABLE_MANAGER_HPP
#define CGE_TABLE_MANAGER_HPP
#include "Game/platform.hpp"
#include <Game/Resource/Sprite.hpp>
#include <string>
#include <list>
namespace cge
{
	struct TableData
	{
		Sprite* m_thumbnail;
		std::string m_tablePath;
		std::string m_tableShadowPath;
		std::string m_name;
		int id;

		TableData(Sprite* thumbnail, std::string tablePath, 
			std::string tableShadowPath, std::string name, int tid)
			: m_thumbnail(thumbnail), m_tablePath(tablePath),
			m_tableShadowPath(tableShadowPath), m_name(name),id(tid)
		{

		}
	};

	class TableManager
	{
		std::list<TableData> m_tables;
		bool _loadTables(const std::string& path, const std::string& tableFile);
		Sprite* m_currentTable;
		Sprite* m_currentTableShadow;
		TableData* m_currentTableData;
		void _cleanTable();
		void _cleanTables();
	public:
		TableManager(const std::string& path, const std::string& tableFile);
		bool loadTable(TableData* data);
		bool isTableLoaded() const;
		TableData* getCurrentTableData() const;
		Sprite* getCurrentTable() const;
		Sprite* getCurrentTableShadow() const;
		TableData* getTableByName(const std::string& name);
		int getTableCount() const;
		std::list<TableData>::iterator begin();
		std::list<TableData>::iterator end();
		TableData* getTableAt(int index);
		TableData* getTableFromID(int id);
		void unload();
		virtual ~TableManager(void);
	};
}

#endif
