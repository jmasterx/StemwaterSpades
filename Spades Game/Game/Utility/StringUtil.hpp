#ifndef CGE_STRING_UTIL_HPP
#define CGE_STRING_UTIL_HPP
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <sstream>
class StringUtil
{
	static std::stringstream m_ss;
public:
	StringUtil(void);
	~StringUtil(void);
	static void toLower(std::string& s);
	static std::string toString(int i);
	static std::string toString(long long i);
	static std::string toString(float i);
	static std::string toString(double i);
	static std::string urlEncode(const std::string& s);
	static std::string commaSeparate(const std::string& s);
	static void replace(std::string& str, const std::string& oldStr, const std::string& newStr);
	static std::string IPV4FromIPV6(const char* ipv);
};
#endif