#ifndef CGE_SESSION_GENERATOR_HPP
#define CGE_SESSION_GENERATOR_HPP
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <stdlib.h>
#include <sstream>

namespace cge
{
	class SessionGenerator
	{
		static boost::uuids::random_generator m_rgen;
		static std::stringstream m_ss;
	public:
		static std::string generate();
	};
}

#endif
