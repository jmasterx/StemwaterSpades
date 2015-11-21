#include "Game/Engine/LanguageManager.hpp"
#include "Game/Utility/File.hpp"
namespace cge
{
	LanguageManager::~LanguageManager(void)
	{
	}

	const std::string& LanguageManager::getElement( const std::string& elementName )
	{
		if(m_strings.find(elementName) == m_strings.end())
		{
			return m_textNotFound;
		}
		else
		{
			return m_strings[elementName];
		}
	}

	void LanguageManager::mapElement( const std::string& elementName, const std::string& text )
	{
		m_strings[elementName] = text;
	}

	bool LanguageManager::_loadTexts( const std::string& path )
	{
		//open the file
		File ifs( path );

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

			std::string elemVal;
			//extract until end
			while(lineAt < (int)line.length())
			{
				//filter newline
				if(line[lineAt] == '\\' && lineAt + 1 < 
					(int)line.length() && line[lineAt + 1] == 'n')
				{
					elemVal += '\n';
					lineAt += 2;
				}
				else 
				{
					elemVal += line[lineAt];
					lineAt++;
				}
			}

			mapElement(elemName, elemVal);
		}

		return true;
	}

	LanguageManager::LanguageManager( const std::string& path )
		: m_textNotFound("Error: Text not found."),m_path(path)
	{
	}

	void LanguageManager::loadLanguage( const std::string& name )
	{
		m_strings.clear();
		_loadTexts(m_path + name + ".lang");
	}

	std::string LanguageManager::getElementWithArg( const std::string& elemName, const std::string& arg )
	{
		std::string msg;
		msg = getElement(elemName);
		msg = appendArg(msg,arg);
		return msg;
	}

	std::string LanguageManager::appendArg( const std::string& msg, const std::string& arg )
	{
		std::string retVal;
		argVec.clear();
		argVec.push_back("");
		for (int i = 0; i < arg.length(); i++)
		{
			if(arg[i] == LANG_SEP_CHAR)
			{
				argVec.push_back("");
			}
			else
			{
				argVec.back() += arg[i];
			}
		}
	
		int count = 0;
		for(size_t i = 0; i < msg.length(); ++i)
		{
			if(msg[i] == '*')
			{
				if(argVec.size() - 1 >= count)
				retVal += argVec[count];
				else
					retVal += "?";

				count++;
			}
			else
			{
				retVal += msg[i];
			}
		}

		return retVal;
	}

}
