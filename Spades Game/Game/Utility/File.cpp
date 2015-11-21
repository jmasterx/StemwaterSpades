#include "Game/Utility/File.hpp"
namespace cge
{
	File::File(void)
		: m_file(NULL)
	{
	}

	File::File( const std::string& fileName, const std::string& openMode /*= "r"*/ )
		: m_file(NULL)
	{
		open(fileName,openMode);
	}

	File::~File(void)
	{
	}

	bool File::isOpen() const
	{
		return m_file != NULL;
	}

	bool File::open( const std::string& fileName, const std::string& openMode )
	{
		if(isOpen())
		{
			close();
		}

		m_file = al_fopen(fileName.c_str(),openMode.c_str());

		return m_file != NULL;
	}

	bool File::close()
	{
		if(isOpen())
		{
			al_fclose(m_file);
			m_file = NULL;
			return true;
		}
		return false;
	}

	bool File::eof() const
	{
		if(!isOpen())
		{
			return true;
		}
		else
		{
			return al_feof(m_file);
		}
	}

	bool File::readLine( std::string& buff )
	{
		buff = "";
		if(eof() || !isOpen())
		{
			return false;
		}

		
		int c = 0;
		while((c = al_fgetc(m_file)) != EOF && c != '\n')
		{
			if(c > 13)
			buff += (char)c;
		}

		return true;
	}

	bool File::readAll( std::string& buff )
	{
		buff = "";
		if(eof() || !isOpen())
		{
			return false;
		}


		int c = 0;
		while((c = al_fgetc(m_file)) != EOF)
		{
			if(c > 13 || c == '\n')
				buff += (char)c;
		}

		return true;
	}

}
