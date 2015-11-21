#include "Game/Engine/ListPairLoader.hpp"
#include "Game/Utility/File.hpp"
namespace cge
{

	ListPairLoader::~ListPairLoader(void)
	{
		clear();
	}

	bool ListPairLoader::_initStrings( const std::string& path, const std::string& appendPath )
	{
		//open the file
		File ifs( path.c_str() );

		//ensure it is open
		if(!ifs.isOpen())
		{
			return false;
		}
		std::string line;

		int uni32 = 128522;
		//read each line
		while( ifs.readLine(line) )
		{

			if(line.length() == 0 || line.length() < 6)
			{
				continue;
			}
			else if(line.length() >= 1 && line[0] == '#')
			{
				continue;
			}

			int lineAt = 0;
			std::string elemName;

			//extract until space
			while(lineAt < (int)line.length())
			{
				if(line[lineAt] != ' ')
				{
					elemName += line[lineAt];
					lineAt++;
				}
				else 
				{
					lineAt += 3; //' = '
					break;
				}
			}

			std::string elemPath;

			//extract until end
			while(lineAt < (int)line.length())
			{

				//filter newline
				if(line[lineAt] == '\\' && lineAt + 1 < 
					(int)line.length() && line[lineAt + 1] == 'n')
				{
					elemPath += '\n';
					lineAt += 2;
				}
				else 
				{
					elemPath += line[lineAt];
					lineAt++;
				}
			}

			ListPairItem item;
			item.descr = elemName;
			item.image = agui::Image::load(appendPath + elemPath);

			char buffer[8];
			for(int i = 0; i < 8; ++i)
			{
				buffer[i] = 0;
			}

			agui::UTF8 unicodeFunctions;
			unicodeFunctions.encodeUtf8(buffer,uni32);
			std::string character = buffer;

			item.val = character;
			uni32++;
			m_strings.push_back(item);
			m_stringsSorted.push_back(item);
		}

		std::sort(m_stringsSorted.begin(),m_stringsSorted.end());
		return true;
	}

	const std::vector<ListPairItem >& ListPairLoader::getStrings() 
	{
		return m_strings;
	}

	void ListPairLoader::clear()
	{
		for(int i = 0; i < m_strings.size(); ++i)
		{
		 delete m_strings[i].image;
		}

		m_strings.clear();
		m_stringsSorted.clear();
	}

	const std::vector<ListPairItem>& ListPairLoader::getStringsSorted()
	{
		return m_stringsSorted;
	}

	agui::Image* ListPairLoader::getImage( const std::string& descr )
	{
		for(int i = 0; i < getStringsSorted().size(); ++i)
		{
			if(getStringsSorted()[i].descr == descr)
			{
				return getStringsSorted()[i].image;
			}
		}

		return NULL;
	}

	std::vector<ListPairItem> ListPairLoader::m_stringsSorted;

	std::vector<ListPairItem> ListPairLoader::m_strings;


	bool ListPairItem::operator<( const ListPairItem &rhs ) const
	{
		return descr.length() > rhs.descr.length();
	}

}

