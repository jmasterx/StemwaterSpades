#ifndef CGE_BASE64_HPP
#define CGE_BASE64_HPP
#include <stdlib.h>
#include <string>
namespace cge
{
	class Base64
	{
	public:
		static std::string encode(const std::string&);
		static std::string decode(std::string const& s);
	};
}
#endif
