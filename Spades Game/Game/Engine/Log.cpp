#include "Game/Engine/Log.hpp"
#include <string>
#include "Game/Utility/TimeOfDay.hpp"
namespace cge
{
	Log::Log(void)
	{
	}

	Log::~Log(void)
	{
	}

	void Log::create( const std::string& path, std::string fileName, int maxFileSize, bool append )
	{
		m_onFirstLog = true;
		m_maxFileSize = maxFileSize;
		m_filename = fileName;
		m_path = path;

		std::string p = path + fileName;
		if(append)
			m_log.open(p.c_str(),std::ios_base::out | std::ios_base::app | std::ios_base::ate);
		else
			m_log.open(p.c_str(),std::ios_base::out | std::ios_base::trunc);

		checkFilesize();

	}

	std::ofstream& Log::get()
	{
		return m_log;
	}

	void Log::write( const std::string& what )
	{
		checkFilesize();
		get() << "[" << TimeOfDay::getDate() << "] ";
		get() << what << std::endl;
	}

	void Log::write( const std::string& where, const std::string& what )
	{
		checkFilesize();
		get() << "[" << TimeOfDay::getDate() << "] ";
		get() << "[" << where << "] " << what << std::endl;
	}

	std::ofstream& Log::write()
	{
		checkFilesize();
		get() << "[" << TimeOfDay::getDate() << "] ";
		return get();
	}

	void Log::checkFilesize()
	{
		if(m_maxFileSize > -1 && m_log.is_open())
		{
			int size = get().tellp();

			if(size > m_maxFileSize)
			{
				bool firstFull = false;
				bool secondFull = false;

				if(m_onFirstLog)
					firstFull = true;
				else
					secondFull = true;

				m_onFirstLog = !m_onFirstLog;

				std::string p = m_path + m_filename;

				if(m_onFirstLog)
				{
					m_log.close();
					m_log.open(p.c_str(),std::ios_base::out | std::ios_base::ate | std::ios_base::app);

					int fsize = get().tellp();

					if(fsize > m_maxFileSize)
						firstFull = true;
				}
				else
				{
					p += ".2";
					m_log.close();
					m_log.open(p.c_str(),std::ios_base::out | std::ios_base::ate | std::ios_base::app);

					int fsize = get().tellp();

					if(fsize > m_maxFileSize)
						secondFull = true;
				}

				if(firstFull && secondFull)
					m_log.close();
				
			}
		}
	}

	bool Log::m_onFirstLog;

	int Log::m_maxFileSize;

	std::string Log::m_filename;

	std::string Log::m_path;

	std::ofstream Log::m_log;

}
