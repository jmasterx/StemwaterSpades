#include "Game/UI/TableManager.hpp"
#include "Game/Utility/File.hpp"
#include <sstream>
namespace cge
{
	TableManager::~TableManager(void)
	{
		_cleanTable();
		_cleanTables();
	}

	TableManager::TableManager( const std::string& path, const std::string& tableFile )
		: m_currentTableData(NULL), m_currentTable(NULL), m_currentTableShadow(NULL)
	{
		_loadTables(path,tableFile);
	}

	bool TableManager::loadTable( TableData* data )
	{
		if(!data)
		{
			return false;
		}

		Sprite* table = new Sprite(data->m_tablePath);
		Sprite* tableShadow = new Sprite(data->m_tableShadowPath);

		if(!table->getBitmap() || !tableShadow->getBitmap())
		{
			delete table;
			delete tableShadow;
			return false;
		}

		_cleanTable();
		m_currentTable = table;
		m_currentTableShadow = tableShadow;
		m_currentTableData = data;

		return true;
	}

	void TableManager::_cleanTable()
	{
		if(m_currentTable)
		{
			delete m_currentTable;
			m_currentTable = NULL;
		}

		if(m_currentTableShadow)
		{
			delete m_currentTableShadow;
			m_currentTableShadow = NULL;
		}
	}

	bool TableManager::isTableLoaded() const
	{
		return m_currentTableData != NULL;
	}

	TableData* TableManager::getCurrentTableData() const
	{
		return m_currentTableData;
	}

	Sprite* TableManager::getCurrentTableShadow() const
	{
		return m_currentTableShadow;
	}

	Sprite* TableManager::getCurrentTable() const
	{
		return m_currentTable;
	}

	bool TableManager::_loadTables( const std::string& path, const std::string& tableFile )
	{
		std::string fullPath = tableFile;	

		_cleanTable();
		_cleanTables();
		m_tables.clear();
		std::stringstream ss;
		//open the file
		File ifs( fullPath.c_str() );

		//ensure it is open
		if(!ifs.isOpen())
		{
			return false;
		}
		std::string line;

		//read each line
		while( ifs.readLine(line) )
		{

			if(line.length() == 0)
			{
				continue;
			}
			else if(line.length() >= 1 && line[0] == '#')
			{
				continue;
			}

			if(line.length() >= 1 && line[0] != '@')
			{
				continue;
			}

			//name, bmp1, bmp2, thumbnail, id
			std::string attrib[5];
			Sprite* tableThumbImg = NULL;

			for(int i = 0; i < 5; ++i)
			{
				if(!ifs.readLine(line))
					return false;

				int pos = -1;
				for(size_t j = 0; j < line.length(); ++j)
				{
					if(line[j] == ':')
					{
						pos = j;
						break;
					}
				}

				//need 4 valid attributes
				if(pos == -1)
				{
					return false;
				}

				//move to first character
				pos+= 2;
				if(pos < (int)line.length())
				{
					attrib[i] = line.substr(pos);
				}
			}

			if(attrib[3] != "")
			{
				tableThumbImg = new Sprite(attrib[3]);
			}

			ss.clear();
			ss.str("");
			int id = -1;
			ss << attrib[4];
			ss >> id;

			m_tables.push_back(TableData(tableThumbImg,
				path + attrib[1],path + attrib[2],attrib[0],id));
		}

		return true;
	}

	void TableManager::_cleanTables()
	{
		for(std::list<TableData>::iterator it = m_tables.begin();
			it != m_tables.end(); ++it)
		{
			if(it->m_thumbnail)
			{
				delete it->m_thumbnail;
				it->m_thumbnail = NULL;
			}
		}
	}

	TableData* TableManager::getTableByName( const std::string& name )
	{
		for(std::list<TableData>::iterator it = m_tables.begin();
			it != m_tables.end(); ++it)
		{
			if(it->m_name == name)
			{
				return &(*it);
			}
		}

		return NULL;
	}

	int TableManager::getTableCount() const
	{
		return m_tables.size();
	}

	std::list<TableData>::iterator TableManager::begin()
	{
		return m_tables.begin();
	}

	std::list<TableData>::iterator TableManager::end()
	{
		return m_tables.end();
	}

	TableData* TableManager::getTableAt( int index )
	{
		int count = 0;
		for(std::list<TableData>::iterator it = m_tables.begin();
			it != m_tables.end(); ++it)
		{
			if(count == index)
			{
				return &(*it);
			}

			count++;
		}

		return NULL;
	}

	TableData* TableManager::getTableFromID( int id )
	{
		int count = 0;
		for(std::list<TableData>::iterator it = m_tables.begin();
			it != m_tables.end(); ++it)
		{
			if(it->id == id)
			{
				return &(*it);
			}

			count++;
		}

		return NULL;
	}

	void TableManager::unload()
	{
		_cleanTable();
	}

}
