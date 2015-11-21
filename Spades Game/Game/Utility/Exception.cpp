#include "Game/Utility/Exception.hpp"

namespace cge 
{
	Exception::~Exception(void)
	{
	}

	Exception::Exception( const std::string& message )
		: msg(message)
	{

	}

	const std::string& Exception::getMessage() const
	{
		return msg;
	}

}

