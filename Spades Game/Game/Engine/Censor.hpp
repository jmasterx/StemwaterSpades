#ifndef CGE_CENSOR_HPP
#define CGE_CENSOR_HPP
#include <stdlib.h>
#include <vector>
#include <string>
#include <set>
namespace cge
{
	class Censor
	{
		std::string m_curWord;
		std::set<std::string> m_words;
		bool _loadWords(const std::string& path);
	public:
		bool isContained(const std::string& word) const;
		void censorString(std::string& string);
		void censorStingStrong(std::string& string);
		Censor(const std::string& fileName);
		~Censor(void);
	};
}

#endif
