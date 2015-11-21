#include "Game/Utility/SessionGenerator.hpp"
namespace cge
{
	std::string SessionGenerator::generate()
	{
		boost::uuids::uuid id = m_rgen();
		m_ss.clear();
		m_ss.str("");
		m_ss << id;

		return m_ss.str();
	}

	std::stringstream SessionGenerator::m_ss;

	boost::uuids::random_generator SessionGenerator::m_rgen;


}
