#ifndef CGE_LOG_HPP
#define CGE_LOG_HPP
#include <fstream>
namespace cge
{
	class Log
	{
		static std::ofstream m_log;
		static std::string m_path;
		static std::string m_filename;
		static int m_maxFileSize;
		static bool m_onFirstLog;
		static void checkFilesize();
	public:
		static void create(const std::string& path, std::string fileName, int maxFileSize, bool append);
		static std::ofstream& get();
		static void write(const std::string& what);
		static void write(const std::string& where, const std::string& what);
		static std::ofstream& write();
		Log(void);
		~Log(void);
	};
}

#endif
