#include "Game/Utility/StringUtil.hpp"
#include <ostream>
#include <sstream>
#include <stdio.h>
#include <ctype.h>
#include <iomanip>
char easytolower(char in){
	if(in<='Z' && in>='A')
		return in-('Z'-'z');
	return in;
} 

StringUtil::StringUtil(void)
{
}

StringUtil::~StringUtil(void)
{
}

void StringUtil::toLower( std::string& s )
{
	std::transform(s.begin(), s.end(), s.begin(), easytolower);
}

std::string StringUtil::toString( int i )
{
	m_ss.clear();
	m_ss.str("");
	m_ss << i;
	return m_ss.str();
}

std::string StringUtil::toString( long long i )
{
	m_ss.clear();
	m_ss.str("");
	m_ss << i;
	return m_ss.str();
}


std::string StringUtil::toString( float i )
{
	m_ss.clear();
	m_ss.str("");
	m_ss << i;
	return m_ss.str();
}

std::string StringUtil::toString( double i )
{
	m_ss.clear();
	m_ss.str("");
	m_ss << i;
	return m_ss.str();
}

void StringUtil::replace( std::string& str, const std::string& oldStr, const std::string& newStr )
{
	size_t pos = 0;
	while((pos = str.find(oldStr, pos)) != std::string::npos)
	{
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

std::string StringUtil::commaSeparate( const std::string& s )
{
	std::string numWithCommas = s;
	int insertPosition = numWithCommas.length() - 3;
	while (insertPosition > 0) {
		numWithCommas.insert(insertPosition, ",");
		insertPosition-=3;
	}

	return numWithCommas;
}

std::string url_encode(const std::string &value) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
		std::string::value_type c = (*i);

		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << '%' << std::setw(2) << int((unsigned char) c);
	}

	return escaped.str();
}

std::string StringUtil::urlEncode( const std::string& s )
{
	return url_encode(s);
}

std::string StringUtil::IPV4FromIPV6( const char* ipv )
{
	char *p,*q;
	char ipv4[32];
	char ipv6[64];
	memset(ipv4,0,32);
	memset(ipv6,0,64);
	strcpy(ipv6,ipv);

	q = strchr (ipv6, ']');  /* search for ']' */
	if (!q)
		return "";
	for (p=q;p!=ipv6 && *p!=':';p--);  /* search for ':' backwards from ']' */
	if (p==ipv6)
		return "";
	strncpy (ipv4,p+1,q-p-1);  /* copy portion of string to ipv4 */
	ipv4[q-p-1]='\0';
	return std::string(ipv4);             /* address found and copied */
}

std::stringstream StringUtil::m_ss;
