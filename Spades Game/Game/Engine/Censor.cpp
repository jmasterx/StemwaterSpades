#include "Game/Engine/Censor.hpp"
#include "Game/Utility/StringUtil.hpp"
#include "Game/Utility/File.hpp"
#include <iostream>
namespace cge
{
	Censor::~Censor(void)
	{
	}

	Censor::Censor( const std::string& fileName )
	{
		_loadWords(fileName);
	}

	bool Censor::_loadWords( const std::string& path )
	{
		//open the file
		File ifs( path.c_str() );

		//ensure it is open
		if(!ifs.isOpen())
		{
			return false;
		}
		std::string line;

		//read each word
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
			StringUtil::toLower(line);
			if(!isContained(line))
			{
				m_words.insert(line);
			}
		
	}

		return true;

}

	bool Censor::isContained( const std::string& word ) const
	{
		return m_words.find(word) != m_words.end();
	}

	void Censor::censorString( std::string& string )
	{
		int startIndex = 0;
		m_curWord = "";
		for(size_t i = 0; i < string.size(); ++i)
		{
			if(string[i] == ' ')
			{
				m_curWord = "";

				for(size_t j = startIndex; j < i; ++j)
				{
					m_curWord += string[j];
				}

				if(isContained(m_curWord))
				{
					for(size_t j = startIndex; j < i; ++j)
					{
						string[j] = '*';
					}
				}

				startIndex = i + 1;
			}
		}

		if(startIndex < string.length())
		{
			m_curWord = "";

			for(size_t j = startIndex; j < string.length(); ++j)
			{
				m_curWord += string[j];
			}

			StringUtil::toLower(m_curWord);

			if(isContained(m_curWord))
			{
				for(size_t j = startIndex; j < string.length(); ++j)
				{
					string[j] = '*';
				}
			}
		}
	}

	void Censor::censorStingStrong( std::string& string )
	{
		for (std::set<std::string>::iterator it = m_words.begin(); it != m_words.end(); ++it )
		{
			if(it->length() > string.length())
			{
				continue;
			}

			int startPos = 0;
			int objIndex = 0;
			int numChars = 0;
			for(size_t i = 0; i < string.length(); ++i)
			{
			

				if(string[i] == ' ')
				{
					numChars = 0;
					continue;
				}

				numChars++;
				if(numChars > 1)
				{
					startPos = i;
					objIndex = 0;
					numChars = 0;
				}
				else
				{
					if(string[i] == (*it)[objIndex])
					{
						objIndex++;
					}
					else
					{
						objIndex = 0;
						startPos = i;
					}
				}

				if(objIndex == it->length())
				{
					for(int j = startPos; j < string.length() && j <= i; ++j)
					{
						string[j] = '*';
					}
				}


			}

			for(size_t i = 0; i < string.length() - it->length() + 1;++i)
			{
				if(string.substr(i,it->length()) == (*it))
				{
					for(size_t j = i; j < i + it->length() && j < string.length(); ++j)
					{
						string[j] = '*';
					}
				}
			}
		}
	}


}