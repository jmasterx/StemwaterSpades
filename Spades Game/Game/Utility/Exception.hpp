#ifndef CGE_EXCEPTION_HPP
#define CGE_EXCEPTION_HPP
#include <stdlib.h>
#include <string>
//used to throw exceptions
namespace cge
{
	class Exception : public std::exception
	{
		std::string msg;
	public:
		Exception(const std::string& message);
		const std::string& getMessage() const;
		virtual ~Exception(void);
	};
}

#endif
