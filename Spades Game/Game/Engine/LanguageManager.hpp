#ifndef CGE_LANGUAGE_MANAGER_HPP
#define CGE_LANGUAGE_MANAGER_HPP
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#define LANG_SEP_CHAR 2
namespace cge
{
	class LanguageManager
	{
		std::map<std::string,std::string> m_strings;
		std::string m_textNotFound;
		std::string m_path;
		std::vector<std::string> argVec;
		bool _loadTexts(const std::string& path);
	public:
		void mapElement(const std::string& elementName, const std::string& text);
		const std::string& getElement(const std::string& elementName);
		std::string getElementWithArg(const std::string& elemName, const std::string& arg);
		std::string appendArg(const std::string& msg, const std::string& arg);
		void loadLanguage(const std::string& name);
		LanguageManager(const std::string& path);
		virtual ~LanguageManager(void);
	};
}

#endif
