#ifndef CGE_FILE_HPP
#define CGE_FILE_HPP
#include "Game/platform.hpp"
namespace cge
{
	class File
	{
		ALLEGRO_FILE* m_file;
	public:
		File(void);
		bool close();
		bool isOpen() const;
		bool readLine(std::string& buff);
		bool readAll(std::string& buff);
		bool open(const std::string& fileName, const std::string& openMode);
		File(const std::string& fileName, const std::string& openMode = "r");
		bool eof() const;
		~File(void);
	};
};

#endif